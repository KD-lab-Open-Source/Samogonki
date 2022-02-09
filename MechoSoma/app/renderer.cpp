//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <sokol_gfx.h>

#include <cassert>

#include "vertex_type.h"

using namespace graphics;

Renderer::Renderer() : _texture_manager(std::make_unique<TextureManager>()) {
  setup_depth_states();
  setup_pipeline_states();

  {
    const auto length = _vertex_count * 3 * sizeof(float);
    _position_buffer.reset(new float[length]);
  }

  {
    const auto length = _vertex_count * 4 * sizeof(float);
    _color_buffer.reset(new float[length]);
  }

  {
    const auto length = _vertex_count * 2 * sizeof(float);
    _uv_buffer.reset(new float[length]);
  }

  {
    const auto length = _vertex_count * 3 * sizeof(uint32_t);
    _index_buffer.reset(new uint32_t[length]);
  }
}

void Renderer::setup_depth_states() {}

void Renderer::setup_pipeline_states() {
  sg_shader shader = sg_make_shader(samogonki_shader_desc(sg_query_backend()));

  sg_pipeline_desc pipeline = {};

  pipeline.shader = shader;
  pipeline.layout.attrs[ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;
  pipeline.layout.attrs[ATTR_vs_pos].offset = 0;
  pipeline.layout.attrs[ATTR_vs_pos].buffer_index = 0;

  defaultPipeline = sg_make_pipeline(pipeline);

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
  // TODO: set actual screen size
  sg_begin_default_pass(defaultPassAction, 800, 600);
  return MD3D_OK;
}

MD3DERROR Renderer::d3dEndScene() {
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
    color[4 * i] = static_cast<float>((argb >> 16) & 0xFF) / 255.0f;
    color[4 * i + 1] = static_cast<float>((argb >> 8) & 0xFF) / 255.0f;
    color[4 * i + 2] = static_cast<float>(argb & 0xFF) / 255.0f;
    color[4 * i + 3] = static_cast<float>((argb >> 24) & 0xFF) / 255.0f;

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
