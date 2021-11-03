#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include "Math3d.h"
#include "Object.h"
#include "Object3d.h"

class cMeshLibrary
{
	cList	*MeshList;
	cList	*Window;
public:
	cMeshLibrary()											{ MeshList=new cList(); Window=0; }
	~cMeshLibrary()											{ Release(0); }
	cMesh* Load3ds(char *fname,char *TexturePath,unsigned int Type=0,float SizeObject=0);
	cMesh* Loadm3d(char *fname,char *TexturePath,unsigned int Type=0,float SizeObject=0);
	cMesh* LoadMorph(unsigned int Type,int NumberMorph,float *time,char **name3dsMorph,char *FilePath,char *TexturePath);
	cMesh* Get(char *fname,char *TexturePath,unsigned int Type=0,float SizeObject=MAX_SIZE_OBJECT);
	cMesh* Get(unsigned int Type);
	void   Release(unsigned int Type=M3D_STATIC);
	inline cMesh* First()									{ Window=MeshList->next; if(Window) return Window->Mesh; return 0; }
	inline cMesh* Prev()									{ if(Window->prev==MeshList) return 0; Window=Window->prev; if(Window->prev==0) return 0; return Window->Mesh; }
	inline cMesh* Next()									{ if(Window==0) return 0; if((Window=Window->next)==0) return 0; return Window->Mesh; }
	cMesh* Find(unsigned int Type);
private:
	cMesh* CopyObject(cMesh *Mesh,unsigned int Type);
	cMesh* AddMesh(char *NameMesh);
	cList* Attach(cMesh *Mesh);
	cMesh* Find(char *fname);
	void Detach(cMesh *Mesh);
	void Delete(cMesh *Mesh);
};

#define STATIC_NUMBER										(1<<31)
#define TRACK_NUMBER0										(1<<0)
#define TRACK_NUMBER1										(1<<1)
#define TRACK_NUMBER2										(1<<2)
#define TRACK_NUMBER3										(1<<3)
#define TRACK_NUMBER4										(1<<4)
#define TRACK_NUMBER5										(1<<5)
#define TRACK_NUMBER6										(1<<6)
#define TRACK_NUMBER7										(1<<7)
#define TRACK_NUMBER8										(1<<8)
#define TRACK_NUMBER9										(1<<9)
#define TRACK_NUMBER10										(1<<10)

typedef cBaseList <cMemBuffer> cMemBufferList;
typedef cBaseStack <cMemBuffer,cMemBufferList> cShadeDispatcher;

class cM3D : cShadeDispatcher
{
	cList	*MeshList;
	cList	*Window;
	unsigned prev_global_time;
public:
	unsigned int		NumberID;
	cM3D()													{ MeshList=new cList(); NumberID=0; Window=0; prev_global_time = 0; }
	~cM3D()													{ Release(0); }
	// ��������
	void LoadLib(const char *fname);
	void LoadMap(char *fname,int LoadNumberTrack=0xFFFFFFFF);
	void SaveMap(char *fname,char Version='O');
	// �����
	void TestVisible(cUnknownClass *UCameraList);
	void Draw(cUnknownClass *UCameraList);
	// �������� �������� � �.�.
	cMesh* CreateObject(unsigned int *vType,int number);	// Constructor objects
	cMesh* CreateObject(char *fname,char *TexturePath,unsigned int Type=0,float SizeObject=MAX_SIZE_OBJECT);
	cMesh* CreateObject(unsigned int Type,float x,float y,float z,float ax,float ay,float az);
	cMesh* CreateObject(unsigned int Type);
	cMesh* CreateObject(cMesh *Mesh)														{ return CreateObject(Mesh->Type); }
	cMesh* RebuildObject(cMesh *Mesh,unsigned int OldType,unsigned int NewType);
	cMesh* FindObject(char* name);
	cMesh* FindNextObject(char* name);
	cMesh* FindObjectByFileName(char* fname);
	void dSetPosition(cMesh *Mesh,float dx,float dy,float dz,float dax,float day,float daz);
	void SetPosition(cMesh *Mesh,float x,float y,float z,float ax,float ay,float az);
	inline void SetPosition(cMesh *Mesh,const class MatXf &Matrix)							{ Mesh->SetPosition(Matrix); }
	inline void SetScale(cMesh *Mesh,float sx,float sy,float sz)							{ if(Mesh) Mesh->SetScale(Vect3f(sx,sy,sz)); }
	inline void dSetScale(cMesh *Mesh,float dsx,float dsy,float dsz)						{ if(Mesh) Mesh->dSetScale(Vect3f(dsx,dsy,dsz)); }
	inline void ClearMatrix(cMesh *Mesh)													{ if(Mesh) Mesh->LocalMatrix.NewMatrix(); }
	inline void Visible(cMesh *Mesh,char Visible)											{ if(Mesh) Mesh->SetVisible(Visible); }
	inline cMesh* operator[] (unsigned int ID)												{ for(cList *start=MeshList->next; start; start=start->next) if(start->Mesh->ID==ID) return start->Mesh; return 0; }
	void SetAttribute(unsigned int Type,int attribute);
	cMesh* FindUnique(unsigned int unique);

	cList* AttachEnd(cMesh *Mesh);
	cList* Attach(cMesh *Mesh);	// ����������� ������� � ������
	void   Detach(cMesh *Mesh);	// ������������ ������� �� ������
	void   Delete(cMesh *Mesh); // �������� ������� � ������
	void   Release(unsigned int Type=M3D_STATIC); // �������� �������� �� ������ �� ����
#ifdef _MECHOSOMA_
	void  SetArcane(void (*FunctionSetArcane)(mchArcaneData*));
#endif
	void DrawMask(cCamera *Camera,unsigned int Mask);
	void DrawReflection(cUnknownClass *UScene);
	void DrawCircleDynamic(Vect3f &pos,sColor4f &color,float radius);
	void RestoreShadeDynamic();
	void ShadeDynamic(cUnknownClass *UCameraList,cUnknownClass *UTileMap);
	
	inline cMesh* First()									{ Window=MeshList->next; if(Window) return Window->Mesh; return 0; }
	inline cMesh* Prev()									{ if(Window->prev==MeshList) return 0; Window=Window->prev; if(Window->prev==0) return 0; return Window->Mesh; }
	inline cMesh* Next()									{ if(Window==0) return 0; if((Window=Window->next)==0) return 0; return Window->Mesh; }

	void Animate(cUnknownClass *UManagerCamera,int CurrentTime,int PreviousTime);

private:
	inline cMesh* FindType(unsigned int Type)				{ for(cList *start=MeshList->next; start; start=start->next) if(start->Mesh->Type==Type) return start->Mesh; return 0; }
	inline cMesh* FindID(unsigned int ID)					{ for(cList *start=MeshList->next; start; start=start->next) if(start->Mesh->ID==ID) return start->Mesh; return 0; }
	void RestoreShadeDynamic(cMemBuffer *MemBuffer);
#ifdef _MECHOSOMA_
	void TraceStaticObjectMechos(cUnknownClass *UCameraList);
#endif
};

extern char *dirM3D,*dirM3Dtexture;

typedef cBaseList <cOmni> cOmniList;
typedef cBaseDispatcher <cOmni,cOmniList> cOmniDispatcher;

extern int NumberTrackShowMask;
extern char OmniShowID;

class cO3D : public cOmniDispatcher
{
public:
	cO3D()																	{ }
	~cO3D()																	{ Release(); }

	void Release(int type=0xFFFFFFFF);
	void SaveMap(char *fname,char Version='b');
	void LoadMap(char *fname);
	cOmni* CreateOmni(float xOmni=0.0f,float yOmni=0.0f,float zOmni=0.0f,float radius=0.0f,float r=0.0f,float g=0.0f,float b=0.0f);
	void Draw(cUnknownClass *UCameraList);
	void DrawID(cUnknownClass *UCameraList);
	inline void SetPosition(cOmni *Omni,float x,float y,float z)			{ if(Omni) Omni->SetPosition(Vect3f(x,y,z)); }
	inline void Translate(cOmni *Omni,float dx,float dy,float dz)			{ if(Omni) Omni->Translate(Vect3f(dx,dy,dz)); }
	inline void SetOmni(cOmni *Omni,float radius,float r,float g,float b)	{ if(Omni) { Omni->SetRadius(radius); Omni->SetColor(sColor4f(r,g,b,1.f)); } }
	inline void SetVisible(cOmni *Omni,char Visible)						{ if(Omni) Omni->SetVisible(Visible); }
	inline cOmni* operator[] (unsigned int ID)								{ return FindID(ID); }
private:
//	inline cOmni* FindID(unsigned int ID)									{ for(cOmniList *start=BaseList;start;start=start->next) if(start->Base->ID==ID) return start->Base; return 0; }
};

extern cMeshLibrary	*MeshLibrary;

#endif // __DISPATCHER_H__

