#include "sdl_gpu/backend.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>

#include <SDL3/SDL.h>

#include "window.h"
#include "xerrhand.h"

namespace graphics::sdl_gpu {
namespace {

// For the texture levels no transfer buffer can hold more than.
constexpr size_t max_upload = 0xFFFFFFFFu;

// How many vertices and indices the frame buffers hold at start; a frame that needs more
// grows them. As many as the renderer's frame arrays hold at start (renderer.h).
constexpr size_t initial_vertex_bytes = 65536 * sizeof(M3D_VERTEX);
constexpr size_t initial_index_bytes = 65536 * sizeof(uint32_t);

constexpr SDL_GPUTextureFormat color_format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;

void abort_with(const char* call) {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer: %s(): %s", call, SDL_GetError());
  ErrH.Abort(call, XERR_USER, 0, SDL_GetError());
}

SDL_GPUBlendFactor blend_factor(BlendFactor factor) {
  switch (factor) {
    case BlendFactor::zero:
      return SDL_GPU_BLENDFACTOR_ZERO;
    case BlendFactor::one:
      return SDL_GPU_BLENDFACTOR_ONE;
    case BlendFactor::source_alpha:
      return SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    case BlendFactor::one_minus_source_alpha:
      return SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
  }
  return SDL_GPU_BLENDFACTOR_ONE;
}

SDL_GPUSampler* make_sampler(SDL_GPUDevice* device, SDL_GPUFilter filter, SDL_GPUSamplerAddressMode address,
                             bool compare) {
  SDL_GPUSamplerCreateInfo info{};
  info.min_filter = filter;
  info.mag_filter = filter;
  // Trilinear when a texture has mipmaps; a single-level texture samples as before.
  info.mipmap_mode =
      filter == SDL_GPU_FILTER_LINEAR ? SDL_GPU_SAMPLERMIPMAPMODE_LINEAR : SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
  info.address_mode_u = address;
  info.address_mode_v = address;
  info.address_mode_w = address;
  info.min_lod = 0.0f;
  info.max_lod = 1000.0f;  // every level there is: SDL's 0 would keep to the first
  if (compare) {
    info.enable_compare = true;
    info.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
  }
  SDL_GPUSampler* sampler = SDL_CreateGPUSampler(device, &info);
  if (sampler == nullptr) {
    abort_with("SDL_CreateGPUSampler");
  }
  return sampler;
}

SDL_GPUTexture* make_texture(SDL_GPUDevice* device, SDL_GPUTextureFormat format, SDL_GPUTextureUsageFlags usage,
                             int width, int height, int levels) {
  SDL_GPUTextureCreateInfo info{};
  info.type = SDL_GPU_TEXTURETYPE_2D;
  info.format = format;
  info.usage = usage;
  info.width = static_cast<Uint32>(width);
  info.height = static_cast<Uint32>(height);
  info.layer_count_or_depth = 1;
  info.num_levels = static_cast<Uint32>(levels);
  info.sample_count = SDL_GPU_SAMPLECOUNT_1;
  // Direct3D 12 clears a target fastest to the value it was made for, and its debug layer
  // warns at every other clear. The game only ever clears to opaque black (Fill(0,0,0),
  // Clear(0)), and the depth maps to 1; SDL's default is 0 for all. Other APIs ignore it.
  info.props = SDL_CreateProperties();
  if (usage & SDL_GPU_TEXTUREUSAGE_COLOR_TARGET) {
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_A_FLOAT, 1.0f);
  }
  if (usage & SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET) {
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_DEPTH_FLOAT, 1.0f);
  }
  SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &info);
  SDL_DestroyProperties(info.props);
  if (texture == nullptr) {
    abort_with("SDL_CreateGPUTexture");
  }
  return texture;
}

SDL_GPUBuffer* make_buffer(SDL_GPUDevice* device, SDL_GPUBufferUsageFlags usage, size_t bytes) {
  SDL_GPUBufferCreateInfo info{};
  info.usage = usage;
  info.size = static_cast<Uint32>(bytes);
  SDL_GPUBuffer* buffer = SDL_CreateGPUBuffer(device, &info);
  if (buffer == nullptr) {
    abort_with("SDL_CreateGPUBuffer");
  }
  return buffer;
}

SDL_GPUTransferBuffer* make_transfer_buffer(SDL_GPUDevice* device, size_t bytes) {
  SDL_GPUTransferBufferCreateInfo info{};
  info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
  info.size = static_cast<Uint32>(bytes);
  SDL_GPUTransferBuffer* buffer = SDL_CreateGPUTransferBuffer(device, &info);
  if (buffer == nullptr) {
    abort_with("SDL_CreateGPUTransferBuffer");
  }
  return buffer;
}

// Copies `bytes` into `buffer`, mapped; cycle: SDL gives a fresh one if it is in use.
void fill_transfer_buffer(SDL_GPUDevice* device, SDL_GPUTransferBuffer* buffer, const void* data, size_t bytes,
                          bool cycle) {
  void* mapped = SDL_MapGPUTransferBuffer(device, buffer, cycle);
  if (mapped == nullptr) {
    abort_with("SDL_MapGPUTransferBuffer");
  }
  std::memcpy(mapped, data, bytes);
  SDL_UnmapGPUTransferBuffer(device, buffer);
}

// The first depth format that can be both a target and sampled: Apple GPUs have no
// D24, others may lack D32 (SDL guarantees one of the two).
SDL_GPUTextureFormat pick_depth_format(SDL_GPUDevice* device) {
  for (const auto format : {SDL_GPU_TEXTUREFORMAT_D32_FLOAT, SDL_GPU_TEXTUREFORMAT_D24_UNORM}) {
    if (SDL_GPUTextureSupportsFormat(device, format, SDL_GPU_TEXTURETYPE_2D,
                                     SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER)) {
      return format;
    }
  }
  return SDL_GPU_TEXTUREFORMAT_D16_UNORM;
}

template <typename T>
void release(SDL_GPUDevice* device, T*& resource, void (*function)(SDL_GPUDevice*, T*)) {
  if (resource != nullptr) {
    function(device, resource);
    resource = nullptr;
  }
}

}  // namespace

std::unique_ptr<Backend> Backend::create(int width, int height, Features wanted, bool debug) {
  const Uint64 start = SDL_GetTicksNS();
  std::unique_ptr<Backend> backend(new Backend());
  if (!backend->init(width, height, wanted, debug)) {
    return nullptr;
  }
  SDL_Log("Renderer: SDL GPU device, window and shaders set up in %.1f ms", (SDL_GetTicksNS() - start) / 1e6);
  return backend;
}

bool Backend::init(int width, int height, Features wanted, bool debug) {
#ifndef NDEBUG
  debug = true;
#endif
  if (debug) {
    // SDL logs its GPU category's errors only, by default. Below that are whether the
    // API's validation layers loaded ("Validation layers enabled", "... not found") and
    // the Direct3D 12 debug layer's warnings.
    SDL_SetLogPriority(SDL_LOG_CATEGORY_GPU, SDL_LOG_PRIORITY_INFO);
  }
  // Only the one format the shaders were compiled to, so SDL picks a backend there is
  // code for.
  _device = SDL_CreateGPUDevice(shader_format, debug, nullptr);
  if (_device == nullptr) {
    SDL_Log("Renderer: no SDL GPU device (%s)", SDL_GetError());
    return false;
  }
  {
    // Which GPU and driver, for bug reports: a laptop may have two GPUs.
    const SDL_PropertiesID properties = SDL_GetGPUDeviceProperties(_device);
    const char* device = SDL_GetStringProperty(properties, SDL_PROP_GPU_DEVICE_NAME_STRING, "an unnamed GPU");
    // Direct3D 12 and Vulkan name the driver and its version; Metal names neither.
    std::string driver = SDL_GetStringProperty(properties, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "");
    const char* version = SDL_GetStringProperty(properties, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, nullptr);
    if (version == nullptr || *version == '\0') {
      version = SDL_GetStringProperty(properties, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "");
    }
    if (*version != '\0') {
      driver += driver.empty() ? version : std::string(" ") + version;
    }
    SDL_Log("Renderer: SDL GPU %s on %s%s%s%s", SDL_GetGPUDeviceDriver(_device), device,
            driver.empty() ? "" : ", driver ", driver.c_str(), debug ? ", debug mode" : "");
  }

  _window = std::make_unique<Window>(width, height);
  if (!SDL_ClaimWindowForGPUDevice(_device, _window->sdl())) {
    SDL_Log("Renderer: SDL_ClaimWindowForGPUDevice(): %s", SDL_GetError());
    return false;
  }
  // Vsync: the game's timing expects a frame per refresh. It is SDL's default, set here
  // so that it stays so; every SDL GPU backend has it.
  if (!SDL_SetGPUSwapchainParameters(_device, _window->sdl(), SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                     SDL_GPU_PRESENTMODE_VSYNC)) {
    SDL_Log("Renderer: SDL_SetGPUSwapchainParameters(): %s", SDL_GetError());
  }
  _swapchain_format = SDL_GetGPUSwapchainTextureFormat(_device, _window->sdl());
  _depth_format = pick_depth_format(_device);
  _name = std::string("SDL GPU (") + SDL_GetGPUDeviceDriver(_device) + ")";

  for (size_t i = 0; i < _shaders.size(); i++) {
    _shaders[i] = create_shader(_device, static_cast<Shader>(i));
    if (_shaders[i] == nullptr) {
      SDL_Log("Renderer: shader %u: %s", static_cast<unsigned>(i), SDL_GetError());
      return false;
    }
  }
  _features = Features{wanted.meshes, wanted.terrain, wanted.meshes && wanted.shadows};

  _clamp_sampler = make_sampler(_device, SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE, false);
  _repeat_sampler = make_sampler(_device, SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_REPEAT, false);
  // The terrain maps and the back buffer are read by texel, so filtering never applies.
  _nearest_sampler = make_sampler(_device, SDL_GPU_FILTER_NEAREST, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE, false);
  // Filtered depth comparison: each tap is already a 2 x 2 average of lit or not.
  _shadow_sampler = make_sampler(_device, SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE, true);

  // What draws bind for a texture they don't have: transparent black, and a shadow map
  // with nothing in it.
  _null_texture = make_texture(_device, color_format, SDL_GPU_TEXTUREUSAGE_SAMPLER, 1, 1, 1);
  {
    static const uint8_t black[4] = {0, 0, 0, 0};
    TextureLevels levels{};
    levels.pixels[0] = black;
    levels.sizes[0] = sizeof(black);
    levels.count = 1;
    upload_texture(_null_texture, 1, 1, levels, false);
  }
  _null_shadow_map = make_texture(_device, _depth_format,
                                  SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, 1, 1, 1);
  {
    end_copy_pass();
    SDL_GPUDepthStencilTargetInfo target{};
    target.texture = _null_shadow_map;
    target.clear_depth = 1.0f;
    target.load_op = SDL_GPU_LOADOP_CLEAR;
    target.store_op = SDL_GPU_STOREOP_STORE;
    target.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
    target.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
    SDL_EndGPURenderPass(SDL_BeginGPURenderPass(command_buffer(), nullptr, 0, &target));
  }

  _vertices.capacity = initial_vertex_bytes;
  _vertices.buffer = make_buffer(_device, SDL_GPU_BUFFERUSAGE_VERTEX, _vertices.capacity);
  _vertices.transfer = make_transfer_buffer(_device, _vertices.capacity);
  _indices.capacity = initial_index_bytes;
  _indices.buffer = make_buffer(_device, SDL_GPU_BUFFERUSAGE_INDEX, _indices.capacity);
  _indices.transfer = make_transfer_buffer(_device, _indices.capacity);

  // The window copy (present) and the back buffer, which needs the offscreen buffer's
  // formats only.
  {
    SDL_GPUColorTargetDescription target{};
    target.format = _swapchain_format;
    SDL_GPUGraphicsPipelineCreateInfo info{};
    info.vertex_shader = shader(Shader::flush_vs);
    info.fragment_shader = shader(Shader::flush_fs);
    info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    info.rasterizer_state.enable_depth_clip = true;
    info.target_info.color_target_descriptions = &target;
    info.target_info.num_color_targets = 1;
    _flush_pipeline = SDL_CreateGPUGraphicsPipeline(_device, &info);
    if (_flush_pipeline == nullptr) {
      SDL_Log("Renderer: the window copy pipeline: %s", SDL_GetError());
      return false;
    }
  }
  {
    SDL_GPUColorTargetDescription target{};
    target.format = color_format;
    SDL_GPUGraphicsPipelineCreateInfo info{};
    info.vertex_shader = shader(Shader::quad_vs);
    info.fragment_shader = shader(Shader::back_buffer_fs);
    info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
    info.rasterizer_state.enable_depth_clip = true;
    info.target_info.color_target_descriptions = &target;
    info.target_info.num_color_targets = 1;
    info.target_info.has_depth_stencil_target = true;
    info.target_info.depth_stencil_format = _depth_format;
    _back_buffer_pipeline = SDL_CreateGPUGraphicsPipeline(_device, &info);
    if (_back_buffer_pipeline == nullptr) {
      SDL_Log("Renderer: the back buffer pipeline: %s", SDL_GetError());
      return false;
    }
  }
  return true;
}

bool Backend::preparePipelines(const std::vector<PipelineKey>& keys) {
  const Uint64 start = SDL_GetTicksNS();
  for (const auto& key : keys) {
    if (scene_pipeline(key) == nullptr) {
      return false;
    }
  }
  if (_features.shadows && (shadow_pipeline(false) == nullptr || shadow_pipeline(true) == nullptr)) {
    return false;
  }
  _pipelines_prepared = true;
  SDL_Log("Renderer: %u scene pipelines%s made in %.1f ms", static_cast<unsigned>(_scene_pipelines.size()),
          _features.shadows ? " and 2 shadow pipelines" : "", (SDL_GetTicksNS() - start) / 1e6);
  return true;
}

Backend::~Backend() {
  if (_device == nullptr) {
    return;
  }
  if (_commands != nullptr) {
    end_copy_pass();
    SDL_SubmitGPUCommandBuffer(_commands);
    _commands = nullptr;
  }
  SDL_WaitForGPUIdle(_device);

  for (auto& [key, pipeline] : _scene_pipelines) {
    SDL_ReleaseGPUGraphicsPipeline(_device, pipeline);
  }
  for (auto& pipeline : _shadow_pipelines) {
    release(_device, pipeline, SDL_ReleaseGPUGraphicsPipeline);
  }
  release(_device, _back_buffer_pipeline, SDL_ReleaseGPUGraphicsPipeline);
  release(_device, _flush_pipeline, SDL_ReleaseGPUGraphicsPipeline);

  for (auto& [handle, texture] : _textures) {
    SDL_ReleaseGPUTexture(_device, texture.texture);
  }
  for (auto& [handle, mesh] : _meshes) {
    SDL_ReleaseGPUBuffer(_device, mesh.vertices);
    SDL_ReleaseGPUBuffer(_device, mesh.indices);
  }
  for (auto& map : _shadow_maps) {
    release(_device, map, SDL_ReleaseGPUTexture);
  }
  release(_device, _terrain_heights, SDL_ReleaseGPUTexture);
  release(_device, _terrain_attributes, SDL_ReleaseGPUTexture);
  for (Stream* stream : {&_vertices, &_indices}) {
    release(_device, stream->buffer, SDL_ReleaseGPUBuffer);
    release(_device, stream->transfer, SDL_ReleaseGPUTransferBuffer);
  }
  release(_device, _back_buffer_transfer, SDL_ReleaseGPUTransferBuffer);
  release(_device, _back_buffer, SDL_ReleaseGPUTexture);
  release(_device, _color, SDL_ReleaseGPUTexture);
  release(_device, _depth, SDL_ReleaseGPUTexture);
  release(_device, _null_texture, SDL_ReleaseGPUTexture);
  release(_device, _null_shadow_map, SDL_ReleaseGPUTexture);
  for (SDL_GPUSampler** sampler : {&_clamp_sampler, &_repeat_sampler, &_nearest_sampler, &_shadow_sampler}) {
    release(_device, *sampler, SDL_ReleaseGPUSampler);
  }
  for (auto& shader : _shaders) {
    release(_device, shader, SDL_ReleaseGPUShader);
  }

  if (_window != nullptr) {
    SDL_ReleaseWindowFromGPUDevice(_device, _window->sdl());
  }
  SDL_DestroyGPUDevice(_device);
  _device = nullptr;
  _window = nullptr;
}

SDL_GPUCommandBuffer* Backend::command_buffer() {
  if (_commands == nullptr) {
    _commands = SDL_AcquireGPUCommandBuffer(_device);
    if (_commands == nullptr) {
      abort_with("SDL_AcquireGPUCommandBuffer");
    }
  }
  return _commands;
}

SDL_GPUCopyPass* Backend::copy_pass() {
  assert(_pass == nullptr && "an upload inside a render pass");
  if (_copy_pass == nullptr) {
    _copy_pass = SDL_BeginGPUCopyPass(command_buffer());
    if (_copy_pass == nullptr) {
      abort_with("SDL_BeginGPUCopyPass");
    }
  }
  return _copy_pass;
}

void Backend::end_copy_pass() {
  if (_copy_pass != nullptr) {
    SDL_EndGPUCopyPass(_copy_pass);
    _copy_pass = nullptr;
  }
}

void Backend::bind_pipeline(SDL_GPUGraphicsPipeline* pipeline) {
  if (pipeline != _bound_pipeline) {
    SDL_BindGPUGraphicsPipeline(_pass, pipeline);
    _bound_pipeline = pipeline;
  }
}

RenderBackend::Scale Backend::setVideoMode(int width, int height, bool isFullScreen) {
  const auto scale = _window->setVideoMode(width, height, isFullScreen);
  if (width != _width || height != _height) {
    make_target_textures(width, height);
  }
  return scale;
}

void Backend::make_target_textures(int width, int height) {
  release(_device, _color, SDL_ReleaseGPUTexture);
  release(_device, _depth, SDL_ReleaseGPUTexture);
  release(_device, _back_buffer, SDL_ReleaseGPUTexture);
  release(_device, _back_buffer_transfer, SDL_ReleaseGPUTransferBuffer);

  _width = width;
  _height = height;
  _color = make_texture(_device, color_format, SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
                        width, height, 1);
  _depth = make_texture(_device, _depth_format, SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, width, height, 1);
  // The 2D back buffer: RGB 565 pixels, converted in back_buffer_fs. SDL GPU cannot sample
  // an integer texture (R16_UINT), so the pixels go into R8G8_UNORM as their two bytes,
  // low then high, which the shader reads back exactly.
  _back_buffer = make_texture(_device, SDL_GPU_TEXTUREFORMAT_R8G8_UNORM, SDL_GPU_TEXTUREUSAGE_SAMPLER, width,
                              height, 1);
  _back_buffer_transfer = make_transfer_buffer(_device, static_cast<size_t>(width) * height * sizeof(uint16_t));
}

void Backend::upload_texture(SDL_GPUTexture* texture, int width, int height, const TextureLevels& levels,
                             bool cycle) {
  size_t total = 0;
  for (int level = 0; level < levels.count; level++) {
    total += levels.sizes[level];
  }
  if (total == 0 || total > max_upload) {
    return;
  }

  SDL_GPUTransferBuffer* transfer = make_transfer_buffer(_device, total);
  auto* mapped = static_cast<uint8_t*>(SDL_MapGPUTransferBuffer(_device, transfer, false));
  if (mapped == nullptr) {
    abort_with("SDL_MapGPUTransferBuffer");
  }
  size_t offset = 0;
  for (int level = 0; level < levels.count; level++) {
    std::memcpy(mapped + offset, levels.pixels[level], levels.sizes[level]);
    offset += levels.sizes[level];
  }
  SDL_UnmapGPUTransferBuffer(_device, transfer);

  SDL_GPUCopyPass* pass = copy_pass();
  offset = 0;
  for (int level = 0; level < levels.count; level++) {
    SDL_GPUTextureTransferInfo source{};
    source.transfer_buffer = transfer;
    source.offset = static_cast<Uint32>(offset);
    SDL_GPUTextureRegion destination{};
    destination.texture = texture;
    destination.mip_level = static_cast<Uint32>(level);
    destination.w = static_cast<Uint32>(std::max(width >> level, 1));
    destination.h = static_cast<Uint32>(std::max(height >> level, 1));
    destination.d = 1;
    // Only the first upload may cycle: a second would leave the first level behind in
    // the copy before.
    SDL_UploadToGPUTexture(pass, &source, &destination, cycle && level == 0);
    offset += levels.sizes[level];
  }
  // Freed once the copy is done.
  SDL_ReleaseGPUTransferBuffer(_device, transfer);
}

GpuTexture Backend::createTexture(int width, int height, int levelCount, const TextureLevels* levels) {
  SDL_GPUTexture* texture = make_texture(_device, color_format, SDL_GPU_TEXTUREUSAGE_SAMPLER, width, height,
                                         levelCount);
  if (levels != nullptr) {
    upload_texture(texture, width, height, *levels, false);
  }
  const GpuTexture handle = ++_last_texture;
  _textures.emplace(handle, Texture{texture, width, height});
  return handle;
}

void Backend::updateTexture(GpuTexture texture, const TextureLevels& levels) {
  const auto entry = _textures.find(texture);
  if (entry != _textures.end()) {
    upload_texture(entry->second.texture, entry->second.width, entry->second.height, levels, true);
  }
}

void Backend::destroyTexture(GpuTexture texture) {
  const auto entry = _textures.find(texture);
  if (entry != _textures.end()) {
    SDL_ReleaseGPUTexture(_device, entry->second.texture);
    _textures.erase(entry);
  }
}

SDL_GPUTexture* Backend::texture_or_null(GpuTexture texture) const {
  const auto entry = _textures.find(texture);
  return entry != _textures.end() ? entry->second.texture : _null_texture;
}

GpuMesh Backend::createMesh(const void* vertices, size_t vertexBytes, const void* indices, size_t indexBytes) {
  if (vertexBytes == 0 || indexBytes == 0 || vertexBytes + indexBytes > max_upload) {
    return 0;
  }
  Mesh mesh{};
  mesh.vertices = make_buffer(_device, SDL_GPU_BUFFERUSAGE_VERTEX, vertexBytes);
  mesh.indices = make_buffer(_device, SDL_GPU_BUFFERUSAGE_INDEX, indexBytes);

  // One transfer buffer for both: the vertices, then the indices.
  SDL_GPUTransferBuffer* transfer = make_transfer_buffer(_device, vertexBytes + indexBytes);
  auto* mapped = static_cast<uint8_t*>(SDL_MapGPUTransferBuffer(_device, transfer, false));
  if (mapped == nullptr) {
    abort_with("SDL_MapGPUTransferBuffer");
  }
  std::memcpy(mapped, vertices, vertexBytes);
  std::memcpy(mapped + vertexBytes, indices, indexBytes);
  SDL_UnmapGPUTransferBuffer(_device, transfer);

  SDL_GPUCopyPass* pass = copy_pass();
  SDL_GPUTransferBufferLocation source{transfer, 0};
  SDL_GPUBufferRegion destination{mesh.vertices, 0, static_cast<Uint32>(vertexBytes)};
  SDL_UploadToGPUBuffer(pass, &source, &destination, false);
  source.offset = static_cast<Uint32>(vertexBytes);
  destination = SDL_GPUBufferRegion{mesh.indices, 0, static_cast<Uint32>(indexBytes)};
  SDL_UploadToGPUBuffer(pass, &source, &destination, false);
  SDL_ReleaseGPUTransferBuffer(_device, transfer);

  const GpuMesh handle = ++_last_mesh;
  _meshes.emplace(handle, mesh);
  return handle;
}

void Backend::destroyMesh(GpuMesh mesh) {
  const auto entry = _meshes.find(mesh);
  if (entry != _meshes.end()) {
    SDL_ReleaseGPUBuffer(_device, entry->second.vertices);
    SDL_ReleaseGPUBuffer(_device, entry->second.indices);
    _meshes.erase(entry);
  }
}

void Backend::uploadTerrain(int width, int height, const uint8_t* heights, const uint8_t* attributes) {
  // A map of another size (another world): new textures.
  if (_terrain_width != width || _terrain_height != height) {
    release(_device, _terrain_heights, SDL_ReleaseGPUTexture);
    release(_device, _terrain_attributes, SDL_ReleaseGPUTexture);
    _terrain_heights = make_texture(_device, SDL_GPU_TEXTUREFORMAT_R8_UNORM, SDL_GPU_TEXTUREUSAGE_SAMPLER, width,
                                    height, 1);
    _terrain_attributes = make_texture(_device, SDL_GPU_TEXTUREFORMAT_R8_UNORM, SDL_GPU_TEXTUREUSAGE_SAMPLER,
                                       width, height, 1);
    _terrain_width = width;
    _terrain_height = height;
  }

  const size_t size = static_cast<size_t>(width) * height;
  for (const auto& [texture, pixels] : {std::pair{_terrain_heights, heights}, std::pair{_terrain_attributes, attributes}}) {
    TextureLevels levels{};
    levels.pixels[0] = pixels;
    levels.sizes[0] = size;
    levels.count = 1;
    upload_texture(texture, width, height, levels, true);
  }
}

void Backend::uploadBackBuffer(const uint16_t* pixels) {
  const size_t bytes = static_cast<size_t>(_width) * _height * sizeof(uint16_t);
  fill_transfer_buffer(_device, _back_buffer_transfer, pixels, bytes, true);

  SDL_GPUTextureTransferInfo source{};
  source.transfer_buffer = _back_buffer_transfer;
  SDL_GPUTextureRegion destination{};
  destination.texture = _back_buffer;
  destination.w = static_cast<Uint32>(_width);
  destination.h = static_cast<Uint32>(_height);
  destination.d = 1;
  SDL_UploadToGPUTexture(copy_pass(), &source, &destination, true);
}

void Backend::upload_stream(Stream& stream, SDL_GPUBufferUsageFlags usage, const void* data, size_t bytes,
                            const char* name) {
  if (bytes == 0) {
    return;
  }
  if (bytes > stream.capacity) {
    // Released once the draws recorded with the old ones are done.
    stream.capacity = std::max(bytes, 2 * stream.capacity);
    release(_device, stream.buffer, SDL_ReleaseGPUBuffer);
    release(_device, stream.transfer, SDL_ReleaseGPUTransferBuffer);
    stream.buffer = make_buffer(_device, usage, stream.capacity);
    stream.transfer = make_transfer_buffer(_device, stream.capacity);
    SDL_Log("Renderer: %s buffer grown to %u KiB", name, static_cast<unsigned>(stream.capacity / 1024));
  }
  fill_transfer_buffer(_device, stream.transfer, data, bytes, true);
  const SDL_GPUTransferBufferLocation source{stream.transfer, 0};
  const SDL_GPUBufferRegion destination{stream.buffer, 0, static_cast<Uint32>(bytes)};
  SDL_UploadToGPUBuffer(copy_pass(), &source, &destination, true);
}

void Backend::uploadFrame(const M3D_VERTEX* vertices, size_t vertexCount, const uint32_t* indices,
                          size_t indexCount) {
  upload_stream(_vertices, SDL_GPU_BUFFERUSAGE_VERTEX, vertices, vertexCount * sizeof(M3D_VERTEX), "vertex");
  upload_stream(_indices, SDL_GPU_BUFFERUSAGE_INDEX, indices, indexCount * sizeof(uint32_t), "index");
}

void Backend::beginShadowPass(int camera) {
  SDL_GPUTexture*& map = _shadow_maps[camera];
  if (map == nullptr) {
    map = make_texture(_device, _depth_format,
                       SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER, shadow_map_size,
                       shadow_map_size, 1);
    SDL_Log("Renderer: shadow map %d (%dx%d)", camera, shadow_map_size, shadow_map_size);
  }

  end_copy_pass();
  SDL_GPUDepthStencilTargetInfo target{};
  target.texture = map;
  target.clear_depth = 1.0f;
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;
  target.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
  target.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
  target.cycle = true;  // all of it is drawn again
  _pass = SDL_BeginGPURenderPass(command_buffer(), nullptr, 0, &target);
  if (_pass == nullptr) {
    abort_with("SDL_BeginGPURenderPass");
  }
  _bound_pipeline = nullptr;
}

void Backend::drawShadow(const ShadowDraw& draw) {
  const auto mesh = _meshes.find(draw.mesh);
  if (mesh == _meshes.end()) {
    return;
  }

  SDL_GPUGraphicsPipeline* pipeline = shadow_pipeline(draw.animated);
  if (pipeline == nullptr) {
    return;
  }
  bind_pipeline(pipeline);
  const SDL_GPUBufferBinding vertices =
      draw.animated ? SDL_GPUBufferBinding{_vertices.buffer, static_cast<Uint32>(draw.pose_offset * sizeof(M3D_VERTEX))}
                    : SDL_GPUBufferBinding{mesh->second.vertices, 0};
  SDL_BindGPUVertexBuffers(_pass, 0, &vertices, 1);
  const SDL_GPUBufferBinding indices{mesh->second.indices, 0};
  SDL_BindGPUIndexBuffer(_pass, &indices, SDL_GPU_INDEXELEMENTSIZE_16BIT);
  const SDL_GPUTextureSamplerBinding texture{texture_or_null(draw.texture),
                                             draw.repeat ? _repeat_sampler : _clamp_sampler};
  SDL_BindGPUFragmentSamplers(_pass, 0, &texture, 1);
  SDL_PushGPUVertexUniformData(_commands, 0, &draw.vs, sizeof(draw.vs));
  SDL_PushGPUFragmentUniformData(_commands, 0, &draw.fs, sizeof(draw.fs));
  SDL_DrawGPUIndexedPrimitives(_pass, draw.index_count, 1, 0, 0, 0);
}

void Backend::beginScenePass(bool clear, const float (&clearColor)[4]) {
  end_copy_pass();
  SDL_GPUColorTargetInfo color{};
  color.texture = _color;
  color.clear_color = SDL_FColor{clearColor[0], clearColor[1], clearColor[2], clearColor[3]};
  // A second scene in a frame draws over the first.
  color.load_op = clear ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
  color.store_op = SDL_GPU_STOREOP_STORE;
  color.cycle = clear;
  SDL_GPUDepthStencilTargetInfo depth{};
  depth.texture = _depth;
  depth.clear_depth = 1.0f;
  depth.load_op = SDL_GPU_LOADOP_CLEAR;
  depth.store_op = SDL_GPU_STOREOP_DONT_CARE;
  depth.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
  depth.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
  depth.cycle = true;
  _pass = SDL_BeginGPURenderPass(command_buffer(), &color, 1, &depth);
  if (_pass == nullptr) {
    abort_with("SDL_BeginGPURenderPass");
  }
  _bound_pipeline = nullptr;
}

void Backend::drawBackBuffer() {
  bind_pipeline(_back_buffer_pipeline);
  const SDL_GPUViewport viewport{0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height), 0.0f, 1.0f};
  SDL_SetGPUViewport(_pass, &viewport);
  const SDL_GPUTextureSamplerBinding texture{_back_buffer, _nearest_sampler};
  SDL_BindGPUFragmentSamplers(_pass, 0, &texture, 1);
  SDL_DrawGPUPrimitives(_pass, 3, 1, 0, 0);
}

void Backend::draw(const SceneDraw& draw) {
  const Geometry geometry = draw.pipeline.geometry;
  SDL_GPUBufferBinding vertices[2]{};
  Uint32 vertex_buffers = 1;
  SDL_GPUBufferBinding indices{};
  SDL_GPUIndexElementSize index_size = SDL_GPU_INDEXELEMENTSIZE_32BIT;
  const void* vs_params = &draw.vs.scene;
  Uint32 vs_size = sizeof(draw.vs.scene);

  if (geometry == Geometry::frame) {
    vertices[0] = {_vertices.buffer, 0};
    indices = {_indices.buffer, 0};
  } else {
    const auto mesh = _meshes.find(draw.mesh);
    if (mesh == _meshes.end()) {
      return;
    }
    indices = {mesh->second.indices, 0};
    if (geometry == Geometry::tile) {
      vertices[0] = {mesh->second.vertices, 0};
      vs_params = &draw.vs.tile;
      vs_size = sizeof(draw.vs.tile);
    } else {
      index_size = SDL_GPU_INDEXELEMENTSIZE_16BIT;
      if (geometry == Geometry::animated_mesh) {
        // Positions and texture coordinates from this frame's pose, normals from the mesh.
        vertices[0] = {_vertices.buffer, static_cast<Uint32>(draw.pose_offset * sizeof(M3D_VERTEX))};
        vertices[1] = {mesh->second.vertices, 0};
        vertex_buffers = 2;
      } else {
        vertices[0] = {mesh->second.vertices, 0};
      }
      vs_params = &draw.vs.mesh;
      vs_size = sizeof(draw.vs.mesh);
    }
  }

  SDL_GPUGraphicsPipeline* pipeline = scene_pipeline(draw.pipeline);
  if (pipeline == nullptr) {
    return;
  }
  bind_pipeline(pipeline);
  const SDL_GPUViewport viewport{static_cast<float>(draw.viewport.x),     static_cast<float>(draw.viewport.y),
                                 static_cast<float>(draw.viewport.width), static_cast<float>(draw.viewport.height),
                                 0.0f,                                    1.0f};
  SDL_SetGPUViewport(_pass, &viewport);
  SDL_BindGPUVertexBuffers(_pass, 0, vertices, vertex_buffers);
  SDL_BindGPUIndexBuffer(_pass, &indices, index_size);

  if (geometry == Geometry::mesh || geometry == Geometry::animated_mesh) {
    const bool terrain = draw.terrain_maps && _terrain_heights != nullptr;
    const SDL_GPUTextureSamplerBinding maps[2] = {
        {terrain ? _terrain_heights : _null_texture, _nearest_sampler},
        {terrain ? _terrain_attributes : _null_texture, _nearest_sampler},
    };
    SDL_BindGPUVertexSamplers(_pass, 0, maps, 2);
  }

  SDL_GPUSampler* sampler = draw.repeat ? _repeat_sampler : _clamp_sampler;
  const int camera = draw.shadow_camera;
  const SDL_GPUTextureSamplerBinding textures[4] = {
      {texture_or_null(draw.textures[0]), sampler},
      {texture_or_null(draw.textures[1]), sampler},
      {camera >= 0 && _shadow_maps[camera] != nullptr ? _shadow_maps[camera] : _null_shadow_map, _shadow_sampler},
      {camera >= 0 && _terrain_attributes != nullptr ? _terrain_attributes : _null_texture, _nearest_sampler},
  };
  SDL_BindGPUFragmentSamplers(_pass, 0, textures, 4);

  SDL_PushGPUVertexUniformData(_commands, 0, vs_params, vs_size);
  SDL_PushGPUFragmentUniformData(_commands, 0, &draw.fs, sizeof(draw.fs));
  SDL_PushGPUFragmentUniformData(_commands, 1, &draw.shadow, sizeof(draw.shadow));
  SDL_DrawGPUIndexedPrimitives(_pass, draw.index_count, 1, draw.first_index, 0, 0);
}

void Backend::endPass() {
  SDL_EndGPURenderPass(_pass);
  _pass = nullptr;
  _bound_pipeline = nullptr;
}

void Backend::present(const FlushFsParams& flush) {
  end_copy_pass();
  SDL_GPUCommandBuffer* commands = command_buffer();

  // Waits for the window to take a frame (vsync). None while it is minimised: the frame
  // is still submitted, so its uploads happen.
  SDL_GPUTexture* swapchain = nullptr;
  Uint32 width = 0;
  Uint32 height = 0;
  if (!SDL_WaitAndAcquireGPUSwapchainTexture(commands, _window->sdl(), &swapchain, &width, &height)) {
    SDL_Log("Renderer: SDL_WaitAndAcquireGPUSwapchainTexture(): %s", SDL_GetError());
  }
  if (swapchain != nullptr && width > 0 && height > 0) {
    SDL_GPUColorTargetInfo target{};
    target.texture = swapchain;
    target.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
    target.load_op = SDL_GPU_LOADOP_CLEAR;
    target.store_op = SDL_GPU_STOREOP_STORE;
    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(commands, &target, 1, nullptr);
    if (pass == nullptr) {
      abort_with("SDL_BeginGPURenderPass");
    }
    SDL_BindGPUGraphicsPipeline(pass, _flush_pipeline);
    // The game's screen at its aspect ratio, as wide as the window's height lets it be.
    const float params[4] = {1.0f * _width / _height * height / width, 0.0f, 0.0f, 0.0f};
    SDL_PushGPUVertexUniformData(commands, 0, params, sizeof(params));
    SDL_PushGPUFragmentUniformData(commands, 0, &flush, sizeof(flush));
    const SDL_GPUTextureSamplerBinding texture{_color, _clamp_sampler};
    SDL_BindGPUFragmentSamplers(pass, 0, &texture, 1);
    SDL_DrawGPUPrimitives(pass, 6, 1, 0, 0);
    SDL_EndGPURenderPass(pass);
  }

  if (!SDL_SubmitGPUCommandBuffer(commands)) {
    SDL_Log("Renderer: SDL_SubmitGPUCommandBuffer(): %s", SDL_GetError());
  }
  _commands = nullptr;
}

SDL_GPUGraphicsPipeline* Backend::shadow_pipeline(bool animated) {
  SDL_GPUGraphicsPipeline*& pipeline = _shadow_pipelines[animated ? 1 : 0];
  if (pipeline != nullptr || _shadow_pipeline_failed[animated ? 1 : 0]) {
    return pipeline;
  }

  // This frame's pose in the frame vertex array, or the mesh's own vertices.
  const SDL_GPUVertexBufferDescription buffer{
      0, static_cast<Uint32>(animated ? sizeof(M3D_VERTEX) : sizeof(M3D_MESH_VERTEX)),
      SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
  const SDL_GPUVertexAttribute attributes[2] = {
      {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
       static_cast<Uint32>(animated ? offsetof(M3D_VERTEX, x) : offsetof(M3D_MESH_VERTEX, x))},
      {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
       static_cast<Uint32>(animated ? offsetof(M3D_VERTEX, u) : offsetof(M3D_MESH_VERTEX, u))},
  };

  // Depth only. No culling: the CPU raster draws both sides of every polygon.
  SDL_GPUGraphicsPipelineCreateInfo info{};
  info.vertex_shader = shader(Shader::shadow_vs);
  info.fragment_shader = shader(Shader::shadow_fs);
  info.vertex_input_state.vertex_buffer_descriptions = &buffer;
  info.vertex_input_state.num_vertex_buffers = 1;
  info.vertex_input_state.vertex_attributes = attributes;
  info.vertex_input_state.num_vertex_attributes = 2;
  info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
  info.rasterizer_state.enable_depth_clip = true;
  info.depth_stencil_state.enable_depth_test = true;
  info.depth_stencil_state.enable_depth_write = true;
  info.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
  info.target_info.has_depth_stencil_target = true;
  info.target_info.depth_stencil_format = _depth_format;

  pipeline = SDL_CreateGPUGraphicsPipeline(_device, &info);
  if (pipeline == nullptr) {
    _shadow_pipeline_failed[animated ? 1 : 0] = true;
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer: %s shadow pipeline: %s", animated ? "animated" : "static",
                 SDL_GetError());
  } else if (_pipelines_prepared) {
    SDL_Log("Renderer: %s shadow pipeline made at its first draw", animated ? "animated" : "static");
  }
  return pipeline;
}

SDL_GPUGraphicsPipeline* Backend::scene_pipeline(const PipelineKey& key) {
  for (const auto& entry : _scene_pipelines) {
    if (entry.first == key) {
      return entry.second;
    }
  }

  // The vertex layouts: locations as the shaders' TEXCOORD<n> inputs.
  SDL_GPUVertexBufferDescription buffers[2]{};
  SDL_GPUVertexAttribute attributes[4]{};
  Uint32 buffer_count = 1;
  Uint32 attribute_count = 3;
  Shader vertex_shader = Shader::mesh_vs;
  const auto offset = [](size_t value) { return static_cast<Uint32>(value); };
  switch (key.geometry) {
    case Geometry::frame:
      // One interleaved M3D_VERTEX per vertex; the colour bytes arrive as 0..1.
      vertex_shader = Shader::scene_vs;
      buffers[0] = {0, sizeof(M3D_VERTEX), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
      attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offset(offsetof(M3D_VERTEX, x))};
      attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, offset(offsetof(M3D_VERTEX, diffuse))};
      attributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, offset(offsetof(M3D_VERTEX, specular))};
      attributes[3] = {3, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offset(offsetof(M3D_VERTEX, u))};
      attribute_count = 4;
      break;
    case Geometry::mesh:
      // One M3D_MESH_VERTEX per vertex; the normal bytes arrive as integers.
      buffers[0] = {0, sizeof(M3D_MESH_VERTEX), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
      attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offset(offsetof(M3D_MESH_VERTEX, x))};
      attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_BYTE4, offset(offsetof(M3D_MESH_VERTEX, normal))};
      attributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offset(offsetof(M3D_MESH_VERTEX, u))};
      break;
    case Geometry::animated_mesh:
      // Buffer 0: this frame's pose in the frame vertex array (position and texture
      // coordinates of M3D_VERTEX). Buffer 1: the mesh's own vertices, for the normal.
      buffers[0] = {0, sizeof(M3D_VERTEX), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
      buffers[1] = {1, sizeof(M3D_MESH_VERTEX), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
      buffer_count = 2;
      attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offset(offsetof(M3D_VERTEX, x))};
      attributes[1] = {1, 1, SDL_GPU_VERTEXELEMENTFORMAT_BYTE4, offset(offsetof(M3D_MESH_VERTEX, normal))};
      attributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offset(offsetof(M3D_VERTEX, u))};
      break;
    case Geometry::tile:
      // One M3D_TILE_VERTEX per vertex; the colour bytes arrive as 0..1.
      vertex_shader = Shader::tile_vs;
      buffers[0] = {0, sizeof(M3D_TILE_VERTEX), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0};
      attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offset(offsetof(M3D_TILE_VERTEX, x))};
      attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, offset(offsetof(M3D_TILE_VERTEX, color))};
      attributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offset(offsetof(M3D_TILE_VERTEX, u))};
      break;
  }

  SDL_GPUColorTargetDescription target{};
  target.format = color_format;
  target.blend_state.enable_blend = key.blend;
  target.blend_state.src_color_blendfactor = blend_factor(key.source);
  target.blend_state.src_alpha_blendfactor = blend_factor(key.source);
  target.blend_state.dst_color_blendfactor = blend_factor(key.destination);
  target.blend_state.dst_alpha_blendfactor = blend_factor(key.destination);
  target.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
  target.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;

  SDL_GPUGraphicsPipelineCreateInfo info{};
  info.vertex_shader = shader(vertex_shader);
  info.fragment_shader = shader(Shader::scene_fs);
  info.vertex_input_state.vertex_buffer_descriptions = buffers;
  info.vertex_input_state.num_vertex_buffers = buffer_count;
  info.vertex_input_state.vertex_attributes = attributes;
  info.vertex_input_state.num_vertex_attributes = attribute_count;
  info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  // The 2000 renderer culled on the CPU, keeping triangles that wind one way on the screen
  // (AddPolygonFixTestPointFix, master). A mirror turns them the other way, so it sent a
  // reflection's corners in the opposite order; a mirrored GPU mesh flips the front face.
  info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
  info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;
  info.rasterizer_state.front_face =
      key.mirrored ? SDL_GPU_FRONTFACE_CLOCKWISE : SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;
  info.rasterizer_state.enable_depth_clip = true;  // clipped at near and far; SDL's default clamps
  // Tested always, for its writes: with the test off, Direct3D 12 writes no depth.
  info.depth_stencil_state.enable_depth_test = true;
  info.depth_stencil_state.enable_depth_write = key.depth_write;
  info.depth_stencil_state.compare_op = key.depth_test ? SDL_GPU_COMPAREOP_LESS_OR_EQUAL : SDL_GPU_COMPAREOP_ALWAYS;
  info.target_info.color_target_descriptions = &target;
  info.target_info.num_color_targets = 1;
  info.target_info.has_depth_stencil_target = true;
  info.target_info.depth_stencil_format = _depth_format;

  // Kept even if refused, so that it is tried and logged once.
  SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(_device, &info);
  _scene_pipelines.emplace_back(key, pipeline);
  const char* kind = key.geometry == Geometry::frame  ? "scene"
                     : key.geometry == Geometry::mesh ? "mesh"
                     : key.geometry == Geometry::tile ? "tile"
                                                      : "animated mesh";
  if (pipeline == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Renderer: %s pipeline (depth test %d, depth write %d, blend %d, factors %s/%s%s): %s", kind,
                 key.depth_test ? 1 : 0, key.depth_write ? 1 : 0, key.blend ? 1 : 0,
                 blend_factor_name(key.source), blend_factor_name(key.destination),
                 key.mirrored ? ", mirrored" : "", SDL_GetError());
  } else if (_pipelines_prepared) {
    // One preparePipelines wasn't given: Renderer::pipeline_keys() should list it.
    SDL_Log("Renderer: %s pipeline made at its first draw (depth test %d, depth write %d, blend %d, factors %s/%s%s)",
            kind, key.depth_test ? 1 : 0, key.depth_write ? 1 : 0, key.blend ? 1 : 0,
            blend_factor_name(key.source), blend_factor_name(key.destination), key.mirrored ? ", mirrored" : "");
  }
  return pipeline;
}

}  // namespace graphics::sdl_gpu
