#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sokol_gfx.h>

#include "render_backend.h"

namespace graphics {

class BackBuffer;
class Display;
class OffscreenBuffer;

// RenderBackend with sokol on OpenGL 4.1 (WebGL 2 on the web): the renderer every build
// has, and the one the web and machines without SDL GPU draw with.
class SokolBackend final : public RenderBackend {
 public:
  // wanted: the GPU paths the game asked for; features() says which could be set up.
  SokolBackend(int width, int height, Features wanted);
  ~SokolBackend() override;

  SokolBackend(const SokolBackend&) = delete;
  SokolBackend& operator=(const SokolBackend&) = delete;

  Features features() const override { return _features; }
  const char* name() const override { return "OpenGL (sokol)"; }
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
  void present() override;

 private:
  sg_view texture_view(GpuTexture texture) const;
  void bind_shadow(int camera, sg_bindings& bindings) const;
  sg_pipeline scene_pipeline(const PipelineKey& key);
  sg_pipeline shadow_pipeline(bool animated);

  // The window and its context. Declared first, so it is destroyed last.
  std::unique_ptr<Display> _display;
  Features _features{};

  sg_shader _sceneShader{};
  sg_shader _meshShader{};
  sg_shader _tileShader{};
  sg_shader _shadowShader{};
  sg_image _nullTexture{};
  sg_view _nullTextureView{};
  sg_sampler _clamp_sampler{};
  sg_sampler _repeat_sampler{};
  sg_sampler _terrain_sampler{};
  sg_sampler _shadow_sampler{};
  sg_image _null_shadow_map{};
  sg_view _null_shadow_map_view{};

  std::unique_ptr<OffscreenBuffer> _offscreenBuffer;
  std::unique_ptr<BackBuffer> _backBuffer;
  bool _in_scene_pass = false;

  // The frame arrays, grown when a frame needs more.
  sg_buffer _vertex_buffer{};
  sg_buffer _index_buffer{};
  size_t _vertex_capacity = 0;
  size_t _index_capacity = 0;

  struct Texture {
    sg_image image;
    sg_view view;
  };
  std::unordered_map<GpuTexture, Texture> _textures;
  GpuTexture _last_texture = 0;

  struct Mesh {
    sg_buffer vertices;
    sg_buffer indices;
  };
  std::unordered_map<GpuMesh, Mesh> _meshes;
  GpuMesh _last_mesh = 0;

  int _terrain_width = 0;
  int _terrain_height = 0;
  bool _has_terrain = false;
  sg_image _terrain_heights{};
  sg_image _terrain_attributes{};
  sg_view _terrain_heights_view{};
  sg_view _terrain_attributes_view{};

  struct ShadowMap {
    sg_image image{};
    sg_view attachment{};
    sg_view texture{};
  };
  std::array<ShadowMap, max_shadow_cameras> _shadow_maps;
  sg_pipeline _shadow_pipelines[2]{};  // static and animated casters

  // Pipelines are created the first time they are needed, and kept; only a handful, so a
  // list is enough.
  std::vector<std::pair<PipelineKey, sg_pipeline>> _scene_pipelines;
};

}  // namespace graphics
