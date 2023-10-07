//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <algorithm>
#include <cassert>

#include "back_buffer.h"
#include "offscreen_buffer.h"
#include "texture_manager.h"
#include "vertex_type.h"
#include "xgraph.h"
#include "xtool.h"

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
        2.0f / (right - left), 0, 0, (left + right) / (left - right),
        0, 2.0f / (top - bottom), 0, (top + bottom) / (bottom - top),
        0, 0, 1.0f / (far - near), near / (near - far),
        0, 0, 0, 1.0f
    );
  }
  // clang-format on
}

using namespace graphics;

Renderer::Renderer(int width, int height, bool isFullScreen) {
  _window = SDL_CreateWindow(
    "Moonshine Runners",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  if (_window == nullptr) {
    ErrH.Abort("SDL_CreateWindow", XERR_USER, 0, "");
  }
  _context = SDL_GL_CreateContext(_window);
  if (_context == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): %s\n", SDL_GetError());
    ErrH.Abort("SDL_GL_CreateContext", XERR_USER, 0, "");
  }

  auto context = sg_context_desc {
    .color_format = SG_PIXELFORMAT_RGBA8,
    .depth_format = SG_PIXELFORMAT_DEPTH
  };
  sg_setup(sg_desc {
    .buffer_pool_size = 8,
    .image_pool_size = TextureManager::max_textures_count,
    .shader_pool_size = 3,
    .pipeline_pool_size = 1,
    .context = context
  });
  if (!sg_isvalid()) {
    ErrH.Abort("sg_setup", XERR_USER, 0, "");
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
    description.usage = SG_USAGE_IMMUTABLE;
    description.pixel_format = SG_PIXELFORMAT_RGBA8;
    description.sample_count = 1;
    description.data.subimage[0][0] = data;
    _nullTexture = sg_make_image(description);
    if (_nullTexture.id == SG_INVALID_ID) {
      ErrH.Abort("sg_make_image", XERR_USER, 0, "");
    }
  }

  {
    _position_buffer.resize(_vertex_count * 3);
    sg_position_buffer = sg_make_buffer(sg_buffer_desc{
        .size = _position_buffer.size() * sizeof(float),
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    _color_buffer.resize(_vertex_count * 4);
    sg_color_buffer = sg_make_buffer(sg_buffer_desc{
        .size = _color_buffer.size() * sizeof(float),
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    _uv_buffer.resize(_vertex_count * 2);
    sg_uv_buffer = sg_make_buffer(sg_buffer_desc{
        .size = _uv_buffer.size() * sizeof(float),
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    _index_buffer.resize(_vertex_count * 3);
    sg_index_buffer = sg_make_buffer(sg_buffer_desc{
        .size = _index_buffer.size() * sizeof(uint32_t),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    description.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    _clamp_sampler = sg_make_sampler(description);
  }

  {
    sg_sampler_desc description{};
    description.min_filter = SG_FILTER_LINEAR;
    description.mag_filter = SG_FILTER_LINEAR;
    description.wrap_u = SG_WRAP_REPEAT;
    description.wrap_v = SG_WRAP_REPEAT;
    _repeat_sampler = sg_make_sampler(description);
  }

  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;

  _texture_manager = std::make_unique<TextureManager>();
  setVideoMode(width, height, isFullScreen);
}

Renderer::~Renderer() {
  _texture_manager = nullptr;
  _backBuffer = nullptr;
  _offscreenBuffer = nullptr;

  sg_shutdown();
  SDL_GL_DeleteContext(_context);
  SDL_DestroyWindow(_window);
}

TextureManager& Renderer::get_texture_manager() { return *_texture_manager; }

void Renderer::setVideoMode(int width, int height, bool isFullScreen) {
  if (!isFullScreen) {
    SDL_SetWindowFullscreen(_window, 0);
    SDL_SetWindowSize(_window, width, height);
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowCursor(SDL_ENABLE);

    XGR_WindowScaleX = 1.0f;
    XGR_WindowScaleY = 1.0f;
  } else {
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(_window, 0, 0);
    SDL_ShowCursor(SDL_DISABLE);

    SDL_DisplayMode displayMode{};
    SDL_GetCurrentDisplayMode(0, &displayMode);
    XGR_WindowScaleX = 1.0f * width / displayMode.w;
    XGR_WindowScaleY = 1.0f * height / displayMode.h;
  }

  _projectionMatrix = make_ortho_projection(0, width, height, 0, -1, 1);

  _offscreenBuffer = nullptr;
  int drawableWidth = 0;
  int drawableHeight = 0;
  SDL_GL_GetDrawableSize(_window, &drawableWidth, &drawableHeight);
  _offscreenBuffer = std::make_unique<OffscreenBuffer>(width, height, drawableWidth, drawableHeight);

  _backBuffer = nullptr;
  _backBuffer = std::make_unique<BackBuffer>(width, height);
}

MD3DERROR Renderer::d3dFlip(bool WaitVerticalBlank) {
  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  _offscreenBuffer->flush();
  SDL_GL_SwapWindow(_window);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dBeginScene() {
  _commands.clear();
  _render_state.reset_texture_stage();
  _texture_manager->delete_textures();
  return MD3D_OK;
}

MD3DERROR Renderer::d3dEndScene() {
  if (_commands.empty()) {
    return MD3D_OK;
  }

  sg_begin_pass(_offscreenBuffer->getRenderingPass(), defaultPassAction);

  if (_is_back_buffer_flush) {
    _backBuffer->flush();
    _is_back_buffer_flush = false;
  }

  sg_update_buffer(sg_position_buffer, sg_range{
                                           .ptr = _position_buffer.data(),
                                           .size = _vertex_count * 3 * sizeof(float),
                                       });
  sg_update_buffer(sg_color_buffer, sg_range{
                                        .ptr = _color_buffer.data(),
                                        .size = _vertex_count * 4 * sizeof(float),
                                    });

  sg_update_buffer(sg_uv_buffer, sg_range{
                                     .ptr = _uv_buffer.data(),
                                     .size = _vertex_count * 2 * sizeof(float),
                                 });

  sg_update_buffer(sg_index_buffer, sg_range{
                                        .ptr = _index_buffer.data(),
                                        .size = _vertex_count * 3 * sizeof(uint32_t),
                                    });

  for (const auto& command : _commands) {
    sg_pipeline_desc pipeline = {};
    sg_bindings bindings = {};

    pipeline.shader = _sceneShader;
    pipeline.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;

    auto vs_params = scene_vs_params_t {};
    const auto m = command.render_state.get_projection_matrix();
    if (m) {
      std::copy(&m->_11, &m->_11 + 16, vs_params.projection_matrix);
    } else {
      std::copy(_projectionMatrix.begin(), _projectionMatrix.end(), vs_params.projection_matrix);
    }

    switch (command.render_state.get_option(D3DRENDERSTATE_CULLMODE)) {
      case D3DCULL_CW: {
        pipeline.cull_mode = SG_CULLMODE_BACK;
        pipeline.face_winding = SG_FACEWINDING_CCW;
        break;
      }

      case D3DCULL_NONE: {
        pipeline.cull_mode = SG_CULLMODE_NONE;
        pipeline.face_winding = SG_FACEWINDING_CCW;
        break;
      }

      default:
        break;
    }

    pipeline.depth.write_enabled = command.render_state.get_option(D3DRENDERSTATE_ZWRITEENABLE);
    pipeline.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;

    if (command.render_state.get_option(D3DRENDERSTATE_ALPHABLENDENABLE)) {
      pipeline.colors[0].blend.enabled = true;

      const auto source_blend = command.render_state.get_option(D3DRENDERSTATE_SRCBLEND);
      switch (source_blend) {
        case D3DBLEND_SRCALPHA:
          pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
          pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
          break;

        case D3DBLEND_INVSRCALPHA:
          pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
          pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
          break;

        case D3DBLEND_ONE:
          pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_ONE;
          pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_ONE;
          break;

        default:
          break;
      }

      const auto destination_blend = command.render_state.get_option(D3DRENDERSTATE_DESTBLEND);
      switch (destination_blend) {
        case D3DBLEND_SRCALPHA:
          pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
          pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
          break;

        case D3DBLEND_INVSRCALPHA:
          pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
          pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
          break;

        case D3DBLEND_ONE:
          pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE;
          pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE;
          break;

        default:
          break;
      }
    }

    const auto alpha_test_enabled = command.render_state.get_option(D3DRENDERSTATE_ALPHATESTENABLE);
    const auto alpha_reference = command.render_state.get_option(D3DRENDERSTATE_ALPHAREF);

    for (uint32_t i = 0; i < 2; i++) {
      bindings.fs.images[i] = _nullTexture;

      auto texture_handle = command.render_state.get_texture(i);
      if (texture_handle) {
        auto texture = _texture_manager->get(*texture_handle);
        if (texture != nullptr) {
          bindings.fs.images[i] = *texture;
        }
      }
    }

    const auto texture_address = command.render_state.get_texture_stage_state(0, D3DTSS_ADDRESS);
    if (texture_address) {
      switch (*texture_address) {
        case D3DTADDRESS_CLAMP:
          bindings.fs.samplers[0] = _clamp_sampler;
          break;

        case D3DTADDRESS_WRAP:
          bindings.fs.samplers[0] = _repeat_sampler;
          break;

        default:
          break;
      }
    } else {
      bindings.fs.samplers[0] = _repeat_sampler;
    }

    auto parameters = command.render_state.get_fragment_shader_parameters();
    auto fs_params = scene_fs_params_t{
        .color_operation_1 = (int) parameters.color_operation_1,
        .color_operation_2 = (int) parameters.color_operation_2,
        .alpha_test_enabled = (int) alpha_test_enabled,
        .alpha_reference = (int) alpha_reference
    };

    pipeline.layout.attrs[ATTR_scene_vs_pos].buffer_index = 0;
    pipeline.layout.attrs[ATTR_scene_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;

    pipeline.layout.attrs[ATTR_scene_vs_color0].buffer_index = 1;
    pipeline.layout.attrs[ATTR_scene_vs_color0].format = SG_VERTEXFORMAT_FLOAT4;

    pipeline.layout.attrs[ATTR_scene_vs_uv0].buffer_index = 2;
    pipeline.layout.attrs[ATTR_scene_vs_uv0].format = SG_VERTEXFORMAT_FLOAT2;

    bindings.vertex_buffers[0] = sg_position_buffer;
    bindings.vertex_buffers[1] = sg_color_buffer;
    bindings.vertex_buffers[2] = sg_uv_buffer;

    if (command.index_buffer_view.length == 0) {
      const auto count = command.vertex_buffer_view.length;
      const auto base_element = command.vertex_buffer_view.offset;
      assert(count > 0);

      bindings.vertex_buffer_offsets[0] = 0;

      auto pip = sg_make_pipeline(pipeline);
      sg_apply_pipeline(pip);
      sg_apply_bindings(bindings);
      sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_scene_vs_params, SG_RANGE(vs_params));
      sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_scene_fs_params, SG_RANGE(fs_params));

      sg_draw(base_element, count, 1);
      sg_destroy_pipeline(pip);
    } else {
      const auto count = command.index_buffer_view.length;
      const auto base_element = command.index_buffer_view.offset; // * sizeof(uint32_t);
      assert(count > 0);

      pipeline.index_type = SG_INDEXTYPE_UINT32;
      bindings.index_buffer = sg_index_buffer;
      bindings.index_buffer_offset = 0;

      auto pip = sg_make_pipeline(pipeline);
      sg_apply_pipeline(pip);
      sg_apply_bindings(bindings);
      sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_scene_vs_params, SG_RANGE(vs_params));
      sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_scene_fs_params, SG_RANGE(fs_params));

      sg_draw(base_element, count, 1);
      sg_destroy_pipeline(pip);
    }
  }

  sg_end_pass();
  sg_commit();

  if (defaultPassAction.colors[0].load_action == SG_LOADACTION_CLEAR) {
    defaultPassAction.colors[0].load_action = SG_LOADACTION_DONTCARE;
  }

  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetProjectionMatrix(const D3DMATRIX &matrix) {
  _render_state.set_projection_matrix(matrix);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dResetProjectionMatrix() {
  _render_state.reset_projection_matrix();
  return MD3D_OK;
}

MD3DERROR Renderer::d3dClear(uint32_t dwColor) {
  const auto red = static_cast<float>((dwColor >> 16) & 0xFF) / 255.0f;
  const auto green = static_cast<float>((dwColor >> 8) & 0xFF) / 255.0f;
  const auto blue = static_cast<float>(dwColor & 0xFF) / 255.0f;

  defaultPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
  defaultPassAction.colors[0].clear_value = {red, green, blue, 1};
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t dwRenderState) {
  _render_state.set_option(dwRenderStateType, dwRenderState);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t* lpdwRenderState) {
  *lpdwRenderState = _render_state.get_option(dwRenderStateType);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue) {
  _render_state.set_texture_stage_state(dwStage, dwState, dwValue);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dTriangleFan(uint32_t dwVertexTypeDesc, void *lpvVertices, uint32_t dwVertexCount) {
  // The system uses vertices v2, v3, and v1 to draw the first triangle;
  // v3, v4, and v1 to draw the second triangle; v4, v5, and v1 to draw the third triangle; and so on.
  // When flat shading is enabled, the system shades the triangle with the color from its first vertex.

  if (dwVertexCount < 3) {
    return MD3DERR_ILLEGALCALL;
  }

  prepare_render_state(0);

  for (uint32_t i = 1; i < dwVertexCount - 1; i++) {
    add_vertex(dwVertexTypeDesc, lpvVertices, 0);
    add_vertex(dwVertexTypeDesc, lpvVertices, i);
    add_vertex(dwVertexTypeDesc, lpvVertices, i + 1);
  }

  return MD3D_OK;
}

MD3DERROR Renderer::d3dTrianglesIndexed(uint32_t dwVertexTypeDesc, void *lpvVertices, uint32_t dwVertexCount,
                                        uint16_t *lpwIndices, uint32_t dwIndexCount) {
  prepare_render_state(dwVertexCount);

  auto& last_command = _commands.back();
  const auto vertex_offset = last_command.vertex_buffer_view.end();

  for (uint32_t i = 0; i < dwVertexCount; i++) {
    add_vertex(dwVertexTypeDesc, lpvVertices, i);
  }

  const auto start_index = last_command.index_buffer_view.end();
  assert(start_index + dwIndexCount < _index_buffer.size());
  auto p = _index_buffer.data();
  for (uint32_t i = 0; i < dwIndexCount; i++) {
    p[start_index + i] = vertex_offset + lpwIndices[i];
  }
  last_command.index_buffer_view.length += dwIndexCount;

  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTexture(uint32_t dwHandle, uint32_t dwStage) {
  _render_state.set_texture(dwHandle, dwStage);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) {
  switch (tbRGBBlend) {
    case MD3DTB_DIFFUSE:
      d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
      break;
    case MD3DTB_TEXTURE1:
      d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      break;
    case MD3DTB_TEXTURE1_MOD_DIFFUSE:
      d3dSetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      break;
    default:
      return MD3DERR_INVALIDPARAM;
  }

  switch (tbAlphaBlend) {
    case MD3DTB_DIFFUSE:
      d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
      break;
    case MD3DTB_TEXTURE1:
      d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      break;
    case MD3DTB_TEXTURE1_MOD_DIFFUSE:
      d3dSetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      break;
    default:
      return MD3DERR_INVALIDPARAM;
  }

  return MD3D_OK;
}

MD3DERROR Renderer::d3dTrianglesIndexed2(uint32_t dwVertexTypeDesc, void *lpvVertices, uint32_t dwVertexCount,
                                         uint16_t *lpwIndices, uint32_t dwIndexCount, uint32_t dwHandleTex0,
                                         uint32_t dwHandleTex1) {
  d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

  d3dSetTexture(dwHandleTex0, 0);
  d3dSetTexture(dwHandleTex1, 1);
  auto result = d3dTrianglesIndexed(dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount);

  d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  return result;
}

MD3DERROR Renderer::d3dLockBackBuffer(void **lplpSurface, uint32_t *lpdwPitch) {
  const auto address = _backBuffer->lock();
  *lplpSurface = address.address;
  *lpdwPitch = address.pitch;
  return MD3D_OK;
}

MD3DERROR Renderer::d3dUnlockBackBuffer() {
  _backBuffer->unlock();
  return MD3D_OK;
}

MD3DERROR Renderer::d3dFlushBackBuffer(MD3DRECT *lprcRect) {
  _is_back_buffer_flush = true;
  return MD3D_OK;
}

void Renderer::prepare_render_state(size_t index_count) {
  if (_commands.empty()) {
    _commands.emplace_back(DrawCommand{_render_state, BufferView{0, 0}, BufferView{0, 0}});
    return;
  }

  const auto& last_command = _commands.back();

  bool use_index = index_count > 0;
  bool last_use_index = last_command.index_buffer_view.length > 0;

  if (_render_state == last_command.render_state && last_use_index == use_index) {
    return;
  }

  _commands.emplace_back(
      DrawCommand{_render_state, last_command.vertex_buffer_view.next(), last_command.index_buffer_view.next()});
}

void Renderer::add_vertex(uint32_t vertex_type, void *vertices, uint32_t index) {
  assert(!_commands.empty());

  auto& last_command = _commands.back();
  size_t i = last_command.vertex_buffer_view.end();

  VertexType type{vertex_type};
  auto vertex_offset = static_cast<uint8_t*>(vertices) + type.get_vertex_size() * index;

  if (vertex_type & D3DFVF_XYZRHW) {
    auto p = reinterpret_cast<const float*>(vertex_offset);

    auto position = _position_buffer.data();
    const auto offset = 3 * i;
    assert(offset + 3 < _position_buffer.size());
    position[offset] = p[0];
    position[offset + 1] = p[1];
    position[offset + 2] = p[2];

    vertex_offset += type.get_offset(D3DFVF_XYZRHW);
  }

  if (vertex_type & D3DFVF_DIFFUSE) {
    auto p = reinterpret_cast<const uint32_t*>(vertex_offset);
    const auto argb = p[0];

    auto color = _color_buffer.data();
    const auto offset = 4 * i;
    assert(offset + 4 < _color_buffer.size());    
    auto a = static_cast<float>((argb >> 24) & 0xFF) / 255.0f;
    auto r = static_cast<float>((argb >> 16) & 0xFF) / 255.0f;
    auto g = static_cast<float>((argb >> 8) & 0xFF) / 255.0f;
    auto b = static_cast<float>(argb & 0xFF) / 255.0f;
    color[offset] = r;
    color[offset + 1] = g;
    color[offset + 2] = b;
    color[offset + 3] = a;

    vertex_offset += type.get_offset(D3DFVF_DIFFUSE);
  }

  if (vertex_type & D3DFVF_SPECULAR) {
    vertex_offset += type.get_offset(D3DFVF_SPECULAR);
  }

  if (vertex_type & D3DFVF_TEX1) {
    auto p = reinterpret_cast<const float*>(vertex_offset);

    auto uv = _uv_buffer.data();
    const auto offset = 2 * i;
    assert(offset + 2 < _uv_buffer.size());
    uv[offset] = p[0];
    uv[offset + 1] = p[1];
  }

  last_command.vertex_buffer_view.length += 1;
}
