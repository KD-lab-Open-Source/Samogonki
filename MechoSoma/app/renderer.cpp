//
// Created by caiiiycuk on 07.02.22.
//

#include "renderer.h"

#include <sokol_gfx.h>

using namespace graphics;

std::unique_ptr<graphics::Renderer> Renderer::shared;

RendererInterface& graphics::get_renderer() { return *Renderer::shared; }

Renderer::Renderer() : _texture_manager(std::make_unique<TextureManager>()) {
  setup_depth_states();
  setup_pipeline_states();
}

void Renderer::setup_depth_states() {}

void Renderer::setup_pipeline_states() {
  sg_pipeline_desc pipeline;

  pipeline.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
  pipeline.layout.attrs[0].offset = 0;
  pipeline.layout.attrs[0].buffer_index = 0;

  pipeline.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
  pipeline.layout.attrs[1].offset = 0;
  pipeline.layout.attrs[1].buffer_index = 1;

  pipeline.layout.attrs[2].format = SG_VERTEXFORMAT_FLOAT2;
  pipeline.layout.attrs[2].offset = 0;
  pipeline.layout.attrs[2].buffer_index = 2;

  pipeline.layout.buffers[0].stride = 12;
  pipeline.layout.buffers[0].step_func = SG_VERTEXSTEP_PER_VERTEX;

  pipeline.layout.buffers[0].stride = 16;
  pipeline.layout.buffers[0].step_func = SG_VERTEXSTEP_PER_VERTEX;

  pipeline.layout.buffers[0].stride = 8;
  pipeline.layout.buffers[0].step_func = SG_VERTEXSTEP_PER_VERTEX;

  {
    pipeline.colors[0].pixel_format = SG_PIXELFORMAT_BGRA8;
    pipeline.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
    defaultPipeline = sg_make_pipeline(pipeline);
  }

  {
    pipeline.colors[0].pixel_format = SG_PIXELFORMAT_BGRA8;
    pipeline.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
    pipeline.colors[0].blend.enabled = true;
    pipeline.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    pipeline.colors[0].blend.src_factor_alpha = SG_BLENDFACTOR_SRC_ALPHA;
    pipeline.colors[0].blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    blendingPipeline = sg_make_pipeline(pipeline);
  }
}

MD3DERROR Renderer::d3dBeginScene() { return MD3D_OK; }

MD3DERROR Renderer::d3dEndScene() { return MD3D_OK; }

MD3DERROR Renderer::d3dClear(DWORD dwColor) { return MD3D_OK; }
MD3DERROR Renderer::d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) { return MD3D_OK; }
MD3DERROR Renderer::d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD* lpdwRenderState) { return MD3D_OK; }
MD3DERROR Renderer::d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) {
  return MD3D_OK;
}
MD3DERROR Renderer::d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) { return MD3D_OK; }
MD3DERROR Renderer::d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                        LPWORD lpwIndices, DWORD dwIndexCount) {
  return MD3D_OK;
}

MD3DERROR Renderer::d3dSetTexture(DWORD dwHandle, DWORD dwStage) { return MD3D_OK; }

MD3DERROR Renderer::d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) {
  return MD3D_OK;
}

MD3DERROR Renderer::d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                         LPWORD lpwIndices, DWORD dwIndexCount, DWORD dwHandleTex0,
                                         DWORD dwHandleTex1) {
  return MD3D_OK;
}

TextureManagerInterface& Renderer::get_texture_manager() { return *_texture_manager; }
