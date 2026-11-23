//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <utility>

#include <SDL3/SDL.h>

#include "sokol_backend.h"
#include "texture_manager.h"
#ifdef GPU_RENDERER
#include "sdl_gpu/backend.h"
#endif
#include "xgraph.h"
#include "xtool.h"

#ifdef GPX
extern void frameReady();
#endif

#ifdef _WIN32
#undef far
#undef near
#endif

namespace graphics
{
  // clang-format off
  std::array<float, 16> matrix_make_rows(
    float m00, float m10, float m20, float m30,
    float m01, float m11, float m21, float m31,
    float m02, float m12, float m22, float m32,
    float m03, float m13, float m23, float m33) {
    return {
      m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m22, m23,
      m30, m31, m32, m33
    };
  }
  // clang-format on

  // clang-format off
  std::array<float, 16> make_ortho_projection(
    float left, float right,
    float bottom, float top,
    float near, float far) {
    return matrix_make_rows(
        2.0f / (right - left), 0, 0, -(left + right) / (right - left),
        0, 2.0f / (top - bottom), 0, -(top + bottom) / (top - bottom),
        0, 0, -2.0f / (far - near), -(far + near) / (far - near),
        0, 0, 0, 1.0f
    );
  }
  // clang-format on
}

namespace {
  template <typename Duration>
  double milliseconds(Duration duration) {
    return std::chrono::duration<double, std::milli>(duration).count();
  }

  // A 3x4 row-major object matrix (M3D_MESH_DRAW) as a shader mat4, which is column-major.
  void to_mat4(const float (&matrix)[12], float *out) {
    for (int column = 0; column < 4; column++) {
      for (int row = 0; row < 3; row++) {
        out[column * 4 + row] = matrix[row * 4 + column];
      }
      out[column * 4 + 3] = column == 3 ? 1.0f : 0.0f;
    }
  }

  // a * b for 3x4 row-major affine matrices: b's transform, then a's.
  void multiply_3x4(const float *a, const float *b, float *out) {
    for (int row = 0; row < 3; row++) {
      for (int column = 0; column < 4; column++) {
        float sum = column == 3 ? a[row * 4 + 3] : 0.0f;
        for (int k = 0; k < 3; k++) {
          sum += a[row * 4 + k] * b[k * 4 + column];
        }
        out[row * 4 + column] = sum;
      }
    }
  }

  void transform_3x4(const float *m, const float *point, float *out) {
    for (int row = 0; row < 3; row++) {
      out[row] = m[row * 4] * point[0] + m[row * 4 + 1] * point[1] + m[row * 4 + 2] * point[2] + m[row * 4 + 3];
    }
  }

  // The blend factor for one of the game's blend modes. Only the modes the game uses are
  // mapped; any other gets `fallback`, the default for that side.
  graphics::BlendFactor blend_factor(eBlendMode mode, graphics::BlendFactor fallback) {
    switch (mode) {
      case BLEND_SRCALPHA:
        return graphics::BlendFactor::source_alpha;
      case BLEND_INVSRCALPHA:
        return graphics::BlendFactor::one_minus_source_alpha;
      case BLEND_ONE:
        return graphics::BlendFactor::one;
      default:
        return fallback;
    }
  }
}

using namespace graphics;

Renderer::Renderer(int width, int height, bool isFullScreen, bool sdlGpu, bool sdlGpuDebug, bool textureMipmaps,
                   bool gpuMeshes, bool gpuShadows, bool gpuTerrain, bool renderStats)
    : _gpu_meshes(gpuMeshes), _gpu_terrain(gpuTerrain), _gpu_shadows(gpuMeshes && gpuShadows),
      _render_stats(renderStats), _stats_start(Clock::now()), _scene_begin(_stats_start) {
  const RenderBackend::Features wanted{_gpu_meshes, _gpu_terrain, _gpu_shadows};
#ifdef GPU_RENDERER
  if (sdlGpu) {
    _backend = sdl_gpu::Backend::create(width, height, wanted, sdlGpuDebug);
    if (_backend != nullptr && !_backend->preparePipelines(pipeline_keys(_backend->features()))) {
      _backend = nullptr;  // and its window with it
    }
    if (_backend == nullptr) {
      SDL_Log("Renderer: SDL GPU is not available here, drawing with OpenGL");
    }
  }
#else
  if (sdlGpu) {
    SDL_Log("Renderer: built without SDL GPU, drawing with OpenGL");
  }
#endif
  if (_backend == nullptr) {
    _backend = std::make_unique<SokolBackend>(width, height, wanted);
  }
  // What the backend could set up: shaders that failed, or a build without them.
  const auto features = _backend->features();
  _gpu_meshes = features.meshes;
  _gpu_terrain = features.terrain;
  _gpu_shadows = features.shadows;
  SDL_Log("Renderer: %s, GPU meshes %s, GPU shadows %s, GPU terrain %s", _backend->name(),
          _gpu_meshes ? "on" : "off", _gpu_shadows ? "on" : "off", _gpu_terrain ? "on" : "off");

  _vertex_buffer.resize(initial_vertex_count);
  _index_buffer.resize(initial_index_count);

  _texture_manager = std::make_unique<TextureManager>(*_backend, textureMipmaps);
  setVideoMode(width, height, isFullScreen);
}

Renderer::~Renderer() {
  // The textures go back to the backend before it shuts down.
  _texture_manager = nullptr;
  _backend = nullptr;
}

TextureManager& Renderer::get_texture_manager() { return *_texture_manager; }

void Renderer::setVideoMode(int width, int height, bool isFullScreen) {
  const auto scale = _backend->setVideoMode(width, height, isFullScreen);
  XGR_WindowScaleX = scale.x;
  XGR_WindowScaleY = scale.y;

  _width = width;
  _height = height;
  _projectionMatrix = make_ortho_projection(0, width, height, 0, -1, 1);

  // The backend's back buffer is new: it gets these pixels at the next flush.
  _back_buffer.assign(static_cast<size_t>(width) * height, 0);
  _is_back_buffer_changed = true;
}

bool Renderer::isInScene() const
{
  return _is_in_scene;
}

MD3DERROR Renderer::flip(bool WaitVerticalBlank) {
  const auto present_start = Clock::now();
  _clear = true;
  FlushFsParams flush{};
  for (int i = 0; i < 3; i++) {
    flush.gamma[i] = 1.0f / _gamma[i];
    if (_gamma[i] != 1.0f) {
      flush.gamma[3] = 1.0f;
    }
  }
  _backend->present(flush);
#ifdef GPX
  frameReady();
#endif
  if (_render_stats) {
    const auto now = Clock::now();
    _stats.present_ms += milliseconds(now - present_start);
    _stats.frames++;
    if (now - _stats_start >= std::chrono::seconds(5)) {
      report_stats(now);
    }
  }
  return MD3D_OK;
}

void Renderer::report_stats(Clock::time_point now) {
  if (_stats.frames > 1) {
    const double frames = _stats.frames;
    const double frame_ms = milliseconds(now - _stats_start) / frames;
    const double scene_ms = _stats.scene_ms / frames;
    const double submit_ms = _stats.submit_ms / frames;
    const double present_ms = _stats.present_ms / frames;
    // The frame vertex array holds the CPU-built vertices and the animated meshes' poses.
    const char *label = _stats_label.c_str();
    SDL_Log("Renderer stats [%s]: %.1f ms/frame = scene %.2f + submit %.2f + present %.2f + other %.2f; "
            "per frame %.0f draws (%.0f GPU meshes, %.0f of them animated and %.0f reflected, %.0f GPU tiles), "
            "%.0f vertices and %.0f indices from the CPU, %.0f pose vertices, %.1f shadow maps (%.0f casters); "
            "%u GPU meshes and %u tile sets (%u KiB), %u terrain uploads; largest frame %u vertices and %u indices "
            "(arrays %u and %u)",
            label, frame_ms, scene_ms, submit_ms, present_ms, frame_ms - scene_ms - submit_ms - present_ms,
            _stats.draws / frames, _stats.mesh_draws / frames, _stats.animated_mesh_draws / frames,
            _stats.mirrored_mesh_draws / frames, _stats.tile_draws / frames,
            (_stats.vertices - _stats.pose_vertices) / frames,
            _stats.indices / frames, _stats.pose_vertices / frames, _stats.shadow_maps / frames,
            _stats.shadow_casters / frames, static_cast<unsigned>(_meshes.size() - _tile_set_count),
            static_cast<unsigned>(_tile_set_count), static_cast<unsigned>(_mesh_bytes / 1024), _stats.terrain_uploads,
            static_cast<unsigned>(_stats.peak_vertices), static_cast<unsigned>(_stats.peak_indices),
            static_cast<unsigned>(_vertex_buffer.size()), static_cast<unsigned>(_index_buffer.size()));

    // Vertices (and draw commands) per frame, by what the game draws with them.
    const auto per_frame = [&](eDrawSource source, const decltype(Stats::source_vertices) &counts) {
      return counts[static_cast<size_t>(source)] / frames;
    };
    const auto &v = _stats.source_vertices;
    const auto &c = _stats.source_commands;
    SDL_Log("Renderer stats [%s]: CPU vertices per frame (commands): static meshes %.0f (%.0f), animated meshes "
            "%.0f (%.0f), reflected meshes %.0f (%.0f), terrain %.0f (%.0f), reflected terrain %.0f (%.0f), "
            "water %.0f (%.0f), sky %.0f (%.0f), effects %.0f (%.0f), other %.0f (%.0f)",
            label, per_frame(eDrawSource::mesh_static, v), per_frame(eDrawSource::mesh_static, c),
            per_frame(eDrawSource::mesh_animated, v), per_frame(eDrawSource::mesh_animated, c),
            per_frame(eDrawSource::mesh_reflection, v), per_frame(eDrawSource::mesh_reflection, c),
            per_frame(eDrawSource::terrain, v), per_frame(eDrawSource::terrain, c),
            per_frame(eDrawSource::terrain_reflection, v), per_frame(eDrawSource::terrain_reflection, c),
            per_frame(eDrawSource::water, v), per_frame(eDrawSource::water, c),
            per_frame(eDrawSource::sky, v), per_frame(eDrawSource::sky, c),
            per_frame(eDrawSource::effects, v), per_frame(eDrawSource::effects, c),
            per_frame(eDrawSource::other, v), per_frame(eDrawSource::other, c));
  }
  _stats = Stats{};
  _stats_start = now;
}

MD3DERROR Renderer::setClipRect(const MD3DRECT &lprcClipRect) {
  _render_state.viewport = lprcClipRect;
  return MD3D_OK;
}

MD3DERROR Renderer::resetClipRect() {
  _render_state.viewport = std::nullopt;
  return MD3D_OK;
}

MD3DERROR Renderer::beginScene() {
  _scene_begin = Clock::now();
  _commands.clear();
  _render_state = RenderState();
  _texture_manager->delete_textures();
  _is_in_scene = true;
  return MD3D_OK;
}

MD3DERROR Renderer::endScene() {
  _is_in_scene = false;
  const auto submit_start = Clock::now();
  _stats.scene_ms += milliseconds(submit_start - _scene_begin);

  if (_commands.empty()) {
    destroy_deleted_meshes();
    for (auto &shadow : _shadow_cameras) {
      shadow.is_set = false;
    }
    return MD3D_OK;
  }

  // Everything the frame uploads goes before its passes: SDL GPU copies outside them.
  // Of the frame arrays, only what this frame used.
  const size_t vertex_count = _commands.back().vertex_buffer_view.end();
  const size_t index_count = _commands.back().index_buffer_view.end();
  _stats.vertices += vertex_count;
  _stats.indices += index_count;
  _stats.peak_vertices = std::max(_stats.peak_vertices, vertex_count);
  _stats.peak_indices = std::max(_stats.peak_indices, index_count);
  _backend->uploadFrame(_vertex_buffer.data(), vertex_count, _index_buffer.data(), index_count);
  // The shadow receivers skip the terrain's static shadow cells, read from its map.
  for (const auto &shadow : _shadow_cameras) {
    if (shadow.is_set && _has_terrain) {
      _is_terrain_needed = true;
    }
  }
  if (_is_terrain_needed) {
    upload_terrain();
    _is_terrain_needed = false;
  }
  if (_is_back_buffer_flush && _is_back_buffer_changed) {
    _backend->uploadBackBuffer(_back_buffer.data());
    _is_back_buffer_changed = false;
  }
  for (const auto &command : _commands) {
    resolve_textures(command.render_state);
  }

  if (_gpu_shadows) {
    render_shadow_maps();
  }

  _backend->beginScenePass(_clear, _clear_color);

  if (_is_back_buffer_flush) {
    _backend->drawBackBuffer();
    _is_back_buffer_flush = false;
  }

  for (const auto& command : _commands) {
    if (command.is_tile) {
      draw_tile_command(command);
      _stats.draws++;
      _stats.tile_draws++;
      continue;
    }
    if (command.mesh != 0) {
      draw_mesh_command(command);
      _stats.draws++;
      _stats.mesh_draws++;
      if (command.is_animated) {
        _stats.animated_mesh_draws++;
        _stats.pose_vertices += command.vertex_buffer_view.length;
      }
      if (command.mesh_draw.mirrored) {
        _stats.mirrored_mesh_draws++;
      }
      continue;
    }
    if (command.index_buffer_view.length == 0) {
      continue;
    }
    _stats.draws++;
    draw_frame_command(command);
  }

  _backend->endPass();
  destroy_deleted_meshes();
  // The game sets its shadow cameras again for the next frame.
  for (auto &shadow : _shadow_cameras) {
    shadow.is_set = false;
    shadow.has_map = false;
  }
  _stats.submit_ms += milliseconds(Clock::now() - submit_start);

  // A second scene before flip() draws over this one.
  _clear = false;

  return MD3D_OK;
}

void Renderer::copy_projection(const RenderState &state, float *matrix) const {
  const auto &m = state.projection_matrix;
  if (m) {
    std::copy(&m->_11, &m->_11 + 16, matrix);
  } else {
    std::copy(_projectionMatrix.begin(), _projectionMatrix.end(), matrix);
  }
}

Viewport Renderer::viewport(const RenderState &state) const {
  // Graph3d's clip rectangles are left, top, width and height (NullClipRect), whatever
  // MD3DRECT's names say.
  const auto &viewport = state.viewport;
  if (viewport) {
    return {viewport->left, viewport->top, viewport->right, viewport->bottom};
  }
  return {0, 0, _width, _height};
}

PipelineKey Renderer::pipeline_key(const RenderState &state, Geometry geometry, bool mirrored) const {
  // RENDERSTATE_ZTEST was D3D's ZENABLE: turned off, the depth buffer is neither tested
  // nor written, whatever RENDERSTATE_ZWRITE says.
  const bool depth_test = state.is_depth_test_enabled;
  PipelineKey key{geometry,
                  depth_test,
                  depth_test && state.is_depth_write_enabled,
                  state.is_alpha_blend_enabled,
                  BlendFactor::one,
                  BlendFactor::zero,
                  mirrored};
  if (key.blend) {
    key.source = blend_factor(state.source_blend_mode, BlendFactor::one);
    key.destination = blend_factor(state.destination_blend_mode, BlendFactor::zero);
  }
  return key;
}

std::vector<PipelineKey> Renderer::pipeline_keys(const RenderBackend::Features &features) const {
  // The game blends only through setMaterial: alpha, or additive (MAT_COLOR_ADD_DIFFUSE).
  // No game code sets RENDERSTATE_SRCBLEND or DESTBLEND; a pipeline with other factors
  // would be made at its first draw.
  const std::pair<eBlendMode, eBlendMode> blends[] = {{BLEND_SRCALPHA, BLEND_INVSRCALPHA}, {BLEND_ONE, BLEND_ONE}};
  std::vector<PipelineKey> keys;
  const auto add = [&keys](const PipelineKey &key) {
    if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
      keys.push_back(key);
    }
  };
  for (const Geometry geometry : {Geometry::frame, Geometry::mesh, Geometry::animated_mesh, Geometry::tile}) {
    const bool is_mesh = geometry == Geometry::mesh || geometry == Geometry::animated_mesh;
    if ((is_mesh && !features.meshes) || (geometry == Geometry::tile && !features.terrain)) {
      continue;
    }
    // Depth test, depth write, blending, and which blending: pipeline_key() folds the
    // combinations that make the same pipeline into one.
    for (int bits = 0; bits < 16; bits++) {
      RenderState state;
      state.is_depth_test_enabled = (bits & 1) != 0;
      state.is_depth_write_enabled = (bits & 2) != 0;
      state.is_alpha_blend_enabled = (bits & 4) != 0;
      state.source_blend_mode = blends[(bits >> 3) & 1].first;
      state.destination_blend_mode = blends[(bits >> 3) & 1].second;
      add(pipeline_key(state, geometry));
      if (is_mesh) {
        add(pipeline_key(state, geometry, true));  // a reflection
      }
    }
  }
  return keys;
}

void Renderer::resolve_textures(const RenderState &state) {
  for (const auto &texture : state.textures) {
    if (texture) {
      _texture_manager->get(*texture);
    }
  }
}

void Renderer::fill_material(const RenderState &state, SceneDraw &draw) {
  for (uint32_t i = 0; i < 2; i++) {
    const auto &texture = state.textures[i];
    draw.textures[i] = texture ? _texture_manager->get(*texture) : 0;
  }
  draw.repeat = state.texture_address != TADDRESS_CLAMP;

  draw.fs.color_operation_1 = state.color_operation1;
  draw.fs.color_operation_2 = state.color_operation2;
  draw.fs.alpha_test_enabled = state.is_alpha_test_enabled;
  draw.fs.alpha_reference = state.alpha_reference;
}

void Renderer::fill_shadow(int camera, SceneDraw &draw) const {
  draw.shadow_camera = camera;
  draw.shadow = SceneShadowParams{};
  if (camera >= 0) {
    const auto &shadow = _shadow_cameras[camera];
    draw.shadow.shadow[0] = 1.0f;
    draw.shadow.shadow[1] = shadow.camera.darkness;
    draw.shadow.shadow[2] = shadow.filter_step;
    draw.shadow.shadow[3] = shadow.depth_bias;
    draw.shadow.shadow_terrain[0] = static_cast<float>(_terrain.worldMaskX);
    draw.shadow.shadow_terrain[1] = static_cast<float>(_terrain.worldMaskY);
    draw.shadow.shadow_terrain[2] = _is_terrain_uploaded ? 1.0f : 0.0f;
  }
}

MD3DERROR Renderer::setProjectionMatrix(const D3DMATRIX &matrix) {
  _render_state.projection_matrix = matrix;
  return MD3D_OK;
}

MD3DERROR Renderer::resetProjectionMatrix() {
  _render_state.projection_matrix = std::nullopt;
  return MD3D_OK;
}

MD3DERROR Renderer::clear(uint32_t dwColor) {
  _clear = true;
  _clear_color[0] = static_cast<float>((dwColor >> 16) & 0xFF) / 255.0f;
  _clear_color[1] = static_cast<float>((dwColor >> 8) & 0xFF) / 255.0f;
  _clear_color[2] = static_cast<float>(dwColor & 0xFF) / 255.0f;
  _clear_color[3] = 1.0f;
  return MD3D_OK;
}

MD3DERROR Renderer::setRenderState(eRenderStateOption option, int value) {
  switch (option) {
    case RENDERSTATE_ZTEST:
      _render_state.is_depth_test_enabled = value == 1;
      break;

    case RENDERSTATE_ZWRITE:
      _render_state.is_depth_write_enabled = value == 1;
      break;

    case RENDERSTATE_ALPHATEST:
      _render_state.is_alpha_test_enabled = value == 1;
      break;

    case RENDERSTATE_ALPHAREF:
      _render_state.alpha_reference = value;
      break;

    case RENDERSTATE_ALPHABLEND:
      _render_state.is_alpha_blend_enabled = value == 1;
      break;

    case RENDERSTATE_SRCBLEND:
      _render_state.source_blend_mode = static_cast<eBlendMode>(value);
      break;

    case RENDERSTATE_DESTBLEND:
      _render_state.destination_blend_mode = static_cast<eBlendMode>(value);
      break;

    case RENDERSTATE_TEXTUREADDRESS:
      _render_state.texture_address = static_cast<eRenderStateTextureAddress>(value);
      break;

    default:
      break;
  }
  return MD3D_OK;
}

MD3DERROR Renderer::setMaterial(eMaterialMode material) {
  _render_state.is_alpha_blend_enabled = material & (MAT_ALPHA_MOD_TEXTURE1 | MAT_ALPHA_MOD_DIFFUSE);
  _render_state.is_alpha_test_enabled = material & MAT_ALPHA_MASK_TEXTURE1;

  if (material & MAT_COLOR_ADD_DIFFUSE) {
    _render_state.source_blend_mode = BLEND_ONE;
    _render_state.destination_blend_mode = BLEND_ONE;
  } else {
    _render_state.source_blend_mode = BLEND_SRCALPHA;
    _render_state.destination_blend_mode = BLEND_INVSRCALPHA;
  }

  switch (material & (MAT_COLOR_MOD_DIFFUSE | MAT_COLOR_MOD_TEXTURE1)) {
    case MAT_COLOR_MOD_DIFFUSE:
      _render_state.color_operation1 = 0;
      break;
    case MAT_COLOR_MOD_TEXTURE1:
      _render_state.color_operation1 = 1;
      break;
    case MAT_COLOR_MOD_DIFFUSE_TEXTURE1:
      _render_state.color_operation1 = 2;
      break;
    case MAT_NULL:
      break;
    default:
      assert(0);
  }

  _render_state.color_operation2 = material & MAT_COLOR_MOD_TEXTURE2 ? 2 : 0;

  return MD3D_OK;
}

MD3DERROR Renderer::setTexture(uint32_t dwHandle, uint32_t dwStage) {
  assert(dwStage < 2);
  _render_state.textures[dwStage] = dwHandle;
  return MD3D_OK;
}

MD3DERROR Renderer::beginDrawCommand(M3D_DRAW_COMMAND &command) {
  // A command starts after the last one that was ended, so whatever a command that was
  // begun but never ended wrote gets overwritten.
  const size_t vertex_offset = _commands.empty() ? 0 : _commands.back().vertex_buffer_view.end();
  const size_t index_offset = _commands.empty() ? 0 : _commands.back().index_buffer_view.end();

  command.positionBuffer.attach(_vertex_buffer, vertex_offset);
  command.diffuseColorBuffer.attach(_vertex_buffer, vertex_offset);
  command.specularColorBuffer.attach(_vertex_buffer, vertex_offset);
  command.uvBuffer.attach(_vertex_buffer, vertex_offset);
  command.indexBuffer.attach(_index_buffer, index_offset);

  return MD3D_OK;
}

MD3DERROR Renderer::endDrawCommand(const M3D_DRAW_COMMAND &command) {
  // Part of the render state, so geometry that takes shadows never merges with any other.
  const bool takes_shadow = _gpu_shadows && command.shadowCamera >= 0 && command.shadowCamera < max_shadow_cameras;
  _render_state.shadow_camera = takes_shadow ? command.shadowCamera : -1;
  prepare_render_state();

  // The game numbers vertices from 0 within a command; make the indices refer to
  // the frame arrays.
  auto &last_command = _commands.back();
  const auto first_vertex = static_cast<uint32_t>(last_command.vertex_buffer_view.end());
  uint32_t *indices = _index_buffer.data() + command.indexBuffer.offset;
  for (size_t i = 0; i < command.indexBuffer.count; i++) {
    indices[i] += first_vertex;
  }

  last_command.vertex_buffer_view.length += command.positionBuffer.count;
  last_command.index_buffer_view.length += command.indexBuffer.count;

  const auto source = static_cast<size_t>(command.source);
  _stats.source_vertices[source] += command.positionBuffer.count;
  _stats.source_commands[source]++;

  return MD3D_OK;
}

uint32_t Renderer::createMesh(const M3D_MESH_VERTEX *vertices, int vertexCount, const uint16_t *indices,
                              int indexCount) {
  if (!_gpu_meshes || vertexCount <= 0 || indexCount <= 0 || vertexCount > 65536) {
    return 0;
  }

  Mesh mesh{};
  mesh.vertex_count = vertexCount;
  mesh.index_count = indexCount;
  mesh.bounds = {vertices[0].x, vertices[0].y, vertices[0].z, vertices[0].x, vertices[0].y, vertices[0].z};
  for (int i = 1; i < vertexCount; i++) {
    const float position[3] = {vertices[i].x, vertices[i].y, vertices[i].z};
    for (int axis = 0; axis < 3; axis++) {
      mesh.bounds[axis] = std::min(mesh.bounds[axis], position[axis]);
      mesh.bounds[axis + 3] = std::max(mesh.bounds[axis + 3], position[axis]);
    }
  }
  return add_mesh(mesh, vertices, vertexCount * sizeof(M3D_MESH_VERTEX), indices, indexCount * sizeof(uint16_t));
}

uint32_t Renderer::createTiles(const M3D_TILE_VERTEX *vertices, int vertexCount, const uint32_t *indices,
                               int indexCount) {
  if (!_gpu_terrain || vertexCount <= 0 || indexCount <= 0) {
    return 0;
  }

  Mesh mesh{};
  mesh.vertex_count = vertexCount;
  mesh.index_count = indexCount;
  mesh.is_tile = true;
  const uint32_t handle =
      add_mesh(mesh, vertices, vertexCount * sizeof(M3D_TILE_VERTEX), indices, indexCount * sizeof(uint32_t));
  if (handle != 0) {
    _tile_set_count++;
    SDL_Log("Renderer: terrain tile set, %d vertices and %d indices (%u KiB)", vertexCount, indexCount,
            static_cast<unsigned>((vertexCount * sizeof(M3D_TILE_VERTEX) + indexCount * sizeof(uint32_t)) / 1024));
  }
  return handle;
}

uint32_t Renderer::add_mesh(Mesh mesh, const void *vertices, size_t vertexBytes, const void *indices,
                            size_t indexBytes) {
  if (_meshes.size() >= max_meshes) {
    if (!_is_mesh_pool_full_logged) {
      SDL_Log("Renderer: %u GPU meshes and tiles, the most there can be; more are drawn on the CPU",
              static_cast<unsigned>(_meshes.size()));
      _is_mesh_pool_full_logged = true;
    }
    return 0;
  }

  mesh.bytes = vertexBytes + indexBytes;
  mesh.gpu = _backend->createMesh(vertices, vertexBytes, indices, indexBytes);
  if (mesh.gpu == 0) {
    return 0;
  }

  const uint32_t handle = ++_last_mesh;
  _meshes.emplace(handle, mesh);
  _mesh_bytes += mesh.bytes;
  if (_meshes.size() % 256 == 0) {
    SDL_Log("Renderer: %u GPU meshes and tiles, %u KiB", static_cast<unsigned>(_meshes.size()),
            static_cast<unsigned>(_mesh_bytes / 1024));
  }
  return handle;
}

void Renderer::deleteMesh(uint32_t handle) {
  if (_meshes.count(handle) != 0) {
    _deleted_meshes.push_back(handle);
  }
}

void Renderer::destroy_deleted_meshes() {
  for (const auto handle : _deleted_meshes) {
    const auto mesh = _meshes.find(handle);
    if (mesh == _meshes.end()) {
      continue;
    }
    _backend->destroyMesh(mesh->second.gpu);
    _mesh_bytes -= mesh->second.bytes;
    if (mesh->second.is_tile) {
      _tile_set_count--;
    }
    _meshes.erase(mesh);
  }
  _deleted_meshes.clear();
}

void Renderer::drawMesh(uint32_t handle, const M3D_MESH_DRAW &draw, const M3D_MESH_POSE *pose) {
  const auto mesh = _meshes.find(handle);
  if (mesh == _meshes.end() || mesh->second.is_tile) {
    return;
  }

  // Recorded like frame geometry, so it is drawn in order with it. Its buffer views carry
  // the frame arrays' positions on to the next command; an animated mesh's pose takes
  // vertices of the frame array, uploaded with the rest of the frame.
  BufferView vertices = _commands.empty() ? BufferView{0, 0} : _commands.back().vertex_buffer_view.next();
  const BufferView indices = _commands.empty() ? BufferView{0, 0} : _commands.back().index_buffer_view.next();
  if (pose != nullptr) {
    const size_t count = mesh->second.vertex_count;
    if (_vertex_buffer.size() < vertices.offset + count) {
      _vertex_buffer.resize(std::max(2 * _vertex_buffer.size(), vertices.offset + count));
    }
    M3D_VERTEX *out = _vertex_buffer.data() + vertices.offset;
    for (size_t i = 0; i < count; i++) {
      out[i].x = pose[i].x;
      out[i].y = pose[i].y;
      out[i].z = pose[i].z;
      out[i].u = pose[i].u;
      out[i].v = pose[i].v;
    }
    vertices.length = count;
  }
  DrawCommand command{_render_state, vertices, indices};
  command.mesh = handle;
  command.is_animated = pose != nullptr;
  command.mesh_draw = draw;
  // A reflection never casts: the shadow pass would draw it mirrored, below the ground.
  if (!_gpu_shadows || draw.mirrored || draw.shadowCamera < 0 || draw.shadowCamera >= max_shadow_cameras) {
    command.mesh_draw.shadowCamera = -1;
  } else if (pose != nullptr) {
    // A shadow caster's box, which the shadow map is fitted to: this frame's pose.
    command.bounds = {pose[0].x, pose[0].y, pose[0].z, pose[0].x, pose[0].y, pose[0].z};
    for (int i = 1; i < mesh->second.vertex_count; i++) {
      const float position[3] = {pose[i].x, pose[i].y, pose[i].z};
      for (int axis = 0; axis < 3; axis++) {
        command.bounds[axis] = std::min(command.bounds[axis], position[axis]);
        command.bounds[axis + 3] = std::max(command.bounds[axis + 3], position[axis]);
      }
    }
  } else {
    command.bounds = mesh->second.bounds;
  }
  if (!_has_terrain) {
    command.mesh_draw.renderShade = 0;
  }
  if (command.mesh_draw.renderShade) {
    _is_terrain_needed = true;
  }
  _commands.push_back(command);
}

void Renderer::drawTile(uint32_t handle, const M3D_TILE_DRAW &draw) {
  const auto mesh = _meshes.find(handle);
  if (mesh == _meshes.end() || !mesh->second.is_tile || draw.firstIndex < 0 || draw.indexCount <= 0 ||
      draw.firstIndex + draw.indexCount > mesh->second.index_count) {
    return;
  }

  // Recorded like a GPU mesh, in order with the rest; it takes nothing from the frame arrays.
  const BufferView vertices = _commands.empty() ? BufferView{0, 0} : _commands.back().vertex_buffer_view.next();
  const BufferView indices = _commands.empty() ? BufferView{0, 0} : _commands.back().index_buffer_view.next();
  DrawCommand command{_render_state, vertices, indices};
  command.mesh = handle;
  command.is_tile = true;
  command.tile_draw = draw;
  command.mesh_draw.shadowCamera = -1;  // a receiver, never a caster
  if (!_gpu_shadows || draw.shadowCamera < 0 || draw.shadowCamera >= max_shadow_cameras) {
    command.tile_draw.shadowCamera = -1;
  }
  _commands.push_back(command);
}

void Renderer::setStatsLabel(const char *label) {
  const std::string text = label != nullptr ? label : "";
  if (text == _stats_label) {
    return;
  }
  if (_render_stats) {
    report_stats(Clock::now());
  }
  _stats_label = text;
}

void Renderer::setTerrain(const M3D_TERRAIN &terrain) {
  _terrain = terrain;
  _has_terrain = terrain.heights != nullptr && terrain.attributes != nullptr && terrain.width > 0 &&
                 terrain.height > 0;
}

bool Renderer::setShadowCamera(int camera, const M3D_SHADOW_CAMERA &shadow) {
  if (!_gpu_shadows || camera < 0 || camera >= max_shadow_cameras) {
    return false;
  }
  _shadow_cameras[camera].is_set = true;
  _shadow_cameras[camera].camera = shadow;
  return true;
}

void Renderer::setGamma(float red, float green, float blue) {
  // The original's range (d3dSetAdjustedGamma); it also keeps 1 / gamma finite.
  _gamma[0] = std::clamp(red, 0.2f, 5.0f);
  _gamma[1] = std::clamp(green, 0.2f, 5.0f);
  _gamma[2] = std::clamp(blue, 0.2f, 5.0f);
}

void Renderer::getGamma(float &red, float &green, float &blue) const {
  red = _gamma[0];
  green = _gamma[1];
  blue = _gamma[2];
}

void Renderer::upload_terrain() {
  const size_t size = static_cast<size_t>(_terrain.width) * _terrain.height;

  // A map of another size (another world): an upload whatever it holds.
  bool changed = false;
  if (_terrain_width != _terrain.width || _terrain_height != _terrain.height) {
    _terrain_width = _terrain.width;
    _terrain_height = _terrain.height;
    _terrain_heights_copy.assign(size, 0);
    _terrain_attributes_copy.assign(size, 0);
    changed = true;
    SDL_Log("Renderer: terrain maps %dx%d for the mesh shadow test", _terrain.width, _terrain.height);
  }

  // The game changes the maps in place (digging, deformation): compare, which costs far
  // less than uploading every frame.
  if (changed || std::memcmp(_terrain_heights_copy.data(), _terrain.heights, size) != 0 ||
      std::memcmp(_terrain_attributes_copy.data(), _terrain.attributes, size) != 0) {
    std::memcpy(_terrain_heights_copy.data(), _terrain.heights, size);
    std::memcpy(_terrain_attributes_copy.data(), _terrain.attributes, size);
    _backend->uploadTerrain(_terrain_width, _terrain_height, _terrain_heights_copy.data(),
                            _terrain_attributes_copy.data());
    _is_terrain_uploaded = true;
    _stats.terrain_uploads++;
  }
}

void Renderer::draw_frame_command(const DrawCommand &command) {
  SceneDraw draw{};
  draw.pipeline = pipeline_key(command.render_state, Geometry::frame);
  draw.viewport = viewport(command.render_state);
  fill_material(command.render_state, draw);
  draw.first_index = static_cast<uint32_t>(command.index_buffer_view.offset);
  draw.index_count = static_cast<uint32_t>(command.index_buffer_view.length);

  auto &vs_params = draw.vs.scene;
  vs_params = SceneVsParams{};
  copy_projection(command.render_state, vs_params.projection_matrix);
  const int shadow_camera = command.render_state.shadow_camera;
  const bool takes_shadow = shadow_camera >= 0 && _shadow_cameras[shadow_camera].has_map;
  if (takes_shadow) {
    const auto &shadow = _shadow_cameras[shadow_camera];
    float view_to_clip[12];
    multiply_3x4(shadow.light_to_clip, shadow.view_to_light, view_to_clip);
    to_mat4(view_to_clip, vs_params.view_to_shadow);
    to_mat4(shadow.camera.viewToWorld, vs_params.view_to_world);
  }
  fill_shadow(takes_shadow ? shadow_camera : -1, draw);
  _backend->draw(draw);
}

void Renderer::draw_mesh_command(const DrawCommand &command) {
  const auto mesh = _meshes.find(command.mesh);
  if (mesh == _meshes.end()) {
    return;
  }
  const auto &draw = command.mesh_draw;
  const bool shade = draw.renderShade && _is_terrain_uploaded;

  SceneDraw scene{};
  scene.pipeline = pipeline_key(command.render_state, command.is_animated ? Geometry::animated_mesh : Geometry::mesh,
                                draw.mirrored != 0);
  scene.viewport = viewport(command.render_state);
  fill_material(command.render_state, scene);
  scene.mesh = mesh->second.gpu;
  scene.pose_offset = command.vertex_buffer_view.offset;
  scene.first_index = 0;
  scene.index_count = static_cast<uint32_t>(mesh->second.index_count);
  scene.terrain_maps = shade;

  auto &vs_params = scene.vs.mesh;
  vs_params = MeshVsParams{};
  copy_projection(command.render_state, vs_params.projection_matrix);
  to_mat4(draw.objectToView, vs_params.object_to_view);
  to_mat4(draw.objectToWorld, vs_params.object_to_world);
  // The integers go as floats (exact for these small whole numbers): an all-float block
  // is one uniform array in GL, set with a single call.
  const auto as_float = [](int value) { return static_cast<float>(value); };
  for (int i = 0; i < 3; i++) {
    vs_params.light[i] = draw.light[i];
    vs_params.view[i] = draw.view[i];
    vs_params.ambient[i] = as_float(draw.ambient[i]);
    vs_params.add[i] = as_float(draw.add[i]);
    vs_params.metal[i] = as_float(draw.metal[i]);
  }
  vs_params.light[3] = draw.cosLightView;
  for (int i = 0; i < 4; i++) {
    vs_params.multiply[i] = as_float(draw.multiply[i]);
  }
  vs_params.flags[0] = as_float(draw.renderDiffuse);
  vs_params.flags[1] = as_float(draw.renderMetal);
  vs_params.flags[2] = shade ? 1.0f : 0.0f;
  vs_params.flags[3] = as_float(draw.clampNearZ);
  vs_params.terrain[0] = as_float(_terrain.worldMaskX);
  vs_params.terrain[1] = as_float(_terrain.worldMaskY);
  vs_params.terrain[2] = as_float(_terrain.heightShift);
  vs_params.terrain[3] = as_float(_terrain.shadeDepth);

  // Meshes take no dynamic shadows; scene_fs still needs its shadow inputs.
  fill_shadow(-1, scene);
  _backend->draw(scene);
}

void Renderer::draw_tile_command(const DrawCommand &command) {
  const auto mesh = _meshes.find(command.mesh);
  if (mesh == _meshes.end()) {
    return;
  }
  const auto &draw = command.tile_draw;

  SceneDraw scene{};
  scene.pipeline = pipeline_key(command.render_state, Geometry::tile);
  scene.viewport = viewport(command.render_state);
  fill_material(command.render_state, scene);
  scene.mesh = mesh->second.gpu;
  scene.first_index = static_cast<uint32_t>(draw.firstIndex);
  scene.index_count = static_cast<uint32_t>(draw.indexCount);

  auto &vs_params = scene.vs.tile;
  vs_params = TileVsParams{};
  copy_projection(command.render_state, vs_params.projection_matrix);
  to_mat4(draw.tileToView, vs_params.tile_to_view);
  const int shadow_camera = draw.shadowCamera;
  const bool takes_shadow = shadow_camera >= 0 && _shadow_cameras[shadow_camera].has_map;
  if (takes_shadow) {
    const auto &shadow = _shadow_cameras[shadow_camera];
    float view_to_clip[12];
    multiply_3x4(shadow.light_to_clip, shadow.view_to_light, view_to_clip);
    to_mat4(view_to_clip, vs_params.view_to_shadow);
    to_mat4(shadow.camera.viewToWorld, vs_params.view_to_world);
  }
  // Whole numbers, exact as floats. The set's buffers are bound at offset 0 for every
  // tile, so from one tile to the next only textures and uniforms change.
  vs_params.share[0] = static_cast<float>(draw.shareOffset[0]);
  vs_params.share[1] = static_cast<float>(draw.shareOffset[1]);
  vs_params.share[2] = static_cast<float>(draw.shareRadius);
  vs_params.share[3] = draw.clampNearZ ? 1.0f : 0.0f;

  fill_shadow(takes_shadow ? shadow_camera : -1, scene);
  _backend->draw(scene);
}

void Renderer::render_shadow_maps() {
  for (int index = 0; index < max_shadow_cameras; index++) {
    ShadowCamera &shadow = _shadow_cameras[index];
    if (!shadow.is_set) {
      continue;
    }

    // Light space, anchored to the world so that its texels stay put as the camera moves:
    // x and y across the light, z towards it.
    const float *l = shadow.camera.light;
    float right[3] = {l[1], -l[0], 0.0f};  // light x (0, 0, 1)
    float length = std::sqrt(right[0] * right[0] + right[1] * right[1]);
    if (length < 1e-4f) {
      right[0] = 1.0f, right[1] = 0.0f, length = 1.0f;
    }
    right[0] /= length, right[1] /= length;
    const float up[3] = {l[1] * right[2] - l[2] * right[1], l[2] * right[0] - l[0] * right[2],
                         l[0] * right[1] - l[1] * right[0]};
    const float world_to_light[12] = {right[0], right[1], right[2], 0.0f, up[0], up[1], up[2], 0.0f,
                                      l[0],     l[1],     l[2],     0.0f};
    multiply_3x4(world_to_light, shadow.camera.viewToWorld, shadow.view_to_light);

    // The casters' boxes in light space.
    float low[3] = {1e30f, 1e30f, 1e30f};
    float high[3] = {-1e30f, -1e30f, -1e30f};
    int casters = 0;
    for (const auto &command : _commands) {
      if (command.mesh == 0 || command.is_tile || command.mesh_draw.shadowCamera != index) {
        continue;
      }
      float object_to_light[12];
      multiply_3x4(shadow.view_to_light, command.mesh_draw.objectToView, object_to_light);
      for (int corner = 0; corner < 8; corner++) {
        const float point[3] = {command.bounds[corner & 1 ? 3 : 0], command.bounds[corner & 2 ? 4 : 1],
                                command.bounds[corner & 4 ? 5 : 2]};
        float light[3];
        transform_3x4(object_to_light, point, light);
        for (int axis = 0; axis < 3; axis++) {
          low[axis] = std::min(low[axis], light[axis]);
          high[axis] = std::max(high[axis], light[axis]);
        }
      }
      casters++;
    }
    if (casters == 0) {
      continue;
    }

    // A square map whose size only changes by powers of two, at whole texels: the same
    // world point keeps falling on the same texel from frame to frame.
    constexpr float margin = 2.0f;
    float extent = 16.0f;
    while (extent < std::max(high[0] - low[0], high[1] - low[1]) + 2 * margin) {
      extent *= 2.0f;
    }
    const float texel = extent / shadow_map_size;
    const float x0 = std::floor(((low[0] + high[0]) * 0.5f - extent * 0.5f) / texel) * texel;
    const float y0 = std::floor(((low[1] + high[1]) * 0.5f - extent * 0.5f) / texel) * texel;
    // Depth from the casters' top (nearest the light) to their bottom; whatever lies
    // beyond is the ground they shade.
    const float near = high[2] + margin;
    const float far = low[2] - margin;
    const float depth = near - far;
    const float light_to_clip[12] = {2.0f / extent, 0.0f, 0.0f, -2.0f * x0 / extent - 1.0f,
                                     0.0f, 2.0f / extent, 0.0f, -2.0f * y0 / extent - 1.0f,
                                     0.0f, 0.0f, -2.0f / depth, 2.0f * near / depth - 1.0f};
    std::copy(light_to_clip, light_to_clip + 12, shadow.light_to_clip);
    // Taps half a world unit apart (at least a texel): about the softness of the CPU
    // shadows' filtered 2-unit light map texels, without their blocks.
    shadow.filter_step = std::max(texel, 0.5f) / extent;
    shadow.depth_bias = 0.1f / depth;

    _backend->beginShadowPass(index);
    for (const auto &command : _commands) {
      if (command.mesh == 0 || command.is_tile || command.mesh_draw.shadowCamera != index) {
        continue;
      }
      const auto mesh = _meshes.find(command.mesh);
      if (mesh == _meshes.end()) {
        continue;
      }

      ShadowDraw draw{};
      draw.mesh = mesh->second.gpu;
      draw.animated = command.is_animated;
      draw.pose_offset = command.vertex_buffer_view.offset;
      draw.index_count = static_cast<uint32_t>(mesh->second.index_count);
      const auto &texture = command.render_state.textures[0];
      draw.texture = texture ? _texture_manager->get(*texture) : 0;
      draw.repeat = command.render_state.texture_address != TADDRESS_CLAMP;

      float object_to_light[12], object_to_clip[12];
      multiply_3x4(shadow.view_to_light, command.mesh_draw.objectToView, object_to_light);
      multiply_3x4(shadow.light_to_clip, object_to_light, object_to_clip);
      to_mat4(object_to_clip, draw.vs.object_to_shadow);
      // Masked tiles cast through their texture, with the alpha test they are drawn with.
      draw.fs.alpha_test[0] = command.render_state.is_alpha_test_enabled ? 1.0f : 0.0f;
      draw.fs.alpha_test[1] = static_cast<float>(command.render_state.alpha_reference);

      _backend->drawShadow(draw);
      _stats.shadow_casters++;
    }
    _backend->endPass();
    shadow.has_map = true;
    _stats.shadow_maps++;
  }
}

MD3DERROR Renderer::lockBackBuffer(void **lplpSurface, uint32_t *lpdwPitch) {
  assert(!_is_back_buffer_locked);
  _is_back_buffer_locked = true;
  *lplpSurface = _back_buffer.data();
  *lpdwPitch = static_cast<uint32_t>(_width * sizeof(uint16_t));
  return MD3D_OK;
}

MD3DERROR Renderer::unlockBackBuffer() {
  assert(_is_back_buffer_locked);
  _is_back_buffer_locked = false;
  _is_back_buffer_changed = true;
  return MD3D_OK;
}

MD3DERROR Renderer::flushBackBuffer(MD3DRECT *lprcRect) {
  _is_back_buffer_flush = true;
  return MD3D_OK;
}

void Renderer::prepare_render_state() {
  if (_commands.empty()) {
    _commands.emplace_back(DrawCommand{_render_state, BufferView{0, 0}, BufferView{0, 0}});
    return;
  }

  // A GPU mesh command draws only its mesh: geometry after it starts a new command.
  const auto& last_command = _commands.back();
  if (last_command.mesh == 0 && _render_state == last_command.render_state) {
    return;
  }

  _commands.emplace_back(
      DrawCommand{_render_state, last_command.vertex_buffer_view.next(), last_command.index_buffer_view.next()});
}
