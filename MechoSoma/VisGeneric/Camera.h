#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "UnkLibrary.h"
#include "VisGenericDefine.h"
#include "Math3d.h"
#include "memory.h"
#include "UMATH.H"
#include "float.h"

#ifdef __APPLE__
#define __forceinline inline
#endif

#define FLOAT_CONST_PREC					1.f

class cRenderDevice;

class cCamera : public cUnknownClass
{
private: // первичные значения
	cMatrix				Matrix;
	Vect3f				Pos;						// местоположение камеры
	Vect3f				PosOfs;						// смещение центра при наклоне
	Vect3f				Angle;						// углы Эйлера определяющие матрицу, в градусах
	eAttributeCamera	Attribute;
	sRectangle4f		Clip;						// Clip.left,Clip.right,Clip.top,Clip.bottom - 0..1 - размеры видимой области
	Vect2f				Center;						// Center.x,Center.y - 0..1 - центр, 
	Vect2f				Focus;						// Focus.z - фокус, порядка единицы
	Vect2f				zPlane;						// расстояние до ближайшей и дальней плоскостей обрезания, используется для zBuffer'а
	Vect2f				zBufferScale;				// zBufferScale.x=zNear,zBufferScale.y=zFar - 0..1 коэффициенты масштабирования zBuffer'а
	cRenderDevice		*RenderDevice;				// устройство вывода
	cBaseArray <sPlane4f>PlaneClip3d;				// плоскости отсечения
public:
	cCamera();
	~cCamera();
	inline void SetAttribute(int attribute)								{ Attribute=eAttributeCamera(Attribute|attribute); }
	inline int GetAttribute(int attribute)								{ return Attribute&attribute; }
	inline void ClearAttribute(int attribute)							{ Attribute=eAttributeCamera(Attribute&(~attribute)); }
	// функции позиционирования камеры и для изменения ее матрицы
	inline void Translate(const Vect3f &dPos)							{ Pos+=dPos; CalcMatrix(); }
	inline void Rotate(const Vect3f &dAngle)							{ Angle+=dAngle; CalcMatrix(); }
	inline void dSetPosition(const Vect3f &dPos,const Vect3f &dAngle)	{ Pos+=dPos; Angle+=dAngle; CalcMatrix(); }
	inline void dSetPosition(const MatXf &matrix)						{ Matrix*=matrix; }
	inline void SetPosition(const Vect3f &pos,const Vect3f &angle)	{ Pos=pos; Angle=angle; CalcMatrix(); }
	inline void SetPosition(const MatXf &matrix)						{ Matrix=matrix; }
	inline void GetPosition(Vect3f &pos,Vect3f &angle)				{ pos=Pos; angle=Angle; }
	inline void GetPosition(MatXf &matrix)								{ matrix=Matrix; }
	inline void SetOfs(const Vect3f &ofs)								{ PosOfs=ofs; CalcMatrix(); }
	inline cMatrix& GetMatrix()											{ return Matrix; }
	inline Vect3f& GetAngle()											{ return Angle; }
	inline Vect3f& GetOfs()											{ return PosOfs; }
	inline Vect3f& GetPos()											{ return Pos; }
	void SetPos(const Vect3f &pos);
	// функции для работы с видимой областью
	void AttachViewPort(const cUnknownClass *URenderDevice);
	void DetachViewPort(const cUnknownClass *URenderDevice=0);
	cRenderDevice* GetViewPort()										{ return RenderDevice; }
	// функции для работы с пирамидой видимости
	inline void SetFrustum(const sRectangle4f &clip,const Vect2f &center,const Vect2f &focus,const Vect2f &zplane,const Vect2f &zscale) { Clip=clip; Center=center; Focus=focus; zPlane=zplane; zBufferScale=zscale; CalcPlaneClip3d(); }
	inline void GetFrustum(sRectangle4f &clip,Vect2f &center,Vect2f &focus,Vect2f &zplane,Vect2f &zscale)	{ clip=Clip; center=Center; focus=Focus; zscale=zBufferScale; zplane=zPlane;}
	inline sRectangle4f& GetClipping()									{ return Clip; }
	inline Vect2f& GetZBufferScale()									{ return zBufferScale; }
	inline Vect2f& GetCenter()											{ return Center; }
	inline Vect2f& GetZPlane()											{ return zPlane; }
	inline Vect2f& GetFocus()											{ return Focus; }
	inline sPlane4f& GetPlaneClip3d(int number)							{ return PlaneClip3d[number]; }
	inline int GetNumberPlaneClip3d()									{ return PlaneClip3d.length(); }
	// функции конвертации, матрица и координаты должны быть зациклены и натянуты на шар
	inline void BuildMatrix(MatXf &matrix,const Vect3f &pos);		// создает матрицу matrix, на основе cCamera::Matrix && pos, зацикливает и натягивает на шар
	inline void LoadMatrix(Vect3f &pos);	// устанавливается pos - зацикливается и натягивается на шар
	inline void LoadMatrix(MatXf &matrix);
	inline void ConvertPoint(const MatXf &matrix,const Vect3f &pw,Vect3f &pv,Vect3f &pe);
	inline void ConvertPoint(const Vect3f &pos,const Vect3f &pw,Vect3f &pv,Vect3f &pe);		// pos - смещение конвертируемой точки от центра камеры, 
	// функции теста видимости, координаты должны быть зациклены и натянуты на шар (полная матрица визуализации)
	eConstVisible TestVisible(const MatXf &matrix,const Vect3f &min,const Vect3f &max);	// для BoundBox с границами min && max
	eConstVisible TestVisible(const MatXf &matrix,const Vect3f &size);		// для параллелепипеда размера size, левый нижний угол лежит в matrix
	eConstVisible TestVisible(const MatXf &matrix,float radius);			// для плоского круга параллельного экрану
	eConstVisible TestVisible(const Vect3f &pos);							// видимость точки относительно камеры
	__forceinline eConstVisible TestVisibleTile(const MatXf &matrix,const Vect3f &pos,const Vect3f &size,float WorldRadiusShare);
	Vect3f Screen2World(int x, int y);
	void CalcPlaneClip3d();
	void CalcMatrix();
};

class cConvertor
{
	MatXf	Matrix;
	float	wCamera;
	int		attribute;
public:
	__forceinline void ConvertPoint(const Vect3f &pw,Vect3f &pv,Vect3f &pe)
	{
		Matrix.xformPoint(pw,pv);
		if(attribute)
			if(fabs(pv.z) > 1.f) pe.z=FLOAT_CONST_PREC/pv.z; else { pe.z=FLOAT_CONST_PREC; pv.z=1.f; }
		else
			pe.z=wCamera;
		pe.x=pv.x*pe.z;
		pe.y=pv.y*pe.z;
	}
	__forceinline void ConvertPoint(const Vect3f &pw,Vect3f &pv)
	{
		Matrix.xformPoint(pw,pv);
	}
	__forceinline void ConvertVector(const Vect3f &pw,Vect3f &pv)
	{
		Matrix.xformVect(pw,pv);
	}
	__forceinline void InverseConvertVector(const Vect3f &pw,Vect3f &pv)
	{
		Matrix.invXformVect(pw,pv);
	}
	inline void SetProjection(cCamera *Camera,int perspective=-1)
	{ 
		if((perspective)&&(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)))
			attribute=1;	
		else
		{ 
			wCamera=FLOAT_CONST_PREC/Camera->GetPos().z; 
			attribute=0; 
		}
	}
	inline void BuildMatrix(const Vect3f &scale=Vect3f(1,1,1))
	{ 
		Matrix.rot().xrow()*=MATRIX_SIGN_X(scale);
		Matrix.rot().yrow()*=MATRIX_SIGN_Y(scale);
		Matrix.rot().zrow()*=MATRIX_SIGN_Z(scale);
		Matrix.trans().set(MATRIX_SIGN_X(Matrix.trans().x),MATRIX_SIGN_Y(Matrix.trans().y),MATRIX_SIGN_Z(Matrix.trans().z));
	}
	inline MatXf& GetMatrix()										{ return Matrix; }
};

inline void WorldToCameraCutting(Vect3f &pos,cCamera *Camera);
inline void WorldToCameraCutting(MatXf &matrix,cCamera *Camera);
inline void WorldShareMapping(Vect3f &pos,cCamera *Camera);
inline void WorldShareMapping(MatXf &matrix,cCamera *Camera);
inline void WorldSetScale(float scale);

////////////////////////////////// inline cCamera //////////////////////////////////
inline void cCamera::BuildMatrix(MatXf &matrix,const Vect3f &pos)
{
	Vect3f v=pos;
	WorldToCameraCutting(v,this);
	WorldShareMapping(v,this);
	matrix.rot()=GetMatrix().rot();
	matrix.trans().set(
		GetMatrix().rot()[0][0]*v.x+GetMatrix().rot()[0][1]*v.y+GetMatrix().rot()[0][2]*v.z+GetMatrix().trans().x,
		GetMatrix().rot()[1][0]*v.x+GetMatrix().rot()[1][1]*v.y+GetMatrix().rot()[1][2]*v.z+GetMatrix().trans().y,
		GetMatrix().rot()[2][0]*v.x+GetMatrix().rot()[2][1]*v.y+GetMatrix().rot()[2][2]*v.z+GetMatrix().trans().z);
}
inline void cCamera::LoadMatrix(Vect3f &pos)
{
	WorldToCameraCutting(pos,this);
	WorldShareMapping(pos,this);
	pos.set(
		MATRIX_SIGN_X(GetMatrix().rot()[0][0]*pos.x+GetMatrix().rot()[0][1]*pos.y+GetMatrix().rot()[0][2]*pos.z+GetMatrix().trans().x),
		MATRIX_SIGN_Y(GetMatrix().rot()[1][0]*pos.x+GetMatrix().rot()[1][1]*pos.y+GetMatrix().rot()[1][2]*pos.z+GetMatrix().trans().y),
		MATRIX_SIGN_Z(GetMatrix().rot()[2][0]*pos.x+GetMatrix().rot()[2][1]*pos.y+GetMatrix().rot()[2][2]*pos.z+GetMatrix().trans().z));
}
inline void cCamera::LoadMatrix(MatXf &matrix)
{ // функция не реализована
	assert(0);
}
inline void cCamera::ConvertPoint(const MatXf &matrix,const Vect3f &pw,Vect3f &pv,Vect3f &pe)
{
	pv.x=MATRIX_SIGN_X(matrix.rot()[0][0]*pw.x+matrix.rot()[0][1]*pw.y+matrix.rot()[0][2]*pw.z+matrix.trans().x);
	pv.y=MATRIX_SIGN_Y(matrix.rot()[1][0]*pw.x+matrix.rot()[1][1]*pw.y+matrix.rot()[1][2]*pw.z+matrix.trans().y);
	pv.z=MATRIX_SIGN_Z(matrix.rot()[2][0]*pw.x+matrix.rot()[2][1]*pw.y+matrix.rot()[2][2]*pw.z+matrix.trans().z);
	if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=pv.z=1.f; 
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE))
		if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=pv.z=1.f; 
	else
		pe.z=1.f/GetPos().z;
	pe.x=pv.x*pe.z;
	pe.y=pv.y*pe.z;
}
inline void cCamera::ConvertPoint(const Vect3f &pos,const Vect3f &pw,Vect3f &pv,Vect3f &pe)
{
	pv.x=MATRIX_SIGN_X(Matrix.rot()[0][0]*pw.x+Matrix.rot()[0][1]*pw.y+Matrix.rot()[0][2]*pw.z+pos.x);
	pv.y=MATRIX_SIGN_Y(Matrix.rot()[1][0]*pw.x+Matrix.rot()[1][1]*pw.y+Matrix.rot()[1][2]*pw.z+pos.y);
	pv.z=MATRIX_SIGN_Z(Matrix.rot()[2][0]*pw.x+Matrix.rot()[2][1]*pw.y+Matrix.rot()[2][2]*pw.z+pos.z);
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE))
		if(pv.z!=0) pe.z=1.f/pv.z; else pe.z=pv.z=1.f; 
	else
		pe.z=1.f/GetPos().z;
	pe.x=pv.x*pe.z;
	pe.y=pv.y*pe.z;
}
////////////////////////////////// inline World function //////////////////////////////////
extern Vect2f GlobalWorldSize;		// GlobalWorldSize.x,GlobalWorldSize.y - размер мира = 2048.f
extern Vect2f GlobalWorldScale;	// GlobalWorldScale.x,GlobalWorldScale.y - масштабный коэффициент для зацикливания = 1.f
extern float GlobalWorldRadius;

inline void WorldToCameraCutting(Vect3f &pos,cCamera *Camera)
{
	if(Camera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_CUTTING)==0) 
		return;
	Vect2f CameraPos(Camera->GetPos());
	CameraPos*=GlobalWorldScale;
	pos.x=GetShortDistX(pos.x,CameraPos.x,GlobalWorldSize.x*GlobalWorldScale.x)+CameraPos.x;
	pos.y=GetShortDistY(pos.y,CameraPos.y,GlobalWorldSize.y*GlobalWorldScale.y)+CameraPos.y;
}
inline void WorldToCameraCutting(MatXf &Matrix,cCamera *Camera)
{
	if(Camera->GetAttribute(ATTRIBUTE_CAMERA_WORLD_CUTTING)==0) 
		return;
	Vect2f CameraPos(Camera->GetPos());
	CameraPos*=GlobalWorldScale;
	Matrix.trans().x=GetShortDistX(Matrix.trans().x,CameraPos.x,GlobalWorldSize.x*GlobalWorldScale.x)+CameraPos.x;
	Matrix.trans().y=GetShortDistY(Matrix.trans().y,CameraPos.y,GlobalWorldSize.y*GlobalWorldScale.y)+CameraPos.y;
}
inline void WorldShareMapping(MatXf &Matrix,cCamera *Camera)
{
	if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)!=ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)
		return;
	Vect3f& CameraPos = Camera -> GetPos();
	float dx = GetShortDistX(Matrix.trans().x, CameraPos.x);
	float dy = GetShortDistY(Matrix.trans().y, CameraPos.y);
	float arg = GlobalWorldRadius*GlobalWorldRadius-dx*dx-dy*dy;
	if(arg < 0)
		arg = 0;
	float dz = GlobalWorldRadius-sqrtf(arg);
	Vect3f OldOrientation(dx, dy, 0);
	Vect3f NewOrientation(dx, dy, -dz);
	//Vect3f OldOrientation(Matrix.trans().x-CameraPos.x,Matrix.trans().y-CameraPos.y,0);
	//Vect3f NewOrientation(Matrix.trans().x-CameraPos.x,Matrix.trans().y-CameraPos.y,-dz);

	Matrix.trans().z -= dz;

	Vect3f cross = OldOrientation % NewOrientation;
	float angle = (float)(2.*acos(dot(OldOrientation, NewOrientation)/(1e-7 + sqrt(OldOrientation.norm2()*NewOrientation.norm2()))));
	Mat3f m(cross, angle);
	Matrix.rot().premult(m);
}
inline void WorldShareMapping(Vect3f &pos,cCamera *Camera)
{
	if(Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)!=ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE)
		return;
	Vect3f &CameraPos=Camera->GetPos();
	float xr=pos.x-CameraPos.x,yr=pos.y-CameraPos.y;
	float f=xr*xr+yr*yr;
	int l=round(f);
	if(l<(GlobalWorldRadius*GlobalWorldRadius))
		pos.z-=SHARE_FLOAT(l);
	else
	{
		float k=(float)GlobalWorldRadius/sqrt(f);
		xr=xr*k; yr=yr*k;
		f=GlobalWorldRadius*GlobalWorldRadius-(xr*xr+yr*yr);
		if(f<0) f=0;
		int d=GlobalWorldRadius-round(sqrt(f));
		pos.x=CameraPos.x+xr; pos.y=CameraPos.y+yr;
		pos.z-=2*GlobalWorldRadius-d;
/*
		float k=(float)GlobalWorldRadius/sqrt(l);
		xr=xr*k; yr=yr*k;
		float desc=GlobalWorldRadius*GlobalWorldRadius-(xr*xr+yr*yr);
		if(desc<0) desc=0;
		int d=GlobalWorldRadius-sqrt(desc);
		pos.x=CameraPos.x+xr; pos.y=CameraPos.y+yr;
		pos.z-=2*GlobalWorldRadius-d;

		float k=(float)GlobalWorldRadius/sqrt(l);
		xr=round(xr*k); yr=round(yr*k);
		l=xr*xr+yr*yr;
		int d=GlobalWorldRadius-sqrt(GlobalWorldRadius*GlobalWorldRadius-l);
		pos.x=CameraPos.x+xr; pos.y=CameraPos.y+yr;
		pos.z-=2*GlobalWorldRadius-d;
*/
	}
}
inline void WorldSetScale(float scale)
{
	GlobalWorldScale.set(scale,scale);
}

const int XMINTEST=1<<0,YMINTEST=1<<4,XMAXTEST=1<<8,YMAXTEST=1<<16,ZMINTEST=1<<20,ZMAXTEST=1<<24;
const int XMINSHIFT=0,YMINSHIFT=4,XMAXSHIFT=8,YMAXSHIFT=16,ZMINSHIFT=20,ZMAXSHIFT=24;

__forceinline eConstVisible cCamera::TestVisibleTile(const MatXf &matrix,const Vect3f &pos,const Vect3f &size,float WorldRadiusShare)
{
	int test=0;
	Vect3f pv,pe, mt(MATRIX_SIGN_X(matrix.trans().x),MATRIX_SIGN_Y(matrix.trans().y),MATRIX_SIGN_Z(matrix.trans().z)),
		mx(MATRIX_SIGN_X(size.x*matrix.rot()[0][0]),MATRIX_SIGN_Y(size.x*matrix.rot()[1][0]),MATRIX_SIGN_Z(size.x*matrix.rot()[2][0])),
		my(MATRIX_SIGN_X(size.y*matrix.rot()[0][1]),MATRIX_SIGN_Y(size.y*matrix.rot()[1][1]),MATRIX_SIGN_Z(size.y*matrix.rot()[2][1])),
		mz(MATRIX_SIGN_X(size.z*matrix.rot()[0][2]),MATRIX_SIGN_Y(size.z*matrix.rot()[1][2]),MATRIX_SIGN_Z(size.z*matrix.rot()[2][2]));
	
	if(GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE)==0)
	{
		if(GetPos().z) pe.z=1.f/GetPos().z; else pe.z=1.f;
		pv.x=mt.x, pv.y=mt.y; pv.z=mt.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x=mx.x+mt.x, pv.y=mx.y+mt.y; pv.z=mx.z+mt.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x=my.x+mt.x, pv.y=my.y+mt.y; pv.z=my.z+mt.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x=mx.x+my.x+mt.x, pv.y=mx.y+my.y+mt.y; pv.z=mx.z+my.z+mt.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
	}
	else if(GetAttribute(ATTRIBUTE_CAMERA_WORLD_SHARE))
	{
		Vect2f ShareOfs(pos.x-GetPos().x,pos.y-GetPos().y);
		int xr=round(ShareOfs.x), yr=round(ShareOfs.y), xrt=xr+round(size.x),yrt=yr+round(size.y);
		int xr2=xr*xr,yr2=yr*yr,xrt2=xrt*xrt,yrt2=yrt*yrt;
		Vect2f TileCenter(ShareOfs.x+size.x*0.5f,ShareOfs.y+size.y*0.5f);
		if(TileCenter.norm2()>(WorldRadiusShare*WorldRadiusShare)) 
			return CONST_VISIBLE_NULL;
		Vect3f dShare;
		
		dShare.set(MATRIX_SIGN_X(Matrix.rot()[0][2]),MATRIX_SIGN_Y(Matrix.rot()[1][2]),MATRIX_SIGN_Z(Matrix.rot()[2][2])); 
		dShare*=-SHARE_FLOAT(xr2+yr2);
		pv.x=mt.x+dShare.x, pv.y=mt.y+dShare.y; pv.z=mt.z+dShare.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		dShare.set(MATRIX_SIGN_X(Matrix.rot()[0][2]),MATRIX_SIGN_Y(Matrix.rot()[1][2]),MATRIX_SIGN_Z(Matrix.rot()[2][2])); 
		dShare*=-SHARE_FLOAT(xrt2+yr2);
		pv.x=mx.x+mt.x+dShare.x, pv.y=mx.y+mt.y+dShare.y; pv.z=mx.z+mt.z+dShare.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		dShare.set(MATRIX_SIGN_X(Matrix.rot()[0][2]),MATRIX_SIGN_Y(Matrix.rot()[1][2]),MATRIX_SIGN_Z(Matrix.rot()[2][2])); 
		dShare*=-SHARE_FLOAT(xr2+yrt2);
		pv.x=my.x+mt.x+dShare.x, pv.y=my.y+mt.y+dShare.y; pv.z=my.z+mt.z+dShare.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		dShare.set(MATRIX_SIGN_X(Matrix.rot()[0][2]),MATRIX_SIGN_Y(Matrix.rot()[1][2]),MATRIX_SIGN_Z(Matrix.rot()[2][2])); 
		dShare*=-SHARE_FLOAT(xrt2+yrt2);
		pv.x=mx.x+my.x+mt.x+dShare.x, pv.y=mx.y+my.y+mt.y+dShare.y; pv.z=mx.z+my.z+mt.z+dShare.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
	}
	else
	{
/*		Vect2f &ScrSize=RenderDevice->GetSize();
		Vect2f Center=GetCenter()*ScrSize,Focus=GetFocus()*ScrSize;
		sVect2i box[8];
*/		pv.x=mt.x, pv.y=mt.y; pv.z=mt.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[0].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[1].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x=mx.x+mt.x, pv.y=mx.y+mt.y; pv.z=mx.z+mt.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[2].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[3].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x=my.x+mt.x, pv.y=my.y+mt.y; pv.z=my.z+mt.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[4].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[5].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x=mx.x+my.x+mt.x, pv.y=mx.y+my.y+mt.y; pv.z=mx.z+my.z+mt.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[6].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);

		pv.x+=mz.x, pv.y+=mz.y; pv.z+=mz.z;
		if(pv.z) pe.z=1/pv.z; else pe.z=1.f;
		pe.x=pv.x*pe.z, pe.y=pv.y*pe.z;
		if(pv.z<zPlane.x) { test+=ZMINTEST;	if(pv.z<0) pe=-pe; }
		if(pe.x<Clip.xmin()) test+=XMINTEST; else if(pe.x>Clip.xmax()) test+=XMAXTEST;
		if(pe.y<Clip.ymin()) test+=YMINTEST; else if(pe.y>Clip.ymax()) test+=YMAXTEST;
//		box[7].set(Center.x+Focus.x*pe.x,Center.y+Focus.y*pe.y);
/*		RenderDevice->GetIGraph3d()->DrawLine(box[0].x,box[0].y,box[1].x,box[1].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[2].x,box[2].y,box[3].x,box[3].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[4].x,box[4].y,box[5].x,box[5].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[6].x,box[6].y,box[7].x,box[7].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[0].x,box[0].y,box[2].x,box[2].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[1].x,box[1].y,box[3].x,box[3].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[4].x,box[4].y,box[6].x,box[6].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[5].x,box[5].y,box[7].x,box[7].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[0].x,box[0].y,box[4].x,box[4].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[1].x,box[1].y,box[5].x,box[5].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[2].x,box[2].y,box[6].x,box[6].y,255,255,255,255);
		RenderDevice->GetIGraph3d()->DrawLine(box[3].x,box[3].y,box[7].x,box[7].y,255,255,255,255);
*/	}
	if(test&((8<<XMINSHIFT)|(8<<XMAXSHIFT)|(8<<YMINSHIFT)|(8<<YMAXSHIFT)|(8<<ZMINSHIFT)|(8<<ZMAXSHIFT)))
		return CONST_VISIBLE_NULL;
	int visible=CONST_VISIBLE_FRUSTUM;
	if(test==0) return CONST_VISIBLE_FRUSTUM;
	if(test&(7<<XMINSHIFT)) visible^=CONST_VISIBLE_XMIN;
	if(test&(7<<XMAXSHIFT)) visible^=CONST_VISIBLE_XMAX;
	if(test&(7<<YMINSHIFT)) visible^=CONST_VISIBLE_YMIN;
	if(test&(7<<YMAXSHIFT)) visible^=CONST_VISIBLE_YMAX;
	if(test&(7<<ZMINSHIFT)) visible^=CONST_VISIBLE_ZMIN;
	if(test&(7<<ZMAXSHIFT)) visible^=CONST_VISIBLE_ZMAX;
	return (eConstVisible)visible;
}

#endif //__CAMERA_H__