//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>

#include <SDL3/SDL.h>

#include "back_buffer.h"
#include "display.h"
#include "offscreen_buffer.h"
#include "texture_manager.h"
#include "xgraph.h"
#include "xtool.h"

#ifdef EMSCRIPTEN
#include "sokol-shader-em.h"
#else
#include "sokol-scene-shader.h"
#endif

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
  // A GPU buffer that the frame's vertices or indices are uploaded into, one element
  // of type T per array element.
  template <typename T>
  sg_buffer make_frame_buffer(size_t count, bool is_index_buffer) {
    sg_buffer_desc description{};
    description.size = count * sizeof(T);
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

  // Uploads the first `count` elements of `array`. If the frame has grown `array`
  // past what `buffer` holds, `buffer` is first recreated at the array's new size.
  template <typename T>
  void upload_frame_buffer(sg_buffer &buffer, const std::vector<T> &array, size_t count, bool is_index_buffer,
                           const char *name) {
    if (count == 0) {
      return;  // sokol does not accept an empty update
    }
    const size_t capacity = array.size() * sizeof(T);
    if (sg_query_buffer_size(buffer) < capacity) {
      sg_destroy_buffer(buffer);
      buffer = make_frame_buffer<T>(array.size(), is_index_buffer);
      SDL_Log("Renderer: %s buffer grown to %u KiB", name, static_cast<unsigned>(capacity / 1024));
    }
    sg_update_buffer(buffer, sg_range{array.data(), count * sizeof(T)});
  }

  template <typename Duration>
  double milliseconds(Duration duration) {
    return std::chrono::duration<double, std::milli>(duration).count();
  }

  // The fragment uniforms, the same for frame geometry and GPU meshes.
  scene_fs_params_t scene_fs_params(const graphics::RenderState &state) {
    scene_fs_params_t params{};
    params.color_operation_1 = state.color_operation1;
    params.color_operation_2 = state.color_operation2;
    params.alpha_test_enabled = state.is_alpha_test_enabled;
    params.alpha_reference = state.alpha_reference;
    return params;
  }

#ifndef EMSCRIPTEN
  // A 3x4 row-major object matrix (M3D_MESH_DRAW) as a GLSL mat4, which is column-major.
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

  // A dynamic single-channel image for one terrain map, and its texture view.
  void make_terrain_map(int width, int height, sg_image &image, sg_view &view) {
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
}

using namespace graphics;

Renderer::Renderer(int width, int height, bool isFullScreen, bool textureMipmaps, bool gpuMeshes, bool gpuShadows,
                   bool gpuTerrain, bool renderStats)
#ifdef EMSCRIPTEN
    // The web build's shader header (sokol-shader-em.h) has no mesh, tile or shadow program.
    : _gpu_meshes(false), _gpu_terrain(false), _gpu_shadows(false),
#else
    : _gpu_meshes(gpuMeshes), _gpu_terrain(gpuTerrain), _gpu_shadows(gpuMeshes && gpuShadows),
#endif
      _render_stats(renderStats), _stats_start(Clock::now()), _scene_begin(_stats_start) {
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
  if (_gpu_meshes) {
    _meshShader = sg_make_shader(mesh_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_meshShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the mesh shader failed, meshes are drawn on the CPU");
      _gpu_meshes = false;
      _gpu_shadows = false;
    }
  }
  if (_gpu_terrain) {
    _tileShader = sg_make_shader(tile_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_tileShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the tile shader failed, terrain is drawn on the CPU");
      _gpu_terrain = false;
    }
  }
  if (_gpu_shadows) {
    _shadowShader = sg_make_shader(shadow_shader_desc(sg_query_backend()));
    if (sg_query_shader_state(_shadowShader) != SG_RESOURCESTATE_VALID) {
      SDL_Log("Renderer: the shadow shader failed, shadows are made on the CPU");
      _gpu_shadows = false;
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
  SDL_Log("Renderer: GPU meshes %s, GPU shadows %s, GPU terrain %s", _gpu_meshes ? "on" : "off",
          _gpu_shadows ? "on" : "off", _gpu_terrain ? "on" : "off");

  {
    sg_range data{};
    data.ptr = new float[4] { 0.0f, 0.0f, 0.0f, 0.0f };
    data.size = 4;
    sg_image_desc description{};
    description.width = 1;
    description.height = 1;
    description.num_slices = 1;
    description.num_mipmaps = 1;
    description.usage.immutable = true;
    description.pixel_format = SG_PIXELFORMAT_RGBA8;
    description.sample_count = 1;
    description.data.mip_levels[0] = data;
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

  _vertex_buffer.resize(initial_vertex_count);
  _index_buffer.resize(initial_index_count);

  sg_vertex_buffer = make_frame_buffer<M3D_VERTEX>(_vertex_buffer.size(), false);
  sg_index_buffer = make_frame_buffer<uint32_t>(_index_buffer.size(), true);

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

  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;

  _texture_manager = std::make_unique<TextureManager>(textureMipmaps);
  setVideoMode(width, height, isFullScreen);
}

Renderer::~Renderer() {
  _texture_manager = nullptr;
  _backBuffer = nullptr;
  _offscreenBuffer = nullptr;

  sg_shutdown();
  _display = nullptr;
}

TextureManager& Renderer::get_texture_manager() { return *_texture_manager; }

void Renderer::setVideoMode(int width, int height, bool isFullScreen) {
  const auto scale = _display->setVideoMode(width, height, isFullScreen);
  XGR_WindowScaleX = scale.x;
  XGR_WindowScaleY = scale.y;

  _projectionMatrix = make_ortho_projection(0, width, height, 0, -1, 1);

  _offscreenBuffer = nullptr;
  _offscreenBuffer =
      std::make_unique<OffscreenBuffer>(width, height, _display->drawableWidth(), _display->drawableHeight());

  _backBuffer = nullptr;
  _backBuffer = std::make_unique<BackBuffer>(width, height);
}

bool Renderer::isInScene() const
{
  return _is_in_scene;
}

MD3DERROR Renderer::flip(bool WaitVerticalBlank) {
  const auto present_start = Clock::now();
  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  _offscreenBuffer->flush(_display->swapchain());
  _display->present();
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

  // Upload only what this frame used, before the pass in case a buffer is recreated.
  const size_t vertex_count = _commands.back().vertex_buffer_view.end();
  const size_t index_count = _commands.back().index_buffer_view.end();
  _stats.vertices += vertex_count;
  _stats.indices += index_count;
  _stats.peak_vertices = std::max(_stats.peak_vertices, vertex_count);
  _stats.peak_indices = std::max(_stats.peak_indices, index_count);
  upload_frame_buffer(sg_vertex_buffer, _vertex_buffer, vertex_count, false, "vertex");
  upload_frame_buffer(sg_index_buffer, _index_buffer, index_count, true, "index");
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

#ifndef EMSCRIPTEN
  if (_gpu_shadows) {
    render_shadow_maps();
  }
#endif

  _offscreenBuffer->begin_pass(defaultPassAction);

  if (_is_back_buffer_flush) {
    _backBuffer->flush();
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

    sg_bindings bindings = {};

    auto vs_params = scene_vs_params_t {};
    copy_projection(command.render_state, vs_params.projection_matrix);
#ifndef EMSCRIPTEN
    const int shadow_camera = command.render_state.shadow_camera;
    const bool takes_shadow = shadow_camera >= 0 && _shadow_cameras[shadow_camera].has_map;
    if (takes_shadow) {
      const auto &shadow = _shadow_cameras[shadow_camera];
      float view_to_clip[12];
      multiply_3x4(shadow.light_to_clip, shadow.view_to_light, view_to_clip);
      to_mat4(view_to_clip, vs_params.view_to_shadow);
      to_mat4(shadow.camera.viewToWorld, vs_params.view_to_world);
    }
#endif

    apply_viewport(command.render_state);
    bind_textures(command.render_state, bindings);
#ifndef EMSCRIPTEN
    bind_shadow(takes_shadow ? shadow_camera : -1, bindings);
#endif
    const auto fs_params = scene_fs_params(command.render_state);

    bindings.vertex_buffers[0] = sg_vertex_buffer;

    const auto count = command.index_buffer_view.length;
    const auto base_element = command.index_buffer_view.offset;

    bindings.index_buffer = sg_index_buffer;
    bindings.index_buffer_offset = 0;

    sg_apply_pipeline(scene_pipeline(command.render_state, Geometry::frame));
    sg_apply_bindings(bindings);
    sg_apply_uniforms(UB_scene_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(UB_scene_fs_params, SG_RANGE(fs_params));
#ifndef EMSCRIPTEN
    apply_shadow_uniforms(takes_shadow ? shadow_camera : -1);
#endif

    sg_draw(base_element, count, 1);
  }

  sg_end_pass();
  sg_commit();
  destroy_deleted_meshes();
  // The game sets its shadow cameras again for the next frame.
  for (auto &shadow : _shadow_cameras) {
    shadow.is_set = false;
    shadow.has_map = false;
  }
  _stats.submit_ms += milliseconds(Clock::now() - submit_start);

  if (defaultPassAction.colors[0].load_action == SG_LOADACTION_CLEAR) {
    defaultPassAction.colors[0].load_action = SG_LOADACTION_DONTCARE;
  }

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

void Renderer::apply_viewport(const RenderState &state) const {
  const auto &viewport = state.viewport;
  if (viewport) {
    sg_apply_viewport(viewport->left, viewport->top, viewport->right, viewport->bottom, true);
  } else {
    sg_apply_viewport(0, 0, _offscreenBuffer->getWidth(), _offscreenBuffer->getHeight(), true);
  }
}

void Renderer::bind_textures(const RenderState &state, sg_bindings &bindings) {
  for (uint32_t i = 0; i < 2; i++) {
    bindings.views[i] = _nullTextureView;

    auto texture_handle = state.textures[i];
    if (texture_handle) {
      auto texture = _texture_manager->get(*texture_handle);
      if (texture != nullptr) {
        bindings.views[i] = *texture;
      }
    }
  }

  switch (state.texture_address) {
    case TADDRESS_CLAMP:
      bindings.samplers[SMP_sampler_1] = _clamp_sampler;
      break;

    case TADDRESS_WRAP:
      bindings.samplers[SMP_sampler_1] = _repeat_sampler;
      break;

    default:
      bindings.samplers[SMP_sampler_1] = _repeat_sampler;
      break;
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
  const auto red = static_cast<float>((dwColor >> 16) & 0xFF) / 255.0f;
  const auto green = static_cast<float>((dwColor >> 8) & 0xFF) / 255.0f;
  const auto blue = static_cast<float>(dwColor & 0xFF) / 255.0f;

  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  defaultPassAction.colors[0].clear_value = {red, green, blue, 1};
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
  return add_mesh(mesh, sg_range{vertices, vertexCount * sizeof(M3D_MESH_VERTEX)},
                  sg_range{indices, indexCount * sizeof(uint16_t)});
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
  const uint32_t handle = add_mesh(mesh, sg_range{vertices, vertexCount * sizeof(M3D_TILE_VERTEX)},
                                   sg_range{indices, indexCount * sizeof(uint32_t)});
  if (handle != 0) {
    _tile_set_count++;
    SDL_Log("Renderer: terrain tile set, %d vertices and %d indices (%u KiB)", vertexCount, indexCount,
            static_cast<unsigned>((vertexCount * sizeof(M3D_TILE_VERTEX) + indexCount * sizeof(uint32_t)) / 1024));
  }
  return handle;
}

uint32_t Renderer::add_mesh(Mesh mesh, sg_range vertices, sg_range indices) {
  if (_meshes.size() >= max_meshes) {
    if (!_is_mesh_pool_full_logged) {
      SDL_Log("Renderer: %u GPU meshes and tiles, the most there can be; more are drawn on the CPU",
              static_cast<unsigned>(_meshes.size()));
      _is_mesh_pool_full_logged = true;
    }
    return 0;
  }

  mesh.bytes = vertices.size + indices.size;
  {
    sg_buffer_desc description{};
    description.usage.vertex_buffer = true;
    description.data = vertices;
    mesh.vertices = sg_make_buffer(&description);
  }
  {
    sg_buffer_desc description{};
    description.usage.index_buffer = true;
    description.data = indices;
    mesh.indices = sg_make_buffer(&description);
  }
  if (sg_query_buffer_state(mesh.vertices) != SG_RESOURCESTATE_VALID ||
      sg_query_buffer_state(mesh.indices) != SG_RESOURCESTATE_VALID) {
    sg_destroy_buffer(mesh.vertices);
    sg_destroy_buffer(mesh.indices);
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
    sg_destroy_buffer(mesh->second.vertices);
    sg_destroy_buffer(mesh->second.indices);
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

void Renderer::upload_terrain() {
#ifndef EMSCRIPTEN
  const size_t size = static_cast<size_t>(_terrain.width) * _terrain.height;

  // A map of another size (another world): new images, and an upload whatever it holds.
  bool changed = false;
  if (_terrain_width != _terrain.width || _terrain_height != _terrain.height) {
    if (_terrain_heights.id != SG_INVALID_ID) {
      sg_destroy_view(_terrain_heights_view);
      sg_destroy_view(_terrain_attributes_view);
      sg_destroy_image(_terrain_heights);
      sg_destroy_image(_terrain_attributes);
    }
    make_terrain_map(_terrain.width, _terrain.height, _terrain_heights, _terrain_heights_view);
    make_terrain_map(_terrain.width, _terrain.height, _terrain_attributes, _terrain_attributes_view);
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

    sg_image_data data{};
    data.mip_levels[0] = sg_range{_terrain_heights_copy.data(), size};
    sg_update_image(_terrain_heights, &data);
    data.mip_levels[0] = sg_range{_terrain_attributes_copy.data(), size};
    sg_update_image(_terrain_attributes, &data);

    _is_terrain_uploaded = true;
    _stats.terrain_uploads++;
  }
#endif
}

void Renderer::draw_mesh_command(const DrawCommand &command) {
#ifndef EMSCRIPTEN
  const auto mesh = _meshes.find(command.mesh);
  if (mesh == _meshes.end()) {
    return;
  }
  const auto &draw = command.mesh_draw;
  const bool shade = draw.renderShade && _is_terrain_uploaded;

  mesh_vs_params_t vs_params{};
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

  sg_bindings bindings = {};
  bind_textures(command.render_state, bindings);
  bindings.views[VIEW_terrain_heights] = shade ? _terrain_heights_view : _nullTextureView;
  bindings.views[VIEW_terrain_attributes] = shade ? _terrain_attributes_view : _nullTextureView;
  bindings.samplers[SMP_terrain_sampler] = _terrain_sampler;
  if (command.is_animated) {
    // Positions and texture coordinates from this frame's pose, normals from the mesh.
    bindings.vertex_buffers[0] = sg_vertex_buffer;
    bindings.vertex_buffer_offsets[0] = static_cast<int>(command.vertex_buffer_view.offset * sizeof(M3D_VERTEX));
    bindings.vertex_buffers[1] = mesh->second.vertices;
  } else {
    bindings.vertex_buffers[0] = mesh->second.vertices;
  }
  bindings.index_buffer = mesh->second.indices;

  // Meshes take no dynamic shadows; scene_fs still needs its shadow bindings.
  bind_shadow(-1, bindings);

  const auto fs_params = scene_fs_params(command.render_state);

  apply_viewport(command.render_state);
  sg_apply_pipeline(scene_pipeline(command.render_state,
                                   command.is_animated ? Geometry::animated_mesh : Geometry::mesh,
                                   draw.mirrored != 0));
  sg_apply_bindings(bindings);
  sg_apply_uniforms(UB_mesh_vs_params, SG_RANGE(vs_params));
  sg_apply_uniforms(UB_scene_fs_params, SG_RANGE(fs_params));
  apply_shadow_uniforms(-1);
  sg_draw(0, mesh->second.index_count, 1);
#endif
}

void Renderer::draw_tile_command(const DrawCommand &command) {
#ifndef EMSCRIPTEN
  const auto mesh = _meshes.find(command.mesh);
  if (mesh == _meshes.end()) {
    return;
  }
  const auto &draw = command.tile_draw;

  tile_vs_params_t vs_params{};
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
  // tile, so from one tile to the next GL only changes textures and uniforms.
  vs_params.share[0] = static_cast<float>(draw.shareOffset[0]);
  vs_params.share[1] = static_cast<float>(draw.shareOffset[1]);
  vs_params.share[2] = static_cast<float>(draw.shareRadius);
  vs_params.share[3] = draw.clampNearZ ? 1.0f : 0.0f;

  sg_bindings bindings = {};
  bind_textures(command.render_state, bindings);
  bind_shadow(takes_shadow ? shadow_camera : -1, bindings);
  bindings.vertex_buffers[0] = mesh->second.vertices;
  bindings.index_buffer = mesh->second.indices;

  const auto fs_params = scene_fs_params(command.render_state);

  apply_viewport(command.render_state);
  sg_apply_pipeline(scene_pipeline(command.render_state, Geometry::tile));
  sg_apply_bindings(bindings);
  sg_apply_uniforms(UB_tile_vs_params, SG_RANGE(vs_params));
  sg_apply_uniforms(UB_scene_fs_params, SG_RANGE(fs_params));
  apply_shadow_uniforms(takes_shadow ? shadow_camera : -1);
  sg_draw(draw.firstIndex, draw.indexCount, 1);
#endif
}

#ifndef EMSCRIPTEN
void Renderer::bind_shadow(int camera, sg_bindings &bindings) {
  bindings.views[VIEW_shadow_map] = camera >= 0 ? _shadow_cameras[camera].texture : _null_shadow_map_view;
  bindings.views[VIEW_shadow_cells] =
      camera >= 0 && _is_terrain_uploaded ? _terrain_attributes_view : _nullTextureView;
  bindings.samplers[SMP_shadow_sampler] = _shadow_sampler;
  bindings.samplers[SMP_shadow_cells_sampler] = _terrain_sampler;
}

void Renderer::apply_shadow_uniforms(int camera) {
  scene_shadow_params_t params{};
  if (camera >= 0) {
    const auto &shadow = _shadow_cameras[camera];
    params.shadow[0] = 1.0f;
    params.shadow[1] = shadow.camera.darkness;
    params.shadow[2] = shadow.filter_step;
    params.shadow[3] = shadow.depth_bias;
    params.shadow_terrain[0] = static_cast<float>(_terrain.worldMaskX);
    params.shadow_terrain[1] = static_cast<float>(_terrain.worldMaskY);
    params.shadow_terrain[2] = _is_terrain_uploaded ? 1.0f : 0.0f;
  }
  sg_apply_uniforms(UB_scene_shadow_params, SG_RANGE(params));
}

sg_pipeline Renderer::shadow_pipeline(bool animated) {
  sg_pipeline &pipeline = _shadow_pipelines[animated ? 1 : 0];
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

    if (shadow.map.id == SG_INVALID_ID) {
      sg_image_desc description{};
      description.usage.depth_stencil_attachment = true;
      description.width = shadow_map_size;
      description.height = shadow_map_size;
      description.pixel_format = SG_PIXELFORMAT_DEPTH;
      shadow.map = sg_make_image(&description);
      sg_view_desc attachment{};
      attachment.depth_stencil_attachment.image = shadow.map;
      shadow.attachment = sg_make_view(&attachment);
      sg_view_desc texture{};
      texture.texture.image = shadow.map;
      shadow.texture = sg_make_view(&texture);
      if (sg_query_view_state(shadow.attachment) != SG_RESOURCESTATE_VALID ||
          sg_query_view_state(shadow.texture) != SG_RESOURCESTATE_VALID) {
        ErrH.Abort("sg_make_view", XERR_USER, 0, "shadow map");
      }
      SDL_Log("Renderer: shadow map %d (%dx%d)", index, shadow_map_size, shadow_map_size);
    }

    sg_pass pass{};
    pass.action.depth.load_action = SG_LOADACTION_CLEAR;
    pass.action.depth.clear_value = 1.0f;
    pass.attachments.depth_stencil = shadow.attachment;
    sg_begin_pass(&pass);
    for (const auto &command : _commands) {
      if (command.mesh == 0 || command.is_tile || command.mesh_draw.shadowCamera != index) {
        continue;
      }
      const auto mesh = _meshes.find(command.mesh);
      if (mesh == _meshes.end()) {
        continue;
      }

      shadow_vs_params_t vs_params{};
      float object_to_light[12], object_to_clip[12];
      multiply_3x4(shadow.view_to_light, command.mesh_draw.objectToView, object_to_light);
      multiply_3x4(shadow.light_to_clip, object_to_light, object_to_clip);
      to_mat4(object_to_clip, vs_params.object_to_shadow);
      // Masked tiles cast through their texture, with the alpha test they are drawn with.
      shadow_fs_params_t fs_params{};
      fs_params.alpha_test[0] = command.render_state.is_alpha_test_enabled ? 1.0f : 0.0f;
      fs_params.alpha_test[1] = static_cast<float>(command.render_state.alpha_reference);

      sg_bindings bindings{};
      bind_textures(command.render_state, bindings);
      bindings.views[VIEW_texture_2] = {};  // not in the shadow program
      if (command.is_animated) {
        bindings.vertex_buffers[0] = sg_vertex_buffer;
        bindings.vertex_buffer_offsets[0] = static_cast<int>(command.vertex_buffer_view.offset * sizeof(M3D_VERTEX));
      } else {
        bindings.vertex_buffers[0] = mesh->second.vertices;
      }
      bindings.index_buffer = mesh->second.indices;

      sg_apply_pipeline(shadow_pipeline(command.is_animated));
      sg_apply_bindings(bindings);
      sg_apply_uniforms(UB_shadow_vs_params, SG_RANGE(vs_params));
      sg_apply_uniforms(UB_shadow_fs_params, SG_RANGE(fs_params));
      sg_draw(0, mesh->second.index_count, 1);
      _stats.shadow_casters++;
    }
    sg_end_pass();
    shadow.has_map = true;
    _stats.shadow_maps++;
  }
}
#endif

MD3DERROR Renderer::lockBackBuffer(void **lplpSurface, uint32_t *lpdwPitch) {
  const auto address = _backBuffer->lock();
  *lplpSurface = address.address;
  *lpdwPitch = address.pitch;
  return MD3D_OK;
}

MD3DERROR Renderer::unlockBackBuffer() {
  _backBuffer->unlock();
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

namespace {
  // The sokol blend factor for one of the game's blend modes. Only the modes the game
  // uses are mapped; any other gets `fallback`, which is sokol's default for that side.
  sg_blend_factor blend_factor(eBlendMode mode, sg_blend_factor fallback) {
    switch (mode) {
      case BLEND_SRCALPHA:
        return SG_BLENDFACTOR_SRC_ALPHA;
      case BLEND_INVSRCALPHA:
        return SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
      case BLEND_ONE:
        return SG_BLENDFACTOR_ONE;
      default:
        return fallback;
    }
  }
}

sg_pipeline Renderer::scene_pipeline(const RenderState &state, Geometry geometry, bool mirrored) {
  // RENDERSTATE_ZTEST was D3D's ZENABLE: turned off, the depth buffer is neither tested
  // nor written, whatever RENDERSTATE_ZWRITE says.
  const bool depth_test = state.is_depth_test_enabled;
  ScenePipelineKey key{geometry,
                       depth_test,
                       depth_test && state.is_depth_write_enabled,
                       state.is_alpha_blend_enabled,
                       SG_BLENDFACTOR_ONE,
                       SG_BLENDFACTOR_ZERO,
                       mirrored};
  if (key.blend) {
    key.source = blend_factor(state.source_blend_mode, SG_BLENDFACTOR_ONE);
    key.destination = blend_factor(state.destination_blend_mode, SG_BLENDFACTOR_ZERO);
  }

  for (const auto &entry : _scene_pipelines) {
    if (entry.first == key) {
      return entry.second;
    }
  }

  // Built from the key alone, so commands with the same key share a pipeline.
  const bool is_frame = key.geometry == Geometry::frame;
  sg_pipeline_desc description = {};
  description.shader = is_frame ? _sceneShader : key.geometry == Geometry::tile ? _tileShader : _meshShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.index_type =
      is_frame || key.geometry == Geometry::tile ? SG_INDEXTYPE_UINT32 : SG_INDEXTYPE_UINT16;
  // The 2000 renderer culled on the CPU, keeping triangles that wind one way on the screen
  // (AddPolygonFixTestPointFix, master). A mirror turns them the other way, so it sent a
  // reflection's corners in the opposite order; a mirrored GPU mesh flips the front face.
  description.cull_mode = SG_CULLMODE_BACK;
  description.face_winding = key.mirrored ? SG_FACEWINDING_CW : SG_FACEWINDING_CCW;

  description.depth.write_enabled = key.depth_write;
  description.depth.compare = key.depth_test ? SG_COMPAREFUNC_LESS_EQUAL : SG_COMPAREFUNC_ALWAYS;

  description.colors[0].blend.enabled = key.blend;
  description.colors[0].blend.src_factor_rgb = key.source;
  description.colors[0].blend.src_factor_alpha = key.source;
  description.colors[0].blend.dst_factor_rgb = key.destination;
  description.colors[0].blend.dst_factor_alpha = key.destination;

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
  const char *kind = key.geometry == Geometry::frame  ? "scene"
                     : key.geometry == Geometry::mesh ? "mesh"
                     : key.geometry == Geometry::tile ? "tile"
                                                      : "animated mesh";
  SDL_Log("Renderer: %s pipeline %u created (depth test %d, depth write %d, blend %d, factors %d/%d%s)", kind,
          static_cast<unsigned>(_scene_pipelines.size()), key.depth_test ? 1 : 0, key.depth_write ? 1 : 0,
          key.blend ? 1 : 0, static_cast<int>(key.source), static_cast<int>(key.destination),
          key.mirrored ? ", mirrored" : "");
  return pipeline;
}
