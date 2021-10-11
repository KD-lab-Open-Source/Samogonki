
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include <StdAfx.h>

#include "mch_rto.h"

#include "mechosoma.h"
#include "race.h"
#include "mesh3ds.h"
#include "terra.h"
#include "arcane.h"

#include "CameraDispatcher.h"
#include "TrackDispatcher.h"

#include "arcane_menu.h"

#include "_xsound.h"
#include "aci_parser.h"
#include "sound.h"
#include "sound_api.h"
#include "snd_def.h"

#include "aci_ids.h"
#include "aci_scr.h"

#include "PlayWSC.h"

#ifdef _MPPLUS_SOUNDTRACK_
#include "PlayMpeg.h"
#endif

#include "mch_common.h" // For far target

#pragma warning( disable : 4244 )

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern ShowImageRTO* mch_imageRTO;

extern int acsActiveFlag;
extern int mchImgRTO_ActiveFlag;

extern int mchTrackDemoMode;
extern int mchA_EndGameMode;

extern int mchResourcesFlag;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchA_d3dOutString(int x,int y,float sx,float sy,void* str,int col,int alpha,int fnt,int space = 0,float mul = 1.0f,int pr_flag = 1,int vspace = 0);

/* --------------------------- DEFINITION SECTION --------------------------- */

int sndDynamicChStart = 10;
int sndDynamicChNum = 6;

int sndDynamicChStart2 = 7;
int sndDynamicChNum2 = 1;

int sndMaxPanning = 3500;
int sndVisibleArea = 600;

int sndCameraVolumeZ0 = 100;
int sndCameraVolumeZ1 = 1000;
int sndCameraVolumeV0 = 0;
int sndCameraVolumeV1 = -3000;

int sndMinVolume = 4000;

unsigned sndRndVal = 83838383;

#ifdef _MPPLUS_SOUNDTRACK_
int sndMPPlusCurTrack = 0;
#endif

char sndTrackLoop[CDTRACK_MAX]; 

int sndVisible(int x,int y)
{
	if(abs(getDistX(x,camera_dispatcher -> observationPoint().xi())) > sndVisibleArea || abs(getDistY(y,camera_dispatcher -> observationPoint().yi())) > sndVisibleArea)
		return 0;

	return 1;
}

int sndVolumeXY(int x,int y)
{
	float xx,yy,vol;

	xx = getDistX(x,camera_dispatcher -> observationPoint().xi());
	yy = getDistY(y,camera_dispatcher -> observationPoint().yi());

	vol =  - (xx * xx + yy * yy) / ((float)sndVisibleArea * (float)sndVisibleArea) * 5000.0;
	if(vol > 0.0f) vol = 0.0f;
	if(vol < -10000.0f) vol = -10000.0f;

	return round(vol);
}

int sndFreqXY(int x,int y,int freq_val,int freq_delta)
{
	float xx,yy,frq;

	xx = getDistX(x,camera_dispatcher -> observationPoint().xi());
	yy = getDistY(y,camera_dispatcher -> observationPoint().yi());

	frq = (float)freq_val * sqrt(xx * xx + yy * yy) / (float)sndVisibleArea * freq_delta / 100.0;

	return round(frq);
}

int sndPanXY(int x,int y)
{
	Vect3f r;
	int xx,pan;
	float pan_max,angle;
	Vect3f vec(getDistX(camera_dispatcher -> observationPoint().xi(),x),getDistY(camera_dispatcher -> observationPoint().yi(),y),0.0f);

	int z,x0,y0,x1,y1;

	if(!mch_raceD || !(mch_raceD -> activeRacer))
		return 0;

	if(!camera_dispatcher -> isRotateEnabled()){
		xx = getDistX(x,camera_dispatcher -> observationPoint().xi());
		pan = (xx * abs(xx) * 10000) / (sndVisibleArea * sndVisibleArea) / 4;
	}
	else { 
		pan_max = (float)sndMaxPanning * vec.norm() / (float)sndVisibleArea;

		z = mch_raceD -> activeRacer -> R().z;
		Vect3f pv,pe;
		camera_dispatcher -> ConvertorWorldToViewPort(Vect3f(x,y,z), pv, pe);
		x1=round(pe.x), y1=round(pe.y);

		x = mch_raceD -> activeRacer -> R().x;
		y = mch_raceD -> activeRacer -> R().y;
		z = mch_raceD -> activeRacer -> R().z;
		camera_dispatcher -> ConvertorWorldToViewPort(Vect3f(x,y,z), pv, pe);
		x0=round(pe.x), y0=round(pe.y);

		r.y = x1 - x0;
		r.x = -y1 + y0;
		r.z = 0.0f;

		angle = r.psi();

		pan = round(pan_max * sin(angle));

		if(pan > sndMaxPanning) pan = sndMaxPanning;
		if(pan < -sndMaxPanning) pan = -sndMaxPanning;
	}

	if(pan > 10000) pan = 10000;
	if(pan < -10000) pan = -10000;

	return pan;
}

int sndPanScrXY(int x,int y)
{
	int xx,pan;

	xx = x - 320;
	pan = (xx * abs(xx) * 10000) / (640 * 640) / 1;

	if(pan > 10000) pan = 10000;
	if(pan < -10000) pan = -10000;

	return pan;
}

int sndGetFreeChannel(int x,int y,int& pr)
{
	int i;
	float dist,xx,yy;

	xx = getDistX(x,camera_dispatcher -> observationPoint().xi());
	yy = getDistY(y,camera_dispatcher -> observationPoint().yi());
	dist = sqrt(xx * xx + yy * yy) / (float)sndVisibleArea * (float)sndDynamicChNum;

	pr = round(dist) + 1;

	for(i = 0; i < sndDynamicChNum; i ++){
		if(ChannelStatus(sndDynamicChStart + i) == -1){
			return sndDynamicChStart + i;
		}
	}
	for(i = 0; i < sndDynamicChNum; i ++){
		if(ChannelStatus(sndDynamicChStart + i) > pr){
			return sndDynamicChStart + i;
		}
	}
	return -1;
}

int sndGetFreeChannel2(int x,int y,int& pr)
{
	int i;
	float dist,xx,yy;

	xx = getDistX(x,camera_dispatcher -> observationPoint().xi());
	yy = getDistY(y,camera_dispatcher -> observationPoint().yi());
	dist = sqrt(xx * xx + yy * yy) / (float)sndVisibleArea * (float)sndDynamicChNum;

	pr = round(dist) + 1;

	for(i = 0; i < sndDynamicChNum2; i ++){
		if(ChannelStatus(sndDynamicChStart2 + i) == -1){
			return sndDynamicChStart2 + i;
		}
	}
	for(i = 0; i < sndDynamicChNum2; i ++){
		if(ChannelStatus(sndDynamicChStart2 + i) > pr){
			return sndDynamicChStart2 + i;
		}
	}
	return -1;
}

char* sndGetTargetName(char* p)
{
	return GetTargetName(p);
}

void mchFreeWorldSound(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Free(1);
}

void mchLoadWorldSound(void)
{
	XBuffer fname;
	fname < "RESOURCE\\SOUND\\world";
	if(mchCurrentWorld < 10) fname < "0";
	fname <= mchCurrentWorld < ".scb";

	if(mchSoundInitFlag)
		mch_sndD -> Load(fname,1);
}

void mchLoadIntroSound(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Load("RESOURCE\\SOUND\\logo_snd.scb",1);
}

void mchLoadIntroMovieSound(void)
{
	if(mchSoundInitFlag)
		mch_sndD -> Load("RESOURCE\\SOUND\\intromovie_snd.scb",1);
}

void sndCoordsRnd(int& x,int& y)
{
	x = camera_dispatcher -> observationPoint().xi() + sndRnd(sndVisibleArea * 2) - sndVisibleArea;
	y = camera_dispatcher -> observationPoint().yi() + sndRnd(sndVisibleArea * 2) - sndVisibleArea;
}

void sndNextCD_Track(void)
{
	if(acsActiveFlag || mchTrackDemoMode || mchTimeMode == MCH_TIME_GAME_OVER || TestWorldScriptPlay.isPlay())
		return;

	if(mch_trackD)
		mch_trackD -> TrackCD_TrackNext();
}

int sndGetCurrentCD_Track(void)
{
	if(mchImgRTO_ActiveFlag){
		if(mch_imageRTO -> Flags[mch_imageRTO -> curFile] & IMG_RTO_START_MUSIC)
			return CDTRACK_MAIN_MENU;

		return 0;
	}

	if(acsActiveFlag){
		if(acsScrD -> curScrID == ACS_CREDITS_SCR)
			return CDTRACK_CREDITS;

		if(mchResourcesFlag)
			return 0;

		return CDTRACK_MAIN_MENU;
	}

	if(TestWorldScriptPlay.isPlay()){
		return 0;
//		return TestWorldScriptPlay.GetMusicTrack();
	}

	if(mchTrackDemoMode)
		return CDTRACK_TR_DEMO;

	if(mchTimeMode == MCH_TIME_GAME_OVER){
		switch(mchA_EndGameMode){
			case AE_RACE_WON:
				return CDTRACK_RACE_WON;
			case AE_RACE_NOT_LOST:
			case AE_RACE_END:
				return CDTRACK_RACE_NOT_LOST;
			case AE_RACE_LOST:
				return CDTRACK_RACE_LOST;
		}

	}

	if(mch_trackD)
		return mch_trackD -> TrackCD_Track();

	return 0;
}

int sndGetCameraVolume(void)
{
	int vol;
	if(acsActiveFlag) return 0;

	if(hCamera <= sndCameraVolumeZ0){
		vol = sndCameraVolumeV0;
	}
	else {
		if(hCamera >= sndCameraVolumeZ1){
			vol = sndCameraVolumeV1;
		}
		else {
			vol = (sndCameraVolumeV1 - sndCameraVolumeV0) * (hCamera - sndCameraVolumeZ0) / (sndCameraVolumeZ1 - sndCameraVolumeZ0) + sndCameraVolumeV0;
		}
	}

	return vol;
}

void sndDumpStats(void)
{
	int i;
	XBuffer XBuf;
	for(i = 0; i < sndDynamicChNum; i ++){
		XBuf < "Ch " <= sndDynamicChStart + i < " -> " <= ChannelStatus(sndDynamicChStart + i) < "\n";
	}
	XBuf < "\n";
	for(i = 0; i < sndDynamicChNum2; i ++){
		XBuf < "Ch " <= sndDynamicChStart2 + i < " -> " <= ChannelStatus(sndDynamicChStart2 + i) < "\n";
	}

	if(RenderMode == DIRECT3D_HICOLOR)
		mchA_d3dOutString(70,70,1.0f,1.0f,XBuf.address(),-1,200,2,0,1.0f);
	else
		XGR_OutText(70,70,0xFFFF,XBuf.address(),1);
}

int sndCheckTrack(scrDataBlock* p)
{
	scrDataBlock* p1;

	if(p -> nextLevel){
		p1 = p -> nextLevel -> search(SND_EXCLUDE_TRACKS);
		if(p1 && p1 -> i_dataPtr[0])
			return !(p1 -> i_dataPtr[0] & (1 << mchCurrentTrack));
		p1 = p -> nextLevel -> search(SND_INCLUDE_TRACKS);
		if(p1 && p1 -> i_dataPtr[0])
			return (p1 -> i_dataPtr[0] & (1 << mchCurrentTrack));
	}
	return 1;
}

void sndMusicPlay(int track)
{
#ifdef _MPPLUS_SOUNDTRACK_
	static XBuffer buf;

	buf.init();
	buf < "RESOURCE\\MUSIC\\HQ\\track";
	if(track < 10) buf < "0";
	buf <= track < ".mp+";

	if(MpegOpenToPlay(buf.address(),sndTrackLoop[track]))
		sndMPPlusCurTrack = track;
	else
		sndMPPlusCurTrack = 0;
#else
	//xsPlayOneTrackCD(track);
#endif
}

void sndMusicStop(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	MpegStop();
	sndMPPlusCurTrack = 0;
#else
	//xsStopCD();
#endif
}

void sndMusicPause(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	MpegPause();
#else
	//xsPauseCD();
#endif
}

void sndMusicResume(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	if(MpegIsPlay() == MPEG_PAUSE)
		MpegResume();
#else
	//xsResumeCD();
#endif
}

int sndMusicStatus(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	switch(MpegIsPlay()){
		case MPEG_PLAY:
			return XCD_PLAYING;
		case MPEG_PAUSE:
			return XCD_PAUSED;
		default:
			return XCD_STOPPED;
	}
#else
	//return xsGetStatusCD();
	return 0;
#endif
}

int sndMusicCurTrack(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	return sndMPPlusCurTrack;
#else
	//return xsGetCurTrackCD();
	return 0;
#endif
}

int sndMusicNumTracks(void)
{
#ifdef _MPPLUS_SOUNDTRACK_
	return CDTRACK_MAX;
#else
	return 0;//return xsGetNumTracksCD();
#endif
}

void sndMusicSetVolume(int val)
{
#ifdef _MPPLUS_SOUNDTRACK_
	MpegSetVolume(val);
#else
	//xsSetVolumeCD(val);
#endif
}

