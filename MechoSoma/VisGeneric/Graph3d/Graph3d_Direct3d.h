#ifndef __GRAPH3D_DIRECT3D_H__
#define __GRAPH3D_DIRECT3D_H__

#include "IGraph3d.h"

class cGraph3dDirect3D : public cInterfaceGraph3d
{
public:
	cGraph3dDirect3D();
	~cGraph3dDirect3D();
	
	virtual int Init(int xScr,int yScr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon=NULL);
	virtual int ReInit(int xScr,int yScr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon=NULL);
	virtual int GetClipRect(int *xmin,int *ymin,int *xmax,int *ymax);
	virtual int SetClipRect(int xmin,int ymin,int xmax,int ymax);
	virtual int NullClipRect();
	virtual int Release();
	virtual int IsActive();

	virtual int BeginScene();
	virtual int EndScene();
	virtual int Fill(int r,int g,int b);
	virtual int Flush();

	virtual int SetMaterial(eMaterialMode material);
	virtual int SetRenderState(eRenderStateOption option,int value);
	virtual int GetTextureFormatData(sTextureFormatData &TexFmtData);

	virtual int PolygonFan(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonStrip(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX);
	virtual int PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat=VERTEXFMT_FIX);

	virtual int SetTexture(int hTexture);
	virtual int LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine);
	virtual int UnlockTexture(int hTexture);
	virtual int CreateTexture(int x,int y,eTextureFormat TextureFormat);
	virtual int DeleteTexture(int hTexture);

	// начало прочие функции
	virtual int CreateSprite(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat, 
							  DWORD dwFlags,DWORD* lpdwHandle );
	virtual int CreateChildSprite(DWORD dwParentHandle,DWORD dwLeft,DWORD dwTop, 
							   DWORD dwWidth,DWORD dwHeight,DWORD* lpdwHandle );
	virtual int DeleteSprite(DWORD dwHandle );
	virtual int LockSprite(DWORD dwHandle,void **lplpSprite,DWORD *lplpPitch );
	virtual int UnlockSprite(DWORD dwHandle );
	virtual int SetSpriteMode(DWORD dwHandle,DWORD dwMode,DWORD dwValue );
	virtual int DrawSprite(DWORD dwHandle,float dvX,float dvY,DWORD dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate );
	virtual int DrawSpriteZ(DWORD dwHandle,float dvX,float dvY,float dvZ, 
						 DWORD dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate );
	virtual int CreateBackBuffer();
	virtual int ReleaseBackBuffer();
	virtual int GetBackBufferFormat(DWORD *dwFormat);
	virtual int LockBackBuffer(void **lplpSurface,DWORD *lpdwPitch);
	virtual int UnlockBackBuffer();
	virtual int FlushBackBuffer(RECT *lprcRect);
	virtual int SetBackBufferColorKey(DWORD dwColor);
	virtual int EnableBackBufferColorKey(BOOL bEnable);

	virtual int QueryGammaSupport( MD3DGAMMASUPPORT *gmGammaSupport);
	virtual int CalibrateGamma( DDGAMMARAMP *lpRampData);
	virtual int SetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma);
	virtual int GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma);
	virtual int SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight);
	virtual int GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight);
	virtual int SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow);
	virtual int GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow);

	virtual int ScreenShot(VOID *lpBuffer,DWORD dwSize);
	virtual int GetWindowHandle( HWND *hWnd );
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
//private:
	eModeGraph3d			GraphMode;
	eMaterialMode			MaterialMode;
	int						SwitchRenderScene;
	int						xScr,yScr,xScrMin,yScrMin,xScrMax,yScrMax;
	int						rBitShift,gBitShift,bBitShift;
	int						rBitCount,gBitCount,bBitCount;
	int						NumberPolygon,MinNumberPolygon,MaxNumberPolygon;
	int						WaitVerticalBlank;

	inline int GetColor(int r,int g,int b)										{ if(r>255) r=255; if(g>255) g=255; if(b>255) b=255; return ((r>>(8-rBitCount))<<rBitShift)+((g>>(8-gBitCount))<<gBitShift)+((b>>(8-bBitCount))<<bBitShift); }
};

#endif //__GRAPH3D_DIRECT3D_H__