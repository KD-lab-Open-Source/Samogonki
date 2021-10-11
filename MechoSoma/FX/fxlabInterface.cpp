#include "StdAfx.h"

#include "aci_parser.h"
#include "win32f.h"

#include "handle.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "OwnerProtection.h"
#include "Meteorite.h"

#include "Levin.h"
#include "TileMap.h"
#include "TileWater.h"
#include "IVisGeneric.h"
#include "terra.h"
#include "race.h"
#include "sound.h"

#include "Igraph3d.h"

#include "sur_scr.h"
#include "SST_Reader.h"

#include "m3d_effects.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"

#include "fxlabApplication.h"

#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientEffect.h"
#include "fxlabClientLauncher.h"
#include "Hermit.h"
#include "fxlabClientLightning.h"
#include "fxlabClientModel.h"
#include "fxlabClientStuff.h"

#include "fxlabServerGeneral.h"
#include "fxlabServerStuff.h"
#include "fxlabServerArcane.h"
#include "fxlabBodyInterface.h"
#include "Mechosoma.h"
#include "Statistics.h"
#include "PersonageDifferetiationPrm.h"
#include "params.h"

fxlabClientObjectDispatcher* fxlabClientD = NULL;
fxlabServerObjectDispatcher* fxlabServerD = NULL;
fxlabResourceDispatcher* fxlabResourceD = NULL;

fxlabRndGenerator fxlabServerRND;
fxlabRndGenerator fxlabClientRND;

//float fxlabServerQuantPeriod = 100.0f; // move to script
float fxlabGlobalTimeRate = 20.0f / 1000.0f;
float fxlabServerTimeRate = 2.0f;
//float fxlabParticleSystemScale = 1.0f; // move to script
float fxlabInvParticleSystemScale = 1.0f / fxlabParticleSystemScale;

extern char* getIniKey(char* fname,char* section,char* key);
extern char* mch_mainINI;

int fxlabPolyGridAlpha;

extern int mchCurrentWorld;
extern int mchCurrentTrack;

int fxlabStopTime;

fxlabTeleportDispatcher* fxlabTeleportD;
fxlabMovieDispacther* fxlabMovieD;

//void fxlabCreateBonusMarket(void);
void fxlabCreatePointMarket(void);
void fxlabCreateLineMarket(void);
void fxlabCreateWaveGround(void);
void fxlabCreatePolarMarket(void);
void fxlabCreateController(void);

MatXf getPlacementGroundPose(const Vect3f& center);

void fxlabMechosArmor(Mechos* p);
void fxlabMechosBio(Mechos* p);
void fxlabMechosSkin(Mechos* p);
void fxlabMechosIsolation(Mechos* p);

static DurationTimer fxlabPartWaveTimer;

int fxlabBigBossGeneratorEnable = 1;
int fxlabBigBossWorldEnable = FXLAB_BIG_BOSS_WORLD_ID_MOUNTAIN | FXLAB_BIG_BOSS_WORLD_ID_FOREST | FXLAB_BIG_BOSS_WORLD_ID_WATCH | FXLAB_BIG_BOSS_WORLD_ID_WATER | FXLAB_BIG_BOSS_WORLD_ID_FIRE | FXLAB_BIG_BOSS_WORLD_ID_TOWN;

void fxlabBigBossStatus(int status);
void fxlabBigBossWorld(int world,int status);

void fxlabOpen(void)
{
	DBGCHECK;

	fxlabStopTime = 0;

/*	fxlabBigBossStatus(1);

	fxlabBigBossWorld(0,1);
	fxlabBigBossWorld(1,1);
	fxlabBigBossWorld(2,1);
	fxlabBigBossWorld(3,1);
	fxlabBigBossWorld(4,1);
	fxlabBigBossWorld(8,1);*/

	fxlabServerRND.Open();
	fxlabClientRND.Open();

	fxlabServerD->Start();
	fxlabClientD->Start();
	fxlabCreateWaveGround();
//	fxlabCreateBonusMarket();
	fxlabCreatePointMarket();
	fxlabCreateLineMarket();
	fxlabCreatePolarMarket();

	fxlabTeleportD->Open();
	fxlabPartWaveTimer.start(0);
	DBGCHECK;
};

void fxlabClose(void)
{
	DBGCHECK;
	fxlabMovieD->Clear();
	fxlabTeleportD->Close();

	fxlabServerD->Close();
	fxlabClientD->Close();
	DBGCHECK;
};


void fxlabInit(void)
{
	DBGCHECK;

//	fxlabServerQuantPeriod = atof(getIniKey(mch_mainINI,"FXLAB","ServerQuantPeriod"));
	fxlabGlobalTimeRate = fxlabGlobalTimeRateMs / 1000.0f;

	fxlabServerTimeRate = fxlabGlobalTimeRateMs / (1000.0f / fxlabServerQuantPeriod);

//	fxlabParticleSystemScale = atof(getIniKey(mch_mainINI,"FXLAB","ParticleSystemScale"));
	fxlabInvParticleSystemScale = 1.0f / fxlabParticleSystemScale;

	fxlabResourceD = new fxlabResourceDispatcher;
	fxlabResourceD->Open();

	fxlabServerD = new fxlabServerObjectDispatcher;
	fxlabClientD = new fxlabClientObjectDispatcher;

	fxlabServerRND.Open();
	fxlabClientRND.Open();

	fxlabServerD->Open();
	fxlabClientD->Open();	

	fxlabTeleportD = new fxlabTeleportDispatcher;
	fxlabTeleportD->Init();

	fxlabMovieD = new fxlabMovieDispacther;
	fxlabMovieD->Open();

	DBGCHECK;
};

void fxlabFinit(void)
{
	DBGCHECK;

	fxlabMovieD->Close();

	fxlabTeleportD->Finit();
	delete fxlabTeleportD;
	fxlabTeleportD = NULL;

	fxlabServerD->Close();
	fxlabClientD->Close();
	fxlabResourceD->Close();

	fxlabServerD = NULL;
	fxlabClientD = NULL;
	fxlabResourceD = NULL;

	DBGCHECK;
};

void fxlabServerQuant(void)
{
	start_autostop_timer(fxlabServerQuant, STAT_RESDISP);
	CONTROL_FP();

//	mch_raceD->ClearTrackFlags(0);
	fxlabServerD->Quant();
//	fxlabServerD->CreateQuant();
};

void fxlabClientQuant(void)
{
	fxlabClearDeltaPolygon();

	fxlabClientD->Quant();
	fxlabClientD->CreateQuant();
};

void fxlabClientClear(void)
{
	fxlabClientD->Quant();
	fxlabClientD->CreateQuant();
	fxlabClientD->Kill();
};


void fxlabCharacterTeleportation(Vect3f& v0,Vect3f& v1);
void fxlabPartWave(int x,int y);
void fxlabPartWaveQuant(void);

void fxlabClientShow(void)
{
	fxlabClientD->Show();

#ifndef _FINAL_VERSION_
	static DurationTimer xxxTimer;
	if(XKey.Pressed('X') && !xxxTimer()){
		fxlabGeneralObjectType* t;

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WORLD_IRRADIATE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Vect3f(mch_raceD->activeRacer->R()));
		t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE);
		t->SetMechosPoint(dynamic_cast<mchMechosRacer*>(mch_raceD -> activeRacer));
		t->Start();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_WORLD_EXPLOSION);
		t->Start();

		xxxTimer.start(300);
	};
#endif

/*	if(XKey.Pressed('X')){
		fxlabPartWave(mch_raceD->activeRacer->R().x,mch_raceD->activeRacer->R().y);
		fxlabPartWaveQuant();
	};*/

/*	if(XKey.Pressed('X'))
		fxlabMovieUpdateProcess("FireC",Vect3f(mch_raceD->activeRacer->R()),Vect3f(0,0,5.0f),1);

	if(XKey.Pressed('Z'))
		fxlabMovieUpdateProcess("FireC",Vect3f(mch_raceD->activeRacer->R()),Vect3f(0,0,5.0f),0);*/

/*	static DurationTimer xxxTimer;
	if(XKey.Pressed('X') && !xxxTimer()){
		fxlabGeneralObjectType* t;	

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Vect3f(mch_raceD->activeRacer->R()));
		t->SetKeyID(FXLAB_ID_KEY_BOSS_STEAM_TAIL);
		t->SetVelocity(Vect3f(0,0,1.0f));
//		t->SetMechosPoint(static_cast<mchMechosRacer*>(mch_raceD->activeRacer));
		t->Start();
		xxxTimer.start(2000);
	};*/
};

void fxlabClientObjectDispatcher::Kill(void)
{
	list<fxlabGeneralObjectType*>::iterator p;

	p = ObjectList.begin();
	while(p != ObjectList.end()){
		if((*p)->GetAlive())
			++p;
		else{
			(*p)->Close();
			delete (*p);
			p = ObjectList.erase(p);
		};
	};
};

void fxlabClientObjectDispatcher::Show(void)
{
	list<fxlabGeneralObjectType*>::iterator p;
	cInterfaceGraph3d *IGraph3d;
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	IGraph3d = (cInterfaceGraph3d*)xrealIVG -> GetIGraph3d();
	IVisGeneric->Release();

	if(IGraph3d->IsActive()){
		p = ObjectList.begin();
		while(p != ObjectList.end()){
			(*p)->Show();
			if((*p)->GetAlive())
				++p;
			else{
				(*p)->Close();
				delete (*p);
				p = ObjectList.erase(p);
			};
		};
	}else{
		p = ObjectList.begin();
		while(p != ObjectList.end()){
			if((*p)->GetAlive())
				++p;
			else{
				(*p)->Close();
				delete (*p);
				p = ObjectList.erase(p);
			};
		};	
	};
};

//---------------------------------------

void fxlabCreateAverageCheckPoint(fxlabProcessInterface* master,int x,int y,int z,int key,float scale,int dx,int dy,int dz,int n_camera)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CHECK_POINT);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(x,y,z));
	t->SetScale(scale);
	t->SetMode(key);
	t->SetVelocity(Vect3f((float)(getDistX(x,dx)),(float)(getDistY(y,dy)),(float)(z - dz)));
	t->SetCheckPointModels(M3D_CHECK_POINT0,M3D_CHECK_POINT1);
	if(n_camera)
		t->SetCheckPointCamera(MESH_CAMERA_MASK_B);
	else
		t->SetCheckPointCamera(MESH_CAMERA_MASK_A);

	t->SetProcessInterface(master);
	t->Start();
};

void fxlabChangeCheckPoint(fxlabProcessInterface* master,int key,struct mchRacer* p,int init_flag)
{
	master->Process->SetCheckPointRacer(key,p,init_flag);
};

void fxlabCreateMasterCheckPoint(fxlabProcessInterface* master,int x,int y,int z,int key,float scale,int dx,int dy,int dz,int ind,int n_camera)
{
	fxlabGeneralObjectType* t;
	const int fxlabActiveStartID[5] = { M3D_START_GREEN_S,M3D_START_GREEN_T,M3D_START_GREEN_A,M3D_START_GREEN_R,M3D_START_GREEN_T };
	const int fxlabPassedStartID[5] = { M3D_START_RED_S,M3D_START_RED_T,M3D_START_RED_A,M3D_START_RED_R,M3D_START_RED_T };

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CHECK_POINT);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(x,y,z));
	t->SetScale(1.0f);
	t->SetMode(key);
	t->SetVelocity(Vect3f((float)(getDistX(x,dx)),(float)(getDistY(y,dy)),(float)(z - dz)));
	t->SetCheckPointModels(fxlabActiveStartID[ind],fxlabPassedStartID[ind]);
	if(n_camera)
		t->SetCheckPointCamera(MESH_CAMERA_MASK_B);
	else
		t->SetCheckPointCamera(MESH_CAMERA_MASK_A);
	t->SetProcessInterface(master);
	t->SetAngle(scale);
	t->Start();
};


int fxlabLine2SphereDist(Vect3f& r1,Vect3f& c,float rad)
{
	Vect3f m,m2,p;
	float t,d;

	m = r1;
//	m -= r0;
	d = m.x * m.x + m.y * m.y + m.z * m.z;
	if(d < 0){
		if(d > -0.001f) return 0;
	}else{
		if(d < 0.001f) return 0;
	};
//	t =	(m.x*(c.x - r0.x) + m.y*(c.y - r0.y) + m.z*(c.z - r0.z)) / d;
	t =	(m.x*c.x + m.y*c.y + m.z*c.z) / d;
	if(t < 0 || t > 1.0f) return 0;

	p = m;
	p *= t;
//	p += r0;

	m = p;
	m -= c;
	t = m.x * m.x + m.y * m.y + m.z * m.z;
	if(t < rad*rad) return 1;
	return 0;
};

//---------------------------------------------

void fxlabClearDeltaPolygon(void)
{
	int i,j;
	sPointTile* n;
	sTileMap* s;

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	cTileMap *tMap=(cTileMap*)IVisGeneric->GetTileMap();
	if(tMap==0) { IVisGeneric->Release(); return; }
	for(i = 0;i < tMap->NumberTileX()*tMap->NumberTileY();i++){
		s = tMap->GetTile(i,0);
		if(s->Point.length()<1) continue;
		n = &(s->Point[0]);
		for(j = 0;j < s->Point.length();j++){
			n->dx = 0;
			n->dy = 0;
			n->dz = 0;
			n++;
		};
	};
	IVisGeneric->Release();
};

extern int mchCurrentTrack;

void fxlabCreateWaveGround(void)
{
	int i;
	cPolyGrid *s;
	fxlabGeneralObjectType* t;
	cTileMap *tMap;
	cTileWater *tWater;
	sTileMap *m;

	fxlabPolyGridAlpha = 255;

	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();

	if(Mdisp->isSnow()){
		tMap=(cTileMap*)IVisGeneric->GetTileMap();
		for(i = 0;i < tMap->NumberTileX()*tMap->NumberTileY();i++){
			m = tMap->GetTile(i,0);
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SNOW_LAYER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetTilePoint(m);
			t->SetPosition(Vect3f(m->xPos,m->yPos,500.0f));
			t->SetKeyID(FXLAB_ID_KEY_SNOW_LAYER);
			t->Start();
		};
	};

	if(mchCurrentWorld < fxlabResourceD->WorldNum && !fxlabGetWorldReflectionEnable()){
		tMap=(cTileMap*)IVisGeneric->GetTileMap();
		tWater=(cTileWater*)IVisGeneric->GetTileWater();
		if(tWater==0) { IVisGeneric->Release(); return; }
		for(i = 0;i < tWater->NumberTileX()*tWater->NumberTileY();i++){
			if(s=tWater->GetPolyGrid(i,0)){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WAVE_GROUND);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetGridPoint(tMap->GetTile(i,0),s);
				t->SetGridRed(fxlabGetWaterRed(),fxlabGetWaterRedDelta());
				t->SetGridGreen(fxlabGetWaterGreen(),fxlabGetWaterGreenDelta());
				t->SetGridBlue(fxlabGetWaterBlue(),fxlabGetWaterBlueDelta());
				t->SetGridAlpha(fxlabGetWaterAlpha(),fxlabGetWaterDeltaAlpha());
				t->SetGridHeight(fxlabGetWaterDeltaHeight());
				t->SetGridUV(fxlabGetWaterDeltaUV());
				t->Start();
			};
		};
	};
	IVisGeneric->Release();
};

//------------------------------------------

int fxlabGetWorldReflectionEnable(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].ReflectionEnable;
};

int fxlabGetWaterRed(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterRed;
};

int fxlabGetWaterRedDelta(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaRed;
};

int fxlabGetWaterGreen(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterGreen;
};

int fxlabGetWaterGreenDelta(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaGreen;
};

int fxlabGetWaterBlue(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterBlue;
};

int fxlabGetWaterBlueDelta(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaBlue;
};

int fxlabGetWaterAlpha(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterAlpha;
};

int fxlabGetWaterDeltaAlpha(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaAlpha;
};

int fxlabGetWaterDeltaHeight(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaHeight;
};

float fxlabGetWaterDeltaUV(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaterDeltaUV;
};

int fxlabGetDustID(int style)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].DustStyle[style];
};

float fxlabGetTrackWaveRed(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaveRed;
};

float fxlabGetTrackWaveGreen(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaveGreen;
};

float fxlabGetTrackWaveBlue(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaveBlue;
};

float fxlabGetTrackWaveAlpha(void)
{
	return fxlabResourceD->WorldData[mchCurrentWorld]->TrackData[mchCurrentTrack].WaveAlpha;
};

//-------------------------------------

float triangleMapH(int x, int y, float z);
int fxGetLevel(int x,int y,float z)
{
	return triangleMapH(x, y, z);
};

//----------------

void fxlabFirePointInterface::Open(int id)
{
	ID = id;
};

void fxlabFirePointInterface::Close(void)
{
	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);
};

void fxlabFirePointInterface::Update(Vect3f& pos,Vect3f& vel)
{
	fxlabGeneralObjectType* t;

	switch(ID){
		case FXLAB_FIRE_POINT_INTERFACE_CLAW:
			if(ClientPoint.Process){
				ClientPoint.Process->SetPosition(pos);
				ClientPoint.Process->SetVelocity(vel);
			}else{
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WILD_CLAW);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_WILD_CLAW);
				t->SetPosition(pos);
				t->SetVelocity(vel);
				t->SetProcessInterface(&ClientPoint);
				t->Start();
			};

			if(mchCurrentTrack == 5){
				if(ServerPoint.Process){
					ServerPoint.Process->SetPosition(pos);
					ServerPoint.Process->SetVelocity(vel);
				}else{
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_PERESTROYKA_CLAW);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_PERESTROYKA_CLAW_CONTROL);
					t->SetPosition(pos);
					t->SetProcessInterface(&ServerPoint);
					t->SetRemoteObject(ClientPoint.Process);
					t->Start();
				};
			}else{
				if(ServerPoint.Process){
					ServerPoint.Process->SetPosition(pos);
					ServerPoint.Process->SetVelocity(vel);
				}else{
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_WILD_CLAW);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_WILD_CLAW_CONTROL);
					t->SetPosition(pos);
					t->SetProcessInterface(&ServerPoint);
					t->SetRemoteObject(ClientPoint.Process);
					t->Start();
				};
			};
			break;
		case FXLAB_FIRE_POINT_INTERFACE_FIRE:
			if(ClientPoint.Process){
				ClientPoint.Process->SetPosition(pos);
				ClientPoint.Process->SetVelocity(vel * 10.0f);
			}else{
				t = fxlabClientD->CreateObject(/*FXLAB_CLIENT_PROCESS_REMOTE_SWITCHER*/FXLAB_CLIENT_PROCESS_DRAGON_FIRE_SWITCHER);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_DRAGON_FIRE_SWITCHER);
				t->SetPosition(pos);
				t->SetVelocity(vel * 10.0f);
				t->SetProcessInterface(&ClientPoint);
				t->Start();
			};

			if(ServerPoint.Process){
				ServerPoint.Process->SetPosition(pos);
				ServerPoint.Process->SetVelocity(vel);
			}else{
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DRAGON_HEAD_FIRE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_DRAGON_FIRE_DAMAGE);
				t->SetPosition(pos);
				t->SetVelocity(vel);
				t->SetProcessInterface(&ServerPoint);
				t->SetRemoteObject(ClientPoint.Process);
				t->Start();
			};
			break;
		case FXLAB_FIRE_POINT_INTERFACE_PUMP:
			if(ClientPoint.Process){
				ClientPoint.Process->SetPosition(pos);
				ClientPoint.Process->SetVelocity(vel);
			}else{
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetPosition(pos);
				t->SetVelocity(vel);
				t->SetKeyID(FXLAB_ID_KEY_WATERPUMP_STREAM);
				t->SetProcessInterface(&ClientPoint);
				t->Start();
			};
			break;
	};
};

void fxlabFirePointInterface::Update(class Vect3f& pos,class Mat3f& vel)
{
	fxlabGeneralObjectType* t;

	switch(ID){
		case FXLAB_FIRE_POINT_INTERFACE_BOSS_WATER:
			if(ClientPoint.Process){
				ClientPoint.Process->SetPosition(pos);
				ClientPoint.Process->SetDirectionMatrix(vel);
			}else{
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_STREAM_FOUNTAIN);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetPosition(pos);
				t->SetDirectionMatrix(vel);
				t->SetKeyID(FXLAB_ID_KEY_BOSS_WATER_CLUSTER);
				t->SetProcessInterface(&ClientPoint);
				t->Start();
			};
			break;
	};
};

const float FXLAB_CANNON_FIRE_BULLET_SPEED = 20.0f;

void fxlabCannonFire(Vect3f& pos,Vect3f& vel,float pow)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CANNON_DUST);
	t->SetPosition(pos);
	t->SetVelocity(vel);
	t->Start();

	vel *= FXLAB_CANNON_FIRE_BULLET_SPEED;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TAIL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CANNON_FIRE);
	t->SetPosition(pos);
	t->SetVelocity(vel);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_CANNON_OMNI);
	t->SetPosition(pos);
	t->Start();

//!!!!
	fxlabCreateBolide<fxlabCannonBomb>(FXLAB_MODEL_INTERFACE_ID_CANNON_START,FXLAB_MODEL_INTERFACE_ID_CANNON_NOISE,pos,vel,pow,FXLAB_CLIENT_PROCESS_BOMB_EXPLODE_LINK,NULL,NULL,NULL);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_CANNON);
	t->SetPosition(pos);
	t->Start();
};

void fxShowBitmap(const int sx,const int sy,unsigned char* sbuf,int dx,int dy,unsigned char* dbuf,double angle)
{
	unsigned char* pd;
	int x,y;
	int x0,y0,x1,y1,x2,y2;
	int x_add,y_add;
	int x_delta,y_delta;
	int cx,cy;
	int tx,ty;
	int cosTurn,sinTurn;
	int i,j;
	int cnt;

	pd = dbuf;

	cx = dx / 2;
	cy = dy / 2;
	tx = sx / 2;
	ty = sy / 2;

	cosTurn = round(cos(angle) * (double)(1 << 16));
	sinTurn = round(sin(angle) * (double)(1 << 16));

	x0 = (-cx)*cosTurn - (-cy)*sinTurn;
	y0 = (-cx)*sinTurn + (-cy)*cosTurn;

	x1 = (cx)*cosTurn - (-cy)*sinTurn;
	y1 = (cx)*sinTurn + (-cy)*cosTurn;

	x2 = (-cx)*cosTurn - (cy)*sinTurn;
	y2 = (-cx)*sinTurn + (cy)*cosTurn;

	x_add = (x1 - x0) / dx;
	y_add = (y1 - y0) / dx;
	x_delta = (x2 - x0) / dy;
	y_delta = (y2 - y0) / dy;

	x0 = x0 + (tx << 16);
	y0 = y0 + (ty << 16);

	cnt = 0;

	for(i = 0;i < dy;i++){
		x = x0;
		y = y0;
		for(j = 0;j < dx;j++){
			if(x >= 0 && x < (sx << 16) && y >= 0 && y < (sy << 16)){
				*pd = sbuf[(y >> 16) * sy + (x >> 16)];
				cnt++;
			}else 
				*pd = 0;
			pd++;
			x += x_add;
			y += y_add;
		};
		x0 += x_delta;
		y0 += y_delta;
	};
};


//---------------------------------------

/*struct fxlabBonusMarketTrigger
{
	int PosX;
	int PosY;
	int PosZ;
	int Group;
	int Time;

	void read(XBuffer& b)
	{ 
		b >= PosX;
		b >= PosY;
		b >= PosZ;
		b >= Group;
		b >= Time;
	};
};

void fxlabCreateBonusMarket(void)
{
	XStream test_bonus(0);
	fxlabGeneralObjectType* t;	
	XBuffer buf;
	SST_Reader<fxlabBonusMarketTrigger>::iterator i_market;

	buf < "BonusMarket" <= (mchCurrentTrack%10) < ".sst";
	if(test_bonus.open(GetTargetName(buf))){
		test_bonus.close();
		SST_Reader<fxlabBonusMarketTrigger> bonus_list(GetTargetName(buf));
		FOR_EACH(bonus_list,i_market){
			switch(i_market->Group){
				case 0:	//Jump
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_MARKET);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(ARCANE054);
					t->SetBonusRespawnTime(i_market->Time);
					t->Start();
					break;
				case 1: //Nitro				
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_MARKET);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(ARCANE055);
					t->SetBonusRespawnTime(i_market->Time);
					t->Start();
					break;
				case 2: //Energy
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_MARKET);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(ARCANE053);
					t->SetBonusRespawnTime(i_market->Time);
					t->Start();
					break;
				case 3: //Mana
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_MARKET);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(ARCANE062);
					t->SetBonusRespawnTime(i_market->Time);
					t->Start();
					break;
				case 4: //Stars
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_MARKET);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(ARCANE063);
					t->SetBonusRespawnTime(i_market->Time);
					t->Start();
					break;
				case 5: //Part
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS_SINGLE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBonusActionID(-1);
					t->Start();
					break;
			};
		};
	};
};
*/

//---------------------------------------

struct fxlabPointMarketTrigger
{
	int PosX;
	int PosY;
	int PosZ;
	int Group;

	void read(XBuffer& b)
	{ 
		b >= PosX;
		b >= PosY;
		b >= PosZ;
		b >= Group;
	};
};

const float FXLAB_STONE_ALTAR_RADIUS = 60.0f;
const int FXLAB_STONE_ALTAR_FIRE = 7;

void fxlabCreatePointMarket(void)
{
	int i,x,y;
	float a,da;
//	XStream test_point(0);
	fxlabGeneralObjectType* t;	
	SST_Reader<fxlabPointMarketTrigger>::iterator i_market;

//	if(test_point.open(GetTargetName("PointMarket.sst"))){
//		test_point.close();
		SST_Reader<fxlabPointMarketTrigger> point_list(GetTargetName("PointMarket.sst"));
		FOR_EACH(point_list,i_market){
			switch(i_market->Group){
				case 0:	//Volcano
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_VOLCANIC_LAUNCHER);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_LAUNCHER);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_FIRE);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_FIRE);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_DUST);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ + 25.0f));
					t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_OMNI);
					t->Start();
					break;
				case 1:	//Head
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_VOLCANIC_LAUNCHER);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_HEAD_LAUNCHER);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_VOLCANIC_DUST);
					t->Start();
					break;
				case 2: //Lava'sDust
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_OBJECT_LAUNCHER);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_LAVA_WIND_LAUNCHER);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;
				case 3: //FishWater
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_STREAM_CLUSTER);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_FISHWATER_CLUSTER);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;
				case 4: //Altar
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_ALTAR_DAMAGE);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_ALTAR_DUST);
					t->Start();

					a = 0;
					da = 2.0f * M_PI / (float)(FXLAB_STONE_ALTAR_FIRE);
					
					for(i = 0;i < FXLAB_STONE_ALTAR_FIRE;i++){
						x = XCYCL(round(cosf(a) * FXLAB_STONE_ALTAR_RADIUS + i_market->PosX));
						y = YCYCL(round(sinf(a) * FXLAB_STONE_ALTAR_RADIUS + i_market->PosY));

						t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_FIRE);
						t->SetStartTime(fxlabClientD->GetTime());
						t->SetPosition(Vect3f(x,y,i_market->PosZ));
						t->SetKeyID(FXLAB_ID_KEY_ALTAR_FIRE);
						t->Start();
						a += da;
					};
					break;
				case 5: //Energy Station
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_PLACE_ENERGY_SENSOR);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_PLACE_ENERGY_FOG);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_PLACE_ENERGY_STAR);
					t->Start();
					break;
				case 6: //Mana Station
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_PLACE_MANA_SENSOR);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_PLACE_MANA);
					t->Start();
					break;
				case 7: //LavaAltar
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_FIRE);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_LAVA_ALTAR_FIRE);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_LAVA_ALTAR_OMNI);
					t->Start();

					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetKeyID(FXLAB_ID_KEY_LAVA_ALTAR_DAMAGE);
					t->Start();
					break;
				case 8: //BlueSlime
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ANIMATION_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_BLUE_SLIME_MODEL);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();

					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BLUE_SLIME_CIRCLE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_BLUE_SLIME_CIRCLE);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;
//WindSound
				case 9:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WIND_SOUND);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;
//Character Bubble
				case 10:
					t = fxlabClientD->CreateObject(/*FXLAB_CLIENT_PROCESS_SPARK_GENERATOR*//*FXLAB_CLIENT_PROCESS_DUST*/FXLAB_CLIENT_PROCESS_CHARACTER_WAVE_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_CHARACTER_BUBBLE);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;

//Part Bubble
				case 11:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;

//WaterFall Mountain
				case 12:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_MOUNTAIN);
					t->Start();
					break;
//WaterFall Forest
				case 13:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_FOREST);
					t->Start();
					break;
//WaterFall Watch
				case 14:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_WATCH);
					t->Start();
					break;

//WaterFall Water
				case 15:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_WATER);
					t->Start();
					break;

//WaterFall Fire
				case 16:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_FIRE);
					t->Start();
					break;

//WaterFall Town
				case 17:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR);
					t->SetKeyID(FXLAB_ID_KEY_WATERFALL_FOG);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetBigBossWorldStatus(FXLAB_BIG_BOSS_WORLD_ID_TOWN);
					t->Start();
					break;

//Large Blue Slim
				case 18:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ANIMATION_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_BLUE_SLIME_LARGE_MODEL);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();

					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BLUE_SLIME_CIRCLE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_BLUE_SLIME_LARGE_CIRCLE);
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->Start();
					break;
			};
		};
//	};
};

//-----------------------------------------------------------

struct fxlabPointLineTrigger
{
	int PosX;
	int PosY;
	int PosZ;
	int ID;
	int Owner;
	int Group;

	void read(XBuffer& b)
	{ 
		b >= PosX;
		b >= PosY;
		b >= PosZ;
		b >= ID;
		b >= Owner;
		b >= Group;
	};
};

const float FXLAB_BUBBLE_STREAM_STEP = 80;
const float FXLAB_WIND_STREAM_STEP = 80;

void fxlabCreateLineMarket(void)
{
	int i,n;
	float d;
	Vect3f v,p,dv;
//	XStream test_point(0);
	fxlabGeneralObjectType* t;
	fxlabGeneralObjectType* st;	
	SST_Reader<fxlabPointLineTrigger>::iterator fi_market;
	SST_Reader<fxlabPointLineTrigger>::iterator si_market;

//	if(test_point.open(GetTargetName("LineMarket.sst"))){
//		test_point.close();
		SST_Reader<fxlabPointLineTrigger> point_list(GetTargetName("LineMarket.sst"));
		FOR_EACH(point_list,fi_market){
			if(fi_market->Owner){
				FOR_EACH(point_list,si_market){
					if(!(si_market->Owner) && fi_market->Group == si_market->Group && si_market->ID == fi_market->ID){
						p = Vect3f(fi_market->PosX,fi_market->PosY,fi_market->PosZ);
						v.x = (float)(getDistX(si_market->PosX,fi_market->PosX));
						v.y = (float)(getDistY(si_market->PosY,fi_market->PosY));
						v.z = (float)(si_market->PosZ - fi_market->PosZ);
						switch(si_market->Group){
							case 0: //FireHead
								v.Normalize(120.0f);

								dv = v;
								dv /= 3.0f;
								v.Normalize();

								t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DRAGON_HEAD_FIRE_SWITCHER);
								t->SetStartTime(fxlabClientD->GetTime());
								t->SetKeyID(FXLAB_ID_KEY_DRAGON_FIRE_SWITCHER);
								t->SetPosition(p);
								t->SetVelocity(v*20.0f);
								t->Start();

								st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DRAGON_FIRE);
								st->SetStartTime(fxlabServerD->GetTime());
								st->SetKeyID(FXLAB_ID_KEY_HEAD_FIRE_DAMAGE);
								st->SetPosition(p);
								st->SetVelocity(v);
								st->SetRemoteObject(t);
								st->Start();

								for(i = 0;i < 3;i++){
									p += dv;

									CYCLE(p);
									t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_REMOTE_SWITCHER);
									t->SetStartTime(fxlabClientD->GetTime());
									t->SetKeyID(FXLAB_ID_KEY_ROSE_FIRE_SWITCHER);
									t->SetPosition(Vect3f(p.x,p.y,fxlabGetLevel(p.x,p.y,p.z)));
									t->Start();

									st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DRAGON_ROSE);
									st->SetStartTime(fxlabServerD->GetTime());
									st->SetKeyID(FXLAB_ID_KEY_HEAD_ROSE_DAMAGE);
									st->SetPosition(Vect3f(p.x,p.y,fxlabGetLevel(p.x,p.y,p.z)));
									st->SetRemoteObject(t);
									st->Start();
								};
								break;
							case 1: //Head'sDust
								v.Normalize(10.0f);
								t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_HEAD_DUST_LAUNCHER);
								t->SetStartTime(fxlabClientD->GetTime());
								t->SetKeyID(FXLAB_ID_KEY_HEAD_BREATH_LAUNCHER);
								t->SetPosition(p);
								t->SetVelocity(v);
								t->Start();
								break;
							case 2: //WaterBubbleStream
								d = v.norm();
								n = round(d / FXLAB_BUBBLE_STREAM_STEP);
								dv = v;
								dv /= (float)(n);
								v.Normalize();
								for(i = 0;i < n;i++){
									CYCLE(p);
									t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_WIND_WATER_CLIP);
									t->SetStartTime(fxlabClientD->GetTime());
									t->SetKeyID(FXLAB_ID_KEY_WATER_BUBBLE_FLUX);
									t->SetPosition(p);
									t->SetVelocity(v);
									t->Start();

									st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_FLUX_FIELD);
									st->SetStartTime(fxlabServerD->GetTime());
									st->SetKeyID(FXLAB_ID_KEY_WATER_FIELD_STREAM);
									st->SetPosition(p);
									st->SetVelocity(v);
									st->Start();
									p += dv;
								};
								break;
							case 3: //ForestBubbleStream
								d = v.norm();
								n = round(d / FXLAB_BUBBLE_STREAM_STEP);
								dv = v;
								dv /= (float)(n);
								v.Normalize();
								for(i = 0;i < n;i++){
									CYCLE(p);
									t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_WIND_WATER_CLIP);
									t->SetStartTime(fxlabClientD->GetTime());
									t->SetKeyID(FXLAB_ID_KEY_FOREST_BUBBLE_FLUX);
									t->SetPosition(p);
									t->SetVelocity(v);
									t->Start();

									st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_FLUX_FIELD);
									st->SetStartTime(fxlabServerD->GetTime());
									st->SetKeyID(FXLAB_ID_KEY_FOREST_FIELD_STREAM);
									st->SetPosition(p);
									st->SetVelocity(v);
									st->Start();
									p += dv;
								};
								break;
							case 4: //WindStream
								d = v.norm();
								n = round(d / FXLAB_WIND_STREAM_STEP);
								dv = v;
								dv /= (float)(n);
								v.Normalize();
								for(i = 0;i < n;i++){
									CYCLE(p);
									t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_WIND);
									t->SetStartTime(fxlabClientD->GetTime());
									t->SetKeyID(FXLAB_ID_KEY_STONE_BUBBLE_FLUX);
									t->SetPosition(p);
									t->SetVelocity(v);
									t->Start();

									st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_WIND_FIELD);
									st->SetStartTime(fxlabServerD->GetTime());
									st->SetKeyID(FXLAB_ID_KEY_STONE_FIELD_STREAM);
									st->SetPosition(p);
									st->SetVelocity(v);
									st->Start();
									p += dv;
								};
								break;

							case 5: //CharacterDust
								v.Normalize();
								t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_STEAM_GENERATOR);
								t->SetStartTime(fxlabClientD->GetTime());
								t->SetKeyID(FXLAB_ID_KEY_BOSS_STEAM_TAIL);
								t->SetPosition(p);
								t->SetVelocity(v);
								t->Start();
								break;

							case 6: //CharacterSmoke
								v.Normalize(7.0f);
								t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOSS_SMOKE_GENERATOR);
								t->SetStartTime(fxlabClientD->GetTime());
								t->SetKeyID(FXLAB_ID_KEY_BOSS_SMOKE_TAIL);
								t->SetPosition(p);
								t->SetVelocity(v);
								t->Start();
								break;

						};
						break;
					};
				};
			};
		};
//	};
};


struct fxlabPolarMarketTrigger
{
	int PosX;
	int PosY;
	int PosZ;

	int VelX;
	int VelY;
	int VelZ;

	int Group;

	void read(XBuffer& b)
	{ 
		b >= PosX;
		b >= PosY;
		b >= PosZ;

		b >= VelX;
		b >= VelY;
		b >= VelZ;

		b >= Group;
	};
};

void fxlabCreatePolarMarket(void)
{
//	XStream test_point(0);
	fxlabGeneralObjectType* t;	
	SST_Reader<fxlabPolarMarketTrigger>::iterator i_market;
	Vect3f v;

//	if(test_point.open(GetTargetName("PolarMarket.sst"))){
//		test_point.close();
		SST_Reader<fxlabPolarMarketTrigger> point_list(GetTargetName("PolarMarket.sst"));
		FOR_EACH(point_list,i_market){
			v.setSpherical((float)(i_market->VelX) * M_PI / 180.0f,(float)(i_market->VelY) * M_PI / 180.0f,i_market->VelZ);
			switch(i_market->Group){
				case 0:	//HeadStream
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(i_market->PosX,i_market->PosY,i_market->PosZ));
					t->SetVelocity(v);
					t->SetKeyID(FXLAB_ID_KEY_HEAD_STREAM);
					t->Start();
					break;
			};
		};
//	};
};

//------------------------------------------------------------

extern int MCH_CP_STAR_NUM;

void fxlabBonusEvent(int id,struct mchRacer* owner)
{
	mchMechosRacer* p;
	int d;
	fxlabGeneralObjectType* t;

	if(owner->Type == MCH_RACER_TYPE_MECHOS){
		p = static_cast<mchMechosRacer*>(owner);
		switch(id){
			case ARCANE053:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_CHARGE_ENERGY);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetMechosPoint(p);
				t->SetChargeValue(p->energy_max() * 0.5f);
				t->SetChargeTime(2500.0f);
				t->Start();
				break;
			case ARCANE054:
				p->jump();

/*				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_JUMP_LEFT);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_JUMP_ACTION_LINK);
				t->SetBody(p);
				t->Start();

				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_JUMP_RIGHT);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_JUMP_ACTION_LINK);
				t->SetBody(p);
				t->Start();*/
				break;
			case ARCANE055:
				p->speed_increment();
/*				p->nitro();
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_NITRO_LEFT);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_NITRO_ACTION_LINK);
				t->SetBody(p);
				t->Start();

				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_NITRO_RIGHT);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_NITRO_ACTION_LINK);
				t->SetBody(p);
				t->Start();*/

				if(p->active()){
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetSoundID(EFF_BONUS_SPEED);
					t->SetPosition(Vect3f(p->R()));
					t->Start();
				};
				break;
			case ARCANE062:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_CHARGE_MANA);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetMechosPoint(p);
				t->SetChargeValue(20.0f);
				t->SetChargeTime(1000.0f);
				t->Start();
				break;
			case ARCANE063:
				if(owner->stPtr->Lap != 0 || owner->stPtr->Checkpoint != 0){
					d = (owner->stPtr->Lap * (mch_raceD->cpLst->size() - 1) + owner->stPtr->Checkpoint - 1) * MCH_CP_STAR_NUM - owner->NStar;
					if(d < 10)
						owner->NStar += d;
					else
						owner->NStar += 10;
				};
				break;
			case ARCANE067:
				break;
			case ARCANE068:
				fxlabMechosSkin(p);
				break;
			case ARCANE069:
				fxlabMechosIsolation(p);
				break;
			case ARCANE070:
				fxlabMechosArmor(p);
				break;
			case ARCANE071:
				fxlabMechosBio(p);
				break;
		};
	};
};


float fxlabObjectDistance(const Body& body)
{
	return 1.0;
};

int flabCheckArcane(int id)
{
	switch(id){
		case FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::TITANIUM_BALL))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::SUICIDE))
				return 0;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::CLOWN))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::EVIL_EYE))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::LITTLE_SPIDER))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::CATERPILLAR))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::CRYSTAL))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::BUBBLE))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::TOMATO))
				return 0;			
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FIREBALL:
		case FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT:
			if(Mdisp->searchBody(&fxlabObjectDistance,Body::MISSILE))
				return 0;
			break;
	};

	if(fxlabServerD->SearchObject(id))
		return 0;
	return 1;
};

int fxlabValideArcane(int id)
{
	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD){
		switch(id){
			case ARCANE050:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_DRAGON_WIND);
			case ARCANE051:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_HELICOPTER);
			case ARCANE060:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_DIRIGIBLE_WIND);
			case ARCANE052:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FROG_WIND);
			case ARCANE059:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BUTTERFLY_WIND);
			case ARCANE018:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FORMICARY_WIND);
	//PlasmaClaw
			case ARCANE036:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_PLASMA_CLAW);
	//BowMachine
			case ARCANE032:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BOW_MACHINE);
	//XRAY
			case ARCANE033:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SHAFT_MACHINE);
	//IronSide
			case ARCANE038:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_IRON_SIDE);
	//WindBall
			case ARCANE045:			
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BALL_WIND);
	//Suicide
			case ARCANE042:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR);
	//Clown
			case ARCANE041:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR);
	//Eye
			case ARCANE007:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR);
	//FireBall
			case ARCANE040:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIREBALL);
	//LightningBolt
			case ARCANE066:	
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT);
	//JumpBall
			case ARCANE037:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL);
	//Spiders
			case ARCANE017:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN);
	//Caterpillars
			case ARCANE021:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN);
	//FireStone
			case ARCANE024:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIRE_STONE);
	//SnowFall
			case ARCANE064:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SNOW_FALL);
	//ElecricStorm
			case ARCANE025:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_ELECTRIC_STORM);
	//FireGarden
			case ARCANE003:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIRE_GARDEN);
	//RageSlime
			case ARCANE004:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_RAGE_SLIME);
	//FuryFungus
			case ARCANE005:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FURY_FUNGUS);
	//VoodooMaster
			case ARCANE039:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_VOODOO_MASTER);
	//KineticHit
			case ARCANE034:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_KINETIC_HIT);
	//ElmLight
			case ARCANE029:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_LIGHNING_SEED);
	//CrystalSpike
			case ARCANE009:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE);
	//Revolt
			case ARCANE014:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_REVOLT_SPACE);
	//0-Teleport
			case ARCANE058:			
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_MASS_SHIFTER);
	//Bubbles
			case ARCANE019:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH);
	//Tomato
			case ARCANE012:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR);
	//0-Trap
			case ARCANE048:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SHIFT_TRAP);
	//Mines
			case ARCANE061:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_MORTAR_MINE);
	//Petard
			case ARCANE002:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_PETARD);
	//FireTrack
			case ARCANE011:
				return flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_RED_TRACK);
		};
	};
	return 1;
};

void mchArcaneData::fxlabStart(void)
{ 
	#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "fxlabStart: " <= ID < "\n";
	#endif

	mchRacer* n;
	mchMechosRacer* p;
	fxlabGeneralObjectType* t;

	p = NULL;
	n = mch_raceD->racerLst->first();
	while(n){
		if(n->ID == ownerID){
			if(n->Type == MCH_RACER_TYPE_MECHOS){
				p = static_cast<mchMechosRacer*>(n);
				break;
			}else
				return;
		};
		n = n->next;
	};

	if(!(p->completed()))
		return;

	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD){
		switch(ID){
			case ARCANE050:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_DRAGON_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_DRAGON_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
			case ARCANE051:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_HELICOPTER)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_HELICOPTER);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
			case ARCANE060:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_DIRIGIBLE_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_DIRIGIBLE_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
			case ARCANE052:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FROG_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FROG_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
			case ARCANE059:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BUTTERFLY_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BUTTERFLY_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
			case ARCANE018:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FORMICARY_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FORMICARY_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//PlasmaClaw
			case ARCANE036:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_PLASMA_CLAW)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_PLASMA_CLAW);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//BowMachine
			case ARCANE032:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BOW_MACHINE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BOW_MACHINE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//XRAY
			case ARCANE033:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SHAFT_MACHINE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SHAFT_MACHINE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//IronSide
			case ARCANE038:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_IRON_SIDE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_IRON_SIDE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//WindBall
			case ARCANE045:			
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BALL_WIND)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BALL_WIND);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(3000.0f);
					t->Start();
				};
				break;
	//Suicide
			case ARCANE042:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//Clown
			case ARCANE041:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//Eye
			case ARCANE007:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//FireBall
			case ARCANE040:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIREBALL)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIREBALL);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(5);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//LightningBolt
			case ARCANE066:	
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(5);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//JumpBall
			case ARCANE037:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//Spiders
			case ARCANE017:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(3);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(6000.0f);
					t->Start();
				};
				break;
	//Caterpillars
			case ARCANE021:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(3);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,10000);
					t->SetArcaneLifeTime(6000.0f);
					t->Start();
				};
				break;
	//FireStone
			case ARCANE024:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIRE_STONE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIRE_STONE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(10);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//SnowFall
			case ARCANE064:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SNOW_FALL)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SNOW_FALL);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(10);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//ElecricStorm
			case ARCANE025:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_ELECTRIC_STORM)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_ELECTRIC_STORM);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(5);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//FireGarden
			case ARCANE003:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FIRE_GARDEN)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIRE_GARDEN);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(3);
					t->SetArcaneActionDelay(500);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//RageSlime
			case ARCANE004:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_RAGE_SLIME)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_RAGE_SLIME);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(3);
					t->SetArcaneActionDelay(500);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//FuryFungus
			case ARCANE005:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_FURY_FUNGUS)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FURY_FUNGUS);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(3);
					t->SetArcaneActionDelay(500);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//VoodooMaster
			case ARCANE039:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_VOODOO_MASTER)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_VOODOO_MASTER);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(10);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//KineticHit
			case ARCANE034:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_KINETIC_HIT)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_KINETIC_HIT);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//ElmLight
			case ARCANE029:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_LIGHNING_SEED)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_LIGHNING_SEED);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(2);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetArcaneTime(5000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(2000.0f);
					t->Start();
				};
				break;
	//CrystalSpike
			case ARCANE009:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(5);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetArcaneTime(5000);
					t->SetProtection(p->Body::ID,2000);
					t->SetArcaneLifeTime(2000.0f);
					t->Start();
				};
				break;
	//Revolt
			case ARCANE014:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_REVOLT_SPACE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_REVOLT_SPACE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
	//0-Teleport
			case ARCANE058:			
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_MASS_SHIFTER)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_MASS_SHIFTER);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,0);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
	//Bubbles
			case ARCANE019:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(2);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,2000);
					t->SetArcaneLifeTime(2000.0f);
					t->Start();
				};
				break;
	//Tomato
			case ARCANE012:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(100);
					t->SetProtection(p->Body::ID,2000);
					t->SetArcaneLifeTime(1.0f);
					t->Start();
				};
				break;
			case ARCANE048:
	//0-Trap
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_SHIFT_TRAP)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SHIFT_TRAP);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(1);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,2000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();		
				};
				break;
			case ARCANE061:
	//Mines
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_MORTAR_MINE)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_MORTAR_MINE);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(2);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetArcaneTime(5000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(2000.0f);
					t->Start();
				};
				break;
			case ARCANE002:
	//Petard
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_PETARD)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_PETARD);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(2);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetArcaneTime(5000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(2000.0f);
					t->Start();
				};
				break;
	//FireTrack
			case ARCANE011:
				if(flabCheckArcane(FXLAB_SERVER_PROCESS_ARCANE_RED_TRACK)){
					t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_RED_TRACK);
					t->SetStartTime(fxlabServerD->GetTime());
					t->SetArcaneLink(this);
					t->SetArcaneOwner(p);
					t->SetArcaneInterruptNum(1);
					t->SetArcaneActionPoint(0);
					t->SetArcaneActionDelay(1000);
					t->SetProtection(p->Body::ID,60000);
					t->SetArcaneLifeTime(5000.0f);
					t->Start();
				};
				break;
		};
	}else{
		switch(ID){
			case ARCANE050:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_DRAGON_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(5000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE051:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_HELICOPTER);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(10000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE060:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_DIRIGIBLE_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(16000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE052:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FROG_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE059:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BUTTERFLY_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE018:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FORMICARY_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//PlasmaClaw
			case ARCANE036:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_PLASMA_CLAW);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,round(60000.0f * p -> diff_data().mana_using_speed));
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//BowMachine
			case ARCANE032:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BOW_MACHINE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,round(60000.0f * p -> diff_data().mana_using_speed));
				t->SetArcaneLifeTime(14000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//XRAY
			case ARCANE033:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SHAFT_MACHINE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,round(60000.0f * p -> diff_data().mana_using_speed));
				t->SetArcaneLifeTime(14000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//IronSide
			case ARCANE038:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_IRON_SIDE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//WindBall
			case ARCANE045:			
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BALL_WIND);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(3000.0f);
				t->Start();
				break;
	//Suicide
			case ARCANE042:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,5000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//Clown
			case ARCANE041:			
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,10000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//Eye
			case ARCANE007:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//FireBall
			case ARCANE040:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIREBALL);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(20.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//LightningBolt
			case ARCANE066:	
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(20.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(20000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//JumpBall
			case ARCANE037:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,10000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//Spiders
			case ARCANE017:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(3.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,10000);
				t->SetArcaneLifeTime(6000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//Caterpillars
			case ARCANE021:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(3.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,10000);
				t->SetArcaneLifeTime(6000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//FireStone
			case ARCANE024:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIRE_STONE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(10.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//SnowFall
			case ARCANE064:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SNOW_FALL);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(20.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//ElecricStorm
			case ARCANE025:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_ELECTRIC_STORM);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(10.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//FireGarden
			case ARCANE003:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FIRE_GARDEN);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(3);
				t->SetArcaneActionDelay(500);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//RageSlime
			case ARCANE004:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_RAGE_SLIME);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(3);
				t->SetArcaneActionDelay(500);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//FuryFungus
			case ARCANE005:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_FURY_FUNGUS);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(3);
				t->SetArcaneActionDelay(500);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//VoodooMaster
			case ARCANE039:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_VOODOO_MASTER);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(10);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,60000 * p -> diff_data().mana_using_speed);
				t->SetArcaneLifeTime(30000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//KineticHit
			case ARCANE034:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_KINETIC_HIT);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,60000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//ElmLight
			case ARCANE029:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_LIGHNING_SEED);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(5.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetArcaneTime(30000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(5000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//CrystalSpike
			case ARCANE009:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(5.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetArcaneTime(30000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(5000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//Revolt
			case ARCANE014:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_REVOLT_SPACE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();
				break;
	//0-Teleport
			case ARCANE058:			
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_MASS_SHIFTER);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,0);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
	//Bubbles
			case ARCANE019:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(5.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(10000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
	//Tomato
			case ARCANE012:
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(100);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(1.0f);
				t->Start();
				break;
			case ARCANE048:
	//0-Trap
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SHIFT_TRAP);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(1);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(20000.0f);
				t->Start();		
				break;
			case ARCANE061:
	//Mines
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_MORTAR_MINE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(round(5.0f * p -> diff_data().arcan_using_effectiveness));
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetArcaneTime(30000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(10000.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
			case ARCANE002:
	//Petard
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_PETARD);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(2);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetArcaneTime(30000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(1000.0f);
				t->Start();
				break;
	//FireTrack
			case ARCANE011:			
	/*			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_SCARLET_TRACK);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(30);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetArcaneTime(30000);
				t->SetProtection(p->Body::ID,2000);
				t->Start();*/
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARCANE_RED_TRACK);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetArcaneLink(this);
				t->SetArcaneOwner(p);
				t->SetArcaneInterruptNum(1);
				t->SetArcaneActionPoint(0);
				t->SetArcaneActionDelay(1000);
				t->SetProtection(p->Body::ID,2000);
				t->SetArcaneLifeTime(3500.0f * p -> diff_data().mana_using_speed);
				t->Start();
				break;
		};
	};

/*	switch(ID){
//BioPower
		case ARCANE030:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_LIFE_POD,&fxEventArcaneType(this,p,FX_ID_KEY_LIFE_POD0)));			
			break;

//Nullification
		case ARCANE022:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_NULLIFICATION,&fxEventArcaneType(this,p,FX_ID_KEY_NULLIFICATION0)));
			break;
//RedSkin
		case ARCANE023:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_RED_SKIN,&fxEventArcaneType(this,p,FX_ID_KEY_RED_SKIN0)));
			break;
//Armor
		case ARCANE056:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_ARMOR_SKIN,&fxEventArcaneType(this,p,FX_ID_KEY_ARMOR0)));
			break;
//Isolation
		case ARCANE057:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_ISOLATION_SKIN,&fxEventArcaneType(this,p,FX_ID_KEY_ISOLATION0)));
			break;

//Attraction
		case ARCANE020:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_ATTRACTION_CLUSTER,&fxEventArcaneType(this,p,FX_ID_KEY_ATTRACTION_CLUSTER0)));
			break;
//Twister
		case ARCANE016:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_HARD_TWISTER,&fxEventArcaneType(this,p,FX_ID_KEY_TWISTER0)));
			break;
//MagneticField
		case ARCANE015:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_PARA_FORCE,&fxEventArcaneType(this,p,FX_ID_KEY_PARA_FORCE0)));
			break;
//Mines
		case ARCANE061:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_MORTAR_LAUNCHER,&fxEventArcaneType(this,p,FX_ID_KEY_MORTAR_LAUNCHER0)));
			break;
//Petard
		case ARCANE002:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_CRAZY_PETARD,&fxEventArcaneType(this,p,FX_ID_KEY_CRAZY_PETARD0)));
			break;
//FireTrack
		case ARCANE011:
			fxContext->Event(FX_CORPUS_CONTEXT_EVENT_CREATE,&fxCorpusContextEventCreateType(FX_PROCESS_STATE_FIRE_TRACK,&fxEventArcaneType(this,p,FX_ID_KEY_FIRE_TRACK0)));
			break;
//0-Trap
	};
*/
};

void mchArcaneData::fxlabStop(void)
{
	if(Interruption && !Status)
		fxlabServerD->StopArcane(ID,ownerID);
};

void fxlabRacerEvent(int type,struct mchRacer* point)
{
	mchMechosRacer* mp;
//	fxlabGeneralObjectType* t;	

	switch(type){
		case FX_RACER_EVENT_FINISH:
			if(point->Type == MCH_RACER_TYPE_MECHOS){
				mp = static_cast<mchMechosRacer*>(point);
/*
				mp->set_feature(Mechos::NON_DESTRUCTING);
				mp->set_feature(Mechos::NON_CONSTRUCTING);
				mp->discharge();
*/				
				mp->startEffect(Mechos::ExultEffect);

/*				if(mp->active()){
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WORLD_IRRADIATE);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetPosition(Vect3f(mp->R()));
					t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE);
					t->SetMechosPoint(mp);
					t->Start();
				};*/
			};
			break;
	};
};

void fxlabClownProcess(Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetKeyID(FXLAB_ID_KEY_CLOWN);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(v);
	t->Start();
};

void fxlabTomatoBurst(Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TOMATO_BURST);
	t->SetPosition(v);
	t->Start();
};

void fxlabGreenSlime(const Vect3f& v_, const MatXf& mat_)
{
	Vect3f v(v_);
	MatXf mat(mat_);

	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ANIMATION_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_GREEN_SLIME_MODEL);
//	t->SetPosition(Vect3f(v));
	t->SetMatrix(mat);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_GREEN_SLIME_BODY);
	t->SetMatrix(mat);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_GREEN_SLIME_CIRCLE);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_GREEN_SLIME_CIRCLE);
	t->SetPosition(Vect3f(v));
	t->Start();
};

void fxlabYellowSlime1(const Vect3f& v_, const MatXf& mat_)
{
	Vect3f v(v_);
	MatXf mat(mat_);
	
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ANIMATION_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_YELLOW_SLIME_MODEL);
//	t->SetPosition(Vect3f(v));
	t->SetMatrix(mat);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_YELLOW_SLIME_BODY);
	t->SetMatrix(mat);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_YELLOW_SLIME_CIRCLE);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_WORM_YELLOW_SLIME_CIRCLE);
	t->SetPosition(Vect3f(v));
	t->Start();
};


void fxlabChargeMana(class mchMechosRacer* p,float power)
{
	mchArcaneData* t;
	cMesh* n;
	int i;

	if(!p)
		return;

	for(i = 1;i < NUM_ACTIVE_PARTS;i++){
		n = p->part_by_index(i);
		if(n){
			t = &(n->arcaneData);
			if(t->Power < t->MaxPower){
				t->Power += power;
				if(t->Power > t->MaxPower)
					t->Power = t->MaxPower;
			};
		};
/*		t = &(p->part_by_index(i)->arcaneData);
//		if(t->Status){
			if(t->Power < t->MaxPower){
				t->Power += power;
				if(t->Power > t->MaxPower)
					t->Power = t->MaxPower;
			};
//		};*/
	};
};

extern int mchTurnBasedGame;

void fxlabCreateController(void)
{
	mchRacer* mp;
	fxlabGeneralObjectType* t;

	mp = mch_raceD->racerLst->first();
	while(mp){
		if(mp->Type == MCH_RACER_TYPE_MECHOS){

			if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD)
				static_cast<mchMechosRacer*>(mp)->set_feature(Mechos::PROTECT_FROM_DAMAGE);

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->Start();

//Front
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PART_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->SetControllerPartID(1);
			t->SetControllerArcaneID(1);
			t->SetKeyID(FXLAB_ID_KEY_ARCANE_INDICATOR);
			t->Start();

//Forward
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PART_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->SetControllerPartID(2);
			t->SetControllerArcaneID(2);
			t->SetKeyID(FXLAB_ID_KEY_ARCANE_INDICATOR);
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PART_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->SetControllerPartID(4);
			t->SetControllerArcaneID(2);
			t->SetKeyID(FXLAB_ID_KEY_ARCANE_INDICATOR);
			t->Start();

//Backward
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PART_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->SetControllerPartID(3);
			t->SetControllerArcaneID(3);
			t->SetKeyID(FXLAB_ID_KEY_ARCANE_INDICATOR);
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PART_CONTROLLER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->SetControllerPartID(5);
			t->SetControllerArcaneID(3);
			t->SetKeyID(FXLAB_ID_KEY_ARCANE_INDICATOR);
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPLINE_WAY);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetMechosPoint(static_cast<mchMechosRacer*>(mp));
			t->Start();
		};
		mp = mp->next;
	};
};

void fxlabMechosFireProcess(Mechos* p)
{
	fxlabGeneralObjectType* t;
	mchMechosRacer* m;

	if(!FXLAB_MECHOS_IN_WATER(p)){
		if(!(p->fxlabMechosInfo.FireTimer())){
			m = dynamic_cast<mchMechosRacer*>(p);
			if(m){
				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_MECHOS_FIRE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetMechosPoint(m);
				t->SetChargeValue(33.0f);
				t->SetChargeTime(3000.0f);
				t->Start();
			};
		};
		p->fxlabMechosInfo.FireTimer.start(1000);
	};
};

void fxlabMechosFireDestruction(Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_BURN_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetBody(p);
	t->Start();
};

void fxlabMechosGlowDestruction(Mechos* p)
{
	int i;
	cMesh* mp;
	fxlabGeneralObjectType* t;
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	const int mesh_id[NUM_PARTS] = { M3D_ENGINE,M3D_FRONT,M3D_RF_WHEEL,M3D_RB_WHEEL,M3D_LF_WHEEL,M3D_LB_WHEEL};

	for(i = 0;i < NUM_PARTS;i++){
		mp = p->part_by_index(i);

		if(mp && p->fxlabMechosInfo.ColorData[i].ColorEnable){
			p->fxlabMechosInfo.ColorData[i].ColorEnable = 0;
			p->fxlabMechosInfo.ColorData[i].Red = 0;
			p->fxlabMechosInfo.ColorData[i].Green = 0;
			p->fxlabMechosInfo.ColorData[i].Blue = 0;
			p->fxlabMechosInfo.ColorData[i].RGBACount = 0;
			p->fxlabMechosInfo.ColorData[i].Alpha = 0;
//			IVisGeneric->SetObjectColor((cUnknownClass*)(mp),&(p->fxlabMechosInfo.ColorData[i].Color0),&(p->fxlabMechosInfo.ColorData[i].Color1),mesh_id[i]);
//			p->setColor(i,&(p->fxlabMechosInfo.ColorData[i].Color0),&(p->fxlabMechosInfo.ColorData[i].Color1));
			p->setColor(i,&sColor4f(1.0f,1.0f,1.0f,1.0f),&sColor4f(0,0,0,1.0f));
		};
	};
	IVisGeneric->Release();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_BURN_SHOCK_WAVE);
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_BURN_OMNI);
	t->SetPosition(Vect3f(p->R()));
	t->Start();

//	fxlabCreateSplinter(31,0,Vect3f(p->R()) + Vect3f(0,0,p->radius()),9,fxlabServerRND.UnitRND()*M_PI,M_PI*0.2f,M_PI * 0.1f,30.0f,10.0f,-1,-1,FXLAB_CLIENT_PROCESS_BODY_LINK,FXLAB_ID_KEY_MECHOS_BURN_SPLINTER_LINK,NULL,NULL);
};

fxlabMechosColorInformation::fxlabMechosColorInformation(void)
{
	RGBACount = 0;
	Red = Green = Blue = Alpha = 0;
	ColorEnable = 0;
};

fxlabMechosProcessInformation::fxlabMechosProcessInformation(void) : 
	FireTimer(*new DurationTimer) , 
	DustTimer(*new DurationTimer) ,
	EnergyTimer(*new DurationTimer) ,
	ManaTimer(*new DurationTimer) ,
	WaveTimer(*new DurationTimer) ,
	DamageTimer(*new DurationTimer) ,
	JumpTimer(*new DurationTimer) ,
	NitroTimer(*new DurationTimer)
{
	WaterEnable = 0;
};


fxlabMechosProcessInformation::~fxlabMechosProcessInformation(void)
{
	if(ArmorPoint.Process){
		ArmorPoint.Process->SetAlive(0);
	};

	if(SkinPoint.Process){
		SkinPoint.Process->SetAlive(0);
	};

	if(IsolationPoint.Process){
		SkinPoint.Process->SetAlive(0);
	};

	if(BioPoint.Process){
		BioPoint.Process->SetAlive(0);
	};

	if(ArmorServerPoint.Process){
		ArmorServerPoint.Process->SetAlive(0);
	};

	if(SkinServerPoint.Process){
		SkinServerPoint.Process->SetAlive(0);
	};

	if(IsolationServerPoint.Process){
		IsolationServerPoint.Process->SetAlive(0);
	};

	if(BioServerPoint.Process){
		BioServerPoint.Process->SetAlive(0);
	};

	delete &WaveTimer;
	delete &FireTimer;
	delete &DustTimer;
	delete &EnergyTimer;
	delete &ManaTimer;
	delete &DamageTimer;
	delete &JumpTimer;
	delete &NitroTimer;
};

void fxlabMechosArmor(Mechos* p)
{
	fxlabGeneralObjectType* t;

	if(!(p->fxlabMechosInfo.ArmorPoint.Process)){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_ARMOR);
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.ArmorPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.ArmorPoint.Process->SetStartTime(fxlabClientD->GetTime());
	};

	if(!(p->fxlabMechosInfo.ArmorServerPoint.Process)){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ARMOR_PROTECTION);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.ArmorServerPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.ArmorServerPoint.Process->SetStartTime(fxlabServerD->GetTime());
	};
};


void fxlabMechosBio(Mechos* p)
{
	fxlabGeneralObjectType* t;

	if(!(p->fxlabMechosInfo.BioPoint.Process)){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_BIO);
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.BioPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.BioPoint.Process->SetStartTime(fxlabClientD->GetTime());
	};

	if(!(p->fxlabMechosInfo.BioServerPoint.Process)){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BIO_PROTECTION);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.BioServerPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.BioServerPoint.Process->SetStartTime(fxlabServerD->GetTime());
	};
};

void fxlabMechosSkin(Mechos* p)
{
	fxlabGeneralObjectType* t;

	if(!(p->fxlabMechosInfo.SkinPoint.Process)){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_SKIN);
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.SkinPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.SkinPoint.Process->SetStartTime(fxlabClientD->GetTime());
	};

	if(!(p->fxlabMechosInfo.SkinServerPoint.Process)){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_SKIN_PROTECTION);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.SkinServerPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.SkinServerPoint.Process->SetStartTime(fxlabServerD->GetTime());
	};
};

void fxlabMechosIsolation(Mechos* p)
{
	fxlabGeneralObjectType* t;

	if(!(p->fxlabMechosInfo.IsolationPoint.Process)){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_ISOLATION);
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.IsolationPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.IsolationPoint.Process->SetStartTime(fxlabClientD->GetTime());
	};

	if(!(p->fxlabMechosInfo.IsolationServerPoint.Process)){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_ISOLATION_PROTECTION);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
		t->SetProcessInterface(&(p->fxlabMechosInfo.IsolationServerPoint));
		t->Start();
	}else{
		p->fxlabMechosInfo.IsolationServerPoint.Process->SetStartTime(fxlabServerD->GetTime());
	};
};

void fxlabBubbleBurst(Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BUBBLE_RUSH_BURST);
	t->SetPosition(v);
	t->Start();
};

//-------------------------------------------

fxlabTeleportInterface::fxlabTeleportInterface(void)
{
	Status = 0;
	WorldID = 0;
	TrackID = 0;
	Position = Vect3f::ZERO;
};

void fxlabTeleportInterface::Init(class Vect3f& p,int status,int world_id,int track_id)
{
	Status = status;
	WorldID = world_id;
	TrackID = track_id;
	Position = p;
};

const int FXLAB_INTERFACE_TELEPORT_INACCESSIBLE = 0;
const int FXLAB_INTERFACE_TELEPORT_ACCESSIBLE = 1;
const int FXLAB_INTERFACE_TELEPORT_OPEN = 2;
const int FXLAB_INTERFACE_TELEPORT_PURCHASE = 3;

void fxlabTeleportInterface::Open(void)
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	v = Position;
	if(fxlabGetWorldReflectionEnable() && GetW(XCYCL(round(v.x)),YCYCL(round(v.y))))
		v.z = vMap->LevelH2O + 2.5f;
	else
		v.z = ZV(XCYCL(round(v.x)),YCYCL(round(v.y))) + 2.5f;

	if(!ClientPoint1.Process && !ClientPoint2.Process){
		switch(Status){
			case FXLAB_INTERFACE_TELEPORT_INACCESSIBLE:
/*				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_PASSIVE_MODEL);
				t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint1);
				t->Start();*/
				break;
			case FXLAB_INTERFACE_TELEPORT_ACCESSIBLE:
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_ACTIVE_MODEL);
				t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint1);
				t->Start();
				break;
			case FXLAB_INTERFACE_TELEPORT_OPEN:
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_ACTIVE_MODEL);
				t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint1);
				t->Start();

				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint2);
				t->Start();
				break;
			case FXLAB_INTERFACE_TELEPORT_PURCHASE:
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_PASSIVE_MODEL);
				t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint1);
				t->Start();

/*				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
				t->SetPosition(v);
				t->SetProcessInterface(&ClientPoint2);
				t->Start();*/
				break;
		};
	}else
		ErrH.Abort("Bad Open fxlabTeleportInterface");

	if(!ServerPoint.Process){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_STATIC_TELEPORT);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetPosition(v);
		t->SetKeyID(FXLAB_ID_KEY_SERVER_TELEPORT_MENU);
		t->SetMenuTeleportStatus(Status);
		t->SetMenuTeleportInfo(WorldID,TrackID);
		t->SetProcessInterface(&ServerPoint);
		t->Start();
	}else
		ErrH.Abort("Bad Open fxlabTeleportInterface");
};

void fxlabTeleportInterface::Close(void)
{
	if(ClientPoint1.Process)
		ClientPoint1.Process->SetAlive(0);

	if(ClientPoint2.Process)
		ClientPoint2.Process->SetAlive(0);

	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);
};

void fxlabTeleportInterface::SetStatus(int status)
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	if(ServerPoint.Process){
		ServerPoint.Process->SetMenuTeleportStatus(status);

		if(Status != status){
			if(ClientPoint1.Process)
				ClientPoint1.Process->SetAlive(0);

			if(ClientPoint2.Process)
				ClientPoint2.Process->SetAlive(0);

			v = Position;

			if(fxlabGetWorldReflectionEnable() && GetW(XCYCL(round(v.x)),YCYCL(round(v.y))))
				v.z = vMap->LevelH2O + 2.5f;
			else
				v.z = ZV(XCYCL(round(v.x)),YCYCL(round(v.y))) + 2.5f;

			switch(status){
				case FXLAB_INTERFACE_TELEPORT_INACCESSIBLE:
/*					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_PASSIVE_MODEL);
					t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint1);
					t->Start();*/
					break;
				case FXLAB_INTERFACE_TELEPORT_ACCESSIBLE:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_ACTIVE_MODEL);
					t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint1);
					t->Start();
					break;
				case FXLAB_INTERFACE_TELEPORT_OPEN:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_ACTIVE_MODEL);
					t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint1);
					t->Start();

					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint2);
					t->Start();
					break;
				case FXLAB_INTERFACE_TELEPORT_PURCHASE:
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_MENU_PASSIVE_MODEL);
					t->SetMatrix(getPlacementGroundPose(Vect3f(Position.x,Position.y,0)));
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint1);
					t->Start();

/*					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
					t->SetPosition(v);
					t->SetProcessInterface(&ClientPoint2);
					t->Start();*/
					break;
			};
		};
	};
	Status = status;
};

//------------------------------------------

void fxlabTeleportDispatcher::Init(void)
{
	fxlabTeleportList.resize(100);
};

void fxlabTeleportDispatcher::Finit(void)
{
	fxlabTeleportListType::iterator p;

	FOR_EACH(fxlabTeleportList,p){
		(*p).second.clear();
	};
	fxlabTeleportList.clear();
};

void fxlabTeleportDispatcher::Open(void)
{
	fxlabTeleportListType::iterator p;
	hash_map<int,fxlabTeleportInterface>::iterator pp;

	if(mchCurrentWorld == 7){
		FOR_EACH(fxlabTeleportList,p){
			FOR_EACH(((*p).second),pp){
				(*pp).second.Open();
			};
		};
	};
};

void fxlabTeleportDispatcher::Close(void)
{
	fxlabTeleportListType::iterator p;
	hash_map<int,fxlabTeleportInterface>::iterator pp;

	FOR_EACH(fxlabTeleportList,p){
		FOR_EACH(((*p).second),pp){
			(*pp).second.Close();
		};
	};
};

void fxlabTeleportDispatcher::Add(int world_id,int track_id,class Vect3f& p,int status)
{
	Search(world_id,track_id)->Init(p,status,world_id,track_id);
};

void fxlabTeleportDispatcher::SetState(int world_id,int track_id,int status)
{
	Search(world_id,track_id)->SetStatus(status);
};

fxlabTeleportInterface* fxlabTeleportDispatcher::Search(int world_id,int track_id)
{
	return (&(fxlabTeleportList[world_id][track_id]));
};


//--------------------------------

void fxlabCreateTeleport(int world_id,int track_id,class Vect3f& p,int status)
{
	fxlabTeleportD->Add(world_id,track_id,p,status);
};

void fxlabChangeTeleport(int world_id,int track_id,int status)
{
	fxlabTeleportD->SetState(world_id,track_id,status);
};

void fxlabDustSource(const class Vect3f& p,float speed,int terrain_type,class Mechos* owner)
{
	fxlabGeneralObjectType* t;
	int key_id;

	if(speed > 0.001f){
		if(speed >= 1.0f){
			if(!(owner->fxlabMechosInfo.DustTimer())){
				key_id = fxlabGetDustID(GetTer(XCYCL(p.xi()),YCYCL(p.yi())));
				if(key_id >= 0){
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(key_id);
					t->SetPosition(Vect3f(p));
					t->Start();
					owner->fxlabMechosInfo.DustTimer.start(20);
				};
			};
		}else{
			if(!(owner->fxlabMechosInfo.DustTimer())){
				key_id = fxlabGetDustID(GetTer(XCYCL(p.xi()),YCYCL(p.yi())));
				if(key_id >= 0){
					t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
					t->SetStartTime(fxlabClientD->GetTime());
					t->SetKeyID(key_id);
					t->SetPosition(Vect3f(p));
					t->Start();
					if(terrain_type == 2)
						owner->fxlabMechosInfo.DustTimer.start(100 + round(100.0f * (1.0f - speed)));
					else
						owner->fxlabMechosInfo.DustTimer.start(30 + round(30.0f * (1.0f - speed)));
				};
			};
		};
	};
};

void fxlabDamageWarinig(Vect3f& v,class Mechos* owner,int damage_style)
{
	fxlabGeneralObjectType* t;
	mchMechosRacer* mp;

	if(!(owner->fxlabMechosInfo.DamageTimer())){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_EXPLODE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_DAMAGE_WARNING_IMPULSE);
		t->SetPosition(v);
		t->Start();
	
/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_DAMAGE);
		t->SetPosition(v);
		t->Start();*/

		if(damage_style == Mechos::LIGHTNING_DAMAGE){
			mp = dynamic_cast<mchMechosRacer*>(owner);
			if(mp){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_LIGHTNING_DAMAGE);
				t->SetMechosPoint(mp);
				t->Start();
			};
		};

		owner->fxlabMechosInfo.DamageTimer.start(200);
	};
};

void fxlabBubbleSource(class Mechos* owner,float speed)
{
	fxlabGeneralObjectType* t;

	if(speed < 1.0f){
		if(!(owner->fxlabMechosInfo.DustTimer())){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_MECHOS_BUBBLE);
			t->SetPosition(Vect3f(owner->R()));
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);
			t->SetPosition(Vect3f(owner->R()));
			t->SetColorVector(Vect3f(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue()));
			t->SetTransparency(fxlabGetTrackWaveAlpha());
			t->Start();

			owner->fxlabMechosInfo.DustTimer.start(100 + 500 * (1.0f - speed));
		};
	}else{
		if(!(owner->fxlabMechosInfo.DustTimer())){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_MECHOS_BUBBLE);
			t->SetPosition(Vect3f(owner->R()));
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);
			t->SetPosition(Vect3f(owner->R()));
			t->SetColorVector(Vect3f(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue()));
			t->SetTransparency(fxlabGetTrackWaveAlpha());
			t->Start();

			owner->fxlabMechosInfo.DustTimer.start(100);
		};
	};
};


void fxlabStoneDust(Vect3f& p,float r)
{
	fxlabSwarmDomain pos,vel;
	fxlabGeneralObjectType* t;

	pos.Sphere(0,0,0,r,r);
	vel.Sphere(0,0,0,1.0f,0);

	if(mchCurrentTrack == 4){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR_EXTERNAL);
		t->SetStartTime(fxlabClientD->GetTime());	
		t->SetKeyID(FXLAB_ID_KEY_STONE_WHITE_DUST);
		t->SetPosition(p);
		t->SetPositionDomain(&pos);
		t->SetVelocityDomain(&vel);
		t->Start();
	}else{
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR_EXTERNAL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_STONE_DUST);
		t->SetPosition(p);
		t->SetPositionDomain(&pos);
		t->SetVelocityDomain(&vel);
		t->Start();
	};
};

void fxlabJumpSource(class Mechos* owner)
{
	fxlabGeneralObjectType* t;

	if(!(owner->fxlabMechosInfo.JumpTimer())){
/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_JUMP_LEFT);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_JUMP_ACTION_LINK);
		t->SetBody(owner);
		t->Start();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_JUMP_RIGHT);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_JUMP_ACTION_LINK);
		t->SetBody(owner);
		t->Start();*/
		
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_JUMP_ACTION_LINK);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_JUMP_ACTION_TRACK);
		t->SetBody(owner);
		t->Start();

/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_BONUS_JUMP);
		t->SetPosition(Vect3f(owner->R()));
		t->Start();*/

		owner->fxlabMechosInfo.JumpTimer.start(1000);
	};
};
 
void fxlabNitroSource(class Mechos* owner)
{
	fxlabGeneralObjectType* t;

	if(!(owner->fxlabMechosInfo.NitroTimer())){
/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_NITRO_LEFT);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_NITRO_ACTION_LINK);
		t->SetBody(owner);
		t->Start();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_ACTION_NITRO_RIGHT);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_NITRO_ACTION_LINK);
		t->SetBody(owner);
		t->Start();*/

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_NITRO_ACTION_LINK);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_NITRO_ACTION_TRACK);
		t->SetBody(owner);
		t->Start();

/*		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_BONUS_NITRO);
		t->SetPosition(Vect3f(owner->R()));
		t->Start();*/

		owner->fxlabMechosInfo.NitroTimer.start(1000);
	};
};

void fxlabBubbleSource(Vect3f& pos,DurationTimer& timer)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_BUBBLE);
	t->SetPosition(pos);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);
	t->SetPosition(pos);
	t->SetColorVector(Vect3f(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue()));
	t->SetTransparency(fxlabGetTrackWaveAlpha());
	t->Start();

	timer.start(250);
};

void fxlabWhaleWaveSource(const Vect3f& v)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_BUBBLE);
	t->SetPosition((Vect3f)v);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_WAVE_MODEL);
	t->SetPosition((Vect3f)v);
	t->SetColorVector(Vect3f(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue()));
	t->SetTransparency(fxlabGetTrackWaveAlpha());
	t->Start();
};

void fxlabSpikeBurst(class Body* p)
{
	Vect3f v;
	fxlabSwarmDomain pos;
	fxlabSwarmDomain vel;
	fxlabGeneralObjectType* t;

	v = p->Yglobal() * Vect3f(0,0,50);
	pos.Cone(v.x,v.y,v.z,-v.x,-v.y,-v.z,8.0f,0);
	vel.Sphere(0,0,0,0.1f,5.0f);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR_EXTERNAL);
	t->SetKeyID(FXLAB_ID_KEY_SPIKE_BURST);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->SetPositionDomain(&pos);
	t->SetVelocityDomain(&vel);
	t->Start();
};

//----------------------------------------------

void fxlabVolcanicExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetKeyID(FXLAB_ID_KEY_STONE_EXPLOSION_SMOKE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_STONE_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_STONE_EXPLOSION);
	t->SetPosition(Vect3f(p->R()));
	t->Start();
	t->Quant();
};


void fxlabBombExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_SHOCK_WAVE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BOMB_EXPLOSION);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION);
	t->SetPosition(Vect3f(p->R()));
	t->Start();
	t->Quant();
};

void fxlabSnowBallExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetKeyID(FXLAB_ID_KEY_SNOW_FALL_EXPLOSION);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();
};

void fxlabTitaniumExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;
	OwnerProtection* op;
	ArcaneStatisticsInfo* sp;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_SHOCK_WAVE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	op = dynamic_cast<OwnerProtection*>(p);
	sp = dynamic_cast<ArcaneStatisticsInfo*>(p);

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION);
	t->SetPosition(Vect3f(p->R()));
	if(op) 
		t->SetProtection(op->owner(), op->duration());
	if(sp) 
		t->SetStatistic(sp->GetOwnerID(),sp->GetArcaneID(),sp->GetArcaneOwner(),sp->GetObjectID());
	t->Start();
	t->Quant();
};

void fxlabBolideExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;
	OwnerProtection* op;
	ArcaneStatisticsInfo* sp;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_EXPLODE);
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION_SMOKE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	op = dynamic_cast<OwnerProtection*>(p);
	sp = dynamic_cast<ArcaneStatisticsInfo*>(p);

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION);
	t->SetPosition(Vect3f(p->R()));
	if(op) 
		t->SetProtection(op->owner(), op->duration());
	if(sp) 
		t->SetStatistic(sp->GetOwnerID(),sp->GetArcaneID(),sp->GetArcaneOwner(),sp->GetObjectID());
	t->Start();
	t->Quant();
};

void fxlabFireBallExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;
	OwnerProtection* op;
	ArcaneStatisticsInfo* sp;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_EXPLODE);
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION_SMOKE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	op = dynamic_cast<OwnerProtection*>(p);
	sp = dynamic_cast<ArcaneStatisticsInfo*>(p);

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION);
	t->SetPosition(Vect3f(p->R()));
	if(op)
		t->SetProtection(op->owner(), op->duration());
	if(sp) 
		t->SetStatistic(sp->GetOwnerID(),sp->GetArcaneID(),sp->GetArcaneOwner(),sp->GetObjectID());
	t->Start();
	t->Quant();
};

void fxlabLightningBoltExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;
	OwnerProtection* op;
	ArcaneStatisticsInfo* sp;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_FIRE_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	op = dynamic_cast<OwnerProtection*>(p);
	sp = dynamic_cast<ArcaneStatisticsInfo*>(p);

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SERVER_LIGHTNING_BOLT_DAMAGE);
	t->SetPosition(Vect3f(p->R()));
	if(op) 
		t->SetProtection(op->owner(),op->duration());
	if(sp) 
		t->SetStatistic(sp->GetOwnerID(),sp->GetArcaneID(),sp->GetArcaneOwner(),sp->GetObjectID());
	t->Start();
	t->Quant();
};

void fxlabMortarExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_SHOCK_WAVE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();
};

void fxlabMinorBonusExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_SHOCK_WAVE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();
};

void fxlabMajorBonusExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetKeyID(FXLAB_ID_KEY_PETARD_BONUS);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();
};

void fxlabSuicideExplosion(class Body* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_SHOCK_WAVE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BOMB_EXPLOSION_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(p->R()));
	t->Start();
};

//---------------------------------------------------------

void fxlabBonusFire(const Vect3f& v_)
{
	Vect3f v(v_);
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetKeyID(FXLAB_ID_KEY_BONUS_FIRE_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(v);
	t->Start();


	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_DUST);
	t->SetKeyID(FXLAB_ID_KEY_BONUS_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(v);
	t->Start();
};

//--------------------------------------------------------

void fxlabCharacterTeleportation(Vect3f v0,Vect3f v1)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_CHARACTER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_PRIMARY_MASS_SHIFTER_CHARACTER);
	t->SetPosition(v0);
	t->Start();

/*	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_CHARACTER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SECONDARY_MASS_SHIFTER_CHARACTER);
	t->SetPosition(v1);
	t->Start();*/

/*	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI_CHARACTER);
	t->SetPosition(v0);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI_CHARACTER);
	t->SetPosition(v1);
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_START_TELEPORT);
	t->SetPosition(v0);
	t->Start();	
};

void fxlabDragonRose(const Vect3f& pos,const Vect3f& vel)
{
	Vect3f v;
	fxlabGeneralObjectType* t;

	v = pos;
	v += vel * 70.0f;
	v.z = fxlabGetLevel(pos.x,pos.y,pos.z);
	v = CYCLE(v);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_OBJECT_SET);
	t->SetKeyID(FXLAB_ID_KEY_DRAGON_ROSE_SET);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(v);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_DRAGON_ROSE_DAMAGE);
	t->SetPosition(v);
	t->Start();
};


/*int fxlabGetLevelOver(int x,int y,float z)
{
	int fz = triangleMapH(x, y, z);
	if(fz < vMap->LevelH2O && (GetAt(XCYCL(x),YCYCL(y)) & At_WATER))
		fz = vMap->LevelH2O;
	return fz;
};
*/

void fxlabSnowBallAction(class Body* p)
{
	fxlabGeneralObjectType* t;
	mchMechosRacer* m;

	m = dynamic_cast<mchMechosRacer*>(p);
	if(m){
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_MECHOS_FROZEN);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetMechosPoint(m);
		t->Start();
	};
};

//-----------------------------------------

void fxlabMovieUpdateProcess(const char* name,Vect3f& position,Vect3f& velocity,int status)
{	
	fxlabMovieD->UpdateProcess(name,position,velocity,status);
};

void fxlabMovieDestroy(void)
{
	fxlabMovieD->Clear();
};

void fxlabMovieDispacther::Open(void)
{
	MovieList.resize(300);
	RegisterName("Kron_fire_first",FXLAB_CLIENT_PROCESS_SET_FACE_FIRE,FXLAB_ID_KEY_MOVIE_HEAD_SET);
	RegisterName("kron_flash",FXLAB_CLIENT_PROCESS_MOVIE_BOW_REPEATER,FXLAB_ID_KEY_MOVIE_STORM_CONTROL);
	RegisterName("Kron_Water1",FXLAB_CLIENT_PROCESS_MOVIE_WAVE_REPEATER,FXLAB_ID_KEY_MOVIE_WAVE_CONTROL);
	RegisterName("Kron_Water2",FXLAB_CLIENT_PROCESS_MOVIE_WAVE_REPEATER,FXLAB_ID_KEY_MOVIE_WAVE_CONTROL);
	RegisterName("Death_Fog_1",FXLAB_CLIENT_PROCESS_MOVIE_CLOUDS,-1);
	RegisterName("Death_Fog_2",FXLAB_CLIENT_PROCESS_MOVIE_CLOUDS,-1);
	RegisterName("Death_Fog_3",FXLAB_CLIENT_PROCESS_MOVIE_CLOUDS,-1);
	RegisterName("Quad_Lightning_1",FXLAB_CLIENT_PROCESS_MOVIE_QUAD_LIGHT,-1);
	RegisterName("Quad_Lightning_2",FXLAB_CLIENT_PROCESS_MOVIE_QUAD_LIGHT,-1);	
	RegisterName("TELEPORT1",FXLAB_CLIENT_PROCESS_MOVIE_SHIFTING,-1);
	RegisterName("TELEPORT2",FXLAB_CLIENT_PROCESS_MOVIE_SHIFTING,-1);
	
	RegisterName("BossSpot01",FXLAB_CLIENT_PROCESS_MOVIE_SPOT,-1);
	RegisterName("BossSpot02",FXLAB_CLIENT_PROCESS_MOVIE_SPOT,-1);
	RegisterName("BossSpot03",FXLAB_CLIENT_PROCESS_MOVIE_SPOT,-1);
	RegisterName("BossSpot04",FXLAB_CLIENT_PROCESS_MOVIE_SPOT,-1);
	RegisterName("BossSpot05",FXLAB_CLIENT_PROCESS_MOVIE_SPOT,-1);

	RegisterName("Teleporting01",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting02",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting03",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting04",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting05",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting06",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting07",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting08",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting09",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting10",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);

	RegisterName("Teleporting11",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting12",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting13",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting14",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting15",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting16",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting17",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting18",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting19",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting20",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);

	RegisterName("Teleporting21",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting22",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting23",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting24",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting25",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting26",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting27",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting28",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);
	RegisterName("Teleporting29",FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT,-1);

	RegisterName("Fire1",FXLAB_CLIENT_PROCESS_FIRE_TREE_MOVIE,-1);
	RegisterName("Fire2",FXLAB_CLIENT_PROCESS_FIRE_TREE_MOVIE,-1);
	RegisterName("Fire3",FXLAB_CLIENT_PROCESS_FIRE_TREE_MOVIE,-1);
	RegisterName("Fire4",FXLAB_CLIENT_PROCESS_FIRE_TREE_MOVIE,-1);

	RegisterName("Morph",FXLAB_CLIENT_PROCESS_MOVIE_BOSS_CHANGE,-1);

	RegisterName("BossFlash1",FXLAB_CLIENT_PROCESS_MOVIE_BOW_REPEATER,FXLAB_ID_KEY_MOVIE_STORM_CONTROL);
	RegisterName("BossFlash2",FXLAB_CLIENT_PROCESS_MOVIE_BOW_REPEATER,FXLAB_ID_KEY_MOVIE_STORM_CONTROL);
	RegisterName("BossBurst1",FXLAB_CLIENT_PROCESS_BOSS_WORLD_EXPLOSION,-1);
	RegisterName("BossBurst2",FXLAB_CLIENT_PROCESS_BOSS_WORLD_EXPLOSION,-1);

	RegisterName("FireA0",FXLAB_CLIENT_PROCESS_MOVIE_DRAGON_FIRE,-1);
	RegisterName("FireB0",FXLAB_CLIENT_PROCESS_MOVIE_DRAGON_FIRE,-1);
	RegisterName("FireC0",FXLAB_CLIENT_PROCESS_MOVIE_DRAGON_FIRE,-1);

	RegisterName("Rocket_FireA0",FXLAB_CLIENT_PROCESS_MOVIE_ROCKET_FIRE,-1);
	RegisterName("Rocket_FireA1",FXLAB_CLIENT_PROCESS_MOVIE_ROCKET_FIRE,-1);

//	RegisterName("TELEPORT1",FXLAB_CLIENT_PROCESS_TELEPORT_OMNI_CONTROLLER,FXLAB_ID_KEY_MENU_TELEPORT_OMNI);
//	RegisterName("TELEPORT2",FXLAB_CLIENT_PROCESS_TELEPORT_OMNI_CONTROLLER,FXLAB_ID_KEY_MENU_TELEPORT_OMNI);
//	RegisterName("Quad_Lightning_1",FXLAB_CLIENT_PROCESS_OBJECT_SET,FXLAB_ID_KEY_QUAD_SET);
//	RegisterName("Quad_Lightning_2",FXLAB_CLIENT_PROCESS_OBJECT_SET,FXLAB_ID_KEY_QUAD_SET);	
//	RegisterName("Death_Fog",FXLAB_CLIENT_PROCESS_TWISTER,FXLAB_ID_KEY_MOVIE_CLOUDS);
//	RegisterName("LightningTest",FXLAB_CLIENT_PROCESS_LIGHTNING_LINE,FXLAB_ID_KEY_MOVIE_LIGHTNING_TEST);
};

void fxlabMovieDispacther::Close(void)
{
	Clear();
	MovieList.clear();
};

void fxlabMovieDispacther::Clear(void)
{
	hash_map<const char*,fxlabMovieDataType,hash<const char*>,fxlabMovieEqStr>::iterator i_movie;

	FOR_EACH(MovieList,i_movie){
		if(i_movie->second.ProcessPoint.Process)
			i_movie->second.ProcessPoint.Process->SetAlive(0);
	};
};

void fxlabMovieDispacther::RegisterName(const char* name,int type,int key_id)
{
	MovieList[name].ProcessType = type;
	MovieList[name].ProcessKeyID = key_id;
};

void fxlabMovieDispacther::UpdateProcess(const char* name,Vect3f& position,Vect3f& velocity,int status)
{
	hash_map<const char*,fxlabMovieDataType,hash<const char*>,fxlabMovieEqStr>::iterator i_movie;
	fxlabGeneralObjectType* t;
	fxlabProcessInterface* p;

	i_movie = MovieList.find(name);

	if(i_movie != MovieList.end()){
		p = &(i_movie->second.ProcessPoint);
		if(p->Process){
			p->Process->SetPosition(position);
			p->Process->SetVelocity(velocity);

			if(!status)
				p->Process->SetAlive(0);
		}else{
			if(status){
				t = fxlabClientD->CreateObject(i_movie->second.ProcessType);
				t->SetKeyID(i_movie->second.ProcessKeyID);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetPosition(position);
				t->SetVelocity(velocity);
				t->SetProcessInterface(&(i_movie->second.ProcessPoint));
				t->Start();
			};
		};
	};
};


int fxlabWorldIrradiate(void)
{
	fxlabGeneralObjectType* t;
	mchRacer* p;
	mchMechosRacer* mp;

	mp = dynamic_cast<mchMechosRacer*>(mch_raceD -> activeRacer);
	if(mp && mch_raceD -> activeRacer -> stPtr -> place == 0){
		p = mch_raceD->racerLst->first();
		while(p){
			if(p->Type == MCH_RACER_TYPE_BOSS){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WORLD_IRRADIATE);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetPosition(Vect3f(mp->R()));
				t->SetKeyID(FXLAB_ID_KEY_WORLD_IRRADIATE);
				t->SetMechosPoint(mp);
				t->Start();

				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_IMPULSE);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetSoundID(EFF_WORLD_EXPLOSION);
				t->Start();
				return 1;
			};
			p = p->next;
		};
	};
	return 0;
};

void fxlabPartWave(int x,int y)
{
	fxlabGeneralObjectType* t;

	if(!fxlabPartWaveTimer()){
		t = fxlabClientD->CreateObject(/*FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL*/FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_CHARACTER_WAVE_MODEL);
		t->SetPosition(Vect3f(x,y,vMap->LevelH2O + 1.0f));
		t->SetColorVector(Vect3f(fxlabGetTrackWaveRed(),fxlabGetTrackWaveGreen(),fxlabGetTrackWaveBlue()));
		t->SetTransparency(fxlabGetTrackWaveAlpha());
		t->Start();
//		fxlabPartWaveTimer.start(300);
	};
};

void fxlabPartWaveQuant(void)
{
	if(!fxlabPartWaveTimer())
		fxlabPartWaveTimer.start(400);
};

void fxlabBigBossStatus(int status)
{
	if(status)
		fxlabBigBossGeneratorEnable = 1;
	else
		fxlabBigBossGeneratorEnable = 0;
};

void fxlabBigBossWorld(int world,int status)
{
	switch(world){
//Moutain
		case 0:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_MOUNTAIN;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_MOUNTAIN;
			break;
//Water
		case 1:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_WATER;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_WATER;
			break;
//Forest
		case 2:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_FOREST;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_FOREST;
			break;
//Town
		case 3:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_TOWN;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_TOWN;
			break;
//Lava	
		case 4:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_FIRE;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_FIRE;
			break;
//Watch
		case 8:
			if(status)
				fxlabBigBossWorldEnable |= FXLAB_BIG_BOSS_WORLD_ID_WATCH;
			else
				fxlabBigBossWorldEnable &= ~FXLAB_BIG_BOSS_WORLD_ID_WATCH;
			break;
	};
};


/*
<Yli><Kron> Слизь: F3-убрать, F2-вернуть на место, F1-на дорогу переместить
<Yli><Kron> F4-убрать слизь
*/
