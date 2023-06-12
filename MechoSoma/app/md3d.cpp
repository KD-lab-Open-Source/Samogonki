#include "Md3d.h"

#include <cassert>

#include "renderer.h"
#include "texture_manager.h"

using namespace graphics;

BOOL g_bInitialized = FALSE;
BOOL g_bInScene = FALSE;  // TRUE when in BeginScene/EndScene bracket

BOOL g_bActive = FALSE;

extern MD3DERROR __d3dInitSpriteManager();

MD3DERROR d3dEnumVideoMode(int *pnumvideomode, MD3DMODE **ppArray) { return MD3D_OK; }

MD3DERROR d3dGetAvailableVidMem(DWORD *allvideomem) { return MD3D_OK; }

MD3DERROR d3dInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon,
                  TCHAR *szTitle) {
  assert(!g_bInitialized);

  MD3DERROR hr = MD3D_OK;

  // Initialize sprite manager
  if (FAILED(hr = __d3dInitSpriteManager())) {
    return hr;
  }

  g_bInitialized = TRUE;
  g_bActive = TRUE;

  return hr;
}

MD3DERROR d3dReInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon,
                    TCHAR *szTitle) {
  return MD3D_OK;
}

MD3DERROR d3dClose() {
  assert(g_bInitialized);
  g_bInitialized = FALSE;
  return MD3D_OK;
}

MD3DERROR d3dQueryCaps(MD3DCAPS Caps, DWORD *dwData) { return MD3D_OK; }

MD3DERROR d3dGetWindowHandle(HWND *hWnd) { return MD3D_OK; }

MD3DERROR d3dClear(DWORD dwColor) {
  return Renderer::shared->d3dClear(dwColor);
}

MD3DERROR d3dFlip(bool WaitVerticalBlank) {
  return Renderer::shared->d3dFlip(WaitVerticalBlank);
}

MD3DERROR d3dFlipToGdiSurface() { return MD3D_OK; }

MD3DERROR d3dCreateBackBuffer() { return MD3D_OK; }

MD3DERROR d3dReleaseBackBuffer() { return MD3D_OK; }

MD3DERROR d3dGetBackBufferFormat(DWORD *dwFormat) { return MD3D_OK; }

MD3DERROR d3dLockBackBuffer(VOID **lplpSurface, DWORD *lpdwPitch) {
  return Renderer::shared->d3dLockBackBuffer(lplpSurface, lpdwPitch);
}

MD3DERROR d3dUnlockBackBuffer() {
  return Renderer::shared->d3dUnlockBackBuffer();
}

MD3DERROR d3dFlushBackBuffer(RECT *lprcRect) {
  return Renderer::shared->d3dFlushBackBuffer(lprcRect);
}

MD3DERROR d3dSetBackBufferColorKey(DWORD dwColor) { return MD3D_OK; }

MD3DERROR d3dEnableBackBufferColorKey(BOOL bEnable) { return MD3D_OK; }

MD3DERROR d3dScreenShot(VOID *lpBuffer, DWORD dwSize) { return MD3D_OK; }

MD3DERROR d3dEndScene() {
  if (g_bInScene == FALSE) {
    return MD3D_OK;
  }

  g_bInScene = FALSE;

  return Renderer::shared->d3dEndScene();
}

MD3DERROR d3dBeginScene() {
  if (g_bInScene == TRUE) {
    return MD3D_OK;
  }

  g_bInScene = TRUE;

  return Renderer::shared->d3dBeginScene();
}

MD3DERROR d3dTestCooperativeLevel() { return MD3D_OK; }

MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) {
  return Renderer::shared->d3dSetRenderState(dwRenderStateType, dwRenderState);
}

MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState) {
  return Renderer::shared->d3dGetRenderState(dwRenderStateType, lpdwRenderState);
}

MD3DERROR d3dSetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue) {
  return Renderer::shared->d3dSetTextureStageState(dwStage, dwState, dwValue);
}

MD3DERROR d3dTriangles(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) { return MD3D_OK; }

MD3DERROR d3dTriangleStrip(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) { return MD3D_OK; }

MD3DERROR d3dTriangleFan(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount) {
  return Renderer::shared->d3dTriangleFan(dwVertexTypeDesc, lpvVertices, dwVertexCount);
}

MD3DERROR d3dPoints(DWORD, LPVOID, DWORD) { return MD3D_OK; }

MD3DERROR d3dTrianglesIndexed(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                              DWORD dwIndexCount) {
  return Renderer::shared->d3dTrianglesIndexed(dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount);
}

MD3DERROR d3dGetTextureFormatData(DWORD dwTexFormatID, M3DTEXTUREFORMAT *pData) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dGetTextureFormatData(dwTexFormatID, pData);
}

MD3DERROR d3dCreateTexture(DWORD dwWidth, DWORD dwHeight, DWORD dwFormat, DWORD *lpdwHandle) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dCreateTexture(dwWidth, dwHeight, dwFormat, lpdwHandle);
}

MD3DERROR d3dDeleteTexture(DWORD dwHandle) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dDeleteTexture(dwHandle);
}

MD3DERROR d3dLockTexture(DWORD dwHandle, VOID **lplpTexture, DWORD *lpPitch) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dLockTexture(dwHandle, lplpTexture, lpPitch);
}

MD3DERROR d3dLockTexture(DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom, VOID **lplpTexture,
                         DWORD *lpPitch) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dLockTexture(dwHandle, dwLeft, dwTop, dwRight, dwBottom, lplpTexture, lpPitch);
}

MD3DERROR d3dUnlockTexture(DWORD dwHandle) {
  auto &texture_manager = Renderer::shared->get_texture_manager();
  return texture_manager.d3dUnlockTexture(dwHandle);
}

MD3DERROR d3dSetTexture(DWORD dwHandle, DWORD dwStage) {
  return Renderer::shared->d3dSetTexture(dwHandle, dwStage);
}

MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend) {
  return Renderer::shared->d3dSetTextureBlendMode(tbRGBBlend, tbAlphaBlend);
}

MD3DERROR d3dSetAdjustedGamma(float fRGamma, float fGGamma, float fBGamma) { return MD3D_OK; }

MD3DERROR d3dGetAdjustedGamma(float *pfRGamma, float *pfGGamma, float *pfBGamma) { return MD3D_OK; }

MD3DERROR d3dSetGammaFxHighlight(float fRHilight, float fGHilight, float fBHilight) { return MD3D_OK; }

MD3DERROR d3dGetGammaFxHighlight(float *pfRHilight, float *pfGHilight, float *pfBHilight) { return MD3D_OK; }

MD3DERROR d3dSetGammaFxShadow(float fRShadow, float fGShadow, float fBShadow) { return MD3D_OK; }

MD3DERROR d3dGetGammaFxShadow(float *pfRShadow, float *pfGShadow, float *pfBShadow) { return MD3D_OK; }

MD3DERROR d3dSetProjectionMatrix(FLOAT fFOV, FLOAT fNearPlane, FLOAT fFarPlane) { return MD3D_OK; }

MD3DERROR d3dSetProjectionMatrixToIdentity() { return MD3D_OK; }

MD3DERROR d3dSetFogParameters(DWORD dwMode, DWORD dwColor, FLOAT fStart, FLOAT fEnd, FLOAT fDensity) { return MD3D_OK; }

MD3DERROR d3dEnableFog(BOOL bEnable) { return MD3D_OK; }

MD3DERROR d3dSetFocusLossBehavior(BOOL bSleep) { return MD3D_OK; }

BOOL d3dIsActive() { return g_bActive; }

MD3DERROR d3dSetClipRect(RECT *lprcClipRect) { return MD3D_OK; }

MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, LPVOID lpvVertices, DWORD dwVertexCount, LPWORD lpwIndices,
                               DWORD dwIndexCount, DWORD dwHandleTex0, DWORD dwHandleTex1) {
  assert(g_bInitialized);
  assert(g_bInScene);

  return Renderer::shared->d3dTrianglesIndexed2(dwVertexTypeDesc, lpvVertices, dwVertexCount, 
                                                lpwIndices, dwIndexCount, dwHandleTex0, dwHandleTex1);
}
