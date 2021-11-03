#include "Scene.h"
#include "cString.h"
#include "TileMap.h"
#include "TileWater.h"
#include "BasePolyGrid.h"
#include "PolyMgr.h"
#include "TexMgr.h"
#include "RenderDevice.h"
// ��� ������ �� ���������
#include "WorldSetup.h"
#include "BaseDefine.h"
#include "aci_parser.h"
// ������ ���������
#include "Dispatcher.h"
#include "BaseTrail.h"
#include "BaseLight.h"
#ifdef ENABLE_TIMERS  // _MECHOSOMA_
#include "mechosoma.h"
#include "Statistics.h"
#else
#define stop_timer(a,b)
#define start_timer(a,b)
#endif//_MECHOSOMA_

void SetTextureNameTrack(char *name,char *fname,int track)
{
	if(!name) { fname[0]=0; return; }
	char *tmp=name;
	for(int CountSpace=-1;(CountSpace<track)&&(CountSpace<9)&&((tmp-name)<strlen(name));CountSpace++)
	{
		int count=0;
		while((*tmp)==9||(*tmp)==32)
			tmp++;
		while((*tmp)!=9&&(*tmp)!=32)
			fname[count++]=*(tmp++);
		fname[count]=0;
	}
}

cScene::cScene() : cUnknownClass(KIND_SCENE)
{
	GetLibTex()=0;
	GetLibMesh()=0;
	M3D=new cM3D;
	O3D=new cO3D;
	Light=new cLight;
	xSize=ySize=2048;
	RadiusPlanet()=1024;
	tMap=0;
	tWater=0;
	Air=0;
	Cloud=0;
	BaseObjectMgr=new cManagerBaseObject;
	CameraArray=new cUnkClassDynArrayPointer(KIND_ARRAYCAMERA);
	SunArray=new cUnkClassDynArrayPointer(KIND_ARRAYSUN);
	AmbientObjectAll.set(0.0f,0.0f,0.0f,1.f);
	AmbientObjectMechos.set(0.0f,0.0f,0.0f,1.f);
	Light->SetAmbient(sColor4f(0.1f,0.1f,0.1f,1.f));
	Light->SetDiffuse(sColor4f(0.4f,0.4f,0.4f,1.f));
	Light->SetRotate(Vect3f(0,60,0));
//	Light->SetRotate(Vect3f(0,-60,0));
	InitArrayShare(RadiusPlanet());
	AmbientLightingMax.set(0.0f,0.0f,0.0f,1.f);
	DiffuseLightingMax.set(0.6f,0.6f,0.6f,1.f);
#ifdef _MECHOSOMA_
	extern char* mch_mainINI;
	extern char* getIniKey(char* fname,char* section,char* key);
	char *p = getIniKey(mch_mainINI,"m3d","MechosLighting");
	XBuffer *XBuf = new XBuffer(p,strlen(p));
	*XBuf >= AmbientObjectMechos.r >= AmbientObjectMechos.g >= AmbientObjectMechos.b;
	delete XBuf;
	AmbientLightingMax.set(0.0f,0.0f,0.0f,1.f);
	DiffuseLightingMax.set(1.5f,1.5f,1.5f,1.f);
#endif
}
cScene::~cScene()
{
	UNKNOWN_DESTRUCTOR; 
	assert(GetLibTex()==0);
	if(BaseObjectMgr) delete BaseObjectMgr; BaseObjectMgr=0;
	if(M3D) delete M3D; M3D=0;
	if(O3D) delete O3D; O3D=0;
	if(Light) delete Light; Light=0;
	if(tMap) delete tMap; tMap=0;
	if(tWater) delete tWater; tWater=0;
	if(Cloud) delete Cloud; Cloud=0;
	if(Air) delete Air; Air=0;
	if(CameraArray) delete CameraArray; CameraArray=0;
	if(SunArray) delete SunArray; SunArray=0;
}
void cScene::Release()
{
	assert(GetLibTex()==0);
	if(BaseObjectMgr) delete BaseObjectMgr; BaseObjectMgr=0;
	if(M3D) delete M3D; M3D=0;
	if(O3D) delete O3D; O3D=0;
	if(Light) delete Light; Light=0;
	if(tMap) delete tMap; tMap=0;
	if(tWater) delete tWater; tWater=0;
	if(Cloud) delete Cloud; Cloud=0;
	if(Air) delete Air; Air=0;
	if(SunArray)
		for(int i=0;i<GetNumberSun();i++)
		{
			delete GetSun(i);
			GetSun(i)=0;
		}
	if(SunArray) delete SunArray; SunArray=0;
	if(CameraArray)
		for(int i=0;i<GetNumberCamera();i++)
		{
			delete GetCamera(i);
			GetCamera(i)=0;
		}
	if(CameraArray) delete CameraArray; CameraArray=0;
}
void cScene::ReleaseWorld()
{
	if(M3D) M3D->Release();
	if(O3D) O3D->Release();
	if(GetTileMap()) delete GetTileMap(); GetTileMap()=0;
	if(GetTileWater()) delete GetTileWater(); GetTileWater()=0;
	if(GetCloud()) delete GetCloud(); GetCloud()=0;
	if(GetAir()) delete GetAir(); GetAir()=0;
	if(SunArray)
	{
		for(int i=0;i<GetNumberSun();i++)
		{
			delete GetSun(i);
			GetSun(i)=0;
		}
		SunArray->Delete();
	}
	if(BaseObjectMgr) BaseObjectMgr->Release();
	GetLibTex()->Release(1); 
}
void cScene::LoadWorld(char *fname,int number,int track)
{
	ReleaseWorld();
	UNKNOWN_ASSERT((fname)&&strlen(fname));
	scrDataBlock *p,*root=loadScript(fname);

	// ������������� ��������� ������
	RadiusPlanet()=1000;
	int DefaultAttribute=0,AirHeight=0,CloudHeight=0;
	cString DefaultPathTextureWater;
	cString DefaultPathObject3dMap,DefaultPathTextureMap;
	cString DefaultTextureSun,DefaultTextureMoon;
	cString DefaultFileNameTileMap,DefaultFileNameObjectMap,DefaultFileNameOmniMap;
	p=root->find(WORLD_DEFAULT_OBJECT3D_PATH);
	if(p) DefaultPathObject3dMap=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_TEXTURE_PATH);
	if(p) DefaultPathTextureMap=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_WATER_PATH);
	if(p) DefaultPathTextureWater=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_ATTRIBUTE);
	if(p) DefaultAttribute=*p->i_dataPtr;
	p=root->find(WORLD_DEFAULT_AIR_HEIGHT);
	if(p) AirHeight=*p->i_dataPtr;
	p=root->find(WORLD_DEFAULT_CLOUD_HEIGHT);
	if(p) CloudHeight=*p->i_dataPtr;
	p=root->find(WORLD_RADIUS_SHARE);
	if(p) RadiusPlanet()=*p->i_dataPtr;
	p=root->find(WORLD_DEFAULT_TEXTURE_SUN);
	if(p) DefaultTextureSun=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_TEXTURE_MOON);
	if(p) DefaultTextureMoon=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_FILE_TILEMAP);
	if(p) DefaultFileNameTileMap=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_FILE_OBJECTMAP);
	if(p) DefaultFileNameObjectMap=(char*)p->c_dataPtr;
	p=root->find(WORLD_DEFAULT_FILE_OMNIMAP);
	if(p) DefaultFileNameOmniMap=(char*)p->c_dataPtr;
	if((!DefaultPathObject3dMap)||(!DefaultPathTextureMap)||(!DefaultPathTextureWater)) 
		ErrH.Abort("cScene::LoadWorld()\r\nWorld path not found\r\n");

	// �������� ���������� ����
	p=root->nextLevel->first();
	while(p)
		if((p->ID==WORLD_STRUCT)&&(number==*p->i_dataPtr)) break;
		else p=p->next;
	// ������������� �� ���������
	cString FileNameTileMap=DefaultFileNameTileMap,
		FileNameObjectMap=DefaultFileNameObjectMap,
		FileNameOmniMap=DefaultFileNameOmniMap,
		PathObject3dMap=DefaultPathObject3dMap, 
		PathTextureMap=DefaultPathTextureMap;
	int attribute=DefaultAttribute;
	int WaterTileSize=32,WaterEnvMap=0;
	double WaterScale=1,WaterVelocityX=0,WaterVelocityY=0;
	cString FileNameTextureWater;
	int AirTileSize=256;
	double AirScale=1,AirVelocityX=0,AirVelocityY=0;
	sColor4f ColorAmbientAir(0.5f,0.5f,0.5f,1.f);
	cString FileNameTextureAir;
	int CloudTileSize=256;
	double CloudScale=1,CloudVelocityX=0,CloudVelocityY=0;
	cString FileNameTextureCloud,FileNameOpacityCloud;
	char fName[256];
	if(p)
	{
		scrDataBlock *pl=p->nextLevel->first();
		while(pl)
		{
			switch(pl->ID)
			{
				case WORLD_OBJECT3D_PATH:
					PathObject3dMap=(char*)pl->c_dataPtr;
					break;
				case WORLD_TEXTURE_PATH:
					PathTextureMap=(char*)pl->c_dataPtr;
					break;
				case WORLD_FILE_TILEMAP:
					FileNameTileMap=(char*)pl->c_dataPtr;
					break;
				case WORLD_FILE_OBJECTMAP:
					FileNameObjectMap=(char*)pl->c_dataPtr;
					break;
				case WORLD_FILE_OMNIMAP:
					FileNameOmniMap=(char*)pl->c_dataPtr;
					break;
				case WORLD_ATTRIBUTE:
					attribute=*pl->i_dataPtr;
					break;
				
				case WORLD_WATER_TILESIZE:
					WaterTileSize=*pl->i_dataPtr;
					break;
				case WORLD_WATER_TEXTURE:
					SetTextureNameTrack((char*)pl->c_dataPtr,fName,track);
					FileNameTextureWater=fName;
					break;
				case WORLD_WATER_ENVMAP:
					WaterEnvMap=*pl->i_dataPtr;
					break;
				case WORLD_WATER_TEXTURE_SCALE:
					WaterScale=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_WATER_TEXTURE_VELOCITYX:
					WaterVelocityX=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_WATER_TEXTURE_VELOCITYY:
					WaterVelocityY=(float)(pl->d_dataPtr[track]);
					break;

				case WORLD_AIR_TEXTURE:
					SetTextureNameTrack((char*)pl->c_dataPtr,fName,track);
					FileNameTextureAir=fName;
					break;
				case WORLD_AIR_TILESIZE:
					AirTileSize=*pl->i_dataPtr;
					break;
				case WORLD_AIR_TEXTURE_SCALE:
					AirScale=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_AIR_TEXTURE_VELOCITYX:
					AirVelocityX=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_AIR_TEXTURE_VELOCITYY:
					AirVelocityY=(float)(pl->d_dataPtr[track]);
					break;
				
				case WORLD_CLOUD_TEXTURE:
					SetTextureNameTrack((char*)pl->c_dataPtr,fName,track);
					FileNameTextureCloud=fName;
					break;
				case WORLD_CLOUD_OPACITY:
					SetTextureNameTrack((char*)pl->c_dataPtr,fName,track);
					FileNameOpacityCloud=fName;
					break;
				case WORLD_CLOUD_TILESIZE:
					CloudTileSize=*pl->i_dataPtr;
					break;
				case WORLD_CLOUD_TEXTURE_SCALE:
					CloudScale=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_CLOUD_TEXTURE_VELOCITYX:
					CloudVelocityX=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_CLOUD_TEXTURE_VELOCITYY:
					CloudVelocityY=(float)(pl->d_dataPtr[track]);
					break;
				case WORLD_COLOR_AMBIENT_AIR:
					ColorAmbientAir.set(pl->d_dataPtr[0],pl->d_dataPtr[1],pl->d_dataPtr[2],1.f);
					break;
				case WORLD_REFLECTION_SURFACE:
					extern int GlobalAlfaReflectionSurface;
					GlobalAlfaReflectionSurface=round((float)*pl->d_dataPtr*255);
					break;
				case WORLD_COLOR_AMBIENT_OBJECT:
					AmbientObjectAll.b=AmbientObjectAll.g=AmbientObjectAll.r=(float)(pl->d_dataPtr[track]);
					break;
			}
			pl=pl->next;
		}
	}
	delete root;
	// ������������� ����
#ifdef _MECHOSOMA_
	extern char* mch_mainINI;
	extern char* getIniKey(char* fname,char* section,char* key);
	RadiusPlanet()=atoi(getIniKey(mch_mainINI,"m3d","RadiusShare"));
#endif
	InitArrayShare(RadiusPlanet()=RadiusPlanet());
	xWorldSize()=2048; yWorldSize()=2048;
	// �������� ������������� ����� ����
	if(GetTileMap()) delete GetTileMap(); GetTileMap()=0;
	GetTileMap()=new cTileMap;
	GetTileMap()->Load(vMap_GetTargetName(FileNameTileMap));
	// �������� ���� �� ����
	if(GetTileWater()) delete GetTileWater(); GetTileWater()=0;
	GetTileWater()=new cTileWater;
	GetTileWater()->Attach(GetTileMap());
	GetTileWater()->SetLevelWater(vMap_LevelH2O());
	GetTileWater()->Load(WaterTileSize,WaterTileSize,WaterScale,WaterScale,WaterVelocityX,WaterVelocityY,FileNameTextureWater,DefaultPathTextureWater,WaterEnvMap);
	// �������� ������� �� ����
	if(GetAir()) delete GetAir(); GetAir()=0;
	GetAir()=LoadAir(AirTileSize,AirTileSize,AirScale,AirScale,AirVelocityX,AirVelocityY,FileNameTextureAir,DefaultPathTextureWater,AirHeight);
	// �������� ������� �� ����
	if(GetCloud()) delete GetCloud(); GetCloud()=0;
	GetCloud()=LoadCloud(CloudTileSize,CloudTileSize,CloudScale,CloudScale,CloudVelocityX,CloudVelocityY,FileNameTextureCloud,FileNameOpacityCloud,DefaultPathTextureWater,CloudHeight);
	// �������� ���������� ����� ��� ���� (�� ����, ������)
	if(GetAir())
		for(int k=0;k<GetAir()->ysize*GetAir()->xsize;k++)
			GetAir()->pColor[k].Set(ColorAmbientAir.GetR(),ColorAmbientAir.GetG(),ColorAmbientAir.GetB());
	if(GetCloud())
		for(int k=0;k<GetCloud()->ysize*GetCloud()->xsize;k++)
			GetCloud()->pColor[k].Set(ColorAmbientAir.GetR(),ColorAmbientAir.GetG(),ColorAmbientAir.GetB());
	LoadWorldLight(number,track,ColorAmbientAir,DefaultTextureSun,DefaultTextureMoon,DefaultPathTextureWater);
	unsigned int track_mask=(1<<track)|(STATIC_NUMBER);
	M3D->LoadMap(vMap_GetTargetName(FileNameObjectMap),track_mask);
	O3D->LoadMap(vMap_GetTargetName(FileNameOmniMap));
}
void cScene::Animate(float CurrentTime,float PreviousTime)
{
	start_timer(cScene_Animate, STAT_M3D);
	float dTime=CurrentTime-PreviousTime;
	GetM3D()->Animate(GetCameraList(),CurrentTime,PreviousTime);
	// ��������
	if((GetAir())&&(GetCloud()))
	{
		GetAir()->uofs=fmod(GetAir()->uofs+GetAir()->du*dTime,1.f);
		GetAir()->vofs=fmod(GetAir()->vofs+GetAir()->dv*dTime,1.f);
		GetCloud()->uofs=fmod(GetCloud()->uofs+GetCloud()->du*dTime,1.f);
		GetCloud()->vofs=fmod(GetCloud()->vofs+GetCloud()->dv*dTime,1.f);
	}
	if(GetBaseObjectMgr()&&GetBaseObjectMgr()->BaseList)
	{
/*		for(cBaseObjectList *start=GetBaseObjectMgr()->BaseList;start;start=start->next) 
		{ // �������� ������
			assert(start->Base);
			if(start->Base->GetType(BASEOBJECT_TYPE_BASETRAIL_TANGENT)) 
			{
				cTangentTrail *TangentTrail=(cTangentTrail*)start->Base;
				TangentTrail->CurrentTime+=dTime;
			}
		}
*/
		//������� �� ������ ��������
		GetBaseObjectMgr()->Release(BASEOBJECT_TYPE_DELETE);
	}
	stop_timer(cScene_Animate, STAT_M3D);
}
void cScene::Draw(int number)
{
	if(number&0x0000000F)
	{	// ������ ���������� ������� 
		start_timer(cTileMap_DrawReflection, STAT_M3D);
		if(GET_RENDER_TUNING(RENDER_TUNING_TILEMAP)) 
			if(GetTileMap()) GetTileMap()->DrawReflection(GetCameraList());
		stop_timer(cTileMap_DrawReflection, STAT_M3D);
		start_timer(cM3D_DrawReflection, STAT_M3D);
		M3D->DrawReflection(this);
		P3D->GetRenderDevice(0)->GetIGraph3d()->NullClipRect();
		stop_timer(cM3D_DrawReflection, STAT_M3D);
	}
	if(number&0x000000F0)
	{	// ���������� �� ���������� ������� ����
		cWorldPolyGrid *pAir=GetAir(),*pCloud=GetCloud();
		start_timer(cTileMap_Draw, STAT_M3D);
		if(GET_RENDER_TUNING(RENDER_TUNING_TILEMAP)) 
			if(GetTileMap()) GetTileMap()->Draw(GetCameraList());
		stop_timer(cTileMap_Draw, STAT_M3D);
		start_timer(Cloud_Sky_Sun_Draw, STAT_M3D);
		if((pAir)&&(pCloud))
		{
			float AirSize=pAir->xstep*pAir->xsize;
			WorldSetScale(AirSize/xWorldSize());
			if(GET_RENDER_TUNING(RENDER_TUNING_SKY))
				P3D->Draw(GetCameraList(),pAir);
			if(GET_RENDER_TUNING(RENDER_TUNING_CLOUD))
				P3D->Draw(GetCameraList(),pCloud);
			if(GET_RENDER_TUNING(RENDER_TUNING_SUN))
				P3D->Draw(GetCameraList(),GetSunArray());
			WorldSetScale(1.f);
		}
		stop_timer(Cloud_Sky_Sun_Draw, STAT_M3D);
	}
	if(number&0x00000F00)
	{	// ������������ �� ���������� �������
		start_timer(cM3D_Draw, STAT_M3D);
		M3D->Draw(this); 
		P3D->GetRenderDevice(0)->GetIGraph3d()->NullClipRect();
		stop_timer(cM3D_Draw, STAT_M3D);
	}
	if(number&0x0000F000)
	{	// ���������� ���������� ������� ����
		start_timer(cTangentTrail_Draw, STAT_M3D);
		if(GetBaseObjectMgr()&&GetBaseObjectMgr()->BaseList)
			for(cBaseObjectList *start=GetBaseObjectMgr()->BaseList;start;start=start->next) 
			{
				assert(start->Base);
				if(start->Base->GetType(BASEOBJECT_TYPE_BASETRAIL_TANGENT)) 
				{
					cTangentTrail *TangentTrail=(cTangentTrail*)start->Base;
					P3D->Draw(GetCameraList(),TangentTrail);
				}
			}
		stop_timer(cTangentTrail_Draw, STAT_M3D);
		start_timer(cTileWater_Draw, STAT_M3D);
		if(GET_RENDER_TUNING(RENDER_TUNING_WATER)) 
			if(GetTileWater()) GetTileWater()->Draw(GetCameraList());
		stop_timer(cTileWater_Draw, STAT_M3D);
//		if(L3D) L3D->Draw(Camera);
		start_timer(cO3D_Draw, STAT_M3D);
		if(GET_RENDER_TUNING(RENDER_TUNING_OMNI))
			O3D->Draw(GetCameraList());
		stop_timer(cO3D_Draw, STAT_M3D);
	}
	if(number&0x00F00000) // ��������� ����� ������ ��� �����
		if(vMap_IsActive())
			for(int i=0;i<GetNumberCamera();i++)
			{
				assert(GetCamera(i)->GetKind(KIND_CAMERA));
				cCamera *Camera=(cCamera*)GetCamera(i);
				Vect3f origin;
				Camera->GetMatrix().invXformVect(Camera->GetMatrix().trans(),origin);
				origin.negate();
				int x=TERRA_XCYCL(round(origin.x)),y=TERRA_YCYCL(round(origin.y));
				if((vMap_GetAt(x,y)&TERRA_At_WATER)&&(vMap_LevelH2O()>origin.z))
					P3D->SetViewColor(Camera,sColor4f(vMap_H2OCR()/31.f,vMap_H2OCG()/63.f,vMap_H2OCB()/31.f,1-vMap_H2OA()/63.f),sColor4f(0,0,0,1));
			}
	if(number&0x000F0000)
		P3D->GetRenderDevice(0)->GetIGraph3d()->SetMaterial(MAT_NULL);
}
void cScene::PreDraw(int number)
{
	start_timer(cTileMap_RadiusShare, STAT_M3D);
	if(GetNumberCamera()==1)
	{ // ���������� ������� ������� � �������
		float HeightMin=512,HeightMax=1024+512,RadiusHeightMin=RadiusPlanet(),RadiusHeightMax=1025;
		assert(GetCamera(0)->GetKind(KIND_CAMERA));
		cCamera *Camera=(cCamera*)GetCamera(0);
		Vect3f origin;
		Camera->GetMatrix().invXformVect(Camera->GetMatrix().trans(),origin);
		origin.negate();
		if(origin.z<HeightMin) origin.z=HeightMin;
		if(origin.z>HeightMax) origin.z=HeightMax;
		float dh=origin.z-HeightMin,dr=RadiusHeightMax-RadiusHeightMin;
		int radius=round(RadiusHeightMin+dr*dh/(HeightMax-HeightMin));
		radius&=0xFFFFFFE0;
		if(GlobalWorldRadius!=radius)
		{
			GlobalWorldRadius=radius;
			InitArrayShare(GlobalWorldRadius);
		}
	}
	else
		GlobalWorldRadius=RadiusPlanet();
	stop_timer(cTileMap_RadiusShare, STAT_M3D);
	// ����� ��������� �������� � ����
	start_timer(cTileMap_TestVisible, STAT_M3D);
	if(number&0x000000F0)
		if(GetTileMap()) GetTileMap()->TestVisible(GetCameraList());
	stop_timer(cTileMap_TestVisible, STAT_M3D);
	start_timer(cM3D_TestVisible, STAT_M3D);
	if(number&0x00000F00)
		M3D->TestVisible(GetCameraList());
	stop_timer(cM3D_TestVisible, STAT_M3D);
	start_timer(cM3D_ShadeDynamic, STAT_M3D);
	if(number&0x000000F0)
		if(GetTileMap()) M3D->ShadeDynamic(GetCameraList(),GetTileMap()); // ���������� ������������ �����
	stop_timer(cM3D_ShadeDynamic, STAT_M3D);
	start_timer(cTileWater_PreDraw, STAT_M3D);
	if(number&0x0000F000)
		if(GetTileWater()) GetTileWater()->PreDraw(this);
	stop_timer(cTileWater_PreDraw, STAT_M3D);
	// ����������� �������
	start_timer(cTileMap_UnlockTexture, STAT_M3D);
	if(number&0x000000F0)
		if(GetTileMap()) GetTileMap()->UnlockTexture(GetCameraList());
	stop_timer(cTileMap_UnlockTexture, STAT_M3D);
}
void cScene::PostDraw(int number)
{ // �������� ����� ����������, ������������ ��������� ������� � �.�. 
	start_timer(cScene_PostDraw, STAT_M3D);
	if(GetTileMap()) M3D->RestoreShadeDynamic();
	stop_timer(cScene_PostDraw, STAT_M3D);
}
void cScene::GetOmniLight(const Vect3f &pos,sColor4f &diffuse,sColor4f &illumination)
{
	if((!GetBaseObjectMgr())||(!GetBaseObjectMgr()->BaseList)) { diffuse+=sColor4f(0,0,0,1); illumination+=sColor4f(0,0,0,1); return; }
	for(cBaseObjectList *start=GetBaseObjectMgr()->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_BASELIGHT))
		{
			float dist;
			cBaseLight *BaseLight=(cBaseLight*)start->Base;
			if(BaseLight->isVolume(pos,dist))
			{
				float f=(BaseLight->Radius()-dist)/BaseLight->Radius();
				diffuse+=BaseLight->Diffuse*f;
				illumination+=BaseLight->Diffuse*f;
//				illumination+=BaseLight->Illumination*f;
			}
		}
}
//////////////////////////////////////// PRIVATE ////////////////////////////////////////
cWorldPolyGrid* cScene::LoadAir(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,char *TextureName,char *TexturePath,int AirHeight)
{
	int xSky=2*((RadiusPlanet()+AirHeight)/xstep+3);
	int ySky=2*((RadiusPlanet()+AirHeight)/ystep+3);

	cWorldPolyGrid *WorldPolyGrid=new cWorldPolyGrid(BASEOBJECT_TYPE_BASEPOLYGRID_WORLD,BASEOBJECT_KIND_DRAW_EFFECT);
	WorldPolyGrid->New(xSky,ySky,xstep,ystep);
	WorldPolyGrid->SetPosition(0,0,AirHeight);
	WorldPolyGrid->u=uScale/WorldPolyGrid->xsize;
	WorldPolyGrid->v=vScale/WorldPolyGrid->ysize;
	WorldPolyGrid->du=duOfs; WorldPolyGrid->dv=dvOfs;
	WorldPolyGrid->uofs=WorldPolyGrid->vofs=0.f;
	if(TextureName&&TexturePath) WorldPolyGrid->SetTexture(TextureName,TexturePath);
	else WorldPolyGrid->SetTexture(0,0);
	for(int j=0;j<WorldPolyGrid->ysize;j++)
		for(int i=0;i<WorldPolyGrid->xsize;i++)
		{
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].r=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].g=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].b=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].a=255;
		}
	return WorldPolyGrid;
}
cWorldPolyGrid* cScene::LoadCloud(int xstep,int ystep,float uScale,float vScale,float duOfs,float dvOfs,char *TextureName,char *OpacityName,char *TexturePath,int CloudHeight)
{
	int xSky=2*((RadiusPlanet()+CloudHeight)/xstep+3);
	int ySky=2*((RadiusPlanet()+CloudHeight)/ystep+3);

	cWorldPolyGrid *WorldPolyGrid=new cWorldPolyGrid(BASEOBJECT_TYPE_BASEPOLYGRID_WORLD,BASEOBJECT_KIND_DRAW_EFFECT);
	WorldPolyGrid->New(xSky,ySky,xstep,ystep);
	WorldPolyGrid->SetPosition(0,0,CloudHeight);
	WorldPolyGrid->u=uScale/WorldPolyGrid->xsize;
	WorldPolyGrid->v=vScale/WorldPolyGrid->ysize;
	WorldPolyGrid->du=duOfs; WorldPolyGrid->dv=dvOfs;
	WorldPolyGrid->uofs=WorldPolyGrid->vofs=0.f;
	if(TextureName&&TexturePath) 
		if(OpacityName) WorldPolyGrid->SetTexture(TextureName,TexturePath,OpacityName);
		else WorldPolyGrid->SetTexture(TextureName,TexturePath);
	else WorldPolyGrid->SetTexture(0,0);
	for(int j=0;j<WorldPolyGrid->ysize;j++)
		for(int i=0;i<WorldPolyGrid->xsize;i++)
		{
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].r=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].g=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].b=255;
			WorldPolyGrid->pColor[i+WorldPolyGrid->xsize*j].a=255;
		}
	return WorldPolyGrid;
}
inline int FunctionLightShare(int x,int y,sColor4f &Diffuse,int RadiusWorldSun,sColor4f &color)
{
	int l=sqrt((float)(x*x+y*y));
	if(l>=RadiusWorldSun) 
	{ 
		color.set(0,0,0,0); 
		return 0; 
	}
	color=Diffuse*(RadiusWorldSun-l)/RadiusWorldSun;
	return 1;
}
void cScene::AddAirLight(Vect3f &Pos,sColor4f &Ambient,sColor4f &Diffuse,sColor4f &Illumination,float RadiusWorldSun,float RadiusLight,float RadiusSun,char *TextureName,char *TexturePath)
{
	cWorldPolyGrid *Air=GetAir();
	if(!Air) return;
	int xsize=Air->xsize, ysize=Air->xsize;
	int xstep=round(Air->xstep), ystep=round(Air->ystep);
	int SizeAir=xstep*xsize;
	float dScale=(float)SizeAir/xWorldSize();
	int xLight=round(Pos.x*=dScale);
	int yLight=round(Pos.y*=dScale);
	RadiusWorldSun*=dScale;
	Pos.z=Air->z();
	if(RadiusSun>0)
	{
		cSun *sun=new cSun;
		sun->Pos=Pos;
		sun->Pos.z+=RadiusLight;
		sun->Ambient=Ambient;
		sun->GetDiffuse()=Diffuse;
		sun->GetSpecular()=Illumination;
		sun->RadiusSun=RadiusSun;
		sun->RadiusLight=RadiusLight;
		cMaterial *Material=sun->GetMaterial();
		if(TextureName)
		{
			Material->SetAttribute(ATTRMAT_MASK|ATTRMAT_TEXTURE_PAL);
			Material->Texture=GetLibTex()->Get(TextureName,TexturePath);
		}
		else
			Material->SetAttribute(ATTRMAT_COLOR);
		GetSunArray()->Attach((cUnknownClass*)sun);
	}
	if(Air)
	{
		sBaseColor4c *Point=Air->pColor;
		int xpos=round(Air->x()), ypos=round(Air->y());
		for(int jr=0;jr<ysize;jr++)
			for(int ir=0;ir<xsize;ir++)
			{
				sColor4f color;
				if(FunctionLightShare(GetShortDistX(xLight,xpos+ir*xstep,SizeAir),
					GetShortDistY(yLight,ypos+jr*ystep,SizeAir),Diffuse,RadiusWorldSun,color))
				{
					SET_BYTE(Point[ir+jr*xsize].r,Point[ir+jr*xsize].r+color.GetR());
					SET_BYTE(Point[ir+jr*xsize].g,Point[ir+jr*xsize].g+color.GetG());
					SET_BYTE(Point[ir+jr*xsize].b,Point[ir+jr*xsize].b+color.GetB());
				}
			}
	}
	cWorldPolyGrid *Cloud=GetCloud();
	xsize=Cloud->xsize, ysize=Cloud->xsize;
	xstep=round(Cloud->xstep), ystep=round(Cloud->ystep);
	if(Cloud)
	{
		sBaseColor4c *Point=Cloud->pColor;
		int xpos=round(Cloud->x()), ypos=round(Cloud->y());
		for(int jr=0;jr<ysize;jr++)
			for(int ir=0;ir<xsize;ir++)
			{
				sColor4f color;
				if(FunctionLightShare(GetShortDistX(xLight,xpos+ir*xstep,SizeAir),
					GetShortDistY(yLight,ypos+jr*ystep,SizeAir),Diffuse,RadiusWorldSun,color))
				{
					SET_BYTE(Point[ir+jr*xsize].r,Point[ir+jr*xsize].r+color.GetR());
					SET_BYTE(Point[ir+jr*xsize].g,Point[ir+jr*xsize].g+color.GetG());
					SET_BYTE(Point[ir+jr*xsize].b,Point[ir+jr*xsize].b+color.GetB());
				}
			}
	}
}
void cScene::LoadWorldLight(int number,int track,sColor4f &ColorAmbientAir,char *TextureSun,char *TextureMoon,char *TexturePath)
{
#ifdef _USE_TERRA_
	extern s_sur_scr sur_scr;
	assert(BaseObjectMgr);
	if(sur_scr.load_scr(GetTargetName("SLightR0.sst"))==0)
		sur_scr.load_scr(GetTargetName("SLight.sst"));
	sur_scr.load_data();
	int t_x,t_y,t_z, t_r,t_g,t_b,t_Sun=0;
	int t_kLight, t_SmoothShadow, t_radius;
	for(int i=0; i<sur_scr.numbers_cell; i++)
	{
		sur_scr.row[0].get_el(i,t_x);
		sur_scr.row[1].get_el(i,t_y);
		sur_scr.row[2].get_el(i,t_z);
		sur_scr.row[3].get_el(i,t_r);
		sur_scr.row[4].get_el(i,t_g);
		sur_scr.row[5].get_el(i,t_b);
		sur_scr.row[6].get_el(i,t_kLight);
		sur_scr.row[7].get_el(i,t_radius);//t_dLight);
		sur_scr.row[8].get_el(i,t_SmoothShadow);
		t_r=(t_r*t_kLight)/100; t_g=(t_g*t_kLight)/100; t_b=(t_b*t_kLight)/100;
		sur_scr.row[9].get_el(i,t_Sun);
		sur_scr.row[10].get_el(i,t_r);
		sur_scr.row[11].get_el(i,t_g);
		sur_scr.row[12].get_el(i,t_b);

		Vect3f pos(t_x,t_y,0);
		sColor4f ambient=ColorAmbientAir;
		sColor4f diffuse((float)t_r/255,(float)t_g/255,(float)t_b/255,1.0f);
		sColor4f illumination((float)t_r/2/255,(float)t_g/2/255,(float)t_b/2/255,1.0f);
		float rIllumination=100;
		AddOmniLight(pos,t_radius*0.7f,ambient,diffuse,illumination);
		switch(t_Sun)
		{
			case 0:
				AddAirLight(pos,ambient,diffuse,illumination,t_radius*0.7f,-1,-1,0,0);
				continue;
			case 1: 
				AddAirLight(pos,ambient,diffuse,illumination,t_radius*0.7f,100,50,TextureSun,TexturePath);
				break;
			case 2: 
				AddAirLight(pos,ambient,diffuse,illumination,t_radius*0.7f,100,50,TextureMoon,TexturePath);
				break;
			case -1: // ������ ������
				break;
			default:
				assert(0);
		}
	}
	sur_scr.delete_data();
#endif //_USE_TERRA_
}
void cScene::AddOmniLight(const Vect3f &pos,float radius,const sColor4f &ambient,const sColor4f &diffuse,const sColor4f &illumination)
{
	assert(BaseObjectMgr);
	cOmniLight *OmniLight=(cOmniLight*)BaseObjectMgr->New(BASEOBJECT_TYPE_BASELIGHT_OMNI,BASEOBJECT_KIND_VOLUME);
	OmniLight->Pos()=pos;
	OmniLight->Ambient=ambient;
	OmniLight->Diffuse=diffuse;
	OmniLight->Illumination=illumination;
	OmniLight->Radius()=radius;
}
