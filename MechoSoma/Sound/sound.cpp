
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include <StdAfx.h>

#include <unordered_map>

#include "mechosoma.h"

#include "Md3d.h"
#include "xsound.h"

#include "aci_parser.h"
#include "sound.h"
#include "sound_api.h"
#include "snd_def.h"

#include "mch_common.h" // For far target

#pragma warning( disable : 4244 )

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int acsActiveFlag;

extern int acsMaxCDTrack;
extern int acsCurCDTrack;

extern int sndDynamicChStart;
extern int sndDynamicChNum;

extern int sndDynamicChStart2;
extern int sndDynamicChNum2;

extern int mchIntroMovieActive;

extern char sndTrackLoop[];

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

mchSoundEFF* mchCreateSoundEFF(scrDataBlock* p);
mchSoundTimer* mchCreateTimer(scrDataBlock* p);

void mchWorldSoundQuant(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define CD_TIMER_PERIOD	50
#define CD_VOLUME_DELTA	64

#define EFFECT_KHZ	22050

int mchSoundMute = 0;
int mchMusicMute = 1;
int mchSoundInitFlag = 0;
mchSoundDispatcher* mch_sndD = NULL;

int mchSoundVolumeBackup;
int mchMusicVolumeBackup;

int mchSoundVolume;
int mchSoundVolume_dB;

int mchMusicVolume;
int mchMusicVolumeTMP;
int mchMusicMode = MCH_CD_STOPPED;

int mchMusicTimer = 0;
int mchMusicStatusLast = -1;

int sndRndPeriod = 50;
int sndRndValue = 2;
int sndRndTimer = 0;

float sndFinishVolume = 1.0f;
float sndGlobalVolume = 1.0f;

mchSoundEFF::mchSoundEFF(void)
{
	ID = -1;

	X = Y = -1;

	Channel = 0;
	Priority = 0;
	Cropos = 0;

	UserVolume = 0.0f;

	flags = 0;

	volMode = 0;
	volValue = 0;

	panMode = 0;
	panValue = 0;

	freqMode = 0;
	freqValue = 0;
	freqDeltaUp = 0;
	freqDeltaDn = 0;

	fileName = NULL;
	dsPtr = NULL;

	list = 0;

	CurTime = 0;
	Period = 1;
}

mchSoundEFF::~mchSoundEFF(void)
{
	if(fileName) free(fileName);
	if(dsPtr){
		if(flags & EFF_STREAM)
			SoundStreamClose(dsPtr);
		else
			SoundRelease(dsPtr);
	}
}

void mchSoundEFF::Load(void)
{
	if(!fileName) return;
	XStream fh(0);

	if(fh.open(fileName,XS_IN)){
		fh.close();
		if(flags & EFF_STREAM)
			SoundStreamOpen(fileName,&dsPtr);
		else {
			SoundLoad(fileName,&dsPtr);
			if(!freqValue)
				freqValue = GetSoundFrequency(dsPtr);
		}
	}
}

void mchSoundEFF::Load(const char* fname)
{
	if(fileName) free(fileName);
	fileName = strdup(fname);
	
	Load();
}

void mchSoundEFF::Release(void)
{
	if(flags & EFF_STREAM)
		SoundStreamClose(dsPtr);
	else
		SoundRelease(dsPtr);
	dsPtr = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		mchSoundDispatcher
//////////////////////////////////////////////////////////////////////////////////////////////////
mchSoundDispatcher::mchSoundDispatcher(void)
{
	flags = 0;
	disable_sound = 0;

	globalEFF = new xtList<mchSoundEFF>;
	localEFF = new xtList<mchSoundEFF>;

	globalTimers = new xtList<mchSoundTimer>;
	localTimers = new xtList<mchSoundTimer>;

#ifndef _FINAL_VERSION_
	log_sound = 0;
	log_file = 0;
	for(int i = 1;i < __argc;i++){
		char* p = strstr(__argv[i], "sound");
		if(p){
			log_sound = atoi(p + strlen("sound"));
			log_file = new XStream("sound_log", XS_OUT);
			log_file -> SetDigits(3);
			}
		}
#endif
}

mchSoundDispatcher::~mchSoundDispatcher(void)
{
	delete globalEFF;
	delete localEFF;

	delete globalTimers;
	delete localTimers;

#ifndef _FINAL_VERSION_
	if(log_file)
		delete log_file;
#endif
}

void mchSoundEFF::Start(int x,int y,float vol,float speed,int loop)
{
	int flag = 0,freq,d_freq,ch,pr;

	if(!dsPtr) return;

	if(flags & EFF_DEFINED_COORDS){
		x = X;
		y = Y;
	}

	if(!Status()){
		speedValue = speed;
		UserVolume = vol;
		volValue = 0;

		X = x;
		Y = y;

		if(x != -1){
			volValue = panValue = 0;
			if(!sndVisible(x,y)) return;

			if(flags & EFF_DYNAMIC_CHANNEL){
				ch = sndGetFreeChannel(x,y,pr);
				if(ch == -1) return;
				Channel = ch;
				Priority = pr;
			}
			if(flags & EFF_DYNAMIC_CHANNEL2){
				ch = sndGetFreeChannel2(x,y,pr);
				if(ch == -1) return;
				Channel = ch;
				Priority = pr;
			}

			if(volMode == EFF_VOL_DIST)
				volValue = sndVolumeXY(x,y);

			if(panMode == EFF_PAN_X)
				panValue = sndPanXY(x,y);

			if(panMode == EFF_PAN_SCR_X)
				panValue = sndPanScrXY(x,y);

			if(freqMode == EFF_FREQ_DIST){
				freq = sndFreqXY(x,y,freqValue,freqDeltaDn);
				SetSoundFrequency(dsPtr,freqValue + freq);
			}
		}

		if(freqMode == EFF_FREQ_RND){
			if(!sndRnd(2))
				d_freq = -(int)sndRnd(freqDeltaDn);
			else
				d_freq = sndRnd(freqDeltaUp);

			freq = freqValue + d_freq * freqValue / 100;
			SetSoundFrequency(dsPtr,freq);
		}

		if(loop || flags & EFF_LOOP) flag |= DS_LOOPING;
		if(flags & EFF_QUEUE) flag |= DS_QUEUE;

		if(flags & EFF_STREAM)
			SoundStreamPlay(dsPtr,Channel,Priority,Cropos,flag);
		else {
			if(vol != 1.0f)
				volValue += round((1.0f - vol) * (float)(-sndMinVolume - volValue));

			if(!(flags & EFF_IGNORE_GLOBAL_VOLUME)){
				volValue += sndGetCameraVolume();

				if(sndGlobalVolume != 1.0f)
					volValue += round((1.0f - sndGlobalVolume) * (float)(-10000 - volValue));
			}

			SetVolume(dsPtr,volValue + mchSoundVolume_dB);
			SoundPlay(dsPtr,Channel,Priority,Cropos,flag);

			if(panValue)
				SoundPan(Channel,panValue);
		}
	}
	else {
		if(UserVolume <= vol){
			X = x;
			Y = y;
			UserVolume = vol;
			speedValue = speed;
		}
	}

	flags &= ~EFF_PAUSED;
}

void mchSoundEFF::Stop(void)
{
	if(dsPtr && dsPtr == GetSound(Channel))
		SoundStop(Channel);
}

void mchSoundEFF::Quant(void)
{
	float v;
	int freq,d_freq,pan,vol;

	CurTime ++;
	if(CurTime >= Period){
		CurTime = 0;
		if(Status()){
			if(freqMode == EFF_FREQ_V){
				v = speedValue;
				d_freq = (v < 0.0) ? freqDeltaDn : freqDeltaUp;
				freq = freqValue + d_freq * freqValue / 100 * v;
				SetSoundFrequency(dsPtr,freq);
			}
			if(volMode == EFF_VOL_DIST && X != -1){
				vol = sndVolumeXY(X,Y);

				if(UserVolume != 1.0f)
					vol += round((1.0f - UserVolume) * (float)(-10000 - vol));

				if(!(flags & EFF_IGNORE_GLOBAL_VOLUME)){
					volValue += sndGetCameraVolume();

					if(sndGlobalVolume != 1.0f)
						vol += round((1.0f - sndGlobalVolume) * (float)(-10000 - vol));
				}

				if(vol != volValue){
					SetVolume(dsPtr,vol + mchSoundVolume_dB);
					volValue = vol;
				}
			}

			if(panMode == EFF_PAN_X && X != -1){
				pan = sndPanXY(X,Y);
				if(pan != panValue){
					SoundPan(Channel,pan);
					panValue = pan;
				}
			}
			if(panMode == EFF_PAN_SCR_X && X != -1){
				pan = sndPanScrXY(X,Y);
				if(pan != panValue){
					SoundPan(Channel,pan);
					panValue = pan;
				}
			}
			if(freqMode == EFF_FREQ_DIST && X != -1){
				freq = sndFreqXY(X,Y,freqValue,freqDeltaDn);
				SetSoundFrequency(dsPtr,freqValue + freq);
			}
		}
	}
}

void mchSoundDispatcher::DisableTimer(int tm_id)
{
	mchSoundTimer* p;

	p = globalTimers -> first();
	while(p){
		if(p -> effID == tm_id){
			p -> Enable(0);
			break;
		}
		p = p -> next;
	}

	p = localTimers -> first();
	while(p){
		if(p -> effID == tm_id){
			p -> Enable(0);
			break;
		}
		p = p -> next;
	}

	StopEFF(tm_id);
}

void mchSoundDispatcher::Quant(void)
{
	mchSoundEFF* p;
	mchSoundTimer* tp;

	if(mchTimeMode == MCH_TIME_GAME_OVER)
		sndGlobalVolume = sndFinishVolume;
	else
		sndGlobalVolume = 1.0f;

	if(mchTimeMode != MCH_TIME_STOPPED && !acsActiveFlag && mchTimeMode != MCH_TIME_WAITING_TURN){
		tp = globalTimers -> first();
		while(tp){
			tp -> Quant();
			tp = tp -> next;
		}
		tp = localTimers -> first();
		while(tp){
			tp -> Quant();
			tp = tp -> next;
		}
	}

	p = globalEFF -> first();
	while(p){
		p -> Quant();
		p = p -> next;
	}
	p = localEFF -> first();
	while(p){
		p -> Quant();
		p = p -> next;
	}
}

void mchSoundDispatcher::Load(const char* scr_name,int local)
{
	if(!mchSoundInitFlag) return;

	XStream fh(0);

	scrDataBlock* root,*p;
	xtList<mchSoundEFF>* lst;
	xtList<mchSoundTimer>* tm_lst;
	lst = (local) ? localEFF : globalEFF;
	tm_lst = (local) ? localTimers : globalTimers;
	mchSoundEFF* eff;
	mchSoundTimer* tm;

	Free(local);

	if(fh.open(scr_name,XS_IN)){
		fh.close();
		root = loadScript(scr_name);
		p = root -> nextLevel -> first();
		while(p){
			switch(p -> ID){
				case SND_MUSIC_LOOP_FLAG:
					if(p -> i_dataPtr[0] > 0 && p -> i_dataPtr[0] < CDTRACK_MAX)
						sndTrackLoop[p -> i_dataPtr[0]] = 1;
					break;
				case SND_DATA:
					if(!local || sndCheckTrack(p)){
						eff = mchCreateSoundEFF(p);
						lst -> append(eff);
					}
					break;
				case SND_TIMER:
					if(!local || sndCheckTrack(p)){
						tm = mchCreateTimer(p);
						tm_lst -> append(tm);
					}
					break;
				case SND_DYNAMIC_CHANNEL1:
					sndDynamicChStart = p -> i_dataPtr[0];
					sndDynamicChNum = p -> i_dataPtr[1];
					break;
				case SND_DYNAMIC_CHANNEL2:
					sndDynamicChStart2 = p -> i_dataPtr[0];
					sndDynamicChNum2 = p -> i_dataPtr[1];
					break;
				case SND_MAX_PANNING:
					sndMaxPanning = p -> i_dataPtr[0];
					break;
				case SND_MIN_VOLUME:
					sndMinVolume = p -> i_dataPtr[0];
					break;
				case SND_VISIBLE_AREA:
					sndVisibleArea = p -> i_dataPtr[0];
					break;
				case SND_HVOLUME_Z0:
					sndCameraVolumeZ0 = p -> i_dataPtr[0];
					break;
				case SND_HVOLUME_Z1:
					sndCameraVolumeZ1 = p -> i_dataPtr[0];
					break;
				case SND_HVOLUME_V0:
					sndCameraVolumeV0 = p -> i_dataPtr[0];
					break;
				case SND_HVOLUME_V1:
					sndCameraVolumeV1 = p -> i_dataPtr[0];
					break;
				case SND_FINISH_VOLUME_VAL:
					sndFinishVolume = p -> d_dataPtr[0];
					break;
			}
			p = p -> next;
		}
	}

	eff = lst -> first();
	while(eff){
		if(local)
			map_localEFF.insert(std::unordered_multimap<int,mchSoundEFF*>::value_type(eff -> ID,eff));
		else
			map_globalEFF.insert(std::unordered_multimap<int,mchSoundEFF*>::value_type(eff -> ID,eff));

		eff -> Load();
		eff = eff -> next;
	}
}

void mchSoundDispatcher::Free(int local)
{
	mchSoundEFF* p;
	mchSoundTimer* t;
	xtList<mchSoundEFF>* lst;
	xtList<mchSoundTimer>* tm_lst;

	if(local){
		lst = localEFF;
		tm_lst = localTimers;
		map_localEFF.clear();
	}
	else {
		lst = globalEFF;
		tm_lst = globalTimers;
		map_globalEFF.clear();
	}

	p = lst -> first();
	while(p){
		p -> Stop();
		lst -> remove(p);
		delete p;
		p = lst -> first();
	}

	t = tm_lst -> first();
	while(t){
		tm_lst -> remove(t);
		delete t;
		t = tm_lst -> first();
	}
}

void mchSoundDispatcher::StartEFF(int id,int x,int y,float vol,float speed)
{
	if(disable_sound)
		return;

	int i,eff_size = 0,eff_id;
	mchSoundEFF* p = NULL;
	
	std::unordered_multimap<int,mchSoundEFF*>::iterator si;
	std::pair<std::unordered_multimap<int,mchSoundEFF*>::iterator,std::unordered_multimap<int,mchSoundEFF*>::iterator> pr;

	eff_size = map_localEFF.count(id);

	if(eff_size){
		pr = map_localEFF.equal_range(id);
		si = pr.first;

		if(eff_size > 1){
			eff_id = sndRnd(eff_size);
			for(i = 0; i < eff_id; i ++) si ++;
		}

		if(si != map_localEFF.end())
			p = si -> second;
	}
	else {
		eff_size = map_globalEFF.count(id);
		if(eff_size){
			pr = map_globalEFF.equal_range(id);
			si = pr.first;

			if(eff_size > 1){
				eff_id = sndRnd(eff_size);
				for(i = 0; i < eff_id; i ++) si ++;
			}

			if(si != map_globalEFF.end())
				p = si -> second;
		}
	}

	if(p) p -> Start(x,y,vol,speed);

#ifndef _FINAL_VERSION_
	if(log_file && (!log_sound || log_sound == id)){
		char buf[2048];
		sprintf(buf, "| Start | %7.3f | %5i | %5i | %5i | %7.3f | %7.3f | %s", clockf()/1000., id, x, y, vol, speed, (p ? "" : "absent") );
		*log_file < buf < "\n";
		}
#endif
}

void mchSoundDispatcher::SetCoordsEFF(int id,int x,int y)
{
	mchSoundEFF* p = NULL;

	std::unordered_multimap<int,mchSoundEFF*>::iterator si = map_localEFF.find(id);

	if(si == map_localEFF.end()){
		si = map_globalEFF.find(id);
		if(si != map_globalEFF.end())
			p = si -> second;
	}
	else
		p = si -> second;

	if(p) p -> UpdateCoords(x,y);
}

void mchSoundDispatcher::StopEFF(int id)
{
#ifndef _FINAL_VERSION_
	if(log_file && (!log_sound || log_sound == id)){
		char buf[2048];
		sprintf(buf, "| Stop  | %7.3f | %5i |       |       |         |         | ", clockf()/1000., id);
		*log_file < buf < "\n";
		}
#endif

	mchSoundEFF* p = NULL;

	std::unordered_multimap<int,mchSoundEFF*>::iterator si = map_localEFF.find(id);

	if(si == map_localEFF.end()){
		si = map_globalEFF.find(id);
		if(si != map_globalEFF.end())
			p = si -> second;
	}
	else
		p = si -> second;

	if(p) p -> Stop();
}

mchSoundEFF* mchCreateSoundEFF(scrDataBlock* p)
{
	scrDataBlock* b = p -> nextLevel -> first();
	mchSoundEFF* eff = new mchSoundEFF;
	
	eff -> ID = *p -> i_dataPtr;
	while(b){
		switch(b -> ID){
			case SND_X:
				eff -> X = *b -> i_dataPtr;
				eff -> flags |= EFF_DEFINED_COORDS;
				break;
			case SND_Y:
				eff -> Y = *b -> i_dataPtr;
				eff -> flags |= EFF_DEFINED_COORDS;
				break;
			case SND_PERIOD:
				eff -> Period = *b -> i_dataPtr;
				break;
			case SND_CHANNEL:
				eff -> Channel = *b -> i_dataPtr;
				break;
			case SND_PRIORITY:	           
				eff -> Priority = *b -> i_dataPtr;
				break;
			case SND_CROPOS:		    
				eff -> Cropos = *b -> i_dataPtr;
				break;
			case SND_FLAG:		
				eff -> flags |= *b -> i_dataPtr;
				break;
			case SND_VOL_MODE:		
				eff -> volMode = *b -> i_dataPtr;
				break;
			case SND_VOL_VALUE:		
				eff -> volValue = *b -> i_dataPtr;
				break;
			case SND_PAN_MODE:            
				eff -> panMode = *b -> i_dataPtr;
				break;
			case SND_PAN_VALUE:		
				eff -> panValue = *b -> i_dataPtr;
				break;
			case SND_FREQ_MODE:          
				eff -> freqMode = *b -> i_dataPtr;
				break;
			case SND_FREQ_VALUE:         
				eff -> freqValue = *b -> i_dataPtr;
				break;
			case SND_FREQ_DELTA_UP:         
				eff -> freqDeltaUp = *b -> i_dataPtr;
				break;
			case SND_FREQ_DELTA_DN:         
				eff -> freqDeltaDn = *b -> i_dataPtr;
				break;
			case SND_FREQ_DELTA:         
				eff -> freqDeltaDn = *b -> i_dataPtr;
				eff -> freqDeltaUp = *b -> i_dataPtr * 2;
				break;
			case SND_FILE:                    
				eff -> fileName = strdup(b -> c_dataPtr);
				break;
		}
		b = b -> next;
	}
	return eff;
}

mchSoundTimer* mchCreateTimer(scrDataBlock* p)
{
	scrDataBlock* b = p -> nextLevel -> first();
	mchSoundTimer* tm = new mchSoundTimer;
	
	tm -> effID = *p -> i_dataPtr;
	while(b){
		switch(b -> ID){
			case SND_PERIOD:
				tm -> periodValue = *b -> i_dataPtr;
				break;
			case SND_FLAG:		
				tm -> flags |= *b -> i_dataPtr;
				break;
			case SND_RND_VALUE:		
				tm -> rndValue = *b -> i_dataPtr;
				break;
		}
		b = b -> next;
	}
	return tm;
}

void mchPrepareSound(void)
{
	xsMixerOpen();
//	mchSoundVolumeBackup = xsGetVolumeWAVE();
#ifndef _MPPLUS_SOUNDTRACK_
	//mchMusicVolumeBackup = xsGetVolumeCD();
#endif
}

void mchInitSound(void)
{
	xsInitCD();

	memset(sndTrackLoop,0,CDTRACK_MAX);

	if(!XGR_hWnd) d3dGetWindowHandle(&XGR_hWnd);

	mch_sndD = new mchSoundDispatcher;
	if(!SoundInit(EFFECT_KHZ,DIG_F_STEREO_16,32)){
		XCon < "\nInitialization of sound failed, mute mode accepted...\n";
		mchSoundMute = 1;
		return;
	}
	mchSoundInitFlag = 1;
	mch_sndD -> Load("RESOURCE\\SOUND\\mch_snd.scb",0);

	sndRndVal = clock();
}

void mchFinitSound(void)
{
	delete mch_sndD;
}

void mchStartSoundEFF(int id,int x,int y,float vol,float speed)
{
	if(!mchSoundMute && mchSoundInitFlag){
		mch_sndD -> StartEFF(id,x,y,vol,speed);
	}
}

void mchStopSoundEFF(int id)
{
	if(mchSoundInitFlag){
		mch_sndD -> StopEFF(id);
	}
}

void mchSoundQuant(void)
{
	if(!mchSoundMute && mchSoundInitFlag){
		mch_sndD -> Quant();
	}
	mchMusicQuant();
}

void mchToggleSound(void)
{
	if(mch_sndD){
		mchSoundMute ^= 1;
		if(mchSoundMute) mchPauseSound();
		else mchResumeSound();
	}
}

int mchSoundEFF::Status(void)
{
	if(dsPtr && dsPtr == GetSound(Channel))
		return SoundStatus(dsPtr);

	return 0;
}

void mchSoundDispatcher::Stop(void)
{
	mchSoundEFF* p;

	p = localEFF -> first();
	while(p){
		if(p -> Status()) p -> Stop();
		p -> flags &= ~EFF_PAUSED;
		p = p -> next;
	}
	p = globalEFF -> first();
	while(p){
		if(p -> Status()) p -> Stop();
		p -> flags &= ~EFF_PAUSED;
		p = p -> next;
	}
}

void mchSoundDispatcher::Pause(void)
{
	mchSoundEFF* p;

	p = localEFF -> first();
	while(p){
		p -> Pause();
		p = p -> next;
	}
	p = globalEFF -> first();
	while(p){
		p -> Pause();
		p = p -> next;
	}
}

void mchSoundDispatcher::Resume(void)
{
	mchSoundEFF* p;

	p = localEFF -> first();
	while(p){
		p -> Resume();
		p = p -> next;
	}
	p = globalEFF -> first();
	while(p){
		p -> Resume();
		p = p -> next;
	}
}

void mchSoundEFF::Pause(void)
{
	if(Status()){
		Stop();
		flags |= EFF_PAUSED;
	}
}

void mchSoundEFF::Resume(void)
{
	if(flags & EFF_PAUSED)
		Start(X,Y);
}

void mchStopSound(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Stop();
}

void mchPauseSound(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Pause();
}

void mchResumeSound(void)
{
	if(!mchSoundMute && mchSoundInitFlag)
		mch_sndD -> Resume();
}

void mchSetSoundCoords(int id,int x,int y)
{
	if(mchSoundInitFlag)
		mch_sndD -> SetCoordsEFF(id,x,y);
}

void mchMusicQuant(void)
{
	int st;

//	if(mchIntroMovieActive) return;

	if(++mchMusicTimer >= CD_TIMER_PERIOD){
		mchMusicTimer = 0;
		st = sndMusicStatus();
		mchMusicStatusLast = st;

		if(!mchMusicMute && mchMusicMode == MCH_CD_PLAYING){
			st = sndMusicStatus();
			mchMusicStatusLast = st;

			if(st != XCD_PLAYING){ 
				sndNextCD_Track();
				mchSetMusicMode(MCH_CD_PLAYING);
			}
		}
	}

	switch(mchMusicMode){
		case MCH_CD_PLAYING:
			if(mchMusicVolumeTMP != mchMusicVolume){ 
				mchMusicVolumeTMP = mchMusicVolume;
				sndMusicSetVolume(mchMusicVolumeTMP);
			}
			break;
		case MCH_CD_HIDE:
			mchMusicVolumeTMP -= CD_VOLUME_DELTA;
			if(mchMusicVolumeTMP < 0) mchMusicVolumeTMP = 0;
			sndMusicSetVolume(mchMusicVolumeTMP);
			if(!mchMusicVolumeTMP){
				sndMusicPause();
				mchMusicMode = MCH_CD_PAUSED;
			}
			break;
		case MCH_CD_SHOW:
			mchMusicVolumeTMP += CD_VOLUME_DELTA;
			if(mchMusicVolumeTMP > mchMusicVolume) mchMusicVolumeTMP = mchMusicVolume;
			sndMusicSetVolume(mchMusicVolumeTMP);
			if(mchMusicVolumeTMP == mchMusicVolume){
				mchMusicMode = MCH_CD_PLAYING;
			}
			break;
	}

	mchMusicStatusLast = -1;
}

void mchSetMusicMode(int mode,int data)
{
	int tr = 0;
//	if(acsMaxCDTrack < 2 || mchIntroMovieActive) return;
	if(acsMaxCDTrack < 2) return;

	switch(mode){
		case MCH_CD_PLAYING:
			acsCurCDTrack = sndGetCurrentCD_Track();
			if(acsCurCDTrack){
				sndMusicPlay(acsCurCDTrack);
/*
				if(acsCurCDTrack < acsMaxCDTrack)
					sndMusicPlay(acsCurCDTrack);
				else
					xsPlayCD(acsCurCDTrack);
*/
			}
			else {
				if(mchMusicStatusLast != XCD_STOPPED)
 					sndMusicStop();
			}
			break;
		case MCH_CD_STOPPED:
			if(mchMusicStatusLast != XCD_STOPPED)
				sndMusicStop();
			break;
		case MCH_CD_PAUSED:
			sndMusicPause();
			break;
		case MCH_CD_HIDE:
			mchMusicVolumeTMP = mchMusicVolume;
			sndMusicSetVolume(mchMusicVolumeTMP);
			break;
		case MCH_CD_SHOW:
			if(mchMusicMode == MCH_CD_PLAYING){
				tr = sndMusicCurTrack();
				acsCurCDTrack = sndGetCurrentCD_Track();

				if(tr != acsCurCDTrack)
					mchSetMusicMode(MCH_CD_PLAYING);
				else
					return;
			}
			if(mchMusicMode == MCH_CD_STOPPED){
				acsCurCDTrack = sndGetCurrentCD_Track();
				mchSetMusicMode(MCH_CD_PLAYING);
			}

			if(mchMusicMode == MCH_CD_PAUSED){
				tr = sndMusicCurTrack();
				acsCurCDTrack = sndGetCurrentCD_Track();
				if(tr != acsCurCDTrack)
					mchSetMusicMode(MCH_CD_PLAYING);
				else {
					if(tr)
						sndMusicResume();
				}
			}
			mchMusicVolumeTMP = mchMusicVolume;
//			mchMusicVolumeTMP = 0;
			sndMusicSetVolume(mchMusicVolumeTMP);
			break;
	}
	mchMusicMode = mode;
}

void mchSoundTimer::Quant(void)
{
	int x,y;

	if(flags & SND_TIMER_DISABLED) return;

	if(++timer >= periodValue){
		x = y = -1;

		timer = 0;
		if(!sndRnd(rndValue)){
			if(flags & SND_TIMER_RND_COORDS)
				sndCoordsRnd(x,y);

			mchStartSoundEFF(effID,x,y,1.0f,0.0f);
		}
	}
}

int mchSoundDispatcher::GetStatusEFF(int id)
{
	int i,eff_size = 0,eff_id;
	mchSoundEFF* p = NULL;

	std::unordered_multimap<int,mchSoundEFF*>::iterator si;
	std::pair<std::unordered_multimap<int,mchSoundEFF*>::iterator,std::unordered_multimap<int,mchSoundEFF*>::iterator> pr;

	eff_size = map_localEFF.count(id);

	if(eff_size){
		pr = map_localEFF.equal_range(id);
		si = pr.first;

		if(eff_size > 1){
			eff_id = sndRnd(eff_size);
			for(i = 0; i < eff_id; i ++) si ++;
		}

		if(si != map_localEFF.end())
			p = si -> second;
	}
	else {
		eff_size = map_globalEFF.count(id);
		if(eff_size){
			pr = map_globalEFF.equal_range(id);
			si = pr.first;

			if(eff_size > 1){
				eff_id = sndRnd(eff_size);
				for(i = 0; i < eff_id; i ++) si ++;
			}

			if(si != map_globalEFF.end())
				p = si -> second;
		}
	}

	if(p) return p -> Status();

	return 0;
}

void mchSoundDispatcher::Startup(void)
{
	mchSoundEFF* p;
	p = localEFF -> first();
	while(p){
		if(p -> flags & EFF_STARTUP)
			p -> Start();
		p = p -> next;
	}
	p = globalEFF -> first();
	while(p){
		if(p -> flags & EFF_STARTUP)
			p -> Start();
		p = p -> next;
	}
}

void mchSoundStartup(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Startup();
}

void sndSetVolume(int v)
{
	mchSoundVolume = v;

	const int DB_MIN = -10000;
	const int DB_MAX = 0;
	const int DB_SIZE = DB_MAX - DB_MIN;

	mchSoundVolume_dB = DB_MIN + round(log10(9.0*log(double(v + 1))/(log(2.0)*8) + 1.0)*DB_SIZE);
}
