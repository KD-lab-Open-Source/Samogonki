
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "race.h"
#include "TrackDispatcher.h"

#include "aci_parser.h"
#include "mch_script.h"

#include "terra.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int MCH_CP_STAR_NUM;
extern int mchNumLaps;

extern char* mch_hotseatINI;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

//#define _DEMO_WORLDS_

mchTrackDispatcher* mch_trackD = NULL;

mchTrackInfo::mchTrackInfo(void)
{
	worldID = trackID = 0;
	level = status = access = 0;

#ifndef _DEMO_WORLDS_
	is_enabled = 1;
#else
	is_enabled = 0;
#endif
	text = NULL;

	cd_trackLen = 0;
	memset(cd_trackSeq,0,MCH_TRACK_CD_SEQ_MAX);

	length = 5;
	partID = 0;

	name = NULL;
	boss = NULL;
	bossID = 0;
	bossFigureID = 0;
	bossSoundID = -1;

	openMovie = closeMovie = NULL;

	AI_NormLevel = 0;
	AI_MaxLevel = 1;

	list = NULL;
}

mchTrackInfo::~mchTrackInfo(void)
{
	if(name) 
		free(name);
	if(text)
		free(text);

	if(openMovie)
		free(openMovie);
	if(closeMovie)
		free(closeMovie);
}

void mchTrackInfo::Init(scrDataBlock* p)
{
	scrDataBlock* p1 = p -> nextLevel -> first();

	worldID = p -> i_dataPtr[0];
	trackID = p -> i_dataPtr[1];

	while(p1){
		switch(p1 -> ID){
			case MCH_SCR_TRACK_ENABLED:
				is_enabled = 1;
#ifdef _DEMO_WORLDS_
//				level = 0;
#endif
				break;
			case MCH_SCR_TRACK_OPEN_MOVIE:
				openMovie = strdup(p1 -> c_dataPtr);
				break;
			case MCH_SCR_TRACK_CLOSE_MOVIE:
				closeMovie = strdup(p1 -> c_dataPtr);
				break;
			case MCH_SCR_TRACK_LEVEL:
#ifdef _DEMO_WORLDS_
				if(!is_enabled)
					level = *p1 -> i_dataPtr;
#else
				level = *p1 -> i_dataPtr;
#endif
				break;
			case MCH_SCR_TRACK_CD:
				cd_trackSeq[cd_trackLen++] = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_PART:
				partID = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_LENGTH:
				length = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_NAME:
				name = strdup(p1 -> c_dataPtr);
				break;
			case MCH_SCR_TRACK_BOSS:
				boss = strdup(p1 -> c_dataPtr);
				break;
			case MCH_SCR_TRACK_BOSS_ID:
				bossID = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_SOUND:
				bossSoundID = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_BOSS_FIGURE_ID:
				bossFigureID = *p1 -> i_dataPtr;
				break;
			case MCH_SCR_TRACK_AI_LEVEL:
				AI_NormLevel = p1 -> i_dataPtr[0];
				AI_MaxLevel = p1 -> i_dataPtr[1];
				break;
			case MCH_SCR_TEXT:
				text = strdup(p1 -> c_dataPtr);
				break;
		}
		p1 = p1 -> next;
	}

	if(!level) access = 1;
}

mchTrackDispatcher::mchTrackDispatcher(void)
{
	mchTrackInfo* tr;
	scrDataBlock* root,*p;
	root = loadScript("RESOURCE\\mch_tracks.scb");

	p = root -> nextLevel -> first();
	while(p){
		if(p -> ID == MCH_SCR_TRACK){
			tr = new mchTrackInfo;
			tr -> Init(p);
			trackLst.append(tr);
		}
		p = p -> next;
	}
	delete p;

	memset(figureFlags,0,20);
	memset(figureFlags2,0,20);
	memset(figureFlags,1,5);
	memset(figureFlags2,1,5);

	curTrack = NULL;
	curLevel = 0;

	int i,world,track;
	sAllWorldsTable awTable("worlds.prm");
	for(i = 0; i < awTable.maxWorld; i ++){
		world=awTable.it[i].numberMechosomWorld;
		track=awTable.it[i].numberMechosomTrack;

		tr = GetTrack(world,track);
		if(tr)
			tr -> SetLength(awTable.it[i].QuantityTrack - 1);
	}
/*
	static char* worldNames[] = {
		"mountain",
		"water   ",
		"forest  ",
		"town    ",
		"lava    ",
		"dolly   ",
		"mechanic",
		"interface",
		"watch   "
	};

	XStream fh("track_price.txt",XS_OUT);
	for(i = 0; i < 4; i ++){
		fh < " ----------- Level " <= i + 1 < " -----------\r\n";
		tr = trackLst.first();
		while(tr){
			if(tr -> Level() == i){
				fh < worldNames[tr -> World()] <= tr -> Track() < "\t" <= tr -> Length() < "cp\t" <= tr -> Length() * 5 < "*\t" <= TrackPrice(tr -> World(),tr -> Track()) < "*\r\n";
			}
			tr = tr -> next;
		}
	}
	fh.close();
*/

	LoadFigureFlags();
}

mchTrackDispatcher::~mchTrackDispatcher(void)
{
	curLevel = 0;
	mchTrackInfo* p = trackLst.first();
	while(p){
		trackLst.remove(p);
		delete p;
		p = trackLst.first();
	}
}

mchTrackInfo* mchTrackDispatcher::GetTrack(int world,int track)
{
	mchTrackInfo* p = trackLst.first();
	while(p){
		if(p -> World() == world && p -> Track() == track)
			break;
		p = p -> next;
	}
	return p;
}

void mchTrackDispatcher::SetCurTrack(int world,int track)
{
	curTrack = GetTrack(world,track);
	if(curTrack) curTrack -> CD_TrackReset();
}

int mchTrackDispatcher::TrackPrice(int world,int track)
{
	mchTrackInfo* p;

	if(world != -1 && track != -1)
		p = GetTrack(world,track);
	else
		p = curTrack;

	return TrackPrice(p);
}

int mchTrackDispatcher::TrackPrice(mchTrackInfo* p)
{
	int num = 0,price = 0;
	mchTrackInfo *p1;

	if(!p) return 0;

	if(!p -> Level())
		return p -> Length() * MCH_CP_STAR_NUM * mchNumLaps;

	p1 = trackLst.first();
	while(p1){
		if(p1 -> isEnabled() && p1 -> Level() == p -> Level() - 1)
			price += p1 -> Length() * MCH_CP_STAR_NUM * mchNumLaps;
		else {
			if(p1 -> isEnabled() && p1 -> Level() == p -> Level())
				num ++;
		}

		p1 = p1 -> next;
	}
	if(num)
		return price / num;

	return 0;
}

int mchTrackDispatcher::CheckLevel(int level)
{
	int n = 0;
	mchTrackInfo* p = trackLst.first();
	while(p){
		if(p -> isEnabled()){
			if(p -> Level() < level && !p -> Status())
				return 0;

			if(p -> Level() == level && !p -> Status())
				n ++;
		}
		p = p -> next;
	}
	if(n > 1) return 0;

	return 1;
}

int mchTrackDispatcher::TrackMode(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track);

	return TrackMode(p);
}

int mchTrackDispatcher::TrackMode(mchTrackInfo* p)
{
	mchTrackInfo* p0;

	if(!p -> isEnabled())
		return MCH_TRACK_NOT_AVAILABLE;

	if(flags & MCH_TRACK_DEBUG_FLAG){
		if(!p)
			return MCH_TRACK_ACCESSIBLE;

		if(p -> Status())
			return MCH_TRACK_DONE;
		else
			return MCH_TRACK_ACCESSIBLE;
	}

	if(!p) 
		return MCH_TRACK_NOT_AVAILABLE;

	if(p -> Boss()){
		p0 = trackLst.first();
		while(p0){
			if(p0 != p && p0 -> World() == p -> World() && (!p0 -> Access() || !p0 -> Status()))
				return MCH_TRACK_DISABLED; 
			p0 = p0 -> next;
		}
		if(p -> Status())
			return MCH_TRACK_HIDDEN;
		else {
			p -> SetAccess();
			return MCH_TRACK_ACCESSIBLE;
		}
	}

	if(p -> Access()){
		if(p -> Status())
			return MCH_TRACK_DONE;
		else
			return MCH_TRACK_ACCESSIBLE;
	}
	else {
		if(p -> Level() <= curLevel)
			return MCH_TRACK_NEXT_LEVEL;
		else
			return MCH_TRACK_DISABLED; 
	}

	return MCH_TRACK_NOT_AVAILABLE;
}

void mchTrackDispatcher::OpenTrack(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track);
	if(p) p -> Open();
}

int mchTrackDispatcher::TrackPart(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track);

	if(p)
		return p -> Part();

	return 0;
}

void mchTrackDispatcher::Save(XStream& fh)
{
	mchTrackInfo* p;
	fh < curLevel < trackLst.size();

	p = trackLst.first();
	while(p){
		fh < p -> World() < p -> Track() < p -> Access() < p -> Status() < p -> PassStatus();
		p = p -> next;
	}

	fh.write(figureFlags,20);
}

void mchTrackDispatcher::Load(XBuffer& xb,int ver)
{
	mchTrackInfo* p;
	int i,world,track,access,status,pass_status,size;
	xb > curLevel > size;

	for(i = 0; i < size; i ++){
		xb > world > track > access > status;

		if(ver >= 106)
			xb > pass_status;

		p = GetTrack(world,track);
		if(p){
			p -> SetStatus(status);
			p -> SetPassStatus(pass_status);
			p -> SetAccess(access);
		}
	}

	if(ver >= 102){
		xb.read(figureFlags,20);
//		for(i = 0; i < 20; i ++)
//			if(figureFlags[i]) figureFlags2[i] = 1;
	}
}

void mchTrackDispatcher::Reset(void)
{
	mchTrackInfo* p;
	flags = 0;

	memset(figureFlags,0,20);
	memset(figureFlags,1,5);

	p = trackLst.first();
	while(p){
		p -> Reset();		
		p = p -> next;
	}

	curLevel = 0;
}

char* mchTrackDispatcher::TrackName(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track);

	if(p) return p -> Name();

	return NULL;
}

char* mchTrackDispatcher::TrackText(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track);

	if(p) return p -> Text();

	return NULL;
}

int mchTrackDispatcher::GetPrize(int nstar)
{
	int status,next_price = 0,prize;
	mchTrackInfo* p;

	p = trackLst.first();
	while(p){
		if(p -> Level() == curLevel){
			status = TrackMode(p);

			if(status == MCH_TRACK_ACCESSIBLE)
				return 0;
			if(status == MCH_TRACK_NEXT_LEVEL)
				next_price = TrackPrice(p);
		}

		p = p -> next;
	}

	if(!next_price){
		p = trackLst.first();
		while(p){
			if(p -> Level() == curLevel + 1){
				next_price = TrackPrice(p);
				break;
			}

			p = p -> next;
		}
	}

	prize = next_price - nstar;
	if(prize < 0) prize = 0;

	return prize;

	return 0;
}

int mchTrackDispatcher::AllBossesDefeated(void)
{
	int num_bosses = 0;
	mchTrackInfo* p = trackLst.first();
	while(p){
		if(p -> Boss()){
			if(!p -> Status())
				return 0;

			num_bosses ++;
		}
		p = p -> next;
	}

	if(num_bosses)
		return 1;

	return 0;
}

int mchTrackDispatcher::IsBossWorldOpen(void)
{
	mchTrackInfo* p = trackLst.first();
	while(p){
		if(p -> Boss() && !p -> Status() && TrackMode(p) == MCH_TRACK_ACCESSIBLE)
			return 1;
		p = p -> next;
	}

	return 0;
}

int mchTrackDispatcher::IsNewWorldOpen(int nstar)
{
	mchTrackInfo* p = trackLst.first();
	while(p){
		if(TrackMode(p) == MCH_TRACK_NEXT_LEVEL && TrackPrice(p) <= nstar)
			return 1;
		p = p -> next;
	}

	return 0;
}

mchTrackInfo* mchTrackDispatcher::NextTrack(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track),*p1;
	if(p){
		p1 = p -> next;
		if(!p1) p1 = trackLst.first();

		while(!p1 -> isEnabled() || p1 -> Boss()){
			p1 = p1 -> next;
			if(!p1) p1 = trackLst.first();
		}

		return p1;
	}
	return NULL;
}

mchTrackInfo* mchTrackDispatcher::PrevTrack(int world,int track)
{
	mchTrackInfo* p = GetTrack(world,track),*p1;
	if(p){
		p1 = p -> prev;

		while(!p1 -> isEnabled() || p1 -> Boss())
			p1 = p1 -> prev;

		return p1;
	}

	return NULL;
}

void mchTrackDispatcher::GetRndTrack(int& wrld,int& track)
{
	int num;
	mchTrackInfo* p = trackLst.first();

	static int prev_world = -1;
	static int prev_track = -1;

	num = XRnd(trackLst.size()) + 1;

	while(num){
		p = p -> next;
		if(!p) p = trackLst.first();

		while(!p -> isEnabled() || p -> Boss() || (prev_track == p -> Track() && prev_world == p -> World())){
			p = p -> next;
			if(!p) p = trackLst.first();
		}

		num --;
	}

	prev_world = p -> World();
	prev_track = p -> Track();

	wrld = p -> World();
	track = p -> Track();
}

void mchTrackDispatcher::SaveFigureFlags(void)
{
	int i;
	XBuffer str;
	str < "\"" <= (int)figureFlags2[0];

	for(i = 1; i < 20; i ++)
		str < " " <= (int)figureFlags2[i];

	str < "\"";

	putIniKey(mch_hotseatINI,"chars","enable",str);
}

void mchTrackDispatcher::LoadFigureFlags(void)
{
	int i,id;
	char* p = getIniKey(mch_hotseatINI,"chars","enable");

	if(!strlen(p))
		return;

	XBuffer str(p,strlen(p) + 1);

	for(i = 0; i < 20; i ++){
		str >= id;
		figureFlags2[i] = id;
	}
}
