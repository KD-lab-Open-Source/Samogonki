#include "PolyMgr.h"
// TODO: @caiiiycuk investigate this
#ifdef SOFTWARE_RENDER
#include "Software16.h"
#endif
#include "Object3d.h"
#include "RenderDevice.h"

//#define SIGN(a)						((a)<0? -1: 1)
#define CLIP_POLYGON_ZMIN			(1<<5)
#define CLIP_POLYGON_XMIN			(1<<6)
#define CLIP_POLYGON_YMIN			(1<<7)
#define CLIP_POLYGON_XMAX			(1<<8)
#define CLIP_POLYGON_YMAX			(1<<9)

#define NUMBER_PLANE_CLIP			5
#define NUMBER_POINT_FIX			10000
#define NUMBER_POINT_ATTRIBUTE		10000
#define NUMBER_POLYGON_FIX			10000

extern void SaveMemoryMessage(const char *message,int i=0);
int					gb_CurrentTexture=0;

cPolyDispatcher::cPolyDispatcher()
{
	SaveMemoryMessage("Begin cPolyDispatcher::cPolyDispatcher()");
	memset(this,0,sizeof(cPolyDispatcher));
	PlaneClip3d.New(NUMBER_PLANE_CLIP); PlaneClip3d.length()=NUMBER_PLANE_CLIP;
	PointFix.New(NUMBER_POINT_FIX);
#ifdef _ONLY_DIRECT3D_
	VertexD3D.New(NUMBER_POINT_FIX);
#endif //_ONLY_DIRECT3D_
	PolygonFix.New(NUMBER_POLYGON_FIX);
	PointAttribute.New(NUMBER_POINT_ATTRIBUTE);
	CurrentCamera=0;
	CurrentConvertorObjectToScreen=new cConvertor;
	SaveMemoryMessage("End cPolyDispatcher::cPolyDispatcher()");
	AlphaForSprite=255;
}
cPolyDispatcher::~cPolyDispatcher()
{
	PlaneClip3d.Delete();
	PointFix.Delete();
	PolygonFix.Delete();
	PointAttribute.Delete();
	VertexD3D.Delete();
	if(CurrentConvertorObjectToScreen) delete CurrentConvertorObjectToScreen; 
	CurrentConvertorObjectToScreen=0;
}
void cPolyDispatcher::BeginScene(cUnknownClass *UCameraList)
{
	cInterfaceGraph3d *IGraph3d=GetRenderDevice(0)->GetIGraph3d();
	IGraph3d->BeginScene(); 
	CurrentCamera=0;
}
void cPolyDispatcher::EndScene(cUnknownClass *UCameraList)
{
	cInterfaceGraph3d *IGraph3d=GetRenderDevice(0)->GetIGraph3d();
	IGraph3d->EndScene(); 
	CurrentCamera=0;
}
void cPolyDispatcher::InitFix(int attribute,int NumberPoint)
{ 
	Attribute=attribute; 
	PointFix.Clear(); 
	PolygonFix.Clear(); 
	PointAttribute.Clear(); 
	PointAttribute.length()=PointFix.length()=NumberPoint;
}
void cPolyDispatcher::SetClippingPlane(cCamera *Camera)
{
	assert(PlaneClip3d.length()>=5&&Camera->GetNumberPlaneClip3d());
	sRectangle4f &Clip=Camera->GetClipping();
	xClipMin=(Clip.xmin()-1e-5f)*FLOAT_CONST_PREC; xClipMax=(Clip.xmax()+1e-5f)*FLOAT_CONST_PREC; 
	yClipMin=(Clip.ymin()-1e-5f)*FLOAT_CONST_PREC; yClipMax=(Clip.ymax()+1e-5f)*FLOAT_CONST_PREC;
	zClipMin=(Camera->GetZPlane().x-1e-5f)*FLOAT_CONST_PREC; zClipMax=(Camera->GetZPlane().y+1e-5f)*FLOAT_CONST_PREC;
	PlaneClip3d[0]=Camera->GetPlaneClip3d(0);	/*zClipMin,CLIP_ZMIN*/
	PlaneClip3d[1]=Camera->GetPlaneClip3d(1);	/*xClipMin,CLIP_XMIN*/
	PlaneClip3d[2]=Camera->GetPlaneClip3d(2);	/*xClipMax,CLIP_XMAX*/
	PlaneClip3d[4]=Camera->GetPlaneClip3d(4);	/*yClipMin,CLIP_YMIN*/
	PlaneClip3d[3]=Camera->GetPlaneClip3d(3);	/*yClipMax,CLIP_YMAX*/
	Vect2f &ScrSize=Camera->GetViewPort()->GetSize();
	Center=Camera->GetCenter()*ScrSize,Focus=Camera->GetFocus()*ScrSize.x/FLOAT_CONST_PREC;	
}
int cPolyDispatcher::AddPointFix(sPlane4f &PlaneClip3d,int i1,int i2)
{
	sVertexFix &p1=PointFix[i1],&p2=PointFix[i2];
	sPointAttribute &pa1=PointAttribute[i1],&pa2=PointAttribute[i2];
	float t=PlaneClip3d.GetCross(pa1.pv,pa2.pv);
	sVertexFix &p=NextPointFix();
	int t16=round(t*65535);
	if(cPolyDispatcher::Attribute&(RENDER_COLOR_MOD_DIFFUSE|RENDER_ALPHA_MOD_DIFFUSE))
	{
		p.dr()=p1.dr()+(((p2.dr()-p1.dr())*t16)>>16);
		p.dg()=p1.dg()+(((p2.dg()-p1.dg())*t16)>>16);
		p.db()=p1.db()+(((p2.db()-p1.db())*t16)>>16);
		p.da()=p1.da()+(((p2.da()-p1.da())*t16)>>16);
	}
	else
		p.dr()=p.dg()=p.db()=p.da()=255;
	if(cPolyDispatcher::Attribute&RENDER_COLOR_ADD_SPECULAR)
	{
		p.sr()=p1.sr()+(((p2.sr()-p1.sr())*t16)>>16);
		p.sg()=p1.sg()+(((p2.sg()-p1.sg())*t16)>>16);
		p.sb()=p1.sb()+(((p2.sb()-p1.sb())*t16)>>16);
		p.sa()=p1.sa()+(((p2.sa()-p1.sa())*t16)>>16);
	}
	if(cPolyDispatcher::Attribute&(RENDER_COLOR_MOD_TEXTURE1|RENDER_COLOR_ADD_TEXTURE1|RENDER_ALPHA_MOD_TEXTURE1|RENDER_ALPHA_MASK_TEXTURE1))
	{
		p.u1()=p1.u1()+(p2.u1()-p1.u1())*t;
		p.v1()=p1.v1()+(p2.v1()-p1.v1())*t;
	}
//	p.u2()=p1.u2()+(p2.u2()-p1.u2())*t;
//	p.v2()=p1.v2()+(p2.v2()-p1.v2())*t;
//	p.u3()=p1.u3()+(p2.u3()-p1.u3())*t;
//	p.v3()=p1.v3()+(p2.v3()-p1.v3())*t;
	sPointAttribute &pa=NextPointAttribute();
	pa.Set(pa1,pa2,t);
	assert(pa.pv.z!=0);
	p.z=FLOAT_CONST_PREC/pa.pv.z;
	p.xe=pa.pv.x*p.z; 
	p.ye=pa.pv.y*p.z;
	pa.clip=GET_CLIP(p.xe,p.ye,pa.pv.z);
	return PointAttribute.length()-1;
}
