#pragma once

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL3/SDL_gpu.h>

#include "render_backend.h"
#include "sdl_gpu/shaders.h"

namespace graphics {
class Window;
}

namespace graphics::sdl_gpu {

// RenderBackend with SDL GPU: Metal on macOS, Direct3D 12 on Windows, Vulkan on Linux,
// with the shaders in sdl_gpu/shaders.
//
// A frame is one command buffer, acquired at the first thing that needs it and submitted
// at present(). Uploads go into a copy pass on it, which is ended before each render pass:
// the renderer makes all of a scene's uploads before its passes. What is uploaded every
// frame (the frame arrays, the back buffer) goes through transfer buffers that SDL cycles,
// and into GPU buffers and textures it cycles too, so a frame never waits for the one
// before it to finish with them.
class Backend final : public RenderBackend {
 public:
  // Null if SDL GPU has no device for the shaders' format here, or it cannot be set up;
  // the game then tells the player and quits (Renderer). debug: SDL's debug mode, with
  // the API's own checks where they are installed (the Direct3D 12 debug layer, Vulkan's
  // validation layers); always on in a debug build.
  static std::unique_ptr<Backend> create(int width, int height, Features wanted, bool debug);
  ~Backend() override;

  Backend(const Backend&) = delete;
  Backend& operator=(const Backend&) = delete;

  Features features() const override { return _features; }
  const char* name() const override { return _name.c_str(); }
  // Direct3D 12 and Vulkan compile a pipeline when it is made: done at its first draw,
  // that would stall the frame, and a pipeline the driver refuses would leave its draws
  // out for the rest of the game.
  bool preparePipelines(const std::vector<PipelineKey>& keys) override;
  Scale setVideoMode(int width, int height, bool isFullScreen) override;

  GpuTexture createTexture(int width, int height, int levelCount, const TextureLevels* levels) override;
  void updateTexture(GpuTexture texture, const TextureLevels& levels) override;
  void destroyTexture(GpuTexture texture) override;

  GpuMesh createMesh(const void* vertices, size_t vertexBytes, const void* indices, size_t indexBytes) override;
  void destroyMesh(GpuMesh mesh) override;

  void uploadTerrain(int width, int height, const uint8_t* heights, const uint8_t* attributes) override;
  void uploadBackBuffer(const uint16_t* pixels) override;
  void uploadFrame(const M3D_VERTEX* vertices, size_t vertexCount, const uint32_t* indices,
                   size_t indexCount) override;

  void beginShadowPass(int camera) override;
  void drawShadow(const ShadowDraw& draw) override;
  void beginScenePass(bool clear, const float (&clearColor)[4]) override;
  void drawBackBuffer() override;
  void draw(const SceneDraw& draw) override;
  void endPass() override;
  void present(const FlushFsParams& flush) override;

 private:
  Backend() = default;
  bool init(int width, int height, Features wanted, bool debug);

  SDL_GPUCommandBuffer* command_buffer();
  SDL_GPUCopyPass* copy_pass();
  void end_copy_pass();
  void bind_pipeline(SDL_GPUGraphicsPipeline* pipeline);

  // A buffer uploaded every frame: grown (with its transfer buffer) when the frame needs
  // more.
  struct Stream {
    SDL_GPUBuffer* buffer = nullptr;
    SDL_GPUTransferBuffer* transfer = nullptr;
    size_t capacity = 0;
  };
  void upload_stream(Stream& stream, SDL_GPUBufferUsageFlags usage, const void* data, size_t bytes,
                     const char* name);

  // Texture levels through a transfer buffer made for them. cycle: the texture may still
  // be in use by draws already recorded; SDL then writes into a fresh copy of it.
  void upload_texture(SDL_GPUTexture* texture, int width, int height, const TextureLevels& levels, bool cycle);

  SDL_GPUTexture* texture_or_null(GpuTexture texture) const;
  SDL_GPUShader* shader(Shader shader) const { return _shaders[static_cast<size_t>(shader)]; }
  // Made at their first use if preparePipelines didn't make them. Null if the driver
  // refuses one: its draws are left out.
  SDL_GPUGraphicsPipeline* scene_pipeline(const PipelineKey& key);
  SDL_GPUGraphicsPipeline* shadow_pipeline(bool animated);
  void make_target_textures(int width, int height);

  SDL_GPUDevice* _device = nullptr;
  std::unique_ptr<Window> _window;
  std::string _name;
  Features _features{};

  SDL_GPUTextureFormat _swapchain_format = SDL_GPU_TEXTUREFORMAT_INVALID;
  SDL_GPUTextureFormat _depth_format = SDL_GPU_TEXTUREFORMAT_INVALID;

  std::array<SDL_GPUShader*, static_cast<size_t>(Shader::count)> _shaders{};
  SDL_GPUSampler* _clamp_sampler = nullptr;
  SDL_GPUSampler* _repeat_sampler = nullptr;
  SDL_GPUSampler* _nearest_sampler = nullptr;
  SDL_GPUSampler* _shadow_sampler = nullptr;
  SDL_GPUTexture* _null_texture = nullptr;
  SDL_GPUTexture* _null_shadow_map = nullptr;

  // The offscreen buffer the scene is drawn into, at the game's resolution, and the back
  // buffer drawn over it.
  int _width = 0;
  int _height = 0;
  SDL_GPUTexture* _color = nullptr;
  SDL_GPUTexture* _depth = nullptr;
  SDL_GPUTexture* _back_buffer = nullptr;
  SDL_GPUTransferBuffer* _back_buffer_transfer = nullptr;

  Stream _vertices;
  Stream _indices;

  struct Texture {
    SDL_GPUTexture* texture;
    int width;
    int height;
  };
  std::unordered_map<GpuTexture, Texture> _textures;
  GpuTexture _last_texture = 0;

  struct Mesh {
    SDL_GPUBuffer* vertices;
    SDL_GPUBuffer* indices;
  };
  std::unordered_map<GpuMesh, Mesh> _meshes;
  GpuMesh _last_mesh = 0;

  int _terrain_width = 0;
  int _terrain_height = 0;
  SDL_GPUTexture* _terrain_heights = nullptr;
  SDL_GPUTexture* _terrain_attributes = nullptr;

  std::array<SDL_GPUTexture*, max_shadow_cameras> _shadow_maps{};

  SDL_GPUGraphicsPipeline* _back_buffer_pipeline = nullptr;
  SDL_GPUGraphicsPipeline* _flush_pipeline = nullptr;
  SDL_GPUGraphicsPipeline* _shadow_pipelines[2]{};  // static and animated casters
  bool _shadow_pipeline_failed[2]{};
  // Made by preparePipelines, or at their first draw, and kept; a few dozen, so a list is
  // enough. A null pipeline is one the driver refused.
  std::vector<std::pair<PipelineKey, SDL_GPUGraphicsPipeline*>> _scene_pipelines;
  bool _pipelines_prepared = false;

  // The frame being recorded.
  SDL_GPUCommandBuffer* _commands = nullptr;
  SDL_GPUCopyPass* _copy_pass = nullptr;
  SDL_GPURenderPass* _pass = nullptr;
  SDL_GPUGraphicsPipeline* _bound_pipeline = nullptr;
};

}  // namespace graphics::sdl_gpu
