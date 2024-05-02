#include "Md3d.h"
#include "math.h"
#include "BaseDefine.h"
#include "assert.h"
#include "Graph3d_Direct3d.h"
//#define _TEST_DIRECT3D_
#ifdef _TEST_DIRECT3D_
#include "fstream.h"
fstream fxx("graph.txt",ios::out);
#endif //_TEST_DIRECT3D_

#include "port.h"
#include "texture_manager.h"

extern void xtRegisterSysFinitFnc(void (*fPtr)(void),int id);
extern void xtDeactivateSysFinitFnc(int id);

extern MD3DERROR d3dCreateSprite(uint32_t dwWidth, uint32_t dwHeight, uint32_t dwFormat, uint32_t dwFlags, uint32_t* lpdwHandle);
extern MD3DERROR d3dCreateChildSprite(uint32_t dwParentHandle, uint32_t dwLeft, uint32_t dwTop, uint32_t dwWidth, uint32_t dwHeight, uint32_t* lpdwHandle);
extern MD3DERROR d3dDeleteSprite(uint32_t dwHandle);
extern MD3DERROR d3dLockSprite(uint32_t dwHandle, void **lplpSprite, uint32_t *lplpPitch);
extern MD3DERROR d3dUnlockSprite(uint32_t dwHandle);
extern MD3DERROR d3dSetSpriteMode(uint32_t dwHandle, uint32_t dwMode, uint32_t dwValue);
extern MD3DERROR d3dSetSpriteRect(uint32_t dwHandle, float dvLeft, float dvTop, float dvRight, float dvBottom);
extern MD3DERROR d3dDrawSprite(uint32_t dwHandle, float dvX, float dvY, uint32_t dwOrigin, float dvScaleX, float dvScaleY, float dvRotate);
extern MD3DERROR d3dDrawSpriteZ(uint32_t dwHandle, float dvX, float dvY, float dvZ, uint32_t dwOrigin, float dvScaleX, float dvScaleY, float dvRotate);


void D3D_FinitFnc(void)
{
	xtDeactivateSysFinitFnc(XD3D_SYSOBJ_ID);            
//	d3dClose();
}

cGraph3dDirect3D::cGraph3dDirect3D()
{
	GraphMode=GRAPH3D_MODE_NULL;
	MaterialMode=MAT_NULL;
	SwitchRenderScene=-1;
	xScr=yScr=xScrMin=yScrMin=xScrMax=yScrMax=0;
	rBitShift=gBitShift=bBitShift=0;
	rBitCount=gBitCount=bBitCount=0;
	NumberPolygon=MaxNumberPolygon=0; MinNumberPolygon=0xFFFF;
	WaitVerticalBlank=1;
}
cGraph3dDirect3D::~cGraph3dDirect3D()
{
}
	
int cGraph3dDirect3D::Init(int xscr,int yscr,int mode,void *hInst,char *szTitle,void *hIcon)
{
	int XGR_flag=0;
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	assert(GraphMode&GRAPH3D_MODE_Z16);

	extern const char* mch_mainINI;

	SwitchRenderScene=-1; 

#ifdef _DEBUG
//	d3dSetDebugLevel(5);
//	d3dSetLogFile("a.txt");
#endif

	char* wnd_title = nullptr;
	int error, DriverMode=0,ColorBit;
	if(GraphMode&GRAPH3D_MODE_RGB16) ColorBit=16;
	else if(GraphMode&GRAPH3D_MODE_RGB32) ColorBit=32;
	if((mode&GRAPH3D_MODE_WINDOW)==0) DriverMode|=MD3D_FULLSCREEN;
#ifdef _MECHOSOMA_
	extern int mch_d3dAltDevice;
	if( mch_d3dAltDevice )
		DriverMode|=MD3D_ALTDEVICE;
#endif
	extern char* mchWndTitle;
	wnd_title = mchWndTitle;

	_renderer = std::make_unique<graphics::Renderer>(xscr, yscr, DriverMode & MD3D_FULLSCREEN);
	_isActive = true;
	xtRegisterSysFinitFnc(D3D_FinitFnc,XD3D_SYSOBJ_ID); 

	SetClipRect(0,0,xscr-1,yscr-1);
	Fill(0,0,0);
	Flush();
	Fill(0,0,0);
	Flush();

	BeginScene();
	EndScene();

	return 0;
}
int cGraph3dDirect3D::ReInit(int xscr,int yscr,int mode,void *hInst,char *szTitle,void *hIcon)
{
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	assert(GraphMode&GRAPH3D_MODE_Z16);

	extern const char* mch_mainINI;

	SwitchRenderScene=-1; 

	char* wnd_title = nullptr;
	int error, DriverMode=0,ColorBit;
	if(GraphMode&GRAPH3D_MODE_RGB16) ColorBit=16;
	else if(GraphMode&GRAPH3D_MODE_RGB32) ColorBit=32;
	if((mode&GRAPH3D_MODE_WINDOW)==0) DriverMode|=MD3D_FULLSCREEN;
#ifdef _MECHOSOMA_
	extern int mch_d3dAltDevice;
	if( mch_d3dAltDevice )
		DriverMode|=MD3D_ALTDEVICE;
#endif
	extern char* mchWndTitle;
	wnd_title = mchWndTitle;

	_renderer->setVideoMode(xscr, yscr, DriverMode & MD3D_FULLSCREEN);
	SetClipRect(0,0,xscr-1,yscr-1);
	Fill(0,0,0);
	Flush();
	Fill(0,0,0);
	Flush();

	BeginScene();
	EndScene();

	return 0;
}
int cGraph3dDirect3D::Release()
{
	delete this;
	return 0;
}
int cGraph3dDirect3D::IsActive()
{
	return _isActive;
}

int cGraph3dDirect3D::BeginScene()
{
	if(SwitchRenderScene>0) return 1;
	int err=_renderer->beginScene();
	if(err==0) SwitchRenderScene=1;
	else 
	{
		SwitchRenderScene=0;
		char str[256];
		port_ultoa(err,str,10);
		ErrAbort(str);
	}
	extern int gb_CurrentTexture;
	gb_CurrentTexture=0;
	InitRenderState();
	return 0;
}
int cGraph3dDirect3D::EndScene()
{
	if(SwitchRenderScene==0) return 1;
	SwitchRenderScene=0;
	SetMaterial(MAT_NULL);
	_renderer->endScene();
	return 0;
}
int cGraph3dDirect3D::NullClipRect()
{
	MD3DRECT viewport{ xScrMin, yScrMin, xScrMax - xScrMin, yScrMax - yScrMin };
	_renderer->setClipRect(viewport);
	return 0;
}
int cGraph3dDirect3D::GetClipRect(int *xmin,int *ymin,int *xmax,int *ymax)
{
	*xmin=xScrMin; *ymin=yScrMin;
	*xmax=xScrMax; *ymax=yScrMax;
	return 0;
}
int cGraph3dDirect3D::SetClipRect(int xmin,int ymin,int xmax,int ymax)
{
	assert((xmin<=xmax)&&(ymin<=ymax)&&(xmin>=0)&&(ymin>=0)&&(xmax<=xScr)&&(ymax<=yScr));
	MD3DRECT rc = { xScrMin=xmin,yScrMin=ymin,xScrMax=xmax,yScrMax=ymax};
//	if(SwitchRenderScene<=0) d3dSetClipRect(&rc);
	return 0;
}
int cGraph3dDirect3D::Fill(int r,int g,int b)
{
	if(SwitchRenderScene>0) EndScene();
	_renderer->clear((r<<16)|(g<<8)|(b<<0));
	return 0;
}
int cGraph3dDirect3D::Flush()
{
	if(SwitchRenderScene>0) EndScene();
	_renderer->flip((bool)WaitVerticalBlank);
	if(NumberPolygon&&MinNumberPolygon>NumberPolygon) MinNumberPolygon=NumberPolygon;
	if(MaxNumberPolygon<NumberPolygon) MaxNumberPolygon=NumberPolygon;
#ifdef _TEST_DIRECT3D_
	fxx<<"NumberPolygon="<<NumberPolygon<<"	("<<MinNumberPolygon<<"-"<<MaxNumberPolygon<<")\n";
#endif //_TEST_DIRECT3D_
	NumberPolygon=0;
	return 0;
}

void cGraph3dDirect3D::SetProjectionMatrix(const MD3DRECT &Viewport, const D3DMATRIX &ProjectionMatrix)
{
	_renderer->setClipRect(Viewport);
	_renderer->setProjectionMatrix(ProjectionMatrix);
}

void cGraph3dDirect3D::ResetProjectionMatrix()
{
	_renderer->resetClipRect();
	_renderer->resetProjectionMatrix();
}

int cGraph3dDirect3D::PolygonFan(void *vertex,int NumberVertex,int VertexFormat)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::PolygonStrip(void *vertex,int NumberVertex,int VertexFormat)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat)
{
	assert(0);
	return 0;
}

int cGraph3dDirect3D::BeginDrawCommand(M3D_DRAW_COMMAND &command)
{
	return _renderer->beginDrawCommand(command) == MD3D_OK;
}
int cGraph3dDirect3D::EndDrawCommand(const M3D_DRAW_COMMAND &command)
{
	return _renderer->endDrawCommand(command) == MD3D_OK;
}

int cGraph3dDirect3D::SetTexture(int hTexture, uint32_t dwStage)
{
	assert(hTexture);
	if(!SwitchRenderScene) return 1;
	return _renderer->setTexture((int)hTexture, dwStage)!=MD3D_OK;
}
int cGraph3dDirect3D::LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine)
{
	assert(hTexture);
	return _renderer->get_texture_manager().lockTexture((uint32_t)hTexture,TextureBuffer,(uint32_t *)BytePerLine)!=MD3D_OK;
}
int cGraph3dDirect3D::UnlockTexture(int hTexture)
{
	assert(hTexture);
	return _renderer->get_texture_manager().unlockTexture((int)hTexture)!=MD3D_OK;
}
int  cGraph3dDirect3D::CreateTexture(int x,int y,eTextureFormat TextureFormat)
{
	uint32_t hTexture;
	int TexFormat3d;
	switch(TextureFormat)
	{
		case TEXFMT_RGB565:
		case TEXFMT_RGB555:
			TexFormat3d=D3DTEXFMT_RGB565;
			break;
		case TEXFMT_ARGB4444:
			TexFormat3d=D3DTEXFMT_ARGB4444;
			break;
		case TEXFMT_ARGB1555:
			TexFormat3d=D3DTEXFMT_ARGB1555;
			break;
		case TEXFMT_ARGB8888:
			TexFormat3d=D3DTEXFMT_RGBA8888;
			break;
		default: 
			assert(0);
	}
	if(_renderer->get_texture_manager().createTexture(x,y,TexFormat3d,&hTexture)==MD3D_OK) 
		return hTexture;
	return 0;
}
int cGraph3dDirect3D::DeleteTexture(int hTexture)
{
	assert(hTexture);
	return _renderer->get_texture_manager().deleteTexture((int)hTexture)==MD3D_OK;
}

void* cGraph3dDirect3D::GetZBuffer()
{
	return 0;
}
void* cGraph3dDirect3D::GetScrBuffer()
{
	return 0;
}
void* cGraph3dDirect3D::GetInfo(int *graph3d)
{
	*graph3d=GRAPH3D_DIRECT3D;
	return &GraphMode;
}
int cGraph3dDirect3D::DrawLine(int x1,int y1,int x2,int y2,int c1,int c2)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r,int g,int b,int a)
{
	assert(0);
	return 0;
}
int cGraph3dDirect3D::DrawPixel(int x1,int y1,int r,int g,int b,int a)
{ 
	assert(0);
	return 0; 
}
int cGraph3dDirect3D::SetMaterial(eMaterialMode material)
{
	if(MaterialMode==material) return 0;
	// восстановление материалов
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
	{
		_renderer->setRenderState(D3DRENDERSTATE_ALPHATESTENABLE,false);
		_renderer->setRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,false);
	}
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		_renderer->setRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,false);
	if(MaterialMode&MAT_COLOR_ADD_SPECULAR)
		_renderer->setRenderState(D3DRENDERSTATE_SPECULARENABLE,false);
	if(MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		_renderer->setRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA);
		_renderer->setRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);
	}
	// установка материалов
	MaterialMode=material;
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		_renderer->setRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,true);
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
	{
		_renderer->setRenderState(D3DRENDERSTATE_ALPHATESTENABLE,true);
		_renderer->setRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,true);
	}
	if(MaterialMode&MAT_COLOR_ADD_SPECULAR)
		_renderer->setRenderState(D3DRENDERSTATE_SPECULARENABLE,true);
	if(MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		_renderer->setRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_ONE);
		_renderer->setRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE);
	}
	switch(MaterialMode&(MAT_COLOR_MOD_DIFFUSE|MAT_COLOR_MOD_TEXTURE1))
	{
		case MAT_COLOR_MOD_DIFFUSE:
			_renderer->setTextureBlendMode(MD3DTB_DIFFUSE,MD3DTB_DIFFUSE);
			break;
		case MAT_COLOR_MOD_TEXTURE1:
			_renderer->setTextureBlendMode(MD3DTB_TEXTURE1,MD3DTB_TEXTURE1);
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1:
			_renderer->setTextureBlendMode(MD3DTB_TEXTURE1_MOD_DIFFUSE,MD3DTB_TEXTURE1_MOD_DIFFUSE);
			break;
		case MAT_NULL:
			break;
		default:
			assert(0);
	}
	return 0;
}
int cGraph3dDirect3D::SetRenderState(eRenderStateOption option,int value)
{
	if(!SwitchRenderScene) return 1;
	D3DRENDERSTATETYPE type;
	switch(option)
	{
		case RENDERSTATE_NULL:
			return 1;
		case RENDERSTATE_ZTEST:
			type=D3DRENDERSTATE_ZENABLE;
			break;
		case RENDERSTATE_ZWRITE:
			type=D3DRENDERSTATE_ZWRITEENABLE;
			break;
		case RENDERSTATE_DITHER:
			type=D3DRENDERSTATE_DITHERENABLE;
			break;
		case RENDERSTATE_SPECULAR:
			type=D3DRENDERSTATE_SPECULARENABLE;
			break;
		case RENDERSTATE_TEXTUREPERSPECTIVE:
			type=D3DRENDERSTATE_TEXTUREPERSPECTIVE;
			break;
		case RENDERSTATE_ZFUNC:
			type=D3DRENDERSTATE_ZFUNC;
			break;
		case RENDERSTATE_ZBIAS:
			type=D3DRENDERSTATE_ZBIAS;
			break;
		case RENDERSTATE_FILLMODE:
			type=D3DRENDERSTATE_FILLMODE;
			break;
		case RENDERSTATE_CULLMODE:
			type=D3DRENDERSTATE_CULLMODE;
			break;
		case RENDERSTATE_SHADEMODE:
			type=D3DRENDERSTATE_SHADEMODE;
			break;
		case RENDERSTATE_SUBPIXEL:
			type=D3DRENDERSTATE_SUBPIXEL;
			break;
		case RENDERSTATE_ALPHATEST:
			type=D3DRENDERSTATE_ALPHATESTENABLE;
			break;
		case RENDERSTATE_ALPHAFUNC:
			type=D3DRENDERSTATE_ALPHAFUNC;
			break;
		case RENDERSTATE_ALPHAREF:
			type=D3DRENDERSTATE_ALPHAREF;
			break;
		case RENDERSTATE_ALPHABLEND:
			type=D3DRENDERSTATE_ALPHABLENDENABLE;
			break;
		case RENDERSTATE_SRCBLEND:
			type=D3DRENDERSTATE_SRCBLEND;
			break;
		case RENDERSTATE_DESTBLEND:
			type=D3DRENDERSTATE_DESTBLEND;
			break;
		case RENDERSTATE_TEXTUREADDRESS:
		    _renderer->setTextureStageState(0,D3DTSS_ADDRESS,value);
			return _renderer->setTextureStageState(1,D3DTSS_ADDRESS,value);
		case RENDERSTATE_TEXTUREPOINT:
		case RENDERSTATE_TEXTURELINEAR:
		default:
			assert(0);
	};
	return _renderer->setRenderState(type,value)!=MD3D_OK;
}
int cGraph3dDirect3D::GetTextureFormatData(sTextureFormatData &TexFmtData)
{
	int TexFormat3d;
	M3DTEXTUREFORMAT d3dTexFmt;
	switch(TexFmtData.TextureFormat)
	{
		case TEXFMT_RGB565:
		case TEXFMT_RGB555:
			TexFormat3d=D3DTEXFMT_RGB565;
			break;
		case TEXFMT_ARGB4444:
			TexFormat3d=D3DTEXFMT_ARGB4444;
			break;
		case TEXFMT_ARGB1555:
			TexFormat3d=D3DTEXFMT_ARGB1555;
			break;
		case TEXFMT_ARGB8888:
			TexFormat3d=D3DTEXFMT_RGBA8888;
			break;
		default: 
			assert(0);
	}
	int error=_renderer->get_texture_manager().getTextureFormatData(TexFormat3d,&d3dTexFmt);
	TexFmtData.Set(TexFmtData.TextureFormat,
		d3dTexFmt.dwRBitCount,d3dTexFmt.dwGBitCount,d3dTexFmt.dwBBitCount,d3dTexFmt.dwAlphaBitCount,
		d3dTexFmt.dwRBitShift,d3dTexFmt.dwGBitShift,d3dTexFmt.dwBBitShift,d3dTexFmt.dwAlphaBitShift);
	return error!=MD3D_OK;
}
////////////////////////// начало прочие функции //////////////////////////
int cGraph3dDirect3D::CreateSprite(uint32_t dwWidth,uint32_t dwHeight,uint32_t dwFormat,uint32_t dwFlags,uint32_t* lpdwHandle )
{
	return d3dCreateSprite(dwWidth,dwHeight,dwFormat,dwFlags,(uint32_t*)lpdwHandle);
}
int cGraph3dDirect3D::CreateChildSprite(uint32_t dwParentHandle,uint32_t dwLeft,uint32_t dwTop, 
							    uint32_t dwWidth,uint32_t dwHeight,uint32_t* lpdwHandle)
{
	return d3dCreateChildSprite(dwParentHandle,dwLeft,dwTop,dwWidth,dwHeight,(uint32_t*)lpdwHandle);
}
int cGraph3dDirect3D::DeleteSprite(uint32_t dwHandle)
{
	return d3dDeleteSprite(dwHandle);
}
int cGraph3dDirect3D::LockSprite(uint32_t dwHandle,void **lplpSprite,uint32_t *lplpPitch)
{
	return d3dLockSprite(dwHandle,lplpSprite,(uint32_t*)lplpPitch);
}
int cGraph3dDirect3D::UnlockSprite(uint32_t dwHandle)
{
	return UnlockSprite(dwHandle);
}
int cGraph3dDirect3D::SetSpriteMode(uint32_t dwHandle,uint32_t dwMode,uint32_t dwValue)
{
	return d3dSetSpriteMode(dwHandle,dwMode,dwValue);
}
int cGraph3dDirect3D::DrawSprite(uint32_t dwHandle,float dvX,float dvY,uint32_t dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate )
{
	return d3dDrawSprite(dwHandle,dvX,dvY,dwOrigin,dvScaleX,dvScaleY,dvRotate);
}
int cGraph3dDirect3D::DrawSpriteZ(uint32_t dwHandle,float dvX,float dvY,float dvZ, 
						 uint32_t dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate )
{
	return d3dDrawSpriteZ(dwHandle,dvX,dvY,dvZ,dwOrigin,dvScaleX,dvScaleY,dvRotate);
}
int cGraph3dDirect3D::ScreenShot(void *lpBuffer,uint32_t dwSize)
{
	return 0;
}
int cGraph3dDirect3D::CreateBackBuffer()
{
	return 0;
}
int cGraph3dDirect3D::ReleaseBackBuffer()
{
	return 0;
}
int cGraph3dDirect3D::GetBackBufferFormat(uint32_t *dwFormat)
{
	*dwFormat = MD3DBBFORMAT_RGB565;
	return 1;
}
int cGraph3dDirect3D::LockBackBuffer(void **lplpSurface,uint32_t *lpdwPitch)
{
	return _renderer->lockBackBuffer(lplpSurface,lpdwPitch);
}
int cGraph3dDirect3D::UnlockBackBuffer()
{
	return _renderer->unlockBackBuffer();
}
int cGraph3dDirect3D::FlushBackBuffer(MD3DRECT *lprcRect)
{
	if(SwitchRenderScene>0) EndScene();
	return _renderer->flushBackBuffer(lprcRect);
}
int cGraph3dDirect3D::SetBackBufferColorKey(uint32_t dwColor)
{
	return 0;
}
int cGraph3dDirect3D::EnableBackBufferColorKey(bool bEnable)
{
	return 0;
}
int cGraph3dDirect3D::QueryGammaSupport( MD3DGAMMASUPPORT *gmGammaSupport )
{
//	return d3dQueryGammaSupport((MD3DGAMMASUPPORT*)gmGammaSupport);
	return 0;
}
int cGraph3dDirect3D::CalibrateGamma( DDGAMMARAMP *lpRampData )
{
//	return d3dCalibrateGamma(lpRampData);
	return 0;
}
int cGraph3dDirect3D::SetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma )
{
	return 0;
}
int cGraph3dDirect3D::GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma )
{
	return 0;
}
int cGraph3dDirect3D::SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight )
{
	return 0;
}
int cGraph3dDirect3D::GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight )
{
	return 0;
}
int cGraph3dDirect3D::SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow )
{
	return 0;
}
int cGraph3dDirect3D::GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow )
{
	return 0;
}
int cGraph3dDirect3D::GetWindowHandle( void **hWnd )
{
	return 0;
}
int cGraph3dDirect3D::SetViewColor(int r,int g,int b,int a)
{
	assert(SwitchRenderScene);

	M3D_DRAW_COMMAND drawCommand;
	_renderer->beginDrawCommand(drawCommand);

	drawCommand.addPosition(0, 0, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(xScr, 0, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(xScr, yScr, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(0, yScr, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addIndex(0, 2, 1);
	drawCommand.addIndex(3, 2, 0);

	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE);
	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,	false ); 
	_renderer->setRenderState( D3DRENDERSTATE_CULLMODE,	D3DCULL_NONE ); 

	_renderer->endDrawCommand(drawCommand);

	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,	true ); 
	return 0;
}
int cGraph3dDirect3D::DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag)
{ 
	M3D_DRAW_COMMAND drawCommand;
	_renderer->beginDrawCommand(drawCommand);

	drawCommand.addPosition(x, y, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(x+dx, y, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(x+dx, y+dy, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addPosition(x, y+dy, 0.0001f);
	drawCommand.addDiffuseColor(r, g, b, a);
	drawCommand.addSpecularColor(0, 0, 0, 0);

	drawCommand.addIndex(0, 1, 2);
	drawCommand.addIndex(2, 3, 0);

	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE);
	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,	false ); 

	_renderer->endDrawCommand(drawCommand);

	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,	true ); 
	return 0; 
}
int cGraph3dDirect3D::OutText(int x,int y,char *string,int r,int g,int b,int a)
{
// TODO: @caiiiycuk investigate this
#ifdef WTF
	HDC hdc;
	RECT rect = { x, y, xScr, yScr};
	GetBackBufferDC(&hdc);
	HFONT hfont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
	HGDIOBJ hold_font = SelectObject(hdc, hfont);
	DrawText(hdc, string, strlen(string), &rect, DT_EXPANDTABS);
	SelectObject(hdc, hold_font);
	ReleaseBackBufferDC(hdc);
#endif
	return 1;
}
////////////////////////// PRIVATE //////////////////////////
void cGraph3dDirect3D::InitRenderState()
{
	_renderer->setTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);  
	_renderer->setTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); // хинт D3DTSS_COLORARG1==D3DTA_TEXTURE, иначе может не работать
	_renderer->setTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	_renderer->setTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	_renderer->setTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); // хинт D3DTSS_COLORARG1==D3DTA_TEXTURE, иначе может не работать
	_renderer->setTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	_renderer->setTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	_renderer->setTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1);  
	_renderer->setTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE ); // хинт D3DTSS_COLORARG1==D3DTA_TEXTURE, иначе может не работать
	_renderer->setTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	_renderer->setTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	_renderer->setTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); // хинт D3DTSS_COLORARG1==D3DTA_TEXTURE, иначе может не работать
	_renderer->setTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	_renderer->setTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	_renderer->setTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	_renderer->setTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

    _renderer->setTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	_renderer->setTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFN_LINEAR );
	_renderer->setTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE);
    _renderer->setTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	_renderer->setTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFN_LINEAR );
	_renderer->setTextureStageState( 1, D3DTSS_MIPFILTER, D3DTFP_NONE);

	_renderer->setRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE,true);
	_renderer->setRenderState( D3DRENDERSTATE_ANTIALIAS,D3DANTIALIAS_NONE);
	_renderer->setRenderState( D3DRENDERSTATE_ZENABLE,1);
	_renderer->setRenderState( D3DRENDERSTATE_FILLMODE,D3DFILL_SOLID);
	_renderer->setRenderState( D3DRENDERSTATE_SHADEMODE,D3DSHADE_GOURAUD);
	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,1);
	_renderer->setRenderState( D3DRENDERSTATE_ALPHATESTENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_LASTPIXEL,true);
	_renderer->setRenderState( D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA);
	_renderer->setRenderState( D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);
//	_renderer->setRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_CW);
	_renderer->setRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	_renderer->setRenderState( D3DRENDERSTATE_ZFUNC,D3DCMP_LESSEQUAL);
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAREF,1);	// 0
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATEREQUAL); //D3DCMP_ALWAYS
	_renderer->setRenderState( D3DRENDERSTATE_DITHERENABLE,true);
	_renderer->setRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_FOGENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_SPECULARENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_FOGCOLOR,0x00000000);
	_renderer->setRenderState( D3DRENDERSTATE_FOGTABLEMODE,D3DFOG_NONE);
	_renderer->setRenderState( D3DRENDERSTATE_FOGTABLESTART,0);
	_renderer->setRenderState( D3DRENDERSTATE_FOGTABLEEND,0);
	_renderer->setRenderState( D3DRENDERSTATE_FOGTABLEDENSITY,0);
	_renderer->setRenderState( D3DRENDERSTATE_FOGSTART,0);
	_renderer->setRenderState( D3DRENDERSTATE_FOGEND,0);
	_renderer->setRenderState( D3DRENDERSTATE_FOGDENSITY,0);
	_renderer->setRenderState( D3DRENDERSTATE_COLORKEYENABLE,0);
	_renderer->setRenderState( D3DRENDERSTATE_ZBIAS,0);
	_renderer->setRenderState( D3DRENDERSTATE_RANGEFOGENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_TEXTUREFACTOR,0x80FFFFFF);

	_renderer->setRenderState( D3DRENDERSTATE_CLIPPING,1);
	_renderer->setRenderState( D3DRENDERSTATE_LIGHTING,0);
	
	_renderer->setRenderState( D3DRENDERSTATE_AMBIENT,false);
	_renderer->setRenderState( D3DRENDERSTATE_FOGVERTEXMODE,false);
	_renderer->setRenderState( D3DRENDERSTATE_COLORVERTEX,1);
	_renderer->setRenderState( D3DRENDERSTATE_COLORKEYBLENDENABLE,false);
	_renderer->setRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	_renderer->setRenderState( D3DRENDERSTATE_SPECULARMATERIALSOURCE,D3DMCS_COLOR2);
	_renderer->setRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
	_renderer->setRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);
	_renderer->setRenderState( D3DRENDERSTATE_VERTEXBLEND,D3DVBLEND_DISABLE);
	_renderer->setRenderState( D3DRENDERSTATE_CLIPPLANEENABLE,false);
/*
	_renderer->setRenderState( D3DRENDERSTATE_SPECULARENABLE, FALSE );
	_renderer->setRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );
	_renderer->setRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE );
	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE,	TRUE ); 
	_renderer->setRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
	_renderer->setRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL );
	_renderer->setRenderState( D3DRENDERSTATE_ZBIAS, 0 );
	_renderer->setRenderState( D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID ); 
//	_renderer->setRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CW ); 
	_renderer->setRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE ); 
	_renderer->setRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD );

	_renderer->setRenderState( D3DRENDERSTATE_ALPHATESTENABLE,FALSE );
//	_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_ALWAYS );
//	_renderer->setRenderState( D3DRENDERSTATE_ALPHAREF,0xFF );
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATEREQUAL );
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAREF,1 );

	_renderer->setRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,FALSE );
	_renderer->setRenderState( D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA );
	_renderer->setRenderState( D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA );

    _renderer->setTextureStageState(0,D3DTSS_ADDRESS,D3DTADDRESS_WRAP);
    _renderer->setTextureStageState(1,D3DTSS_ADDRESS,D3DTADDRESS_WRAP);
*/
}

int cGraph3dDirect3D::EnumVideoMode(int* pNumVideoMode, MD3DMODE** ppArray)
{
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

	return 1;
}

int cGraph3dDirect3D::GetTextureFormatData(uint32_t dwTexFormatID, M3DTEXTUREFORMAT* pData)
{
	return _renderer->get_texture_manager().getTextureFormatData(dwTexFormatID, pData) == MD3D_OK;
}

int cGraph3dDirect3D::SetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t dwRenderState)
{
	return _renderer->setRenderState(dwRenderStateType, dwRenderState) == MD3D_OK;
}

int cGraph3dDirect3D::GetRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32_t *lpdwRenderState)
{
	return _renderer->getRenderState(dwRenderStateType, lpdwRenderState) == MD3D_OK;
}

int cGraph3dDirect3D::SetTextureStageState(uint32_t dwStage, D3DTEXTURESTAGESTATETYPE dwState, uint32_t dwValue)
{
	return _renderer->setTextureStageState(dwStage, dwState, dwValue) == MD3D_OK;
}

int cGraph3dDirect3D::SetTextureBlendMode(MD3DTEXTUREBLEND tbRGBBlend, MD3DTEXTUREBLEND tbAlphaBlend)
{
	return _renderer->setTextureBlendMode(tbRGBBlend, tbAlphaBlend) == MD3D_OK;
}

int cGraph3dDirect3D::SetSpriteRect(uint32_t dwHandle, float dvLeft, float dvTop, float dvRight, float dvBottom)
{
	return d3dSetSpriteRect(dwHandle, dvLeft, dvTop, dvRight, dvBottom) == MD3D_OK;
}

int cGraph3dDirect3D::Clear(uint32_t dwColor)
{
	return _renderer->clear(dwColor) == MD3D_OK;
}

int cGraph3dDirect3D::Flip(bool bWaitVerticalBlank)
{
	return _renderer->flip(bWaitVerticalBlank) == MD3D_OK;
}

int cGraph3dDirect3D::SetClipRect(const MD3DRECT &lprcClipRect)
{
	return _renderer->setClipRect(lprcClipRect);
}
