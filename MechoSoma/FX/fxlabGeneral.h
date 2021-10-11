#ifndef __FXLAB_CONTEXT_GENERAL__
#define __FXLAB_CONTEXT_GENERAL__



struct fxlabGeneralObjectType
{
	int ID;
	int Type;
	int Alive;

	float CurrentTime; // Relativity Time
	float StartTime,FinishTime; // Absolute Time

	fxlabGeneralObjectType(void)
	{ 
		ID = 0;
		Type = -1; 
		Alive = 1;
		CurrentTime = StartTime = 0;
		FinishTime = -1.0f;
	};

	virtual ~fxlabGeneralObjectType(){};

	virtual void Open(void){};
	virtual void Close(void){};
	virtual void Quant(void){};
	virtual void Start(void){};
	virtual void StopQuant(void){};

	virtual void Show(void){};

	virtual int GetID(void){ return ID; };
	virtual void SetID(int id){ ID = id; };

	virtual int GetType(void){ return Type; };
	virtual void SetType(int type){ Type = type; };

	virtual int GetAlive(void){ return Alive; }; 
	virtual void SetAlive(int alive){ Alive = alive; }; 

	virtual void SetProcessInterface(struct fxlabProcessInterface* p){};

	virtual void SetKeyID(int id){};

	virtual void SetCurrentTime(float time){ CurrentTime = time; };
	virtual void SetStartTime(float time){ StartTime = time; };
	virtual void SetFinishTime(float time){ FinishTime = time; };

	virtual void SetPosition(class Vect3f& v){};
	virtual void GetPosition(class Vect3f& v){};

	virtual void SetConversion(int conv){};
	virtual void SetSplineOwner(void* point){};

	virtual void SetVelocity(Vect3f& v){};

	virtual void SetScale(float scale){};
	virtual void SetMatrix(class MatXf& m){};

	virtual void SetAngle(float angle){};
	virtual void SetMode(int mode){};

	virtual void SetCheckPointModels(int id0,int id1){};
	virtual void SetCheckPointRacer(int mode,struct mchRacer* p,int init_flag = 0){};
	virtual void SetCheckPointCamera(int camera){};

	virtual void SetScaleVector(Vect3f& v){};
	virtual void SetAngleVector(Vect3f& v){};
	virtual void SetColorVector(Vect3f& v){};
	virtual void SetTransparency(float a){};
	virtual void SetModel(int id){};
	virtual void SetRacerPoint(struct mchRacer* p){};
	virtual void SetMechosPoint(class mchMechosRacer* p){};

	virtual void SetVisible(int v){};

	virtual void SetBody(class Body* p){};

	virtual void SetProtection(int id,int time){};
	virtual void SetStatistic(int owner_id,int arcane_id,fxlabGeneralObjectType* p,int object_id){};

	virtual void SetStartObject(int type,int id){};
	virtual void SetAliveObject(int type,int id){};
	virtual void SetDestroyObject(int type,int id){};

	virtual void ArcaneAction(void){};
	virtual void SetArcaneLink(struct mchArcaneData* p){};
	virtual void SetArcaneOwner(class mchMechosRacer* p){};
	virtual void SetArcaneInterruptNum(int num){};
	virtual void SetArcaneActionPoint(int num){};
	virtual void SetArcaneActionDelay(int time){};
	virtual void SetArcaneAlive(int arcane_id,int owner_id){};
	virtual void SetArcaneTime(int time){};
	virtual void SetArcaneLifeTime(float time){};

	virtual int GetStatisticOwnerID(void){ return 0; };
	virtual class ArcaneStatisticsInfo* GetStatistic(void){ return NULL; };

	virtual void SetGridPoint(void* pTileMap,void* pPolyGrid){};
//	virtual void SetGridPoint(void* p){};
	virtual void SetGridRed(int red,int delta_red){};
	virtual void SetGridGreen(int green,int delta_green){};
	virtual void SetGridBlue(int blue,int delta_blue){};
	virtual void SetGridAlpha(int alpha,int delta_alpha){};
	virtual void SetGridHeight(int z){};
	virtual void SetGridUV(float uv){};

	virtual void SetBonusModelType(int type){};
	virtual void SetBonusModelID(int id){};
	virtual void SetBonusActionID(int id){};
	virtual void SetBonusPartID(int id){};
	virtual void SetBonusRespawnTime(int time){};

	virtual void SetFireEnableTime(int time){};
	virtual void SetFireDisableTime(int time){};
	virtual void SetFireStartTime(int time){};

	virtual void SetRemoteInterface(fxlabProcessInterface* link){};
	virtual void SetRemoteObject(fxlabGeneralObjectType* p){};
	virtual void SetRemoteID(int id){};

	virtual void SetRadius(float r){};

	virtual void SetPositionDomain(struct fxlabSwarmDomain* p){};
	virtual void SetVelocityDomain(struct fxlabSwarmDomain* p){};

	virtual void SetChargeValue(float v){};
	virtual void SetChargeTime(float t){};

	virtual void SetSoundID(int id){};

	virtual void SetMenuTeleportStatus(int id){};
	virtual void SetMenuTeleportInfo(int world_id,int track_id){};
	virtual void SetMenuTeleportModels(int* id){};

	virtual void SetControllerPartID(int id){};
	virtual	void SetControllerArcaneID(int id){};

	virtual void SetTilePoint(void* pTileMap){};

	virtual void SetDeltaAngle(float delta){};
	virtual void SetTarget(class mchMechosRacer* p){};

	virtual void SetScaleAnimation(float start_scale,float end_scale,int time){};

	virtual void SetDirectionMatrix(class Mat3f& m){};

	virtual void SetBigBossWorldStatus(int world){};
};

//-------------------------------------

struct fxlabGeneralObjectDispatcher
{
	list<fxlabGeneralObjectType*> ObjectList;
	list<fxlabGeneralObjectType*> CreateList;

	virtual void Open(void);
	virtual void Close(void);
	virtual void Quant(void);
	virtual void Start(void){};

	virtual fxlabGeneralObjectType* CreateObject(int type);
	virtual void CreateQuant(void);
	fxlabGeneralObjectType* SearchObject(int type);
};

//--------------------------------------

struct fxlabGeneralActionType
{
	int ID;
	fxlabGeneralObjectType* Point;

	fxlabGeneralActionType(void){ Point = NULL; ID = -83; };
	fxlabGeneralActionType(fxlabGeneralObjectType* p,int id){ Point = p; ID = id; };
};

struct fxlabServerObjectDispatcher : fxlabGeneralObjectDispatcher
{
	float Time;
	list<fxlabGeneralActionType> ActionList;
	int LockList;
	int RootID;

	void Open(void);
	void Close(void);
	void Start(void);
	void Quant(void);
	void CreateQuant(void);
	fxlabGeneralObjectType* CreateObject(int type);
	float GetTime(void){ return Time; };

	void AddAction(fxlabGeneralObjectType* p,int id);
	void ActionQuant(void);
	void ActionOpen(void);
	void ActionClose(void);
	void StopArcane(int id,int owner_id);
};

//--------------------------------------

struct fxlabClientObjectDispatcher : fxlabGeneralObjectDispatcher
{
	float Time;
	float DeltaTime;
	MeasurementTimer DeltaTimer;

	void Start(void);
	void Quant(void);
	void Show(void);
	void Kill(void);
	fxlabGeneralObjectType* CreateObject(int type);

	float GetTime(void){ return Time; };
	float GetDeltaTime(void){ return DeltaTime; };
};

extern fxlabClientObjectDispatcher* fxlabClientD;
extern fxlabServerObjectDispatcher* fxlabServerD;

extern float fxlabServerQuantPeriod;
extern float fxlabGlobalTimeRate;
extern float fxlabServerTimeRate;
extern float fxlabParticleSystemScale;
extern float fxlabInvParticleSystemScale;

const unsigned FXLAB_RND_BASE = 1000;
const float FXLAB_RND_BASE_INV = (float)(1.0) / (float)(FXLAB_RND_BASE);

struct fxlabRndGenerator
{
	unsigned fxlabRNDVAL;

	void Open(void);

	unsigned RND(unsigned m)
	{
		fxlabRNDVAL ^= fxlabRNDVAL >> 3;
		fxlabRNDVAL ^= fxlabRNDVAL << 28;
		fxlabRNDVAL &= 0x7FFFFFFF;
		if(!m) return 0;
		return fxlabRNDVAL%m;
	};

	float UnitRND(void)
	{
		return ((float)(RND(FXLAB_RND_BASE)) * FXLAB_RND_BASE_INV);
	};
};

extern fxlabRndGenerator fxlabServerRND;
extern fxlabRndGenerator fxlabClientRND;

#endif
