#include "BaseDefine.h"
#include "xgraph.h"
#include "Graph3d_Software.h"
#include "Software16.h"

inline int ReturnBit(int a)
{
	int count=0;
	while((a>>=1)>=1) count++;
	return count;
}

sTextureSoftware::sTextureSoftware(int id,eTextureFormat fmtTex,int xTex,int yTex)
{
	ID=id;
	_x=ReturnBit(xTex); _y=ReturnBit(yTex);
	switch(format=fmtTex)
	{
		case TEXFMT_PAL8:
			bpl=sizeof(unsigned char)<<_x;
			break;
		case TEXFMT_RGB565:
		case TEXFMT_RGB555:
		case TEXFMT_ARGB4444:
		case TEXFMT_ARGB1555:
			bpl=sizeof(unsigned short)<<_x;
			break;
		case TEXFMT_ARGB8888:
			bpl=sizeof(unsigned int)<<_x;
			break;
		default:
			assert(0);
	}
	buf=new unsigned char[bpl<<_y];
}

cGraph3dSoftware::cGraph3dSoftware()
{
	scr=0;
	zBuffer=0;
	GraphMode=GRAPH3D_MODE_NULL;
	xScr=yScr=0;
	xScrMin=yScrMin=xScrMax=yScrMax=0;
	AlfaRef=1;
	rPal16=gPal16=bPal16=0;
	RenderState_zwrite=0;
}
cGraph3dSoftware::~cGraph3dSoftware()
{
	assert(TextureLibrary.BaseList==0);
}
	
int cGraph3dSoftware::Init(int xscr,int yscr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon)
{
	int XGR_flag=0;
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	if(GraphMode&GRAPH3D_MODE_RGB16) XGR_flag|=XGR_HICOLOR;
	else if(GraphMode&GRAPH3D_MODE_RGB32) XGR_flag|=XGR_HICOLOR;
	if((GraphMode&GRAPH3D_MODE_WINDOW)==0) XGR_flag|=XGR_EXCLUSIVE;
	if(XGR_Obj.ScreenBuf==0)
		XGR_Init(xScr,yScr,XGR_flag);
	scr=(unsigned short*)XGR_VIDEOBUF;
	assert(GraphMode&GRAPH3D_MODE_Z16);
	zBuffer=new unsigned short[xScr*yScr];
	if(GraphMode&GRAPH3D_MODE_Z16) InitColor16();
	SwitchRenderScene=-1;
	SetRenderState(RENDERSTATE_ZWRITE,TRUE);
	Fill(0,0,0);
	return 0;
}
int cGraph3dSoftware::ReInit(int xscr,int yscr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon)
{
	int XGR_flag=0;
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	if(GraphMode&GRAPH3D_MODE_RGB16) XGR_flag|=XGR_HICOLOR;
	else if(GraphMode&GRAPH3D_MODE_RGB32) XGR_flag|=XGR_HICOLOR;
	if((GraphMode&GRAPH3D_MODE_WINDOW)==0) XGR_flag|=XGR_EXCLUSIVE;
	XGR_Finit();
	if(XGR_Obj.ScreenBuf==0)
		XGR_Init(xScr,yScr,XGR_flag);
	scr=(unsigned short*)XGR_VIDEOBUF;
	assert(GraphMode&GRAPH3D_MODE_Z16);
	zBuffer=new unsigned short[xScr*yScr];
	if(GraphMode&GRAPH3D_MODE_Z16) InitColor16();
	SwitchRenderScene=-1;
	SetRenderState(RENDERSTATE_ZWRITE,TRUE);
	Fill(0,0,0);
	return 0;
}
int cGraph3dSoftware::GetClipRect(int *xmin,int *ymin,int *xmax,int *ymax)
{
	*xmin=xScrMin; *ymin=yScrMin;
	*xmax=xScrMax; *ymax=yScrMax;
	return 0;
}
int cGraph3dSoftware::SetClipRect(int xmin,int ymin,int xmax,int ymax)
{
	assert((xmin<=xmax)&&(ymin<=ymax)&&(xmin>=0)&&(ymin>=0)&&(xmax<xScr)&&(ymax<yScr));
	xScrMin=xmin; yScrMin=ymin;
	xScrMax=xmax; yScrMax=ymax;
	return 0;
}
int cGraph3dSoftware::Release()
{
	if(GraphMode)
	{
		XGR_Obj.close();
		if(zBuffer) delete zBuffer; zBuffer=0;
		for(sTextureSoftwareList *start=TextureLibrary.BaseList;start;start=TextureLibrary.BaseList)
			TextureLibrary.Delete(start->Base);
	}
	delete this;
	return 0;
}
int cGraph3dSoftware::IsActive()
{
	return 1;
}

int cGraph3dSoftware::BeginScene()
{
	if(SwitchRenderScene>0) return 1;
	SwitchRenderScene=1;
	return 0;
}
int cGraph3dSoftware::EndScene()
{
	if(SwitchRenderScene==0) return 1;
	SwitchRenderScene=0;
	MaterialMode=MAT_NULL;
	return 0;
}
int cGraph3dSoftware::Fill(int r,int g,int b)
{
	if(SwitchRenderScene>0) EndScene();
	XGR_Fill16(GetColor(r,g,b));
	if(zBuffer) 
	{
		unsigned long *tmp=(unsigned long*)zBuffer,*end=0;
		if(GraphMode&GRAPH3D_MODE_Z32) end=tmp+xScr*yScr;
		if(GraphMode&GRAPH3D_MODE_Z16) end=tmp+xScr*yScr/2;
		while(tmp<end) (*(tmp++))=0xFFFFFFFF;
	}
	return 0;
}
int cGraph3dSoftware::Flush()
{
	if(SwitchRenderScene>0) EndScene();
	XGR_Flush(0,0,xScr,yScr);
	return 0;
}

int cGraph3dSoftware::PolygonFan(void *vertex,int NumberVertex,int VertexFormat)
{
	if(!SwitchRenderScene) return 1;
	return 0;
}
int cGraph3dSoftware::PolygonStrip(void *vertex,int NumberVertex,int VertexFormat)
{
	if(!SwitchRenderScene) return 1;
	return 0;
}
int cGraph3dSoftware::PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat)
{
	if(!SwitchRenderScene) return 1;
	assert(polygon&&vertex);
	assert(VertexFormat==VERTEXFMT_FIX);
	assert((NumberVertex<65536)&&(NumberVertex>0)&&(NumberPolygon>0));
	sVertexFix *vFix=(sVertexFix*)vertex;
	sPolygonFix *pFix=(sPolygonFix*)polygon,*pFixEnd=&pFix[NumberPolygon];
	switch(MaterialMode)
	{
		case MAT_COLOR_MOD_DIFFUSE:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZTW_CMD,L16_ZTW_CMD);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZT_CMD_AMD,L16_ZT_CMD_AMD);
			}
			break;
		case MAT_COLOR_MOD_TEXTURE1:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZTW_CMT1,L16_ZTW_CMT1);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZTW_CMDT1,L16_ZTW_CMDT1);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_TEXTURE1:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_TEXTURE1:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MASK_TEXTURE1:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_MASK_TEXTURE1:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZT_CMDT1_AMDT1,L16_ZT_CMDT1_AMDT1);
			}
			break;
		case MAT_COLOR_MOD_TEXTURE1|MAT_ALPHA_MOD_TEXTURE1:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZT_CMT1_AMT1,L16_ZT_CMT1_AMT1);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ADD_SPECULAR:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZTW_CMDT1AS,L16_ZTW_CMDT1AS);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_ADD_SPECULAR:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZTW_CMDAS,L16_ZTW_CMDAS);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE|MAT_COLOR_ADD_SPECULAR:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_TEXTURE1|MAT_COLOR_ADD_SPECULAR:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_TEXTURE1|MAT_COLOR_ADD_SPECULAR:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MASK_TEXTURE1|MAT_COLOR_ADD_SPECULAR:
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1_ALPHA_MOD_DIFFUSE_MASK_TEXTURE1|MAT_COLOR_ADD_SPECULAR:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZT_CMDT1AS_AMDT1,L16_ZT_CMDT1AS_AMDT1);
			}
			break;
		case MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE|MAT_COLOR_ADD_SPECULAR:
			for(;pFix<pFixEnd;pFix++)
			{
				sVertexFix &a=vFix[pFix->p1],&b=vFix[pFix->p2],&c=vFix[pFix->p3];
				SOFTWARE_DRAW_FIX(a,b,c,R16_ZT_CMDAS_AMD,L16_ZT_CMDAS_AMD);
			}
			break;
		default:
			assert(0);
	}
	return 0;
}
int cGraph3dSoftware::PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat)
{
	return -1;
}

int cGraph3dSoftware::SetTexture(int hTexture)
{
	assert(hTexture);
	if(!SwitchRenderScene) return 1;
	sTextureSoftware *Texture=GetTexture(hTexture)->Base;
	texture=Texture->buf;
	_xt1=Texture->_x; xst1_=(1<<_xt1)<<PREC_DRAW; xmt1=((1<<_xt1)-1)<<PREC_DRAW;
	_yt1=Texture->_y; yst1_=(1<<_yt1)<<PREC_DRAW; ymt1=((1<<_yt1)-1)<<(PREC_DRAW+_xt1);
	return 0;
}
int cGraph3dSoftware::LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine)
{
	assert(hTexture);
	sTextureSoftware *Texture=GetTexture(hTexture)->Base;
	*TextureBuffer=Texture->buf;
	*BytePerLine=Texture->bpl;
	return 0;
}
int cGraph3dSoftware::UnlockTexture(int hTexture)
{
	assert(hTexture);
	sTextureSoftware *Texture=GetTexture(hTexture)->Base;
	return 0;
}
int  cGraph3dSoftware::CreateTexture(int x,int y,eTextureFormat TextureFormat)
{
	TextureLibrary.Attach(new sTextureSoftware(++TextureLibrary.NumberID,TextureFormat,x,y));
	return TextureLibrary.NumberID;
}
int cGraph3dSoftware::DeleteTexture(int hTexture)
{
	sTextureSoftwareList *TextureList=GetTexture(hTexture);
	delete TextureList->Base;
	TextureList->Base=0;
	TextureLibrary.Detach(TextureList);
	return 0;
}
int cGraph3dSoftware::SetMaterial(eMaterialMode material)
{
	if(!SwitchRenderScene) return 1;
	MaterialMode=material;
	return 0;
}
int cGraph3dSoftware::SetRenderState(eRenderStateOption option,int value)
{
	if(!SwitchRenderScene) return 1;
	switch(option)
	{
		case RENDERSTATE_NULL:
			return 1;
		case RENDERSTATE_ZTEST:
			break;
		case RENDERSTATE_ZWRITE:
			RenderState_zwrite=value;
			break;
		case RENDERSTATE_DITHER:
			break;
		case RENDERSTATE_SPECULAR:
			break;
		case RENDERSTATE_TEXTUREPERSPECTIVE:
			break;
		case RENDERSTATE_ZFUNC:
			break;
		case RENDERSTATE_ZBIAS:
			break;
		case RENDERSTATE_FILLMODE:
			break;
		case RENDERSTATE_CULLMODE:
			break;
		case RENDERSTATE_SHADEMODE:
			break;
		case RENDERSTATE_SUBPIXEL:
			break;
		case RENDERSTATE_ALPHATEST:
			break;
		case RENDERSTATE_ALPHAFUNC:
			break;
		case RENDERSTATE_ALPHAREF:
			AlfaRef=value;
			break;
		case RENDERSTATE_ALPHABLEND:
			break;
		case RENDERSTATE_SRCBLEND:
			break;
		case RENDERSTATE_DESTBLEND:
			break;
		case RENDERSTATE_TEXTUREADDRESS:
			break;
		case RENDERSTATE_TEXTUREPOINT:
			break;
		case RENDERSTATE_TEXTURELINEAR:
			break;
		default:
			assert(0);
	};
	return 0;
}
int cGraph3dSoftware::GetTextureFormatData(sTextureFormatData &TexFmtData)
{
	switch(TexFmtData.TextureFormat)
	{
/*		case TEXFMT_PAL8:
			return 1;
			break;
		case TEXFMT_RGB565:
			TexFmtData.Set(5,6,5,0,11,5,0,0);
			break;
		case TEXFMT_RGB555:
			TexFmtData.Set(5,5,5,0,10,5,0,0);
			break;
		case TEXFMT_ARGB1555:
			TexFmtData.Set(5,5,5,1,10,5,0,15);
			break;
		case TEXFMT_ARGB4444:
			TexFmtData.Set(4,4,4,4,8,4,0,12);
			break;
		case TEXFMT_ARGB8888:
			TexFmtData.Set(8,8,8,8,16,8,0,24);
			break;
*/
		case TEXFMT_RGB565:
		case TEXFMT_RGB555:
		case TEXFMT_ARGB4444:
		case TEXFMT_ARGB1555:
		case TEXFMT_ARGB8888:
			TexFmtData.Set(TEXFMT_ARGB4444,4,4,4,4,8,4,0,12);
			break;
		default:
			assert(0);
	}
	return 0;
}

void* cGraph3dSoftware::GetZBuffer()
{
	return zBuffer;
}
void* cGraph3dSoftware::GetScrBuffer()
{
	return scr;
}
void* cGraph3dSoftware::GetInfo(int *graph3d)
{
	*graph3d=GRAPH3D_SOFTWARE;
	return &GraphMode;
}
int cGraph3dSoftware::DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a)
{
	if((xScrMin<=x1)&&(x1<xScrMax)&&(xScrMin<=x2)&&(x2<xScrMax)&&
		(yScrMin<=y1)&&(y1<yScrMax)&&(yScrMin<=y2)&&(y2<yScrMax))
		XGR_Line16(x1,y1,x2,y2,GetColor(r,g,b));
	return 0;
}
int cGraph3dSoftware::DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r,int g,int b,int a)
{
	int ix1=mround(x1),iy1=mround(y1),ix2=mround(x2),iy2=mround(y2);
	if((xScrMin<=ix1)&&(ix1<xScrMax)&&(xScrMin<=ix2)&&(ix2<xScrMax)&&
		(yScrMin<=iy1)&&(iy1<yScrMax)&&(yScrMin<=iy2)&&(iy2<yScrMax))
		XGR_Line16(ix1,iy1,ix2,iy2,GetColor(r,g,b));
	return 0;
}
int cGraph3dSoftware::DrawPixel(int x1,int y1,int r,int g,int b,int a)
{
	if((xScrMin<=x1)&&(x1<xScrMax)&&(yScrMin<=y1)&&(y1<yScrMax))
		XGR_SetPixel16(x1,y1,GetColor(r,g,b));
	return 0;
}
int cGraph3dSoftware::DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag)
{ 
	XGR_Rectangle16(x,y,dx,dy,GetColor(r,g,b),GetColor(r,g,b),flag);
	return 0; 
}
////////////////////////// PRIVATE //////////////////////////
void cGraph3dSoftware::InitColor16()
{
	switch(XGR_HighColorMode)
	{
		case 0: //5x6x5
			rBitCount=5; gBitCount=6; bBitCount=5;
			rBitShift=11; gBitShift=5; bBitShift=0;
			ScrTextureFormat=TEXFMT_RGB565;
			break;
		case 1: //5x5x5
			rBitCount=5; gBitCount=5; bBitCount=5;
			rBitShift=10; gBitShift=5; bBitShift=0;
			ScrTextureFormat=TEXFMT_RGB555;
			break;
		default:
			assert(0);
	}
	rPal16=&rPal16Mem[-GRAPH3D_SOFTWARE_TONE_MIN];
	gPal16=&gPal16Mem[-GRAPH3D_SOFTWARE_TONE_MIN];
	bPal16=&bPal16Mem[-GRAPH3D_SOFTWARE_TONE_MIN];
	for(int i=GRAPH3D_SOFTWARE_TONE_MIN;i<GRAPH3D_SOFTWARE_TONE_MAX;i++)
	{ // переводит 256 компонентный цвет (8 битный) в 16 битный режим (Hicolor)
		int tone;
		if(i<0)	tone=0; else if(i<256) tone=i; else tone=255;
		rPal16[i]=(tone>>(8-rBitCount))<<rBitShift;
		gPal16[i]=(tone>>(8-gBitCount))<<gBitShift;
		bPal16[i]=(tone>>(8-bBitCount))<<bBitShift;
	}
}
////////////////////////// начало прочие функции //////////////////////////
int cGraph3dSoftware::CreateSprite(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat,DWORD dwFlags,DWORD* lpdwHandle)
{
	*lpdwHandle=0;
	return 1;
}
int cGraph3dSoftware::CreateChildSprite(DWORD dwParentHandle,DWORD dwLeft,DWORD dwTop, 
							    DWORD dwWidth,DWORD dwHeight,DWORD* lpdwHandle)
{
	*lpdwHandle=0;
	return 1;
}
int cGraph3dSoftware::DeleteSprite(DWORD dwHandle)
{
	return 0;
}
int cGraph3dSoftware::LockSprite(DWORD dwHandle,void **lplpSprite,DWORD *lplpPitch)
{
	lplpSprite=0;
	return 1;
}
int cGraph3dSoftware::UnlockSprite(DWORD dwHandle)
{
	return 0;
}
int cGraph3dSoftware::SetSpriteMode(DWORD dwHandle,DWORD dwMode,DWORD dwValue)
{
	return 0;
}
int cGraph3dSoftware::DrawSprite(DWORD dwHandle,float dvX,float dvY,DWORD dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate )
{
	return 0;
}
int cGraph3dSoftware::DrawSpriteZ(DWORD dwHandle,float dvX,float dvY,float dvZ, 
						 DWORD dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate )
{
	return 0;
}
int cGraph3dSoftware::ScreenShot(VOID *lpBuffer,DWORD dwSize)
{
	return 0;
}
int cGraph3dSoftware::CreateBackBuffer()
{
	return 0;
}
int cGraph3dSoftware::ReleaseBackBuffer()
{
	return 0;
}
int cGraph3dSoftware::GetBackBufferFormat(DWORD *dwFormat)
{
	return 0;
}
int cGraph3dSoftware::LockBackBuffer(void **lplpSurface,DWORD *lpdwPitch)
{
	return 0;
}
int cGraph3dSoftware::UnlockBackBuffer()
{
	return 0;
}
int cGraph3dSoftware::FlushBackBuffer(RECT *lprcRect)
{
	XGR_Flush(lprcRect->left,lprcRect->top,lprcRect->right-lprcRect->left,lprcRect->bottom-lprcRect->top);
	return 0;
}
int cGraph3dSoftware::SetBackBufferColorKey(DWORD dwColor)
{
	return 0;
}
int cGraph3dSoftware::EnableBackBufferColorKey(BOOL bEnable)
{
	return 0;
}
int cGraph3dSoftware::QueryGammaSupport( MD3DGAMMASUPPORT *gmGammaSupport )
{
	return 0;
}
int cGraph3dSoftware::CalibrateGamma( DDGAMMARAMP *lpRampData )
{
	return 0;
}
int cGraph3dSoftware::SetAdjustedGamma( float fRGamma, float fGGamma, float fBGamma )
{
	return 0;
}
int cGraph3dSoftware::GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma )
{
	return 0;
}
int cGraph3dSoftware::SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight )
{
	return 0;
}
int cGraph3dSoftware::GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight )
{
	return 0;
}
int cGraph3dSoftware::SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow )
{
	return 0;
}
int cGraph3dSoftware::GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow )
{
	return 0;
}
int cGraph3dSoftware::GetWindowHandle( HWND *hWnd )
{
	*hWnd=(HWND)xtGet_hWnd();
	return 0;
}
int cGraph3dSoftware::OutText(int x,int y,char *string,int r,int g,int b,int a)
{
	XGR_OutText(x,y,GetColor(r,g,b),string);
	return 1;
}
int cGraph3dSoftware::SetViewColor(int r,int g,int b,int a)
{
	return 1;
}
int cGraph3dSoftware::NullClipRect()
{
	return 1;
}
