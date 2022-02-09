#pragma once

#include "Md3d.h"

namespace graphics {

struct TextureManagerInterface;

struct RendererInterface {
  virtual ~RendererInterface() = default;

  virtual TextureManagerInterface& get_texture_manager() = 0;

  virtual MD3DERROR d3dClear(DWORD dwColor) = 0;
  virtual MD3DERROR d3dBeginScene() = 0;
  virtual MD3DERROR d3dEndScene() = 0;
  virtual MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) = 0;
  virtual MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD* lpdwRenderState) = 0;
  virtual MD3DERROR d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) = 0;
  virtual MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) = 0;
  virtual MD3DERROR d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                        LPWORD lpwIndices, DWORD dwIndexCount) = 0;

  virtual MD3DERROR d3dSetTexture(DWORD dwHandle, DWORD dwStage) = 0;
  virtual MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) = 0;

  virtual MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount,
                                         LPWORD lpwIndices, DWORD dwIndexCount, DWORD dwHandleTex0,
                                         DWORD dwHandleTex1) = 0;
};

RendererInterface& get_renderer();

}  // namespace graphics
