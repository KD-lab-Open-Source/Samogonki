#include "VisGeneric.h"
#include "IncTerra.h"
#include "Dispatcher.h"
#include "PolyMgr.h"
#include "TileMap.h"
#include "BaseLight.h"

cUnknownClass* cVisGeneric::CreateWorld(const std::string &path,int number,int track,int LoadTerra)
{
	if(LoadTerra)
	{
		const char* WorldPrm = "worlds.prm";
		vMap_Prepare(WorldPrm,number);
		vMap_load3Buf(0,0,track);
	}
	assert(GetActiveScene());
	Scene->LoadWorld(path,number,track);
	return GetActiveScene();
}
int cVisGeneric::ReleaseWorld(cUnknownClass *UWorld)
{
	vMap_Delete();
	assert(GetActiveScene());
	GetActiveScene()->ReleaseWorld();
	return 0;
}
cUnknownClass* cVisGeneric::FindObject(const char *name)
{ // поиск объекта на миру по имени объекта
	assert(GetActiveScene());
	return (cUnknownClass*) GetActiveScene()->GetM3D()->FindObject(name);
}
cUnknownClass* cVisGeneric::GetTileWater(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetTileWater();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetTileWater();
}
cUnknownClass* cVisGeneric::GetTileMap(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetTileMap();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetTileMap();
}
cUnknownClass* cVisGeneric::GetCameraList(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetCameraList();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetCameraList();
}
cUnknownClass* cVisGeneric::GetMeshLibrary(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetLibMesh();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetLibMesh();
}
cUnknownClass* cVisGeneric::GetM3D(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetM3D();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetM3D();
}
cUnknownClass* cVisGeneric::GetO3D(cUnknownClass *UScene)
{
	if(UScene)
		return (cUnknownClass*)((cScene*)UScene)->GetO3D();
	assert(GetActiveScene());
	return (cUnknownClass*)GetActiveScene()->GetO3D();
}
int cVisGeneric::GetPolygonMapAttribute(int x1,int y1,int x2,int y2,int x3,int y3)
{
	return P3D->GetAttribute(x1,y1,x2,y2,x3,y3);
}
int cVisGeneric::SetWorldPolygonFirst()
{
	if((GetActiveScene()==0)||(GetActiveScene()->GetTileMap()==0)) return -1;
	GetActiveScene()->GetTileMap()->SetPolygonFirst();
	return 0;
}
int cVisGeneric::GetWorldPolygonNext(Vect3f &v1,Vect3f &v2,Vect3f &v3,int &attribute)
{
	if((GetActiveScene()==0)||(GetActiveScene()->GetTileMap()==0)) return -1;
	return GetActiveScene()->GetTileMap()->GetPolygonNext(v1,v2,v3,attribute);
}

cUnknownClass* cVisGeneric::CreateLight(int id)
{
	if(GetActiveScene()==0) return 0;
	cOmniLight *OmniLight=(cOmniLight*)GetActiveScene()->GetBaseObjectMgr()->New(BASEOBJECT_TYPE_BASELIGHT_OMNI,BASEOBJECT_KIND_VOLUME);
	return (cUnknownClass*)OmniLight;
}
int cVisGeneric::SetLightPosition(cUnknownClass *ULight,const Vect3f *pos)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	OmniLight->Pos()=*pos;
	return 0;
}
int cVisGeneric::GetLightPosition(cUnknownClass *ULight,Vect3f *pos)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	*pos=OmniLight->Pos();
	return 0;
}
int cVisGeneric::SetLightSize(cUnknownClass *ULight,float radius)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	OmniLight->Radius()=radius;
	return 0;
}
int cVisGeneric::GetLightSize(cUnknownClass *ULight,float radius)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	radius=OmniLight->Radius();
	return 0;
}
int cVisGeneric::SetLightColor(cUnknownClass *ULight,const sColor4f *ambient,const sColor4f *diffuse,const sColor4f *illumination)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	if(ambient) OmniLight->Ambient=*ambient;
	if(diffuse) OmniLight->Diffuse=*diffuse;
	if(illumination) OmniLight->Illumination=*illumination;
	return 0;
}
int cVisGeneric::GetLightColor(cUnknownClass *ULight,sColor4f *ambient,sColor4f *diffuse,sColor4f *illumination)
{
	cOmniLight *OmniLight=(cOmniLight*)ULight;
	if(ambient) *ambient=OmniLight->Ambient;
	if(diffuse) *diffuse=OmniLight->Diffuse;
	if(illumination) *illumination=OmniLight->Illumination;
	return 0;
}
cUnknownClass* cVisGeneric::GetLight(int id)
{
	if(GetActiveScene()==0) return 0;
	if((!GetActiveScene()->GetBaseObjectMgr())||(!GetActiveScene()->GetBaseObjectMgr()->BaseList)) { return 0; }
	for(cBaseObjectList *start=GetActiveScene()->GetBaseObjectMgr()->BaseList->next;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_BASELIGHT))
			if(((cBaseLight*)start->Base)->id==id)
				return (cUnknownClass*)start->Base;
	return 0;
}
int cVisGeneric::ReleaseLight(cUnknownClass *ULight)
{
	if(GetActiveScene()==0) return -1;
	if((!GetActiveScene()->GetBaseObjectMgr())||(!GetActiveScene()->GetBaseObjectMgr()->BaseList)) { return 0; }
	for(cBaseObjectList *start=GetActiveScene()->GetBaseObjectMgr()->BaseList->next;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_BASELIGHT))
			if(((cUnknownClass*)start->Base)==ULight)
			{
				start->Base->Type|=BASEOBJECT_TYPE_DELETE;
				break;
			}
	return 0;
}
