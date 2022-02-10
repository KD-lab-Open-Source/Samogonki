#ifndef __IGRAPH3D_H__
#define __IGRAPH3D_H__

#ifdef _WIN32
#include <windows.h>
#include <ddraw.h>
#else
#define NULL 0
#endif
#include <Md3d.h>

#define GET_INT_RGBA(r,g,b,a)		(((a)<<24)|((r)<<16)|((g)<<8)|(b))

enum eInterfaceGraph3d
{
	GRAPH3D_NULL		=	0,
	GRAPH3D_SOFTWARE	=	1,
	GRAPH3D_DIRECT3D	=	2
};

enum eModeGraph3d
{
	GRAPH3D_MODE_NULL	=	0<<0,	
	GRAPH3D_MODE_DEBUG	=	1<<1,	// отладочный запуск
	GRAPH3D_MODE_WINDOW	=	1<<2,	// вывод в окно
	
	GRAPH3D_MODE_RGB16	=	1<<10,	// 16 битный цвет
	GRAPH3D_MODE_RGB32	=	1<<11,	// 32 битный цвет
	GRAPH3D_MODE_Z16	=	1<<12,	// 16 битный z-буффер
	GRAPH3D_MODE_Z32	=	1<<13,	// 32 битный z-буффер
	GRAPH3D_MODE_T16	=	1<<14,	// 16 битные текстуры
	GRAPH3D_MODE_T32	=	1<<15	// 32 битные текстуры
};

enum eTextureFormat
{
	TEXFMT_NULL			=	0,
	TEXFMT_RGB565		=	1,
	TEXFMT_RGB555		=	2,
	TEXFMT_ARGB4444		=	3,
	TEXFMT_ARGB1555		=	4,
	TEXFMT_ARGB8888		=	5,
	TEXFMT_PAL8			=	6,
};

struct sTextureFormatData
{
	eTextureFormat TextureFormat;
	int rBitCount,gBitCount,bBitCount,aBitCount;
	int rBitShift,gBitShift,bBitShift,aBitShift;
	
	void Set(int texfmt,int rcount,int gcount,int bcount,int acount,int rshift,int gshift,int bshift,int ashift)
	{
		TextureFormat=(eTextureFormat)texfmt;
		rBitCount=rcount; gBitCount=gcount; bBitCount=bcount; aBitCount=acount;
		rBitShift=rshift; gBitShift=gshift;	bBitShift=bshift; aBitShift=ashift;
	}
	void Get(int &texfmt,int &rcount,int &gcount,int &bcount,int &acount,int &rshift,int &gshift,int &bshift,int &ashift)
	{
		texfmt=(int)TextureFormat;
		rcount=rBitCount; gcount=gBitCount; bcount=bBitCount; acount=aBitCount;
		rshift=rBitShift; gshift=gBitShift;	bshift=bBitShift; ashift=aBitShift;
	}
};

enum eMaterialMode
{
	MAT_NULL									=	0,
	MAT_COLOR_MOD_DIFFUSE						=	1<<1,						// color*=Diffuse
	MAT_COLOR_MOD_TEXTURE1						=	1<<2,						// color*=Texture1
	MAT_COLOR_ADD_TEXTURE1						=	1<<3,						// color+=Texture1
	MAT_COLOR_ADD_SPECULAR						=	1<<4,						// color+=Specular
	MAT_ALPHA_MOD_DIFFUSE						=	1<<5,						// alpha*=Diffuse
	MAT_ALPHA_MOD_TEXTURE1						=	1<<6,						// alpha*=Texture1
	MAT_ALPHA_MASK_TEXTURE1						=	1<<7,						// alpha*=Texture1
	MAT_COLOR_ADD_DIFFUSE						=	1<<8,
	// смешанные материалы Diffuse & Texture1 & Specular
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1,	// color=Texture1*Diffuse
	MAT_COLOR_MOD_DIFFUSE_ADD_SPECULAR			=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_ADD_SPECULAR,	// color=Diffuse+Specular
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR	=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_COLOR_ADD_SPECULAR,	// color=Texture1*Diffuse+Specular
	// смешанные материалы Diffuse & Texture1 & Alpha
	MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE							=	MAT_COLOR_MOD_DIFFUSE	|	MAT_ALPHA_MOD_DIFFUSE,		// color=Diffuse, a=aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MOD_DIFFUSE,	// color=Texture1*Diffuse, a=aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_TEXTURE1				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MOD_TEXTURE1,	// color=Texture1*Diffuse, a=aTexture1,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_TEXTURE1		=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MOD_DIFFUSE	|	MAT_ALPHA_MOD_TEXTURE1,	// color=Texture*Diffuse, a=aTexture*aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MASK_TEXTURE1				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MASK_TEXTURE1,	// color=Texture1*Diffuse, a=aTexture1,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_MASK_TEXTURE1	=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MOD_DIFFUSE	|	MAT_ALPHA_MASK_TEXTURE1,	// color=Texture*Diffuse, a=aTexture*aDiffuse,
	// смешанные материалы Diffuse & Texture1 & Specular & Alpha
	MAT_COLOR_MOD_DIFFUSE_ADD_SPECULAR_ALPHA_MOD_DIFFUSE						=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_ADD_SPECULAR	|	MAT_COLOR_ADD_SPECULAR	|	MAT_ALPHA_MOD_DIFFUSE,	// color=Diffuse+Specular, a=aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR_ALPHA_MOD_DIFFUSE				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_COLOR_ADD_SPECULAR	|	MAT_ALPHA_MOD_DIFFUSE,	// color=Texture1*Diffuse+Specular, a=aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR_ALPHA_MOD_TEXTURE1				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_COLOR_ADD_SPECULAR	|	MAT_ALPHA_MOD_TEXTURE1,	// color=Texture1*Diffuse+Specular, a=aTexture1,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR_ALPHA_MOD_DIFFUSE_TEXTURE1		=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_COLOR_ADD_SPECULAR	|	MAT_ALPHA_MOD_DIFFUSE	|	MAT_ALPHA_MOD_TEXTURE1,	// color=Texture*Diffuse+Specular, a=aTexture*aDiffuse,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR_ALPHA_MASK_TEXTURE1				=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MASK_TEXTURE1,	// color=Texture1*Diffuse+Specular, a=aTexture1,
	MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR_ALPHA_MOD_DIFFUSE_MASK_TEXTURE1	=	MAT_COLOR_MOD_DIFFUSE	|	MAT_COLOR_MOD_TEXTURE1	|	MAT_ALPHA_MOD_DIFFUSE	|	MAT_ALPHA_MASK_TEXTURE1,	// color=Texture*Diffuse+Specular, a=aTexture*aDiffuse,
};

enum eRenderStateOption
{
	RENDERSTATE_NULL					=	0,
	RENDERSTATE_ZTEST					=	1,
	RENDERSTATE_ZWRITE					=	2,
	RENDERSTATE_DITHER					=	3,
	RENDERSTATE_SPECULAR				=	4,
	RENDERSTATE_TEXTUREPERSPECTIVE		=	5,
	RENDERSTATE_ZFUNC					=	6,
	RENDERSTATE_ZBIAS					=	7,
	RENDERSTATE_FILLMODE				=	8,
	RENDERSTATE_CULLMODE				=	9,
	RENDERSTATE_SHADEMODE				=	10,
	RENDERSTATE_SUBPIXEL				=	11,
	RENDERSTATE_ALPHATEST				=	12,
	RENDERSTATE_ALPHAFUNC				=	13,
	RENDERSTATE_ALPHAREF				=	14,
	RENDERSTATE_ALPHABLEND				=	15,
	RENDERSTATE_SRCBLEND				=	16,
	RENDERSTATE_DESTBLEND				=	17,
	RENDERSTATE_TEXTUREADDRESS			=	18,
	RENDERSTATE_TEXTUREPOINT			=	19,
	RENDERSTATE_TEXTURELINEAR			=	20,
// временно не используются
	RENDERSTATE_MIPMAP,
};

enum eRenderStateCullMode
{
    RENDERSTATE_CULL_NONE				=	1,
    RENDERSTATE_CULL_CW					=	2,
    RENDERSTATE_CULL_CCW				=	3,
    RENDERSTATE_CULL_FORCE				=	0x7fffffff,
};
enum eRenderStateTextureAddress 
{
    TADDRESS_WRAP						= 1,
    TADDRESS_MIRROR						= 2,
    TADDRESS_CLAMP						= 3,
    TADDRESS_BORDER						= 4,
    TADDRESS_FORCE_DWORD				= 0x7fffffff, 
};

enum eBlendMode
{
    BLEND_ZERO              = 1,
    BLEND_ONE               = 2,
    BLEND_SRCCOLOR          = 3,
    BLEND_INVSRCCOLOR       = 4,
    BLEND_SRCALPHA          = 5,
    BLEND_INVSRCALPHA       = 6,
    BLEND_DESTALPHA         = 7,
    BLEND_INVDESTALPHA      = 8,
    BLEND_DESTCOLOR         = 9,
    BLEND_INVDESTCOLOR      = 10,
    BLEND_SRCALPHASAT       = 11,
    BLEND_BOTHSRCALPHA      = 12,
    BLEND_BOTHINVSRCALPHA   = 13,
    BLEND_FORCE_DWORD       = 0x7fffffff, /* force 32-bit size enum */
};

struct sVertexFix
{ // диффузный цвет - drgba, спекулярный цвет - srgba, 3 пары текстурных координат - uv[3][2]
	float			xe,ye,z,w;
	unsigned char	drgba[4];
	unsigned char	srgba[4];
	float			uv[1][2];
	
	inline unsigned char& dr()			{ return drgba[2]; }
	inline unsigned char& dg()			{ return drgba[1]; }
	inline unsigned char& db()			{ return drgba[0]; }
	inline unsigned char& da()			{ return drgba[3]; }
	inline unsigned char& sr()			{ return srgba[2]; }
	inline unsigned char& sg()			{ return srgba[1]; }
	inline unsigned char& sb()			{ return srgba[0]; }
	inline unsigned char& sa()			{ return srgba[3]; }
	inline int& diffuse()				{ return *(int*)&drgba[0]; }
	inline int& specular()				{ return *(int*)&srgba[0]; }
	inline float& u1()					{ return uv[0][0]; }
	inline float& v1()					{ return uv[0][1]; }
	inline float& u2()					{ return uv[0][0]; }
	inline float& v2()					{ return uv[0][1]; }
	inline float& u3()					{ return uv[0][0]; }
	inline float& v3()					{ return uv[0][1]; }
};
class Vect2f;
class Vect3f;
struct sVertexD3D
{ // диффузный цвет - drgba, спекулярный цвет - srgba, 3 пары текстурных координат - uv[3][2]
	float			x,y,z;
	unsigned char	drgba[4];
	unsigned char	srgba[4];
	float			uv[1][2];
	static int		fmt;
	
	inline unsigned char& dr()			{ return drgba[2]; }
	inline unsigned char& dg()			{ return drgba[1]; }
	inline unsigned char& db()			{ return drgba[0]; }
	inline unsigned char& da()			{ return drgba[3]; }
	inline unsigned char& sr()			{ return srgba[2]; }
	inline unsigned char& sg()			{ return srgba[1]; }
	inline unsigned char& sb()			{ return srgba[0]; }
	inline unsigned char& sa()			{ return srgba[3]; }
	inline float& u1()					{ return uv[0][0]; }
	inline float& v1()					{ return uv[0][1]; }
	inline float& u2()					{ return uv[0][0]; }
	inline float& v2()					{ return uv[0][1]; }
	inline float& u3()					{ return uv[0][0]; }
	inline float& v3()					{ return uv[0][1]; }
	inline int& diffuse()				{ return *(int*)&drgba[0]; }
	inline int& specular()				{ return *(int*)&srgba[0]; }
	inline Vect3f& GetVect3f()			{ return *(Vect3f*)&x; }
	inline Vect2f& GetTexel()			{ return *(Vect2f*)&uv[0][0]; }
};

struct sPolygonFix
{
	unsigned short p1,p2,p3;
};

enum eVertexFormat
{
	VERTEXFMT_FIX		=	0,
};

class cInterfaceGraph3d
{
public:
	cInterfaceGraph3d();
	virtual ~cInterfaceGraph3d();

	virtual int Init(int xScr,int yScr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon=NULL)=0;
	virtual int ReInit(int xScr,int yScr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon=NULL)=0;
	virtual int GetClipRect(int *xmin,int *ymin,int *xmax,int *ymax)=0;
	virtual int SetClipRect(int xmin,int ymin,int xmax,int ymax)=0;
	virtual int NullClipRect()=0;
	virtual int Release()=0;
	virtual int IsActive()=0;

	virtual int BeginScene()=0;
	virtual int EndScene()=0;
	virtual int Fill(int r,int g,int b)=0;
	virtual int Flush()=0;

	virtual int SetMaterial(eMaterialMode material)=0;
	virtual int SetRenderState(eRenderStateOption option,int value)=0;
	virtual int GetTextureFormatData(sTextureFormatData &TexFmtData)=0;

	virtual int PolygonFan(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX)=0;
	virtual int PolygonStrip(void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX)=0;
	virtual int PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat=VERTEXFMT_FIX)=0;
	virtual int PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat=VERTEXFMT_FIX)=0;

	virtual int SetTexture(int hTexture)=0;
	virtual int LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine)=0;
	virtual int UnlockTexture(int hTexture)=0;
	virtual int CreateTexture(int x,int y,eTextureFormat TextureFormat)=0;
	virtual int DeleteTexture(int hTexture)=0;

	// начало прочие функции
	// спрайты
	virtual int CreateSprite(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat, 
							  DWORD dwFlags,DWORD* lpdwHandle )=0;
	virtual int CreateChildSprite(DWORD dwParentHandle,DWORD dwLeft,DWORD dwTop, 
							   DWORD dwWidth,DWORD dwHeight,DWORD* lpdwHandle )=0;
	virtual int DeleteSprite(DWORD dwHandle )=0;
	virtual int LockSprite(DWORD dwHandle,void **lplpSprite,DWORD *lplpPitch )=0;
	virtual int UnlockSprite(DWORD dwHandle )=0;
	virtual int SetSpriteMode(DWORD dwHandle,DWORD dwMode,DWORD dwValue )=0;
	virtual int DrawSprite(DWORD dwHandle,float dvX,float dvY,DWORD dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate )=0;
	virtual int DrawSpriteZ(DWORD dwHandle,float dvX,float dvY,float dvZ, 
						 DWORD dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate )=0;
	// BackBuffer
	virtual int CreateBackBuffer()=0;
	virtual int ReleaseBackBuffer()=0;
	virtual int GetBackBufferFormat(DWORD *dwFormat)=0;
	virtual int LockBackBuffer(void **lplpSurface,DWORD *lpdwPitch)=0;
	virtual int UnlockBackBuffer()=0;
	virtual int FlushBackBuffer(RECT *lprcRect)=0;
	virtual int SetBackBufferColorKey(DWORD dwColor)=0;
	virtual int EnableBackBufferColorKey(BOOL bEnable)=0;
	// Gamma
	virtual int QueryGammaSupport( MD3DGAMMASUPPORT *gmGammaSupport )=0;
	virtual int CalibrateGamma( DDGAMMARAMP *lpRampData )=0;
	virtual int SetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma )=0;
	virtual int GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma )=0;
	virtual int SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight )=0;
	virtual int GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight )=0;
	virtual int SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow )=0;
	virtual int GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow )=0;
	// other
	virtual int SetViewColor(int r,int g,int b,int alfa)=0;
	virtual int GetWindowHandle( HWND *hWnd )=0;
	virtual int ScreenShot(VOID *lpBuffer,DWORD dwSize)=0;
	// конец прочие функции

	// вспомогательные функции, могут быть не реализоаваны
	virtual int	GetSizeX()														{ return 0; }
	virtual int	GetSizeY()														{ return 0; }
	virtual void* GetZBuffer()													{ return 0; }
	virtual void* GetScrBuffer()												{ return 0; }
	virtual void* GetInfo(int *graph3d)											{ return 0; }
	virtual int GetInfo(int *xScr,int *yScr,int *mode)							{ return 0; }
	virtual int DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a)	{ return 0; }
	virtual int DrawLine(int x1,int y1,int x2,int y2,int c1,int c2)				{ return 0; }
	virtual int DrawPixel(int x1,int y1,int r,int g,int b,int a)				{ return 0; }
	virtual int DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag=0) { return 0; }
	virtual int OutText(int x,int y,char *string,int r=255,int g=255,int b=255,int a=255)	{ return 0; }
	virtual int DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r=255,int g=255,int b=255,int a=255)	{ return 0; }
	virtual void InitRenderState()												{}
};

cInterfaceGraph3d* CreateIGraph3D(int InterfaceGraph3d);

/*
#pragma comment (lib,"xtool")
#pragma comment (lib,"dxguid")
#pragma comment (lib,"ddraw")
#ifdef _DEBUG
#pragma comment (lib,"md3dDBG")
#else
#pragma comment (lib,"md3d")
#endif
*/

#endif //__IGRAPH3D_H__
