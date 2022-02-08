#ifndef __SCENE_H__
#define __SCENE_H__

#include <filesystem>

#include "UMATH.H"
#include "UnkLibrary.h"

class cM3D;
class cO3D;
class cSun;
class cLight;
class cCamera;
class cTileMap;
class cTileWater;
class cWorldPolyGrid;
class cManagerBaseObject;
class cTextureBuffer;
class cMeshLibrary;

class cScene : public cUnknownClass
{
public:
	cScene();
	~cScene();
	void Release();
	void Animate(float CurrentTime,float PreviousTime);
	void Draw(int number=0xFFFFFFFF);		// ��������� ��������� ����� ����
	void PreDraw(int number=0xFFFFFFFF);	// ���������� � ����������, ����������� ���������, ����������� �������� � �.�. 
	void PostDraw(int number=0xFFFFFFFF);	// �������� ����� ����������, ������������ ��������� ������� � �.�. 
	void GetOmniLight(const Vect3f &pos,sColor4f &diffuse,sColor4f &illumination); // ���������� ������������ ����� � �����
	// ������ � ����������
	inline cM3D*&			GetM3D()							{ return M3D; }
	inline cO3D*&			GetO3D()							{ return O3D; }
	inline cLight*&			GetLight()							{ return Light; }
	inline cTileMap*&		GetTileMap()						{ return tMap; }
	inline cTileWater*&		GetTileWater()						{ return tWater; }
	inline cWorldPolyGrid*&	GetAir()							{ return Air; }
	inline cWorldPolyGrid*&	GetCloud()							{ return Cloud; }
	inline cManagerBaseObject*& GetBaseObjectMgr()				{ return BaseObjectMgr; }
	inline cTextureBuffer*& GetLibTex()							{ return TextureLibrary; }
	inline cMeshLibrary*&	GetLibMesh()						{ return MeshLibrary; }
	inline cSun*&			GetSun(int number)					{ assert(0<=number&&number<SunArray->length()); return (cSun*&)(*SunArray)[number]; }
	inline cUnkClassDynArrayPointer*& GetSunArray()				{ return SunArray; }
	inline int GetNumberSun()									{ return SunArray->length(); }
	inline int& xWorldSize()									{ return xSize; }
	inline int& yWorldSize()									{ return ySize; }
	inline int xWorldCycl(int x)								{ return x&(xSize-1); }
	inline int yWorldCycl(int y)								{ return y&(ySize-1); }
	inline int &RadiusPlanet()									{ return RadiusShare; }
	inline sColor4f& GetAmbientObjectAll()						{ return AmbientObjectAll; }
	inline sColor4f& GetAmbientObjectMechos()					{ return AmbientObjectMechos; }
	inline sColor4f& GetDiffuseLightingMax()					{ return DiffuseLightingMax; }
	inline sColor4f& GetAmbientLightingMax()					{ return AmbientLightingMax; }
	// ��� ������ �� ������� �����
	inline int GetNumberCamera()								{ return CameraArray->length(); }
	inline cCamera*& GetCamera(int number)						{ assert(0<=number&&number<CameraArray->length()); return (cCamera*&)(*CameraArray)[number]; }
	inline cUnknownClass*& GetCameraList()						{ return (cUnknownClass*&)CameraArray; }
	inline void Attach(cCamera *Camera)							{ for(int i=0;i<GetNumberCamera();i++) if(GetCamera(i)==Camera) return; CameraArray->Attach((cUnknownClass*)Camera); }
	inline void Detach(cCamera *Camera)							{ CameraArray->Detach((cUnknownClass*)Camera); }
	// ��� ������ � ����������� �������
	void Attach(cTextureBuffer *pTextureLibrary)				{ TextureLibrary=pTextureLibrary; }
	void Detach(cTextureBuffer *pTextureLibrary)				{ if(TextureLibrary) TextureLibrary=0; }
	// ��� ������ � ����������� ��������
	void Attach(cMeshLibrary *pMeshLibrary)						{ MeshLibrary=pMeshLibrary; }
	void Detach(cMeshLibrary *pMeshLibrary)						{ if(MeshLibrary) MeshLibrary=0; }
	// �������������
	void LoadWorld(const std::filesystem::path &path,int number=0,int track=0);		// �������� ������� ������������ ������������� ����
	void ReleaseWorld();										// �������� ������� ������������ ������������� ����
	void AddOmniLight(const Vect3f &pos,float radius,const sColor4f &ambient,const sColor4f &diffuse,const sColor4f &illumination);// ��������� �������� ��������
private:
	cTextureBuffer					*TextureLibrary;		// ���������� �������
	cMeshLibrary					*MeshLibrary;			// ���������� 3d-��������
	cUnkClassDynArrayPointer		*CameraArray;			// ������ ���� ��������� �����
	cM3D							*M3D;					// ��������� 3d-��������
	cO3D							*O3D;					// ��������� ���������� ������������ ���������� ����� 
	cManagerBaseObject				*BaseObjectMgr;			// ����� ����������� ��������
	cLight							*Light;					// ���������� �������� ���������
	// �������� ����
	int								xSize;					// x - ������ ����
	int								ySize;					// y - ������ ����
	int								RadiusShare;			// ������ ������� ����
	cTileMap						*tMap;					// ��������� �� cTileMap
	cTileWater						*tWater;				// ��������� �� cTileWater
	cWorldPolyGrid					*Air;					// ��������� �� �WorldPolyGrid
	cWorldPolyGrid					*Cloud;					// ��������� �� �WorldPolyGrid
	cUnkClassDynArrayPointer		*SunArray;				// ������ �������� ������
	sColor4f						AmbientObjectAll,AmbientObjectMechos,DiffuseLightingMax,AmbientLightingMax;

	cWorldPolyGrid* LoadAir(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,char *TextureName,char *TexturePath,int AirHeight);
	cWorldPolyGrid* LoadCloud(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,char *TextureName,char *OpacityName,char *TexturePath,int CloudHeight);
	void LoadWorldLight(int number,int track,sColor4f &ColorAmbientAir,char *TextureSun,char *TextureMoon,char *TexturePath);
	void AddAirLight(Vect3f &Pos,sColor4f &Ambient,sColor4f &Diffuse,sColor4f &Illumination,float RadiusWorldSun,float RadiusLight,float RadiusSun,char *TextureName,char *TexturePath);
};

#endif //__SCENE_H__