#ifndef __VISGENERIC_H__
#define __VISGENERIC_H__

#include "IVisGeneric.h"
#include "Scene.h"

class cInterfaceGraph3d;

class cVisGeneric : public cInterfaceVisGeneric
{
public:
	// ����������������� ����� 
	cVisGeneric();
	~cVisGeneric();
	// ����������������� ����� 
	virtual int Create(char *CreateBuffer);
	virtual int Release();
	// ������� ��� ������ ���������� � ����������� ������
	virtual int PreDraw(int mask=0xFFFFFFFF);
	virtual int Draw(int mask=0xFFFFFFFF);
	virtual int PostDraw(int mask=0xFFFFFFFF);
	virtual int SetTime(int Time);
	virtual int dSetTime(int dTime);
	virtual int GetTime(int *Time);
	// ������� ��� ������ �� ������
	virtual cUnknownClass* CreateScene();
	virtual int ReleaseScene(cUnknownClass* Scene);
	virtual int SetScene(cUnknownClass* Scene);
	// ����� ������� ��� ������ ��������� cUnknownClass: Camera
	virtual int Release(cUnknownClass *UnkClass);
	virtual int SetAttribute(cUnknownClass *UnkClass,int Attribute);
	virtual int ClearAttribute(cUnknownClass *UnkClass,int Attribute);
	virtual int GetAttribute(cUnknownClass *UnkClass,int Attribute=0xFFFFFFFF);
	virtual int SetPosition(cUnknownClass *UnkClass,const Vect3f *Pos,const Vect3f *AngleGrad=0);
	virtual int dSetPosition(cUnknownClass *UnkClass,const Vect3f *dPos,const Vect3f *dAngleGrad);
	virtual int GetPosition(cUnknownClass *UnkClass,Vect3f *Pos,Vect3f *AngleGrad);
	// ������� ��� ������ � ����� ������
	virtual cUnknownClass* CreateGraph(int xScr,int yScr,int GraphMode=0,int FullScr=0,int ColorBit=16);
	virtual int ReInitGraph(cUnknownClass *URenderDevice,int xScr,int yScr,int GraphMode=0,int FullScr=0,int ColorBit=16);
	virtual int GetGraphInfo(cUnknownClass *URenderDevice,int *xScr,int *yScr,int *GraphMode=0,int *FullScr=0,int *ColorBit=0);
	virtual int SetGraphClipping(cUnknownClass *URenderDevice,const sRectangle4f *clip);
	virtual int GetGraphClipping(cUnknownClass *URenderDevice,sRectangle4f *clip);
	virtual int FillGraph(cUnknownClass *URenderDevice=0,int r=0,int g=0,int b=0);
	virtual int FlushGraph(cUnknownClass *URenderDevice=0);
	virtual int ReleaseGraph(cUnknownClass *URenderDevice=0);
	virtual cInterfaceGraph3d* GetIGraph3d(cUnknownClass *URenderDevice=0);
	// ������� ��� ������ � ���������
	virtual int LoadObjectLibrary(const std::filesystem::path &path);
	virtual int FreeObjectLibrary(int kind=0xFFFFFFFF,int type=0xFFFFFFFF);
	virtual int ReleaseObject(int kind,int type);
	virtual cUnknownClass* CreateObject(char *fname,char *TexturePath,int Kind=0,int Type=0);
	virtual cUnknownClass* CreateObject(int kind,int type);
	virtual int SetObjectAttribute(cUnknownClass *UObject,int attribute);
	virtual int ClearObjectAttribute(cUnknownClass *UObject,int attribute);
	virtual int GetObjectAttribute(cUnknownClass *UObject,int attribute=0xFFFFFFFF);
	virtual int ReleaseObject(cUnknownClass* UObject);
	virtual int dSetObjectPosition(cUnknownClass *UObject,const Vect3f *dPos,const Vect3f *dAngleGrad=0);
	virtual int SetObjectPosition(cUnknownClass *UObject,const Vect3f *Pos,const Vect3f *AngleGrad=0);
	virtual int SetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix);
	virtual int dSetObjectPosition(cUnknownClass *UObject,const MatXf *Matrix);
	virtual int GetObjectPosition(cUnknownClass *UObject,Vect3f *Pos,Vect3f *AngleGrad);
	virtual int GetObjectPosition(cUnknownClass *UObject,MatXf *Matrix);
	virtual int dSetObjectScale(cUnknownClass *UObject,const Vect3f *dScale);
	virtual int SetObjectScale(cUnknownClass *UObject,const Vect3f *Scale);
	virtual int GetObjectScale(cUnknownClass *UObject,Vect3f *Scale);
	virtual int dSetObjectAnimation(cUnknownClass *UObject,float dTime);
	virtual int GetObjectAnimation(cUnknownClass *UObject,float *AnimationPeriod,float *StartPhase);
	virtual int SetObjectAnimation(cUnknownClass *UObject,float AnimationPeriod,float StartPhase=0.f,float FinishPhase=1.f,int mask=0xFFFFFFFF);
	virtual int SetObjectChannel(cUnknownClass *UObject,int NumberChain,float TransitionTime=200.f);
	virtual int SetObjectChannel(cUnknownClass *UObject,char *NameChainMask=0,float TransitionTime=200.f,int number=0);
	virtual int GetObjectNumberChannel(cUnknownClass *UObject,char *NameChain=0);
	virtual int IsObjectAnimationPhasePassed(cUnknownClass *UObject,float Phase);
	virtual int SetObjectColor(cUnknownClass *UObject,const sColor4f *diffuse,const sColor4f *specular,int mask=0xffffffff);
	virtual int GetObjectColor(cUnknownClass *UObject,sColor4f *diffuse,sColor4f *specular,int mask=0xffffffff);
	virtual int SetObjectVisible(cUnknownClass *UObject,int visible);
	virtual int GetObjectVisible(cUnknownClass *UObject,int visible=0xffffffff);
	virtual int SetRenderObjectSwitch(int attribute);
	virtual int GetRenderObjectSwitch(int attribute=0xFFFFFFFF);
	virtual int ClearRenderObjectSwitch(int attribute=0xFFFFFFFF);
	virtual int LoadObject(cUnknownClass **UObject,XBuffer &buf,cUnknownClass *UParent=0);
	virtual int SaveObject(cUnknownClass *UObject,XBuffer &buf);
	// ������� ��� ������ � ��������� ����������� �����
	virtual cUnknownClass* CreateOmni(const Vect3f *Pos,float radius,const sColor4f *Color);
	virtual int ReleaseOmni(cUnknownClass *UOmni);
	virtual int ReleaseOmni(int type);
	virtual int dSetOmniPosition(cUnknownClass *UOmni,const Vect3f *dPos);
	virtual int SetOmniPosition(cUnknownClass *UOmni,const Vect3f *Pos);
	virtual int GetOmniPosition(cUnknownClass *UOmni,Vect3f *Pos);
	virtual int SetOmniColor(cUnknownClass *UOmni,const sColor4f *Color);
	virtual int GetOmniColor(cUnknownClass *UOmni,sColor4f *Color);
	virtual int SetOmniSize(cUnknownClass *UOmni,float radius);
	virtual int GetOmniSize(cUnknownClass *UOmni,float *radius);
	virtual int SetOmniVisible(cUnknownClass *UOmni,int visible);
	virtual int GetOmniVisible(cUnknownClass *UOmni,int visible=0xFFFFFFFF);
	// ������� ��� ������ � ��������
	virtual cUnknownClass* CreateCamera(char *CreateBuffer=0);
	virtual int AttachCamera(cUnknownClass *UCamera);
	virtual int DetachCamera(cUnknownClass *UCamera);
	virtual int SetCameraAttribute(cUnknownClass *UCamera,int Attribute);
	virtual int ClearCameraAttribute(cUnknownClass *UCamera,int Attribute);
	virtual int GetCameraAttribute(cUnknownClass *UCamera,int Attribute=0xFFFFFFFF);
	virtual int dSetCameraPosition(cUnknownClass *UCamera,const Vect3f *dPos,const Vect3f *dAngleGrad);
	virtual int SetCameraPosition(cUnknownClass *UCamera,const Vect3f *Pos,const Vect3f *AngleGrad,const Vect3f *PosOfs=0);
	virtual int dSetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix);
	virtual int SetCameraPosition(cUnknownClass *UCamera,const MatXf *Matrix,const Vect3f *pos=0);
//	virtual int GetCameraPosition(cUnknownClass *UCamera,Vect3f *Pos,Vect3f *AngleGrad,Vect3f *PosOfs);
	virtual int SetCameraFrustum(cUnknownClass *UCamera,const Vect2f *Center,const sRectangle4f *Clipping,const Vect2f *Focus,const Vect2f *zPlane,const Vect2f *zBufferScale);
	virtual int GetCameraFrustum(cUnknownClass *UCamera,Vect2f *Center,sRectangle4f *Clipping,Vect2f *Focus,Vect2f *zPlane,Vect2f *zBufferScale);
	virtual int AttachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *RenderDevice);
	virtual int DetachCameraViewPort(cUnknownClass *UCamera,const cUnknownClass *RenderDevice);
	virtual int SetCameraOfs(cUnknownClass *UCamera,const Vect3f *Ofs);
	virtual int GetCameraOfs(cUnknownClass *UCamera,Vect3f *Ofs);
	virtual int ConvertorWorldToCamera(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe);
	virtual int ConvertorWorldToViewPort(cUnknownClass *UCamera,const Vect3f *pw,Vect3f *pv,Vect3f *pe);
	virtual int ReleaseCamera(cUnknownClass *UCamera);
	// ������� ��� ������ � ����������� �����, ��������� �� ��������� �������� ������� �����
	virtual cUnknownClass* CreateLight(int id=-1);// ��������� �������� ��������
	virtual int SetLightPosition(cUnknownClass *ULight,const Vect3f *pos);
	virtual int GetLightPosition(cUnknownClass *ULight,Vect3f *pos);
	virtual int SetLightSize(cUnknownClass *ULight,float radius);
	virtual int GetLightSize(cUnknownClass *ULight,float radius);
	virtual int SetLightColor(cUnknownClass *ULight,const sColor4f *ambient=0,const sColor4f *diffuse=0,const sColor4f *illumination=0);
	virtual int GetLightColor(cUnknownClass *ULight,sColor4f *ambient=0,sColor4f *diffuse=0,sColor4f *illumination=0);
	virtual cUnknownClass* GetLight(int id);
	virtual int ReleaseLight(cUnknownClass *ULight);// ������� �������� ��������
	// ������� ��� ������ �� �������
	virtual cUnknownClass* CreateTangentTrail(const Vect3f *pos,const sColor4f *c1,const Vect3f *dpos,const sColor4f *c2,float Time=10000.f,float dAnimTime=1.e30f,float Width=2.f);
	virtual int ReleaseTangentTrail(cUnknownClass* UTangentTrail);
	// �������� ����, �� ���������� � ������� fname, number - ����� ����
	virtual cUnknownClass* CreateWorld(const std::filesystem::path &path,int number=0,int track=0,int LoadTerra=0);
	virtual int ReleaseWorld(cUnknownClass *UWorld);
	virtual int GetPolygonMapAttribute(int x1,int y1,int x2,int y2,int x3,int y3);
	virtual cUnknownClass* FindObject(char *name); // ����� ������� �� ���� �� ����� �������
	virtual cUnknownClass* FindObjectByFileName(char *fname); // ����� ������� �� ���� �� ����� ����� �������
	virtual cUnknownClass* GetMeshLibrary(cUnknownClass *UScene=0);
	virtual cUnknownClass* GetCameraList(cUnknownClass *UScene=0);
	virtual cUnknownClass* GetTileWater(cUnknownClass *UScene=0);
	virtual cUnknownClass* GetTileMap(cUnknownClass *UScene=0);
	virtual cUnknownClass* GetM3D(cUnknownClass *UScene=0);
	virtual cUnknownClass* GetO3D(cUnknownClass *UScene=0);
	virtual int SetWorldPolygonFirst();
	virtual int GetWorldPolygonNext(Vect3f &v1,Vect3f &v2,Vect3f &v3,int &attribute);

	inline cScene* GetActiveScene()					{ return Scene; }
private:
	int								CurrentTime,PreviousTime;
	cUnkClassDynArrayPointer		SceneArray;		// ������� �����
	cScene							*Scene;			// ��������� �� ������� �����
	
	inline int GetNumberScene()						{ return SceneArray.length(); }
	inline cScene*& GetScene(int number)			{ return (cScene*&)SceneArray[number]; }
	void ReleaseType(unsigned int Type);
};

#endif //__VISGENERIC_H__