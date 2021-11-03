#include "Md3d.h"

MD3DERROR d3dEnumVideoMode(int* pnumvideomode,MD3DMODE** ppArray)
{
	return MD3D_OK;
}

MD3DERROR d3dGetAvailableVidMem(DWORD* allvideomem)
{
	return MD3D_OK;
}

MD3DERROR d3dInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle)
{
	return MD3D_OK;
}

MD3DERROR d3dReInit(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwInitFlags, HINSTANCE hInst, HICON hIcon, TCHAR *szTitle)
{
	return MD3D_OK;
}

MD3DERROR d3dClose()
{
	return MD3D_OK;
}

MD3DERROR d3dQueryCaps(MD3DCAPS Caps, DWORD *dwData)
{
	return MD3D_OK;
}

MD3DERROR d3dGetWindowHandle(HWND *hWnd)
{
	return MD3D_OK;
}

MD3DERROR d3dClear(DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dFlip(bool WaitVerticalBlank)
{
	return MD3D_OK;
}

MD3DERROR d3dFlipToGdiSurface()
{
	return MD3D_OK;
}

MD3DERROR d3dCreateBackBuffer()
{
	return MD3D_OK;
}

MD3DERROR d3dReleaseBackBuffer()
{
	return MD3D_OK;
}

MD3DERROR d3dGetBackBufferFormat(DWORD *dwFormat)
{
	return MD3D_OK;
}

MD3DERROR d3dLockBackBuffer(VOID **lplpSurface, DWORD *lpdwPitch)
{
	return MD3D_OK;
}

MD3DERROR d3dUnlockBackBuffer()
{
	return MD3D_OK;
}

MD3DERROR d3dFlushBackBuffer(RECT *lprcRect)
{
	return MD3D_OK;
}

MD3DERROR d3dSetBackBufferColorKey(DWORD dwColor)
{
	return MD3D_OK;
}

MD3DERROR d3dEnableBackBufferColorKey(BOOL bEnable)
{
	return MD3D_OK;
}

MD3DERROR d3dScreenShot(VOID *lpBuffer, DWORD dwSize)
{
	return MD3D_OK;
}

MD3DERROR d3dEndScene()
{
	return MD3D_OK;
}

MD3DERROR d3dBeginScene()
{
	return MD3D_OK;
}

MD3DERROR d3dTestCooperativeLevel()
{
	return MD3D_OK;
}

MD3DERROR d3dSetRenderState(D3DRENDERSTATETYPE, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dGetRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD *lpdwRenderState)
{
	return MD3D_OK;
}

MD3DERROR d3dSetTextureStageState(DWORD dwStage,D3DTEXTURESTAGESTATETYPE dwState,DWORD dwValue)
{
	return MD3D_OK;
}

MD3DERROR d3dTriangles(DWORD, LPVOID, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dTriangleStrip(DWORD, LPVOID, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dTriangleFan(DWORD, LPVOID, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dPoints(DWORD, LPVOID, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dTrianglesIndexed(DWORD, LPVOID, DWORD, LPWORD, DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dGetTextureFormatData(DWORD, M3DTEXTUREFORMAT*)
{
	return MD3D_OK;
}

MD3DERROR d3dCreateTexture(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat,DWORD* lpdwHandle)
{
	return MD3D_OK;
}

MD3DERROR d3dDeleteTexture(DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dLockTexture(DWORD dwHandle, VOID **lplpTexture, DWORD *lplpPitch)
{
	return MD3D_OK;
}

MD3DERROR d3dLockTexture(DWORD dwHandle, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom, VOID **lplpTexture, DWORD *lplpPitch)
{
	return MD3D_OK;
}

MD3DERROR d3dUnlockTexture(DWORD)
{
	return MD3D_OK;
}

MD3DERROR d3dSetTexture(DWORD hTexture,DWORD StageState)
{
	return MD3D_OK;
}

MD3DERROR d3dSetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend)
{
	return MD3D_OK;
}

MD3DERROR d3dCreateSprite(DWORD dwWidth, DWORD dwHeight, DWORD dwFormat, DWORD dwFlags, DWORD* lpdwHandle)
{
	return MD3D_OK;
}

MD3DERROR d3dCreateChildSprite(DWORD dwParentHandle, DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight, DWORD* lpdwHandle)
{
	return MD3D_OK;
}

MD3DERROR d3dDeleteSprite(DWORD dwHandle)
{
	return MD3D_OK;
}

MD3DERROR d3dLockSprite(DWORD dwHandle, VOID **lplpSprite, DWORD *lplpPitch)
{
	return MD3D_OK;
}

MD3DERROR d3dUnlockSprite(DWORD dwHandle)
{
	return MD3D_OK;
}

MD3DERROR d3dSetSpriteMode(DWORD dwHandle, DWORD dwMode, DWORD dwValue)
{
	return MD3D_OK;
}

MD3DERROR d3dSetSpriteRect(DWORD dwHandle, D3DVALUE dvLeft, D3DVALUE dvTop, D3DVALUE dvRight, D3DVALUE dvBottom)
{
	return MD3D_OK;
}

MD3DERROR d3dDrawSprite(DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, DWORD dwOrigin, D3DVALUE dvScaleX, D3DVALUE dvScaleY, D3DVALUE dvRotate)
{
	return MD3D_OK;
}

MD3DERROR d3dDrawSpriteZ(DWORD dwHandle, D3DVALUE dvX, D3DVALUE dvY, D3DVALUE dvZ, DWORD dwOrigin, D3DVALUE dvScaleX, D3DVALUE dvScaleY, D3DVALUE dvRotate)
{
	return MD3D_OK;
}

MD3DERROR d3dSetAdjustedGamma(float fRGamma, float fGGamma, float fBGamma)
{
	return MD3D_OK;
}

MD3DERROR d3dGetAdjustedGamma(float *pfRGamma, float *pfGGamma, float *pfBGamma)
{
	return MD3D_OK;
}

MD3DERROR d3dSetGammaFxHighlight(float fRHilight, float fGHilight, float fBHilight)
{
	return MD3D_OK;
}

MD3DERROR d3dGetGammaFxHighlight(float *pfRHilight, float *pfGHilight, float *pfBHilight)
{
	return MD3D_OK;
}

MD3DERROR d3dSetGammaFxShadow(float fRShadow, float fGShadow, float fBShadow)
{
	return MD3D_OK;
}

MD3DERROR d3dGetGammaFxShadow(float *pfRShadow, float *pfGShadow, float *pfBShadow)
{
	return MD3D_OK;
}

MD3DERROR d3dSetProjectionMatrix(FLOAT fFOV, FLOAT fNearPlane, FLOAT fFarPlane)
{
	return MD3D_OK;
}

MD3DERROR d3dSetProjectionMatrixToIdentity()
{
	return MD3D_OK;
}

MD3DERROR d3dSetFogParameters(DWORD dwMode, DWORD dwColor, FLOAT fStart, FLOAT fEnd, FLOAT fDensity)
{
	return MD3D_OK;
}

MD3DERROR d3dEnableFog(BOOL bEnable)
{
	return MD3D_OK;
}

MD3DERROR d3dSetFocusLossBehavior(BOOL bSleep)
{
	return MD3D_OK;
}

BOOL d3dIsActive()
{
	return FALSE;
}

MD3DERROR d3dSetClipRect(RECT* lprcClipRect)
{
	return MD3D_OK;
}

MD3DERROR d3dTrianglesIndexed2(DWORD dwVertexTypeDesc, 
							  LPVOID lpvVertices, DWORD dwVertexCount, 
							  LPWORD lpwIndices, DWORD dwIndexCount,
							  DWORD dwHandleTex0,DWORD dwHandleTex1)
{
	return MD3D_OK;
}
