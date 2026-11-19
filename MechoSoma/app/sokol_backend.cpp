#include "sokol_backend.h"

#include <algorithm>
#include <cstddef>

#include <SDL3/SDL.h>

#include "back_buffer.h"
#include "display.h"
#include "offscreen_buffer.h"
#include "texture_manager.h"
#include "xerrhand.h"

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-scene-shader.h"
#endif

using namespace graphics;

#ifndef EMSCRIPTEN
// The renderer fills the shared uniform blocks; sokol-shdc's must be the same bytes.
static_assert(sizeof(SceneVsParams) == sizeof(scene_vs_params_t));
static_assert(sizeof(MeshVsParams) == sizeof(mesh_vs_params_t));
static_assert(sizeof(TileVsParams) == sizeof(tile_vs_params_t));
static_assert(sizeof(SceneShadowParams) == sizeof(scene_shadow_params_t));
static_assert(sizeof(ShadowVsParams) == sizeof(shadow_vs_params_t));
static_assert(sizeof(ShadowFsParams) == sizeof(shadow_fs_params_t));
#endif
static_assert(sizeof(SceneFsParams) == sizeof(scene_fs_params_t));

namespace {
  // How many vertices and indices the frame buffers hold at start; a frame that needs
  // more grows them. The largest frames seen: about 31k vertices in the menu world with
  // the GPU paths on (its water reflections are built on the CPU), and 40k vertices and
  // 51k indices with GpuMeshes=0.
  constexpr size_t initial_vertex_count = 65536;
  constexpr size_t initial_index_count = 65536;

  sg_buffer make_frame_buffer(size_t bytes, bool is_index_buffer) {
    sg_buffer_desc description{};
    description.size = bytes;
    if (is_index_buffer) {
      description.usage.index_buffer = true;
    } else {
      description.usage.vertex_buffer = true;
    }
    description.usage.dynamic_update = true;
    const auto buffer = sg_make_buffer(&description);
    if (sg_query_buffer_state(buffer) != SG_RESOURCESTATE_VALID) {
      ErrH.Abort("sg_make_buffer", XERR_USER, 0, "");
    }
    return buffer;
  }

  // Uploads `bytes` of `data`, first recreating `buffer` larger if it is too small.
  void upload_frame_buffer(sg_buffer& buffer, size_t& capacity, const void* data, size_t bytes,
                           bool is_index_buffer, const char* name) {
    if (bytes == 0) {
      return;  // sokol does not accept an empty update
    }
    if (bytes > capacity) {
      capacity = std::max(bytes, 2 * capacity);
      sg_destroy_buffer(buffer);
      buffer = make_frame_buffer(capacity, is_index_buffer);
      SDL_Log("Renderer: %s buffer grown to %u KiB", name, static_cast<unsigned>(capacity / 1024));
    }
    sg_update_buffer(buffer, sg_range{data, bytes});
  }

  sg_image_data image_data(const TextureLevels& levels) {
    sg_image_data data{};
    for (int level = 0; level < levels.count && level < SG_MAX_MIPMAPS; level++) {
      data.mip_levels[level] = {levels.pixels[level], levels.sizes[level]};
    }
    return data;
  }

  sg_blend_factor blend_factor(BlendFactor factor) {
    switch (factor) {
      case BlendFactor::zero:
        return SG_BLENDFACTOR_ZERO;
      case BlendFactor::one:
        return SG_BLENDFACTOR_ONE;
      case BlendFactor::source_alpha:
        return SG_BLENDFACTOR_SRC_ALPHA;
      case BlendFactor::one_minus_source_alpha:
        return SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    }
    return SG_BLENDFACTOR_ONE;
  }

#ifndef EMSCRIPTEN
  // A dynamic single-channel image for one terrain map, and its texture view.
  void make_terrain_map(int width, int height, sg_image& image, sg_view& view) {
    sg_image_desc description{};
    description.width = width;
    description.height = height;
    description.num_slices = 1;
    description.num_mipmaps = 1;
    description.pixel_format = SG_PIXELFORMAT_R8;
    description.usage.dynamic_update = true;
    image = sg_make_image(&description);

    sg_view_desc view_description{};
    view_description.texture.image = image;
    view = sg_make_view(&view_description);
  }
#endif
}  // namespace

SokolBackend::SokolBackend(int width, int height, Features wanted) {
#ifdef EMSCRIPTEN
  // The web build's shader header (sokol-shader-em.h) has no mesh, tile or shadow program.
  wanted = Features{};
#endif
  _features = Features{wanted.meshes, wanted.terrain, wanted.meshes && wanted.shadows};
  _display = std::make_unique<Display>(width, height);

  {
    sg_desc description{};
    // The frame buffers and the back buffer's, plus two for each GPU mesh or tile.
    description.buffer_pool_size = 9 + 2 * max_meshes;
    // The textures, the two terrain maps of the mesh shadow test, and the shadow maps
    // (each with an attachment and a texture view) with the stand-in for draws without.
    description.image_pool_size = TextureManager::max_textures_count + 2 + max_shadow_cameras + 1;
    description.view_pool_size = TextureManager::max_textures_count + 2 + 2 * max_shadow_cameras + 1;
    description.shader_pool_size = 8;
    // Pipelines are created once and kept: the scene's (scene_pipeline(), a few in
    // practice and at most 4 geometries x 3 depth modes x 5 blends = 60), the two shadow
    // pipelines, and one each for the back buffer and the window copy.
    description.pipeline_pool_size = 96;
    description.environment = _display->environment();

    sg_setup(&description);
    if (!sg_isvalid()) {
      ErrH.Abort("sg_setup", XERR_USER, 0, "");
    }
  }

  _sceneShader = sg_make_shader(scene_shader_desc(sg_query_backend()));
  if (_sceneShader.id == SG_INVALID_ID) {
    ErrH.Abort("sg_make_shader", XERR_USER, 0, "");
  }

#ifndef EMSCRIPTEN
  if (_features.meshes) {
    _meshShader = sg_make_shader(mesh_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_meshShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the mesh shader failed, meshes are drawn on the CPU");
      _features.meshes = false;
      _features.shadows = false;
    }
  }
  if (_features.terrain) {
    _tileShader = sg_make_shader(tile_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_tileShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the tile shader failed, terrain is drawn on the CPU");
      _features.terrain = false;
    }
  }
  if (_features.shadows) {
    _shadowShader = sg_make_shader(shadow_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_shadowShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the shadow shader failed, shadows are made on the CPU");
      _features.shadows = false;
    }
  }

  // Every scene_fs draw binds the terrain attribute map and a shadow map, real or not.
  {
    // The terrain maps are read with texelFetch, so filtering never applies.
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_NEAREST;
    description.mag_filter = SG_FILTER_NEAREST;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    _terrain_sampler = sg_make_sampler(description);
  }
  {
    // Filtered depth comparison: each tap is already a 2 x 2 average of lit or not.
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    description.compare = SG_COMPAREFUNC_LESS_EQUAL;
    _shadow_sampler = sg_make_sampler(description);
  }
  {
    sg_image_desc description{};
    description.usage.depth_stencil_attachment = true;
    description.width = 1;
    description.height = 1;
    description.pixel_format = SG_PIXELFORMAT_DEPTH;
    _null_shadow_map = sg_make_image(&description);
    sg_view_desc view_description{};
    view_description.texture.image = _null_shadow_map;
    _null_shadow_map_view = sg_make_view(&view_description);
    if (sg_query_view_state(_null_shadow_map_view) != SG_RESOURCESTATE_VALID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
    }
  }
#endif

  {
    static const uint8_t black[4] = {0, 0, 0, 0};
    sg_image_desc description{};
    description.width = 1;
    description.height = 1;
    description.num_slices = 1;
    description.num_mipmaps = 1;
    description.usage.immutable = true;
    description.pixel_format = SG_PIXELFORMAT_RGBA8;
    description.sample_count = 1;
    description.data.mip_levels[0] = SG_RANGE(black);
    _nullTexture = sg_make_image(description);
    if (_nullTexture.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_image", XERR_USER, 0, "");
    }
  }
  {
    sg_view_desc description{};
    description.texture.image = _nullTexture;
    _nullTextureView = sg_make_view(&description);
    if (_nullTextureView.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "");
    }
  }

  _vertex_capacity = initial_vertex_count * sizeof(M3D_VERTEX);
  _index_capacity = initial_index_count * sizeof(uint32_t);
  _vertex_buffer = make_frame_buffer(_vertex_capacity, false);
  _index_buffer = make_frame_buffer(_index_capacity, true);

  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    // Trilinear when the texture has mipmaps; a single-level texture samples as before.
    description.mipmap_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    _clamp_sampler = sg_make_sampler(description);
  }
  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    description.mipmap_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_REPEAT;
    description.wrap_v = SG_WRAP_REPEAT;
    _repeat_sampler = sg_make_sampler(description);
  }
}

SokolBackend::~SokolBackend() {
  _backBuffer = nullptr;
  _offscreenBuffer = nullptr;
  sg_shutdown();
  _display = nullptr;
}

RenderBackend::Scale SokolBackend::setVideoMode(int width, int height, bool isFullScreen) {
  const auto scale = _display->window().setVideoMode(width, height, isFullScreen);

  _offscreenBuffer = nullptr;
  _offscreenBuffer = std::make_unique<OffscreenBuffer>(width, height, _display->window().drawableWidth(),
                                                       _display->window().drawableHeight());
  _backBuffer = nullptr;
  _backBuffer = std::make_unique<BackBuffer>(width, height);
  return scale;
}

GpuTexture SokolBackend::createTexture(int width, int height, int levelCount, const TextureLevels* levels) {
  sg_image_desc description{};
  description.width = width;
  description.height = height;
  description.num_slices = 1;
  description.num_mipmaps = levelCount;
  description.pixel_format = SG_PIXELFORMAT_RGBA8;
  description.sample_count = 1;
  if (levels != nullptr) {
    description.data = image_data(*levels);  // immutable, filled at creation
  } else {
    description.usage.dynamic_update = true;
  }
  const auto image = sg_make_image(description);
  if (sg_query_image_state(image) != SG_RESOURCESTATE_VALID) {
    ErrH.Abort("sg_make_image", XERR_USER, 0, "");
  }

  sg_view_desc view_description{};
  view_description.texture.image = image;
  const auto view = sg_make_view(&view_description);
  if (sg_query_view_state(view) != SG_RESOURCESTATE_VALID) {
    ErrH.Abort("sg_make_view", XERR_USER, 0, "");
  }

  const GpuTexture texture = ++_last_texture;
  _textures.emplace(texture, Texture{image, view});
  return texture;
}

void SokolBackend::updateTexture(GpuTexture texture, const TextureLevels& levels) {
  const auto entry = _textures.find(texture);
  if (entry != _textures.end()) {
    sg_update_image(entry->second.image, image_data(levels));
  }
}

void SokolBackend::destroyTexture(GpuTexture texture) {
  const auto entry = _textures.find(texture);
  if (entry != _textures.end()) {
    sg_destroy_view(entry->second.view);
    sg_destroy_image(entry->second.image);
    _textures.erase(entry);
  }
}

sg_view SokolBackend::texture_view(GpuTexture texture) const {
  const auto entry = _textures.find(texture);
  return entry != _textures.end() ? entry->second.view : _nullTextureView;
}

GpuMesh SokolBackend::createMesh(const void* vertices, size_t vertexBytes, const void* indices, size_t indexBytes) {
  Mesh mesh{};
  {
    sg_buffer_desc description{};
    description.usage.vertex_buffer = true;
    description.data = sg_range{vertices, vertexBytes};
    mesh.vertices = sg_make_buffer(&description);
  }
  {
    sg_buffer_desc description{};
    description.usage.index_buffer = true;
    description.data = sg_range{indices, indexBytes};
    mesh.indices = sg_make_buffer(&description);
  }
  if (sg_query_buffer_state(mesh.vertices) != SG_RESOURCESTATE_VALID ||
      sg_query_buffer_state(mesh.indices) != SG_RESOURCESTATE_VALID) {
    sg_destroy_buffer(mesh.vertices);
    sg_destroy_buffer(mesh.indices);
    return 0;
  }
  const GpuMesh handle = ++_last_mesh;
  _meshes.emplace(handle, mesh);
  return handle;
}

void SokolBackend::destroyMesh(GpuMesh mesh) {
  const auto entry = _meshes.find(mesh);
  if (entry != _meshes.end()) {
    sg_destroy_buffer(entry->second.vertices);
    sg_destroy_buffer(entry->second.indices);
    _meshes.erase(entry);
  }
}

void SokolBackend::uploadTerrain(int width, int height, const uint8_t* heights, const uint8_t* attributes) {
#ifndef EMSCRIPTEN
  // A map of another size (another world): new images.
  if (_terrain_width != width || _terrain_height != height) {
    if (_terrain_heights.id != SG_INVALID_ID) {
      sg_destroy_view(_terrain_heights_view);
      sg_destroy_view(_terrain_attributes_view);
      sg_destroy_image(_terrain_heights);
      sg_destroy_image(_terrain_attributes);
    }
    make_terrain_map(width, height, _terrain_heights, _terrain_heights_view);
    make_terrain_map(width, height, _terrain_attributes, _terrain_attributes_view);
    _terrain_width = width;
    _terrain_height = height;
  }

  const size_t size = static_cast<size_t>(width) * height;
  sg_image_data data{};
  data.mip_levels[0] = sg_range{heights, size};
  sg_update_image(_terrain_heights, &data);
  data.mip_levels[0] = sg_range{attributes, size};
  sg_update_image(_terrain_attributes, &data);
  _has_terrain = true;
#endif
}

void SokolBackend::uploadBackBuffer(const uint16_t* pixels) {
  _backBuffer->upload(pixels);
}

void SokolBackend::uploadFrame(const M3D_VERTEX* vertices, size_t vertexCount, const uint32_t* indices,
                               size_t indexCount) {
  upload_frame_buffer(_vertex_buffer, _vertex_capacity, vertices, vertexCount * sizeof(M3D_VERTEX), false,
                      "vertex");
  upload_frame_buffer(_index_buffer, _index_capacity, indices, indexCount * sizeof(uint32_t), true, "index");
}

void SokolBackend::beginShadowPass(int camera) {
#ifndef EMSCRIPTEN
  ShadowMap& shadow = _shadow_maps[camera];
  if (shadow.image.id == SG_INVALID_ID) {
    sg_image_desc description{};
    description.usage.depth_stencil_attachment = true;
    description.width = shadow_map_size;
    description.height = shadow_map_size;
    description.pixel_format = SG_PIXELFORMAT_DEPTH;
    shadow.image = sg_make_image(&description);
    sg_view_desc attachment{};
    attachment.depth_stencil_attachment.image = shadow.image;
    shadow.attachment = sg_make_view(&attachment);
    sg_view_desc texture{};
    texture.texture.image = shadow.image;
    shadow.texture = sg_make_view(&texture);
    if (sg_query_view_state(shadow.attachment) != SG_RESOURCESTATE_VALID ||
        sg_query_view_state(shadow.texture) != SG_RESOURCESTATE_VALID) {
      ErrH.Abort("sg_make_view", XERR_USER, 0, "shadow map");
    }
    SDL_Log("Renderer: shadow map %d (%dx%d)", camera, shadow_map_size, shadow_map_size);
  }

  sg_pass pass{};
  pass.action.depth.load_action = SG_LOADACTION_CLEAR;
  pass.action.depth.clear_value = 1.0f;
  pass.attachments.depth_stencil = shadow.attachment;
  sg_begin_pass(&pass);
#endif
}

void SokolBackend::drawShadow(const ShadowDraw& draw) {
#ifndef EMSCRIPTEN
  const auto mesh = _meshes.find(draw.mesh);
  if (mesh == _meshes.end()) {
    return;
  }

  sg_bindings bindings{};
  bindings.views[VIEW_texture_1] = texture_view(draw.texture);
  bindings.samplers[SMP_sampler_1] = draw.repeat ? _repeat_sampler : _clamp_sampler;
  if (draw.animated) {
    bindings.vertex_buffers[0] = _vertex_buffer;
    bindings.vertex_buffer_offsets[0] = static_cast<int>(draw.pose_offset * sizeof(M3D_VERTEX));
  } else {
    bindings.vertex_buffers[0] = mesh->second.vertices;
  }
  bindings.index_buffer = mesh->second.indices;

  sg_apply_pipeline(shadow_pipeline(draw.animated));
  sg_apply_bindings(bindings);
  sg_apply_uniforms(UB_shadow_vs_params, SG_RANGE(draw.vs));
  sg_apply_uniforms(UB_shadow_fs_params, SG_RANGE(draw.fs));
  sg_draw(0, draw.index_count, 1);
#endif
}

void SokolBackend::beginScenePass(bool clear, const float (&clearColor)[4]) {
  // The depth buffer takes sokol's default: cleared to 1.
  sg_pass_action action{};
  if (clear) {
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = {clearColor[0], clearColor[1], clearColor[2], clearColor[3]};
  } else {
    action.colors[0].load_action = SG_LOADACTION_LOAD;
  }
  _offscreenBuffer->begin_pass(action);
  _in_scene_pass = true;
}

void SokolBackend::drawBackBuffer() {
  _backBuffer->flush();
}

void SokolBackend::bind_shadow(int camera, sg_bindings& bindings) const {
#ifndef EMSCRIPTEN
  bindings.views[VIEW_shadow_map] = camera >= 0 ? _shadow_maps[camera].texture : _null_shadow_map_view;
  bindings.views[VIEW_shadow_cells] = camera >= 0 && _has_terrain ? _terrain_attributes_view : _nullTextureView;
  bindings.samplers[SMP_shadow_sampler] = _shadow_sampler;
  bindings.samplers[SMP_shadow_cells_sampler] = _terrain_sampler;
#endif
}

void SokolBackend::draw(const SceneDraw& draw) {
  sg_bindings bindings{};
  bindings.views[VIEW_texture_1] = texture_view(draw.textures[0]);
  bindings.views[VIEW_texture_2] = texture_view(draw.textures[1]);
  bindings.samplers[SMP_sampler_1] = draw.repeat ? _repeat_sampler : _clamp_sampler;

  sg_range vs_params{};
  int vs_slot = 0;
  const Geometry geometry = draw.pipeline.geometry;
  if (geometry == Geometry::frame) {
    bindings.vertex_buffers[0] = _vertex_buffer;
    bindings.index_buffer = _index_buffer;
    bind_shadow(draw.shadow_camera, bindings);
    vs_params = sg_range{&draw.vs.scene, sizeof(scene_vs_params_t)};
    vs_slot = UB_scene_vs_params;
  } else {
#ifndef EMSCRIPTEN
    const auto mesh = _meshes.find(draw.mesh);
    if (mesh == _meshes.end()) {
      return;
    }
    bindings.index_buffer = mesh->second.indices;
    if (geometry == Geometry::tile) {
      bindings.vertex_buffers[0] = mesh->second.vertices;
      bind_shadow(draw.shadow_camera, bindings);
      vs_params = SG_RANGE(draw.vs.tile);
      vs_slot = UB_tile_vs_params;
    } else {
      bindings.views[VIEW_terrain_heights] = draw.terrain_maps ? _terrain_heights_view : _nullTextureView;
      bindings.views[VIEW_terrain_attributes] = draw.terrain_maps ? _terrain_attributes_view : _nullTextureView;
      bindings.samplers[SMP_terrain_sampler] = _terrain_sampler;
      if (geometry == Geometry::animated_mesh) {
        // Positions and texture coordinates from this frame's pose, normals from the mesh.
        bindings.vertex_buffers[0] = _vertex_buffer;
        bindings.vertex_buffer_offsets[0] = static_cast<int>(draw.pose_offset * sizeof(M3D_VERTEX));
        bindings.vertex_buffers[1] = mesh->second.vertices;
      } else {
        bindings.vertex_buffers[0] = mesh->second.vertices;
      }
      // Meshes take no dynamic shadows; scene_fs still needs its shadow bindings.
      bind_shadow(-1, bindings);
      vs_params = SG_RANGE(draw.vs.mesh);
      vs_slot = UB_mesh_vs_params;
    }
#else
    return;
#endif
  }

  sg_apply_viewport(draw.viewport.x, draw.viewport.y, draw.viewport.width, draw.viewport.height, true);
  sg_apply_pipeline(scene_pipeline(draw.pipeline));
  sg_apply_bindings(bindings);
  sg_apply_uniforms(vs_slot, vs_params);
  sg_apply_uniforms(UB_scene_fs_params, SG_RANGE(draw.fs));
#ifndef EMSCRIPTEN
  sg_apply_uniforms(UB_scene_shadow_params, SG_RANGE(draw.shadow));
#endif
  sg_draw(draw.first_index, draw.index_count, 1);
}

void SokolBackend::endPass() {
  sg_end_pass();
  if (_in_scene_pass) {
    sg_commit();
    _in_scene_pass = false;
  }
}

void SokolBackend::present() {
  _offscreenBuffer->flush(_display->swapchain());
  _display->present();
}

sg_pipeline SokolBackend::shadow_pipeline(bool animated) {
#ifndef EMSCRIPTEN
  sg_pipeline& pipeline = _shadow_pipelines[animated ? 1 : 0];
  if (pipeline.id != SG_INVALID_ID) {
    return pipeline;
  }

  // Depth only. No culling: the CPU raster draws both sides of every polygon.
  sg_pipeline_desc description{};
  description.shader = _shadowShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.index_type = SG_INDEXTYPE_UINT16;
  description.cull_mode = SG_CULLMODE_NONE;
  description.colors[0].pixel_format = SG_PIXELFORMAT_NONE;
  description.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
  description.depth.write_enabled = true;
  description.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
  if (animated) {
    // This frame's pose in the frame vertex array.
    description.layout.buffers[0].stride = sizeof(M3D_VERTEX);
    description.layout.attrs[ATTR_shadow_pos].offset = offsetof(M3D_VERTEX, x);
    description.layout.attrs[ATTR_shadow_uv_in].offset = offsetof(M3D_VERTEX, u);
  } else {
    description.layout.buffers[0].stride = sizeof(M3D_MESH_VERTEX);
    description.layout.attrs[ATTR_shadow_pos].offset = offsetof(M3D_MESH_VERTEX, x);
    description.layout.attrs[ATTR_shadow_uv_in].offset = offsetof(M3D_MESH_VERTEX, u);
  }
  description.layout.attrs[ATTR_shadow_pos].format = SG_VERTEXFORMAT_FLOAT3;
  description.layout.attrs[ATTR_shadow_uv_in].format = SG_VERTEXFORMAT_FLOAT2;

  pipeline = sg_make_pipeline(description);
  if (sg_query_pipeline_state(pipeline) != SG_RESOURCESTATE_VALID) {
    ErrH.Abort("sg_make_pipeline", XERR_USER, 0, "");
  }
  SDL_Log("Renderer: %s shadow pipeline created", animated ? "animated" : "static");
  return pipeline;
#else
  return {};
#endif
}

sg_pipeline SokolBackend::scene_pipeline(const PipelineKey& key) {
  for (const auto& entry : _scene_pipelines) {
    if (entry.first == key) {
      return entry.second;
    }
  }

  // Built from the key alone, so draws with the same key share a pipeline.
  const bool is_frame = key.geometry == Geometry::frame;
  sg_pipeline_desc description = {};
  description.shader = is_frame ? _sceneShader : key.geometry == Geometry::tile ? _tileShader : _meshShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.index_type = is_frame || key.geometry == Geometry::tile ? SG_INDEXTYPE_UINT32 : SG_INDEXTYPE_UINT16;
  // The 2000 renderer culled on the CPU, keeping triangles that wind one way on the screen
  // (AddPolygonFixTestPointFix, master). A mirror turns them the other way, so it sent a
  // reflection's corners in the opposite order; a mirrored GPU mesh flips the front face.
  description.cull_mode = SG_CULLMODE_BACK;
  description.face_winding = key.mirrored ? SG_FACEWINDING_CW : SG_FACEWINDING_CCW;

  description.depth.write_enabled = key.depth_write;
  description.depth.compare = key.depth_test ? SG_COMPAREFUNC_LESS_EQUAL : SG_COMPAREFUNC_ALWAYS;

  const sg_blend_factor source = blend_factor(key.source);
  const sg_blend_factor destination = blend_factor(key.destination);
  description.colors[0].blend.enabled = key.blend;
  description.colors[0].blend.src_factor_rgb = source;
  description.colors[0].blend.src_factor_alpha = source;
  description.colors[0].blend.dst_factor_rgb = destination;
  description.colors[0].blend.dst_factor_alpha = destination;

  if (key.geometry == Geometry::mesh) {
#ifndef EMSCRIPTEN
    // One M3D_MESH_VERTEX per vertex; the normal bytes arrive in the shader as integers.
    description.layout.buffers[0].stride = sizeof(M3D_MESH_VERTEX);

    description.layout.attrs[ATTR_mesh_pos].offset = offsetof(M3D_MESH_VERTEX, x);
    description.layout.attrs[ATTR_mesh_pos].format = SG_VERTEXFORMAT_FLOAT3;

    description.layout.attrs[ATTR_mesh_normal_in].offset = offsetof(M3D_MESH_VERTEX, normal);
    description.layout.attrs[ATTR_mesh_normal_in].format = SG_VERTEXFORMAT_BYTE4;

    description.layout.attrs[ATTR_mesh_uv_in].offset = offsetof(M3D_MESH_VERTEX, u);
    description.layout.attrs[ATTR_mesh_uv_in].format = SG_VERTEXFORMAT_FLOAT2;
#endif
  } else if (key.geometry == Geometry::animated_mesh) {
#ifndef EMSCRIPTEN
    // Buffer 0: this frame's pose in the frame vertex array (position and texture
    // coordinates of M3D_VERTEX). Buffer 1: the mesh's own vertices, for the normal.
    description.layout.buffers[0].stride = sizeof(M3D_VERTEX);
    description.layout.buffers[1].stride = sizeof(M3D_MESH_VERTEX);

    description.layout.attrs[ATTR_mesh_pos].buffer_index = 0;
    description.layout.attrs[ATTR_mesh_pos].offset = offsetof(M3D_VERTEX, x);
    description.layout.attrs[ATTR_mesh_pos].format = SG_VERTEXFORMAT_FLOAT3;

    description.layout.attrs[ATTR_mesh_uv_in].buffer_index = 0;
    description.layout.attrs[ATTR_mesh_uv_in].offset = offsetof(M3D_VERTEX, u);
    description.layout.attrs[ATTR_mesh_uv_in].format = SG_VERTEXFORMAT_FLOAT2;

    description.layout.attrs[ATTR_mesh_normal_in].buffer_index = 1;
    description.layout.attrs[ATTR_mesh_normal_in].offset = offsetof(M3D_MESH_VERTEX, normal);
    description.layout.attrs[ATTR_mesh_normal_in].format = SG_VERTEXFORMAT_BYTE4;
#endif
  } else if (key.geometry == Geometry::tile) {
#ifndef EMSCRIPTEN
    // One M3D_TILE_VERTEX per vertex; the colour bytes arrive in the shader as 0..1.
    description.layout.buffers[0].stride = sizeof(M3D_TILE_VERTEX);

    description.layout.attrs[ATTR_tile_pos].offset = offsetof(M3D_TILE_VERTEX, x);
    description.layout.attrs[ATTR_tile_pos].format = SG_VERTEXFORMAT_FLOAT3;

    description.layout.attrs[ATTR_tile_color_in].offset = offsetof(M3D_TILE_VERTEX, color);
    description.layout.attrs[ATTR_tile_color_in].format = SG_VERTEXFORMAT_UBYTE4N;

    description.layout.attrs[ATTR_tile_uv_in].offset = offsetof(M3D_TILE_VERTEX, u);
    description.layout.attrs[ATTR_tile_uv_in].format = SG_VERTEXFORMAT_FLOAT2;
#endif
  } else {
    // One interleaved M3D_VERTEX per vertex; the colour bytes arrive in the shader as 0..1.
    description.layout.buffers[0].stride = sizeof(M3D_VERTEX);

    description.layout.attrs[ATTR_scene_pos].offset = offsetof(M3D_VERTEX, x);
    description.layout.attrs[ATTR_scene_pos].format = SG_VERTEXFORMAT_FLOAT3;

    description.layout.attrs[ATTR_scene_diffuse_in].offset = offsetof(M3D_VERTEX, diffuse);
    description.layout.attrs[ATTR_scene_diffuse_in].format = SG_VERTEXFORMAT_UBYTE4N;

    description.layout.attrs[ATTR_scene_specular_in].offset = offsetof(M3D_VERTEX, specular);
    description.layout.attrs[ATTR_scene_specular_in].format = SG_VERTEXFORMAT_UBYTE4N;

    description.layout.attrs[ATTR_scene_uv_in].offset = offsetof(M3D_VERTEX, u);
    description.layout.attrs[ATTR_scene_uv_in].format = SG_VERTEXFORMAT_FLOAT2;
  }

  const auto pipeline = sg_make_pipeline(description);
  if (sg_query_pipeline_state(pipeline) != SG_RESOURCESTATE_VALID) {
    ErrH.Abort("sg_make_pipeline", XERR_USER, 0, "");
  }
  _scene_pipelines.emplace_back(key, pipeline);
  const char* kind = key.geometry == Geometry::frame  ? "scene"
                     : key.geometry == Geometry::mesh ? "mesh"
                     : key.geometry == Geometry::tile ? "tile"
                                                      : "animated mesh";
  SDL_Log("Renderer: %s pipeline %u created (depth test %d, depth write %d, blend %d, factors %d/%d%s)", kind,
          static_cast<unsigned>(_scene_pipelines.size()), key.depth_test ? 1 : 0, key.depth_write ? 1 : 0,
          key.blend ? 1 : 0, static_cast<int>(source), static_cast<int>(destination),
          key.mirrored ? ", mirrored" : "");
  return pipeline;
}
