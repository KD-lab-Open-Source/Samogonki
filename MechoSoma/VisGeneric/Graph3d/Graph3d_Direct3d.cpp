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

// Sprites

struct TVertex
{
	float	x,y,z,rhw;
	uint32_t rgba;
	float	u,v;
};

struct TSpriteSlot {

	uint32_t dwHandle;			// If the 31-st bit is 1, it's a child sprite
							// otherwise it's a parent. 0 means the slot is free.

	uint32_t dwWidth;			// Width of the sprite
	uint32_t dwHeight;			// Height of the sprite

	uint32_t dwFlags;			// Sprite modes etc.
	uint32_t dwAlphaRef;		
	uint32_t dwAlphaFactor;
	uint32_t dwColorFactor;

	TVertex Vertices[4];	// Vertices for the triangle fan

	union {

		// Parent sprite
		struct {
		uint32_t dwChildrenCount;		// Number of children for this parent
		uint32_t dwTexHandle;			// Texture handle
		};

		// Child sprite 
		struct {
		uint32_t dwParentHandle;		// Handle of the parent sprite
		uint32_t dwLeft;				// Coordinates of the upper-left corner 
		uint32_t dwTop;				// on the parent sprite
		};
	};
};

#define SLOTS_INITIAL_SIZE 200
#define SLOTS_EXPAND_CHUNK 50

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
	free( _lpSpriteSlots );
	_lpSpriteSlots = NULL;
	_dwSpriteSlotsCount = 0;
	_dwSpriteSlotsUsed = 0;
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

	_lpSpriteSlots = (TSpriteSlot*)malloc( sizeof(TSpriteSlot)*SLOTS_INITIAL_SIZE );
	assert( NULL != _lpSpriteSlots );
	memset( _lpSpriteSlots, 0, sizeof(TSpriteSlot)*SLOTS_INITIAL_SIZE );
	_dwSpriteSlotsCount = SLOTS_INITIAL_SIZE;
	_dwSpriteSlotsUsed = 1;	// First slot is never used
	_bSpriteZEnable = false;

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
	uint32_t dwSlot;
	if( 0 == ( dwSlot = FindUnusedSlot() ) )
	{
		// No unused slots. Try to create a new one.
		assert( 0 != ( dwSlot = CreateNewSlot() ) );
	}

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwSlot];

	// Create the texture

	uint32_t hr;
	uint32_t dwTexHandle;

	assert(_renderer->get_texture_manager().createTexture( dwWidth, dwHeight, dwFormat, &dwTexHandle ) == MD3D_OK);

	// Fill the slot for the new parent sprite

	lpSprite->dwHandle = dwSlot;
	lpSprite->dwWidth = dwWidth;
	lpSprite->dwHeight = dwHeight;
	lpSprite->dwChildrenCount = 0;
	lpSprite->dwTexHandle = dwTexHandle;

	lpSprite->dwFlags = dwFlags;
	lpSprite->dwAlphaRef = 0;
	lpSprite->dwAlphaFactor = 255;
	lpSprite->dwColorFactor = RGB_MAKE(255,255,255);

	// Vertices

	for( uint32_t i = 0; i < 4; i++ ) {
		lpSprite->Vertices[i].x = 0.0f;
		lpSprite->Vertices[i].y = 0.0f;
		lpSprite->Vertices[i].z = 0.0f;
		lpSprite->Vertices[i].rhw = 1.0f;
		lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
	}
	lpSprite->Vertices[0].u = 0.0f;
	lpSprite->Vertices[0].v = 0.0f;
	lpSprite->Vertices[1].u = 1.0f;
	lpSprite->Vertices[1].v = 0.0f;
	lpSprite->Vertices[2].u = 1.0f;
	lpSprite->Vertices[2].v = 1.0f;
	lpSprite->Vertices[3].u = 0.0f;
	lpSprite->Vertices[3].v = 1.0f;

	// Return the new handle
	*lpdwHandle = dwSlot;

	return 0;
}
int cGraph3dDirect3D::CreateChildSprite(uint32_t dwParentHandle,uint32_t dwLeft,uint32_t dwTop, 
							    uint32_t dwWidth,uint32_t dwHeight,uint32_t* lpdwHandle)
{
	assert( dwParentHandle > 0 && dwParentHandle < _dwSpriteSlotsUsed );
	assert( _lpSpriteSlots[dwParentHandle].dwHandle == dwParentHandle );

	// Try to find an unused slot.
	uint32_t dwSlot;
	if( 0 == ( dwSlot = FindUnusedSlot() ) )
	{
		// No unused slots. Try to create a new one.
		assert( 0 != ( dwSlot = CreateNewSlot() ) );
	}

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwSlot];

	// Pointer to the parent sprite
	TSpriteSlot *lpParentSprite = &_lpSpriteSlots[dwParentHandle];


	// Fill the slot and give the user the new handle

	lpSprite->dwHandle = dwSlot | 0x80000000;
	lpSprite->dwParentHandle = dwParentHandle;

	lpSprite->dwLeft = dwLeft;
	lpSprite->dwTop = dwTop;
	lpSprite->dwWidth = dwWidth;
	lpSprite->dwHeight = dwHeight;

	lpSprite->dwFlags = lpParentSprite->dwFlags;
	lpSprite->dwAlphaRef = lpParentSprite->dwAlphaRef;
	lpSprite->dwAlphaFactor = lpParentSprite->dwAlphaFactor;
	lpSprite->dwColorFactor = lpParentSprite->dwColorFactor;

	for( uint32_t i = 0; i < 4; i++ ) {
		lpSprite->Vertices[i].x = 0.0f;
		lpSprite->Vertices[i].y = 0.0f;
		lpSprite->Vertices[i].z = 0.0f;
		lpSprite->Vertices[i].rhw = 1.0f;
		lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
	}

	float dvLeft = float(lpSprite->dwLeft) / float(lpParentSprite->dwWidth);
	float dvTop = float(lpSprite->dwTop) / float(lpParentSprite->dwHeight);
	float dvRight = float(lpSprite->dwLeft + lpSprite->dwWidth) / float(lpParentSprite->dwWidth);
	float dvBottom = float(lpSprite->dwTop + lpSprite->dwHeight) / float(lpParentSprite->dwHeight);

	lpSprite->Vertices[0].u = dvLeft;
	lpSprite->Vertices[0].v = dvTop;
	lpSprite->Vertices[1].u = dvRight;
	lpSprite->Vertices[1].v = dvTop;
	lpSprite->Vertices[2].u = dvRight;
	lpSprite->Vertices[2].v = dvBottom;
	lpSprite->Vertices[3].u = dvLeft;
	lpSprite->Vertices[3].v = dvBottom;

	// Increment parent sprite's child count
	lpParentSprite->dwChildrenCount++;

	// Return the handle
	*lpdwHandle = dwSlot;

	return 0;
}
int cGraph3dDirect3D::DeleteSprite(uint32_t dwHandle)
{
	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];

	// If this is a child sprite, just free the slot
	if( 0 != (lpSprite->dwHandle & 0x80000000) ) {
		// Mark the slot as free
		lpSprite->dwHandle = 0;

		// Decrement the child count for the parent slot
		_lpSpriteSlots[lpSprite->dwParentHandle].dwChildrenCount--;

		return MD3D_OK;
	}

	// Else it's a parent slot

	// First, free all the children if any
	if( 0 != lpSprite->dwChildrenCount ) {
		
		uint32_t dwSlot;
		for( dwSlot = 1; dwSlot < _dwSpriteSlotsUsed; dwSlot++ )
		{
			if( (_lpSpriteSlots[dwSlot].dwHandle & 0x80000000) != 0 && 
				 _lpSpriteSlots[dwSlot].dwParentHandle == dwHandle ) {

				// Mark the child slot as free
				_lpSpriteSlots[dwSlot].dwHandle = 0;

#ifdef _DEBUG
				lpSprite->dwChildrenCount--;
#endif
			}
		}
		assert( 0 == lpSprite->dwChildrenCount );
	}

	// Free the texture handle 
	assert(_renderer->get_texture_manager().deleteTexture( lpSprite->dwTexHandle ) == MD3D_OK);

	// Mark the slot as free
	lpSprite->dwHandle = 0;

	return 0;
}
int cGraph3dDirect3D::LockSprite(uint32_t dwHandle,void **lplpSprite,uint32_t *lplpPitch)
{
	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];

	// Lock the sprite texture
	uint32_t hr;
	if( (lpSprite->dwHandle & 0x80000000) == 0 ) {
		// It's a parent sprite
		assert(_renderer->get_texture_manager().lockTexture( lpSprite->dwTexHandle, lplpSprite, lplpPitch ) == MD3D_OK);
	} else {
		// It's a child sprite
		uint32_t dwTexHandle = _lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;
		assert(_renderer->get_texture_manager().lockTexture( dwTexHandle, lpSprite->dwLeft, lpSprite->dwTop,
							 lpSprite->dwLeft + lpSprite->dwWidth-1, 
							 lpSprite->dwTop + lpSprite->dwHeight-1,	
							 lplpSprite, lplpPitch ) == MD3D_OK);
	}

	return 0;
}
int cGraph3dDirect3D::UnlockSprite(uint32_t dwHandle)
{
	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );


	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];

	// Get the texture handle
	uint32_t dwTexHandle;
	if( (lpSprite->dwHandle & 0x80000000) == 0 ) {
		// It's a parent sprite
		dwTexHandle = lpSprite->dwTexHandle;
	} else {
		// It's a child sprite
		dwTexHandle = _lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;
	}

	return _renderer->get_texture_manager().unlockTexture( dwTexHandle );
}
int cGraph3dDirect3D::SetSpriteMode(uint32_t dwHandle,uint32_t dwMode,uint32_t dwValue)
{
	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];
	uint32_t i;

	// Set mode for this sprite

	switch( dwMode ) {
	case MD3DSP_ALPHATESTENABLE:
		if( dwValue )
			lpSprite->dwFlags |= MD3DSP_USEALPHATEST;
		else
			lpSprite->dwFlags &= ~MD3DSP_USEALPHATEST;

		break;

	case MD3DSP_ALPHABLENDENABLE:
		if( dwValue )
			lpSprite->dwFlags |= MD3DSP_USEALPHABLEND;
		else
			lpSprite->dwFlags &= ~MD3DSP_USEALPHABLEND;

		break;

	case MD3DSP_ALPHAREF:
		lpSprite->dwAlphaRef = dwValue;
		break;

	case MD3DSP_COLORFACTOR:
		lpSprite->dwColorFactor = dwValue & RGBA_MAKE(255,255,255,0);
		for( i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
		}
		break;

	case MD3DSP_ALPHAFACTOR:
		lpSprite->dwAlphaFactor = dwValue;
		for( i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
		}
		break;
	}


	// See if this sprite has children

	if( (lpSprite->dwHandle & 0x80000000) == 0 && lpSprite->dwChildrenCount != 0) {
		for( uint32_t dwSlot = 1; dwSlot < _dwSpriteSlotsUsed; dwSlot++ )
		{
			if( (_lpSpriteSlots[dwSlot].dwHandle & 0x80000000) != 0 && 
				 _lpSpriteSlots[dwSlot].dwParentHandle == dwHandle ) {

				// Pointer to the child sprite
				TSpriteSlot *lpChild = &_lpSpriteSlots[dwSlot];

				// Set mode for this child

				switch( dwMode ) {
				case MD3DSP_ALPHATESTENABLE:
					if( dwValue )
						lpSprite->dwFlags |= MD3DSP_USEALPHATEST;
					else
						lpSprite->dwFlags &= ~MD3DSP_USEALPHATEST;

					break;

				case MD3DSP_ALPHABLENDENABLE:
					if( dwValue )
						lpSprite->dwFlags |= MD3DSP_USEALPHABLEND;
					else
						lpSprite->dwFlags &= ~MD3DSP_USEALPHABLEND;

					break;

				case MD3DSP_ALPHAREF:
					lpSprite->dwAlphaRef = dwValue;
					break;

				case MD3DSP_COLORFACTOR:
					lpSprite->dwColorFactor = dwValue & RGBA_MAKE(255,255,255,0);
					for( i = 0; i < 4; i++ ) {
						lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
					}
					break;

				case MD3DSP_ALPHAFACTOR:
					lpSprite->dwAlphaFactor = dwValue;
					for( i = 0; i < 4; i++ ) {
						lpSprite->Vertices[i].rgba = lpSprite->dwColorFactor | RGBA_MAKE(0,0,0,lpSprite->dwAlphaFactor);
					}
					break;
				}
			}
		}
	}

	return 0;
}
int cGraph3dDirect3D::DrawSprite(uint32_t dwHandle,float dvX,float dvY,uint32_t dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate )
{
	assert( _renderer->isInScene() );

	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];

	// Texutre handle
	uint32_t dwTexHandle;
	if( (lpSprite->dwHandle & 0x80000000) == 0 )
		dwTexHandle = lpSprite->dwTexHandle;
	else
		dwTexHandle = _lpSpriteSlots[lpSprite->dwParentHandle].dwTexHandle;

	uint32_t hr;

	// Set current texture to the sprite texture
	hr = _renderer->setTexture( dwTexHandle, 0 );
	if( hr < 0 )
		return hr;

	// Calculate coordinates

	float dvWidth, dvHeight;
	float dvLeft, dvTop, dvRight, dvBottom;

	if( lpSprite->dwHandle & 0x80000000 )
	{
		// Child
		dvWidth = float(lpSprite->dwWidth);
		dvHeight = float(lpSprite->dwHeight);
	} else {
		dvWidth = float(lpSprite->dwWidth) * (lpSprite->Vertices[1].u - lpSprite->Vertices[0].u);
		dvHeight = float(lpSprite->dwHeight) * (lpSprite->Vertices[2].v - lpSprite->Vertices[1].v);
	}

	switch(dwOrigin) {
	case MD3DORG_CENTER:
		if( dvScaleX == 1.0f ) {
			dvLeft = dvX - dvWidth/2.0f;
			dvRight = dvX + dvWidth/2.0f;
		} else {
			dvLeft = dvX - (dvWidth/2.0f)*dvScaleX;
			dvRight = dvX + (dvWidth/2.0f)*dvScaleX;
		}
		if( dvScaleY == 1.0f ) {
			dvTop = dvY - dvHeight/2.0f;
			dvBottom = dvY + dvHeight/2.0f;
		} else {
			dvTop = dvY - (dvHeight/2.0f)*dvScaleY;
			dvBottom = dvY + (dvHeight/2.0f)*dvScaleY;
		}

		break;

	case MD3DORG_TOPLEFT:
		dvLeft = dvX;
		dvTop = dvY;

		if( dvScaleX == 1.0f ) {
			dvRight = dvX + dvWidth;
		} else {
			dvRight = dvX + dvWidth*dvScaleX;
		}
		if( dvScaleY == 1.0f ) {
			dvBottom = dvY + dvHeight;
		} else {
			dvBottom = dvY + dvHeight*dvScaleY;
		}

		break;
	}

	lpSprite->Vertices[0].x = dvLeft;
	lpSprite->Vertices[0].y = dvTop;

	lpSprite->Vertices[1].x = dvRight;
	lpSprite->Vertices[1].y = dvTop;

	lpSprite->Vertices[2].x = dvRight;
	lpSprite->Vertices[2].y = dvBottom;

	lpSprite->Vertices[3].x = dvLeft;
	lpSprite->Vertices[3].y = dvBottom;
	
	// See if we have to rotate the sprite

	if( dvRotate != 0.0f ) {
		float sin_a = (float)sin(dvRotate);
		float cos_a = (float)cos(dvRotate);

		float xc = (dvRight+dvLeft)/2.0f;
		float yc = (dvBottom+dvTop)/2.0f;
		float x;
		float y;
		for( uint32_t i = 0; i < 4; i++ ) {
			x = lpSprite->Vertices[i].x - xc;
			y = lpSprite->Vertices[i].y - yc;
			lpSprite->Vertices[i].x = x*cos_a - y*sin_a + xc;
			lpSprite->Vertices[i].y = x*sin_a + y*cos_a + yc;
		}
	}

	// If Z is enabled, set z values in the vertices
	if( _bSpriteZEnable ) {
		for( uint32_t i = 0; i < 4; i++ ) {
			lpSprite->Vertices[i].z = _dvSpriteZ;
		}
	}


	// Save current render states

	uint32_t dwAlphaTestEnable;
	uint32_t dwAlphaFunc;
	uint32_t dwAlphaRef;
	uint32_t dwAlphaBlendEnable;
	uint32_t dwSrcFactor;
	uint32_t dwDestFactor;
	uint32_t dwZEnable;
	uint32_t dwZWriteEnable;

	_renderer->getRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTestEnable );
	_renderer->getRenderState( D3DRENDERSTATE_ALPHAFUNC, &dwAlphaFunc );
	_renderer->getRenderState( D3DRENDERSTATE_ALPHAREF, &dwAlphaRef );

	_renderer->getRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlendEnable );
	_renderer->getRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcFactor );
	_renderer->getRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestFactor );

	_renderer->getRenderState( D3DRENDERSTATE_ZENABLE, &dwZEnable );
	_renderer->getRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable );

	// Set render states

	if( lpSprite->dwFlags & MD3DSP_USEALPHATEST ) {
		_renderer->setRenderState( D3DRENDERSTATE_ALPHATESTENABLE, true );
		_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		_renderer->setRenderState( D3DRENDERSTATE_ALPHAREF, lpSprite->dwAlphaRef );
	} else {
		_renderer->setRenderState( D3DRENDERSTATE_ALPHATESTENABLE, false );
		_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_ALWAYS );
	}

	if( lpSprite->dwFlags & MD3DSP_USEALPHABLEND ) {
		_renderer->setRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, true );
		_renderer->setRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		_renderer->setRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	} else {
		_renderer->setRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, false );
	}

	if( _bSpriteZEnable ) {
		_renderer->setRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
		_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE, false );
	} else {
		_renderer->setRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_FALSE );
		_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE, false );
	}

	_renderer->setTextureBlendMode( MD3DTB_TEXTURE1_MOD_DIFFUSE, 
				//MD3DTB_TEXTURE1);//Для совсем слабеньких карточек
				MD3DTB_TEXTURE1_MOD_DIFFUSE );

	_renderer->setRenderState( D3DRENDERSTATE_SPECULARENABLE, false );

	M3D_DRAW_COMMAND drawCommand;
	_renderer->beginDrawCommand(drawCommand);

	for (int i = 0; i < 4; i++)
	{
		drawCommand.addPosition(lpSprite->Vertices[i].x, lpSprite->Vertices[i].y, lpSprite->Vertices[i].z);
		drawCommand.addDiffuseColor(
			((lpSprite->Vertices[i].rgba >> 16) & 0xFF),
			((lpSprite->Vertices[i].rgba >> 8) & 0xFF),
			(lpSprite->Vertices[i].rgba & 0xFF),
			((lpSprite->Vertices[i].rgba >> 24) & 0xFF)
		);
		drawCommand.addSpecularColor(0, 0, 0, 0);
		drawCommand.addUV(lpSprite->Vertices[i].u, lpSprite->Vertices[i].v);
	}
	drawCommand.addIndex(2, 1, 0);
	drawCommand.addIndex(3, 2, 0);

	_renderer->endDrawCommand(drawCommand);

//	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_CW);


	// Restore render states

	_renderer->setRenderState( D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTestEnable );
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAFUNC, dwAlphaFunc );
	_renderer->setRenderState( D3DRENDERSTATE_ALPHAREF, dwAlphaRef );

	_renderer->setRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlendEnable );
	_renderer->setRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcFactor );
	_renderer->setRenderState( D3DRENDERSTATE_DESTBLEND, dwDestFactor );

	_renderer->setRenderState( D3DRENDERSTATE_ZENABLE, dwZEnable );
	_renderer->setRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable );

	return 0;
}
int cGraph3dDirect3D::DrawSpriteZ(uint32_t dwHandle,float dvX,float dvY,float dvZ, 
						 uint32_t dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate )
{
	_dvSpriteZ = dvZ;
	_bSpriteZEnable = true;

	MD3DERROR hr;
	hr = DrawSprite( dwHandle, dvX, dvY, dwOrigin, dvScaleX, dvScaleY, dvRotate );

	_bSpriteZEnable = false;

	return hr;
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
	// Check if the handle is valid
	assert( dwHandle > 0 && dwHandle < _dwSpriteSlotsUsed );
	assert( (_lpSpriteSlots[dwHandle].dwHandle & 0x7FFFFFFF) == dwHandle );

	// Make sure the coordinates are valid
	assert( dvLeft <= dvRight );
	assert( dvTop <= dvBottom );
	assert( dvLeft >= 0.0f && dvLeft <= 1.0f );
	assert( dvTop >= 0.0f && dvTop <= 1.0f );
	assert( dvRight >= 0.0f && dvRight <= 1.0f );
	assert( dvBottom >= 0.0f && dvBottom <= 1.0f );

	// Pointer to this sprite
	TSpriteSlot *lpSprite = &_lpSpriteSlots[dwHandle];

	// Change texture coordinates to the rectangle specified
	lpSprite->Vertices[0].u = dvLeft;
	lpSprite->Vertices[0].v = dvTop;
	lpSprite->Vertices[1].u = dvRight;
	lpSprite->Vertices[1].v = dvTop;
	lpSprite->Vertices[2].u = dvRight;
	lpSprite->Vertices[2].v = dvBottom;
	lpSprite->Vertices[3].u = dvLeft;
	lpSprite->Vertices[3].v = dvBottom;

	return 0;
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

uint32_t cGraph3dDirect3D::FindUnusedSlot()
{
	uint32_t dwSlot;

	for( dwSlot = 1; dwSlot < _dwSpriteSlotsUsed; dwSlot++ )
	{
		if( 0 == _lpSpriteSlots[dwSlot].dwHandle )
			return dwSlot;	// Found
	}

	// Not found.
	return 0;
}

uint32_t cGraph3dDirect3D::CreateNewSlot()
{
	if( _dwSpriteSlotsUsed < _dwSpriteSlotsCount )
	{
		// We still have free space
		_dwSpriteSlotsUsed++;
		return _dwSpriteSlotsUsed-1;
	}

	// Else we need to expand the array

	assert( NULL != ( _lpSpriteSlots = (TSpriteSlot *)realloc( _lpSpriteSlots, 
		 (_dwSpriteSlotsCount+SLOTS_EXPAND_CHUNK)*sizeof(TSpriteSlot) ) ) );

	// Clear the newly allocated block
	memset( _lpSpriteSlots + _dwSpriteSlotsCount, 0, SLOTS_EXPAND_CHUNK*sizeof(TSpriteSlot) );

	_dwSpriteSlotsCount += SLOTS_EXPAND_CHUNK;

	_dwSpriteSlotsUsed++;
	return _dwSpriteSlotsUsed-1;
}
