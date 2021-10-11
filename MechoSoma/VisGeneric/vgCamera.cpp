#include "VisGeneric.h"
#include "Camera.h"
#include "RenderDevice.h"

cUnknownClass* cVisGeneric::CreateCamera(char *CreateBuffer)
{
	cCamera *Camera=new cCamera;
	Camera->SetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE);
	return Camera;
}
int cVisGeneric::ReleaseCamera(cUnknownClass *UCamera)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	GetActiveScene()->Detach(Camera);
	delete Camera;
	return 0;
}
int cVisGeneric::AttachCamera(cUnknownClass *UCamera)
{
	assert(GetActiveScene());
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	GetActiveScene()->Attach(Camera);
	return 0;
}
int cVisGeneric::DetachCamera(cUnknownClass *UCamera)
{
	assert(GetActiveScene());
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Scene->Detach(Camera);
	return 0;
}
int cVisGeneric::SetCameraPosition(cUnknownClass *UCamera,const Vect3f *Pos,const Vect3f *AngleGrad,const Vect3f *PosOfs)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->SetPosition(*Pos,*AngleGrad);
	if(PosOfs) Camera->SetOfs(*PosOfs);
	return 0;
}
int cVisGeneric::dSetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->SetPosition(Camera->GetMatrix()*(*Matrix));
	return 0;
}
int cVisGeneric::SetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix,const Vect3f *pos)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	if(Matrix) Camera->SetPosition(*Matrix);
	if(pos) Camera->SetPos(*pos);
	return 0;
}
int cVisGeneric::dSetCameraPosition(cUnknownClass *UCamera,const Vect3f *dPos,const Vect3f *dAngleGrad)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->dSetPosition(*dPos,*dAngleGrad);
	return 0;
}
/*
int cVisGeneric::GetCameraPosition(cUnknownClass *UCamera,Vect3f *Pos,Vect3f *AngleGrad,Vect3f *Ofs)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->GetPosition(*Pos,*AngleGrad);
	Camera->GetOfs(*Ofs);
	return 0;
}
*/
int cVisGeneric::SetCameraAttribute(cUnknownClass *UCamera,int Attribute)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->SetAttribute(Attribute);
	return 0;
}
int cVisGeneric::GetCameraAttribute(cUnknownClass *UCamera,int Attribute)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	return Camera->GetAttribute(Attribute);
}
int cVisGeneric::ClearCameraAttribute(cUnknownClass *UCamera,int Attribute)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->ClearAttribute(Attribute);
	return 0;
}
int cVisGeneric::SetCameraFrustum(cUnknownClass *UCamera,const Vect2f *Center,const sRectangle4f *Clipping,const Vect2f *Focus,const Vect2f *zPlane,const Vect2f *zBufferScale)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	sRectangle4f Clip;
	if(Center==0) Center=&Camera->GetCenter();
	if(Focus==0) Focus=&Camera->GetFocus();
	if(zPlane==0) zPlane=&Camera->GetZPlane();
	if(zBufferScale==0) zBufferScale=&Camera->GetZBufferScale();
	if(Clipping==0) Clip=Camera->GetClipping();
	else Clip.set(Clipping->xmin()/Focus->x,Clipping->ymin()/Focus->y,Clipping->xmax()/Focus->x,Clipping->ymax()/Focus->y);
	Camera->SetFrustum(Clip,*Center,*Focus,*zPlane,*zBufferScale);
	return 0;
}
int cVisGeneric::GetCameraFrustum(cUnknownClass *UCamera,Vect2f *Center,sRectangle4f *Clipping,Vect2f *Focus,Vect2f *zPlane,Vect2f *zBufferScale)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	sRectangle4f Clip;
	Camera->GetFrustum(Clip,*Center,*Focus,*zPlane,*zBufferScale);
	Clipping->xmin()=Center->x+Clip.xmin()*Focus->x;
	Clipping->xmax()=Center->x+Clip.xmax()*Focus->x;
	Clipping->ymin()=Center->y+Clip.ymin()*Focus->y;
	Clipping->ymax()=Center->y+Clip.ymax()*Focus->y;
	return 0;
}
int cVisGeneric::AttachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *URenderDevice)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->AttachViewPort(URenderDevice);
	return 0;
}
int cVisGeneric::DetachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *URenderDevice)
{
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->DetachViewPort(URenderDevice);
	return 0;
}
int cVisGeneric::SetCameraOfs(cUnknownClass *UCamera,const Vect3f *Ofs)
{
	if(UCamera==0) 
	{
		assert(GetActiveScene());
		UCamera=GetActiveScene()->GetCamera(0);
	}
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	Camera->SetOfs(*Ofs);
	return 0;
}
int cVisGeneric::GetCameraOfs(cUnknownClass *UCamera,Vect3f *Ofs)
{
	if(UCamera==0) 
	{
		assert(GetActiveScene());
		UCamera=GetActiveScene()->GetCamera(0);
	}
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	*Ofs=Camera->GetOfs();
	return 0;
}
int cVisGeneric::ConvertorWorldToCamera(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe)
{
	if(UCamera==0) 
	{
		assert(GetActiveScene());
		UCamera=GetActiveScene()->GetCamera(0);
	}
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	cConvertor ConvertorObjectToScreen;
	ConvertorObjectToScreen.GetMatrix().set(Mat3f::ID,*pw);
	WorldToCameraCutting(ConvertorObjectToScreen.GetMatrix(),Camera);
	WorldShareMapping(ConvertorObjectToScreen.GetMatrix().trans(),Camera);
	ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*ConvertorObjectToScreen.GetMatrix();
	ConvertorObjectToScreen.BuildMatrix();
	ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
	ConvertorObjectToScreen.ConvertPoint(Vect3f(0,0,0),*pv,*pe);
	(*pe)/=FLOAT_CONST_PREC;
	return 0;
}
int cVisGeneric::ConvertorWorldToViewPort(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe)
{
	if(UCamera==0) 
	{
		assert(GetActiveScene());
		UCamera=GetActiveScene()->GetCamera(0);
	}
	assert(UCamera&&UCamera->GetKind(KIND_CAMERA));
	cCamera *Camera=(cCamera*)UCamera;
	cConvertor ConvertorObjectToScreen;
	ConvertorObjectToScreen.GetMatrix().set(Mat3f::ID,*pw);
	WorldToCameraCutting(ConvertorObjectToScreen.GetMatrix(),Camera);
	WorldShareMapping(ConvertorObjectToScreen.GetMatrix().trans(),Camera);
	ConvertorObjectToScreen.GetMatrix()=Camera->GetMatrix()*ConvertorObjectToScreen.GetMatrix();
	ConvertorObjectToScreen.BuildMatrix();
	ConvertorObjectToScreen.SetProjection(Camera,Camera->GetAttribute(ATTRIBUTE_CAMERA_PERSPECTIVE));
	ConvertorObjectToScreen.ConvertPoint(Vect3f(0,0,0),*pv,*pe);
	cRenderDevice *RenderDevice=(cRenderDevice*)Camera->GetViewPort();
	cInterfaceGraph3d *IGraph3d=RenderDevice->GetIGraph3d();
	Vect2f &ScrSize=RenderDevice->GetSize();
	Vect2f Center=Camera->GetCenter()*ScrSize,Focus=Camera->GetFocus()*ScrSize.x/FLOAT_CONST_PREC;	
	Vect2f &zPlane=Camera->GetZPlane(),&zBufferScale=Camera->GetZBufferScale();
	pe->set(Center.x+Focus.x*pe->x,Center.y+Focus.y*pe->y,
		zBufferScale.y-(zBufferScale.y-zBufferScale.x)*zPlane.x*pe->z);
	return 0;
}
