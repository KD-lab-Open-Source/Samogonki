#include "Md3d.h"

#include <cassert>

#include "renderer.h"
#include "texture_manager.h"

using namespace graphics;

std::unique_ptr<Renderer> renderer;

bool g_bInitialized = false;
bool g_bInScene = false;  // TRUE when in BeginScene/EndScene bracket

bool g_bActive = false;

extern MD3DERROR __d3dInitSpriteManager();

MD3DERROR d3dEnumVideoMode(int *pNumVideoMode, MD3DMODE **ppArray) {
  *pNumVideoMode = 6;
  auto modes = new MD3DMODE[*pNumVideoMode];

  modes[0].dx = 640;
  modes[0].dy = 480;
  modes[0].bitPerPixel = 32;

  modes[1].dx = 800;
  modes[1].dy = 600;
  modes[1].bitPerPixel = 32;

  modes[2].dx = 1024;
  modes[2].dy = 768;
  modes[2].bitPerPixel = 32;

  modes[3].dx = 1152;
  modes[3].dy = 864;
  modes[3].bitPerPixel = 32;

  modes[4].dx = 1280;
  modes[4].dy = 1024;
  modes[4].bitPerPixel = 32;

  modes[5].dx = 1600;
  modes[5].dy = 1200;
  modes[5].bitPerPixel = 32;

  *ppArray = modes;
  return MD3D_OK;
}

MD3DERROR d3dGetAvailableVidMem(uint32_t* allVideoMem) { return MD3D_OK; }

MD3DERROR d3dInit(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwBpp, uint32_t dwInitFlags, void* hInst, void* hIcon, char* szTitle) {
  assert(!g_bInitialized);

  MD3DERROR hr = MD3D_OK;

  renderer = std::make_unique<Renderer>(dwWidth, dwHeight, dwInitFlags & MD3D_FULLSCREEN);

  // Initialize sprite manager
  if ((hr = __d3dInitSpriteManager()) < 0) {
    return hr;
  }

  g_bInitialized = true;
  g_bActive = true;

  return hr;
}

MD3DERROR d3dReInit(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwBpp, uint32_t dwInitFlags, void* hInst, void* hIcon, char* szTitle) {
  assert(g_bInitialized);

  renderer->setVideoMode(dwWidth, dwHeight, dwInitFlags & MD3D_FULLSCREEN);
  return MD3D_OK;
}

MD3DERROR d3dClose() {
  assert(g_bInitialized);
  g_bInitialized = false;
  return MD3D_OK;
}

MD3DERROR d3dQueryCaps(MD3DCAPS Caps, uint32_t* dwData) { return MD3D_OK; }

MD3DERROR d3dGetWindowHandle(void* hWnd) { return MD3D_OK; }

MD3DERROR d3dClear(uint32_t dwColor) {
  return renderer->d3dClear(dwColor);
}

MD3DERROR d3dFlip(bool WaitVerticalBlank) {
  return renderer->d3dFlip(WaitVerticalBlank);
}

MD3DERROR d3dFlipToGdiSurface() { return MD3D_OK; }

MD3DERROR d3dCreateBackBuffer() { return MD3D_OK; }

MD3DERROR d3dReleaseBackBuffer() { return MD3D_OK; }

MD3DERROR d3dGetBackBufferFormat(uint32_t *dwFormat) { return MD3D_OK; }

MD3DERROR d3dLockBackBuffer(void** lplpSurface, uint32_t* lpdwPitch) {
  return renderer->d3dLockBackBuffer(lplpSurface, lpdwPitch);
}

MD3DERROR d3dUnlockBackBuffer() {
  return renderer->d3dUnlockBackBuffer();
}

MD3DERROR d3dFlushBackBuffer(MD3DRECT* lprcRect) {
  return renderer->d3dFlushBackBuffer(lprcRect);
}

MD3DERROR d3dSetBackBufferColorKey(uint32_t dwColor) { return MD3D_OK; }

MD3DERROR d3dEnableBackBufferColorKey(bool bEnable) { return MD3D_OK; }

MD3DERROR d3dScreenShot(void* lpBuffer, uint32_t dwSize) { return MD3D_OK; }

MD3DERROR d3dEndScene() {
  if (!g_bInScene) {
    return MD3D_OK;
  }

  g_bInScene = false;

  return renderer->d3dEndScene();
}

MD3DERROR d3dBeginScene() {
  if (g_bInScene) {
    return MD3D_OK;
  }

  g_bInScene = true;

  return renderer->d3dBeginScene();
}

MD3DERROR d3dTestCooperativeLevel() { return MD3D_OK; }

MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t dwRenderState) {
  return renderer->d3dSetRenderState(dwRenderStateType, dwRenderState);
}

MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t *lpdwRenderState) {
  return renderer->d3dGetRenderState(dwRenderStateType, lpdwRenderState);
}

MD3DERROR d3dSetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue) {
  return renderer->d3dSetTextureStageState(dwStage, dwState, dwValue);
}

MD3DERROR d3dTriangles(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount) { return MD3D_OK; }

MD3DERROR d3dTriangleStrip(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount) { return MD3D_OK; }

MD3DERROR d3dTriangleFan(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount) {
  return renderer->d3dTriangleFan(dwVertexTypeDesc, lpvVertices, dwVertexCount);
}

MD3DERROR d3dPoints(uint32_t, void*, uint32_t) { return MD3D_OK; }

MD3DERROR d3dTrianglesIndexed(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount, uint16_t* lpwIndices, uint32_t dwIndexCount) {
  return renderer->d3dTrianglesIndexed(dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount);
}

MD3DERROR d3dGetTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dGetTextureFormatData(dwTexFormatID, pData);
}

MD3DERROR d3dCreateTexture(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwFormat, uint32_t* lpdwHandle) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dCreateTexture(dwWidth, dwHeight, dwFormat, lpdwHandle);
}

MD3DERROR d3dDeleteTexture(uint32_t dwHandle) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dDeleteTexture(dwHandle);
}

MD3DERROR d3dLockTexture(uint32_t dwHandle, void** lplpTexture, uint32_t* lpPitch) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dLockTexture(dwHandle, lplpTexture, lpPitch);
}

MD3DERROR d3dLockTexture(uint32_t dwHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwRight, uint32_t dwBottom, void** lplpTexture, uint32_t* lpPitch) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dLockTexture(dwHandle, dwLeft, dwTop, dwRight, dwBottom, lplpTexture, lpPitch);
}

MD3DERROR d3dUnlockTexture(uint32_t dwHandle) {
  auto &texture_manager = renderer->get_texture_manager();
  return texture_manager.d3dUnlockTexture(dwHandle);
}

MD3DERROR d3dSetTexture(uint32_t dwHandle, uint32_t dwStage) {
  return renderer->d3dSetTexture(dwHandle, dwStage);
}

MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) {
  return renderer->d3dSetTextureBlendMode(tbRGBBlend, tbAlphaBlend);
}

MD3DERROR d3dSetAdjustedGamma(float fRGamma, float fGGamma, float fBGamma) { return MD3D_OK; }

MD3DERROR d3dGetAdjustedGamma(float* pfRGamma, float* pfGGamma, float* pfBGamma) { return MD3D_OK; }

MD3DERROR d3dSetGammaFxHighlight(float fRHilight, float fGHilight, float fBHilight) { return MD3D_OK; }

MD3DERROR d3dGetGammaFxHighlight(float* pfRHilight, float* pfGHilight, float* pfBHilight) { return MD3D_OK; }

MD3DERROR d3dSetGammaFxShadow(float fRShadow, float fGShadow, float fBShadow) { return MD3D_OK; }

MD3DERROR d3dGetGammaFxShadow(float* pfRShadow, float* pfGShadow, float* pfBShadow) { return MD3D_OK; }

MD3DERROR d3dSetProjectionMatrix(float fFOV, float fNearPlane, float fFarPlane) { return MD3D_OK; }

MD3DERROR d3dSetProjectionMatrixToIdentity() { return MD3D_OK; }

MD3DERROR d3dSetFogParameters(uint32_t dwMode, uint32_t dwColor, float fStart, float fEnd, float fDensity) { return MD3D_OK; }

MD3DERROR d3dEnableFog(bool bEnable) { return MD3D_OK; }

MD3DERROR d3dSetFocusLossBehavior(bool bSleep) { return MD3D_OK; }

bool d3dIsActive() { return g_bActive; }

MD3DERROR d3dSetClipRect(MD3DRECT* lprcClipRect) { return MD3D_OK; }

MD3DERROR d3dTrianglesIndexed2(uint32_t dwVertexTypeDesc, void* lpvVertices, uint32_t dwVertexCount, uint16_t* lpwIndices, uint32_t dwIndexCount, uint32_t dwHandleTex0, uint32_t dwHandleTex1) {
  assert(g_bInitialized);
  assert(g_bInScene);

  return renderer->d3dTrianglesIndexed2(dwVertexTypeDesc, lpvVertices, dwVertexCount, 
    lpwIndices, dwIndexCount, dwHandleTex0, dwHandleTex1);
}
