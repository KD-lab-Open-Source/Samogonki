#ifndef __IVISGENERIC_H__
#define __IVISGENERIC_H__

#include "Unknown.h"

class Vect2f;
class Vect3f;
class MatXf;
struct sColor4f;
struct sRectangle4f;
struct XBuffer;
class cInterfaceGraph3d;

UI_BEGIN(cInterfaceVisGeneric,KIND_UI_VISGENERIC)
	// ����������������� ����� 
	UI_IMPORT( int Create(char *CreateBuffer) );
	UI_IMPORT( int Release() );
	// ������� ��� ������ ���������� � ����������� ������
	UI_IMPORT( int PreDraw(int mask=0xFFFFFFFF) );
	UI_IMPORT( int Draw(int mask=0xFFFFFFFF) );
	UI_IMPORT( int PostDraw(int mask=0xFFFFFFFF) );
	UI_IMPORT( int SetTime(int Time) );
	UI_IMPORT( int dSetTime(int dTime) );
	UI_IMPORT( int GetTime(int *Time) );
	// ������� ��� ������ �� ������
	UI_IMPORT( cUnknownClass* CreateScene() );
	UI_IMPORT( int ReleaseScene(cUnknownClass* Scene) );
	UI_IMPORT( int SetScene(cUnknownClass* Scene) );
	// ����� ������� ��� ������ ��������� cUnknownClass
	UI_IMPORT( int Release(cUnknownClass *UnkClass) );
	UI_IMPORT( int SetAttribute(cUnknownClass *UnkClass,int Attribute) );
	UI_IMPORT( int GetAttribute(cUnknownClass *UnkClass,int Attribute=0xFFFFFFFF) );
	UI_IMPORT( int ClearAttribute(cUnknownClass *UnkClass,int Attribute=0xFFFFFFFF) );
	UI_IMPORT( int SetPosition(cUnknownClass *UnkClass,const Vect3f *Pos,const Vect3f *AngleGrad=0) );
	UI_IMPORT( int dSetPosition(cUnknownClass *UnkClass,const Vect3f *dPos,const Vect3f *dAngleGrad) );
	UI_IMPORT( int GetPosition(cUnknownClass *UnkClass,Vect3f *Pos,Vect3f *AngleGrad) );
	// ������� ��� ������ � ����� ������
	UI_IMPORT( cUnknownClass* CreateGraph(int xScr,int yScr,int GraphMode=0,int FullScr=0,int ColorBit=16) );
	UI_IMPORT( int SetGraphClipping(cUnknownClass *URenderDevice,const sRectangle4f *clip) );
	UI_IMPORT( int GetGraphClipping(cUnknownClass *URenderDevice,sRectangle4f *clip) );
	UI_IMPORT( int FillGraph(cUnknownClass *URenderDevice=0,int r=0,int g=0,int b=0) );
	UI_IMPORT( int FlushGraph(cUnknownClass *URenderDevice=0) );
	UI_IMPORT( int ReleaseGraph(cUnknownClass *URenderDevice=0) );
	UI_IMPORT( int ReInitGraph(cUnknownClass *URenderDevice,int xScr,int yScr,int GraphMode=0,int FullScr=0,int ColorBit=16) );
	UI_IMPORT( int GetGraphInfo(cUnknownClass *URenderDevice,int *xScr,int *yScr,int *GraphMode=0,int *FullScr=0,int *ColorBit=0) );
	UI_IMPORT( cInterfaceGraph3d* GetIGraph3d(cUnknownClass *URenderDevice=0) );
	// ������� ��� ������ � ���������
	UI_IMPORT( int LoadObjectLibrary(const char *fname="RESOURCE\\m3d.scb") );
	UI_IMPORT( int FreeObjectLibrary(int kind=0xFFFFFFFF,int type=0xFFFFFFFF) );
	UI_IMPORT( int ReleaseObject(int kind,int type) );
	UI_IMPORT( cUnknownClass* CreateObject(char *fname,char *TexturePath,int Kind=0,int Type=0) );
	UI_IMPORT( cUnknownClass* CreateObject(int kind,int type) );
	UI_IMPORT( int ReleaseObject(cUnknownClass* UObject) );
	UI_IMPORT( int SetObjectAttribute(cUnknownClass *UObject,int attribute) );
	UI_IMPORT( int GetObjectAttribute(cUnknownClass *UObject,int attribute=0xFFFFFFFF) );
	UI_IMPORT( int ClearObjectAttribute(cUnknownClass *UObject,int attribute=0xFFFFFFFF) );
	UI_IMPORT( int dSetObjectPosition(cUnknownClass *UObject,const Vect3f *dPos,const Vect3f *dAngleGrad=0) );
	UI_IMPORT( int SetObjectPosition(cUnknownClass *UObject,const Vect3f *Pos,const Vect3f *AngleGrad=0) );
	UI_IMPORT( int dSetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix) );
	UI_IMPORT( int SetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix) );
	UI_IMPORT( int GetObjectPosition(cUnknownClass *UObject,Vect3f *Pos,Vect3f *AngleGrad) );
	UI_IMPORT( int GetObjectPosition(cUnknownClass *UObject,MatXf *Matrix) );
	UI_IMPORT( int dSetObjectScale(cUnknownClass *UObject,const Vect3f *dScale) );
	UI_IMPORT( int SetObjectScale(cUnknownClass *UObject,const Vect3f *Scale) );
	UI_IMPORT( int GetObjectScale(cUnknownClass *UObject,Vect3f *Scale) );
	UI_IMPORT( int dSetObjectAnimation(cUnknownClass *UObject,float dTime) );
	UI_IMPORT( int GetObjectAnimation(cUnknownClass *UObject,float *AnimationPeriod,float *StartPhase) );
	UI_IMPORT( int SetObjectAnimation(cUnknownClass *UObject,float AnimationPeriod=2000.f,float StartPhase=0.f,float FinishPhase=-1.f,int mask=0xFFFFFFFF) );
	UI_IMPORT( int IsObjectAnimationPhasePassed(cUnknownClass *UObject,float Phase) );
	UI_IMPORT( int SetObjectChannel(cUnknownClass *UObject,int NumberChain,float TransitionTime=200.f) );
	UI_IMPORT( int SetObjectChannel(cUnknownClass *UObject,char *NameChainMask=0,float TransitionTime=200.f,int number=0) );
	UI_IMPORT( int GetObjectNumberChannel(cUnknownClass *UObject,char *NameChainMask=0) );
	UI_IMPORT( int SetObjectColor(cUnknownClass *UObject,const sColor4f *diffuse,const sColor4f *specular=0,int mask=0xffffffff) );
	UI_IMPORT( int GetObjectColor(cUnknownClass *UObject,sColor4f *diffuse,sColor4f *specular=0,int mask=0xffffffff) );
	UI_IMPORT( int SetObjectVisible(cUnknownClass *UObject,int visible) );
	UI_IMPORT( int GetObjectVisible(cUnknownClass *UObject,int visible=0xffffffff) );
	UI_IMPORT( int SetRenderObjectSwitch(int attribute) );
	UI_IMPORT( int GetRenderObjectSwitch(int attribute=0xFFFFFFFF) );
	UI_IMPORT( int ClearRenderObjectSwitch(int attribute=0xFFFFFFFF) );
	UI_IMPORT( int LoadObject(cUnknownClass **UObject,XBuffer &buf,cUnknownClass *UParent=0) );
	UI_IMPORT( int SaveObject(cUnknownClass *UObject,XBuffer &buf) );
	// ������� ��� ������ � ��������� ����������� �����
	UI_IMPORT( cUnknownClass* CreateOmni(const Vect3f *Pos,float radius,const sColor4f *Color) );
	UI_IMPORT( int ReleaseOmni(cUnknownClass *UOmni) );
	UI_IMPORT( int ReleaseOmni(int type) );
	UI_IMPORT( int dSetOmniPosition(cUnknownClass *UOmni,const Vect3f *dPos) );
	UI_IMPORT( int SetOmniPosition(cUnknownClass *UOmni,const Vect3f *Pos) );
	UI_IMPORT( int GetOmniPosition(cUnknownClass *UOmni,Vect3f *Pos) );
	UI_IMPORT( int SetOmniColor(cUnknownClass *UOmni,const sColor4f *Color) );
	UI_IMPORT( int GetOmniColor(cUnknownClass *UOmni,sColor4f *Color) );
	UI_IMPORT( int SetOmniSize(cUnknownClass *UOmni,float radius) );
	UI_IMPORT( int GetOmniSize(cUnknownClass *UOmni,float *radius) );
	UI_IMPORT( int SetOmniVisible(cUnknownClass *UOmni,int visible) );
	UI_IMPORT( int GetOmniVisible(cUnknownClass *UOmni,int visible=0xFFFFFFFF) );
	// ������� ��� ������ � ��������
	UI_IMPORT( cUnknownClass* CreateCamera(char *CreateBuffer=0) );
	UI_IMPORT( int AttachCamera(cUnknownClass *UCamera) );
	UI_IMPORT( int DetachCamera(cUnknownClass *UCamera) );
	UI_IMPORT( int SetCameraAttribute(cUnknownClass *UCamera,int Attribute) );
	UI_IMPORT( int GetCameraAttribute(cUnknownClass *UCamera,int Attribute=0xFFFFFFFF) );
	UI_IMPORT( int ClearCameraAttribute(cUnknownClass *UCamera,int Attribute=0xFFFFFFFF) );
	UI_IMPORT( int dSetCameraPosition(cUnknownClass *UCamera,const Vect3f *dPos,const Vect3f *dAngleGrad) );
	UI_IMPORT( int SetCameraPosition(cUnknownClass *UCamera,const Vect3f *Pos,const Vect3f *AngleGrad,const Vect3f *PosOfs=0) );
	UI_IMPORT( int dSetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix) );
	UI_IMPORT( int SetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix,const Vect3f *pos=0) );
//	UI_IMPORT( int GetCameraPosition(cUnknownClass *UCamera,Vect3f *Pos,Vect3f *AngleGrad,Vect3f *PosOfs) );
	UI_IMPORT( int SetCameraFrustum(cUnknownClass *UCamera,const Vect2f *Center,const sRectangle4f *Clipping,const Vect2f *Focus,const Vect2f *zPlane,const Vect2f *zBufferScale) );
	UI_IMPORT( int GetCameraFrustum(cUnknownClass *UCamera,Vect2f *Center,sRectangle4f *Clipping,Vect2f *Focus,Vect2f *zPlane,Vect2f *zBufferScale) );
	UI_IMPORT( int AttachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *RenderDevice) );
	UI_IMPORT( int DetachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *RenderDevice) );
	UI_IMPORT( int ConvertorWorldToCamera(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe) );
	UI_IMPORT( int ConvertorWorldToViewPort(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe) );
	UI_IMPORT( int SetCameraOfs(cUnknownClass *UCamera,const Vect3f *Ofs) );
	UI_IMPORT( int GetCameraOfs(cUnknownClass *UCamera,Vect3f *Ofs) );
	UI_IMPORT( int ReleaseCamera(cUnknownClass *UCamera=0) );
	// ������� ��� ������ � ����������� �����, ��������� �� ��������� �������� ������� �����
	UI_IMPORT( cUnknownClass* CreateLight(int id=-1) );
	UI_IMPORT( int SetLightPosition(cUnknownClass *ULight,const Vect3f *pos) );
	UI_IMPORT( int GetLightPosition(cUnknownClass *ULight,Vect3f *pos) );
	UI_IMPORT( int SetLightSize(cUnknownClass *ULight,float radius) );
	UI_IMPORT( int GetLightSize(cUnknownClass *ULight,float radius) );
	UI_IMPORT( int SetLightColor(cUnknownClass *ULight,const sColor4f *ambient=0,const sColor4f *diffuse=0,const sColor4f *illumination=0) );
	UI_IMPORT( int GetLightColor(cUnknownClass *ULight,sColor4f *ambient=0,sColor4f *diffuse=0,sColor4f *illumination=0) );
	UI_IMPORT( cUnknownClass* GetLight(int id) );
	UI_IMPORT( int ReleaseLight(cUnknownClass *ULight) );// ������� �������� ��������
	// ������� ��� ������ �� �������
	UI_IMPORT( cUnknownClass* CreateTangentTrail(const Vect3f *pos,const sColor4f *c1,const Vect3f *dpos,const sColor4f *c2,float Time=10000.f,float dAnimTime=1.e30f,float Width=2.f) );
	UI_IMPORT( int ReleaseTangentTrail(cUnknownClass* UTangentTrail) );
	// �������� ����, �� ���������� � ������� fname, number - ����� ����
	UI_IMPORT( cUnknownClass* CreateWorld(char *fname,int number=0,int track=0,int LoadTerra=0) );
	UI_IMPORT( int ReleaseWorld(cUnknownClass *UWorld=0) );
	UI_IMPORT( int GetPolygonMapAttribute(int x1,int y1,int x2,int y2,int x3,int y3) );
	UI_IMPORT( cUnknownClass* FindObject(char *name) ); // ����� ������� �� ���� �� ����� �������
	UI_IMPORT( cUnknownClass* FindObjectByFileName(char *fname) ); // ����� ������� �� ���� �� ����� ����� �������
	UI_IMPORT( cUnknownClass* GetMeshLibrary(cUnknownClass *UScene=0) );
	UI_IMPORT( cUnknownClass* GetCameraList(cUnknownClass *UScene=0) );
	UI_IMPORT( cUnknownClass* GetTileWater(cUnknownClass *UScene=0) );
	UI_IMPORT( cUnknownClass* GetTileMap(cUnknownClass *UScene=0) );
	UI_IMPORT( cUnknownClass* GetM3D(cUnknownClass *UScene=0) );
	UI_IMPORT( cUnknownClass* GetO3D(cUnknownClass *UScene=0) );
	UI_IMPORT( int SetWorldPolygonFirst() );
	UI_IMPORT( int GetWorldPolygonNext(Vect3f &v1,Vect3f &v2,Vect3f &v3,int &attribute) );
UI_END(cInterfaceVisGeneric)

extern cInterfaceVisGeneric* CreateIVisGeneric(char *CreateBuffer=0);

#endif //__IVISGENERIC_H__