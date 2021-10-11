#ifndef __SOFTWARE_H__
#define __SOFTWARE_H__

#include "IGraph3d.h"
#include "BaseClass.h"

#define GRAPH3D_SOFTWARE_TONE_MIN			-32
#define GRAPH3D_SOFTWARE_TONE_MAX			512

struct sTextureSoftware
{
	unsigned int	ID;
	eTextureFormat	format;
	int				_x,_y;	// битовый размер текстуры
	int				bpl;	// байт на строку текстуры
	void			*buf;

	sTextureSoftware(int id,eTextureFormat fmtTex,int xTex,int yTex);
	sTextureSoftware()								{ buf=0; format=TEXFMT_NULL; }
	~sTextureSoftware()								{ if(buf) delete buf; buf=0; }
};

typedef cBaseList <sTextureSoftware> sTextureSoftwareList;
typedef cBaseDispatcher <sTextureSoftware,sTextureSoftwareList> sTextureSoftwareLibrary;

class cGraph3dSoftware : public cInterfaceGraph3d
{
public:
	cGraph3dSoftware();
	~cGraph3dSoftware();
	
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
	virtual int SetViewColor(int r,int g,int b,int a);

	virtual int	GetSizeX()											{ return xScr; }
	virtual int	GetSizeY()											{ return yScr; }
	virtual void* GetZBuffer();
	virtual void* GetScrBuffer();
	virtual void* GetInfo(int *graph3d);
	virtual int GetInfo(int *xScr,int *yScr,int *mode)				{ *xScr=this->xScr, *yScr=this->yScr; *mode=GraphMode; return 0; }
	virtual int DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a);
	virtual int DrawPixel(int x1,int y1,int r,int g,int b,int a);
	virtual int DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag);
	virtual int OutText(int x,int y,char *string,int r,int g,int b,int a);
	virtual int DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r,int g,int b,int a);
private:
	eModeGraph3d			GraphMode;
	void					*scr,*zBuffer;
	int						xScr,yScr;
	int						xScrMin,yScrMin,xScrMax,yScrMax;
	int						rBitShift,gBitShift,bBitShift;
	int						rBitCount,gBitCount,bBitCount;
	int						SwitchRenderScene;
	eMaterialMode			MaterialMode;
	sTextureSoftwareLibrary	TextureLibrary;
	int						RenderState_zwrite;

	inline sTextureSoftwareList* GetTexture(unsigned int hTexture)				{ for(sTextureSoftwareList *start=TextureLibrary.BaseList;start;start=start->next) if(start->Base->ID==hTexture) return start; assert(0); return 0; }
	inline int GetColor(int r,int g,int b)										{ if(r>255) r=255; if(g>255) g=255; if(b>255) b=255; return ((r>>(8-rBitCount))<<rBitShift)+((g>>(8-gBitCount))<<gBitShift)+((b>>(8-bBitCount))<<bBitShift); }
// данные для софтварного рендера
	void			*texture;							// указатель на текущую текстуру
	int				_xt1,_yt1;							// битовый размер текстуры
	int				xmt1,ymt1;							// маска текстуры сдвинутая
	int				xst1_,yst1_;						// размер текстуры
	int				xe1,ye1,z1, mr1,mg1,mb1,ma1, ar1,ag1,ab1,aa1, u11,v11, u21,v21, u31,v31;
	int				xe2,ye2,z2, mr2,mg2,mb2,ma2, ar2,ag2,ab2,aa2, u12,v12, u22,v22, u32,v32;
	int				xe3,ye3,z3, mr3,mg3,mb3,ma3, ar3,ag3,ab3,aa3, u13,v13, u23,v23, u33,v33;
	int				AlfaRef;
	sVertexFix		*v1,*v2,*v3;
////////// HiColor //////////
	eTextureFormat	ScrTextureFormat;
	unsigned short	rPal16Mem[GRAPH3D_SOFTWARE_TONE_MAX-GRAPH3D_SOFTWARE_TONE_MIN];	// перевод TrueColor режима в HiColor, с запасом для аддитивных операций
	unsigned short	gPal16Mem[GRAPH3D_SOFTWARE_TONE_MAX-GRAPH3D_SOFTWARE_TONE_MIN];
	unsigned short	bPal16Mem[GRAPH3D_SOFTWARE_TONE_MAX-GRAPH3D_SOFTWARE_TONE_MIN];	
	unsigned short	*rPal16,*gPal16,*bPal16;	// перевод TrueColor режима в HiColor, с запасом для аддитивных операций

	void InitColor16();
	// функции вывода самого низкого уровня
	// Right, 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse
	void R16_ZTW_CMD(void *pscr,void *pzBuffer);		
	void L16_ZTW_CMD(void *pscr,void *pzBuffer);		
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Texture
	void R16_ZTW_CMT1(void *pscr,void *pzBuffer);	
	void L16_ZTW_CMT1(void *pscr,void *pzBuffer);	
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse*Texture
	void R16_ZTW_CMDT1(void *pscr,void *pzBuffer);	
	void L16_ZTW_CMDT1(void *pscr,void *pzBuffer);	
	// 16 bit color (hi color), Zbuffer - Test, Write (if RenderState_zwrite), Color = Diffuse, Alpha - Diffuse
	void R16_ZT_CMD_AMD(void *pscr,void *pzBuffer);
	void L16_ZT_CMD_AMD(void *pscr,void *pzBuffer);
	// 16 bit color (hi color), Zbuffer - Test, Write (if RenderState_zwrite), Color = Texture, Alpha - Texture
	void R16_ZT_CMT1_AMT1(void *pscr,void *pzBuffer);	
	void L16_ZT_CMT1_AMT1(void *pscr,void *pzBuffer);	
	// 16 bit color (hi color), Zbuffer - Test, Write (if RenderState_zwrite), Color = Diffuse*Texture, Alpha - Diffuse*Texture
	void R16_ZT_CMDT1_AMDT1(void *pscr,void *pzBuffer);
	void L16_ZT_CMDT1_AMDT1(void *pscr,void *pzBuffer);
	// 16 bit color (hi color), Zbuffer - Test, Write (if RenderState_zwrite), Color = Color+Diffuse
	void R16_ZT_CAD(void *pscr,void *pzBuffer);	
	void L16_ZT_CAD(void *pscr,void *pzBuffer);	
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse+Specular
	void R16_ZTW_CMDAS(void *pscr,void *pzBuffer);
	void L16_ZTW_CMDAS(void *pscr,void *pzBuffer);
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse+Specular, Alpha - Diffuse
	void R16_ZT_CMDAS_AMD(void *pscr,void *pzBuffer);
	void L16_ZT_CMDAS_AMD(void *pscr,void *pzBuffer);
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse*Texture+Specular
	void R16_ZTW_CMDT1AS(void *pscr,void *pzBuffer);
	void L16_ZTW_CMDT1AS(void *pscr,void *pzBuffer);
	// 16 bit color (hi color), Zbuffer - Test, Write, Color = Diffuse*Texture+Specular, Alpha - Diffuse*Texture
	void R16_ZT_CMDT1AS_AMDT1(void *pscr,void *pzBuffer);
	void L16_ZT_CMDT1AS_AMDT1(void *pscr,void *pzBuffer);
};

#endif //__SOFTWARE_H__