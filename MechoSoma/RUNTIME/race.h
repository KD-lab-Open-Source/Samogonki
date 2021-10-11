#ifndef __RACE_H__
#define __RACE_H__

#include "fxlabInterface.h"
#include "ctl_point.h"
#include "track.h"
#include "Mechos.h"

#ifndef _FINAL_VERSION_
//#define _RACE_LOG_
#endif /* _FINAL_VERSION_ */

#define _RACE_LOG_

#ifdef _RACE_LOG_
extern XStream mch_raceLog;
#define _RACE_LOG_STR(p)	if(mchRaceLogFlag && mchTimeMode != MCH_TIME_GAME_OVER) mch_raceLog < (p)
#define _RACE_LOG_TXT(p)	if(mchRaceLogFlag && mchTimeMode != MCH_TIME_GAME_OVER) mch_raceLog <= (p)
#else
#define _RACE_LOG_STR(p)
#define _RACE_LOG_TXT(p)
#endif

class cMesh;
struct mchControlPoint;
struct mchBonusArcaneData;

const int MCH_MAX_NUM_TRACKS	= 10;

enum mch_AI_ArcaneTypes
{
	AI_ARC_BEST = 0,	// 0
	AI_ARC_AREA_HIT,	// 1 
	AI_ARC_DEFENCE,		// 2
	AI_ARC_MOVEMENT,	// 3
	AI_ARC_PREV_RACER_HIT,	// 4
	AI_ARC_NEXT_RACER_HIT,	// 5

	AI_ARC_MAX_TYPE
};

struct mch_AI_Prm 
{
	int maxSeedCnt;
	int maxTrackElCnt;
	int maxTrackElCntDelta;

	int arcaneRnd;

	float maxTurnAngle;
	float maxTurnAngleDelta;

	int tractionFlag;
	float tractionPrm;

	float tractionPrmMin;
	float tractionPrmMax;

	int tractionDistMin;
	int tractionDistMax;

	void init(char* ini_file,char* section);
};

// mchRacerStats::flags
#define MCH_RACER_MECHOS_CFG		0x01
#define MCH_RACER_WAS_ON_WORLD		0x02

struct mchRacerStats 
{
	int ID;
	int poolID;
	int essenceID;

	int flags;

	int time;
	int cur_time;
	int start_time;

	int place;
	int prev_place;
	float placeTimer;

	int last_place;
	int last_config[5];

	int Lap;
	int Checkpoint;
	int Turn;

	int NStar;
	int NSeeds;
	int NArcanes;
	int NDestroys;

	unsigned char partFlags[ARCANE_MAX];

	char name[10];
	char mechos_config[30];
	char backup_config[30];

	void* list;
	mchRacerStats* prev;
	mchRacerStats* next;

	void init(void);
	void convert(XBuffer& XBuf);

	void save(XStream* xb);
	void save(XBuffer* xb);
	void load(XBuffer* xb);

	void UpdateMechosCfg(void);
	void SetMechos(char* cfg);
	void GetMechos(int mode = 0);
	void PutMechos(void);
	void ReplacePart(int pt);

	int isAuthorized(int pt){ return partFlags[pt]; }
	void AuthorizePart(int pt);
	void AuthorizePart(cMesh* p);

	mchRacerStats(void);
	~mchRacerStats(void){ }
};

struct mchStartPoint
{
	int flags; 

	int PosX;
	int PosY;

	int aX;
	int aY;
	int aZ;

	void* list;
	mchStartPoint* next;
	mchStartPoint* prev;

	mchStartPoint(void){ list = 0; flags = 0; }
	~mchStartPoint(void){ }
};

#define MCH_CP_WRONG		0
#define MCH_CP_CURRENT		1
#define MCH_CP_PASSED		2

struct mchUniqueCheckpoint
{
	int RacerIndex;
	struct mchCheckpoint* Owner;
	fxlabProcessInterface* objPtr;

	mchUniqueCheckpoint(mchCheckpoint* owner,int racer_index);
	~mchUniqueCheckpoint(void);
	int curState;

	void create_objects(void);
	void create_start_objects(void);
	void set_state(int v,mchRacer* p,int init_flag = 0);
};

struct mchCheckpoint 
{
	int ID;
//	int curState;

	int PosX0;
	int PosY0;

	int PosX1;
	int PosY1;

	int Height;

	int ChStarMAX;
	int NStar;

	int trackSize;
	xtList<mchTrackBranch>* trackPtr;

	struct mchUniqueCheckpoint* UniqueData[2]; 

//	int check(int x,int y,int sx,int sy);
	int check(int x0,int y0,int x1,int y1);
	void redraw(void);
	void set(int x0,int y0,int x1,int y1){ PosX0 = x0; PosY0 = y0; PosX1 = x1; PosY1 = y1; };

	void init(void);
	void create_objects(void);
	void create_start_objects(void);
	void set_state(int v,mchRacer* p,int init_flag = 0);

	mchTrackBranch* getTrack(void);

	void* list;
	mchCheckpoint* next;
	mchCheckpoint* prev;

	mchCheckpoint(void);
	~mchCheckpoint(void);
};

// mchRacer::Type...
enum mchRacerTypes 
{
	MCH_RACER_TYPE_BASE = 0,
	MCH_RACER_TYPE_MECHOS,
	MCH_RACER_TYPE_BOSS,

	MCH_RACER_TYPE_MAX
};

// mchRacer::flags...
#define MCH_FINISHED		0x01
#define MCH_AI			0x02
#define MCH_ACTIVE_RACER	0x04
#define MCH_ACTIVE_RACER2	0x08
#define MCH_ESSENCE_SLIME	0x10	
#define MCH_ENERGY_CHANGE	0x20
#define MCH_GET_SEED		0x40
#define MCH_SEED_PASSED		0x80
#define MCH_SEEDS_LOADED	0x100
#define MCH_SLEEPING		0x200
#define MCH_ALL_SEEDS		0x400

enum RacerState {
	NormalRacer, // normal mechos & formic 
	DestroyedRacer, // need 2 control points to assemble
	AssemblingRacer // assembling, doesn't need points
};

struct mchRacer : virtual public PointControlledObject
{
	int ID;
	int Type;
	char* name;

	unsigned char cpData[64];

	int NStar;

	int flags;

	int AI_Type;
	int AI_Mode;

	int pickedUpArcane;

	mchRacerStats* stPtr;

	mchCheckpoint* prevCp;
	mchTrackElement* trackP;
	mchTrackBranch* trackBr;

	mchTrackElement* last_trackP;

	mchTrackElement* last_trackP0;
	mchTrackElement* last_trackP1;

	xtList<mchBonusArcaneData> bonusLst;

	Vect3f prevPos;

	// Control Points
	int numSeeds;
	mchControlPoint* curSeed;
	mchControlPoint* mouseSeed;
	mchControlPoint* lastSeed;
	xtList<mchControlPoint> seedLst;

	void* list;
	mchRacer* next;
	mchRacer* prev;

	void SetPointControl(void);

	virtual void StartEffect(int eff){ };
	virtual void ChangePart(int tp,int id){ };

	void getPosition(int& x, int& y,int& z);
	void get_prevPosition(int& x, int& y,int& z);
	void getSize(int& sx,int& sy);
	int CheckMouse(void);

	void setPrevPos(void);

	void SetTime(int tm);
	void SetTractionPrm(int fl,float v);
	void GetTractionPrm(int& fl,float& v);

	virtual int nitroTimer(void){ return 0; }
	virtual int isFlying(void){ return 0; }

	virtual float EnergyValue(void);
	virtual float EnergyValueMax(void){ return 1.0f; }
	virtual float EnergyTimer(void){ return 0.0f; }

	void AddBonus(int id);  // virtual 
	void AddPart(int id){ pickedUpArcane = id; }
	void RemoveBonus(int id);
	int CheckBonus(int id);
	int CheckBonusSlot(int id); // virtual

	virtual RacerState State(void){ return NormalRacer; }
	virtual void EnergyQuant(int dt){ }

	virtual int SpeedCount(void){ return 0; }

	void ClearCP(void){ memset(cpData,0,64); }
	void ClearCP(int id){ cpData[id >> 3] &= ~(1 << (id % 8)); }
	void SetCP(int id){ cpData[id >> 3] |= (1 << (id % 8)); }
	int CheckCP(int id){ return (cpData[id >> 3] & (1 << (id % 8))); }

	void initCheckpoints(int sz);

	int AddSeed(const Vect3f& point, int tp = 0); // virtual
	int AddSeed(mchControlPoint* p);

	void DumpSeeds(void);

	int GetArcaneCount(mchControlPoint* p,int id = -1);
	int GetArcaneCount(int id);

	int isAuthorized(int pt){ return stPtr -> isAuthorized(pt); }

	int GetDist(mchRacer* p);

	virtual void Quant(void);

	virtual int RemoveLastSeed(void);
	virtual void removePassedSeed();
	virtual int RemoveSelectedSeed(void);
	virtual void RemoveSeeds(void);

	void MoveSeed(int x,int y,int z);
	void NextSeed(void);

	void ShowSeeds(void);
	void HideSeeds(void);

	void LightSeed(int& x,int& y,int& z);

	int stopFnc(void);

	mchRacer(void);
	~mchRacer(void);
};

#pragma warning( disable : 4250 )  

class mchMechosRacer : public mchRacer, public Mechos
{
	int lastStatus;

	float lastEnergy;
	int energyTimer;
public:
	void Quant(void);

	void StartEffect(int eff);

	RacerState State(void);
	void EnergyQuant(int dt);

	int SpeedCount(void){ return speed_bonus_counter(); } // virtual

	float EnergyValue(void);
	float EnergyValueMax(void);
	float EnergyTimer(void);

	int isFlying(void);

	void AddBonus(int id);  // virtual 
	void ChangePart(int tp,int id); // virtual

	void sensorIn(const class SensorFieldSource* sensor); // virtual
	void sensorOut(const SensorFieldSource* sensor); // virtual

	void AuthorizeMechos(void);

	virtual int RemoveLastSeed(void);
	virtual void removePassedSeed();
	virtual int RemoveSelectedSeed(void);
	virtual void RemoveSeeds(void);

	mchMechosRacer(char* cfg_str = 0);
	~mchMechosRacer(void);
};

#pragma warning( default : 4250 )  

class mchBossRacer : public mchRacer
{
public:
	virtual void removePassedSeed();

	mchBossRacer(void){ Type = MCH_RACER_TYPE_BOSS; type_ = BOSS; };
	~mchBossRacer(void){};
};

// mchRaceDispatcher::flags...
#define MCH_RACE_STATS		0x01

struct mchRaceDispatcher
{
	int flags;

	mchControlPoint** cpBackup;

	xtList<mchCheckpoint>* cpLst;
	xtList<mchRacer>* racerLst;
	xtList<mchStartPoint>* startLst;

	mchRacer* activeRacer;
	mchRacer* activeRacer2;

	int mechosCfgBackup[5][6];

	void BackupParts(void);
	void RestoreParts(void);

	void SetTime(int tm);

	void showStats(void);
	void switchStats(void);

	void quant(void);
	void stQuant(int mode = 1);

	int CheckRacerPlace(mchRacer* p,mchRacer* p1);
	int CheckRacerPlaceHS(mchRacer* p,mchRacer* p1);

	void ArcaneQuant(mchRacer* p,float dt);
	void ArcaneInit(mchRacer* p);

	void CtlQuant(void);
	void CP_Quant(void);
	void TrackQuant(void);

	void ClearTrackFlags(int mode = 0);

	void CP_Quant(mchRacer* p);

	void redraw(void);

	void initCheckpoints(mchRacer* r);
	void setCheckpointNStar(mchRacer* r = NULL);
	int getCheckpointNStar(mchRacer* r = NULL);

	void GetChpCenterPos(int id,int& x,int& y,int& z);
	void GetChpCenterPos(mchCheckpoint* p,int& x,int& y,int& z);

	mchStartPoint* getStartPoint(int index);
	mchCheckpoint* getCP(int id);

	void setActiveRacer(mchRacer* p,int cnt_flag = 0,int player = 0);
	void nextRacer(int mode = 1);
	void nextRacerHS(void);
	void firstRacer(int id = -1);
	mchRacerStats* get_firstRacer(int id = -1);

	int getActiveRacerID(void);

	int isActiveRacerLast(void);

	int checkEndGame(void);
	int online_checkEndGame(void);

	void TrackInit(void);
	void DumpTrack(void);

	void prepareStartTime(void);
	int checkEndTurn(void);

	void AI_Init(mchRacer* p);
	void AI_Quant(mchRacer* p);
	void AI_PrmQuant(void);
	void AI_SeedQuant(mchRacer* p);
	void AI_SetSeed(mchRacer* p);
	void AI_SetPrm(mchRacer* p);
	int AI_CheckRacerCP(mchRacer* p);

	mch_AI_Prm* AI_GetPrm(mchRacer* p);

	void AI_Toggle(mchRacer* p = NULL,int v = -1);

	void AI_ArcaneQuant(mchMechosRacer* p);
	mchTrackElement* AI_GetTrackEl(mchTrackElement* fp,int fl,int count,int dir = 1,int rnd = 1);
	int AI_CheckArea(mchRacer* r,int dist);

	void move2checkpoint(mchRacer* r,mchCheckpoint* cp,int mode = 0);
	void move2trackpoint(mchRacer* r,mchTrackElement* tp);
	void put2checkpoint(mchRacer* r,int cp);

	int nextCp(int cp){ if(cp >= cpLst -> size() - 1) return 0; return cp + 1; }
	int prevCp(int cp){ if(cp <= 0) return cpLst -> size() - 1; return cp - 1; }

	void AddSeeds(int mode = 1);
	void InitSeeds(void);

	void CenterCamera(void);
	void CenterRacer(mchRacer* p = NULL);
	void AssembleRacer(mchRacer* p = NULL);

	void SetBrake(int on);

	void KeyTrap(void);

	void SaveAllSeeds(void);
	void LoadAllSeeds(void);
	void SaveSeeds(mchRacer* p = NULL);
	void LoadSeeds(int sz,void* p);

	void InitReplay(int all_flag);

	void EndTurn(void);
	void ResetSeeds(mchRacer* r = NULL);

	mchRaceDispatcher(void);
	~mchRaceDispatcher(void);
};

extern int mchRaceLogFlag;
extern int mch_raceFPS;
extern mchRaceDispatcher* mch_raceD;

void mchInitRaceDispatcher(void);
void mchFinitRaceDispatcher(int mode = 0);
void mchFinitRacers(void);

mchRacerStats* mchGetRacer(int id);

#endif  //__RACE_H__
