//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <sokol_gfx.h>

#include <cassert>

#include "vertex_type.h"

using namespace graphics;

float* make_ortho_projection(float left, float right, float bottom, float top, float near, float far);

Renderer::Renderer() : _texture_manager(std::make_unique<TextureManager>()) {
  setup_depth_states();
  setup_pipeline_states();

  {
    const auto length = _vertex_count * 3 * sizeof(float);
    _position_buffer.reset(new float[length]);
    sg_position_buffer = sg_make_buffer(sg_buffer_desc{
        .size = length,
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    const auto length = _vertex_count * 4 * sizeof(float);
    _color_buffer.reset(new float[length]);
    sg_color_buffer = sg_make_buffer(sg_buffer_desc{
        .size = length,
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    const auto length = _vertex_count * 2 * sizeof(float);
    _uv_buffer.reset(new float[length]);
    sg_uv_buffer = sg_make_buffer(sg_buffer_desc{
        .size = length,
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }

  {
    const auto length = _vertex_count * 3 * sizeof(uint32_t);
    _index_buffer.reset(new uint32_t[length]);
    sg_index_buffer = sg_make_buffer(sg_buffer_desc{
        .size = length,
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_DYNAMIC,
    });
  }
}

void Renderer::setup_depth_states() {}

void Renderer::setup_pipeline_states() {
  //  sg_pipeline_desc pipeline = {};
  //
  //  pipeline.shader = shader;
  //  pipeline.layout.attrs[ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;
  //  pipeline.layout.attrs[ATTR_vs_pos].offset = 0;
  //  pipeline.layout.attrs[ATTR_vs_pos].buffer_index = 0;
  //
  //  defaultPipeline = sg_make_pipeline(pipeline);

  //  pipeline.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
  //  pipeline.layout.attrs[1].offset = 0;
  //  pipeline.layout.attrs[1].buffer_index = 1;
  //
  //  pipeline.layout.attrs[2].format = SG_VERTEXFORMAT_FLOAT2;
  //  pipeline.layout.attrs[2].offset = 0;
  //  pipeline.layout.attrs[2].buffer_index = 2;
  //
  //  pipeline.layout.buffers[0].stride = 12;
  //  pipeline.layout.buffers[0].step_func = SG_VERTEXSTEP_PER_VERTEX;
  //
  //  pipeline.layout.buffers[1].stride = 16;
  //  pipeline.layout.buffers[1].step_func = SG_VERTEXSTEP_PER_VERTEX;
  //
  //  pipeline.layout.buffers[2].stride = 8;
  //  pipeline.layout.buffers[2].step_func = SG_VERTEXSTEP_PER_VERTEX;
  //
  //  {
  //    pipeline.colors[0].pixel_format = SG_PIXELFORMAT_BGRA8;
  //    pipeline.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
  //    defaultPipeline = sg_make_pipeline(pipeline);
  //  }
  //
  //  {
  //    pipeline.colors[0].pixel_format = SG_PIXELFORMAT_BGRA8;
  //    pipeline.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
  //    pipeline.colors[0].blend.enabled = true;
  //    pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
  //    pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
  //
  //    pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
  //    pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
  //
  //    blendingPipeline = sg_make_pipeline(pipeline);
  //  }
}

MD3DERROR Renderer::d3dBeginScene() {
  _commands.clear();
  return MD3D_OK;
}

MD3DERROR Renderer::d3dEndScene() {
  if (_commands.empty()) {
    return MD3D_OK;
  }

  static sg_shader shader = sg_make_shader(samogonki_shader_desc(sg_query_backend()));
  static sg_range nullData = {
      .ptr = new float[4] { 0.0f, 0.0f, 0.0f, 0.0f},
      .size = 4,
  };
  static sg_image nullTexture = sg_make_image(sg_image_desc {
      .width = 1,
      .height = 1,
      .usage = SG_USAGE_IMMUTABLE,
      .pixel_format = SG_PIXELFORMAT_RGBA8,
      .data = nullData,
  });

  auto projection_matrix = make_ortho_projection(0, 800, 600, 0, -1, 1);
  auto vs_params = vs_params_t {};
  for (int i = 0; i < 16; ++i) {
    vs_params.projection_matrix[i] = projection_matrix[i];
  }
  delete[] projection_matrix;

  // TODO: set actual screen size
  sg_begin_default_pass(defaultPassAction, 800, 600);

  // prepare pipeline
  //  [render_encoder setVertexBuffer:_position_buffer.get() offset:0 atIndex:0];
  //  [render_encoder setVertexBuffer:_color_buffer.get() offset:0 atIndex:1];
  //  [render_encoder setVertexBuffer:_uv_buffer.get() offset:0 atIndex:2];
  //
  //  [render_encoder setVertexBytes:&_uniforms length:sizeof(BasicUniforms) atIndex:3];

  sg_update_buffer(sg_position_buffer, sg_range{
                                           .ptr = _position_buffer.get(),
                                           .size = _vertex_count * 3 * sizeof(float),
                                       });
  sg_update_buffer(sg_color_buffer, sg_range{
                                        .ptr = _color_buffer.get(),
                                        .size = _vertex_count * 4 * sizeof(float),
                                    });

  sg_update_buffer(sg_uv_buffer, sg_range{
                                     .ptr = _uv_buffer.get(),
                                     .size = _vertex_count * 2 * sizeof(float),
                                 });

  sg_update_buffer(sg_index_buffer, sg_range{
                                        .ptr = _position_buffer.get(),
                                        .size = _vertex_count * 3 * sizeof(uint32_t),
                                    });

  for (const auto& command : _commands) {
    sg_pipeline_desc pipeline = {};
    sg_bindings bindings = {};

    pipeline.shader = shader;
    pipeline.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;

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

    if (command.render_state.get_option(D3DRENDERSTATE_ALPHABLENDENABLE)) {
      pipeline.colors[0].blend.enabled = true;
      pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
      pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

      pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
      pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    }

    for (DWORD i = 0; i < 2; i++) {
      bindings.fs_images[i] = nullTexture;

      auto texture_handle = command.render_state.get_texture(i);
      if (texture_handle) {
        auto texture = _texture_manager->get(*texture_handle);
        if (texture != nullptr) {
          bindings.fs_images[i] = *texture;
        }
      }
    }

    auto parameters = command.render_state.get_fragment_shader_parameters();
    auto fs_params = fs_params_t{
        .color_operation_1 = (int) parameters.color_operation_1,
        .color_operation_2 = (int) parameters.color_operation_2,
    };

    pipeline.layout.attrs[ATTR_vs_pos].buffer_index = 0;
    pipeline.layout.attrs[ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;

    pipeline.layout.attrs[ATTR_vs_color0].buffer_index = 1;
    pipeline.layout.attrs[ATTR_vs_color0].format = SG_VERTEXFORMAT_FLOAT4;

    pipeline.layout.attrs[ATTR_vs_uv0].buffer_index = 2;
    pipeline.layout.attrs[ATTR_vs_uv0].format = SG_VERTEXFORMAT_FLOAT2;

    bindings.vertex_buffers[0] = sg_position_buffer;
    bindings.vertex_buffers[1] = sg_color_buffer;
    bindings.vertex_buffers[2] = sg_uv_buffer;

    if (command.index_buffer_view.length == 0) {
      const auto count = command.vertex_buffer_view.length;
      const auto offset = command.vertex_buffer_view.offset;
      assert(count > 0);

      bindings.vertex_buffer_offsets[0] = 0;

      auto pip = sg_make_pipeline(pipeline);
      sg_apply_pipeline(pip);
      sg_apply_bindings(bindings);
      sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
      sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));

      sg_draw(offset, count, 1);
      sg_destroy_pipeline(pip);
    } else {
      const auto count = command.index_buffer_view.length;
      const auto offset = command.index_buffer_view.offset; // * sizeof(uint32_t);
      assert(count > 0);

      pipeline.index_type = SG_INDEXTYPE_UINT32;
      bindings.index_buffer = sg_index_buffer;
      bindings.index_buffer_offset = 0;

      auto pip = sg_make_pipeline(pipeline);
      sg_apply_pipeline(pip);
      sg_apply_bindings(bindings);
      sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
      sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));

      sg_draw(offset, count, 1);
      sg_destroy_pipeline(pip);
    }
  }

  sg_end_pass();
  sg_commit();
  return MD3D_OK;
}

MD3DERROR Renderer::d3dClear(DWORD dwColor) {
  const auto red = static_cast<float>((dwColor >> 16) & 0xFF) / 255.0f;
  const auto green = static_cast<float>((dwColor >> 8) & 0xFF) / 255.0f;
  const auto blue = static_cast<float>(dwColor & 0xFF) / 255.0f;

  defaultPassAction.colors[0].action = SG_ACTION_CLEAR;
  defaultPassAction.colors[0].value = {red, green, blue, 1};
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) {
  _render_state.set_option(dwRenderStateType, dwRenderState);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD* lpdwRenderState) {
  *lpdwRenderState = _render_state.get_option(dwRenderStateType);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) {
  _render_state.set_texture_stage_state(dwStage, dwState, dwValue);
  return MD3D_OK;
}
MD3DERROR Renderer::d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) {
  // The system uses vertices v2, v3, and v1 to draw the first triangle;
  // v3, v4, and v1 to draw the second triangle; v4, v5, and v1 to draw the third triangle; and so on.
  // When flat shading is enabled, the system shades the triangle with the color from its first vertex.

  if (dwVertexCount < 3) {
    return MD3DERR_ILLEGALCALL;
  }

  prepare_render_state(0);

  for (DWORD i = 1; i < dwVertexCount - 1; i++) {
    add_vertex(dwVertexTypeDesc, lpvVertices, 0);
    add_vertex(dwVertexTypeDesc, lpvVertices, i);
    add_vertex(dwVertexTypeDesc, lpvVertices, i + 1);
  }

  return MD3D_OK;
}
MD3DERROR Renderer::d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                        LPWORD lpwIndices, DWORD dwIndexCount) {
  prepare_render_state(dwVertexCount);

  auto& last_command = _commands.back();
  const auto vertex_offset = last_command.vertex_buffer_view.end();

  for (DWORD i = 0; i < dwVertexCount; i++) {
    add_vertex(dwVertexTypeDesc, lpvVertices, i);
  }

  const auto start_index = last_command.index_buffer_view.end();
  auto p = _index_buffer.get();
  for (DWORD i = 0; i < dwIndexCount; i++) {
    p[start_index + i] = vertex_offset + lpwIndices[i];
  }
  last_command.index_buffer_view.length += dwIndexCount;

  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTexture(DWORD dwHandle, DWORD dwStage) {
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

MD3DERROR Renderer::d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                         LPWORD lpwIndices, DWORD dwIndexCount, DWORD dwHandleTex0,
                                         DWORD dwHandleTex1) {
  d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

  d3dSetTexture(dwHandleTex0, 0);
  d3dSetTexture(dwHandleTex1, 1);
  auto result = d3dTrianglesIndexed(dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount);

  d3dSetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  d3dSetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  return result;
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

void Renderer::add_vertex(DWORD vertex_type, LPVOID vertices, DWORD index) {
  assert(!_commands.empty());

  auto& last_command = _commands.back();
  size_t i = last_command.vertex_buffer_view.end();

  VertexType type{vertex_type};
  auto vertex_offset = static_cast<uint8_t*>(vertices) + type.get_vertex_size() * index;

  if (vertex_type & D3DFVF_XYZRHW) {
    auto p = reinterpret_cast<const float*>(vertex_offset);

    auto position = _position_buffer.get();
    position[3 * i] = p[0];
    position[3 * i + 1] = p[1];
    position[3 * i + 2] = p[2];

    vertex_offset += type.get_offset(D3DFVF_XYZRHW);
  }

  if (vertex_type & D3DFVF_DIFFUSE) {
    auto p = reinterpret_cast<const DWORD*>(vertex_offset);
    const auto argb = p[0];

    auto color = _color_buffer.get();
    auto a = static_cast<float>((argb >> 24) & 0xFF) / 255.0f;
    auto r = static_cast<float>((argb >> 16) & 0xFF) / 255.0f;
    auto g = static_cast<float>((argb >> 8) & 0xFF) / 255.0f;
    auto b = static_cast<float>(argb & 0xFF) / 255.0f;
    color[4 * i] = r;
    color[4 * i + 1] = g;
    color[4 * i + 2] = b;
    color[4 * i + 3] = a;

    vertex_offset += type.get_offset(D3DFVF_DIFFUSE);
  }

  if (vertex_type & D3DFVF_SPECULAR) {
    vertex_offset += type.get_offset(D3DFVF_SPECULAR);
  }

  if (vertex_type & D3DFVF_TEX1) {
    auto p = reinterpret_cast<const float*>(vertex_offset);

    auto uv = _uv_buffer.get();
    uv[2 * i] = p[0];
    uv[2 * i + 1] = p[1];
  }

  last_command.vertex_buffer_view.length += 1;
}

TextureManagerInterface& Renderer::get_texture_manager() { return *_texture_manager; }

float* matrix_make_rows(float m00, float m10, float m20, float m30, float m01, float m11, float m21, float m31,
                        float m02, float m12, float m22, float m32, float m03, float m13, float m23, float m33) {
  // clang-format off
  return new float[16] {
           m00, m01, m02, m03 ,
           m10, m11, m12, m13 ,
           m20, m21, m22, m23 ,
           m30, m31, m32, m33
  };
  // clang-format on
}

float* make_ortho_projection(float left, float right, float bottom, float top, float near, float far) {
  // clang-format off
  return matrix_make_rows(
      2.0f / (right - left), 0, 0, (left + right) / (left - right),
      0, 2.0f / (top - bottom), 0, (top + bottom) / (bottom - top),
      0, 0, 1.0f / (far - near), near / (near - far),
      0, 0, 0, 1.0f
  );
  // clang-format on
}
