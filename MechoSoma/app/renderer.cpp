//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <algorithm>
#include <cassert>

#include <SDL2/SDL.h>

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
}

using namespace graphics;

Renderer::Renderer(int width, int height, bool isFullScreen, bool textureMipmaps) {
  _display = std::make_unique<Display>(width, height);

  {
    sg_desc description{};
    description.buffer_pool_size = 9;
    description.image_pool_size = TextureManager::max_textures_count;
    description.view_pool_size = TextureManager::max_textures_count;
    description.shader_pool_size = 3;
    // Pipelines are created once and kept: the scene's (scene_pipeline(), 6 in
    // practice and at most 26) plus one each for the back buffer and the window copy.
    description.pipeline_pool_size = 64;
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

  {
    sg_range data = {
        .ptr = new float[4] { 0.0f, 0.0f, 0.0f, 0.0f },
        .size = 4,
    };
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
  _index_buffer.resize(3 * initial_vertex_count);

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
  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  _offscreenBuffer->flush(_display->swapchain());
  _display->present();
#ifdef GPX
  frameReady();
#endif
  return MD3D_OK;
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
  _commands.clear();
  _render_state = RenderState();
  _texture_manager->delete_textures();
  _is_in_scene = true;
  return MD3D_OK;
}

MD3DERROR Renderer::endScene() {
  _is_in_scene = false;

  if (_commands.empty()) {
    return MD3D_OK;
  }

  // Upload only what this frame used, before the pass in case a buffer is recreated.
  const size_t vertex_count = _commands.back().vertex_buffer_view.end();
  const size_t index_count = _commands.back().index_buffer_view.end();
  upload_frame_buffer(sg_vertex_buffer, _vertex_buffer, vertex_count, false, "vertex");
  upload_frame_buffer(sg_index_buffer, _index_buffer, index_count, true, "index");

  _offscreenBuffer->begin_pass(defaultPassAction);

  if (_is_back_buffer_flush) {
    _backBuffer->flush();
    _is_back_buffer_flush = false;
  }

  for (const auto& command : _commands) {
    if (command.index_buffer_view.length == 0) {
      continue;
    }

    sg_bindings bindings = {};

    auto vs_params = scene_vs_params_t {};
    const auto m = command.render_state.projection_matrix;
    if (m) {
      std::copy(&m->_11, &m->_11 + 16, vs_params.projection_matrix);
    } else {
      std::copy(_projectionMatrix.begin(), _projectionMatrix.end(), vs_params.projection_matrix);
    }

    const auto viewport = command.render_state.viewport;
    if (viewport) {
      sg_apply_viewport(viewport->left, viewport->top, viewport->right, viewport->bottom, true);
    } else {
      sg_apply_viewport(0, 0, _offscreenBuffer->getWidth(), _offscreenBuffer->getHeight(), true);
    }

    for (uint32_t i = 0; i < 2; i++) {
      bindings.views[i] = _nullTextureView;

      auto texture_handle = command.render_state.textures[i];
      if (texture_handle) {
        auto texture = _texture_manager->get(*texture_handle);
        if (texture != nullptr) {
          bindings.views[i] = *texture;
        }
      }
    }

    switch (command.render_state.texture_address) {
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

    auto fs_params = scene_fs_params_t{
        .color_operation_1 = command.render_state.color_operation1,
        .color_operation_2 = command.render_state.color_operation2,
        .alpha_test_enabled = command.render_state.is_alpha_test_enabled,
        .alpha_reference = command.render_state.alpha_reference
    };

    bindings.vertex_buffers[0] = sg_vertex_buffer;

    const auto count = command.index_buffer_view.length;
    const auto base_element = command.index_buffer_view.offset;

    bindings.index_buffer = sg_index_buffer;
    bindings.index_buffer_offset = 0;

    sg_apply_pipeline(scene_pipeline(command.render_state));
    sg_apply_bindings(bindings);
    sg_apply_uniforms(UB_scene_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(UB_scene_fs_params, SG_RANGE(fs_params));

    sg_draw(base_element, count, 1);
  }

  sg_end_pass();
  sg_commit();

  if (defaultPassAction.colors[0].load_action == SG_LOADACTION_CLEAR) {
    defaultPassAction.colors[0].load_action = SG_LOADACTION_DONTCARE;
  }

  return MD3D_OK;
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

  return MD3D_OK;
}

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

  const auto& last_command = _commands.back();
  if (_render_state == last_command.render_state) {
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

sg_pipeline Renderer::scene_pipeline(const RenderState &state) {
  // RENDERSTATE_ZTEST was D3D's ZENABLE: turned off, the depth buffer is neither tested
  // nor written, whatever RENDERSTATE_ZWRITE says.
  const bool depth_test = state.is_depth_test_enabled;
  ScenePipelineKey key{depth_test, depth_test && state.is_depth_write_enabled, state.is_alpha_blend_enabled,
                       SG_BLENDFACTOR_ONE, SG_BLENDFACTOR_ZERO};
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
  sg_pipeline_desc description = {};
  description.shader = _sceneShader;
  description.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
  description.index_type = SG_INDEXTYPE_UINT32;
  description.cull_mode = SG_CULLMODE_BACK;
  description.face_winding = SG_FACEWINDING_CCW;

  description.depth.write_enabled = key.depth_write;
  description.depth.compare = key.depth_test ? SG_COMPAREFUNC_LESS_EQUAL : SG_COMPAREFUNC_ALWAYS;

  description.colors[0].blend.enabled = key.blend;
  description.colors[0].blend.src_factor_rgb = key.source;
  description.colors[0].blend.src_factor_alpha = key.source;
  description.colors[0].blend.dst_factor_rgb = key.destination;
  description.colors[0].blend.dst_factor_alpha = key.destination;

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

  const auto pipeline = sg_make_pipeline(description);
  if (sg_query_pipeline_state(pipeline) != SG_RESOURCESTATE_VALID) {
    ErrH.Abort("sg_make_pipeline", XERR_USER, 0, "");
  }
  _scene_pipelines.emplace_back(key, pipeline);
  SDL_Log("Renderer: scene pipeline %u created (depth test %d, depth write %d, blend %d, factors %d/%d)",
          static_cast<unsigned>(_scene_pipelines.size()), key.depth_test ? 1 : 0, key.depth_write ? 1 : 0,
          key.blend ? 1 : 0, static_cast<int>(key.source), static_cast<int>(key.destination));
  return pipeline;
}
