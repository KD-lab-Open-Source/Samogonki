
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "race.h"
#include "demo_dispatcher.h"
#include "TrackDispatcher.h"
#include "TrackRecorder.h"
#include "Mesh3ds.h"
#include "Xreal.h"
#include "Params.h"
#include "TERRA.H"
#include "arcane.h"
#include "arcane_menu.h"
#include "sound.h"

#include "iText.h"

#include "KEYS.H"
#include "controls.h"

#include "CameraDispatcher.h"
#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "track.h"

#include "ACI_IDS.H"
#include "aci_parser.h"
#include "M3DSETUP.H"

#include "parts_pool.h"
#include "SCR_DEFS.H"

#include "ai_arcane.h"

#include "I-World.h"
#include "CustomFieldSources.h"

#include "mch_script.h"
#include "savegame.h"

#include "Statistics.h"

// TODO: @caiiiycuk network support
#ifdef NETWORK
#include "online_game.h"
#endif

#include "cdcheck.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern unsigned int XRndVal;
extern char acsJoyUse[];

extern int mchPBEM_Pause;
extern int mchPBEM_HiddenTime;
extern int mchPBEM_ViewEntireGame;
extern int mchPBEM_DataFlag;
extern int mchFreeResourcesFlag;

extern int mchNextWorld;
extern int mchNextTrack;

extern int xgrGameMode;

extern const char* mchA_StopTimeStr;
extern const unsigned char* mchNameStr;
extern int mchCurrentTrack;
extern int mchAutoCenterCamera;
extern int mchTimeStopArcaneCount;
extern int mchEnableStartCounter;
extern int mchEnableTrackDemo;
extern int acsCurrentWorld;
extern int mch_iScreenID;

extern int mchSelectedWorld;
extern int mchWorldSelectFlag;
extern int mchTrackSelectFlag;

extern int acsMaxEssenceID;

extern int iWorldMode;

extern xtList<mchBonusArcaneData> mchBonusLst;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

extern int acsLoadFlag;
extern char mch_playerNames[15][HS_NAME_LEN + 2];

extern int mchHS_SaveFlag;
extern int mchHS_LoadFlag;

extern const char* mchIscreenINI;

extern XBuffer mchA_XBuf;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchA_BuildOnlinePlaceOrder(void);

void mchEndGameFinit(int mode = 0);
void mchReInitGraph(int mode);

void* mchSaveGameParams(int& sz);
void mchLoadGameParams(void* p,int sz);
void mchRestoreGameParams(void);

void mchA_BuyTracks(void);
void mchSelectKeysConfig(int cfg_id);

void acsSaveGame(void);
void acsLoadGame(void);

void mchA_ShowStartCountEnable(void);

void mchParseScript(void);
void mchGetMouseCoords3D(int& x,int& y,int& z);
void mchEndGame(void);
void mchEndGame2(void);

void mchInitRacers(void);
void mchInitPartIDs(void);
void mchInitAI(void);
void mchInitWorldsPrm(void);
void mchA_DropCPDisable(mchRacer* p);

void mchSetNextQuantRTO(void);

int mchGetKey(int key);

// v1 - dest
// v2 - src
void mch_vect_axis_rotate(Vect3f& v1,Vect3f v2,Vect3f axis,float angle);

void mchA_ShowLapEnable(int player);
void mchA_ChangeOnlinePlayer(int dir);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define MCH_CP_DELTA_Z		0

//#define _RACE_DEBUG_
#ifdef _RACE_LOG_
XStream mch_raceLog;
#endif

int mchRaceLogFlag = 0;
int mchRaceLogID = -1;

#define MCH_CP_DELTA		20
#define MCH_CP_RADIUS		10.0f

#define MCH_CP_OBJ_NUM		5

#define MCH_CP_SZ0		2.0f
#define MCH_CP_SZ1		2.0f

int mchNumRacers = 0;		// number of bots (defined in mechosoma.ini)...
int mchNumRacersDef = 0;
int mchPlayerRecorder = 0;
mchRaceDispatcher* mch_raceD;

int mchNumLaps = 2;
int mchNumLapsDef = 2;
int mchNumLapsDefHS = 2;
int mchTurn = 0;
int mchBotArcaneUsing = 1;
int mchDefaultFigure = 0;

int mchSplitScreenBots = 0;

int mchPartID[NUM_ACTIVE_PARTS];

xtList<mchRacerStats> mch_racerLst;
int mchRacersInitFlag = 0;

const char* mch_botsINI = "RESOURCE\\bots.ini";

int mchAI_NumCfg = 0;
mch_AI_Prm* mchAI_Cfg = NULL;

int mchAI_RealTimeNumCfg = 0;
mch_AI_Prm* mchAI_RealTimeCfg = NULL;
int mchAI_TurnBasedNumCfg = 0;
mch_AI_Prm* mchAI_TurnBasedCfg = NULL;

xtList<mchAI_ArcanePrm> mchAI_Arcanes;
unsigned char mch_partFlags[ARCANE_MAX];
unsigned char mch_partFlags2[ARCANE_MAX];
unsigned char mch_excludeFlags[ARCANE_MAX];

int mch_raceFPS = 20;
int mchLapLength = 0;

int MCH_CP_STAR_NUM = 5;

XBuffer mch_raceSaveBuf(64000);

int mchSeedID = 0;

mchUniqueCheckpoint::mchUniqueCheckpoint(mchCheckpoint* owner,int racer_index)
{
	Owner = owner;
	curState = MCH_CP_WRONG;
	RacerIndex = racer_index;
}

mchUniqueCheckpoint::~mchUniqueCheckpoint(void)
{
	if(objPtr) delete objPtr;
}

extern int fxlabCheckPointIndex;
extern int fxlabCheckPointMax;
int fxlabGetWorldReflectionEnable(void);

void mchUniqueCheckpoint::create_objects(void)
{
	int i;
	int mz;
 	float sc;

	Vect3f v,dv,c,p;

	v.x = getDistX(Owner->PosX1,Owner->PosX0);
	v.y = getDistY(Owner->PosY1,Owner->PosY0);
	v.z = 0;

	dv = v / Owner->ChStarMAX;

	sc = v.psi() * 180.0f / M_PI;

	c.x = (Owner->PosX1 + Owner->PosX0) / 2;
	c.y = (Owner->PosY1 + Owner->PosY0) / 2;
	if(Owner->Height == -1){
		if(fxlabGetWorldReflectionEnable() && (GetW(XCYCL(Owner->PosX0),YCYCL(Owner->PosY0)) || GetW(XCYCL(Owner->PosX1),YCYCL(Owner->PosY1))))
			c.z = vMap->LevelH2O;
		else
			c.z = (ZV(XCYCL(Owner->PosX0),YCYCL(Owner->PosY0)) + ZV(XCYCL(Owner->PosX1),YCYCL(Owner->PosY1))) / 2.0;

		mz = ZV(XCYCL(Owner->PosX0 + round(v.x) / 2),YCYCL(Owner->PosY0 + round(v.y) / 2));
		if(c.z < mz) c.z = mz;
	}else c.z = Owner->Height;

#ifndef _WRLD_SCREENSHOTS_
	objPtr = new fxlabProcessInterface[Owner->ChStarMAX];
#else
	objPtr = NULL;
#endif

	fxlabCheckPointMax = Owner->ChStarMAX;
	fxlabCheckPointIndex = 0;

	p = Vect3f(Owner->PosX0,Owner->PosY0,c.z + 50.0f);
	for(i = 0; i < Owner->ChStarMAX;i++){
		CYCLE(p);
#ifndef _WRLD_SCREENSHOTS_
		fxlabCreateAverageCheckPoint(&objPtr[i],p.x,p.y,p.z,FXLAB_CHECK_POINT_MODE_FUTURE,MCH_CP_SZ1,c.x,c.y,c.z,RacerIndex);
#endif
		fxlabCheckPointIndex++;
		p += dv;
	}
}

void mchUniqueCheckpoint::create_start_objects(void)
{
	int i;
	int mz;
 	float sc;

	Vect3f v,dv,c,p;

	v.x = getDistX(Owner->PosX1,Owner->PosX0);
	v.y = getDistY(Owner->PosY1,Owner->PosY0);
	v.z = 0;
	
	dv = v / Owner->ChStarMAX;

	sc = v.psi() * 180.0f / M_PI;

	c.x = (Owner->PosX1 + Owner->PosX0) / 2;
	c.y = (Owner->PosY1 + Owner->PosY0) / 2;
	if(Owner->Height == -1){
		if(fxlabGetWorldReflectionEnable() && (GetW(XCYCL(Owner->PosX0),YCYCL(Owner->PosY0)) || GetW(XCYCL(Owner->PosX1),YCYCL(Owner->PosY1))))
			c.z = vMap->LevelH2O;
		else
			c.z = (ZV(XCYCL(Owner->PosX0),YCYCL(Owner->PosY0)) + ZV(XCYCL(Owner->PosX1),YCYCL(Owner->PosY1))) / 2.0;

		mz = ZV(XCYCL(Owner->PosX0 + round(v.x) / 2),YCYCL(Owner->PosY0 + round(v.y) / 2));
		if(c.z < mz) c.z = mz;
	}else c.z = Owner->Height;

#ifndef _WRLD_SCREENSHOTS_
	objPtr = new fxlabProcessInterface[Owner->ChStarMAX];
#else
	objPtr = NULL;
#endif

	fxlabCheckPointMax = Owner->ChStarMAX;
	fxlabCheckPointIndex = 0;

	p = Vect3f(Owner->PosX0 + dv.x * 0.5f,Owner->PosY0 + dv.y * 0.5f,c.z + 50.0f + dv.z * 0.5f);
	for(i = 0; i < Owner->ChStarMAX; i++){
		CYCLE(p);
#ifndef _WRLD_SCREENSHOTS_
		fxlabCreateMasterCheckPoint(&objPtr[i],p.x,p.y,p.z,FXLAB_CHECK_POINT_MODE_ACTIVE_START,sc,c.x,c.y,c.z,i,RacerIndex);
#endif
		fxlabCheckPointIndex++;
		p += dv;
	}
};

void mchUniqueCheckpoint::set_state(int v,mchRacer* p,int init_flag)
{
	int i;
	int hl,hr;
	
	curState = v;

	if(p && (p -> flags & MCH_FINISHED)){
		if(objPtr){
			if(Owner->ID){
				for(i = 0; i < Owner->ChStarMAX;i++){
					fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_SKIP,NULL,init_flag);
				};
				return;
			};
		};
	};

	switch(v){
		case MCH_CP_WRONG:
			if(objPtr){
				if(Owner->ID){
					for(i = 0; i < Owner->ChStarMAX; i ++){
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_FUTURE,NULL,init_flag);
					}
				}else{
					for(i = 0; i < Owner->ChStarMAX; i ++){
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_PASSIVE_START,NULL,init_flag);
					}						
				};
			};
			break;
		case MCH_CP_CURRENT:
			if(objPtr){					
				if(Owner->ID){
					hr = (Owner->ChStarMAX - Owner->NStar) >> 1;
					hl = (Owner->ChStarMAX - Owner->NStar) - hr;
					hr = Owner->ChStarMAX - hr;

					for(i = 0;i < hl;i ++)
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_FADE,NULL,init_flag);
					for(i = hl;i < hr;i++)
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_CURRENT,NULL,init_flag);
					for(i = hr;i < Owner->ChStarMAX;i++)
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_FADE,NULL,init_flag);
				}else{
					if(mch_raceD->racerLst->size() <= Owner->ChStarMAX && mch_raceD->activeRacer){
						hr = mch_raceD->activeRacer->stPtr->place >> 1;
						hl = mch_raceD->activeRacer->stPtr->place - hr;
						hr = Owner->ChStarMAX - hr;

						for(i = 0;i < hl;i ++)
							fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_FADE_START,NULL,init_flag);
						for(i = hl;i < hr;i++)
							fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_ACTIVE_START,NULL,init_flag);
						for(i = hr;i < Owner->ChStarMAX;i++)
							fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_FADE_START,NULL,init_flag);
					};
				};
			};
			break;
		case MCH_CP_PASSED:
			if(objPtr){
				if(Owner->ID){
					fxlabCheckPointMax = Owner->NStar;
					fxlabCheckPointIndex = 0;
					for(i = 0; i < Owner->ChStarMAX; i ++){
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_MOVE,p,init_flag);
					}
				}else{
					for(i = 0; i < Owner->ChStarMAX; i ++)
						fxlabChangeCheckPoint(&objPtr[i],FXLAB_CHECK_POINT_MODE_PAST,p,init_flag);
				};
			};
			break;
	}
//	curState = v;
}

extern int mchSplitScreenGame;

mchCheckpoint::mchCheckpoint(void)
{
	list = 0;
//	curState = MCH_CP_WRONG;
//	objPtr = NULL;

	UniqueData[0] = NULL;
	UniqueData[1] = NULL;
	UniqueData[0] = new mchUniqueCheckpoint(this,0);
	if(mchSplitScreenGame)
		UniqueData[1] = new mchUniqueCheckpoint(this,1);

	trackPtr = new xtList<mchTrackBranch>;
	NStar = ChStarMAX = MCH_CP_STAR_NUM;
	Height = -1;
}

mchCheckpoint::~mchCheckpoint(void)
{
	mchTrackBranch* p = trackPtr -> first();
	while(p){
		trackPtr -> remove(p);
		delete p;
		p = trackPtr -> first();
	}
	delete trackPtr;

	if(UniqueData[0]) delete UniqueData[0];
	if(UniqueData[1]) delete UniqueData[1];
}

extern int fxlabCheckPointIndex;
extern int fxlabCheckPointMax;

int fxlabGetWorldReflectionEnable(void);

void mchCheckpoint::create_objects(void)
{
	if(UniqueData[0]) UniqueData[0]->create_objects();
	if(UniqueData[1]) UniqueData[1]->create_objects();
}

void mchCheckpoint::create_start_objects(void)
{
	if(UniqueData[0]) UniqueData[0]->create_start_objects();
	if(UniqueData[1]) UniqueData[1]->create_start_objects();
};

void mchCheckpoint::init(void)
{
	if(ID)
		create_objects();
	else
		create_start_objects();
}

void mchCheckpoint::set_state(int v,mchRacer* p,int init_flag)
{
	if(mchSplitScreenGame){
		if(p){
			if(p->flags & MCH_ACTIVE_RACER){
				if(p->flags & MCH_ACTIVE_RACER2){
					if(UniqueData[1]){
						UniqueData[1]->set_state(v,p,init_flag);
					};
				}else{
					if(UniqueData[0]){
						UniqueData[0]->set_state(v,p,init_flag);
					};
				};
			};
		}else{
			if(UniqueData[0]) UniqueData[0]->set_state(v,p,init_flag);
		};
	}else{
		if(UniqueData[0]) UniqueData[0]->set_state(v,p,init_flag);
	};
};

mchRacer::mchRacer(void)
{
	ID = -1;
	flags = 0;
	name = NULL;

	AI_Type = 0;

	trackP = NULL;
	trackBr = NULL;

	pickedUpArcane = -1;

	numSeeds = hsTurnSeedsMax;
	NStar = 0;

	last_trackP = last_trackP0 = last_trackP1 = NULL;

	curSeed = mouseSeed = NULL;
	lastSeed = new mchControlPoint;

	list = 0;
}

mchRacer::~mchRacer(void)
{
	mchBonusArcaneData* p = bonusLst.first();
	while(p){
		bonusLst.remove(p);
		delete p;
		p = bonusLst.first();
	}
}

void mchRacer::getPosition(int& x,int& y,int& z)
{
	x = R().x;
	y = R().y;
	z = R().z;
}

void mchRacer::get_prevPosition(int& x,int& y,int& z)
{
	x = prevPos.x;
	y = prevPos.y;
	z = prevPos.z;
}

void mchRacer::getSize(int& sx,int& sy)
{
	sx = sy = round(radius());
}

mchRaceDispatcher::mchRaceDispatcher(void)
{
	int i;

	mchTurn = 0;
	flags = MCH_RACE_STATS;

	activeRacer = activeRacer2 = NULL;

	cpBackup = new mchControlPoint*[10];
	for(i = 0; i < 10; i ++)
		cpBackup[i] = new mchControlPoint;

	racerLst = new xtList<mchRacer>;
	cpLst = new xtList<mchCheckpoint>;
	startLst = new xtList<mchStartPoint>;
}

mchRaceDispatcher::~mchRaceDispatcher(void)
{
	int i;
	for(i = 0; i < 10; i ++)
		delete cpBackup[i];

	delete[] cpBackup;

	mchRacer* rp = racerLst -> first();
	while(rp){
		mchRacer* rp_next = rp -> next;
		if(!--rp -> handleCount){
			racerLst -> remove(rp);
			delete rp;
		}
		rp = rp_next;
	}
	delete racerLst;
	
	mchCheckpoint* cp = cpLst -> first();
	while(cp){
		cpLst -> remove(cp);
		delete cp;
		cp = cpLst -> first();
	}
	delete cpLst;

	mchStartPoint* sp = startLst -> first();
	while(sp){
		startLst -> remove(sp);
		delete sp;
		sp = startLst -> first();
	}
	delete startLst;
}

int mchCheckpoint::check(int x0,int y0,int x1,int y1)
{
	int xc,yc;
	float A1,B1,C1,A2,B2,C2,D1,D2,D3,fl;

	int x00,y00,x11,y11;

	x00 = x0;
	y00 = y0;
	x11 = x1;
	y11 = y1;

	x0 = PosX0 + getDistX(x0,PosX0);
	y0 = PosY0 + getDistY(y0,PosY0);

	x1 = x0 + getDistX(x1,x0);
	y1 = y0 + getDistY(y1,y0);

	A1 = float(y1 - y0);
	B1 = float(x0 - x1);
	C1 = float((x1 - x0) * y0 - (y1 - y0) * x0);

	A2 = float(PosY1 - PosY0);
	B2 = float(PosX0 - PosX1);
	C2 = float((PosX1 - PosX0) * PosY0 - (PosY1 - PosY0) * PosX0);

	D1 = A1*B2 - A2*B1;

	if(!D1) return 0;

	D2 = B1*C2 - B2*C1;
	D3 = C1*A2 - C2*A1;

	xc = round(D2/D1);
	yc = round(D3/D1);

	fl = 0;
	if((x1 >= x0 && xc >= x0 && xc <= x1) || (x1 <= x0 && xc <= x0 && xc >= x1)) fl ++;
	if((PosX1 >= PosX0 && xc >= PosX0 && xc <= PosX1) || (PosX1 <= PosX0 && xc <= PosX0 && xc >= PosX1)) fl ++;

	if((y1 >= y0 && yc >= y0 && yc <= y1) || (y1 <= y0 && yc <= y0 && yc >= y1)) fl ++;
	if((PosY1 >= PosY0 && yc >= PosY0 && yc <= PosY1) || (PosY1 <= PosY0 && yc <= PosY0 && yc >= PosY1)) fl ++;

	if(fl == 4) return 1;

	return 0;
}

void mchCheckpoint::redraw(void)
{
}

void mchRaceDispatcher::quant(void)
{
	int cp_flag;
	mchRacer* p = racerLst -> first();

	cp_flag = 0;

	if(iWorldMode){
		ArcaneQuant(activeRacer,100000.0f);

		if(mchSplitScreenGame)
			ArcaneQuant(activeRacer2,100000.0f);

		CP_Quant();
		return;
	}

	if(mchTimeMode != MCH_TIME_WAITING_TURN && mchTimeMode != MCH_TIME_STOPPED){
		start_timer(raceD_Quant1, STAT_RACEDISP);

		stop_timer(raceD_Quant1, STAT_RACEDISP);
		start_timer(raceD_Quant2, STAT_RACEDISP);

		TrackQuant();
		CtlQuant();

		stop_timer(raceD_Quant2, STAT_RACEDISP);
		start_timer(raceD_Quant3, STAT_RACEDISP);

		if(!mchTrackDemoMode)
			CP_Quant();

		stop_timer(raceD_Quant3, STAT_RACEDISP);
		start_timer(raceD_Quant4, STAT_RACEDISP);
/*
		while(p){
			if((p -> flags & MCH_AI) && (mchGameMode != MCH_SPLIT_CONTROL_HS || p == activeRacer)){
				AI_Quant(p);
				if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchTimeMode == MCH_TIME_RUNNING && !(p -> flags & MCH_FINISHED) && p -> flags & MCH_AI && p -> numSeeds <= -hsTurnSeedsMax)
					mchGameFlags |= MCH_STOP_TIME_FLAG;
			}
			else {
				if(mchTurnBasedGame){
					if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
						if(mchTimeMode == MCH_TIME_RUNNING && !(p -> flags & MCH_FINISHED) && !p -> seedLst.size())
							mchGameFlags |= MCH_STOP_TIME_FLAG;
					}
					else {
						if(!(p -> flags & MCH_FINISHED) && p == activeRacer && !p -> seedLst.size())
							mchGameFlags |= MCH_STOP_TIME_FLAG;
					}
				}
			}
			p = p -> next;
		}
*/
		while(p){
			if((p -> flags & MCH_AI) && (mchGameMode != MCH_SPLIT_CONTROL_HS || p == activeRacer)){
				AI_Quant(p);
			}
			else {
				if(mchTurnBasedGame && mchGameMode != MCH_ENTIRE_CONTROL_HS){
					if(!(p -> flags & MCH_FINISHED) && p == activeRacer && !p -> seedLst.size())
						mchGameFlags |= MCH_STOP_TIME_FLAG;
				}
			}
			p = p -> next;
		}

		if(mchTimeMode == MCH_TIME_RUNNING && mchGameMode == MCH_ENTIRE_CONTROL_HS){
			if(checkEndTurn())
				mchGameFlags |= MCH_STOP_TIME_FLAG;
		}

		stop_timer(raceD_Quant4, STAT_RACEDISP);
		start_timer(raceD_Quant5, STAT_RACEDISP);

		AI_PrmQuant();

		if(!mchTrackDemoMode && mchTimeMode != MCH_TIME_GAME_OVER)
			stQuant();

		if(mchGameMode != MCH_SPLIT_CONTROL_HS){
			p = racerLst -> first();
			while(p){
				p -> Quant();
				p = p -> next;
			}
		}
		else {
			if(activeRacer -> flags & MCH_AI && activeRacer -> numSeeds <= 0)
				mchGameFlags |= MCH_STOP_TIME_FLAG;

			activeRacer -> Quant();
		}

		stop_timer(raceD_Quant5, STAT_RACEDISP);
	}
}

mchStartPoint* mchRaceDispatcher::getStartPoint(int index)
{
	mchStartPoint* st = startLst -> first();
	int i = 0;
	while(st){
		if(i++ == index)
			break;
		st = st -> next;
	}
	return st;
}

void mchInitRaceDispatcher(void)
{
	Vect3f r;
	int angle;
	int i,j,num,hs_flag = 0,idx,boss_flag = 0,place = 0;

	if(mchPBEM_Game)
		XRndVal = hsRndSeed;

#ifdef _RACE_LOG_
	static int log_flag = 0;

	if(mchGameMode == MCH_SINGLE_GAME) mchRaceLogFlag = 0;

	if(mchRaceLogFlag){
		mchA_XBuf.init();
		mchA_XBuf < "LOGS\\race";

		if(mchRaceLogID != -1)
			mchA_XBuf <= mchRaceLogID < "_";

		if(log_flag < 100)
			mchA_XBuf < "0";
		if(log_flag < 10)
			mchA_XBuf < "0";

		mchA_XBuf <= log_flag;

		mchA_XBuf < ".log";

		mch_raceLog.open(mchA_XBuf,XS_OUT);

		log_flag ++;
	}

	mchA_XBuf.init();
	mchA_XBuf < "XRndVal = " <= XRndVal < "\r\n";
	_RACE_LOG_STR("\r\n--------------------------\r\n");
	_RACE_LOG_STR(mchA_XBuf.address());
#endif

	int boss_id = 5;
	char* boss_name = strdup(getIniKey(mch_mainINI,"race","boss"));
	mchBossRacer* boss_ptr = NULL;

	mchSeedID = 0;

	if(!sgD)
		sgD = new sgDispatcher;
	else {
		if(!mchHS_LoadFlag)
			sgD -> clear();
	}

	if(mchPBEM_Game && mchHS_LoadFlag)
		sgGetOnlineData();

	num = mchNumRacers + 1;

	mch_trackD -> SetCurTrack(mchCurrentWorld,mchCurrentTrack);

	cdCheck("readme.rtf","autorun.ini");

	if(mchSplitScreenGame)
		mchA_BuyTracks();

	if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
		num = HS_PLAYERS_MAX;
		hs_flag = 1;
	}
	else {
		if(strlen(boss_name)){
			boss_flag = 1;
			num = 2;
		}
		else {
			if(mch_trackD -> TrackBoss()){
				boss_name = strdup(mch_trackD -> TrackBoss());
				boss_id = mch_trackD -> TrackBossID();
				boss_flag = 1;
				num = 2;
			}
		}
	}

	if(iWorldMode || trkEnableFlag) 
		num = 1;

	if(mchSplitScreenGame && (num < 2 || !mchSplitScreenBots)) num = 2;

	mchRacer* rp;
	mchMechosRacer* mrp;

	mchRacerStats* rs,*rs0;
	mchStartPoint* st;
	mchCheckpoint* cp;

	XBuffer XBuf;

	mch_raceD = new mchRaceDispatcher;

	mchInitAI();
	mchParseScript();

	if(mchTurnBasedGame){
		mchAI_NumCfg = mchAI_TurnBasedNumCfg;
		mchAI_Cfg = mchAI_TurnBasedCfg;
	}
	else {
		mchAI_NumCfg = mchAI_RealTimeNumCfg;
		mchAI_Cfg = mchAI_RealTimeCfg;
	}

	cp = mch_raceD -> cpLst -> first();
	while(cp){
		cp -> init();
		cp = cp -> next;
	}

	mch_raceD -> TrackInit();
	mchA_LoadWorldMap(mchCurrentWorld);

	if(num > HS_PLAYERS_MAX) num = HS_PLAYERS_MAX;

	trkInitRecorder();
	
	if(!mch_racerLst.size())
		mchInitRacers();

	if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
		rs = mch_racerLst.first();
		while(rs){
			rs -> SetMechos(hsPlayers[rs -> ID].configStr);
			rs = rs -> next;
		}
	}
	else {
		rs0 = mch_racerLst.search(0);
		if(!(rs0 -> flags & MCH_RACER_MECHOS_CFG))
			rs0 -> GetMechos(1);

		rs = mch_racerLst.first();
		while(rs){
			if(!(rs -> flags & MCH_RACER_MECHOS_CFG))
				rs -> GetMechos();

			if(iWorldMode && rs != rs0)
				rs -> PutMechos();

			rs = rs -> next;
		}
	}

	st = mch_raceD -> startLst -> first();
	while(st){
		st -> flags = 0;
		st = st -> next;
	}

	idx = 0;

	for(i = 0; i < num; i ++){
		if(!hs_flag || hsPlayers[i].type){
			iwSelect(i);

			rs = mch_racerLst.search(i);
			rs -> placeTimer = 0.0f;
			rs -> Turn = rs -> Checkpoint = rs -> Lap = 0;
			rs -> NSeeds = rs -> NArcanes = rs -> NDestroys = 0;

			if(!iWorldMode) 
				rs -> flags |= MCH_RACER_WAS_ON_WORLD;

			idx = XRnd(mch_raceD -> startLst -> size());
			st = mch_raceD -> startLst -> first();
			if(st)
				for(j = 0; j < idx; j ++) st = st -> next;

			idx = 0;
			if(st){
				while(st -> flags){
					st = st -> prev;
					if(++idx > 100) break;
				}
			}

			if(st && !st -> flags){
				st -> flags = 1;
				r = Vect3f(st -> PosX,st -> PosY,0);
				angle = -st -> aZ;
			}
			else {
				if(iWorldMode && i < 2){
					iworld_Cur -> LastPosition(r,angle);
				}
				else {
					r = Vect3f(512.0f,512.0f,0.0f);
					angle = 0;
				}
			}

			if(boss_flag && i){
				boss_ptr = dynamic_cast<mchBossRacer*>(Mdisp -> createBody(boss_name,r,angle));
				rp = boss_ptr;
				rs -> essenceID = boss_id;
				boss_flag = 0;
			}
			else {
				if(!mchDebugMode)
					mrp = new mchMechosRacer(rs -> mechos_config);
				else
					mrp = new mchMechosRacer(hsPlayers[i].configStr);

				if(mchSplitScreenGame && i < 2){
					mrp -> set_control_config(i + 1);
					memset(rs -> partFlags,1,ARCANE_MAX);
				}

				mrp -> stPtr = rs;
//				mrp -> AuthorizeMechos();

				rp = mrp;
				Mdisp -> attachBody(rp);
			}

			rp -> handleCount ++;

			rp -> setPosition(r, G2R(angle));
			rp -> stPtr = rs;
			rp -> ID = i + 1;

			if(((!hs_flag && i) && (!mchSplitScreenGame || i > 1)) || (hs_flag && hsPlayers[i].type > 1)){
				rp -> flags |= MCH_AI;
				mch_raceD -> AI_Init(rp);
				mch_raceD -> AI_SetPrm(rp);
			}

			if(!iWorldMode && !hs_flag && mchPlayerRecorder && !i){
				rp -> flags |= MCH_AI;
				mch_raceD -> AI_Init(rp);
				mch_raceD -> AI_SetPrm(rp);
			}
			if(mchSplitScreenGame && !iWorldMode && !hs_flag && mchPlayerRecorder && i == 1){
				rp -> flags |= MCH_AI;
				mch_raceD -> AI_Init(rp);
				mch_raceD -> AI_SetPrm(rp);
			}

			if(!hs_flag){
				if(i && (!mchSplitScreenGame || i >= 2))
					strcpy(rs -> name,mch_playerNames[rs -> essenceID]);
			}
			else
				strcpy(rs -> name,hsPlayers[i].name);

			rp -> name = rs -> name;

			rs -> place = place;
			place ++;

			rp -> initCheckpoints(mch_raceD -> cpLst -> size());

			if(mchGameMode == MCH_SPLIT_CONTROL_HS)
				rp -> set_brake_for_turn_based(1);

			mch_raceD -> ArcaneInit(rp);
			mch_raceD -> racerLst -> append(rp);

			if(!i){
				if(acsJoyUse[mchSplitScreenGame])
					rp -> set_joystick_control(1);
				mch_raceD -> setActiveRacer(rp);
			}

			if(i == 1 && mchSplitScreenGame){
				if(acsJoyUse[2])
					rp -> set_joystick_control(1);

				mch_raceD -> setActiveRacer(rp,0,1);
			}

			if(mchGameMode == MCH_ENTIRE_CONTROL_HS || mchGameMode == MCH_SPLIT_CONTROL_HS)
				rp -> SetPointControl();

			rp -> setPrevPos();
		}
	}

	if(!iWorldMode)
		mch_raceD -> TrackQuant();

	if(!hs_flag)
		mch_raceD -> BackupParts();

	if(mchGameMode == MCH_SPLIT_CONTROL_HS)
		mch_raceD -> setActiveRacer(mch_raceD -> racerLst -> first());

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
		mch_raceD -> firstRacer(mchPBEM_CurPlayer);

		switch(mchPBEM_GameMode){
			case MCH_PBEM_EDIT_TURN:
				mchPBEM_TargetTurn = sgD -> turn_max();
				break;
			case MCH_PBEM_VIEW_TURN:
				mchPBEM_TargetTurn = sgD -> turn_max();
				break;
			case MCH_PBEM_END_TURN:
				mchPBEM_TargetTurn = sgD -> turn_max() + 1;
				break;
		}

/*
		switch(mchPBEM_GameMode){
			case MCH_PBEM_EDIT_TURN:
				mchPBEM_TargetTurn = og_inP.turn_max();
				break;
			case MCH_PBEM_VIEW_TURN:
				mchPBEM_TargetTurn = og_inP.turn_max();
				break;
			case MCH_PBEM_END_TURN:
				mchPBEM_TargetTurn = og_inP.turn_max() + 1;
				break;
		}
*/
		if(mchHS_LoadFlag)
			mch_raceD -> LoadAllSeeds();

		if(mchTurn > mchPBEM_TargetTurn){
			mchHS_LoadFlag = 0;
			switch(mchPBEM_GameMode){
				case MCH_PBEM_EDIT_TURN:
					mchHS_SaveFlag = 1;
					mchPBEM_HiddenTime = -1;
					break;
				case MCH_PBEM_VIEW_TURN:
					break;
				case MCH_PBEM_END_TURN:
					mchHS_SaveFlag = 1;
					break;
			}
		}
	}

	mch_raceD -> SetTime(mch_clock());

#ifndef _WRLD_SCREENSHOTS_
	if(!iWorldMode && !trkEnableFlag) {
		//@caiiiycuk
		Mdisp -> createBonusMen(const_cast<char*>(bonus_men_cfg));
	}
#endif

	acsSaveGame();

	free(boss_name);

#ifdef _RACE_LOG_
	mchA_XBuf.init();
	mchA_XBuf < "XRndVal = " <= XRndVal < "\r\n";
	_RACE_LOG_STR(mchA_XBuf.address());

	rp = mch_raceD -> racerLst -> first();
	while(rp){
		mchA_XBuf.init();
		mchA_XBuf <= rp -> ID < "\t" <= rp -> R().x < "\t" <= rp -> R().y < "\t" <= rp -> R().z < "\r\n";
		_RACE_LOG_STR(mchA_XBuf.address());

		rp = rp -> next;
	}

	mch_raceD -> DumpTrack();
#endif

	XGR_MouseSetPos(20,20);
}

void mchFinitRaceDispatcher(int mode)
{
	mchRacer* r;

	if(mode){
		r = mch_raceD -> racerLst -> first();
		if(mch_raceD -> activeRacer -> flags & MCH_FINISHED){
			if(mch_raceD -> racerLst -> size() > 1 && mch_raceD -> activeRacer -> stPtr -> place == mch_raceD -> racerLst -> size() - 1){
				mch_raceD -> activeRacer -> NStar = 0;
				memcpy(mch_raceD -> activeRacer -> stPtr -> mechos_config,mch_raceD -> activeRacer -> stPtr -> backup_config,30);
				mch_raceD -> activeRacer -> pickedUpArcane = -1;
			}
			else {
				if(mch_raceD -> activeRacer -> pickedUpArcane != -1){
					mch_raceD -> activeRacer -> stPtr -> AuthorizePart(mch_raceD -> activeRacer -> pickedUpArcane);
					mch_raceD -> activeRacer -> stPtr -> ReplacePart(mch_raceD -> activeRacer -> pickedUpArcane);
				}
				mch_raceD -> activeRacer -> stPtr -> UpdateMechosCfg();
			}

			while(r){
				r -> stPtr -> last_place = r -> stPtr -> place;
				r -> stPtr -> NStar += r -> NStar;

				r -> stPtr -> init();
				r = r -> next;
			}
			mch_raceD -> RestoreParts();
		}
	}
	delete mch_raceD;
	mch_raceD = NULL;

	trkFinitRecorder();

#ifdef _RACE_LOG_
	if(mchRaceLogFlag)
		mch_raceLog.close();
#endif
}

void mchParseScript(void)
{
	int i;
	mchCheckpoint* cp;
	mchStartPoint* st;

	for(i = 0; i < Ch_points.max_number; i ++){
		cp = new mchCheckpoint;
		cp -> ID = i;

		cp -> PosX0 = Ch_points.El[i].x1;
		cp -> PosY0 = Ch_points.El[i].y1;

		cp -> PosX1 = Ch_points.El[i].x1 + Ch_points.El[i].x2;
		cp -> PosY1 = Ch_points.El[i].y1 + Ch_points.El[i].y2;

		cp -> Height = Ch_points.El[i].h;

		mch_raceD -> cpLst -> append(cp);
	}

	for(i = 0; i < Ch_points.max_start; i ++){
		st = new mchStartPoint;
		st -> PosX = Ch_points.St[i].x;
		st -> PosY = Ch_points.St[i].y;

		st -> aX = Ch_points.St[i].ax;
		st -> aY = Ch_points.St[i].ay;
		st -> aZ = Ch_points.St[i].az;

		mch_raceD -> startLst -> append(st);
	}
}

void mchRaceDispatcher::setActiveRacer(mchRacer* p,int cnt_flag,int player)
{
	int ip_flag = 1;
	mchRacer* p1;
	mchRacerStats* rp;

	if(player == 0){
		if(activeRacer){
			if(p != activeRacer){
				if(mchGameMode == MCH_SPLIT_CONTROL_HS)
					activeRacer -> set_brake_for_turn_based(1);
				activeRacer -> HideSeeds();
				activeRacer -> flags &= ~(MCH_ACTIVE_RACER | MCH_ACTIVE_RACER2);
				activeRacer -> set_activity(0);
			}
			else {
				if(cnt_flag != -1)
					camera_dispatcher -> setObserver(p,0);
				return;
			}
		}
		else
			ip_flag = 0;

		if(mchGameMode == MCH_SPLIT_CONTROL_HS)
			p -> set_brake_for_turn_based(0);

		p -> ShowSeeds();
		p -> flags |= MCH_ACTIVE_RACER;

		if(cnt_flag != -1)
			camera_dispatcher -> setObserver(p,0);

		initCheckpoints(p);

//		Mdisp -> setActiveBody(p);
		p -> set_activity(1);

		activeRacer = p;

		if(mch_arcStatD)
			mch_arcStatD -> Clear();

		fxlabArcaneStatisticInit();

		if(mchGameMode == MCH_SINGLE_GAME){
			if(p -> stPtr -> ID){
				rp = mch_racerLst.search(0);
				if(rp){
					rp -> ID = p -> stPtr -> ID;
					p -> stPtr -> ID = 0;
				}
			}
			if(p -> ID){
				p1 = racerLst -> search(0);
				if(p1){
					p1 -> ID = p -> ID;
					p -> ID = 0;
				}
			}
		}
	}
	else {
		if(activeRacer2){
			if(p != activeRacer2){
				activeRacer2 -> HideSeeds();
				activeRacer2 -> flags &= ~(MCH_ACTIVE_RACER | MCH_ACTIVE_RACER2);
				activeRacer2 -> set_activity(0);
			}
			else
				return;
		}
		else
			ip_flag = 0;

		p -> ShowSeeds();
		p -> flags |= MCH_ACTIVE_RACER | MCH_ACTIVE_RACER2;

		if(cnt_flag != -1){
			if(camera_dispatcher2)
				camera_dispatcher2 -> setObserver(p,0);
		}

		initCheckpoints(p);

//		Mdisp -> setActiveBody(p);
		p -> set_activity(1);

		activeRacer2 = p;

		if(mch_arcStatD)
			mch_arcStatD -> Clear();

		fxlabArcaneStatisticInit();

		if(mchGameMode == MCH_SINGLE_GAME){
			if(p -> stPtr -> ID){
				rp = mch_racerLst.search(1);
				if(rp){
					rp -> ID = p -> stPtr -> ID;
					p -> stPtr -> ID = 1;
				}
			}
			if(p -> ID){
				p1 = racerLst -> search(1);
				if(p1){
					p1 -> ID = p -> ID;
					p -> ID = 1;
				}
			}
		}
	}
}

#define SWITCH_OBJ 		i = p -> stPtr -> place;	\
	if(!p -> stPtr -> placeTimer && !p1 -> stPtr -> placeTimer){	\
		p -> stPtr -> place = p1 -> stPtr -> place;	\
		p1 -> stPtr -> place = i; \
		if(!(p -> flags & MCH_FINISHED)) \
			p -> stPtr -> placeTimer = 500; \
		if(!(p1 -> flags & MCH_FINISHED)) \
			p1 -> stPtr -> placeTimer = 500; \
		flag = 1;	\
	}


int mchRaceDispatcher::CheckRacerPlace(mchRacer* p,mchRacer* p1)
{
	int i,flag = 0;
	if((p -> flags & MCH_FINISHED) && (p1 -> flags & MCH_FINISHED) && p -> stPtr -> time < p1 -> stPtr -> time){
		SWITCH_OBJ;
	}
	else {
		if((p -> flags & MCH_FINISHED) && !(p1 -> flags & MCH_FINISHED)){
			p -> stPtr -> placeTimer = p1 -> stPtr -> placeTimer = 0;
			SWITCH_OBJ;
		}
		else {
			if(p -> stPtr -> Lap > p1 -> stPtr -> Lap){
				SWITCH_OBJ;
			}
			else {
				if(p -> stPtr -> Lap == p1 -> stPtr -> Lap){
					if(p -> stPtr -> Checkpoint > p1 -> stPtr -> Checkpoint){
						SWITCH_OBJ;
					}
					else {
						if(!(p -> flags & MCH_FINISHED) && !(p1 -> flags & MCH_FINISHED) && p -> stPtr -> Checkpoint == p1 -> stPtr -> Checkpoint){
							if(p -> last_trackP && p1 -> last_trackP && p -> last_trackP -> owner == p1 -> last_trackP -> owner && p -> last_trackP -> get_dist() < p1 -> last_trackP -> get_dist()){
								SWITCH_OBJ;
							}
						}

					}
				}
			}
		}
	}
	return flag;
}

int mchRaceDispatcher::CheckRacerPlaceHS(mchRacer* p,mchRacer* p1)
{
	int i,flag = 0;
	if((p -> flags & MCH_FINISHED) && (p1 -> flags & MCH_FINISHED)){
		if(p -> stPtr -> time < p1 -> stPtr -> time){
			SWITCH_OBJ;
		}
		else {
			if(p -> stPtr -> Turn < p1 -> stPtr -> Turn){
				SWITCH_OBJ;
			}
		}
	}
	else {
		if((p -> flags & MCH_FINISHED) && !(p1 -> flags & MCH_FINISHED)){
			p -> stPtr -> placeTimer = p1 -> stPtr -> placeTimer = 0;
			SWITCH_OBJ;
		}
		else {
			if(p -> stPtr -> Lap > p1 -> stPtr -> Lap){
				SWITCH_OBJ;
			}
			else {
				if(p -> stPtr -> Lap == p1 -> stPtr -> Lap){
					if(p -> stPtr -> Checkpoint > p1 -> stPtr -> Checkpoint){
						SWITCH_OBJ;
					}
					else {
						if(!(p -> flags & MCH_FINISHED) && !(p1 -> flags & MCH_FINISHED) && p -> stPtr -> Checkpoint == p1 -> stPtr -> Checkpoint){
							if(p -> last_trackP && p1 -> last_trackP && p -> last_trackP -> owner == p1 -> last_trackP -> owner && p -> last_trackP -> get_dist() < p1 -> last_trackP -> get_dist()){
								SWITCH_OBJ;
							}
						}

					}
				}
			}
		}
	}
	return flag;
}

void mchRaceDispatcher::stQuant(int mode)
{
	int i = 0,flag,cl,cnt = 0;
	mchRacer* p,*p1;

	cl = mch_clock();

	int fl;
	float v;

	_RACE_LOG_STR("stQuant() -> ");
	_RACE_LOG_TXT(cl);
	_RACE_LOG_STR("\r\n");

	mchRacerStats* rs = mch_racerLst.first();
	while(rs){
		_RACE_LOG_STR("Time -> ");
		_RACE_LOG_TXT(rs -> ID);
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(rs -> time);
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(rs -> start_time);
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(rs -> cur_time);
		_RACE_LOG_STR("\r\n");

		rs = rs -> next;
	}

	_RACE_LOG_STR("XRnd -> ");
	_RACE_LOG_TXT(XRndVal);
	_RACE_LOG_STR("\r\n");

	p = racerLst -> first();
	while(p){
		_RACE_LOG_STR(" ------------------- \r\n ");
		_RACE_LOG_TXT(p -> ID);
		_RACE_LOG_STR(" ");
		_RACE_LOG_STR(p -> name);
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("Pos -> ");
		_RACE_LOG_TXT(p -> stPtr -> place);
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(p -> R().xi());
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(p -> R().yi());
		_RACE_LOG_STR("\t");
		_RACE_LOG_TXT(p -> R().zi());
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("Cp -> ");
		_RACE_LOG_TXT(p -> stPtr -> Checkpoint);
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("NStar -> ");
		_RACE_LOG_TXT(p -> NStar);
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("NSeeds -> ");
		_RACE_LOG_TXT(p -> stPtr -> NSeeds);
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("NArcanes -> ");
		_RACE_LOG_TXT(p -> stPtr -> NArcanes);
		_RACE_LOG_STR("\r\n");

		_RACE_LOG_STR("NDestroys -> ");
		_RACE_LOG_TXT(p -> stPtr -> NDestroys);
		_RACE_LOG_STR("\r\n");

		p -> GetTractionPrm(fl,v);

#ifdef _RACE_LOG_
		mch_raceSaveBuf.init();
		mch_raceSaveBuf < "Traction -> " <= v < "\r\n";
		_RACE_LOG_STR(mch_raceSaveBuf);

		if(p -> last_trackP){
			_RACE_LOG_STR("TrackPos: ");
			mch_raceSaveBuf.init();
			mch_raceSaveBuf <= p -> last_trackP -> ID < " ";
			mch_raceSaveBuf <= p -> last_trackP -> index < " ";
			mch_raceSaveBuf <= p -> last_trackP -> Cx < " ";
			mch_raceSaveBuf <= p -> last_trackP -> Cy < " ";
			mch_raceSaveBuf <= p -> last_trackP -> Cz < "\r\n";
			_RACE_LOG_STR(mch_raceSaveBuf);
		}

		p -> DumpSeeds();
#endif
		p -> stPtr -> cur_time = cl - p -> stPtr -> start_time;
		if(!(p -> flags & MCH_FINISHED) && mchTimeMode != MCH_TIME_STOPPED && mchTimeMode != MCH_TIME_WAITING_TURN){
			if(mchGameMode != MCH_SPLIT_CONTROL_HS || p == activeRacer){
				p -> stPtr -> time += p -> stPtr -> cur_time;
				ArcaneQuant(p,p -> stPtr -> cur_time);
				p -> EnergyQuant(p -> stPtr -> cur_time);
			}
			p -> stPtr -> placeTimer -= p -> stPtr -> cur_time;
			if(p -> stPtr -> placeTimer < 0.0f)
				p -> stPtr -> placeTimer = 0.0f;
		}

		p -> stPtr -> start_time = cl;
		p -> stPtr -> cur_time = 0;
		p -> stPtr -> prev_place = p -> stPtr -> place;

		p = p -> next;

		if(!p)
			_RACE_LOG_STR(" ------------------- \r\n ");

		i ++;
	}
	flag = 1;

	if(mchGameMode == MCH_SINGLE_GAME){
		while(flag){
			flag = 0;
			cnt ++;

			if(cnt > 10000) break;
//			if(cnt > 10000) ErrH.Abort("!!!");

			p = racerLst -> first();
			while(p){
				p1 = racerLst -> first();
				while(p1){
					if(p -> stPtr -> place > p1 -> stPtr -> place)
						flag |= CheckRacerPlace(p,p1);
					p1 = p1 -> next;
				}
				p = p -> next;
			}
		}
	}
	if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
		while(flag){
			flag = 0;
			cnt ++;

			if(cnt > 10000) break;
//			if(cnt > 10000) ErrH.Abort("!!!");

			p = racerLst -> first();
			while(p){
				p1 = racerLst -> first();
				while(p1){
					if(p -> stPtr -> place > p1 -> stPtr -> place)
						flag |= CheckRacerPlace(p,p1);
					p1 = p1 -> next;
				}
				p = p -> next;
			}
		}
	}
	if(mchGameMode == MCH_SPLIT_CONTROL_HS){
		while(flag){
			flag = 0;
			cnt ++;

			if(cnt > 10000) break;
//			if(cnt > 10000) ErrH.Abort("!!!");

			p = racerLst -> first();
			while(p){
				p1 = racerLst -> first();
				while(p1){
					if(p -> stPtr -> place > p1 -> stPtr -> place)
						flag |= CheckRacerPlaceHS(p,p1);
					p1 = p1 -> next;
				}
				p = p -> next;
			}
		}
	}
	p = racerLst -> first();
	while(p){
		if(p -> stPtr -> prev_place > p -> stPtr -> place){
			p -> StartEffect(Mechos::Effect::OutstripTheNeighbourEffect);
		}
		if(p -> stPtr -> prev_place < p -> stPtr -> place){
			p -> StartEffect(Mechos::Effect::OutstrippedByNeighbourEffect);
		}
		p = p -> next;
	}
}

#define MCH_STATS_X	10
#define MCH_STATS_Y	10
#define MCH_STATS_SX	125
#define MCH_STATS_SY	90

void mchRaceDispatcher::switchStats(void)
{ 
	flags ^= MCH_RACE_STATS;
}

void mchRaceDispatcher::showStats(void)
{
	mchRacerStats* p = activeRacer -> stPtr;
	if(!(flags & MCH_RACE_STATS)) return;

	static XBuffer XBuf;

	XBuf.init();

	if(activeRacer -> name && mchNameStr)
		strcpy((char*)mchNameStr,activeRacer -> name);

	XBuf < "Lap: " <= p -> Lap + 1 < "/" <= mchNumLaps < "  ";
	XBuf < "Pos: " <= p -> place + 1 < "/" <= racerLst -> size();
/*
	XBuf < "Time: ";
	p -> convert(XBuf);
	if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
		XBuf < "  ";
		XBuf < "Turn: " <= mchTurn;
	}
*/
}

mchRacerStats::mchRacerStats(void)
{
	Lap = Checkpoint = Turn = 0;
	time = start_time = cur_time = 0;
	place = last_place = 0;
	placeTimer = 0.0f;

	NStar = 0;
	flags = 0;

	list = NULL;

	memcpy(partFlags,mch_partFlags2,ARCANE_MAX);

	essenceID = XRnd(acsMaxEssenceID);

	memset(mechos_config,0,30);
}

void mchRacerStats::AuthorizePart(int pt)
{
	partFlags[pt] = 1;
}

void mchRacerStats::AuthorizePart(cMesh* p)
{
	int pt = p -> arcaneData.ID;
	if(pt != -1)
		partFlags[pt] = 1;
}

void mchRacerStats::init(void)
{
	Lap = Checkpoint = 0;
	time = start_time = cur_time = 0;
//	place = 0;
}

void mchRacerStats::convert(XBuffer& XBuf)
{
	int tm,hr,mn,sc,dsc;
	tm = (time + cur_time) / 1000;
	hr = tm / 3600;
	mn = (tm % 3600) / 60;
	sc = tm % 60;
	dsc = (tm % 1000) / (1000 / 10);

	if(hr){
		if(hr < 10) XBuf < "0";
		XBuf <= hr < ":";
	}

	if(mn < 10) XBuf < "0";
	XBuf <= mn < ":";

	if(sc < 10) XBuf < "0";
	XBuf <= sc;
/*
	if(dsc < 10) XBuf < "0";
	XBuf <= dsc;
*/
}

void mchRacer::SetTime(int tm)
{
	stPtr -> start_time = tm;
	stPtr -> time += stPtr -> cur_time;
	stPtr -> cur_time = 0;
}

void mchRaceDispatcher::SetTime(int tm)
{
	mchRacer* p = racerLst -> first();
	while(p){
		p -> SetTime(tm);
		p = p -> next;
	}
/*
	_RACE_LOG_STR("mch_raceD -> SetTime(");
	_RACE_LOG_TXT(tm);
	_RACE_LOG_STR(");\r\n");
*/
}

void mchRaceDispatcher::GetChpCenterPos(int id,int& x,int& y,int& z)
{
	mchCheckpoint* p = getCP(id);
	GetChpCenterPos(p,x,y,z);
}

void mchRaceDispatcher::GetChpCenterPos(mchCheckpoint* p,int& x,int& y,int& z)
{
	if(p){
		x = XCYCL((p -> PosX0 + p -> PosX1)/2);
		y = YCYCL((p -> PosY0 + p -> PosY1)/2);
		z = ZV(x,y);
	}
	else {
		x = y = z = 0;
	}
}

void mchRacer::initCheckpoints(int sz)
{
	int i,idx;

	ClearCP();

	idx = stPtr -> Checkpoint;
	for(i = 0; i < sz/2; i ++){
		SetCP(idx);
		idx --;
		if(idx < 0){
			if(stPtr -> Lap){
				idx += sz;
			}
			else
				break;
		}
	}
}

void mchRaceDispatcher::initCheckpoints(mchRacer* r)
{
	if(!r) r = activeRacer;

	mchCheckpoint* p = cpLst -> first(),*p0;
	r -> initCheckpoints(cpLst -> size());
	p0 = getCP(r -> stPtr -> Checkpoint);
	if(mchTurnBasedGame && mchFreezeTime){
		while(p){
			p -> set_state(MCH_CP_WRONG,r,1);
			p = p -> next;
		}
	}
	else {
		while(p){
			if(p != p0 && r -> CheckCP(p -> ID)){
				p -> set_state(MCH_CP_PASSED,r,1);
			}
			else {
				if(p -> ID) 
					p -> set_state(MCH_CP_WRONG,r,1);
			}
			p = p -> next;
		}
	}
	if(p0){
		p0 -> set_state(MCH_CP_CURRENT,r,1);
		r -> SetCP(p0 -> ID);
	}

	if(r -> flags & MCH_ACTIVE_RACER) 
		setCheckpointNStar(r);
}

int mchRacer::CheckMouse(void)
{
	float x,y,r,dist;
	int mx,my,mz;

	mchGetMouseCoords3D(mx,my,mz);
	r = radius();
	x = mx - R().x;
	y = my - R().y;
	dist = x * x + y * y; 
	if(dist > r * r) return 1;

	return 0;
}

void mchRaceDispatcher::nextRacer(int mode)
{
	int next_turn = 0;
	mchRacer* p = activeRacer -> next;
	if(!p){
		next_turn = 1;
		p = racerLst -> first();
	}

	if(mchGameMode == MCH_SPLIT_CONTROL_HS){
		while(p -> flags & MCH_FINISHED){
			p = p -> next;
			if(!p){
				next_turn = 1;
				p = racerLst -> first();
			}
		}
	}

	if(next_turn){ 
		mchTurn ++;
		stQuant();
	}
	setActiveRacer(p,mode);
}

int mchRaceDispatcher::checkEndGame(void)
{
	mchRacer* p;
	int i,human_players = 0;

	if(mchPBEM_Game)
		return online_checkEndGame();

	if(mchGameMode == MCH_SINGLE_GAME){
		if(mch_demoD.is_enabled() && mchTimeMode != MCH_TIME_GAME_OVER){
			p = racerLst -> first();
			while(p){
				if(p -> flags & MCH_FINISHED) return 1;
				p = p -> next;
			}

			return 0;
		}

		if(!mchSplitScreenGame){
			if(racerLst -> last() -> Type == MCH_RACER_TYPE_BOSS && racerLst -> last() -> flags & MCH_FINISHED) return 1;

			if(!(activeRacer -> flags & MCH_FINISHED)) return 0;
		}
		else {
			if(!(activeRacer -> flags & MCH_FINISHED) || !(activeRacer2 -> flags & MCH_FINISHED)) return 0;
		}
	}
	else {
		for(i = 0; i < HS_PLAYERS_MAX; i ++){
			if(hsPlayers[i].type == 1)
				human_players ++;
		}

		if(human_players){
			p = racerLst -> first();
			while(p){
				if(!(p -> flags & MCH_FINISHED) && !(p -> flags & MCH_AI)) return 0;
				p = p -> next;
			}
		}
		else {
			p = racerLst -> first();
			while(p){
				if(!(p -> flags & MCH_FINISHED)) return 0;
				p = p -> next;
			}
		}
	}
	return 1;
}

void mchRacerStats::save(XStream* fh)
{
	int i;
	*fh < ID < poolID < essenceID < flags;
	*fh < time < cur_time < start_time;
	*fh < place < prev_place < placeTimer;

	*fh < last_place;

	for(i = 0; i < 5; i ++)
		*fh < last_config[i];

	*fh < Lap < Checkpoint < Turn < NStar;
	fh -> write(partFlags,ARCANE_MAX);

	*fh < (int) strlen(name);
	fh -> write(name,strlen(name));

	fh -> write(mechos_config,30);
	fh -> write(backup_config,30);
}

void mchRacerStats::save(XBuffer* fh)
{
	int i;
	*fh < ID < poolID < essenceID < flags;
	*fh < time < cur_time < start_time;
	*fh < place < prev_place < placeTimer;

	*fh < last_place;

	for(i = 0; i < 5; i ++)
		*fh < last_config[i];

	*fh < Lap < Checkpoint < Turn < NStar;
	fh -> write(partFlags,ARCANE_MAX);

	*fh < (int) strlen(name);
	fh -> write(name,strlen(name));

	fh -> write(mechos_config,30);
	fh -> write(backup_config,30);
}

void mchRacerStats::load(XBuffer* xb)
{
	int i,num;
	*xb > ID > poolID > essenceID > flags;
	*xb > time > cur_time > start_time;
	*xb > place > prev_place > placeTimer;

	*xb > last_place;

	for(i = 0; i < 5; i ++)
		*xb > last_config[i];

	*xb > Lap > Checkpoint > Turn > NStar;
	xb -> read(partFlags,ARCANE_MAX);

	*xb > num;
	xb -> read(name,num);
	name[num] = 0;

	xb -> read(mechos_config,30);
	xb -> read(backup_config,30);
}

unsigned mch_clock(void)
{
//	return non_stop_global_time();
	return global_time();
}

void mchRaceDispatcher::AddSeeds(int mode)
{
	mchRacer* p;

	if(mode){
		if(hsSeedsAccumulation)
			activeRacer -> numSeeds += hsTurnSeedsMax;
		else
			activeRacer -> numSeeds = hsTurnSeedsMax;
	}
	else {
		p = racerLst -> first();
		while(p){
			if(hsSeedsAccumulation)
				p -> numSeeds += hsTurnSeedsMax;
			else
				p -> numSeeds = hsTurnSeedsMax;
			p = p -> next;
		}
	}
}

void mchRaceDispatcher::InitSeeds(void)
{
}

void mchRaceDispatcher::CenterCamera(void)
{ 
//	mchSetCamera(activeRacer -> R().x,activeRacer -> R().y);
}

// v1 - dest
// v2 - src
void mch_vect_axis_rotate(Vect3f& v1,Vect3f v2,Vect3f axis,float angle)
{
	float cosa,sina;

	float mat[4][4];

	cosa = cos((M_PI/180.0) * angle);
	sina = sin((M_PI/180.0) * angle);

	mat[0][0] = (axis[0] * axis[0]) + ((1.0 - (axis[0] * axis[0]))*cosa);
	mat[0][1] = (axis[0] * axis[1] * (1.0 - cosa)) - (axis[2] * sina);
	mat[0][2] = (axis[0] * axis[2] * (1.0 - cosa)) + (axis[1] * sina);
	mat[0][3] = 0.0;

	mat[1][0] = (axis[0] * axis[1] * (1.0 - cosa)) + (axis[2] * sina);
	mat[1][1] = (axis[1] * axis[1]) + ((1.0 - (axis[1] * axis[1])) * cosa);
	mat[1][2] = (axis[1] * axis[2] * (1.0 - cosa)) - (axis[0] * sina);
	mat[1][3] = 0.0;

	mat[2][0] = (axis[0] * axis[2] * (1.0 - cosa)) - (axis[1] * sina);
	mat[2][1] = (axis[1] * axis[2] * (1.0 - cosa)) + (axis[0] * sina);
	mat[2][2] = (axis[2] * axis[2]) + ((1.0 - (axis[2] * axis[2])) * cosa);
	mat[2][3] = 0.0;

	mat[3][0] = mat[3][1] = mat[3][2] = mat[3][3] = 0.0;

	v1[0] = (v2[0] * mat[0][0]) + (v2[1] * mat[1][0]) + (v2[2] * mat[2][0]) + mat[3][0];
	v1[1] = (v2[0] * mat[0][1]) + (v2[1] * mat[1][1]) + (v2[2] * mat[2][1]) + mat[3][1];
	v1[2] = (v2[0] * mat[0][2]) + (v2[1] * mat[1][2]) + (v2[2] * mat[2][2]) + mat[3][2];
}

void mchRaceDispatcher::AI_Quant(mchRacer* r)
{
	int i,cnt,stop;

	if(!r -> seedLst.size()){
		if(mchTrackDemoMode || AI_CheckRacerCP(r)){
			cnt = AI_GetPrm(r) -> maxSeedCnt;
			for(i = 0; i < cnt; i ++)
				AI_SetSeed(r);
		}
		else {
			move2checkpoint(r,getCP(r -> stPtr -> Checkpoint));
		}
	}
	else {
		if(r -> flags & MCH_AI)
			stop = r -> stopFnc();
		else
			stop = 0;

		if(!mchTrackDemoMode && !AI_CheckRacerCP(r)){
			r -> RemoveSeeds();
			move2checkpoint(r,getCP(r -> stPtr -> Checkpoint));
		}
		else {
			if(!stop){
				AI_SeedQuant(r);
				cnt = AI_GetPrm(r) -> maxSeedCnt - r -> seedLst.size();
				for(i = 0; i < cnt; i ++)
					AI_SetSeed(r);
			}
		}
	}
}

void mchRaceDispatcher::AI_Init(mchRacer* r)
{
	mchTrackBranch* br;
	mchCheckpoint* cp;

	cp = getCP(r -> stPtr -> Checkpoint);

	if(!cp){
		r -> flags &= ~MCH_AI;
		return;
	}

	br = cp -> getTrack();

	if(br){
		r -> trackBr = br;
		r -> trackP = mch_trackMap -> GetTrackEl(r -> R(),r -> radius(),r -> radius());
		if(!r -> trackP){
			r -> trackP = br -> pointLst -> last();
		}
	}
	else
		r -> flags &= ~MCH_AI;
}

void mchRaceDispatcher::move2checkpoint(mchRacer* r,mchCheckpoint* cp,int mode)
{
	int x,y,z;
	GetChpCenterPos(cp,x,y,z);
//	r -> AddSeed(Vect3f(x,y,z + 20));

	mchTrackBranch* br;

	br = cp -> getTrack();
	r -> trackBr = br;
	if(br){
//		r -> trackP = br -> pointLst -> first();
		r -> trackP = br -> pointLst -> last() -> prev;


		if(!mode){
			x = XCYCL(r -> trackP -> Cx);
			y = XCYCL(r -> trackP -> Cy);
			z = r -> trackP -> Cz;
		}
		else {
			x = XCYCL((cp -> PosX0 + cp -> PosX1)/2);
			y = XCYCL((cp -> PosY0 + cp -> PosY1)/2);
			z = ZV(x,y);
		}

		r -> AddSeed(Vect3f(x,y,z));
		if(r -> seedLst.size())
			r -> seedLst.last() -> trackEl = r -> trackP;
		r -> flags |= MCH_GET_SEED;
	}
}

void mchRaceDispatcher::move2trackpoint(mchRacer* r,mchTrackElement* tp)
{
	int sz = r -> seedLst.size();
	r -> AddSeed(*tp);

	if(r -> seedLst.size() == sz + 1)
		r -> seedLst.last() -> trackEl = tp;
}

void mchRaceDispatcher::TrackInit(void)
{
	int i,sz,dist,id;
	mchTrackBranch* p,*p1,*np;
	mchCheckpoint* cp = cpLst -> first();

	xtList<mchTrackBranch> tmpLst;

	while(cp){
		p = mchTrackData -> first();
		while(p){
			np = p -> next;
			if(p -> ID == cp -> ID){
				mchTrackData -> remove(p);
				cp -> trackPtr -> append(p);
				p -> owner = cp;
			}
			p = np;
		}

		sz = cp -> trackPtr -> size();
		if(sz > 1){
			for(i = 0; i < sz; i ++){
				p = cp -> trackPtr -> first();
				while(p){
					np = p -> next;
					if(p -> index == i && !p -> flags){
						cp -> trackPtr -> remove(p);
						tmpLst.append(p);
					}
					p = np;
				}
			}

			while(cp -> trackPtr -> size()){
				p = cp -> trackPtr -> first();
				while(p){
					np = p -> next;
					p1 = tmpLst.first();
					while(p1){
						if(p1 -> index == p -> index && p1 -> flags == p -> flags - 1){
							p1 -> merge(p);
							p1 -> flags ++;

							cp -> trackPtr -> remove(p);
							delete p;
						}
						p1 = p1 -> next;
					}
					p = np;
				}
			}
			p = tmpLst.first();
			while(p){
				tmpLst.remove(p);
				cp -> trackPtr -> append(p);
				p = tmpLst.first();
			}
		}
		cp -> trackSize = cp -> trackPtr -> size();
		cp = cp -> next;
	}

	id = 0;
	sz = 0;
	dist = 0;
	cp = cpLst -> first();
	while(cp){
		if(cp -> trackPtr -> size()){
			p = cp -> trackPtr -> first();
			while(p){
				mch_trackMap -> InsertBranch(p);
				sz += p -> pointLst -> size();
				p -> init_dist(dist);
				p -> init_ids(id);
				p = p -> next;
			}
			dist += sz / cp -> trackPtr -> size();
			sz = 0;
		}
		cp = cp -> next;
	}
	mchLapLength = dist;
}

mchTrackBranch* mchCheckpoint::getTrack(void)
{
	int i,id = 0;
	mchTrackBranch* p = trackPtr -> first();
	if(trackSize > 1) id = XRnd(trackSize);

	for(i = 0; i < id; i ++)
		p = p -> next;

	return p;
}

int mchRacer::CheckBonusSlot(int id)
{
	mchBonusArcaneData* p;

	if(mchTrackDemoMode) return 0;
	if(id == ARCANE063) return 1;

	p = bonusLst.search(id);
	if(p) 
		return 1;
	else {
		if(bonusLst.size() < 3)
			return 1;
	}

	return 0;
}

void mchRacer::AddBonus(int id)
{
	mchBonusArcaneData* p;

	if(flags & MCH_ACTIVE_RACER)
		mchStartSoundEFF(EFF_BONUS,R().x,R().y);

	p = mchGetBonusDataByType(id);
	if(!p) return;

	fxlabBonusEvent(p -> ID,this);
}

void mchMechosRacer::AddBonus(int id)
{
	int i;
	cMesh* m;

	mchBonusArcaneData* p;

	startEffect(BonusExultEffect);

	if(flags & MCH_ACTIVE_RACER)
		mchStartSoundEFF(EFF_BONUS,R().x,R().y);

	p = mchGetBonusDataByType(id);
	if(!p) return;

	if(p -> ID == ARCANE062){ // Mana
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			m = part_by_type(mchPartID[i]);
			if(m && m -> arcaneData.ID != -1){
				if(m -> arcaneData.Power < m -> arcaneData.MaxPower){
					fxlabBonusEvent(p -> ID,this);
					return;
				}
			}
		}
		return;
	}
	if(p -> ID == ARCANE053){ // Energy
		if(EnergyValue() < 0.99f)
			fxlabBonusEvent(p -> ID,this);
		return;
	}

	fxlabBonusEvent(p -> ID,this);
}

void mchRacer::RemoveBonus(int id)
{
	mchBonusArcaneData* p;
	if(id != -1){
		p = bonusLst.search(id);
		if(p){
			bonusLst.remove(p);
			delete p;
		}
	}
	else {
		p = bonusLst.first();
		while(p){
			bonusLst.remove(p);
			delete p;
			p = bonusLst.first();
		}
	}
}	

int mchRacer::CheckBonus(int id)
{
	if(bonusLst.search(id)) return 1;
	return 0;
}

mchCheckpoint* mchRaceDispatcher::getCP(int id)
{
	int sz = cpLst -> size();
	if(!sz)
		return 0;
	while(id < 0) id += sz;
	while(id >= sz) id -= sz;
	return cpLst -> search(id);
}

void mchInitRacers(void)
{
	int i,id,num,hs_flag = 0;
	mchRacerStats* rs,*rs0,*rs1;

	char essence_flags[20];

	mchInitPartIDs();

	num = mchNumRacers + 1;
	if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
		hs_flag = 1;
		num = HS_PLAYERS_MAX;
	}
	if(num > HS_PLAYERS_MAX) num = HS_PLAYERS_MAX;

	if(!mch_racerLst.size()){
		mchCreatePartsPool();
		for(i = 0; i < HS_PLAYERS_MAX; i ++){
//		for(i = 0; i < num; i ++){
			rs = new mchRacerStats;
			rs -> ID = i;

			if(!hs_flag){
				if(i){
					if(i == num - 1) rs -> last_place = num - 1;
					else rs -> last_place = i - 1;
				}
				else 
					rs -> last_place = num - 2;
				if(rs -> last_place < 0) rs -> last_place = 0;
			}
			else
				rs -> last_place = 0;

//			strcpy(rs -> name,hsPlayers[i].name);

			if(!hs_flag)
				rs -> poolID = 0;
			else {
				if(hsPlayers[i].type)
					rs -> poolID = (hsPlayers[i].type > 1) ? num : i;
				else
					rs -> poolID = num + 1;
			}

			mch_racerLst.append(rs);
		}
			
		rs0 = mch_racerLst.search(0);

		if(mchDefaultFigure != -1)
			rs0 -> essenceID = mchDefaultFigure;
		else
			rs0 -> essenceID = XRnd(5);

		strcpy(rs0 -> name,mch_playerNames[rs0 -> essenceID]);
		memcpy(rs0 -> partFlags,mch_partFlags,ARCANE_MAX);
		rs0 -> GetMechos();
//		memset(rs0 -> partFlags,0,ARCANE_MAX);

		rs = mch_racerLst.first();
		while(rs){
			if(rs != rs0) rs -> essenceID = -1;
			rs = rs -> next;
		}

		memset(essence_flags,0,20);
		essence_flags[rs0 -> essenceID] = 1;

		for(id = 0; id < 10; id ++){
			if(!mch_trackD -> FigureEnabled(id))
				essence_flags[id] = 1;
		}

		rs = mch_racerLst.first();
		while(rs){
			if(rs != rs0){
				id = 0;
				num = XRnd(acsMaxEssenceID);
				while(num){
					id ++;
					if(id >= acsMaxEssenceID) id = 0;
					while(essence_flags[id]){ 
						id ++;
						if(id >= acsMaxEssenceID) id = 0;
					}
					num --;
				}
				while(essence_flags[id]){ 
					id ++;
					if(id >= acsMaxEssenceID) id = 0;
				}
				rs -> essenceID = id;
				essence_flags[id] = 1;
			}
			rs = rs -> next;
		}

		for(i = 0; i <= rs0 -> last_place; i ++){
			rs = mch_racerLst.first();
			while(rs){
				if(rs != rs0 && rs -> last_place == i)
					rs -> GetMechos();
				rs = rs -> next;
			}
		}
	}
	else {
		rs0 = mch_racerLst.search(0);
		rs1 = mch_racerLst.search(1);

		rs = mch_racerLst.first();
		while(rs){
			rs -> PutMechos();
			rs = rs -> next;
		}
		mchResetPartsPool(0);

		rs0 -> GetMechos(1);
		if(mchSplitScreenGame)
			rs1 -> GetMechos(1);

		for(i = 0; i < rs0 -> last_place; i ++){
			rs = mch_racerLst.first();
			while(rs){
				if(rs != rs0 && (!mchSplitScreenGame || rs != rs1) && rs -> last_place == i)
					rs -> GetMechos();
				rs = rs -> next;
			}
		}
	}
	mchRacersInitFlag = 1;
}

void mchRacerStats::GetMechos(int mode)
{
	int i;
	int id[NUM_ACTIVE_PARTS];
	XBuffer XBuf;

	if((flags & MCH_RACER_MECHOS_CFG))
		PutMechos();

	mchGetMechos(mechos_config,30,partFlags,poolID,mode);

	XBuf.write(mechos_config,30);
	XBuf.set(0,XB_BEG);

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		XBuf >= id[i];

	id[0] = essenceID + 1;

	XBuf.init();
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		XBuf <= id[i] < " ";
		last_config[i] = id[i];
	}

	XBuf.set(0,XB_BEG);
	XBuf.read(mechos_config,30);

	flags |= MCH_RACER_MECHOS_CFG;

	memcpy(backup_config,mechos_config,30);
}

void mchRacerStats::PutMechos(void)
{
	if(flags & MCH_RACER_MECHOS_CFG){
		mchPutMechos(mechos_config,poolID);
		flags ^= MCH_RACER_MECHOS_CFG;
	}
}

void mchRacerStats::UpdateMechosCfg(void)
{
	int i,arc_id;
	int cfg[NUM_ACTIVE_PARTS];

	XBuffer mechos_cfg(mechos_config,30);
	XBuffer backup_cfg(backup_config,30);

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		backup_cfg >= cfg[i];

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		arc_id = mchGetPartArcane(i,last_config[i]);
		if(arc_id != -1 && !isAuthorized(arc_id))
			mechos_cfg <= cfg[i] < " ";
		else
			mechos_cfg <= last_config[i] < " ";
	}
}

void mchFinitRacers(void)
{
	mchRacerStats* p = mch_racerLst.first();
	while(p){
		mch_racerLst.remove(p);
		delete p;
		p = mch_racerLst.first();
	}
	mchDeletePartsPool();
	mchRacersInitFlag = 0;
}

mchRacerStats* mchGetRacer(int id)
{
	return mch_racerLst.search(id);
}

void mchResetRacers(void)
{
	mchRacerStats* p = mch_racerLst.first();
	while(p){
		if(p -> ID) p -> PutMechos();
		else p -> UpdateMechosCfg();
		p -> init();
		p = p -> next;
	}
}


void mchRaceDispatcher::redraw(void)
{
#ifdef _RACE_DEBUG_
/*
	XBuffer XBuf;
	XBuf <= mch_raceMinIdx < " " <= mch_raceMaxIdx;
	if(RenderMode == XGRAPH_HICOLOR)
		XGR_OutString(gameWnd -> PosX + 5,gameWnd -> PosY + 20,0xFFFF,XBuf.address(),1);
	else
		mchA_d3dOutString(gameWnd -> PosX + 5,gameWnd -> PosY + 20,0.8f,0.8f,XBuf.address(),mchA_ColorF[6],160,2,0);
*/
	if(activeRacer -> trackBr) 
		activeRacer -> trackBr -> redraw();

	gameWnd -> set_clipping();
//	mch_trackMap -> show(mchactiveRacer -> R().x,activeRacer -> R().y,activeRacer -> radius(),0);
	mch_trackMap -> show(mchCameraX,mchCameraY,400,0);
	gameWnd -> remove_clipping();
#endif 
}

void mch_AI_Prm::init(const char* ini_file,const char* section)
{
	arcaneRnd = atoi(getIniKey(ini_file,section,"arcane_rnd"));

	maxSeedCnt = atoi(getIniKey(ini_file,section,"seed_count"));
	maxTrackElCnt = atoi(getIniKey(ini_file,section,"track_len"));
	maxTrackElCntDelta = atoi(getIniKey(ini_file,section,"track_len_delta"));

	maxTurnAngle = atof(getIniKey(ini_file,section,"turn_angle")) * M_PI / 180.0;
	maxTurnAngleDelta = atof(getIniKey(ini_file,section,"turn_angle_delta")) * M_PI / 180.0;

	tractionFlag = atoi(getIniKey(ini_file,section,"traction_flag"));
	tractionPrm = atof(getIniKey(ini_file,section,"traction_prm"));

	tractionPrmMin = atof(getIniKey(ini_file,section,"traction_prm_min"));
	tractionPrmMax = atof(getIniKey(ini_file,section,"traction_prm_max"));

	tractionDistMin = atoi(getIniKey(ini_file,section,"traction_dist_min"));
	tractionDistMax = atoi(getIniKey(ini_file,section,"traction_dist_max"));
}

void mchRaceDispatcher::AI_SetSeed(mchRacer* r)
{
	int i,index,x,y,x1,y1;
	float angle,max_angle;

	mch_AI_Prm* ai_prm = &mchAI_Cfg[r -> AI_Type];

	mchTrackBranch* br;
	mchTrackElement* p,*p0,*p1;
	mchCheckpoint* cp;

	if(r -> check_points_only_controlled()){
		if(r -> seedLst.size())
			return;

		cp = getCP(r -> stPtr -> Checkpoint);
		move2checkpoint(r,cp,1);
	}

	if(r -> seedLst.size()){
		p = r -> seedLst.last() -> trackEl;
		if(!p) return;

		br = p -> owner;
	}
	else {
		p = r -> trackP;
		br = r -> trackBr;
	}

	if(br && p){
		angle = 0;
		max_angle = ai_prm -> maxTurnAngle + (float)XRnd(200) * ai_prm -> maxTurnAngleDelta / 100.0f - ai_prm -> maxTurnAngleDelta;
		index = ai_prm -> maxTrackElCnt + XRnd(ai_prm -> maxTrackElCntDelta * 2) - ai_prm -> maxTrackElCntDelta;

		p0 = NULL;
		p1 = p;
		for(i = 0; i < index; i ++){
			p = p -> next;
			if(!p){
				cp = br -> owner -> next;
				if(!cp) cp = cpLst -> first();

				br = cp -> getTrack();
				if(br)
					p = br -> pointLst -> first();
			}
			if(p0 && p1){
				x = p1 -> Cx - p0 -> Cx;
				y = p1 -> Cy - p0 -> Cy;
				x1 = p -> Cx - p1 -> Cx;
				y1 = p -> Cy - p1 -> Cy;

				angle += Acos((float)(x * x1 + y * y1)/(sqrt((float)(x * x + y * y)) * sqrt((float)(x1 * x1 + y1 * y1)) + FLT_EPS));
			}
			p0 = p1;
			p1 = p;

			if(fabs(angle) > max_angle){
				p = p0;
				break;
			}
		}		
		if(p) move2trackpoint(r,p);
	}
}

void mchInitAI(void)
{
	int i;

	mchAI_ArcanePrm* prm;
	scrDataBlock* b,*root;

	XBuffer XBuf;
	XStream fh;

	if(!mchAI_RealTimeCfg){
		mchAI_RealTimeNumCfg = atoi(getIniKey(mch_botsINI,"general","num_rt_ai_cfg"));
		mchAI_RealTimeCfg = new mch_AI_Prm[mchAI_RealTimeNumCfg];
		for(i = 0; i < mchAI_RealTimeNumCfg; i ++){
			XBuf < "rt_ai_cfg" <= i;
			mchAI_RealTimeCfg[i].init(mch_botsINI,XBuf.address());
			XBuf.init();
		}
	}

	if(!mchAI_TurnBasedCfg){
		mchAI_TurnBasedNumCfg = atoi(getIniKey(mch_botsINI,"general","num_tb_ai_cfg"));
		mchAI_TurnBasedCfg = new mch_AI_Prm[mchAI_TurnBasedNumCfg];
		for(i = 0; i < mchAI_TurnBasedNumCfg; i ++){
			XBuf < "tb_ai_cfg" <= i;
			mchAI_TurnBasedCfg[i].init(mch_botsINI,XBuf.address());
			XBuf.init();
		}
	}

	if(!mchAI_Arcanes.size()){
		acsOpenResource("arcane.scb",fh);
		root = loadScript(fh);
		if(root -> nextLevel)
			b = root -> nextLevel -> first();
		else 
			b = NULL;

		while(b){
			switch(b -> ID){
				case ARC_PRM:
					prm = new mchAI_ArcanePrm(b);
					mchAI_Arcanes.append(prm);
					break;
			}
			b = b -> next;
		}
		delete root;
	}
}

void mchRaceDispatcher::AI_SetPrm(mchRacer* p)
{
	mch_AI_Prm* prm;

//	p -> AI_Type = XRnd(mchAI_NumCfg);
	p -> AI_Type = mch_trackD -> TrackAI_Level();
	prm = &mchAI_Cfg[p -> AI_Type];

	p -> SetTractionPrm(prm -> tractionFlag,prm -> tractionPrm);
}

int mchRaceDispatcher::AI_CheckRacerCP(mchRacer* p)
{
	int cp_flag = 1;

	mchTrackBranch* br;
	mchCheckpoint* cp;

	cp = getCP(p -> stPtr -> Checkpoint);
	br = p -> trackBr;
	if(br){
		if(br -> owner == cp || br -> owner == cp -> prev)
			cp_flag = 1;
		else {
			if(p -> last_trackP0 && p -> last_trackP0 -> owner == p -> trackBr)
				cp_flag = 0;
		}
	}
	return cp_flag;
}

mch_AI_Prm* mchRaceDispatcher::AI_GetPrm(mchRacer* p)
{
	mch_AI_Prm* ai_prm = &mchAI_Cfg[p -> AI_Type];
	return ai_prm;
}

void mchRaceDispatcher::AI_ArcaneQuant(mchMechosRacer* p)
{
	int arc_type,fl;

	cMesh* m;
	mch_AI_Prm* pr;
	mchAI_ArcanePrm* prm;

	if(p -> State() != NormalRacer || mchTimeMode == MCH_TIME_GAME_OVER) return;

	pr = AI_GetPrm(p);

	for(arc_type = 0; arc_type < NUM_ACTIVE_PARTS; arc_type ++){
		fl = 0;
		prm = NULL;
		m = p -> part_by_type(mchPartID[arc_type]);

//		if(m && m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
		if(m)
			prm = mchAI_Arcanes.search(m -> arcaneData.ID);

		if(!XRnd(pr -> arcaneRnd) && prm && p -> last_trackP){
			switch(m -> arcaneData.ID){
				case ARCANE002: // Petard
				case ARCANE003: // Lurid Billow
				case ARCANE004: // Fury of Slime
				case ARCANE005: // Pesky Mould
				case ARCANE007: // Evil Eye
				case ARCANE008: // Bubble Gum
				case ARCANE009: // Crystallization
				case ARCANE011: // Scarlet Trace
				case ARCANE012: // Overlarge Tomato
				case ARCANE015: // Magnetic Field
				case ARCANE016: // Whirlwind
				case ARCANE017: // Spiders Attack
				case ARCANE019: // Soap-Bubbles
				case ARCANE021: // Caterpillars
				case ARCANE024:	// Star Shower
				case ARCANE025: // Thunderstorm
				case ARCANE029: // Electric Seeds
				case ARCANE048: // Teleportation Trap
				case ARCANE061:	// Mines
				case ARCANE064:	// Snowfall
//					if(AI_GetTrackEl(p -> last_trackP,TRACK_EL_BUSY,prm -> prmData[AI_ARCANE_DIST_PRM],0))
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						fl = 1;
					break;
				case ARCANE032:	// Electric Body
				case ARCANE034:	// Kinetic Hit
				case ARCANE036:	// Insatiable Tentacle
				case ARCANE039:	// Voodoo Master
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						fl = AI_CheckArea(p,prm -> prmData[AI_ARCANE_DIST_PRM]);
					break;
				case ARCANE033: // X-Ray Attack
				case ARCANE035: // Sting
				case ARCANE037: // Tomatoes Attack
				case ARCANE038:	// Iron Side
				case ARCANE040: // Fireballs
				case ARCANE066:	// Sparking
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						if(AI_GetTrackEl(p -> last_trackP,TRACK_EL_BUSY,prm -> prmData[AI_ARCANE_DIST_PRM]))
							fl = 1;
					break;
				case ARCANE052:	// Frog
					if(m -> arcaneData.Status){
						if(m -> arcaneData.Power >= m -> arcaneData.ActivatePower && !p -> submersion() && (AI_GetTrackEl(p -> last_trackP,TRACK_EL_FIRE | TRACK_EL_SLIME,prm -> prmData[AI_ARCANE_DIST_PRM]) || p -> stPtr -> place >= racerLst -> size() - 2))
							fl = 1;
					}
					else {
						if(p -> submersion())
							fl = 2;
					}
					break;
				case ARCANE045: // Ball of Speed
				case ARCANE050: // Dragon Power
				case ARCANE051:	// Helicopter
				case ARCANE059:	// Butterfly
				case ARCANE060:	// Dirigible
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						if(AI_GetTrackEl(p -> last_trackP,TRACK_EL_FIRE | TRACK_EL_SLIME,prm -> prmData[AI_ARCANE_DIST_PRM]) || p -> stPtr -> place >= racerLst -> size() - 2)
							fl = 1;
					break;
				case ARCANE023: // Skin Shield
				case ARCANE056: // Armor
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						if(AI_GetTrackEl(p -> last_trackP,TRACK_EL_FIRE,prm -> prmData[AI_ARCANE_DIST_PRM]))
							fl = 1;
					break;
				case ARCANE057: // Insulation
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						if(AI_GetTrackEl(p -> last_trackP,TRACK_EL_ELECTRICITY,prm -> prmData[AI_ARCANE_DIST_PRM]))
							fl = 1;
					break;
				case ARCANE022: // Nullification
				case ARCANE014: // Magnetic Cushion
				case ARCANE018: // Formicary
				case ARCANE030: // BioPower
				case ARCANE041:	// Clown
				case ARCANE042:	// Suicide
				case ARCANE058: // 0-Transportation
					if(m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower)
						fl = 1;
					break;
			}

			if(fl == 1){
				_RACE_LOG_STR("AI Arcane Start -> ");
				_RACE_LOG_TXT(mch_clock());
				_RACE_LOG_STR("\t");
				_RACE_LOG_TXT(p -> ID);
				_RACE_LOG_STR("\t");
				_RACE_LOG_TXT(m -> arcaneData.ID);
				_RACE_LOG_STR("\r\n");

				m -> arcaneData.ownerID = p -> ID;
				m -> arcaneData.fxlabStart();
				p -> stPtr -> NArcanes ++;
			}
			if(fl == 2){
				_RACE_LOG_STR("AI Arcane Stop -> ");
				_RACE_LOG_TXT(mch_clock());
				_RACE_LOG_STR("\t");
				_RACE_LOG_TXT(p -> ID);
				_RACE_LOG_STR("\t");
				_RACE_LOG_TXT(m -> arcaneData.ID);
				_RACE_LOG_STR("\r\n");

				m -> arcaneData.ownerID = p -> ID;
				m -> arcaneData.fxlabStop();
			}
		}
	}
}

int fxlabValideArcane(int id);

void mchRaceDispatcher::ArcaneQuant(mchRacer* p,float dt)
{
	int i;
	float delta;

	cMesh* m;
	mchBonusArcaneData* bp,*bp1;
	bp = p -> bonusLst.first();
	while(bp){
		bp -> Timer -= dt;
		bp1 = bp -> next;
		if(bp -> Timer < 0.0f) {
			p -> bonusLst.remove(bp);
			delete bp;
		}
		bp = bp1;
	}

	if(mchTurnBasedGame) dt *= 3.0f;

	_RACE_LOG_STR("Arcanes -> ");

	mchMechosRacer* rp;

	if(p -> Type == MCH_RACER_TYPE_MECHOS){
		rp = (mchMechosRacer*)p;
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			m = ((mchMechosRacer*)p) -> part_by_type(mchPartID[i]);
			if(m && fxlabValideArcane(m -> arcaneData.ID) && m -> arcaneData.Status && m -> arcaneData.Power < m -> arcaneData.MaxPower){
				delta = dt * m -> arcaneData.ChargePower * rp -> diff_data().mana_charge_factor;
				m -> arcaneData.Power += delta;
				if(m -> arcaneData.Power > m -> arcaneData.MaxPower)
					m -> arcaneData.Power = m -> arcaneData.MaxPower;

				if(m -> arcaneData.ID != -1){
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(m -> arcaneData.ID);
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(round(m -> arcaneData.Power * 100.0f));
				}
			}
		}
	}

	_RACE_LOG_STR("\r\n");
}

void mchRaceDispatcher::ArcaneInit(mchRacer* p)
{
	int i;

	cMesh* m;
	if(p -> Type == MCH_RACER_TYPE_MECHOS){
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			m = ((mchMechosRacer*)p) -> part_by_type(mchPartID[i]);
			if(m && m -> arcaneData.Status){
				if(!mchTurnBasedGame && (!iworld_D || !iworld_D -> CheckFlag(IW_TURNBASED_FLAG)))
					m -> arcaneData.Power = 0;
				else
					m -> arcaneData.Power = m -> arcaneData.MaxPower/2;
			}
		}
	}
}

void mchRaceDispatcher::CtlQuant(void)
{
	mchRacer* p = racerLst -> first();
	while(p){
		if(p -> State() != NormalRacer)
			p -> RemoveBonus(-1);
		p = p -> next;
	}
}

void mchRaceDispatcher::CP_Quant(void)
{
	mchRacer* p = racerLst -> first();
	while(p){
		CP_Quant(p);
		switch(p -> State()){
			case NormalRacer:
				break;
			case DestroyedRacer:
				if((p -> flags & MCH_AI) || !p -> seedLst.size())
					CenterRacer(p);
				break;
			case AssemblingRacer:
				break;
			}
		p = p -> next;
	}
}

void mchRaceDispatcher::CP_Quant(mchRacer* p)
{
	int x1,y1,z1,x2,y2,z2,last_cp,cp_id,fl;
	mchCheckpoint* cp,*cp0;

	cp = getCP(p -> stPtr -> Checkpoint);
	if(cp){ 
		if(!(p -> flags & MCH_SEED_PASSED)){
			if(!p -> check_points_only_controlled()){
				p -> getPosition(x1,y1,z1);
				p -> get_prevPosition(x2,y2,z2);
				fl = cp -> check(x1,y1,x2,y2);
			}
			else
				fl = 0;
		}
		else {
			fl = 1;
			p -> flags &= ~MCH_SEED_PASSED;
		}

		if(fl){
			p -> prevCp = cp;
			p -> flags &= ~MCH_GET_SEED;

			if(!(p -> flags & MCH_FINISHED)){
				if(!mch_demoD.is_enabled())
					p -> NStar += getCheckpointNStar(p);
				p -> stPtr -> Checkpoint ++;

				_RACE_LOG_STR("PassCp -> ");
				_RACE_LOG_TXT(p -> ID);
				_RACE_LOG_STR("\r\n");

				fxlabRacerEvent(FX_RACER_EVENT_RIGHT_CHECK_POINT,p);
				if(p -> stPtr -> Checkpoint > cpLst -> size()){
					p -> stPtr -> Checkpoint = 1;
					if(p -> stPtr -> Lap >= mchNumLaps - 1){
						if(mchTimeMode != MCH_TIME_GAME_OVER)
							p -> flags |= MCH_FINISHED;

						if(mchGameMode == MCH_ENTIRE_CONTROL_HS)
							mchGameFlags |= MCH_STOP_TIME_FLAG;

						if(!(p -> flags & MCH_AI))
							mch_raceD -> AI_Toggle(p);

						fxlabRacerEvent(FX_RACER_EVENT_FINISH,p);
						mchStartSoundEFF(EFF_FINISH,p -> R().x,p -> R().y);

						if(!(p -> flags & MCH_ACTIVE_RACER))
							p -> StartEffect(Mechos::Effect::ExultEffect);
						else {
							if(mchGameMode == MCH_SINGLE_GAME){
								if(p -> flags & MCH_ACTIVE_RACER2)
									camera_dispatcher2 -> runFinishScenario();
								else
									camera_dispatcher -> runFinishScenario();
							}

							if(mchGameMode == MCH_SPLIT_CONTROL_HS && !checkEndGame())
								mchGameFlags |= MCH_STOP_TIME_FLAG;
						}

						p -> stPtr -> Turn = mchTurn;
					}
//					if(!mch_demoD.is_enabled() && mchTimeMode != MCH_TIME_GAME_OVER)
					if(mchTimeMode != MCH_TIME_GAME_OVER)
						p -> stPtr -> Lap ++;
					fxlabRacerEvent(FX_RACER_EVENT_RIGHT_CHECK_POINT,p);

//					if(!mchPBEM_Game && mchTimeMode != MCH_TIME_GAME_OVER){
					if(mchTimeMode != MCH_TIME_GAME_OVER){
						if(checkEndGame()) 
							mchEndGame();
						else {
							if(p -> flags & MCH_ACTIVE_RACER && !mchPBEM_HiddenTime) 
								mchA_ShowLapEnable(p -> flags & MCH_ACTIVE_RACER2);
						}
					}
				}
			}
			else
				p -> stPtr -> Checkpoint ++;

			cp_id = p -> stPtr -> Checkpoint;
			if(cp_id >= cpLst -> size())
				cp_id -= cpLst -> size();
			last_cp = cp_id - cpLst -> size() / 2;
			if(last_cp < 0){
				last_cp += cpLst -> size();
			}
			p -> ClearCP(last_cp);
			p -> SetCP(cp_id);

			if(p -> flags & MCH_ACTIVE_RACER){
				if(cp -> ID)
					mchStartSoundEFF(EFF_PASS_CHP,p -> R().x,p -> R().y);
				cp0 = getCP(last_cp);
				if(cp0)	cp0 -> set_state(MCH_CP_WRONG,p);
				cp -> set_state(MCH_CP_PASSED,p);
				cp0 = getCP(p -> stPtr -> Checkpoint);
				if(cp0)	cp0 -> set_state(MCH_CP_CURRENT,p);

				mchA_DropCPDisable(p);
			}
			setCheckpointNStar();
		}
	}
}

void mchRaceDispatcher::TrackQuant(void)
{
	mchRacer* p = racerLst -> first();
	mchTrackElement* el;
	mchCheckpoint* cp;

	ClearTrackFlags(1);
	while(p){
		cp = getCP(p -> stPtr -> Checkpoint);

		el = mch_trackMap -> GetTrackEl(p -> R(),p -> radius(),p -> radius(),cp -> ID);
		if(!el) el = mch_trackMap -> GetTrackEl(p -> R(),p -> radius(),p -> radius());

		if(el){ 
			if(p -> State() == NormalRacer){
				el -> flags |= TRACK_EL_BUSY;
				if(el -> owner -> owner == cp){
					p -> last_trackP = el;
					_RACE_LOG_STR("last_trackP -> ");
					_RACE_LOG_TXT(p -> ID);
					_RACE_LOG_STR(" = ");
					_RACE_LOG_TXT(el -> ID);
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(el -> index);
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(el -> Cx);
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(el -> Cy);
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(el -> Cz);
					_RACE_LOG_STR("\r\n");

					_RACE_LOG_TXT(p -> R());
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(p -> radius());
					_RACE_LOG_STR(" ");
					_RACE_LOG_TXT(cp -> ID);
					_RACE_LOG_STR("\r\n");
//					el -> flags |= TRACK_EL_BUSY;
				}
			}
		}
		p -> last_trackP0 = el;
		p = p -> next;
	}
}

void mchRaceDispatcher::ClearTrackFlags(int mode)
{
	mchTrackBranch* p;
	mchCheckpoint* cp = cpLst -> first();
	while(cp){
		p = cp -> trackPtr -> first();
		while(p){
			p -> clear_flags(mode);
			p = p -> next;
		}
		cp = cp -> next;
	}
}

int mchRacer::AddSeed(const Vect3f& point, int tp)
{
	if(mchGameMode == MCH_SPLIT_CONTROL_HS && mchTimeMode != MCH_TIME_GAME_OVER && numSeeds && seedLst.size() >= numSeeds) return seedLst.size();
	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && !(flags & MCH_AI) && mchTimeMode != MCH_TIME_GAME_OVER && numSeeds && seedLst.size() >= numSeeds) return seedLst.size();

	mchControlPoint* p = new mchControlPoint(point.x, point.y, point.z - MCH_CP_DELTA_Z,tp);

	p -> ID = ++mchSeedID;

	seedLst.append(p);
	p -> owner = this;
	if(flags & MCH_ACTIVE_RACER) p -> createObj();

	if(add_control_point(*p))
		curSeed = p;
	else
		curSeed = NULL;

#ifdef _RACE_LOG_
/*
	XBuffer buf;
	if(flags & MCH_ACTIVE_RACER){
		buf.init();
		buf <= ID < " -> " <= p -> PosX < " " <= p -> PosY < " " <= p -> PosZ < "\r\n";
		_RACE_LOG_STR(buf.address());
	}
*/
#endif
	return seedLst.size();
}

int mchRacer::AddSeed(mchControlPoint* pt)
{
	if(mchGameMode == MCH_SPLIT_CONTROL_HS && mchTimeMode != MCH_TIME_GAME_OVER && numSeeds && seedLst.size() >= numSeeds) return seedLst.size();
	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && !(flags & MCH_AI) && mchTimeMode != MCH_TIME_GAME_OVER && numSeeds && seedLst.size() >= numSeeds) return seedLst.size();

	mchControlPoint* p = new mchControlPoint;
	memcpy(p,pt,sizeof(mchControlPoint));

	p -> ID = ++mchSeedID;

	seedLst.append(p);
	p -> owner = this;
	if(flags & MCH_ACTIVE_RACER) p -> createObj();

	if(add_control_point(*p))
		curSeed = p;
	else 
		curSeed = NULL;

	return seedLst.size();
}

int mchRacer::RemoveLastSeed(void)
{
	int sz;
	mchControlPoint* p = seedLst.last();

	sz = seedLst.size();

	if(sz){
		if(sz > 1 && p == curSeed)
			curSeed = p -> prev;
		else
			curSeed = NULL;

		if(mchTurnBasedGame && flags & MCH_ACTIVE_RACER)
			mchStartSoundEFF(EFF_SEED_DELETE,p -> PosX,p -> PosY);

		p -> RemoveArcane(1);
		seedLst.remove(p);
		delete p;
	}

	mouseSeed = NULL;
	del_control_point();

	return seedLst.size();
}

void mchRacer::removePassedSeed()
{
	mchControlPoint* p = seedLst.first();

	if(!p) return;

#ifdef _RACE_LOG_
	mch_raceSaveBuf.init();
	mch_raceSaveBuf < "Seed " <= mch_clock() < " " <= ID < " " <= p -> arcaneData.ID < " " <= p -> PosX < " " <= p -> PosY < " " <= p -> PosZ < "\r\n";
	_RACE_LOG_STR(mch_raceSaveBuf);
#endif

	flags &= ~MCH_GET_SEED;

	fxlabRacerEvent(FX_RACER_EVENT_SEED,this);

	if(mchTurnBasedGame && flags & MCH_ACTIVE_RACER)
		mchStartSoundEFF(EFF_SEED_PICKUP,R().x,R().y);

	lastSeed -> PosX = p -> PosX;
	lastSeed -> PosY = p -> PosY;
	lastSeed -> PosZ = p -> PosZ;

	if(p == curSeed) 
		curSeed = p -> next;

	if(p == mouseSeed){ 
		mouseSeed = NULL;
/*
		if(mchCpLight != -1)
			SetOmniVisible(mchCpLight,0);
*/
	}

	if(flags & MCH_AI){
		if(p -> trackEl){
			trackP = p -> trackEl;
			trackBr = trackP -> owner;
		}
		else {
			trackP = last_trackP;
			if(trackP)
				trackBr = trackP -> owner;
		}
	}

	p -> RemoveArcane();
	seedLst.remove(p);
	delete p;

	if(!curSeed) mouseSeed = NULL;

	numSeeds --;

	stPtr -> NSeeds ++;

	return;
}

int mchRacer::RemoveSelectedSeed(void)
{
	int sz;
	if(!mouseSeed) return seedLst.size();

	sz = seedLst.size();

	if(sz){
		if(sz > 1 && mouseSeed == curSeed)
			curSeed = mouseSeed -> prev;

		del_control_point(*mouseSeed);
		mouseSeed -> RemoveArcane(1);
		seedLst.remove(mouseSeed);  
		delete mouseSeed;
	}

	if(sz == 1) 
		curSeed = NULL;

	mouseSeed = NULL;

	return seedLst.size();
}

void mchRacer::RemoveSeeds(void)
{
	int sz = 1;
	while(sz) sz = RemoveLastSeed();
}

void mchRacer::MoveSeed(int x,int y,int z)
{
	if(!mouseSeed || (mchTimeMode == MCH_TIME_RUNNING && seedLst.size() && mouseSeed == seedLst.first())) return;

	if(mouseSeed -> mPtr)
		z += round((mouseSeed -> mPtr -> zmaxReal() - mouseSeed -> mPtr -> zminReal()) / 2.0);

	move_control_point(*mouseSeed, Vect3f(x,y,z));

	mouseSeed -> PosX = x;
	mouseSeed -> PosY = y;
	mouseSeed -> PosZ = z;

	if(mouseSeed -> mPtr)
	{
		Vect3f v1((float)mouseSeed -> PosX,(float)mouseSeed -> PosY,(float)mouseSeed -> PosZ);
		Vect3f v2(0,0,0);
		gb_IVisGeneric->SetObjectPosition((cUnknownClass*)mouseSeed -> mPtr,&v1,&v2);
	}
}

void mchRacer::NextSeed(void)
{
	if(curSeed)
		curSeed = curSeed -> prev;
}

void mchRacer::ShowSeeds(void)
{
	mchControlPoint* p = seedLst.first();
	while(p){
		p -> createObj();
		p = p -> next;
	}
}

void mchRacer::HideSeeds(void)
{
	mchControlPoint* p = seedLst.first();
	while(p){
		p -> deleteObj();
		p = p -> next;
	}
}

void mchRacer::LightSeed(int& x,int& y,int& z)
{
}

mchMechosRacer::mchMechosRacer(char* cfg_str) : Mechos(cfg_str)
{
	Type = MCH_RACER_TYPE_MECHOS;
	lastStatus = -1;
	lastEnergy = 0;
	energyTimer = -1;
}

void mchRacer::Quant(void)
{
	if(mchTimeMode == MCH_TIME_RUNNING && seedLst.size())
		curSeed = seedLst.first();

	if(mchTurnBasedGame && !(flags & MCH_AI))
		stopFnc();

	if(mchTrackDemoMode)
		stop_movement();

	setPrevPos();
}

void mchMechosRacer::Quant(void)
{
	int i,st = lastStatus;

	mchRacer::Quant();

	if(essence_slime_timer())
		flags |= MCH_ESSENCE_SLIME;
	else
		flags &= ~MCH_ESSENCE_SLIME;

	lastStatus = state();
	if(lastStatus == Mechos::NORMAL_MECHOS){
		if(!(flags & MCH_FINISHED)){
			for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
				stPtr -> last_config[i] = (M3D_KIND(part_by_type(mchPartID[i]) -> Type)) & 0x3F;
			}
			if(!trkEnableFlag && mchBotArcaneUsing && flags & MCH_AI)
				mch_raceD -> AI_ArcaneQuant(this);
		}
	}

	if(st == Mechos::NORMAL_MECHOS && lastStatus != FORMIC_TRANSPORTED_MECHOS && st != lastStatus)
		stPtr -> NDestroys ++;
}

int mchRacer::GetArcaneCount(mchControlPoint* p,int id)
{
	int count = 0;
	mchControlPoint* cp = seedLst.first();

	if(id == -1) id = p -> arcaneData.ID;
	while(cp && cp != p){
		if(cp -> arcaneData.ID == id) count ++;
		cp = cp -> next;
	}
	return count;
}

int mchRacer::GetArcaneCount(int id)
{
	int count = 0;
	mchControlPoint* cp = seedLst.first();
	while(cp){
		if(cp -> arcaneData.ID == id) count ++;
		cp = cp -> next;
	}
	return count;
}

int mchMechosRacer::RemoveLastSeed(void)
{
	mchRacer::RemoveLastSeed();
	return seedLst.size();
}

void mchMechosRacer::removePassedSeed()
{
	int i;
	cMesh* m;

	mchControlPoint* p;

	fxlabRacerEvent(FX_RACER_EVENT_SEED,this);

	p = seedLst.first();

	if(p && p -> arcaneData.ID != -1){
		for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
			m = part_by_type(mchPartID[i]);
			if(m && m -> arcaneData.ID == p -> arcaneData.ID){
				if(m -> arcaneData.Status){
					if(m -> arcaneData.Power >= m -> arcaneData.ActivatePower){
						m -> arcaneData.ownerID = ID;
						m -> arcaneData.fxlabStart();
						stPtr -> NArcanes ++;
					}
				}
				else 
					m -> arcaneData.fxlabStop();
			}
		}
	}

	mchRacer::removePassedSeed();
}

void mchBossRacer::removePassedSeed()
{
	if(check_points_only_controlled())
		flags |= MCH_SEED_PASSED;

	mchRacer::removePassedSeed();
}

int mchMechosRacer::RemoveSelectedSeed(void)
{
	return mchRacer::RemoveSelectedSeed();
}

void mchMechosRacer::RemoveSeeds(void)
{
	mchRacer::RemoveSeeds();
}

mchMechosRacer::~mchMechosRacer(void)
{
}

void mchRaceDispatcher::CenterRacer(mchRacer* p)
{
	mchRacer* r;

	mchTrackElement* el;
	mchCheckpoint* cp;
	mchTrackBranch* br;

	if(!p) p = activeRacer;

	if(mchTurnBasedGame){ 
		if(p -> State() == NormalRacer)
			p -> damage(100000);
		return;
	}

	if(!mchCurrentWorld && mchCurrentTrack == 5){
		r = racerLst -> first();
		while(r){
			if(r -> Type == MCH_RACER_TYPE_BOSS){
				if(p -> State() == NormalRacer)
					p -> damage(100000);
				cp = getCP(p -> stPtr -> Checkpoint);
				move2checkpoint(p,cp,1);
				return;
			}
			r = r -> next;
		}
	}

	if(p -> last_trackP){
		if(p -> last_trackP0 && p -> last_trackP1 && p -> last_trackP0 == p -> last_trackP1){
			el = AI_GetTrackEl(p -> last_trackP0,-1,4,0);
			if(el){ 
				p -> last_trackP = el;
				p -> last_trackP1 = NULL;
			}
		}
		else 
			p -> last_trackP1 = p -> last_trackP0;

		if(p -> State() == NormalRacer)
			p -> damage(100000);

		p -> RemoveSeeds();

		el = p -> last_trackP -> next;
		if(!el){
			cp = p -> last_trackP -> owner -> owner -> next;
			if(!cp) cp = cpLst -> first();

			br = cp -> getTrack();
			if(br) el = br -> pointLst -> first();
		}
		
		p -> AddSeed(*p -> last_trackP);
		if(p -> seedLst.size())
			p -> seedLst.last() -> trackEl = p -> last_trackP;

		if(el){
			p -> AddSeed(*el);
			if(p -> seedLst.size())
				p -> seedLst.last() -> trackEl = el;
		}
	}
	else {
		if(p -> State() != NormalRacer)
			p -> AddSeed(p -> R());
	}
}

void mchRaceDispatcher::AssembleRacer(mchRacer* p)
{
	if(!p) p = activeRacer;

	if(mchTurnBasedGame) return;

	if(p -> State() == DestroyedRacer)
		p -> AddSeed(p -> R());
}

int mchRacer::stopFnc(void)
{
	int ret = stopping;
	if(stopping){
		damage(100000);
		stopping = 0;
	}
	return ret;
}

void mchRaceDispatcher::setCheckpointNStar(mchRacer* r)
{
	int pl = 0;
	mchRacer* p;
	mchCheckpoint* cp;

	if(!r) r = activeRacer;

	if(r -> flags & MCH_FINISHED) 
		return;

	cp = getCP(r -> stPtr -> Checkpoint);

	p = racerLst -> first();
	while(p){
		if(p -> stPtr -> Lap > r -> stPtr -> Lap || (p -> stPtr -> Lap == r -> stPtr -> Lap && p -> stPtr -> Checkpoint > r -> stPtr -> Checkpoint))
			pl ++;
		p = p -> next;
	}

	if(cp){
		cp -> NStar = cp -> ChStarMAX - pl;
		if(cp -> NStar < 0 || !cp -> ID) cp -> NStar = 0;
		cp -> set_state(MCH_CP_CURRENT,r/*,1 Only for change ActivePlayer in HotSeat Mode*/);	}
}

int mchRaceDispatcher::getCheckpointNStar(mchRacer* r)
{
	int pl = 0,cnt;
	mchRacer* p;
	mchCheckpoint* cp;

	if(!r) r = activeRacer;

	cp = getCP(r -> stPtr -> Checkpoint);

	if(r -> flags & MCH_FINISHED || !cp -> ID) 
		return 0;

	p = racerLst -> first();
	while(p){
		if(p -> stPtr -> Lap > r -> stPtr -> Lap || (p -> stPtr -> Lap == r -> stPtr -> Lap && p -> stPtr -> Checkpoint > r -> stPtr -> Checkpoint))
			pl ++;
		p = p -> next;
	}

	cnt = cp -> ChStarMAX - pl;
	if(cnt < 0) cnt = 0;

	return cnt;
}

mchAI_ArcanePrm::mchAI_ArcanePrm(scrDataBlock* b)
{
	int i,idx;
	scrDataBlock* p = NULL;
	for(i = 0; i < AI_ARC_PRM_MAX; i ++) prmData[i] = 0;

	ID = *b -> i_dataPtr;
	list = NULL;

	if(b -> nextLevel)
		p = b -> nextLevel -> first();

	while(p){
		idx = p -> i_dataPtr[0];
		if(idx >= 0 && idx < AI_ARC_PRM_MAX)
			prmData[idx] = p -> i_dataPtr[1];
		p = p -> next;
	}
}

mchTrackElement* mchRaceDispatcher::AI_GetTrackEl(mchTrackElement* fp,int fl,int count,int dir,int rnd)
{
	int i;
	mchTrackElement* el;
	mchCheckpoint* cp;

	if(dir)
		cp = fp -> owner -> owner -> next;
	else 
		cp = fp -> owner -> owner -> prev;

	if(!cp) cp = cpLst -> first();
	el = fp;

	i = 0;
	while(i < count){
		if(dir){
			el = el -> next;
			if(!el){
				if(rnd)
					el = cp -> getTrack() -> pointLst -> first();
				else
					el = cp -> trackPtr -> first() -> pointLst -> first();
				cp = cp -> next;
				if(!cp) cp = cpLst -> first();
			}
		}
		else {
			if(el == el -> owner -> pointLst -> first()){
				el = el -> prev;
				if(rnd)
					el = cp -> getTrack() -> pointLst -> last();
				else
					el = cp -> trackPtr -> first() -> pointLst -> last();
				cp = cp -> prev;
			}
			else {
				el = el -> prev;
			}
		}
		if(fl != -1 && el -> flags & fl){
			return el;
		}
		i ++;
	}
	if(fl != -1)
		return NULL;

	return el;
}

int mchRaceDispatcher::AI_CheckArea(mchRacer* r,int dist)
{
	int x,y,dist2;

	dist2 = dist * dist;
	mchRacer* p = racerLst -> first();
	while(p){
		if(p != r){
			x = p -> R().x - r -> R().x;
			y = p -> R().y - r -> R().y;
			if((x * x + y * y) <= dist2)
				return 1;		
		}
		p = p -> next;
	}
	return 0;
}

void mchInitWorldsPrm(void)
{
	int i,j,idx;

	const char* p;
	XBuffer* xb;

	memset(mch_partFlags,1,ARCANE_MAX);
	memset(mch_partFlags2,1,ARCANE_MAX);
	memset(mch_excludeFlags,0,ARCANE_MAX);

	p = getIniKey(mchIscreenINI,"parts","exclude");
	xb = new XBuffer(p,strlen(p));

	*xb >= idx;
	while(idx != -1){
		mch_partFlags[idx] = 0;
		mch_partFlags2[idx] = 0;
		mch_excludeFlags[idx] = 1;
		*xb >= idx;
	}

	delete xb;

	if(!mch_trackD)
		mch_trackD = new mchTrackDispatcher;

	for(i = 0; i < 20; i ++){
		for(j = 0; j < 10; j ++){
			idx = mch_trackD -> TrackPart(i,j);
			mch_partFlags[idx] = 0;
		}
	}
}

void mchRacer::SetTractionPrm(int fl,float v)
{
	set_traction_by_distance = fl;	// \E7\E0\E2\E8\F1\E8\EC\EE\F1\F2\FC \EE\F2 \F0\E0\F1\F1\F2\EE\FF\ED\E8\FF \EC\E5\E6\E4\F3 \F1\E5\EC\E5\ED\E0\EC\E8
	traction_modulation = v;	// 0..2 - \EC\EE\E4\F3\EB\FF\F6\E8\FF \F2\FF\E3\E8
}

void mchRacer::GetTractionPrm(int& fl,float& v)
{
	fl = set_traction_by_distance;	// \E7\E0\E2\E8\F1\E8\EC\EE\F1\F2\FC \EE\F2 \F0\E0\F1\F1\F2\EE\FF\ED\E8\FF \EC\E5\E6\E4\F3 \F1\E5\EC\E5\ED\E0\EC\E8
	v = traction_modulation;	// 0..2 - \EC\EE\E4\F3\EB\FF\F6\E8\FF \F2\FF\E3\E8
}

void mchRacerStats::ReplacePart(int pt)
{
	int id,tp_id;
	id = mchGetPart(poolID,pt,tp_id);
	if(id != -1){
		last_config[tp_id] = id;
		UpdateMechosCfg();
	}
}

void mchRaceDispatcher::BackupParts(void)
{
/*
	int i = 0,j;
	XBuffer XBuf;
	mchRacerStats* p = mch_racerLst.first();
	while(p){
		XBuf.init();
		XBuf < p -> mechos_config;
		XBuf.set(0);
		for(j = 0; j < 5; j ++){
			XBuf >= mechosCfgBackup[j][i];
		}
		p = p -> next;
		i ++;
	}
*/
}

void mchRaceDispatcher::RestoreParts(void)
{
/*
	int i,j,id;
	mchRacerStats* p = mch_racerLst.search(0);
	if(!p) return;

	XBuffer XBuf(p -> mechos_config,30);

	for(i = 0; i < 5; i ++){
		XBuf >= id;
		for(j = 0; j < mch_racerLst.size(); j ++){
			if(mechosCfgBackup[i][j] != id)
				mchPutPart(0,i,mechosCfgBackup[i][j]);
		}
	}
*/
}

int mchRacer::GetDist(mchRacer* p)
{
	int dist;
	if(last_trackP && p -> last_trackP){
		dist = abs(p -> last_trackP -> index - last_trackP -> index);
		dist += abs(p -> stPtr -> Lap - stPtr -> Lap) * mchLapLength;
		return  dist;
	}
	return -1;
}

void mchRaceDispatcher::AI_PrmQuant(void)
{
	int dist;
	float traction;
	mch_AI_Prm* pr;
	mchRacer* p,*p0;
	
	p0 = NULL;
	p = racerLst -> first();
	while(p){
		if(!(p -> flags & MCH_AI)){
			if(!p0 || p0 -> stPtr -> place > p -> stPtr -> place)
				p0 = p;
		}
		p = p -> next;
	}

	if(!p0){
		p = racerLst -> first();
		while(p){
			if(!p0 || p0 -> stPtr -> place > p -> stPtr -> place)
				p0 = p;

			p = p -> next;
		}
	}

	p = racerLst -> first();
	while(p){
		if(p -> flags & MCH_AI){
			pr = AI_GetPrm(p);
			if(!(p -> flags & MCH_FINISHED)){
				if(p != p0){
					traction = pr -> tractionPrm;

					if(p0 -> flags & MCH_FINISHED)
						dist = -1;
					else
						dist = p0 -> GetDist(p);

					if(dist != -1){
						if(p -> stPtr -> place > p0 -> stPtr -> place){
							if(dist < pr -> tractionDistMin){
								traction = pr -> tractionPrm;
							}
							else {
								if(dist >= pr -> tractionDistMax){
									traction = pr -> tractionPrmMax;
								}
								else {
									traction = pr -> tractionPrm + (pr -> tractionPrmMax - pr -> tractionPrm) * (float)(dist - pr -> tractionDistMin) / (float)(float)(pr -> tractionDistMax - pr -> tractionDistMin);
								}
							}
						}
						else {
							if(dist < pr -> tractionDistMin){
								traction = pr -> tractionPrm;
							}
							else {
								if(dist >= pr -> tractionDistMax){
									traction = pr -> tractionPrmMin;
								}
								else {
									traction = pr -> tractionPrm + (pr -> tractionPrmMin - pr -> tractionPrm) * (float)(dist - pr -> tractionDistMin) / (float)(float)(pr -> tractionDistMax - pr -> tractionDistMin);
								}
							}
						}
					}
				}
				else 
					traction = pr -> tractionPrm;

//				traction += traction * 0.1f * (float(XRnd(20)) - 10.0f) * 0.1f;
				p -> SetTractionPrm(pr -> tractionFlag,traction);
			}
			else
				p -> SetTractionPrm(pr -> tractionFlag,0.5);
		}
		
		p = p -> next;
	}
}

void mchRaceDispatcher::AI_Toggle(mchRacer* p,int v)
{
	if(!p) p = activeRacer;

	if(v != -1){
		if(v) p -> flags ^= MCH_AI;
		else p -> flags &= ~MCH_AI;
	}
	else
		p -> flags ^= MCH_AI;

	if(p -> flags & MCH_AI){
		AI_SetPrm(p);

		if(p -> last_trackP)
			p -> trackP = p -> last_trackP;
		else
			p -> trackP = mch_trackMap -> GetTrackEl(p -> R(),p -> radius(),p -> radius());

		if(p -> trackP)
			p -> trackBr = p -> trackP -> owner;
	}
	else 
		p -> RemoveSeeds();
}

float mchRacer::EnergyValue(void)
{
	return -1.0f;
} 

float mchMechosRacer::EnergyValue(void)
{
	if(energy_max())
		return (energy / energy_max());

	return -1.0f;
} 

float mchMechosRacer::EnergyValueMax(void)
{
	return (energy_max() / 100.0f);
} 

void mchInitTrackDemo(void)
{
	mchTrackDemoMode = 1;
	mchA_SetTimer(0);
	mchSetTimeMode(MCH_TIME_STOPPED);

	camera_dispatcher -> setTrackObserver(0, 60, 300, 0);

	if(mchSplitScreenGame)
		camera_dispatcher2 -> setTrackObserver(0, 60, 300, 0);

	mch_arcScrD -> InitTrackDemoScreen();

	KeyBuf -> clear();

	mch_raceD -> SetBrake(1);

	if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
}

void mchFinitTrackDemo(void)
{
	mchTrackDemoMode = 0;

	if(mchEnableStartCounter){
		mchSetTimeMode(MCH_TIME_STOPPED);
		mchA_SetTimer(4);
		mchA_ShowStartCountEnable();
		camera_dispatcher -> runStartScenario();

		if(mchSplitScreenGame)
			camera_dispatcher2 -> runStartScenario();

		if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
	}
	else {
		mch_raceD -> SetBrake(0);

		mchSetTimeMode(MCH_TIME_RUNNING);
		camera_dispatcher -> setMode(CAM_MODE_SLOPE_ROTATE);
		camera_dispatcher -> setObserver(0); // restore active racer

		if(mchSplitScreenGame){
			camera_dispatcher2 -> setMode(CAM_MODE_SLOPE_ROTATE);
			camera_dispatcher2 -> setObserver(0); // restore active racer
		}

		if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
	}
}

RacerState mchMechosRacer::State()
{
	switch(state()){
		case NORMAL_MECHOS:
		case FORMIC_TRANSPORTED_MECHOS:
			return NormalRacer;
		case STAYING_PERSONAGE:
			return DestroyedRacer;
		case MOVING_PERSONAGE:
		case ASSEMBLING_MECHOS:
			return AssemblingRacer;
		default:
			return NormalRacer;
		}
}

const int mchRacerEnergyTimer = 4000;
void mchMechosRacer::EnergyQuant(int dt)
{
	const int max_tm = mchRacerEnergyTimer;

	float v;
	if(energyTimer == -1){
		lastEnergy = EnergyValue();
		energyTimer = 0;
		return;
	}

	v = EnergyValue();
	if(fabs(v - lastEnergy) >= 0.05){
		flags |= MCH_ENERGY_CHANGE;
		energyTimer = 0;
	}
	lastEnergy = v;

	if(!(flags & MCH_ENERGY_CHANGE))
		energyTimer = 0;
	else {
		energyTimer += dt;
		if(energyTimer >= max_tm)
			flags ^= MCH_ENERGY_CHANGE;
	}
}

float mchMechosRacer::EnergyTimer(void)
{
	return (float)energyTimer/(float)mchRacerEnergyTimer;
}

void mchRaceDispatcher::AI_SeedQuant(mchRacer* p)
{
	mchTrackElement* el0 = NULL,*el1 = NULL;

	if(p -> flags & MCH_GET_SEED) return;

	el0 = p -> last_trackP;
	if(p -> seedLst.size())
		el1 = p -> seedLst.first() -> trackEl;

	if(el0 && el1 && el0 -> index >= el1 -> index - 2 && el0 -> index <= el1 -> index + 2){
		p -> mouseSeed = p -> seedLst.first();
		if(p -> mouseSeed -> trackEl){
			p -> trackP = p -> mouseSeed -> trackEl;
			p -> trackBr = p -> trackP -> owner;
		}
		else {
			p -> trackP = p -> last_trackP;
			if(p -> trackP)
				p -> trackBr = p -> trackP -> owner;
		}
#ifdef _RACE_LOG_
		mch_raceSaveBuf.init();
		mch_raceSaveBuf < "AI Seed " <= mch_clock() < " " <= p -> ID < " " <= p -> mouseSeed -> arcaneData.ID < " " <= p -> mouseSeed -> PosX < " " <= p -> mouseSeed -> PosY < " " <= p -> mouseSeed -> PosZ < "\r\n";
		_RACE_LOG_STR(mch_raceSaveBuf);

		mch_raceSaveBuf.init();
		mch_raceSaveBuf < "XRnd -> " <= XRndVal < "\r\n";
		_RACE_LOG_STR(mch_raceSaveBuf);
#endif
		p -> RemoveSelectedSeed();

//		if(p -> check_points_only_controlled())
//			p -> flags |= MCH_SEED_PASSED;

		p -> numSeeds --;
		p -> stPtr -> NSeeds ++;
	}
}

int mchMechosRacer::isFlying(void)
{
	return isFlyingByArcane();
}

void mchMechosRacer::StartEffect(int eff)
{
	startEffect((Mechos::Effect)eff);
}

void mchMechosRacer::sensorIn(const SensorFieldSource* sensor)
{
	switch(sensor -> ID()){
		case IW_ELECTIONARY_SENSOR:
			if(flags & MCH_ACTIVE_RACER2)
				iworld_D2 -> InitElectionary();
			else
				iworld_D -> InitElectionary();
			break;
		case IW_ASSEMBLY_SENSOR:
			if(flags & MCH_ACTIVE_RACER2){
				if(!iworld_D -> AssemblyMode())
					iworld_D2 -> InitAssembly();
			}
			else {
				if(!mchSplitScreenGame || !iworld_D2 -> AssemblyMode())
					iworld_D -> InitAssembly();
			}
			break;
		case IW_CREDITS_SENSOR:
			mch_iScreenID = ACS_CREDITS_SCR;
			KeyBuf -> put(mchGetKey(MCH_KEY_EXIT),KBD_CUR_KEY_PRESSED);
			break;
		case IW_DOGGY_SENSOR:
			if(!mchSplitScreenGame)
				iworld_D -> InitDoggy();
/*
			if(flags & MCH_ACTIVE_RACER2){
				if(!iworld_D -> AdvisorMode())
					iworld_D2 -> InitDoggy();
			}
			else {
				if(!mchSplitScreenGame || !iworld_D2 -> AdvisorMode())
					iworld_D -> InitDoggy();
			}
*/
			break;
	}
}

void mchMechosRacer::sensorOut(const SensorFieldSource* sensor)
{
/*
	switch(sensor -> ID()){

		case IW_DOGGY_SENSOR:
			if(flags & MCH_ACTIVE_RACER2)
				iworld_D2 -> FinitDoggy();
			else 
				iworld_D -> FinitDoggy();
			break;
	}
*/
}

void mchMechosRacer::ChangePart(int tp,int id)
{
	int i,_id[NUM_ACTIVE_PARTS];
	XBuffer XBuf;

	if(!tp)
		stPtr -> essenceID = id;

	changePart(tp,id + 1);

	XBuf.write(stPtr -> mechos_config,30);
	XBuf.set(0,XB_BEG);

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		XBuf >= _id[i];

	_id[tp] = id + 1;

	XBuf.init();
	for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
		XBuf <= _id[i] < " ";

	XBuf.set(0,XB_BEG);
	XBuf.read(stPtr -> mechos_config,30);

	memcpy(stPtr -> backup_config,stPtr -> mechos_config,30);
}

void mchMechosRacer::AuthorizeMechos(void)
{
	int i;
	cMesh* p;
	for(i = 1; i < NUM_ACTIVE_PARTS; i ++){
		p = part_by_type(mchPartID[i]);
		if(p && p -> arcaneData.ID != -1)
			stPtr -> AuthorizePart(p);
	}
}


void mchRaceDispatcher::put2checkpoint(mchRacer* r,int c)
{
	int x,y,z;

	if(!r)
		r = activeRacer;

	if(c >= cpLst -> size())
		return;

	mchCheckpoint* cp = getCP(c);
	GetChpCenterPos(cp,x,y,z);
	r -> setPosition(Vect3f(x,y,z),r -> psi());
	r -> stop_movement();

	r -> stPtr -> Checkpoint = c;
	initCheckpoints(r);
}

void mchRaceDispatcher::SetBrake(int on)
{
	mchRacer* p = mch_raceD -> racerLst -> first();
	while(p){
		p -> set_brake(on);
		p = p -> next;
	}

}

void mchRaceDispatcher::KeyTrap(void)
{
	int k,id;

//	mchRacerStats* rp;

	KeyBuf -> backup();

	if(mchSplitScreenGame)
		mchSelectKeysConfig(1);
	else
		mchSelectKeysConfig(0);

	if(mchTurnBasedGame && mchTimeMode == MCH_TIME_STOPPED && mchKeyPressed(MCH_KEY_ROTATE_CAM))
		camera_dispatcher -> rotateWhenStopTime();

	while(KeyBuf -> size){
		k = KeyBuf -> get();

		if(mch_trkRec)
			mch_trkRec -> KeyTrap(k);

// TODO: @caiiiycuk invesitgate this
#ifdef WTF
		if(k == VK_SPACE){
			if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_Game){
				if(mchTimeMode == MCH_TIME_WAITING_TURN && !(activeRacer -> flags & MCH_FINISHED) && mchPBEM_CheckFlag(PBEM_DATA_SENT) && og_inP.express_game()){
					mchPBEM_DropFlag(PBEM_ERROR);
					mchPBEM_SetFlag(PBEM_SHOW_GAME | PBEM_DATA_SENT | PBEM_CHECK_RETURN);
					mchPBEM_DataFlag = -1;
					mchPBEM_HiddenTime = 1;

					sgPrepareRefreshData();

					mchA_SetTurnFlag(0,-1);
				}
			}
		}
		if(k == 'C'){
			if(mchPBEM_Game && mchTurn && mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_Game){
				if(mchTimeMode == MCH_TIME_WAITING_TURN){
					mchPBEM_SetFlag(PBEM_CHANGE_PLAYER);
					mchA_BuildOnlinePlaceOrder();
				}
			}
		}
		if(k == VK_UP){
			if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER)){
				mchA_ChangeOnlinePlayer(0);
				firstRacer(mchPBEM_CurPlayer);
			}
		}
		if(k == VK_DOWN){
			if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER)){
				mchA_ChangeOnlinePlayer(1);
				firstRacer(mchPBEM_CurPlayer);
			}
		}
#endif

		id = mchGetKeyID_First(k);

		while(id != -1){
			switch(id){
				case MCH_KEY_REPLAY:
					InitReplay(0);
					break;
				case MCH_KEY_REPLAY_ALL:
					InitReplay(1);
					break;
				case MCH_KEY_REMOVE_ARCANE:
					if(activeRacer -> curSeed && mchGameMode != MCH_ENTIRE_CONTROL_HS || (!mchPBEM_Pause && mchTimeMode == MCH_TIME_STOPPED))
						activeRacer -> curSeed -> RemoveArcane(1);
					break;
				case MCH_KEY_MAP:
					mch_arcScrD -> playerSet1 -> ToggleMap();
					break;
				case MCH_KEY_SET_ARCANE0:
					if(mchGameMode != MCH_ENTIRE_CONTROL_HS || (!mchPBEM_Pause && mchTimeMode == MCH_TIME_STOPPED))
						mch_arcScrD -> playerSet1 -> menu -> KeyHandler(1);
					break;
				case MCH_KEY_SET_ARCANE1:
					if(mchGameMode != MCH_ENTIRE_CONTROL_HS || (!mchPBEM_Pause && mchTimeMode == MCH_TIME_STOPPED))
						mch_arcScrD -> playerSet1 -> menu -> KeyHandler(2);
					break;
				case MCH_KEY_SET_ARCANE2:
					if(mchGameMode != MCH_ENTIRE_CONTROL_HS || (!mchPBEM_Pause && mchTimeMode == MCH_TIME_STOPPED))
						mch_arcScrD -> playerSet1 -> menu -> KeyHandler(3);
					break;
				case MCH_KEY_REMOVE_SEED:
					if(mchGameMode != MCH_ENTIRE_CONTROL_HS || (!mchPBEM_Pause && mchTimeMode == MCH_TIME_STOPPED)){
						if(XKey.Pressed(VK_SHIFT))
							activeRacer -> RemoveSeeds();
						else
							activeRacer -> RemoveLastSeed();
					}
					break;
				case MCH_KEY_START_TIME:
					if(!mchTurnBasedGame) break;
					if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_Game){
// TODO: @caiiiycuk invesitgate this
#ifdef WTF
						if(mchPBEM_GameMode == MCH_PBEM_REPLAY_TURN && (mchTimeMode == MCH_TIME_RUNNING || mchPBEM_Pause)){
							if(!mchPBEM_Pause){
								mchGameFlags |= MCH_STOP_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
							}
							else {
								mchGameFlags |= MCH_START_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
							}
						}
						if(!(activeRacer -> flags & MCH_FINISHED) && !mchPBEM_CheckFlag(PBEM_LATE_TURN) && !mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER) && mchPBEM_CurPlayer == mchPBEM_CurPlayer0){
							if(mchPBEM_GameMode == MCH_PBEM_EDIT_TURN){
								if(mchTimeMode == MCH_TIME_STOPPED && (activeRacer -> seedLst.size() || activeRacer -> flags & MCH_AI))
									mchGameFlags |= MCH_START_TIME_FLAG;

								if(mchTimeMode == MCH_TIME_WAITING_TURN && !online_checkEndGame())
									mchGameFlags |= MCH_START_TIME_FLAG;
							}
						}
						else {
							if(mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER)){
								mchPBEM_DropFlag(PBEM_CHANGE_PLAYER);
								if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE) && mchHS_LoadFlag){
									mchPBEM_SetFlag(PBEM_START_TIME);
									mchGameFlags |= MCH_START_TIME_FLAG;

									if(mchPBEM_Pause)
										mchGameFlags |= MCH_TOGGLE_PAUSE_FLAG;
								}
								ogSetRefreshTime();
							}
						}
#endif						
					}
					else {
						if(mchTimeMode == MCH_TIME_STOPPED && (activeRacer -> seedLst.size() || activeRacer -> flags & MCH_AI))
							mchGameFlags |= MCH_START_TIME_FLAG;

						if(mchTimeMode == MCH_TIME_WAITING_TURN)
							mchGameFlags |= MCH_START_TIME_FLAG;

						if(mchTimeMode == MCH_TIME_RUNNING && mchGameMode == MCH_SINGLE_GAME)
							mchGameFlags |= MCH_STOP_TIME_FLAG;
					}
					break;
				case MCH_KEY_CENTER_VIEW:
					if(mchTimeMode == MCH_TIME_RUNNING){
						if(mchGameMode == MCH_SINGLE_GAME)
							CenterRacer();
					}
					else {
						if(mchTurnBasedGame && mchTimeMode == MCH_TIME_STOPPED)
							camera_dispatcher -> centerMechosWhenStopTime();
					}
					break;
				case MCH_KEY_VIS_MODE:
					if(mchTimeMode == MCH_TIME_RUNNING)
						camera_dispatcher -> setMode(camera_dispatcher -> currentMode() != CAM_MODE_SLOPE_ROTATE ? CAM_MODE_SLOPE_ROTATE : CAM_MODE_FIXED_3D, 1);
					break;
			}

			id = mchGetKeyID_Next();
		}
	}

	KeyBuf -> restore();

	mchSelectKeysConfig(-1);

	if(mchSplitScreenGame)
		mchSelectKeysConfig(2);
	else 
		return;

	while(KeyBuf -> size){
		k = KeyBuf -> get();

		id = mchGetKeyID_First(k);

		while(id != -1){
			switch(id){
				case MCH_KEY_REMOVE_ARCANE:
					if(activeRacer2 -> curSeed){
						activeRacer2 -> curSeed -> RemoveArcane(1);
					}
					break;
				case MCH_KEY_MAP:
					mch_arcScrD -> playerSet2 -> ToggleMap();
					break;
				case MCH_KEY_SET_ARCANE0:
					mch_arcScrD -> playerSet2 -> menu -> KeyHandler(1);
					break;
				case MCH_KEY_SET_ARCANE1:
					mch_arcScrD -> playerSet2 -> menu -> KeyHandler(2);
					break;
				case MCH_KEY_SET_ARCANE2:
					mch_arcScrD -> playerSet2 -> menu -> KeyHandler(3);
					break;
				case MCH_KEY_REMOVE_SEED:
					if(XKey.Pressed(VK_SHIFT))
						activeRacer2 -> RemoveSeeds();
					else
						activeRacer2 -> RemoveLastSeed();
					break;
				case MCH_KEY_START_TIME:
					if(mchTurnBasedGame){
						if(mchTimeMode == MCH_TIME_RUNNING)
							mchGameFlags |= MCH_STOP_TIME_FLAG;
						else 
							mchGameFlags |= MCH_START_TIME_FLAG;
					}
					break;
				case MCH_KEY_CENTER_VIEW:
					if(mchTimeMode == MCH_TIME_RUNNING)
						CenterRacer();
					break;
				case MCH_KEY_VIS_MODE:
					if(mchTimeMode == MCH_TIME_RUNNING)
						camera_dispatcher -> setMode(camera_dispatcher -> currentMode() != CAM_MODE_SLOPE_ROTATE ? CAM_MODE_SLOPE_ROTATE : CAM_MODE_FIXED_3D, 1);
					break;
			}

			id = mchGetKeyID_Next();
		}
	}

	KeyBuf -> restore();

	mchSelectKeysConfig(-1);
}

void mchRacerStats::SetMechos(char* cfg)
{
	XBuffer str;

	strcpy(mechos_config,cfg);

	str.write(cfg,strlen(cfg) + 1);
	str.set(0,XB_BEG);
	str >= essenceID;

	essenceID --;

	GetMechos(1);
}

void mchRaceDispatcher::nextRacerHS(void)
{
	mchRacer* p = activeRacer -> next;

	while(p -> flags & (MCH_AI | MCH_FINISHED)){
		p = p -> next;
		if(!p){
			p = racerLst -> first();
			break;
		}
	}

	setActiveRacer(p,mchAutoCenterCamera);
}

int mchRaceDispatcher::isActiveRacerLast(void)
{
	if((mchPBEM_Game || mchHS_LoadFlag) && mchGameMode == MCH_ENTIRE_CONTROL_HS) return 1;
//	if(mchPBEM_Game) return 1;

	mchRacer* p = activeRacer -> next;
	while(p){
		if(!(p -> flags & MCH_AI) && !(p -> flags & MCH_FINISHED))
			return 0;
		p = p -> next;
	}
	return 1;
}

mchRacerStats* mchRaceDispatcher::get_firstRacer(int id)
{
	mchRacer* p = racerLst -> first();
	if(id == -1){
		while(p -> flags & (MCH_AI | MCH_FINISHED)){
			p = p -> next;
			if(!p){
				p = racerLst -> first();
				break;
			}
		}
	}
	else {
		while(id){
			p = p -> next;
			if(!p){
				p = racerLst -> first();
				break;
			}
			id --;
		}
		if(!mchPBEM_Game){
			while(p -> flags & (MCH_AI | MCH_FINISHED)){
				p = p -> next;
				if(!p){
					p = racerLst -> first();
					break;
				}
			}
		}
	}

	return p -> stPtr;
}

void mchRaceDispatcher::firstRacer(int id)
{
	mchRacer* p = racerLst -> first();
	if(id == -1){
		while(p -> flags & (MCH_AI | MCH_FINISHED)){
			p = p -> next;
			if(!p){
				p = racerLst -> first();
				break;
			}
		}
	}
	else {
		while(id){
			p = p -> next;
			if(!p){
				p = racerLst -> first();
				break;
			}
			id --;
		}
		if(!mchPBEM_Game){
			while(p -> flags & (MCH_AI | MCH_FINISHED)){
				p = p -> next;
				if(!p){
					p = racerLst -> first();
					break;
				}
			}
		}
	}

	setActiveRacer(p,mchAutoCenterCamera);
}

mchTrackElement* mchGetTrackEl(int id)
{
	mchTrackElement* p;
	mchTrackBranch* br;
	mchCheckpoint* cp = mch_raceD -> cpLst -> first();
	while(cp){
		br = cp -> trackPtr -> first();
		while(br){
			p = br -> pointLst -> search(id);
			if(p) return p;
			br = br -> next;
		}
		cp = cp -> next;
	}
	return NULL;
}

void mchRaceDispatcher::LoadAllSeeds(void)
{
	int turn_flag = 0;
	mchRacer* rp;
	sgRecord* p = sgD -> get_first(mchTurn);

	if(mchPBEM_Game){
		rp = racerLst -> first();
		while(rp){
			if(hsPlayers[rp -> stPtr -> ID].type == 1){
				if((rp -> flags & MCH_AI) && !(rp -> flags & MCH_FINISHED)){
					AI_Toggle(rp,0);
					rp -> SetTractionPrm(1,1.0f);
					rp -> RemoveSeeds();
				}

				rp -> flags &= ~MCH_SEEDS_LOADED;
			}

			rp = rp -> next;
		}
	}

	while(p){
		turn_flag = 1;
		switch(p -> Type()){
			case SG_SEEDS_RECORD:
				LoadSeeds(p -> DataSize(),p -> Data());
				break;
		}
		p = sgD -> get_next();
	}

	if(mchPBEM_Game && turn_flag){
		rp = racerLst -> first();
		while(rp){
			if(!(rp -> flags & MCH_SEEDS_LOADED)){
				if(!(rp -> flags & MCH_AI)){
					rp -> RemoveSeeds();
					rp -> stop_movement();
//					AI_Toggle(rp,1);
				}
			}
			rp = rp -> next;
		}
	}
}

void mchRaceDispatcher::SaveAllSeeds(void)
{
	int id = mchPBEM_CurPlayer;

	sgRecord* r;
	mchRacer* p;

	if(mchPBEM_Game){
		p = racerLst -> first();
		while(id){
			p = p -> next;
			if(!p) p = racerLst -> first();
			id --;
		}

		r = sgD -> get_first(mchTurn);
		while(r){
			if(r -> Type() == SG_SEEDS_RECORD){
				mch_raceSaveBuf.init();
				mch_raceSaveBuf.write(r -> Data(),r -> DataSize());
				mch_raceSaveBuf.set(0,XB_BEG);
				mch_raceSaveBuf > id;
				if(id == p -> ID)
					r -> SetTime(-1);
			}
			r = sgD -> get_next();
		}

		sgD -> clean_list();

		SaveSeeds(p);
	}
	else {
		r = sgD -> get_first(mchTurn);
		while(r){
			if(r -> Type() == SG_SEEDS_RECORD)
				r -> SetTime(-1);

			r = sgD -> get_next();
		}

		sgD -> clean_list();
		p = racerLst -> first();
		while(p){
			if(!(p -> flags & MCH_AI))
				SaveSeeds(p);
			p = p -> next;
		}
	}
}

void mchRaceDispatcher::SaveSeeds(mchRacer* p)
{
	mchControlPoint* cp;
	if(!p) p = activeRacer;

	if(!p -> seedLst.size() || p -> flags & MCH_AI) return;

	mch_raceSaveBuf.init();
	mch_raceSaveBuf < p -> ID < p -> seedLst.size();

	cp = p -> seedLst.first();
	while(cp){
		cp -> save(mch_raceSaveBuf);
		cp = cp -> next;
	}

	sgD -> add_record(mchTurn,SG_SEEDS_RECORD,mch_raceSaveBuf.tell(),mch_raceSaveBuf.address());
}

void mchRaceDispatcher::LoadSeeds(int sz,void* p)
{
	int i,id,cp_sz;
	mchControlPoint* cp;
	mchRacer* r;

	XBuffer buf(p,sz);

	buf > id > cp_sz;

	r = racerLst -> search(id);

	if(!r) return;

	if(r -> flags & MCH_AI)
		AI_Toggle(r,0);

	r -> RemoveSeeds();

	r -> numSeeds = hsTurnSeedsMax;

	cp = new mchControlPoint;

	for(i = 0; i < cp_sz; i ++){
		cp -> load(buf);
		r -> AddSeed(cp);
	}

	if(cp_sz)
		r -> flags |= MCH_SEEDS_LOADED;

	delete cp;
}

void mchRaceDispatcher::ResetSeeds(mchRacer* r)
{
	if(!r){ 
		r = racerLst -> first();
		while(r){
			if(r -> seedLst.size())
				r -> take_control_point();
			r = r -> next;
		}
	}
	else {
		if(r -> seedLst.size())
			r -> take_control_point();
	}
}

void mchRaceDispatcher::EndTurn(void)
{
	XBuffer buf;
	buf < mch_clock();

	sgD -> add_record(mchTurn,SG_END_TURN_RECORD,buf.tell(),buf.address());
}

void* mchSaveGameParams(int& sz)
{
	int i;
	mchRacerStats* rs;

	mch_raceSaveBuf.init();
	mch_raceSaveBuf < mchNumRacers < mchPlayerRecorder < mchNumLaps < mchBotArcaneUsing < mchDefaultFigure < XRndVal < mchCurrentWorld < mchCurrentTrack < xgrGameMode < mchTurnBasedGame < mchEnableStartCounter < mchEnableTrackDemo;

	if(!mch_racerLst.size())
		mchInitRacers();

	rs = mch_racerLst.first();
	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		rs -> save(&mch_raceSaveBuf);
		rs = rs -> next;
	}

	camera_dispatcher -> saveControls(mch_raceSaveBuf);

	mch_raceSaveBuf < i;

	sz = mch_raceSaveBuf.tell();
	return mch_raceSaveBuf.address();
}

void mchLoadGameParams(void* p,int sz)
{
	int i;
	mchRacerStats* rs;

	XBuffer buf(p,sz);
	buf > mchNumRacers > mchPlayerRecorder > mchNumLaps > mchBotArcaneUsing > mchDefaultFigure > XRndVal > mchCurrentWorld > mchCurrentTrack > xgrGameMode > mchTurnBasedGame > mchEnableStartCounter > mchEnableTrackDemo;

	rs = mch_racerLst.first();
	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		rs -> load(&buf);
		rs = rs -> next;
	}

	camera_dispatcher -> loadControls(buf);

	mchReInitGraph(xgrGameMode);
}

void mchRestoreGameParams(void)
{
	mchLoadGameParams(mch_raceSaveBuf.address(),mch_raceSaveBuf.tell());
}

int mchRaceDispatcher::getActiveRacerID(void)
{
	int id = 0;
	mchRacer* p = racerLst -> first();
	while(p){
		if(p == activeRacer) 
			break;

		id ++;
		p = p -> next;
	}

	return id;
}

void mchRacer::SetPointControl(void)
{
	controled_only_by_point = 1;
}

int mchRaceDispatcher::online_checkEndGame(void)
{
#ifdef NETWORK
	int num_not_finished_humans;
	mchRacer* p;
	ogPlayerInfo* pl;
/*
	int i,num_finished,num_not_finished_humans,num_bots;
	if(og_inP.full_game()){
		num_finished = num_not_finished_humans = num_bots = 0;

		for(i = 0; i < og_inP.num_players(); i ++){
			pl = og_inP.get_player(i);
			if(pl -> ROBOT == 'Y') num_bots ++;
		}

		p = racerLst -> first();
		while(p){
			pl = og_inP.get_player(p -> stPtr -> ID);

			if(!(p -> flags & MCH_FINISHED) && pl -> ROBOT == 'N') 
				num_not_finished_humans ++;

			p = p -> next;
		}
		if(p -> flags & MCH_FINISHED)
			num_finished ++;

		if(!num_not_finished_humans) 
			return 1;

		if(num_not_finished_humans == 1 && num_finished == og_inP.num_players() - 1)
			return 1;

	}
	else {
		p = racerLst -> first();
		while(p){
			if(p -> flags & MCH_FINISHED) return 1;
			p = p -> next;
		}
	}
*/
	if(og_inP.full_game()){
		num_not_finished_humans = 0;

		p = racerLst -> first();
		while(p){
			pl = og_inP.get_player(p -> stPtr -> ID);

			if(!(p -> flags & MCH_FINISHED) && pl -> ROBOT == 'N') 
				num_not_finished_humans ++;

			p = p -> next;
		}

		if(num_not_finished_humans <= 1)
			return 1;
	}
	else {
		p = racerLst -> first();
		while(p){
			if(p -> flags & MCH_FINISHED) return 1;
			p = p -> next;
		}
	}

	return 0;
#endif
    return 0;
}

void mchRaceDispatcher::InitReplay(int all_flag)
{
	if(mchPBEM_GameMode != MCH_PBEM_REPLAY_TURN && (mchTimeMode == MCH_TIME_WAITING_TURN || mchTimeMode == MCH_TIME_GAME_OVER) && mchGameMode == MCH_ENTIRE_CONTROL_HS && mchTurn){
		if(!mchPBEM_Game)
			SaveAllSeeds();

		mchHS_SaveFlag = 0;
		mchHS_LoadFlag = 1;

		mchPBEM_TargetTurn = mchTurn - 1;
		mchTurn = 0;

		if(!mchPBEM_Game)
			mchPBEM_CurPlayer = getActiveRacerID();

		mchFreeResourcesFlag = 1;

		if(mchTimeMode == MCH_TIME_GAME_OVER)
			mchEndGameFinit(1);
		else
			mchSetNextQuantRTO();

		mchNextWorld = mchCurrentWorld;
		mchNextTrack = mchCurrentTrack;
		mchResetRacers();

		mchPBEM_GameMode = MCH_PBEM_REPLAY_TURN;

		mchPBEM_ViewEntireGame = all_flag;
	}
}

void mchRacer::setPrevPos(void)
{
	prevPos = R();
}

void mchRacer::DumpSeeds(void)
{
#ifdef _RACE_LOG_
	mchControlPoint* p = seedLst.first();

	_RACE_LOG_STR("NumSeeds: ");
	_RACE_LOG_TXT(numSeeds);
	_RACE_LOG_STR("\r\n");

	_RACE_LOG_STR("seedLst.size(): ");
	_RACE_LOG_TXT(seedLst.size());
	_RACE_LOG_STR("\r\n");

	while(p){
		mch_raceSaveBuf.init();
		mch_raceSaveBuf < "Seed -> " <= p -> PosX < " " <= p -> PosX < " " <= p -> PosZ < " " <= p -> arcaneData.ID < "\r\n";
		_RACE_LOG_STR(mch_raceSaveBuf.address());
		p = p -> next;
	}
#endif
}

void mchRaceDispatcher::DumpTrack(void)
{
#ifdef _RACE_LOG_
	mchCheckpoint* cp;
	mchTrackBranch* tr;
	mchTrackElement* el;

	_RACE_LOG_STR("\r\n-TRACK--------------------\r\n");

	cp = cpLst -> first();
	while(cp){
		tr = cp -> trackPtr -> first();
		while(tr){
			el = tr -> pointLst -> first();
			while(el){
				mch_raceSaveBuf.init();
				mch_raceSaveBuf <= el -> ID < " " <= el -> index < " " <= el -> flags < " " <= el -> Cx < " " <= el -> Cy < " " <= el -> Cz < "\r\n";
				_RACE_LOG_STR(mch_raceSaveBuf.address());
				el = el -> next;
			}
			tr = tr -> next;
		}
		cp = cp -> next;
	}

	_RACE_LOG_STR("\r\n-TRACK--------------------\r\n");
#endif
}

int mchRaceDispatcher::checkEndTurn(void)
{
	int ret = 0,num_humans = 0,num_sleeping = 0;
	mchRacer* p = racerLst -> first();

	while(p){
		if(!(p -> flags & MCH_AI) && !(p -> flags & MCH_FINISHED)) num_humans ++;
		p = p -> next;
	}

	if(num_humans > 1){
		p = racerLst -> first();
		while(p){
			if(!(p -> flags & MCH_FINISHED) && !(p -> flags & MCH_AI) && !p -> seedLst.size()){
				if(!(p -> flags & MCH_ALL_SEEDS)){
					if(!(p -> flags & MCH_SLEEPING)){
						p -> flags |= MCH_SLEEPING;
						p -> stop_movement();
					}
					num_sleeping ++;
				}
				else 
					return 1;
			}
			if(!(p -> flags & MCH_FINISHED) && p -> flags & MCH_AI && p -> numSeeds <= -hsTurnSeedsMax)
				return 1;

			p = p -> next;
		}

		if(num_sleeping == num_humans) return 1;
	}
	else {
		p = racerLst -> first();
		while(p){
			if(!(p -> flags & MCH_FINISHED) && p -> flags & MCH_AI && p -> numSeeds <= -hsTurnSeedsMax)
				return 1;
			if(!(p -> flags & MCH_FINISHED) && !(p -> flags & MCH_AI) && !p -> seedLst.size())
				return 1;

			p = p -> next;
		}
	}

	return 0;
}

void mchRaceDispatcher::prepareStartTime(void)
{
	mchRacer* p = racerLst -> first();

	while(p){
		p -> flags &= ~(MCH_ALL_SEEDS | MCH_SLEEPING);

		if(!(p -> flags & MCH_AI)){
			if(p -> seedLst.size() >= p -> numSeeds)
				p -> flags |= MCH_ALL_SEEDS;
		}
		p = p -> next;
	}
}
