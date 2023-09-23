
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "race.h"
#include "TrackDispatcher.h"
#include "I-World.h"

#include "ACI_IDS.H"
#include "ACI_EVNT.H"
#include "ACI_SCR.H"
#include "KEYS.H"

#include "iText.h"

#include "savegame.h"
#include "wininet_api.h"
#include "online_game.h"

#include "demo_dispatcher.h"

#define DBGCHECK

#include "mch_common.h" // For far target

#ifndef _WIN32
#include <ctime>
#endif

/* ----------------------------- STRUCT SECTION ----------------------------- */

struct acsSaveSlot 
{
	int ID;
	int flag;

	char playerName[HS_NAME_LEN + 1];

	long timeVal;
	char timeString[256];

	int numStarlets;
	int figureID;

	int lastWorld;
	int lastTrack;

	void* list;
	acsSaveSlot* prev;
	acsSaveSlot* next;

	void scan(void);
	void save(void);
	void load(void);

	void pack(void);
	void unpack(void);

	char* string(void);

	acsSaveSlot(int id);
	~acsSaveSlot(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern xtList<mchRacerStats> mch_racerLst;
extern int iwElectionaryFlag;
extern int mchXRecorderMode;
extern int mchTurn;

extern int mchPBEM_DataFlag;
extern int mch_ShowImages;
extern int mchSkipMainMenu;

extern int mchHS_SaveFlag;
extern int mchHS_LoadFlag;

extern int mchFreeResourcesFlag;
extern int mchLoadingRTO_NextID;

extern XBuffer mch_raceSaveBuf;

extern int mchNextWorld;
extern int mchNextTrack;

extern int mch_iScreen;

extern int mchRaceLogID;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchA_SetOnlinePlaceOrder(int pl_id,int idx);
int mchA_GetOnlinePlaceOrder(int idx);
void mchA_SetTurnFlag(int fl = 1,int id = -1);

void mchInitRacers(void);

int acsInitSaveSlots(void);
void acsInitSaveOptions(void);

void acsInitSaveScreen(void);
void acsBrowseSaveSlots(void);
void acsSortSaveSlots(void);

void acsLoadGame(void);
void acsSaveGame(void);

int acsGetSaveSlotFigure(int n);
int acsGetSaveSlotIndex(int n);

char* acsGetSaveString(int n);

int acsCheckSaveName(char* p);
int acsGetFreeSlotID(void);

void acsScrollSaveSlots(int dir);

void mchResetRacers(void);
void mchSetNextQuantRTO(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

int acsFirstSlot = 0;

int acsCurrentSave = -1;
int acsNumSaves = 0;
int acsLoadFlag = 0;

xtList<acsSaveSlot> acsSlotLst;

const int acsSaveVersion = 107;
const int sgVersion = 100;

sgDispatcher* sgD = NULL;

void acsSaveSlot::scan(void)
{
	int sz,ver;

	XBuffer fname;
	XStream fh(0);

	fname < "save";
	if(ID < 10) fname < "0";
	fname <= ID < ".dat";

	if(!fh.open(fname,XS_IN)){
		flag = 0;
		return;
	}

	flag = 1;

	fh > ver;
	if(ver < 101){
		flag = 0;
		fh.close();
		return;
	}

	fh.read(playerName,HS_NAME_LEN + 1);

	fh > sz > timeVal;
	fh.read(timeString,sz);

	fh > numStarlets > figureID > lastWorld > lastTrack;

	fh.close();
}

void acsSaveSlot::save(void)
{
	XBuffer fname;
	XStream fh;

	char tm[20],dt[20];

	mchRacerStats* p = mch_racerLst.search(0);
	if(!p) return;

#ifdef _WIN32
	_strtime(tm);
	_strdate(dt);
#endif
	time_t curTime = timeVal;
	time(&curTime);

#ifndef _WIN32
	const auto localTime = std::localtime(&curTime);
	std::strftime(tm, sizeof(tm), "%T", localTime);
	std::strftime(dt, sizeof(dt), "%D", localTime);
#endif

	sprintf(timeString,"%s, %s",tm,dt);

	flag = 1;

	fname < "save";
	if(ID < 10) fname < "0";
	fname <= ID < ".dat";

	fh.open(fname,XS_OUT);

	strcpy(playerName,p -> name);

	numStarlets = p -> NStar;
	lastWorld = mchCurrentWorld;
	lastTrack = mchCurrentTrack;
	figureID = p -> essenceID;

	fh < acsSaveVersion;
	fh.write(playerName,HS_NAME_LEN + 1);

	fh < (int) strlen(timeString) + 1 < timeVal;
	fh.write(timeString,strlen(timeString) + 1);

	fh < numStarlets < figureID < lastWorld < lastTrack;

	p = mch_racerLst.first();
	while(p){
		p -> save(&fh);
		p = p -> next;
	}

	mch_trackD -> Save(fh);
	iworld_D -> Save(fh);

	fh.close();

	pack();
}

void acsSaveSlot::pack(void)
{
	int ver,sz,buf_sz,buf_sz1;
	char* buf,*buf1;

	XStream fh;
	XBuffer fname;

	fname < "save";
	if(ID < 10) fname < "0";
	fname <= ID < ".dat";

	fh.open(fname,XS_IN);

	buf_sz = fh.size();
	buf = new char[buf_sz];
	fh.read(buf,buf_sz);

	fh.seek(0,XS_BEG);

	fh > ver;
	fh.read(playerName,HS_NAME_LEN + 1);

	fh > sz > timeVal;
	fh.read(timeString,sz);

	fh > numStarlets > figureID > lastWorld > lastTrack;

	fh.close();

	buf1 = new char[buf_sz + sizeof(int)];
	buf_sz1 = ZIP_compress(buf1,buf_sz + sizeof(int),buf,buf_sz);

	fh.open(fname,XS_OUT);

	fh < acsSaveVersion;
	fh.write(playerName,HS_NAME_LEN + 1);

	fh < (int) strlen(timeString) + 1 < timeVal;
	fh.write(timeString,strlen(timeString) + 1);

	fh < numStarlets < figureID < lastWorld < lastTrack;
	fh.write(buf1,buf_sz1);

	fh.close();

	delete[] buf;
	delete[] buf1;
}

void acsSaveSlot::unpack(void)
{
	int ver,sz,buf_sz,buf_sz1;
	char* buf,*buf1;

	XStream fh;
	XBuffer fname;

	fname < "save";
	if(ID < 10) fname < "0";
	fname <= ID < ".dat";

	fh.open(fname,XS_IN);

	fh > ver;
	fh.read(playerName,HS_NAME_LEN + 1);

	fh > sz > timeVal;
	fh.read(timeString,sz);

	fh > numStarlets > figureID > lastWorld > lastTrack;

	buf_sz = fh.size() - fh.tell();
	buf = new char[buf_sz];
	fh.read(buf,buf_sz);

	fh.close();

	buf_sz1 = ZIP_GetExpandedSize(buf);
	buf1 = new char[buf_sz1];

	ZIP_expand(buf1,buf_sz1,buf,buf_sz);

	fh.open(fname,XS_OUT);
	fh.write(buf1,buf_sz1);
	fh.close();

	delete[] buf;
	delete[] buf1;
}

void acsSaveSlot::load(void)
{
	int ver,sz;
	char* buf;
	XStream fh;
	XBuffer fname;

	if(!mch_racerLst.size())
		mchInitRacers();

	mchRacerStats* p = mch_racerLst.search(0);
	if(!p) return;

	scan();
	if(!flag) return;

	fname < "save";
	if(ID < 10) fname < "0";
	fname <= ID < ".dat";

	fh.open(fname,XS_IN);

	fh > ver;

	if(ver >= 107){
		fh.close();
		unpack();
		fh.open(fname,XS_IN);
	}
	else
		fh.seek(0,XS_BEG);

	sz = fh.size();
	buf = new char[sz];
	fh.read(buf,sz);
	fh.close();

	XBuffer xb(buf,sz);

	xb > ver;
	xb.read(playerName,HS_NAME_LEN + 1);

	xb > sz > timeVal;
	xb.read(timeString,sz);

	xb > numStarlets > figureID > lastWorld > lastTrack;

	p = mch_racerLst.first();
	while(p){
		p -> load(&xb);
		p = p -> next;
	}

	mch_trackD -> Load(xb,ver);
	iworld_D -> Load(xb,ver);

	delete[] buf;

	pack();
}

int acsInitSaveSlots(void)
{
	char* fname;
	acsSaveSlot* p;

	fname = XFindFirst("save*.dat");

	acsNumSaves = 0;

	if(acsSlotLst.size()){
		p = acsSlotLst.first();
		while(p){
			acsSlotLst.remove(p);
			delete p;
			p = acsSlotLst.first();
		}
	}

	while(fname){
		p = new acsSaveSlot(atoi(fname + 4));
		p -> scan();
		if(p -> flag) acsNumSaves ++;

		acsSlotLst.append(p);
		fname = XFindNext();
	}

	acsSortSaveSlots();

	if(acsNumSaves && acsCurrentSave == -1)
		acsCurrentSave = acsSlotLst.first() -> ID;

	if(!acsNumSaves)
		acsCurrentSave = -1;

	acsInitSaveOptions();

	return acsNumSaves;
}

void acsLoadGame(void)
{
	acsSaveSlot* p = acsSlotLst.search(acsCurrentSave);

	if(p && p -> flag)
		p -> load();

	acsLoadFlag = 0;
}

void acsSaveGame(void)
{
	acsSaveSlot* p;

	if(!mch_iScreen || mchSplitScreenGame || mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS || mchXRecorderMode || mch_demoD.is_enabled()) return;

	if(iwElectionaryFlag) 
		return;

	mchRacerStats* rp = mch_racerLst.search(0);
	if(!rp) return;

	acsCurrentSave = acsCheckSaveName(rp -> name) - 1;

	if(acsCurrentSave == -1){
		p = new acsSaveSlot(acsGetFreeSlotID());
		acsCurrentSave = p -> ID;
		acsSlotLst.append(p);
	}
	else
		p = acsSlotLst.search(acsCurrentSave);

	p -> save();
}

void acsInitSaveOptions(void)
{
	XBuffer XBuf;

	aciScreen* scr;
	acsSaveSlot* sl;
	aciScreenInputField* p;

	sl = acsSlotLst.search(acsCurrentSave);

	if(sl && sl -> flag){
		XBuf < iGetText(iTXT_CONTINUE_WITH) < sl -> playerName;

		p = NULL;
		scr = acsScrD -> GetScreen(ACS_MAIN_MENU_SCR1);
		if(scr)
			p = (aciScreenInputField*)scr -> GetObject(ACS_CONTINUE_OPTION);
		if(p)
			strcpy(p -> string,XBuf.address());

		p = NULL;
		scr = acsScrD -> GetScreen(ACS_MAIN_MENU_SCR2);
		if(scr)
			p = (aciScreenInputField*)scr -> GetObject(ACS_CONTINUE_OPTION);
		if(p)
			strcpy(p -> string,XBuf.address());
	}
	scr = acsScrD -> GetScreen(ACS_LOAD_GAME_SCR);
	if(!scr) return;

	if(!acsNumSaves)
		scr -> prevID = ACS_MAIN_MENU_SCR0;
	if(acsNumSaves == 1)
		scr -> prevID = ACS_MAIN_MENU_SCR1;
	if(acsNumSaves > 1)
		scr -> prevID = ACS_MAIN_MENU_SCR2;
}

int acsGetSaveSlotFigure(int n)
{
	return -1;
}

int acsGetSaveSlotIndex(int n)
{
	return -1;
}

char* acsGetSaveString(int n)
{
	int i;
	acsSaveSlot* p = acsSlotLst.first();

	for(i = 0; i < n; i ++)
		if(p) p = p -> next;

	if(p) return p -> string();

	return NULL;
}

int acsCheckSaveName(char* name)
{
	acsSaveSlot* p = acsSlotLst.first();

	while(p){
		if(p -> flag && !stricmp(p -> playerName,name)) 
			return p -> ID + 1;
		p = p -> next;
	}
	return 0;
}

acsSaveSlot::acsSaveSlot(int id)
{
	ID = id;
	flag = 0;
	list = NULL;
}

acsSaveSlot::~acsSaveSlot(void)
{
}

char* acsSaveSlot::string(void)
{
	static XBuffer str;

	str.init();
	str < (char)4 < "x" <= numStarlets < "  " < timeString;
	return str.address();
}

void acsInitSaveScreen(void)
{
	acsInitSaveSlots();

	if(acsFirstSlot >= acsSlotLst.size())
		acsFirstSlot = 0;

	acsBrowseSaveSlots();
}

void acsBrowseSaveSlots(void)
{
	int i;

	acsSaveSlot* sl;
	aciScreen* scr;
	aciScreenInputField* p,*p1;

	scr = acsScrD -> GetScreen(ACS_LOAD_GAME_SCR);
	if(!scr) return;

	sl = acsSlotLst.first();
	for(i = 0; i < acsFirstSlot; i ++)
		if(sl) sl = sl -> next;

	for(i = 0; i < 6; i ++){
		p = (aciScreenInputField*)scr -> GetObject(i + 1);
		p1 = (aciScreenInputField*)scr -> GetObject(i + 10 + 1);
		if(p && p1){
			if(sl){
				strcpy(p -> string,sl -> playerName);
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
				p1 -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
				sl = sl -> next;
			}
			else {
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
				p1 -> flags |= ACS_HIDDEN | ACS_BLOCKED;
			}
		}
	}

	p = (aciScreenInputField*)scr -> GetObject(100);
	if(p){
		if(acsFirstSlot)
			p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		else
			p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
	}

	p = (aciScreenInputField*)scr -> GetObject(101);
	if(p){
		if(sl)
			p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		else
			p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
	}
}

void acsSelectSave(int n)
{
	int i;
	acsSaveSlot* p = acsSlotLst.first();

	n += acsFirstSlot;

	for(i = 0; i < n; i ++)
		if(p) p = p -> next;

	if(p)
		acsCurrentSave = p -> ID;

	acsInitSaveOptions();
}

void acsDeleteSave(int n)
{
	int i;
	XBuffer fname;
	acsSaveSlot* p = acsSlotLst.first();

	n += acsFirstSlot;

	for(i = 0; i < n; i ++)
		if(p) p = p -> next;

	if(p){
		if(acsCurrentSave == p -> ID){
			acsCurrentSave = p -> prev -> ID;
		}
		fname < "save";
		if(p -> ID < 10) fname < "0";
		fname <= p -> ID < ".dat";

		xtDeleteFile(fname);

		acsSlotLst.remove(p);
		delete p;

		if(!acsSlotLst.size())
			acsCurrentSave = -1;
	}

	acsInitSaveSlots();
	acsBrowseSaveSlots();
}

int acsGetFreeSlotID(void)
{
	int i;
	for(i = 0; i < acsSlotLst.size() + 1; i ++){
		if(!acsSlotLst.search(i))
			return i;
	}
	return 0;
}

void acsScrollSaveSlots(int dir)
{
	if(dir){
		if(acsFirstSlot < acsSlotLst.size() - 6){
			acsFirstSlot ++;
			acsBrowseSaveSlots();
		}
	}
	else {
		if(acsFirstSlot){
			acsFirstSlot --;
			acsBrowseSaveSlots();
		}
	}
}

void acsSortSaveSlots(void)
{
	int flag = 1,cnt = 0;
	acsSaveSlot* p,*p1;

	p = acsSlotLst.first();
	while(p){
		p1 = p -> next;
		if(!p -> flag){
			acsSlotLst.remove(p);
			delete p;
		}
		p = p1;
	}

	while(flag){
		flag = 0;

		if(++cnt > 10000) ErrH.Abort("!!!");

		p = acsSlotLst.first();
		while(p){
			p1 = p -> next;
			if(p1 && p1 -> timeVal > p -> timeVal){
				acsSlotLst.remove(p);
				acsSlotLst.append(p);
				flag = 1;
			}
			p = p1;
		}
	}
}

sgRecord::sgRecord(void)
{
	list = NULL;
	type = SG_VOID_RECORD;

	time = 0;
	gameID = 0;

	data_size = 0;
	data = NULL;
}

sgRecord::sgRecord(int tm,int tp,int game,int sz)
{
	list = NULL;
	data_size = 0;
	data = NULL;

	time = tm;
	type = tp;
	gameID = game;
	if(sz) Alloc(sz);
}

sgRecord::~sgRecord(void)
{
	if(data) 
		delete data;
}

sgDispatcher::sgDispatcher(void)
{
	gameID = 0;
	save_flag_ = 0;
}

sgDispatcher::~sgDispatcher(void)
{
	clear();
}

void sgDispatcher::clear(void)
{
	sgRecord* p;

	p = recLst.first();
	while(p){
		recLst.remove(p);
		delete p;
		p = recLst.first();
	}
}

int sgCurTime = 0;
sgRecord* sgCurRec = NULL;

sgRecord* sgDispatcher::get_first(int time)
{
	sgRecord* p = recLst.first();
	sgCurTime = time;

	while(p){
		if(p -> Time() == time)
			break;
		p = p -> next;
	}
	sgCurRec = p;

	return p;
}

sgRecord* sgDispatcher::get_next(void)
{
	sgRecord* p = sgCurRec;
	int time = sgCurTime;

	if(p) p = p -> next;

	while(p){
		if(p -> Time() == time)
			break;
		p = p -> next;
	}
	sgCurRec = p;

	return p;
}

void sgDispatcher::load(char* fname,int clear_flag)
{
	int i,ver,tm,tp,game,sz,rec_sz;
	sgRecord* rec;

	XStream fh(0);

	if(!fh.open(fname,XS_IN)) return;

	if(clear_flag) clear();

	fh > ver > sz;

	for(i = 0; i < sz; i ++){
		fh > tm > tp > game > rec_sz;
		rec = new sgRecord(tm,tp,game,rec_sz);

		if(rec_sz)
			fh.read(rec -> Data(),rec -> DataSize());

		recLst.append(rec);
	}

	fh.close();
}

void sgDispatcher::save(char* fname,int time)
{
	sgRecord* rec;

	XStream fh(fname,XS_OUT);
	fh < sgVersion < recLst.size();

	rec = recLst.first();
	while(rec){
		if(time == -1 || rec -> Time() == time){
			fh < rec -> Time() < rec -> Type() < rec -> GameID() < rec -> DataSize();
			if(rec -> DataSize())
				fh.write(rec -> Data(),rec -> DataSize());
		}
		rec = rec -> next;
	}

	fh.close();
}

void sgDispatcher::add_record(int time,int type,int data_sz,void* data)
{
	sgRecord* p = new sgRecord(time,type,gameID,data_sz);

	if(data_sz) memcpy(p -> Data(),data,data_sz);

	recLst.append(p);
}

void sgDispatcher::clean_list(void)
{
	sgRecord* p,*p1;

	p = recLst.first();
	while(p){
		p1 = p -> next;
		if(p -> Time() == -1){
			recLst.remove(p);
			delete p;
		}
		p = p1;
	}
}

int sgDispatcher::time_max(void)
{
	sgRecord* p = recLst.last();
	if(p) return p -> Time();

	return -1;
}

int sgDispatcher::turn_max(void)
{
	int i,tm,num_pl,num_rc;

	sgRecord* p = recLst.last();

	if(!p) return -1;

	tm = p -> Time();

	if(p -> Type() == SG_CONTROL_RECORD)
		return tm;

	num_pl = num_rc = 0;

	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		if(hsPlayers[i].type == 1)
			num_pl ++;
	}

	p = get_first(tm);
	while(p){
		if(p -> Type() == SG_SEEDS_RECORD)
			num_rc ++;
		p = get_next();
	}

	if(num_rc && num_rc == num_pl) 
		return tm;

	return tm - 1;
/*
	sgRecord* p = recLst.last();
	if(p){
		if(p -> Type() == SG_END_TURN_RECORD)
			return p -> Time();
		else
			return p -> Time() - 1;
	}

	return -1;
*/
}

void sgGetOnlineData(void)
{
	int i,j,seed,id;
	ogTurnInfo* turn;
	ogPlayerTurnInfo* pt;

	DBGCHECK;

	sgD -> clear();

	for(j = 0; j < og_inP.num_players(); j ++)
		mchA_SetOnlinePlaceOrder(j,j);

	for(i = 0; i < og_inP.turn_max(); i ++){
		turn = og_inP.get_turn_by_num(i + 1);

		if(turn){
			for(j = 0; j < turn -> USERS_CNT; j ++){
				pt = turn -> turns[j];

				if(pt -> USER_SEEDS_CNT){
					mch_raceSaveBuf.init();

					id = og_inP.get_player_id(pt -> UID) + 1;
					mch_raceSaveBuf < id < pt -> USER_SEEDS_CNT;
					for(seed = 0; seed < pt -> USER_SEEDS_CNT; seed ++)
						mch_raceSaveBuf < pt -> seeds[seed].X < pt -> seeds[seed].Y < pt -> seeds[seed].Z < pt -> seeds[seed].ArcaneID;

					sgD -> add_record(i,SG_SEEDS_RECORD,mch_raceSaveBuf.tell(),mch_raceSaveBuf.address());
				}
			}

			mch_raceSaveBuf.init();
			mch_raceSaveBuf < turn -> USERS_CNT;
			for(j = 0; j < turn -> USERS_CNT; j ++){
				pt = turn -> turns[j];
				mch_raceSaveBuf < pt -> UID < pt -> RANK < pt -> MOVE_STEPS < pt -> MOVE_TIME < pt -> BOTTLES_CNT < pt -> TOTAL_SEEDS_CNT < pt -> ARCANES_CNT <	pt -> DESTROYS_CNT;
			}

			sgD -> add_record(i,SG_CONTROL_RECORD,mch_raceSaveBuf.tell(),mch_raceSaveBuf.address());

			if(turn -> USERS_CNT == og_inP.num_players()){
				for(j = 0; j < turn -> USERS_CNT; j ++){
					pt = turn -> turns[j];
					mchA_SetOnlinePlaceOrder(j,pt -> RANK - 1);
				}
			}
		}
		else {
			mch_raceSaveBuf.init();
			mch_raceSaveBuf < (int)0;
			sgD -> add_record(i,SG_CONTROL_RECORD,mch_raceSaveBuf.tell(),mch_raceSaveBuf.address());
		}
	}

	if(mchPBEM_CurPlayer == -1)
		mchPBEM_CurPlayer = mchA_GetOnlinePlaceOrder(0);

	DBGCHECK;
}

int sgOnlineControlCheck(void)
{
#ifndef _OFFLINE_DEBUG_
	int i,sz,id,uid,place,turns,time,bottles,seeds,arcanes,destroys,failed = 0;

	mchRacerStats* rp;
	sgRecord* p = sgD -> get_first(mchTurn - 1);

	mch_raceSaveBuf.init();

	while(p){
		if(p -> Type() == SG_CONTROL_RECORD){
			mch_raceSaveBuf.write(p -> Data(),p -> DataSize());
			mch_raceSaveBuf.set(0);

			mch_raceSaveBuf > sz;

			for(i = 0; i < sz; i ++){
				mch_raceSaveBuf > uid > place > turns > time > bottles > seeds > arcanes > destroys;

				if(place + turns + time + bottles + seeds + arcanes + destroys){
					id = og_inP.get_player_id(uid);
					rp = mch_racerLst.search(id);

					if(rp){
						if(rp -> place != place - 1){
							ogLogError("TURN = %d UID = %d [%d %d] wrong RANK",uid,mchTurn,rp -> place + 1,place);
							failed = 1;
						}
						if(rp -> Turn != turns){
							ogLogError("TURN = %d UID = %d [%d %d] wrong MOVE_STEPS",uid,mchTurn,rp -> Turn,turns);
							failed = 1;
						}
						if(rp -> time / 1000 != time){
							ogLogError("TURN = %d UID = %d [%d %d] wrong MOVE_TIME",uid,mchTurn,rp -> time/1000,time);
							failed = 1;
						}

						if(rp -> NSeeds != seeds){
							ogLogError("TURN = %d UID = %d [%d %d] wrong MOVE_TIME",uid,mchTurn,rp -> NSeeds,seeds);
							failed = 1;
						}

						if(rp -> NArcanes != arcanes){
							ogLogError("TURN = %d UID = %d [%d %d] wrong MOVE_TIME",uid,mchTurn,rp -> NArcanes,arcanes);
							failed = 1;
						}

						if(rp -> NDestroys != destroys){
							ogLogError("TURN = %d UID = %d [%d %d] wrong MOVE_TIME",uid,mchTurn,rp -> NDestroys,destroys);
							failed = 1;
						}
					}
				}
			}
		}
		p = sgD -> get_next();
	}

	if(failed) return 0;
#endif
	return 1;
}

void sgPrepareSeedsData(void)
{
	int i;
	ogTurnInfo* p; 
	mchControlPoint* cp;
	mchRacer* r = mch_raceD -> activeRacer;

	og_outP.free_turns();
	og_inP.copy_header(og_outP);
	og_outP.set_type(OG_SEEDS_PACKET);

	og_outP.alloc_turns(1);

	p = og_outP.get_turn_by_idx(0);

	p -> STEP_ID = mchTurn + 1;
	p -> alloc_turns(1);
	p -> turns[0] -> alloc_seeds(r -> seedLst.size());

	p -> turns[0] -> UID = mchPBEM_PlayerUID;
	p -> turns[0] -> RANK = r -> stPtr -> place;
	p -> turns[0] -> MOVE_STEPS = r -> stPtr -> Turn;
	p -> turns[0] -> MOVE_TIME = r -> stPtr -> time / 1000;
	p -> turns[0] -> IS_FINISHED = (r -> flags & MCH_FINISHED) ? 'Y' : 'N';
	p -> turns[0] -> BOTTLES_CNT = r -> NStar;
	p -> turns[0] -> TOTAL_SEEDS_CNT = r -> stPtr -> NSeeds;
	p -> turns[0] -> ARCANES_CNT = r -> stPtr -> NArcanes;
	p -> turns[0] -> DESTROYS_CNT = r -> stPtr -> NDestroys;

	i = 0;
	cp = r -> seedLst.first();
	while(cp){
		p -> turns[0] -> init_seed(i,cp -> PosX,cp -> PosY,cp -> PosZ,cp -> arcaneData.ID);
		cp = cp -> next;
		i ++;
	}

	ogBuffer outbuf(32000);
	og_outP.encode(outbuf);
	wi_D.alloc_outbuf(outbuf.tell() - 1);
	memcpy(wi_D.output_buffer(),outbuf.address(),outbuf.tell() - 1);
}

void sgPrepareRefreshData(void)
{
	og_outP.free_turns();
	og_inP.copy_header(og_outP);
	og_outP.set_type(OG_REFRESH_PACKET);

	ogBuffer outbuf(32000);
	og_outP.encode(outbuf);
	wi_D.alloc_outbuf(outbuf.tell() - 1);
	memcpy(wi_D.output_buffer(),outbuf.address(),outbuf.tell() - 1);
}

void sgPrepareControlData(void)
{
	int i;
	ogTurnInfo* p; 

	mchRacer* r = mch_raceD -> racerLst -> first();

	_RACE_LOG_STR("sgPrepareControlData()\r\n");

	og_outP.free_turns();
	og_inP.copy_header(og_outP);
	og_outP.set_type(OG_CONTROL_PACKET);

	if(mchTurn){
		og_outP.alloc_turns(1);

		p = og_outP.get_turn_by_idx(0);

		p -> STEP_ID = mchTurn;
		p -> alloc_turns(mch_raceD -> racerLst -> size());

		i = 0;
		while(r){
			p -> turns[i] -> UID = og_outP.get_player_uid(i);
			p -> turns[i] -> RANK = r -> stPtr -> place;
			p -> turns[i] -> MOVE_STEPS = r -> stPtr -> Turn;
			p -> turns[i] -> MOVE_TIME = r -> stPtr -> time / 1000;
			p -> turns[i] -> IS_FINISHED = (r -> flags & MCH_FINISHED) ? 'Y' : 'N';
			p -> turns[i] -> BOTTLES_CNT = r -> NStar;
			p -> turns[i] -> TOTAL_SEEDS_CNT = r -> stPtr -> NSeeds;
			p -> turns[i] -> ARCANES_CNT = r -> stPtr -> NArcanes;
			p -> turns[i] -> DESTROYS_CNT = r -> stPtr -> NDestroys;
			r = r -> next;
			i ++;
		}
	}

	ogBuffer outbuf(32000);
	og_outP.encode(outbuf);
	wi_D.alloc_outbuf(outbuf.tell() - 1);
	memcpy(wi_D.output_buffer(),outbuf.address(),outbuf.tell() - 1);
}

void sgProcessRefreshData(void)
{
	int i,uid;
	ogTurnInfo* turn;

	if(og_inP2.get_type() == OG_REFRESH_ANSWER_PACKET){
		turn = og_inP2.get_turn_by_idx(0);
		if(!turn) return;

		mchA_SetTurnFlag(0);

		for(i = 0; i < og_inP2.num_players(); i ++){
			uid = og_inP.get_player_uid(i);

			if(turn -> get_player_turn(uid))
				mchA_SetTurnFlag(1,i);
		}
	}
	if(og_inP2.get_type() == OG_GAME_PACKET){
		og_inP.decode(wi_D.input_buffer(),strlen(wi_D.input_buffer()));
		og_inP.dump();
		sgD -> clear();
		sgGetOnlineData();

		mchHS_SaveFlag = 0;
		mchHS_LoadFlag = 1;

		mchPBEM_TargetTurn = sgD -> turn_max();
//		mchTurn = 0;

		mchPBEM_SetFlag(PBEM_START_TIME);
		mchPBEM_DropFlag(PBEM_DATA_SENT);
		mchPBEM_DropFlag(PBEM_CONTROL_DATA_SENT);
		mchPBEM_DropFlag(PBEM_LATE_TURN);
		mchGameFlags |= MCH_START_TIME_FLAG;

		mch_raceD -> LoadAllSeeds();
//		mchTurn ++;

/*		mchFreeResourcesFlag = 1;
		mchSetNextQuantRTO();
		mchNextWorld = mchCurrentWorld;
		mchNextTrack = mchCurrentTrack;
		mchResetRacers();*/

		mchPBEM_GameMode = MCH_PBEM_REPLAY_TURN;
	}
}

int sgCheckPacket(char* fname)
{
	int sz,day,month;
	char* buf;
	ogPacket p;

	time_t ltime;
	struct tm* tp;

	XStream fh(fname,XS_IN);
	sz = fh.size() + 1;
	buf = new char[sz];
	fh.read(buf,sz - 1);
	fh.close();

	buf[sz - 1] = 0;

	if(p.decode(buf,sz)){
		if(p.get_type() == OG_ARCADE_GAME_PACKET){
			mchGameMode = MCH_SINGLE_GAME;
			mchHS_LoadFlag = 0;
			mch_iScreen = 1;
			mch_ShowImages = 1;
			mchPBEM_Game = 0;
			mchPBEM_DataFlag = 0;
			mchSkipMainMenu = 0;

			time(&ltime);
			ltime += 3600 * 3;
			tp = gmtime(&ltime);
			
			day = tp -> tm_mday ^ 5673;
			month = (tp -> tm_mon + 1) ^ 9345;

			if(day != p.world() || month != p.track()) return 0;

			return 1;
		}
	}

	return 0;
}

void sgGetWorldID(char* fname)
{
	int sz;
	char* buf;
	ogPacket p;

	XStream fh(fname,XS_IN);
	sz = fh.size() + 1;
	buf = new char[sz];
	fh.read(buf,sz - 1);
	fh.close();

	buf[sz - 1] = 0;

	if(p.decode(buf,sz)){
		mchCurrentWorld = hsWorld = p.world();
		mchCurrentTrack = hsTrack = p.track();

		mchRaceLogID = p.owner_uid();

		mchLoadingRTO_NextID = 1;
	}
}
