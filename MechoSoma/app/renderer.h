//
// Created by caiiiycuk on 08.02.22.
//

#ifndef MOONSHINE_RUNNERS_RENDERER_H
#define MOONSHINE_RUNNERS_RENDERER_H

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IGraph3d.h"
#include "render_backend.h"
#include "render_state.h"

namespace graphics {

class TextureManager;

// The game's Direct3D-era drawing interface (Graph3d), on top of a RenderBackend. Renderer
// records the frame's draw commands, keeps the meshes' and the terrain's bookkeeping, fits
// the shadow maps and counts the stats; the backend only turns resolved draws into API
// calls.
class Renderer final {
 public:
  // sdlGpu: draw with SDL GPU (Metal, Direct3D 12 or Vulkan) where the build has it and
  // the machine can; otherwise, and off, with OpenGL.
  // sdlGpuDebug: SDL GPU's debug mode, with the API's own checks where installed.
  // textureMipmaps: build a mip chain for every texture, for trilinear filtering.
  // gpuMeshes: keep meshes on the GPU (createMesh); off, createMesh always refuses.
  // gpuShadows: dynamic shadows from shadow maps (setShadowCamera); needs gpuMeshes.
  // gpuTerrain: keep terrain tiles on the GPU (createTiles); off, createTiles always refuses.
  // renderStats: log where the frame time goes, every 5 seconds.
  Renderer(int width, int height, bool isFullScreen, bool sdlGpu, bool sdlGpuDebug, bool textureMipmaps,
           bool gpuMeshes, bool gpuShadows, bool gpuTerrain, bool renderStats);
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
  // A world's terrain tiles kept on the GPU in one pair of buffers, each tile a range of
  // the indices, bent into the planet in a shader. The set shares the mesh handles:
  // deleteMesh deletes it.
  uint32_t createTiles(const M3D_TILE_VERTEX *vertices, int vertexCount, const uint32_t *indices, int indexCount);
  void drawTile(uint32_t handle, const M3D_TILE_DRAW &draw);
  // Names the frames in the stats lines; a new label ends the report so far.
  void setStatsLabel(const char *label);
  void setTerrain(const M3D_TERRAIN &terrain);
  // A camera's dynamic shadows for this frame; false if the renderer doesn't draw them.
  bool setShadowCamera(int camera, const M3D_SHADOW_CAMERA &shadow);
  // The game's gamma option for each channel, from 0.2 to 5; 1 changes nothing. It applies
  // from the next flip, to everything on the screen.
  void setGamma(float red, float green, float blue);
  void getGamma(float &red, float &green, float &blue) const;

  MD3DERROR lockBackBuffer(void** lplpSurface, uint32_t* lpdwPitch);
  MD3DERROR unlockBackBuffer();
  MD3DERROR flushBackBuffer(MD3DRECT* lprcRect);

 private:
  struct DrawCommand;

  void prepare_render_state();
  void copy_projection(const RenderState &state, float *matrix) const;
  Viewport viewport(const RenderState &state) const;
  PipelineKey pipeline_key(const RenderState &state, Geometry geometry, bool mirrored = false) const;
  // Every key pipeline_key() gives for the states the game sets, for the backend to make
  // at startup.
  std::vector<PipelineKey> pipeline_keys(const RenderBackend::Features &features) const;
  // Uploads the command's textures if the game changed them: before any pass begins.
  void resolve_textures(const RenderState &state);
  // The draw's textures, sampler and fragment uniforms, from the command's render state.
  void fill_material(const RenderState &state, SceneDraw &draw);
  void fill_shadow(int camera, SceneDraw &draw) const;
  void draw_frame_command(const DrawCommand &command);
  void draw_mesh_command(const DrawCommand &command);
  void draw_tile_command(const DrawCommand &command);

  // How many vertices and indices the per-frame arrays hold at start. A frame that needs
  // more grows them (M3D_BUFFER_VIEW, drawMesh), and the backend then grows its buffers.
  static constexpr size_t initial_vertex_count = 65536;
  static constexpr size_t initial_index_count = 65536;
  static constexpr size_t max_meshes = RenderBackend::max_meshes;
  static constexpr int max_shadow_cameras = RenderBackend::max_shadow_cameras;
  static constexpr int shadow_map_size = RenderBackend::shadow_map_size;

  // The backend owns the window. Declared first, so it is destroyed last.
  std::unique_ptr<RenderBackend> _backend;
  std::unique_ptr<TextureManager> _texture_manager;

  int _width = 0;
  int _height = 0;
  std::array<float, 16> _projectionMatrix;

  std::vector<M3D_VERTEX> _vertex_buffer;
  std::vector<uint32_t> _index_buffer;

  // The scene pass starts from the clear colour after clear() and flip(), and from what
  // the offscreen buffer holds otherwise (a second scene in a frame).
  bool _clear = true;
  float _clear_color[4]{0.0f, 0.0f, 0.0f, 1.0f};  // opaque, as clear() makes it
  float _gamma[3]{1.0f, 1.0f, 1.0f};
  RenderState _render_state;

  // The game's 2D back buffer: RGB 565, the game's resolution. Uploaded when the frame
  // that flushes it is drawn, if the game changed it.
  std::vector<uint16_t> _back_buffer;
  bool _is_back_buffer_locked = false;
  bool _is_back_buffer_changed = false;

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
    // Non-zero: this command draws a GPU mesh (drawMesh) or terrain tile (drawTile)
    // instead of frame geometry. An animated mesh's pose is in the frame vertex array, at
    // vertex_buffer_view.
    uint32_t mesh = 0;
    bool is_animated = false;
    bool is_tile = false;
    M3D_MESH_DRAW mesh_draw{};
    M3D_TILE_DRAW tile_draw{};
    // A shadow caster's box in object space: minimum x, y, z, then maximum.
    std::array<float, 6> bounds{};
  };

  std::vector<DrawCommand> _commands;
  bool _is_back_buffer_flush = false;
  bool _is_in_scene = false;

  // GPU meshes and terrain tile sets, made by createMesh and createTiles. A deleted one
  // is kept until the end of the frame, as a command recorded before the delete may
  // still use it.
  struct Mesh
  {
    GpuMesh gpu;
    int vertex_count;
    int index_count;
    size_t bytes;
    std::array<float, 6> bounds;  // in object space: minimum x, y, z, then maximum
    bool is_tile;                 // a tile set: M3D_TILE_VERTEX vertices, 32-bit indices
  };
  bool _gpu_meshes;
  bool _gpu_terrain;
  std::unordered_map<uint32_t, Mesh> _meshes;
  size_t _tile_set_count = 0;
  uint32_t add_mesh(Mesh mesh, const void *vertices, size_t vertexBytes, const void *indices, size_t indexBytes);
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
  void upload_terrain();

  // Dynamic shadows: a depth map per camera (split screen has two), fitted each frame to
  // the camera's casters in a light space anchored to the world, and drawn before the
  // scene. The terrain draws that take them sample it with a comparison sampler.
  struct ShadowCamera
  {
    bool is_set = false;      // setShadowCamera this frame
    bool has_map = false;     // its map holds this frame's casters
    M3D_SHADOW_CAMERA camera{};
    float view_to_light[12]{};   // 3x4 row-major: camera space to light space
    float light_to_clip[12]{};   // 3x4 row-major: light space to the map's clip space
    float filter_step = 0;       // between filter taps, in map texture coordinates
    float depth_bias = 0;        // in map depth
  };
  bool _gpu_shadows;
  std::array<ShadowCamera, max_shadow_cameras> _shadow_cameras;
  void render_shadow_maps();

  // game.ini [m3d] RenderStats: sums over the frames since the last report. The scene is
  // the game building it (beginScene to endScene: its CPU work, CPU mesh lighting
  // included), submit is endScene sending it to the backend, present is flip.
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
    uint64_t mirrored_mesh_draws = 0;
    uint64_t tile_draws = 0;
    uint64_t pose_vertices = 0;
    uint64_t shadow_maps = 0;
    uint64_t shadow_casters = 0;
    uint64_t vertices = 0;
    uint64_t indices = 0;
    size_t peak_vertices = 0;  // the largest frame
    size_t peak_indices = 0;
    unsigned terrain_uploads = 0;
    // The CPU-built geometry by what it draws (M3D_DRAW_COMMAND::source).
    std::array<uint64_t, static_cast<size_t>(eDrawSource::count)> source_vertices{};
    std::array<uint64_t, static_cast<size_t>(eDrawSource::count)> source_commands{};
  };
  bool _render_stats;
  Stats _stats;
  Clock::time_point _stats_start;
  Clock::time_point _scene_begin;
  std::string _stats_label;
  void report_stats(Clock::time_point now);
};

}  // namespace graphics

#endif  // MOONSHINE_RUNNERS_RENDERER_H
