
#ifndef __SOUND_H__
#define __SOUND_H__

//#define _MPPLUS_SOUNDTRACK_

#include "snd_ids.h"
#include "snd_cd.h"

// mchMusicMode values
enum mchMusicModes  
{
	MCH_CD_STOPPED	= 0,
	MCH_CD_PAUSED,		// 1
	MCH_CD_PLAYING,		// 2
	MCH_CD_HIDE,		// 3
	MCH_CD_SHOW,		// 4

	MCH_CD_MAX_MODE
};

// mchSoundTimer::flags
#define SND_TIMER_RND_COORDS	0x01
#define SND_TIMER_DISABLED	0x02

struct mchSoundTimer
{
	int flags;

	int effID;

	int timer;
	int periodValue;
	int rndValue;

	void* list;
	mchSoundTimer* prev;
	mchSoundTimer* next;

	void Quant(void);
	void Enable(int fl){ if(fl) flags &= ~SND_TIMER_DISABLED; else flags |= SND_TIMER_DISABLED; }
	int isEnabled(void) const { return (flags & SND_TIMER_DISABLED); }

	mchSoundTimer(void){ flags = 0; effID = -1; periodValue = rndValue = timer = 0; list = NULL; }
};

// mchSoundEFF::flags...
#define EFF_LOOP			0x01
#define EFF_QUEUE			0x02
#define EFF_STREAM			0x04
#define EFF_PAUSED			0x08
#define EFF_DYNAMIC_CHANNEL		0x10
#define EFF_DYNAMIC_CHANNEL2		0x20
#define EFF_DEFINED_COORDS		0x40
#define EFF_STARTUP			0x80
#define EFF_IGNORE_GLOBAL_VOLUME	0x100

// mchSoundEFF::freqMode
#define EFF_FREQ_V			0x01
#define EFF_FREQ_RND			0x02
#define EFF_FREQ_DIST			0x03

// mchSoundEFF::volMode
#define EFF_VOL_DIST			0x01

// mchSoundEFF::panMode
#define EFF_PAN_X			0x01
#define EFF_PAN_SCR_X			0x02

struct mchSoundEFF
{
	int ID;

	int X;
	int Y;

	float UserVolume;

	int Period;
	int CurTime;

	int Channel;
	int Priority;
	int Cropos;

	int flags;

	int volMode;
	int volValue;

	int panMode;
	int panValue;

	int freqMode;
	int freqValue;
	int freqDeltaUp;
	int freqDeltaDn;

	float speedValue;

	char* fileName;
	void* dsPtr;

	void* list;
	mchSoundEFF* next;
	mchSoundEFF* prev;

	void Load(void);
	void Load(char* fname);

	void Start(int x = -1,int y = -1,float vol = 1.0f,float speed = 0.0f,int loop = 0);
	void Stop(void);
	void Pause(void);
	void Resume(void);

	void UpdateCoords(int x,int y){ X = x; Y = y; }

	int Status(void);

	void Release(void);
	void Quant(void);

	mchSoundEFF(void);
	~mchSoundEFF(void);
};

struct mchSoundDispatcher
{
	int flags;
	int disable_sound;

	xtList<mchSoundEFF>* globalEFF;
	xtList<mchSoundEFF>* localEFF;

	xtList<mchSoundTimer>* globalTimers;
	xtList<mchSoundTimer>* localTimers;

	typedef hash_multimap<int, mchSoundEFF*> SoundMap;
	SoundMap map_globalEFF;
	SoundMap map_localEFF;

	#ifndef _FINAL_VERSION_	
	// Sounds logging system. Activated when there's "sound" in comline. To remove define "_FINAL_VERSION_".
	int log_sound; // the only sound being logged
	struct XStream* log_file; // -> "sound"
	#endif

	void Load(char* fname,int local = 1);
	void Free(int local = 1);

	void StartEFF(int id,int x,int y,float vol,float speed);
	void StopEFF(int id);
	void SetCoordsEFF(int id,int x,int y);
	int GetStatusEFF(int id);

	void Stop(void);
	void Pause(void);
	void Resume(void);
	void Disable(int disable) { disable_sound = disable; }

	void DisableTimer(int tm_id);

	void Startup(void);

	void Quant(void);

	mchSoundDispatcher(void);
	~mchSoundDispatcher(void);
};

void mchPrepareSound(void);
void mchInitSound(void);
void mchFinitSound(void);
void mchSoundQuant(void);
void mchStartSoundEFF(int id,int x = -1,int y = -1,float vol = 1.0f,float speed = 0.0f);
void mchStopSoundEFF(int id);
void mchSetSoundCoords(int id,int x,int y);
void mchToggleSound(void);

void sndSetVolume(int v);

void mchFreeWorldSound(void);
void mchLoadWorldSound(void);
void mchLoadIntroSound(void);
void mchLoadIntroMovieSound(void);

void mchStopSound(void);
void mchResumeSound(void);
void mchPauseSound(void);
void mchSoundStartup(void);

void mchMusicQuant(void);
void mchSetMusicMode(int mode,int data = 0);

extern mchSoundDispatcher* mch_sndD;
extern int mchSoundMute;
extern int mchSoundInitFlag;
extern int mchMusicMute;

extern int mchSoundVolume;
extern int mchMusicVolume;

extern int mchMusicMode;
#endif