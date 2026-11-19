#pragma once

#include <cstddef>
#include <cstdint>

#include "IGraph3d.h"

namespace graphics {

// The uniform blocks of the scene and shadow shaders. Both shader sets lay them out the
// same way (std140 in the sokol GLSL, cbuffers in the SDL GPU HLSL), because they hold
// only 16-byte vectors and column-major 4x4 matrices.
struct SceneVsParams {
  float projection_matrix[16];
  float view_to_shadow[16];
  float view_to_world[16];
};

struct MeshVsParams {
  float projection_matrix[16];
  float object_to_view[16];
  float object_to_world[16];
  float light[4];
  float view[4];
  float ambient[4];
  float multiply[4];
  float add[4];
  float metal[4];
  float flags[4];
  float terrain[4];
};

struct TileVsParams {
  float projection_matrix[16];
  float tile_to_view[16];
  float view_to_shadow[16];
  float view_to_world[16];
  float share[4];
};

struct SceneFsParams {
  int32_t color_operation_1;
  int32_t color_operation_2;
  int32_t alpha_test_enabled;
  int32_t alpha_reference;
};

struct SceneShadowParams {
  float shadow[4];
  float shadow_terrain[4];
};

struct ShadowVsParams {
  float object_to_shadow[16];
};

struct ShadowFsParams {
  float alpha_test[4];
};

// What a scene draw draws, which decides the shader and the vertex layout.
enum class Geometry {
  frame,          // the frame vertex and index arrays (scene shader)
  mesh,           // a GPU mesh (mesh shader)
  animated_mesh,  // a GPU mesh, positions and texture coordinates from the frame array
  tile,           // a GPU terrain tile (tile shader)
};

enum class BlendFactor { zero, one, source_alpha, one_minus_source_alpha };

// For the logs, the same in every backend.
inline const char* blend_factor_name(BlendFactor factor) {
  switch (factor) {
    case BlendFactor::zero:
      return "zero";
    case BlendFactor::one:
      return "one";
    case BlendFactor::source_alpha:
      return "src_alpha";
    case BlendFactor::one_minus_source_alpha:
      return "1-src_alpha";
  }
  return "?";
}

// The state a scene pipeline is made from. Everything else (culling) is the same for all.
struct PipelineKey {
  Geometry geometry;
  bool depth_test;
  bool depth_write;
  bool blend;
  BlendFactor source;
  BlendFactor destination;
  bool mirrored;  // a reflection (M3D_MESH_DRAW::mirrored): the front faces wind clockwise

  bool operator==(const PipelineKey& other) const {
    return geometry == other.geometry && depth_test == other.depth_test && depth_write == other.depth_write &&
           blend == other.blend && source == other.source && destination == other.destination &&
           mirrored == other.mirrored;
  }
};

// A rectangle of the render target, top-left origin, in game pixels.
struct Viewport {
  int x;
  int y;
  int width;
  int height;
};

// Texture, mesh and buffer handles a backend gives out; 0 is none.
using GpuTexture = uint32_t;
using GpuMesh = uint32_t;

// One texture's pixels: RGBA8, level 0 first, then each mip level.
struct TextureLevels {
  static constexpr int max_levels = 16;
  const uint8_t* pixels[max_levels];
  size_t sizes[max_levels];
  int count;
};

// One draw of the scene pass, everything resolved: the backend only binds and draws.
struct SceneDraw {
  PipelineKey pipeline;
  Viewport viewport;
  GpuTexture textures[2];   // 0: the empty texture
  bool repeat;              // the texture sampler wraps, or clamps
  GpuMesh mesh;             // 0: the frame arrays
  size_t pose_offset;       // an animated mesh: its pose's first vertex in the frame array
  uint32_t first_index;
  uint32_t index_count;
  bool terrain_maps;        // a mesh: bind the terrain maps for its shadow test
  int shadow_camera;        // the shadow map and cells to bind; -1 for none
  union {
    SceneVsParams scene;
    MeshVsParams mesh;
    TileVsParams tile;
  } vs;
  SceneFsParams fs;
  SceneShadowParams shadow;
};

// One shadow caster drawn into a shadow map.
struct ShadowDraw {
  GpuMesh mesh;
  bool animated;            // its pose is in the frame array, at pose_offset
  size_t pose_offset;
  uint32_t index_count;
  GpuTexture texture;       // for the alpha test
  bool repeat;
  ShadowVsParams vs;
  ShadowFsParams fs;
};

// What a graphics API does for the Renderer: the window, GPU resources and the passes of
// a frame. Renderer builds each frame and resolves every draw; a backend turns them into
// API calls. Implemented with sokol (OpenGL; sokol_backend.cpp) and SDL GPU (Metal,
// Direct3D 12, Vulkan; sdl_gpu/backend.cpp).
//
// A frame, from endScene() on: uploads (frame arrays, terrain maps, textures, the back
// buffer) can come any time outside a pass; then the shadow passes, the scene pass, and
// present() at flip().
class RenderBackend {
 public:
  virtual ~RenderBackend() = default;

  // The most GPU meshes and tile sets at once, and the shadow cameras (split screen has
  // two) with the size of their maps.
  static constexpr size_t max_meshes = 4096;
  static constexpr int max_shadow_cameras = 4;
  static constexpr int shadow_map_size = 2048;

  // What the backend could set up of what was asked for.
  struct Features {
    bool meshes;
    bool terrain;
    bool shadows;
  };
  virtual Features features() const = 0;
  virtual const char* name() const = 0;

  // The game's screen: the window size, and the offscreen buffer and back buffer, which
  // have the game's resolution. Returns how the game's screen maps onto the window.
  struct Scale {
    float x;
    float y;
  };
  virtual Scale setVideoMode(int width, int height, bool isFullScreen) = 0;

  // Textures. One created with levels is uploaded once; one created without is uploaded
  // with updateTexture, as often as the game changes it.
  virtual GpuTexture createTexture(int width, int height, int levelCount, const TextureLevels* levels) = 0;
  virtual void updateTexture(GpuTexture texture, const TextureLevels& levels) = 0;
  virtual void destroyTexture(GpuTexture texture) = 0;

  // A mesh or tile set: a vertex and an index buffer, uploaded once.
  virtual GpuMesh createMesh(const void* vertices, size_t vertexBytes, const void* indices, size_t indexBytes) = 0;
  virtual void destroyMesh(GpuMesh mesh) = 0;

  // The terrain's height and attribute maps, one byte a cell; made again at a new size.
  virtual void uploadTerrain(int width, int height, const uint8_t* heights, const uint8_t* attributes) = 0;

  // The 2D back buffer, RGB 565 at the game's resolution.
  virtual void uploadBackBuffer(const uint16_t* pixels) = 0;

  // The frame's CPU-built vertices and indices; the GPU buffers grow to fit.
  virtual void uploadFrame(const M3D_VERTEX* vertices, size_t vertexCount, const uint32_t* indices,
                           size_t indexCount) = 0;

  // A camera's shadow map: a depth-only pass.
  virtual void beginShadowPass(int camera) = 0;
  virtual void drawShadow(const ShadowDraw& draw) = 0;

  // The scene, into the offscreen buffer. clear: start from clearColor; otherwise from
  // what the buffer holds. The depth buffer is always cleared.
  virtual void beginScenePass(bool clear, const float (&clearColor)[4]) = 0;
  virtual void drawBackBuffer() = 0;
  virtual void draw(const SceneDraw& draw) = 0;

  // Ends a shadow or the scene pass.
  virtual void endPass() = 0;

  // The offscreen buffer to the window, and the frame is shown.
  virtual void present() = 0;
};

}  // namespace graphics
