/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "race.h"
#include "CameraDispatcher.h"
#include "TERRA.H"

#include "ACI_IDS.H"
#include "ACI_SCR.H"

#include "TrackDispatcher.h"
#include "demo_dispatcher.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int mchWorldSelectFlag;
extern int mchPlayerRecorder;
extern int mchEnableTrackDemo;
extern int mchEnableStartCounter;

extern int mch_iScreen;
extern int mch_iScreenID;

extern int mchFreeResourcesFlag;

extern char* mchIscreenINI;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsInitSaveSlots(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

int DEMO_IDLE_TIMER		 = 5000;
int DEMO_IDLE_TIMER_RND		 = 3000;

int DEMO_ROTATING_CAMERA_TIMER	 = 15000;
int DEMO_ROTATING_CAMERA_TIMER_RND = 3000;

int DEMO_MOVING_CAMERA_TIMER	 = 3000;
int DEMO_MOVING_CAMERA_TIMER_RND = 1000;

int DEMO_PLANET_VIEW_TIMER	 = 10000;
int DEMO_PLANET_VIEW_TIMER_RND	 = 3000;

int DEMO_CHANGE_PLAYER_TIMER	 = 3000;

int DEMO_CAMERA_DIST0	= 5;
int DEMO_CAMERA_DIST1	= 8;
int DEMO_CAMERA_DZ	= 100;

int mch_demoModeWeights[DEMO_MODE_MAX] = { 12, 20, 28, 20, 20 };

mchDemoDispatcher mch_demoD;

void mchInitDemo(void)
{
	int i;
	char* p;
	XBuffer buf;

	DEMO_IDLE_TIMER	= atoi(getIniKey(mchIscreenINI,"demo","idle_timer"));
	DEMO_IDLE_TIMER_RND = atoi(getIniKey(mchIscreenINI,"demo","idle_timer_rnd"));
	DEMO_ROTATING_CAMERA_TIMER = atoi(getIniKey(mchIscreenINI,"demo","rotating_camera_timer"));
	DEMO_ROTATING_CAMERA_TIMER_RND = atoi(getIniKey(mchIscreenINI,"demo","rotating_camera_timer_rnd"));
	DEMO_MOVING_CAMERA_TIMER = atoi(getIniKey(mchIscreenINI,"demo","moving_camera_timer"));
	DEMO_MOVING_CAMERA_TIMER_RND = atoi(getIniKey(mchIscreenINI,"demo","moving_camera_timer_rnd"));
	DEMO_PLANET_VIEW_TIMER = atoi(getIniKey(mchIscreenINI,"demo","planet_view_timer"));
	DEMO_PLANET_VIEW_TIMER_RND = atoi(getIniKey(mchIscreenINI,"demo","planet_view_timer_rnd"));
	DEMO_CHANGE_PLAYER_TIMER = atoi(getIniKey(mchIscreenINI,"demo","change_player_timer"));
	DEMO_CAMERA_DIST0 = atoi(getIniKey(mchIscreenINI,"demo","camera_dist0"));
	DEMO_CAMERA_DIST1 = atoi(getIniKey(mchIscreenINI,"demo","camera_dist1"));
	DEMO_CAMERA_DZ = atoi(getIniKey(mchIscreenINI,"demo","camera_dz"));

	p = getIniKey(mchIscreenINI,"demo","mode_weights");
	buf.write(p,strlen(p));
	buf.set(0);

	for(i = 0; i < DEMO_MODE_MAX; i ++)
		buf >= mch_demoModeWeights[i];

	if(!mchWorldSelectFlag)
		mch_trackD -> GetRndTrack(mchCurrentWorld,mchCurrentTrack);

	mchPlayerRecorder = 1;
	mchEnableTrackDemo = 0;
	mchEnableStartCounter = 0;
	mch_demoD.enable(1);

	mchGameMode = MCH_SINGLE_GAME;
}

void mchFinitDemo(void)
{
	int num;

	if(!mch_demoD.is_enabled()) return;

	if(mch_iScreen){
		num = acsInitSaveSlots();
		if(num < 1) mch_iScreenID = ACS_MAIN_MENU_SCR0;
		if(num == 1) mch_iScreenID = ACS_MAIN_MENU_SCR1;
		if(num > 1) mch_iScreenID = ACS_MAIN_MENU_SCR2;
	}

	mchPlayerRecorder = 0;
	mchEnableTrackDemo = 1;
	mchEnableStartCounter = 1;
	mch_demoD.enable(0);

	mchFreeResourcesFlag = 1;
}

mchDemoDispatcher::mchDemoDispatcher(void)
{
	cur_mode = DEMO_IDLE;
	cur_mode_timer_max = get_timer(DEMO_IDLE_TIMER,DEMO_IDLE_TIMER_RND);
	cur_mode_timer_start = -1;

	enabled = 0;
}

mchDemoDispatcher::~mchDemoDispatcher(void)
{
}

void mchDemoDispatcher::set_mode(int mode,int timer)
{
	int i,cnt = 0;

	Vect3f r,r1;

	mchTrackElement* el0,*el1;

	if(mode == -1){
		mode = XRnd(100);
		for(i = 0; i < DEMO_MODE_MAX; i ++){
			if(mode >= cnt && mode < mch_demoModeWeights[i] + cnt){
				mode = i;
				break;
			}
			cnt += mch_demoModeWeights[i];
		}

		switch(mode){
			case DEMO_IDLE:
				timer = get_timer(DEMO_IDLE_TIMER,DEMO_IDLE_TIMER_RND);
				break;
			case DEMO_ROTATING_CAMERA:
				timer = get_timer(DEMO_ROTATING_CAMERA_TIMER,DEMO_ROTATING_CAMERA_TIMER_RND);
				break;
			case DEMO_MOVING_CAMERA:
				timer = get_timer(DEMO_MOVING_CAMERA_TIMER,DEMO_MOVING_CAMERA_TIMER_RND);
				break;
			case DEMO_PLANET_VIEW:
				timer = get_timer(DEMO_PLANET_VIEW_TIMER,DEMO_PLANET_VIEW_TIMER_RND);
				break;
			case DEMO_CHANGE_PLAYER:
				timer = DEMO_CHANGE_PLAYER_TIMER;
				break;
		}
	}

	if(cur_mode != mode){
		switch(mode){
			case DEMO_IDLE:
				if(cur_mode == DEMO_ROTATING_CAMERA){
					camera_dispatcher -> stopFinishScenario();
				}
				else {
					if(cur_mode == DEMO_PLANET_VIEW)
						camera_dispatcher -> setMode(CAM_MODE_SLOPE_ROTATE, 1);
					else {
						if(cur_mode == DEMO_MOVING_CAMERA)
							camera_dispatcher -> setObserver(mch_raceD -> activeRacer,0);
					}
				}
				break;
			case DEMO_ROTATING_CAMERA:
				camera_dispatcher -> runFinishScenario();
				break;
			case DEMO_MOVING_CAMERA:
				if(mch_raceD -> activeRacer -> last_trackP){
					el0 = mch_raceD -> AI_GetTrackEl(mch_raceD -> activeRacer -> last_trackP,-1,DEMO_CAMERA_DIST0,1,1);
					el1 = mch_raceD -> AI_GetTrackEl(el0,-1,DEMO_CAMERA_DIST1,1,1);

					r.x = el0 -> Cx;
					r.y = el0 -> Cy;
					r.z = el0 -> Cz;

					r1.x = el1 -> Cx;
					r1.y = el1 -> Cy;
					r1.z = el1 -> Cz + DEMO_CAMERA_DZ;

//					r = Vect3f(200,200,100);
//					r1 = Vect3f(200,1800,300);

					camera_dispatcher -> setCoords(r1,r);
				}
				break;
			case DEMO_PLANET_VIEW:
				camera_dispatcher -> setMode(CAM_MODE_FIXED_3D,1);
				break;
			case DEMO_CHANGE_PLAYER:
				mch_raceD -> nextRacer(0);
				break;
		}
	}

	cur_mode = mode;
	cur_mode_timer_start = mch_clock();
	cur_mode_timer_max = timer;
}

void mchDemoDispatcher::quant(void)
{
	int timer;

	if(!enabled) return;

	timer = mch_clock();

	if(cur_mode_timer_start == -1)
		cur_mode_timer_start = timer;

	if(timer - cur_mode_timer_start >= cur_mode_timer_max){
		switch(cur_mode){
			case DEMO_ROTATING_CAMERA:
			case DEMO_MOVING_CAMERA:
			case DEMO_PLANET_VIEW:
			case DEMO_CHANGE_PLAYER:
				set_mode(DEMO_IDLE,get_timer(DEMO_IDLE_TIMER,DEMO_IDLE_TIMER_RND));
				break;
			case DEMO_IDLE:
				set_mode(-1,0);
				break;
		}
	}
}

int mchDemoDispatcher::get_timer(int tm,int rnd) const
{
//	return (tm + XRnd(rnd) - rnd/2);

	int timer = (tm + XRnd(rnd) - rnd/2);

	if(timer < 0 || abs(timer - tm) > rnd * 2)
		ErrH.Exit();

	return timer;
}

