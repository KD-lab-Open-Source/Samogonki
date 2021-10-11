#include "Unknown.h"
#include "Camera.h"
#include "RenderDevice.h"

// глобальные объявления мира
Vect2f GlobalWorldSize(2048.f,2048.f);
Vect2f GlobalWorldScale(1.f,1.f);
float  GlobalWorldRadius=1000.f;

cCamera::cCamera() : cUnknownClass(KIND_CAMERA)
{ 
	Matrix.NewMatrix();
	PlaneClip3d.New(5);
	PlaneClip3d.length()=5;
	Pos.set(0.f,0.f,0.f);
	PosOfs.set(0.f,0.f,128.f);
	Angle.set(0.f,0.f,0.f);
	Attribute=ATTRIBUTE_CAMERA_PERSPECTIVE;
	SetFrustum(sRectangle4f(0.01f,0.01f,0.99f,0.99f),Vect2f(0.5f,0.5f),Vect2f(1.f,1.f),Vect2f(10.f,10000.f),Vect2f(0.2f,0.90f));
	CalcMatrix();
}
cCamera::~cCamera()
{ 
	UNKNOWN_DESTRUCTOR; 
}
void cCamera::AttachViewPort(const cUnknownClass *URenderDevice)
{
	if((RenderDevice=(cRenderDevice*)URenderDevice)==0) return;	
}
void cCamera::DetachViewPort(const cUnknownClass *URenderDevice)
{
	if(((cRenderDevice*)URenderDevice)==0) return;	
	assert(RenderDevice->GetKind(KIND_RENDERDEVICE)&&(RenderDevice==URenderDevice));
	RenderDevice=0;
}
void cCamera::CalcPlaneClip3d() 
{ 
	float xClipMin=GetClipping().xmin(),xClipMax=GetClipping().xmax(), 
		yClipMin=GetClipping().ymin(),yClipMax=GetClipping().ymax(),
		zClipMin=GetZPlane().x,zClipMax=GetZPlane().y;
	Vect3f vC(0,0,0);
	Vect3f vLU(xClipMin,-yClipMax,1);
	Vect3f vRU(xClipMax,-yClipMax,1);
	Vect3f vRD(xClipMax,-yClipMin,1);
	Vect3f vLD(xClipMin,-yClipMin,1);
	assert(PlaneClip3d.length()>=5);
	PlaneClip3d[0].Set(Vect3f(0,0,zClipMin),Vect3f(1,0,zClipMin),Vect3f(0,1,zClipMin));	/*zClipMin,CLIP_ZMIN*/
	PlaneClip3d[1].Set(vC,vLU,vLD);	/*xClipMin,CLIP_XMIN*/
	PlaneClip3d[2].Set(vC,vRD,vRU);	/*xClipMax,CLIP_XMAX*/
	PlaneClip3d[4].Set(vC,vRU,vLU);	/*yClipMin,CLIP_YMIN*/
	PlaneClip3d[3].Set(vC,vLD,vRD);	/*yClipMax,CLIP_YMAX*/
}
void cCamera::SetPos(const Vect3f &pos)
{
	GetPos().set(Cycl(pos.x,GlobalWorldSize.x),Cycl(pos.y,GlobalWorldSize.y),pos.z);
}
void cCamera::CalcMatrix()
{
	GetPos().x=Cycl(GetPos().x,GlobalWorldSize.x); GetPos().y=Cycl(GetPos().y,GlobalWorldSize.y);
	GetMatrix() = MatXf(Mat3f(G2R(GetAngle().x), X_AXIS)*
					Mat3f(G2R(GetAngle().y), Y_AXIS)*
					Mat3f(G2R(GetAngle().z), Z_AXIS), Vect3f(-GetOfs().x,-GetOfs().y,-GetPos().z+GetOfs().z))*
				MatXf(Mat3f::ID, Vect3f(-GetPos().x,-GetPos().y,-GetOfs().z));
}
eConstVisible cCamera::TestVisible(const MatXf &matrix,const Vect3f &size)
{
	int test=0;
	Vect3f pv,pe, mt(MATRIX_SIGN_X(matrix.trans().x),MATRIX_SIGN_Y(matrix.trans().y),MATRIX_SIGN_Z(matrix.trans().z)),
		mx(MATRIX_SIGN_X(size.x*matrix.rot()[0][0]),MATRIX_SIGN_Y(size.x*matrix.rot()[1][0]),MATRIX_SIGN_Z(size.x*matrix.rot()[2][0])),
		my(MATRIX_SIGN_X(size.y*matrix.rot()[0][1]),MATRIX_SIGN_Y(size.y*matrix.rot()[1][1]),MATRIX_SIGN_Z(size.y*matrix.rot()[2][1])),
		mz(MATRIX_SIGN_X(size.z*matrix.rot()[0][2]),MATRIX_SIGN_Y(size.z*matrix.rot()[1][2]),MATRIX_SIGN_Z(size.z*matrix.rot()[2][2]));
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE))
	{
		pv.z=mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=mt.x);	pe.y=pe.z*(pv.y=mt.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=mx.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(mx.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=my.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(my.x+mt.x)); pe.y=pe.z*(pv.y=(my.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=mx.z+my.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(mx.x+my.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+my.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
	}
	else
	{
		pe.z=1.f/GetPos().z;
		if(mt.z<GetZPlane().x) return CONST_VISIBLE_NULL;
		pe.x=pe.z*(pv.x=mt.x);	pe.y=pe.z*(pv.y=mt.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(mx.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(my.x+mt.x)); pe.y=pe.z*(pv.y=(my.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(mx.x+my.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+my.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
	}
	if(test&((8<<XMINSHIFT)|(8<<XMAXSHIFT)|(8<<YMINSHIFT)|(8<<YMAXSHIFT)|(8<<ZMINSHIFT)|(8<<ZMAXSHIFT)))
		return CONST_VISIBLE_NULL;
	if(test==0) return CONST_VISIBLE_FRUSTUM;
	int visible=CONST_VISIBLE_FRUSTUM;
	if(test&(7<<XMINSHIFT)) visible^=CONST_VISIBLE_XMIN;
	if(test&(7<<XMAXSHIFT)) visible^=CONST_VISIBLE_XMAX;
	if(test&(7<<YMINSHIFT)) visible^=CONST_VISIBLE_YMIN;
	if(test&(7<<YMAXSHIFT)) visible^=CONST_VISIBLE_YMAX;
	if(test&(7<<ZMINSHIFT)) visible^=CONST_VISIBLE_ZMIN;
	if(test&(7<<ZMAXSHIFT)) visible^=CONST_VISIBLE_ZMAX;
	return (eConstVisible)visible;
}
eConstVisible cCamera::TestVisible(const MatXf &matrix,float radius)
{
	Vect3f pv,pe;
	ConvertPoint(matrix,Vect3f(0,0,0),pv,pe);
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)) radius*=1.f/pv.z;
	else radius*=1.f/GetPos().z;
	if(pv.z<GetZPlane().x) return CONST_VISIBLE_NULL;
	if((pe.x-radius)>Clip.xmax()) return CONST_VISIBLE_NULL;
	if((pe.x+radius)<Clip.xmin()) return CONST_VISIBLE_NULL;
	if((pe.y-radius)>Clip.ymax()) return CONST_VISIBLE_NULL;
	if((pe.y+radius)<Clip.ymin()) return CONST_VISIBLE_NULL;
	int visible=CONST_VISIBLE_FRUSTUM;
	if((pe.x-radius)<Clip.xmin()) visible^=CONST_VISIBLE_XMIN;
	if((pe.x+radius)>Clip.xmax()) visible^=CONST_VISIBLE_XMAX;
	if((pe.y-radius)<Clip.ymin()) visible^=CONST_VISIBLE_YMIN;
	if((pe.y+radius)>Clip.ymax()) visible^=CONST_VISIBLE_YMAX;
	return (eConstVisible)visible;
}
eConstVisible cCamera::TestVisible(const MatXf &matrix,const Vect3f &min,const Vect3f &max)
{ // для BoundBox с границами min && max
	int test=0;
	Vect3f pv,pe;
	Vect3f size=max-min,
		mx(MATRIX_SIGN_X(size.x*matrix.rot()[0][0]),MATRIX_SIGN_Y(size.x*matrix.rot()[1][0]),MATRIX_SIGN_Z(size.x*matrix.rot()[2][0])),
		my(MATRIX_SIGN_X(size.y*matrix.rot()[0][1]),MATRIX_SIGN_Y(size.y*matrix.rot()[1][1]),MATRIX_SIGN_Z(size.y*matrix.rot()[2][1])),
		mz(MATRIX_SIGN_X(size.z*matrix.rot()[0][2]),MATRIX_SIGN_Y(size.z*matrix.rot()[1][2]),MATRIX_SIGN_Z(size.z*matrix.rot()[2][2]));
	Vect3f mt(MATRIX_SIGN_X(matrix.rot()[0][0]*min.x+matrix.rot()[0][1]*min.y+matrix.rot()[0][2]*min.z+matrix.trans().x),
		MATRIX_SIGN_Y(matrix.rot()[1][0]*min.x+matrix.rot()[1][1]*min.y+matrix.rot()[1][2]*min.z+matrix.trans().y),
		MATRIX_SIGN_Z(matrix.rot()[2][0]*min.x+matrix.rot()[2][1]*min.y+matrix.rot()[2][2]*min.z+matrix.trans().z));
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE))
	{
		pv.z=mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=mt.x);	pe.y=pe.z*(pv.y=mt.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=mx.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(mx.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=my.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(my.x+mt.x)); pe.y=pe.z*(pv.y=(my.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pv.z=mx.z+my.z+mt.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x=(mx.x+my.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+my.y+mt.y));
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pv.z+=mz.z; if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=1.f; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pv.z<GetZPlane().x) { test+=ZMINTEST; if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
	}
	else
	{
		pe.z=1.f/GetPos().z;
		if(mt.z<GetZPlane().x) return CONST_VISIBLE_NULL;
		pe.x=pe.z*(pv.x=mt.x);	pe.y=pe.z*(pv.y=mt.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(mx.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(my.x+mt.x)); pe.y=pe.z*(pv.y=(my.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST; 
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 

		pe.x=pe.z*(pv.x=(mx.x+my.x+mt.x)); pe.y=pe.z*(pv.y=(mx.y+my.y+mt.y));
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
		pe.x=pe.z*(pv.x+=mz.x);	pe.y=pe.z*(pv.y+=mz.y);
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST; 
	}
	if(test&((8<<XMINSHIFT)|(8<<XMAXSHIFT)|(8<<YMINSHIFT)|(8<<YMAXSHIFT)|(8<<ZMINSHIFT)|(8<<ZMAXSHIFT)))
		return CONST_VISIBLE_NULL;
	if(test==0) return CONST_VISIBLE_FRUSTUM;
	int visible=CONST_VISIBLE_FRUSTUM;
	if(test&(7<<XMINSHIFT)) visible^=CONST_VISIBLE_XMIN;
	if(test&(7<<XMAXSHIFT)) visible^=CONST_VISIBLE_XMAX;
	if(test&(7<<YMINSHIFT)) visible^=CONST_VISIBLE_YMIN;
	if(test&(7<<YMAXSHIFT)) visible^=CONST_VISIBLE_YMAX;
	if(test&(7<<ZMINSHIFT)) visible^=CONST_VISIBLE_ZMIN;
	if(test&(7<<ZMAXSHIFT)) visible^=CONST_VISIBLE_ZMAX;
	return (eConstVisible)visible;
}

Vect3f cCamera::Screen2World(int x_scr, int y_scr)
{
#ifdef _MECHOSOMA_
	int CastRayForS2G(const Vect3f& origin, Vect3f& offset, cCamera* camera); // 1 - ray intersects nearest triangle and offset_returned < offset
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE))
	{
		Vect3f direct(x_scr,y_scr,-GetFocus().x*GetViewPort()->GetSize().x);
		GetMatrix().invXformVect(direct);
		direct.normalize(2000);
		Vect3f origin;
		GetMatrix().invXformVect(GetMatrix().trans(),origin);
		origin.negate();
		CastRayForS2G(origin, direct, this);
		return origin + direct;
	}
	else
	{ // сканируем параллельными лучами
		Vect3f direct(0,0,2000);
		GetMatrix().invXformVect(direct);
		float k = GetPos().z/(GetFocus().x*GetViewPort()->GetSize().x);
		Vect3f origin(GetPos().x + x_scr*k, GetPos().y + y_scr*k, GetPos().z);
		CastRayForS2G(origin, direct, this);
		return origin + direct;
	}
#else
	return Vect3f::ZERO;
#endif
}

