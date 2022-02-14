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

int sVertexD3D::fmt	=	D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;

extern void xtRegisterSysFinitFnc(void (*fPtr)(void),int id);
extern void xtDeactivateSysFinitFnc(int id);
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
	
int cGraph3dDirect3D::Init(int xscr,int yscr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon)
{
	int XGR_flag=0;
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	assert(GraphMode&GRAPH3D_MODE_Z16);

	extern char* mch_mainINI;

	SwitchRenderScene=-1; 

#ifdef _DEBUG
//	d3dSetDebugLevel(5);
//	d3dSetLogFile("a.txt");
#endif

	HICON hic = NULL;
	char* wnd_title = "";
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

	void* win32_load_icon(void);
	hic = (HICON)win32_load_icon();

	error=d3dInit(xscr,yscr,ColorBit,DriverMode,(HINSTANCE)xtGet_hInstance(),hic,wnd_title);
	if(error!=MD3D_OK)
			ErrH.Abort ("No compatible 3D devices found.");
/*
		if(error==MD3DERR_NOCOMPATIBLEDEVICES) 
			ErrH.Abort ("No compatible 3D devices found.");
		else 
		{
			XBuffer buf; 
			buf < "Error initializing Direct3D. Code=" <= error < "."; 
			ErrH.Abort (buf.address());
		}
*/	
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
int cGraph3dDirect3D::ReInit(int xscr,int yscr,int mode,HINSTANCE hInst,TCHAR *szTitle,HICON hIcon)
{
	GraphMode=eModeGraph3d(mode);
	xScr=xscr; yScr=yscr;
	assert(GraphMode&GRAPH3D_MODE_Z16);

	extern char* mch_mainINI;

	SwitchRenderScene=-1; 

	HICON hic = NULL;
	char* wnd_title = "";
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

	void* win32_load_icon(void);
	hic = (HICON)win32_load_icon();

	error=d3dReInit(xscr,yscr,ColorBit,DriverMode,(HINSTANCE)xtGet_hInstance(),hic,wnd_title);
	if(error!=MD3D_OK)
		ErrH.Abort ("No compatible 3D devices found.");
/*
		if(error==MD3DERR_NOCOMPATIBLEDEVICES) 
			ErrH.Abort ("No compatible 3D devices found.");
		else 
		{
			XBuffer buf; 
			buf < "Error initializing Direct3D. Code=" <= error < "."; 
			ErrH.Abort (buf.address());
		}
*/
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
	d3dClose();
	delete this;
	return 0;
}
int cGraph3dDirect3D::IsActive()
{
	return d3dIsActive();
}

int cGraph3dDirect3D::BeginScene()
{
	if(SwitchRenderScene>0) return 1;
	int err=d3dBeginScene();
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
	d3dEndScene();
	return 0;
}
int cGraph3dDirect3D::NullClipRect()
{
// TODO: @caiiiycuk investigate this
#ifdef WTF
	extern LPDIRECT3DDEVICE7    g_pd3dDevice;       // The D3D device
	D3DVIEWPORT7 vp={xScrMin,yScrMin,xScrMax-xScrMin,yScrMax-yScrMin,0,1};
	g_pd3dDevice->SetViewport(&vp);
#endif
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
	assert((xmin<=xmax)&&(ymin<=ymax)&&(xmin>=0)&&(ymin>=0)&&(xmax<xScr)&&(ymax<yScr));
	RECT rc = { xScrMin=xmin,yScrMin=ymin,xScrMax=xmax,yScrMax=ymax};
//	if(SwitchRenderScene<=0) d3dSetClipRect(&rc);
	return 0;
}
int cGraph3dDirect3D::Fill(int r,int g,int b)
{
	if(SwitchRenderScene>0) EndScene();
	d3dClear((r<<16)|(g<<8)|(b<<0));
	return 0;
}
int cGraph3dDirect3D::Flush()
{
	if(SwitchRenderScene>0) EndScene();
	d3dFlip((bool)WaitVerticalBlank);
	if(NumberPolygon&&MinNumberPolygon>NumberPolygon) MinNumberPolygon=NumberPolygon;
	if(MaxNumberPolygon<NumberPolygon) MaxNumberPolygon=NumberPolygon;
#ifdef _TEST_DIRECT3D_
	fxx<<"NumberPolygon="<<NumberPolygon<<"	("<<MinNumberPolygon<<"-"<<MaxNumberPolygon<<")\n";
#endif //_TEST_DIRECT3D_
	NumberPolygon=0;
	return 0;
}

int cGraph3dDirect3D::PolygonFan(void *vertex,int NumberVertex,int VertexFormat)
{
	return 0;
}
int cGraph3dDirect3D::PolygonStrip(void *vertex,int NumberVertex,int VertexFormat)
{
	return 0;
}
int cGraph3dDirect3D::PolygonIndexed(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int VertexFormat)
{
	assert(polygon&&vertex);
	assert(VertexFormat==VERTEXFMT_FIX);
	assert((NumberVertex<65536)&&(NumberVertex>0)&&(NumberPolygon>0));
	if(!SwitchRenderScene) return 1;
	sVertexFix *vFix=(sVertexFix*)vertex;
	sPolygonFix *pFix=(sPolygonFix*)polygon,*pFixEnd=&pFix[NumberPolygon];
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	d3dTrianglesIndexed(FixFormatd3d,vFix,NumberVertex,(unsigned short*)pFix,3*NumberPolygon);
	cGraph3dDirect3D::NumberPolygon+=NumberPolygon;
	return 0;
}
int cGraph3dDirect3D::PolygonIndexed2(void *polygon,int NumberPolygon,void *vertex,int NumberVertex,int hTexture,int hLightMap,int VertexFormat)
{
	assert(polygon&&vertex);
	assert(VertexFormat==VERTEXFMT_FIX);
	assert((NumberVertex<65536)&&(NumberVertex>0)&&(NumberPolygon>0));
	if(!SwitchRenderScene) return 1;
	sVertexFix *vFix=(sVertexFix*)vertex;
	sPolygonFix *pFix=(sPolygonFix*)polygon,*pFixEnd=&pFix[NumberPolygon];
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	d3dTrianglesIndexed2(FixFormatd3d,vFix,NumberVertex,(unsigned short*)pFix,3*NumberPolygon,hTexture,hLightMap);
	cGraph3dDirect3D::NumberPolygon+=NumberPolygon;
	return 0;
}

int cGraph3dDirect3D::SetTexture(int hTexture)
{
	assert(hTexture);
	if(!SwitchRenderScene) return 1;
	return d3dSetTexture((int)hTexture)!=MD3D_OK;
}
int cGraph3dDirect3D::LockTexture(int hTexture,void **TextureBuffer,int *BytePerLine)
{
	assert(hTexture);
	return d3dLockTexture((DWORD)hTexture,TextureBuffer,(DWORD *)BytePerLine)!=MD3D_OK;
}
int cGraph3dDirect3D::UnlockTexture(int hTexture)
{
	assert(hTexture);
	return d3dUnlockTexture((int)hTexture)!=MD3D_OK;
}
int  cGraph3dDirect3D::CreateTexture(int x,int y,eTextureFormat TextureFormat)
{
	DWORD hTexture;
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
	if(d3dCreateTexture(x,y,TexFormat3d,&hTexture)==MD3D_OK) 
		return hTexture;
	return 0;
}
int cGraph3dDirect3D::DeleteTexture(int hTexture)
{
	assert(hTexture);
	return d3dDeleteTexture((int)hTexture)==MD3D_OK;
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
	if(!SwitchRenderScene) return 1;
	if(x1<xScrMin||x2<xScrMin||x1>xScrMax||x2>xScrMax||y1<yScrMin||y2<yScrMin||y1>yScrMax||y2>yScrMax) return 0;
	sVertexFix vFix[4];
//	sPolygonFix pFix[2]={ 0,1,2, 2,1,3 };
	sPolygonFix pFix[2]={ 0,2,1, 1,2,3 };
	float dy=(float)(x1-x2),dx=(float)-(y1-y2);
	float d=(float)sqrt(dx*dx+dy*dy);
	if(d>=1) d=0.5f/d; else return 0;
	dx*=d; dy*=d;
	vFix[0].xe=(float)x1+dx; vFix[0].ye=(float)y1+dy; vFix[0].z=0.001f; vFix[0].w=0.999f;
	vFix[0].diffuse()=c1;
	vFix[1].xe=(float)x2+dx; vFix[1].ye=(float)y2+dy; vFix[1].z=0.001f; vFix[1].w=0.999f;
	vFix[1].diffuse()=c2;
	vFix[2].xe=(float)x1-dx; vFix[2].ye=(float)y1-dy; vFix[2].z=0.001f; vFix[2].w=0.999f;
	vFix[2].diffuse()=c1;
	vFix[3].xe=(float)x2-dx; vFix[3].ye=(float)y2-dy; vFix[3].z=0.001f; vFix[3].w=0.999f;
	vFix[3].diffuse()=c2;
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE);
	d3dTrianglesIndexed(FixFormatd3d,vFix,4,(unsigned short*)pFix,3*2);
	return 0;
}
int cGraph3dDirect3D::DrawLine(int x1,int y1,int x2,int y2,int r,int g,int b,int a)
{
	if(!SwitchRenderScene) return 1;
	if(x1<xScrMin||x2<xScrMin||x1>xScrMax||x2>xScrMax||y1<yScrMin||y2<yScrMin||y1>yScrMax||y2>yScrMax) return 0;
	sVertexFix vFix[4];
//	sPolygonFix pFix[2]={ 0,1,2, 2,1,3 };
	sPolygonFix pFix[2]={ 0,2,1, 1,2,3 };
	float dy=(float)(x1-x2),dx=(float)-(y1-y2);
	float d=(float)sqrt(dx*dx+dy*dy);
	if(d>=1) d=0.5f/d; else return 0;
	dx*=d; dy*=d;
	vFix[0].xe=(float)x1+dx; vFix[0].ye=(float)y1+dy; vFix[0].z=0.001f; vFix[0].w=0.999f;
	vFix[0].dr()=r; vFix[0].dg()=g; vFix[0].db()=b; vFix[0].da()=a;
	vFix[1].xe=(float)x2+dx; vFix[1].ye=(float)y2+dy; vFix[1].z=0.001f; vFix[1].w=0.999f;
	vFix[1].dr()=r; vFix[1].dg()=g; vFix[1].db()=b; vFix[1].da()=a;
	vFix[2].xe=(float)x1-dx; vFix[2].ye=(float)y1-dy; vFix[2].z=0.001f; vFix[2].w=0.999f;
	vFix[2].dr()=r; vFix[2].dg()=g; vFix[2].db()=b; vFix[2].da()=a;
	vFix[3].xe=(float)x2-dx; vFix[3].ye=(float)y2-dy; vFix[3].z=0.001f; vFix[3].w=0.999f;
	vFix[3].dr()=r; vFix[3].dg()=g; vFix[3].db()=b; vFix[3].da()=a;
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE);
	d3dTrianglesIndexed(FixFormatd3d,vFix,4,(unsigned short*)pFix,3*2);
	return 0;
}
int cGraph3dDirect3D::DrawLine(float x1,float y1,float z1,float x2,float y2,float z2,int r,int g,int b,int a)
{
	if(!SwitchRenderScene) return 1;
	if(x1<xScrMin||x2<xScrMin||x1>xScrMax||x2>xScrMax||y1<yScrMin||y2<yScrMin||y1>yScrMax||y2>yScrMax) return 0;
	sVertexFix vFix[4];
	sPolygonFix pFix[2]={ 0,1,2, 2,1,3 };
	float dy=(float)(x1-x2),dx=(float)-(y1-y2);
	float d=(float)sqrt(dx*dx+dy*dy);
	if(d>=1) d=0.5f/d; else return 0;
	dx*=d; dy*=d;
	vFix[0].xe=(float)x1+dx; vFix[0].ye=(float)y1+dy; vFix[0].z=z1; vFix[0].w=0.999f;
	vFix[0].dr()=r; vFix[0].dg()=g; vFix[0].db()=b; vFix[0].da()=a;
	vFix[1].xe=(float)x2+dx; vFix[1].ye=(float)y2+dy; vFix[1].z=z2; vFix[1].w=0.999f;
	vFix[1].dr()=r; vFix[1].dg()=g; vFix[1].db()=b; vFix[1].da()=a;
	vFix[2].xe=(float)x1-dx; vFix[2].ye=(float)y1-dy; vFix[2].z=z1; vFix[2].w=0.999f;
	vFix[2].dr()=r; vFix[2].dg()=g; vFix[2].db()=b; vFix[2].da()=a;
	vFix[3].xe=(float)x2-dx; vFix[3].ye=(float)y2-dy; vFix[3].z=z2; vFix[3].w=0.999f;
	vFix[3].dr()=r; vFix[3].dg()=g; vFix[3].db()=b; vFix[3].da()=a;
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	SetMaterial(MAT_COLOR_MOD_DIFFUSE);
	d3dTrianglesIndexed(FixFormatd3d,vFix,4,(unsigned short*)pFix,3*2);
	return 0;
}
int cGraph3dDirect3D::DrawPixel(int x1,int y1,int r,int g,int b,int a)
{ 
	if(!SwitchRenderScene) return 1;
	if(x1<xScrMin||x1>xScrMax||y1<yScrMin||y1>yScrMax) return 0;
	struct sPointFD
	{
		float xe,ye,z,w;
		unsigned char drgba[4];
		inline unsigned char& dr()			{ return drgba[2]; }
		inline unsigned char& dg()			{ return drgba[1]; }
		inline unsigned char& db()			{ return drgba[0]; }
		inline unsigned char& da()			{ return drgba[3]; }
	} vPoint;
	vPoint.xe=(float)x1; vPoint.ye=(float)y1; vPoint.z=0.001f; vPoint.w=0.999f;
	vPoint.dr()=r; vPoint.dg()=g; vPoint.db()=b; vPoint.da()=a;
	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE);
	d3dPoints(D3DFVF_XYZRHW|D3DFVF_DIFFUSE,&vPoint,1);
	return 0; 
}
int cGraph3dDirect3D::SetMaterial(eMaterialMode material)
{
	if(MaterialMode==material) return 0;
	// \E2\EE\F1\F1\F2\E0\ED\EE\E2\EB\E5\ED\E8\E5 \EC\E0\F2\E5\F0\E8\E0\EB\EE\E2
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
	{
		d3dSetRenderState(D3DRENDERSTATE_ALPHATESTENABLE,FALSE);
		d3dSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
	}
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		d3dSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
	if(MaterialMode&MAT_COLOR_ADD_SPECULAR)
		d3dSetRenderState(D3DRENDERSTATE_SPECULARENABLE,FALSE);
	if(MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		d3dSetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA);
		d3dSetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);
	}
	// \F3\F1\F2\E0\ED\EE\E2\EA\E0 \EC\E0\F2\E5\F0\E8\E0\EB\EE\E2
	MaterialMode=material;
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MOD_DIFFUSE))
		d3dSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
	if(MaterialMode&(MAT_ALPHA_MOD_TEXTURE1|MAT_ALPHA_MASK_TEXTURE1))
	{
		d3dSetRenderState(D3DRENDERSTATE_ALPHATESTENABLE,TRUE);
		d3dSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
	}
	if(MaterialMode&MAT_COLOR_ADD_SPECULAR)
		d3dSetRenderState(D3DRENDERSTATE_SPECULARENABLE,TRUE);
	if(MaterialMode&MAT_COLOR_ADD_DIFFUSE)
	{
		d3dSetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_ONE);
		d3dSetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_ONE);
	}
	switch(MaterialMode&(MAT_COLOR_MOD_DIFFUSE|MAT_COLOR_MOD_TEXTURE1))
	{
		case MAT_COLOR_MOD_DIFFUSE:
			d3dSetTextureBlendMode(MD3DTB_DIFFUSE,MD3DTB_DIFFUSE);
			break;
		case MAT_COLOR_MOD_TEXTURE1:
			d3dSetTextureBlendMode(MD3DTB_TEXTURE1,MD3DTB_TEXTURE1);
			break;
		case MAT_COLOR_MOD_DIFFUSE_TEXTURE1:
			d3dSetTextureBlendMode(MD3DTB_TEXTURE1_MOD_DIFFUSE,MD3DTB_TEXTURE1_MOD_DIFFUSE);
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
	_D3DRENDERSTATETYPE type;
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
		    d3dSetTextureStageState(0,D3DTSS_ADDRESS,value);
			return d3dSetTextureStageState(1,D3DTSS_ADDRESS,value);
		case RENDERSTATE_TEXTUREPOINT:
		case RENDERSTATE_TEXTURELINEAR:
		default:
			assert(0);
	};
	return d3dSetRenderState(type,value)!=MD3D_OK;
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
	int error=d3dGetTextureFormatData(TexFormat3d,&d3dTexFmt);
	TexFmtData.Set(TexFmtData.TextureFormat,
		d3dTexFmt.dwRBitCount,d3dTexFmt.dwGBitCount,d3dTexFmt.dwBBitCount,d3dTexFmt.dwAlphaBitCount,
		d3dTexFmt.dwRBitShift,d3dTexFmt.dwGBitShift,d3dTexFmt.dwBBitShift,d3dTexFmt.dwAlphaBitShift);
	return error!=MD3D_OK;
}
////////////////////////// \ED\E0\F7\E0\EB\EE \EF\F0\EE\F7\E8\E5 \F4\F3\ED\EA\F6\E8\E8 //////////////////////////
int cGraph3dDirect3D::CreateSprite(DWORD dwWidth,DWORD dwHeight,DWORD dwFormat,DWORD dwFlags,DWORD* lpdwHandle )
{
	return d3dCreateSprite(dwWidth,dwHeight,dwFormat,dwFlags,(DWORD*)lpdwHandle);
}
int cGraph3dDirect3D::CreateChildSprite(DWORD dwParentHandle,DWORD dwLeft,DWORD dwTop, 
							    DWORD dwWidth,DWORD dwHeight,DWORD* lpdwHandle)
{
	return d3dCreateChildSprite(dwParentHandle,dwLeft,dwTop,dwWidth,dwHeight,(DWORD*)lpdwHandle);
}
int cGraph3dDirect3D::DeleteSprite(DWORD dwHandle)
{
	return d3dDeleteSprite(dwHandle);
}
int cGraph3dDirect3D::LockSprite(DWORD dwHandle,void **lplpSprite,DWORD *lplpPitch)
{
	return d3dLockSprite(dwHandle,lplpSprite,(DWORD*)lplpPitch);
}
int cGraph3dDirect3D::UnlockSprite(DWORD dwHandle)
{
	return UnlockSprite(dwHandle);
}
int cGraph3dDirect3D::SetSpriteMode(DWORD dwHandle,DWORD dwMode,DWORD dwValue)
{
	return d3dSetSpriteMode(dwHandle,dwMode,dwValue);
}
int cGraph3dDirect3D::DrawSprite(DWORD dwHandle,float dvX,float dvY,DWORD dwOrigin,
						float dvScaleX,float dvScaleY,float dvRotate )
{
	return d3dDrawSprite(dwHandle,dvX,dvY,dwOrigin,dvScaleX,dvScaleY,dvRotate);
}
int cGraph3dDirect3D::DrawSpriteZ(DWORD dwHandle,float dvX,float dvY,float dvZ, 
						 DWORD dwOrigin,float dvScaleX,float dvScaleY, 
						 float dvRotate )
{
	return d3dDrawSpriteZ(dwHandle,dvX,dvY,dvZ,dwOrigin,dvScaleX,dvScaleY,dvRotate);
}
int cGraph3dDirect3D::ScreenShot(VOID *lpBuffer,DWORD dwSize)
{
	return d3dScreenShot(lpBuffer,dwSize);
}
int cGraph3dDirect3D::CreateBackBuffer()
{
	return d3dCreateBackBuffer();
}
int cGraph3dDirect3D::ReleaseBackBuffer()
{
	return d3dReleaseBackBuffer();
}
int cGraph3dDirect3D::GetBackBufferFormat(DWORD *dwFormat)
{
	return d3dGetBackBufferFormat(dwFormat);
}
int cGraph3dDirect3D::LockBackBuffer(void **lplpSurface,DWORD *lpdwPitch)
{
	return d3dLockBackBuffer(lplpSurface,lpdwPitch);
}
int cGraph3dDirect3D::UnlockBackBuffer()
{
	return UnlockBackBuffer();
}
int cGraph3dDirect3D::FlushBackBuffer(RECT *lprcRect)
{
	if(SwitchRenderScene>0) EndScene();
	return d3dFlushBackBuffer(lprcRect);
}
int cGraph3dDirect3D::SetBackBufferColorKey(DWORD dwColor)
{
	return d3dSetBackBufferColorKey(dwColor);
}
int cGraph3dDirect3D::EnableBackBufferColorKey(BOOL bEnable)
{
	return d3dEnableBackBufferColorKey(bEnable);
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
	return d3dSetAdjustedGamma(fRGamma,fGGamma,fBGamma);
}
int cGraph3dDirect3D::GetAdjustedGamma( float *pfRGamma, float *pfGGamma, float *pfBGamma )
{
	return d3dGetAdjustedGamma(pfRGamma,pfGGamma,pfBGamma);
}
int cGraph3dDirect3D::SetGammaFxHighlight( float fRHilight, float fGHilight, float fBHilight )
{
	return d3dSetGammaFxHighlight(fRHilight,fGHilight,fBHilight);
}
int cGraph3dDirect3D::GetGammaFxHighlight( float *pfRHilight, float *pfGHilight, float *pfBHilight )
{
	return d3dGetGammaFxHighlight(pfRHilight,pfGHilight,pfBHilight );
}
int cGraph3dDirect3D::SetGammaFxShadow( float fRShadow, float fGShadow, float fBShadow )
{
	return d3dSetGammaFxShadow(fRShadow,fGShadow,fBShadow);
}
int cGraph3dDirect3D::GetGammaFxShadow( float *pfRShadow, float *pfGShadow, float *pfBShadow )
{
	return d3dGetGammaFxShadow(pfRShadow,pfGShadow,pfBShadow);
}
int cGraph3dDirect3D::GetWindowHandle( HWND *hWnd )
{
	return d3dGetWindowHandle(hWnd);
}
int cGraph3dDirect3D::SetViewColor(int r,int g,int b,int a)
{
	assert(SwitchRenderScene);
	sVertexFix vFix[4];
	sPolygonFix pFix[2]={ 0,2,1, 3,2,0 };
	vFix[0].xe=(float)0; vFix[0].ye=(float)0; vFix[0].z=0.0001f; vFix[0].w=0.999f;
	vFix[0].dr()=r; vFix[0].dg()=g; vFix[0].db()=b; vFix[0].da()=a;
	vFix[1].xe=(float)xScr; vFix[1].ye=(float)0; vFix[1].z=0.0001f; vFix[1].w=0.999f;
	vFix[1].dr()=r; vFix[1].dg()=g; vFix[1].db()=b; vFix[1].da()=a;
	vFix[2].xe=(float)xScr; vFix[2].ye=(float)yScr; vFix[2].z=0.0001f; vFix[2].w=0.999f;
	vFix[2].dr()=r; vFix[2].dg()=g; vFix[2].db()=b; vFix[2].da()=a;
	vFix[3].xe=(float)0; vFix[3].ye=(float)yScr; vFix[3].z=0.0001f; vFix[3].w=0.999f;
	vFix[3].dr()=r; vFix[3].dg()=g; vFix[3].db()=b; vFix[3].da()=a;
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE);
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,	FALSE ); 
	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,	D3DCULL_NONE ); 
	d3dTrianglesIndexed(FixFormatd3d,vFix,4,(unsigned short*)pFix,3*2);
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,	TRUE ); 
	return 0;
}
int cGraph3dDirect3D::DrawRectangle(int x,int y,int dx,int dy,int r,int g,int b,int a,int flag)
{ 
	sVertexFix vFix[4];
	sPolygonFix pFix[2]={ 0,1,2, 2,3,0 };
	vFix[0].xe=(float)x; vFix[0].ye=(float)y; vFix[0].z=0.0001f; vFix[0].w=0.999f;
	vFix[0].dr()=r; vFix[0].dg()=g; vFix[0].db()=b; vFix[0].da()=a;
	vFix[1].xe=(float)(x+dx); vFix[1].ye=(float)y; vFix[1].z=0.0001f; vFix[1].w=0.999f;
	vFix[1].dr()=r; vFix[1].dg()=g; vFix[1].db()=b; vFix[1].da()=a;
	vFix[2].xe=(float)(x+dx); vFix[2].ye=(float)(y+dy); vFix[2].z=0.0001f; vFix[2].w=0.999f;
	vFix[2].dr()=r; vFix[2].dg()=g; vFix[2].db()=b; vFix[2].da()=a;
	vFix[3].xe=(float)x; vFix[3].ye=(float)(y+dy); vFix[3].z=0.0001f; vFix[3].w=0.999f;
	vFix[3].dr()=r; vFix[3].dg()=g; vFix[3].db()=b; vFix[3].da()=a;
	int FixFormatd3d=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_SPECULAR|D3DFVF_TEX1;
	SetMaterial(MAT_NULL);
	SetMaterial(MAT_COLOR_MOD_DIFFUSE_ALPHA_MOD_DIFFUSE);
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,	FALSE ); 
	d3dTrianglesIndexed(FixFormatd3d,vFix,4,(unsigned short*)pFix,3*2);
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,	TRUE ); 
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
	d3dSetProjectionMatrixToIdentity();

	d3dSetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);  
	d3dSetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); // \F5\E8\ED\F2 D3DTSS_COLORARG1==D3DTA_TEXTURE, \E8\ED\E0\F7\E5 \EC\EE\E6\E5\F2 \ED\E5 \F0\E0\E1\EE\F2\E0\F2\FC
	d3dSetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	d3dSetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	d3dSetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); // \F5\E8\ED\F2 D3DTSS_COLORARG1==D3DTA_TEXTURE, \E8\ED\E0\F7\E5 \EC\EE\E6\E5\F2 \ED\E5 \F0\E0\E1\EE\F2\E0\F2\FC
	d3dSetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	d3dSetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	d3dSetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1);  
	d3dSetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE ); // \F5\E8\ED\F2 D3DTSS_COLORARG1==D3DTA_TEXTURE, \E8\ED\E0\F7\E5 \EC\EE\E6\E5\F2 \ED\E5 \F0\E0\E1\EE\F2\E0\F2\FC
	d3dSetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	d3dSetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	d3dSetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); // \F5\E8\ED\F2 D3DTSS_COLORARG1==D3DTA_TEXTURE, \E8\ED\E0\F7\E5 \EC\EE\E6\E5\F2 \ED\E5 \F0\E0\E1\EE\F2\E0\F2\FC
	d3dSetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	d3dSetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	d3dSetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	d3dSetTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

    d3dSetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	d3dSetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFN_LINEAR );
	d3dSetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE);
    d3dSetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	d3dSetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFN_LINEAR );
	d3dSetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTFP_NONE);

	d3dSetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE,TRUE);
	d3dSetRenderState( D3DRENDERSTATE_ANTIALIAS,D3DANTIALIAS_NONE);
	d3dSetRenderState( D3DRENDERSTATE_ZENABLE,1);
	d3dSetRenderState( D3DRENDERSTATE_FILLMODE,D3DFILL_SOLID);
	d3dSetRenderState( D3DRENDERSTATE_SHADEMODE,D3DSHADE_GOURAUD);
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,1);
	d3dSetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_LASTPIXEL,TRUE);
	d3dSetRenderState( D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA);
	d3dSetRenderState( D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);
//	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_CW);
	d3dSetRenderState( D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	d3dSetRenderState( D3DRENDERSTATE_ZFUNC,D3DCMP_LESSEQUAL);
	d3dSetRenderState( D3DRENDERSTATE_ALPHAREF,1);	// 0
	d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATEREQUAL); //D3DCMP_ALWAYS
	d3dSetRenderState( D3DRENDERSTATE_DITHERENABLE,TRUE);
	d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_FOGENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_SPECULARENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_FOGCOLOR,0x00000000);
	d3dSetRenderState( D3DRENDERSTATE_FOGTABLEMODE,D3DFOG_NONE);
	d3dSetRenderState( D3DRENDERSTATE_FOGTABLESTART,0);
	d3dSetRenderState( D3DRENDERSTATE_FOGTABLEEND,0);
	d3dSetRenderState( D3DRENDERSTATE_FOGTABLEDENSITY,0);
	d3dSetRenderState( D3DRENDERSTATE_FOGSTART,0);
	d3dSetRenderState( D3DRENDERSTATE_FOGEND,0);
	d3dSetRenderState( D3DRENDERSTATE_FOGDENSITY,0);
	d3dSetRenderState( D3DRENDERSTATE_COLORKEYENABLE,0);
	d3dSetRenderState( D3DRENDERSTATE_ZBIAS,0);
	d3dSetRenderState( D3DRENDERSTATE_RANGEFOGENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_TEXTUREFACTOR,0x80FFFFFF);

	d3dSetRenderState( D3DRENDERSTATE_CLIPPING,1);
	d3dSetRenderState( D3DRENDERSTATE_LIGHTING,0);
	
	d3dSetRenderState( D3DRENDERSTATE_AMBIENT,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_COLORVERTEX,1);
	d3dSetRenderState( D3DRENDERSTATE_COLORKEYBLENDENABLE,FALSE);
	d3dSetRenderState( D3DRENDERSTATE_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	d3dSetRenderState( D3DRENDERSTATE_SPECULARMATERIALSOURCE,D3DMCS_COLOR2);
	d3dSetRenderState( D3DRENDERSTATE_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
	d3dSetRenderState( D3DRENDERSTATE_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);
	d3dSetRenderState( D3DRENDERSTATE_VERTEXBLEND,D3DVBLEND_DISABLE);
	d3dSetRenderState( D3DRENDERSTATE_CLIPPLANEENABLE,FALSE);
/*
	d3dSetRenderState( D3DRENDERSTATE_SPECULARENABLE, FALSE );
	d3dSetRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );
	d3dSetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE );
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE,	TRUE ); 
	d3dSetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_TRUE );
	d3dSetRenderState( D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL );
	d3dSetRenderState( D3DRENDERSTATE_ZBIAS, 0 );
	d3dSetRenderState( D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID ); 
//	d3dSetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CW ); 
	d3dSetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE ); 
	d3dSetRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD );

	d3dSetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,FALSE );
//	d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_ALWAYS );
//	d3dSetRenderState( D3DRENDERSTATE_ALPHAREF,0xFF );
	d3dSetRenderState( D3DRENDERSTATE_ALPHAFUNC,D3DCMP_GREATEREQUAL );
	d3dSetRenderState( D3DRENDERSTATE_ALPHAREF,1 );

	d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,FALSE );
	d3dSetRenderState( D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA );
	d3dSetRenderState( D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA );

    d3dSetTextureStageState(0,D3DTSS_ADDRESS,D3DTADDRESS_WRAP);
    d3dSetTextureStageState(1,D3DTSS_ADDRESS,D3DTADDRESS_WRAP);
*/
}
