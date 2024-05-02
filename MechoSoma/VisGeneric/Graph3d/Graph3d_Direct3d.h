#ifndef __GRAPH3D_DIRECT3D_H__
#define __GRAPH3D_DIRECT3D_H__

#include "IGraph3d.h"

#include "renderer.h"

class cGraph3dDirect3D : public cInterfaceGraph3d
{
public:
	cGraph3dDirect3D();
	~cGraph3dDirect3D();
	
	virtual int Init(int xScr,int yScr,int mode,void *hInst,char *szTitle,void *hIcon=NULL);
	virtual int ReInit(int xScr,int yScr,int mode,void *hInst,char *szTitle,void *hIcon=NULL);
	virtual int GetClipRect(int *xmin,int *ymin,int *xmax,int *ymax);
	virtual int SetClipRect(int xmin,int ymin,int xmax,int ymax);
	virtual int NullClipRect();
	virtual int Release();
	virtual int IsActive();

	virtual int BeginScene();
	virtual int EndScene();
	virtual int Fill(int r,int g,int b);
	virtual int Flush();

	virtual void SetProjectionMatrix(const MD3DRECT &Viewport, const D3DMATRIX &ProjectionMatrix);
	virtual void ResetProjectionMatrix();

	virtual int SetMaterial(eMaterialMode material);
	virtual int SetRenderState(eRenderStateOption option,int value);
	virtual int GetTextureFormatData(sTextureFormatData &TexFmtData);

	virtual int PolygonFan(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonStrip(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat=VERTEXFMT_FIX);

	virtual int BeginDrawCommand(M3D_DRAW_COMMAND &command);
	virtual int EndDrawCommand(const M3D_DRAW_COMMAND &command);

	virtual int SetTexture(int hTexture, uint32_t dwStage);
	virtual int LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine);
	virtual int UnlockTexture(int hTexture);
	virtual int CreateTexture(int x,int y,eTextureFormat TextureFormat);
	virtual int DeleteTexture(int hTexture);

	// начало прочие функции
	virtual int CreateSprite(uint32_t dwWidth,uint32_t dwHeight,uint32_t dwFormat, 
							  uint32_t dwFlags,uint32_t* lpdwHandle );
	virtual int CreateChildSprite(uint32_t dwParentHandle,uint32_t dwLeft,uint32_t dwTop, 
							   uint32_t dwWidth,uint32_t dwHeight,uint32_t* lpdwHandle );
	virtual int DeleteSprite(uint32_t dwHandle );
	virtual int LockSprite(uint32_t dwHandle,void **lplpSprite,uint32_t *lplpPitch );
	virtual int UnlockSprite(uint32_t dwHandle );
	virtual int SetSpriteMode(uint32_t dwHandle,uint32_t dwMode,uint32_t dwValue );
	virtual int DrawSprite(uint32_t dwHandle,float dvX,float dvY,uint32_t dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate );
	virtual int DrawSpriteZ(uint32_t dwHandle,float dvX,float dvY,float dvZ, 
						 uint32_t dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate );
	virtual int CreateBackBuffer();
	virtual int ReleaseBackBuffer();
	virtual int GetBackBufferFormat(uint32_t *dwFormat);
	virtual int LockBackBuffer(void **lplpSurface,uint32_t *lpdwPitch);
	virtual int UnlockBackBuffer();
	virtual int FlushBackBuffer(MD3DRECT *lprcRect);
	virtual int SetBackBufferColorKey(uint32_t dwColor);
	virtual int EnableBackBufferColorKey(bool bEnable);

	virtual int QueryGammaSupport( MD3DGAMMASUPPORT *gmGammaSupport);
	virtual int CalibrateGamma( DDGAMMARAMP *lpRampData);
	virtual int SetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma);
	virtual int GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma);
	virtual int SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight);
	virtual int GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight);
	virtual int SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow);
	virtual int GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow);

	virtual int ScreenShot(void *lpBuffer,uint32_t dwSize);
	virtual int GetWindowHandle( void **hWnd );
	// конец прочие функции
	virtual int SetViewColor(int r,int g,int b,int alfa);

	virtual int	GetSizeX()											{ return xScr; }
	virtual int	GetSizeY()											{ return yScr; }
	virtual void* GetZBuffer();
	virtual void* GetScrBuffer();
	virtual void* GetInfo(int *graph3d);
	virtual int GetInfo(int *xScr,int *yScr,int *mode)				{ *xScr=this->xScr, *yScr=this->yScr; *mode=GraphMode; return 0; }
	virtual int DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a);
	virtual int DrawLine(int x1,int y1,int x2,int y2,int c1,int c2);
	virtual int DrawPixel(int x1,int y1,int r,int g,int b,int a);
	virtual int DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r,int g,int b,int a);
	virtual int OutText(int x,int y,char *string,int r,int g,int b,int a);
	virtual int DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag);
	virtual void InitRenderState();

	// legacy
	virtual int EnumVideoMode(int* pNumVideoMode, MD3DMODE** ppArray);
	virtual int GetTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData);
	virtual int SetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t dwRenderState);
	virtual int GetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t *lpdwRenderState);
	virtual int SetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue);
	virtual int SetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend);
	virtual int SetSpriteRect(uint32_t dwHandle, float dvLeft, float dvTop, float dvRight, float dvBottom);
	virtual int Clear(uint32_t dwColor);
	virtual int Flip(bool bWaitVerticalBlank);
	virtual int SetClipRect(const MD3DRECT &lprcClipRect);

private:
	eModeGraph3d			GraphMode;
	eMaterialMode			MaterialMode;
	int						SwitchRenderScene;
	int						xScr,yScr,xScrMin,yScrMin,xScrMax,yScrMax;
	int						rBitShift,gBitShift,bBitShift;
	int						rBitCount,gBitCount,bBitCount;
	int						NumberPolygon,MinNumberPolygon,MaxNumberPolygon;
	int						WaitVerticalBlank;

	inline int GetColor(int r,int g,int b)										{ if(r>255) r=255; if(g>255) g=255; if(b>255) b=255; return ((r>>(8-rBitCount))<<rBitShift)+((g>>(8-gBitCount))<<gBitShift)+((b>>(8-bBitCount))<<bBitShift); }
	std::unique_ptr<graphics::Renderer> _renderer;
	bool _isActive = false;
};

#endif //__GRAPH3D_DIRECT3D_H__