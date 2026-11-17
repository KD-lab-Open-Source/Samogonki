//
// Created by caiiiycuk on 08.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDERER_H
#define MOONSHINE_RUNNERS_RENDERER_H

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <sokol_gfx.h>

#include "IGraph3d.h"
#include "render_state.h"

namespace graphics {

class BackBuffer;
class Display;
class OffscreenBuffer;
class TextureManager;

class Renderer final {
 public:
  // textureMipmaps: build a mip chain for every texture, for trilinear filtering.
  // gpuMeshes: keep meshes on the GPU (createMesh); off, createMesh always refuses.
  // gpuShadows: dynamic shadows from shadow maps (setShadowCamera); needs gpuMeshes.
  // renderStats: log where the frame time goes, every 5 seconds.
  Renderer(int width, int height, bool isFullScreen, bool textureMipmaps, bool gpuMeshes, bool gpuShadows,
           bool renderStats);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  TextureManager& get_texture_manager();
  void setVideoMode(int width, int height, bool isFullScreen);
  bool isInScene() const;

  MD3DERROR clear(uint32_t dwColor);
  MD3DERROR flip(bool WaitVerticalBlank);
  MD3DERROR setClipRect(const MD3DRECT &lprcClipRect);
  MD3DERROR resetClipRect();
  MD3DERROR beginScene();
  MD3DERROR endScene();
  MD3DERROR setProjectionMatrix(const D3DMATRIX &matrix);
  MD3DERROR resetProjectionMatrix();

  MD3DERROR setRenderState(eRenderStateOption option, int value);
  MD3DERROR setMaterial(eMaterialMode material);
  MD3DERROR setTexture(uint32_t dwHandle, uint32_t dwStage);

  MD3DERROR beginDrawCommand(M3D_DRAW_COMMAND &command);
  MD3DERROR endDrawCommand(const M3D_DRAW_COMMAND &command);

  // Meshes kept on the GPU: uploaded once, drawn with per-draw uniforms only.
  uint32_t createMesh(const M3D_MESH_VERTEX *vertices, int vertexCount, const uint16_t *indices, int indexCount);
  void deleteMesh(uint32_t handle);
  // pose: an animated mesh's positions and texture coordinates this frame, or null.
  void drawMesh(uint32_t handle, const M3D_MESH_DRAW &draw, const M3D_MESH_POSE *pose);
  void setTerrain(const M3D_TERRAIN &terrain);
  // A camera's dynamic shadows for this frame; false if the renderer doesn't draw them.
  bool setShadowCamera(int camera, const M3D_SHADOW_CAMERA &shadow);

  MD3DERROR lockBackBuffer(void** lplpSurface, uint32_t* lpdwPitch);
  MD3DERROR unlockBackBuffer();
  MD3DERROR flushBackBuffer(MD3DRECT* lprcRect);

 private:
  void prepare_render_state();
  void copy_projection(const RenderState &state, float *matrix) const;
  void apply_viewport(const RenderState &state) const;
  void bind_textures(const RenderState &state, sg_bindings &bindings);

private:
  // How many vertices the per-frame arrays hold at start. A frame that needs more
  // grows them (M3D_BUFFER_VIEW), and endScene() then recreates the GPU buffers at
  // the new size.
  static constexpr size_t initial_vertex_count = 100000;

  // The window and its context. Declared first, so it is destroyed last.
  std::unique_ptr<Display> _display;
  std::array<float, 16> _projectionMatrix;

  sg_shader _sceneShader;
  sg_shader _meshShader{};
  sg_image _nullTexture;
  sg_view _nullTextureView;

  std::vector<M3D_VERTEX> _vertex_buffer;
  std::vector<uint32_t> _index_buffer;

  sg_buffer sg_vertex_buffer;
  sg_buffer sg_index_buffer;
  sg_sampler _clamp_sampler;
  sg_sampler _repeat_sampler;

  std::unique_ptr<OffscreenBuffer> _offscreenBuffer;
  std::unique_ptr<BackBuffer> _backBuffer;
  std::unique_ptr<TextureManager> _texture_manager;

  sg_pass_action defaultPassAction = {};
  RenderState _render_state;

  struct BufferView
  {
    size_t length;
    size_t offset;

    size_t end() const
    {
      return length + offset;
    }

    BufferView next() const
    {
      return BufferView{0, end()};
    }
  };

  struct DrawCommand
  {
    RenderState render_state;
    BufferView vertex_buffer_view;
    BufferView index_buffer_view;
    // Non-zero: this command draws a GPU mesh (drawMesh) instead of frame geometry. An
    // animated mesh's pose is in the frame vertex array, at vertex_buffer_view.
    uint32_t mesh = 0;
    bool is_animated = false;
    M3D_MESH_DRAW mesh_draw{};
    // A shadow caster's box in object space: minimum x, y, z, then maximum.
    std::array<float, 6> bounds{};
  };

  std::vector<DrawCommand> _commands;
  void draw_mesh_command(const DrawCommand &command);
  bool _is_back_buffer_flush = false;
  bool _is_in_scene = false;

  // GPU meshes: two immutable buffers each, made by createMesh. A deleted mesh is kept
  // until the end of the frame, as a command recorded before the delete may still use it.
  static constexpr size_t max_meshes = 4096;
  struct Mesh
  {
    sg_buffer vertices;
    sg_buffer indices;
    int vertex_count;
    int index_count;
    size_t bytes;
    std::array<float, 6> bounds;  // in object space: minimum x, y, z, then maximum
  };
  bool _gpu_meshes;
  std::unordered_map<uint32_t, Mesh> _meshes;
  std::vector<uint32_t> _deleted_meshes;
  uint32_t _last_mesh = 0;
  size_t _mesh_bytes = 0;
  bool _is_mesh_pool_full_logged = false;
  void destroy_deleted_meshes();

  // The terrain maps for the mesh shadow test. setTerrain() gives the game's own maps;
  // a frame whose meshes test shadows compares them with the copy here and uploads them
  // again before its pass if they changed.
  M3D_TERRAIN _terrain{};
  bool _has_terrain = false;
  bool _is_terrain_needed = false;
  bool _is_terrain_uploaded = false;
  int _terrain_width = 0;
  int _terrain_height = 0;
  std::vector<uint8_t> _terrain_heights_copy;
  std::vector<uint8_t> _terrain_attributes_copy;
  sg_image _terrain_heights{};
  sg_image _terrain_attributes{};
  sg_view _terrain_heights_view{};
  sg_view _terrain_attributes_view{};
  sg_sampler _terrain_sampler{};
  void upload_terrain();

  // Dynamic shadows: a depth map per camera (split screen has two), fitted each frame to
  // the camera's casters in a light space anchored to the world, and drawn before the
  // scene. The terrain draws that take them sample it with a comparison sampler.
  static constexpr int max_shadow_cameras = 4;
  static constexpr int shadow_map_size = 2048;
  struct ShadowCamera
  {
    bool is_set = false;      // setShadowCamera this frame
    bool has_map = false;     // its map holds this frame's casters
    M3D_SHADOW_CAMERA camera{};
    float view_to_light[12]{};   // 3x4 row-major: camera space to light space
    float light_to_clip[12]{};   // 3x4 row-major: light space to the map's clip space
    float filter_step = 0;       // between filter taps, in map texture coordinates
    float depth_bias = 0;        // in map depth
    sg_image map{};
    sg_view attachment{};
    sg_view texture{};
  };
  bool _gpu_shadows;
  std::array<ShadowCamera, max_shadow_cameras> _shadow_cameras;
  sg_shader _shadowShader{};
  sg_pipeline _shadow_pipelines[2]{};  // static and animated casters
  sg_sampler _shadow_sampler{};
  sg_image _null_shadow_map{};
  sg_view _null_shadow_map_view{};
  void render_shadow_maps();
  sg_pipeline shadow_pipeline(bool animated);
  // The shadow bindings and uniforms every scene_fs draw needs; camera -1 for none.
  void bind_shadow(int camera, sg_bindings &bindings);
  void apply_shadow_uniforms(int camera);

  // What a command draws, which decides the shader and the vertex layout.
  enum class Geometry
  {
    frame,          // the frame vertex and index arrays (scene shader)
    mesh,           // a GPU mesh (mesh shader)
    animated_mesh,  // a GPU mesh, positions and texture coordinates from the frame array
  };

  // The part of a command's render state that a scene pipeline depends on, and what it
  // draws. Everything else (culling) is the same for all draws.
  struct ScenePipelineKey
  {
    Geometry geometry;
    bool depth_test;
    bool depth_write;
    bool blend;
    sg_blend_factor source;
    sg_blend_factor destination;

    bool operator==(const ScenePipelineKey &other) const
    {
      return geometry == other.geometry && depth_test == other.depth_test && depth_write == other.depth_write &&
             blend == other.blend && source == other.source && destination == other.destination;
    }
  };

  // The pipeline for a command, created the first time it is needed.
  sg_pipeline scene_pipeline(const RenderState &state, Geometry geometry);

  // Only a handful of entries, so a list is enough.
  std::vector<std::pair<ScenePipelineKey, sg_pipeline>> _scene_pipelines;

  // game.ini [m3d] RenderStats: sums over the frames since the last report. The scene is
  // the game building it (beginScene to endScene: its CPU work, CPU mesh lighting
  // included), submit is endScene sending it to GL, present is flip.
  using Clock = std::chrono::steady_clock;
  struct Stats
  {
    unsigned frames = 0;
    double scene_ms = 0;
    double submit_ms = 0;
    double present_ms = 0;
    uint64_t draws = 0;
    uint64_t mesh_draws = 0;
    uint64_t animated_mesh_draws = 0;
    uint64_t pose_vertices = 0;
    uint64_t shadow_maps = 0;
    uint64_t shadow_casters = 0;
    uint64_t vertices = 0;
    uint64_t indices = 0;
    unsigned terrain_uploads = 0;
    // The CPU-built geometry by what it draws (M3D_DRAW_COMMAND::source).
    std::array<uint64_t, static_cast<size_t>(eDrawSource::count)> source_vertices{};
    std::array<uint64_t, static_cast<size_t>(eDrawSource::count)> source_commands{};
  };
  bool _render_stats;
  Stats _stats;
  Clock::time_point _stats_start;
  Clock::time_point _scene_begin;
  void report_stats(Clock::time_point now);
};

}  // namespace graphics

#endif  // MOONSHINE_RUNNERS_RENDERER_H
