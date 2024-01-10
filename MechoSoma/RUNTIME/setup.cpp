
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "xsound.h"

#include "mechosoma.h"
#include "aci_parser.h"
#include "ACI_EVNT.H"
#include "ACI_IDS.H"
#include "ACI_SCR.H"
#include "sound.h"
#include "sound_api.h"

#include "iText.h"

#include "arcane_menu_d3d.h"

#include "Maths.h"
#include "Base.h"
#include "Mesh3ds.h"

#ifdef NETWORK
#include "wininet_api.h"
#include "online_game.h"
#endif

#define _MAX_PATH 1024
#define MAX_PATH 1024
#include "filesystem.h"
#include "port.h"

#include "CameraDispatcher.h"
#include "Xreal_utl.h"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int xgrInitModeSelected;
extern int xgrFullscreenMode;
extern int xgrColorDepth;
extern int mchResourcesFlag;

extern int acsMouseLPFlag;
extern int acsMouseRPFlag;

extern int xgrInitFlags;
extern int xgrGameMode;
extern int xgrGammaValue;
extern int mch_readonlyINI;

extern int mchEnableTips;
extern int mchEnableJoystick;
extern int mchEnableTrackDemo;
extern int mchTurnBasedGame;
extern int mchCamera3D;
extern int mch_iScreen;
extern int mch_ShowImages;
extern int mchAutoCenterCamera;
extern int mchEnableStartCounter;
extern int mchSplitScreenMode;
extern int mchSplitScreenBots;
extern int iwEnableIntroMovie;

extern int mchSkipMainMenu;

extern int mchFeedbackFlag;
extern int mchPBEM_DisableReturnFlag;

extern int mchNumRacers;
extern int mchNumRacersDef;
extern int mchPlayerRecorder;
extern int mchNumLaps;
extern int mchNumLapsDef;
extern int mchNumLapsDefHS;

extern int mchSoundMute;
extern int mchBotArcaneUsing;
extern int mchDefaultFigure;

extern int mch_RealRnd;

extern int mchSoundVolume;
extern int mchMusicVolume;

extern int mchUseContainer;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

extern int xINI_Enable;
extern int xINI_InitFlag;

extern int acsActiveFlag;
extern int acsPrepareFlag;
extern int iwEnableMovies;

extern int mchImgRTO_ActiveFlag;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

const char* win32_get_path_from_regkey(int key_id,const char* subkey_name,const char* value_name);

void acsCreateWater(void);
void mchReInitGraph(int mode);
void acsExtSetCamera(int zbuf_flag = 0);
void mchInitCamera(void);

void xINI_Init(void);
int xINI_PutKey(const char* fname,const char* section,const char* key,const char* val);
char* xINI_GetKey(const char* fname,const char* section,const char* key);

const char* getIniKey(const char* fname,const char* section,const char* key);
void putIniKey(const char* fname,const char* section,const char* key,const char* val);
void putIniKey(const char* fname,const char* section,const char* key,int val);

void acsSetMM_Option(int scr,int obj,int val);
void acsLockObject(int scr_id,int obj_id,int mode);

void mchSetup(void);
void mchCompileScriptM3D(void);
void mchCompileMainScript(void);
void mchOptionsSetup(void);
void mchUpdateOptions(void);

void mchA_d3dSetGamma(float v);

/* --------------------------- DEFINITION SECTION --------------------------- */

// HotSeat параметры
int hsStartAssembled = 1;
int hsTurnSeedsMax = 5;
int hsWaitTurnTime = 100;
int hsTurnPrepareTime = 100;
int hsTurnTime = 100;
int hsCameraLimit = 1;
int hsSeedsAccumulation = 0;
int hsReturnMax = 0;

int hsWorld = 2;
int hsTrack = 0;

int hsRndSeed = 8383;

mchOptionData mchOptions[ACS_NUM_OPTIONS];

char* fcPath = NULL;

int mchOptionObjIDs[ACS_NUM_OPTIONS] = 
{
	ACS_GR_SCREEN_RES1,
	ACS_GR_AUTO_VOXELS1,
	ACS_SND_EFFECTS1,
	ACS_SND_EFF_VOL1,
	ACS_SND_MUSIC1,
	ACS_SND_MUS_VOL1,
	ACS_ADV_AUTO_CENTER1,
	ACS_ADV_SCROLLING_MAX1,
	ACS_ADV_SCROLLING_ACC1,
	ACS_ADV_SCROLLING_DEC1,
	ACS_ADV_SCROLLING_MODE1,
	ACS_GR_GAMMA1,
	ACS_GR_SHADOWS1,
	ACS_GR_SKY1,
	ACS_GR_FURROWS1,
	ACS_GR_CHROME1,
	ACS_GR_COLOR_DEPTH1,
	ACS_GR_FULLSCR1
};

const char* mchOptionKeys[ACS_NUM_OPTIONS] =
{
	"resolution",
	"auto_3d",
	"sound",
	"sound_vol",
	"music",
	"music_vol",
	"auto_center",
	"scroll_speed",
	"scroll_acc",
	"scroll_dec",
	"camera_scrolling",
	"gamma",
	"shadows",
	"sky",
	"furrows",
	"chrome",
	"color_depth",
	"fullscreen"
};

hsPlayerData hsPlayers[HS_PLAYERS_MAX];
char mch_playerNames[15][HS_NAME_LEN + 2];

const char* getIniKey(const char* fname,const char* section,const char* key)
{
	char* p = NULL;

	static char buf[256];

	if(!xINI_InitFlag) xINI_Init();

	const auto path = file::normalize_path(fname);

	if(xINI_Enable){
		p = xINI_GetKey(path.c_str(),section,key);
		if(p)
			strcpy(buf,p);
		else
			*buf = 0;
	}

// TODO: @caiiiycuk invesitgate this
#ifdef WTF
	if(!p){
		if(!GetPrivateProfileString(section,key,NULL,buf,256,path))
			*buf = 0;
	}
#endif

	return buf;
}

void putIniKey(const char* fname,const char* section,const char* key,int val)
{
	static XBuffer str;

	str.init();
	str <= val;

	putIniKey(fname,section,key,str);
}

void putIniKey(const char* fname,const char* section,const char* key,const char* val)
{
	int flag = 0;
	static char path[_MAX_PATH];

	if(mch_readonlyINI) return;

	if(!xINI_InitFlag) xINI_Init();

	if(_fullpath(path,fname,_MAX_PATH) == NULL) ErrH.Abort("Invalid path...");  

	if(xINI_Enable)
		flag = xINI_PutKey(path,section,key,val);

// TODO: @caiiiycuk invesitgate this
#ifdef WTF
	if(!flag)
		WritePrivateProfileString(section,key,val,path);
#endif
}

void mchSetup(void)
{
	int flag;

	xINI_Init();

	const char* p = win32_get_path_from_regkey(1,"Software\\SamoGonki","SourcePath");
	if(p)
		fcPath = strdup(p);

	if(!xgrInitModeSelected){
		xgrFullscreenMode = atoi(getIniKey(mch_optionsINI,"current","fullscreen"));
		flag = atoi(getIniKey(mch_optionsINI,"current","color_depth"));
		xgrColorDepth = 16 + 16 * flag;
	}

#ifdef NETWORK
	wiServerPort = atoi(getIniKey(mch_mainINI,"online","server_port"));
	wiServerName = strdup(getIniKey(mch_mainINI,"online","server"));
	wiGameURL = strdup(getIniKey(mch_mainINI,"online","url"));
#endif

	mchPBEM_DisableReturnFlag = atoi(getIniKey(mch_mainINI,"online","disable_return"));

	mchFeedbackFlag = atoi(getIniKey(mch_mainINI,"general","feedback"));

	iwEnableMovies = atoi(getIniKey(mch_mainINI,"general","play_movies"));
	iwEnableIntroMovie = atoi(getIniKey(mch_mainINI,"general","intro_movie"));
	mchEnableTips = atoi(getIniKey(mch_mainINI,"general","tips"));
	mchEnableJoystick = atoi(getIniKey(mch_mainINI,"general","joystick"));

	if(mch_iScreen == -1)
		mch_iScreen = atoi(getIniKey(mch_mainINI,"general","main_menu"));
	
	if(mch_ShowImages == -1)
		mch_ShowImages = atoi(getIniKey(mch_mainINI,"general","show_images"));

	mchTurnBasedGame = atoi(getIniKey(mch_mainINI,"general","turn_based_game"));
	mchSplitScreenGame = atoi(getIniKey(mch_mainINI,"general","splitscreen_game"));
	flag = atoi(getIniKey(mch_mainINI,"general","hotseat_game"));
	if(flag){ 
		mchGameMode = MCH_ENTIRE_CONTROL_HS;
		mchTurnBasedGame = 1;
	}

	mchUseContainer = atoi(getIniKey(mch_mainINI,"general","resources"));

	if(mch_RealRnd == -1)
		mch_RealRnd = atoi(getIniKey(mch_mainINI,"general","real_rnd"));

	flag = atoi(getIniKey(mch_mainINI,"general","direct3d"));
	if(flag) xgrInitFlags |= DIRECT3D_HICOLOR;

	flag = atoi(getIniKey(mch_mainINI,"m3d","compile_script"));
	if(flag) mchCompileScriptM3D();

	flag = atoi(getIniKey(mch_mainINI,"race","compile_script"));
	if(flag) mchCompileMainScript();

#ifndef _FINAL_VERSION_
	mchNumRacers = mchNumRacersDef = atoi(getIniKey(mch_mainINI,"race","num_bots"));
	mchPlayerRecorder = atoi(getIniKey(mch_mainINI,"race","player_ai"));
	mchNumLaps = mchNumLapsDef = atoi(getIniKey(mch_mainINI,"race","num_laps"));
	mchBotArcaneUsing = atoi(getIniKey(mch_mainINI,"race","ai_arcanes"));
	mchDefaultFigure = atoi(getIniKey(mch_mainINI,"race","default_figure"));
	mchEnableStartCounter = atoi(getIniKey(mch_mainINI,"general","start_counter"));
	mchEnableTrackDemo = atoi(getIniKey(mch_mainINI,"general","track_demonstration"));

	mchSoundMute = 0;
	mchDebugMode = 0;
#else
	mchNumRacers = mchNumRacersDef = 4;
	mchPlayerRecorder = 0;
	mchNumLaps = mchNumLapsDef = 5;
	mchBotArcaneUsing = 1;
	mchDefaultFigure = -1;
	mchEnableStartCounter = 1;
	mchEnableTrackDemo = 1;

	mchSoundMute = !atoi(getIniKey(mch_mainINI,"general","sound"));
	mchDebugMode = atoi(getIniKey(mch_mainINI,"general","debug"));
#endif
	mchSplitScreenMode = atoi(getIniKey(mch_optionsINI,"split_screen","mode"));
	mchSplitScreenBots = atoi(getIniKey(mch_optionsINI,"split_screen","bots"));


	hsSetup();
	mchOptionsSetup();
}

void mchCompileScriptM3D(void)
{
	XBuffer XBuf;
	scrDataBlock* p;

	scrSetVerbose();
	p = parseScript("M3D\\SCRIPTS\\m3d.scr","M3D\\SCRIPTS\\m3d.def");
	saveScript("RESOURCE\\m3d.scb",p);
	delete p;

	scrSetVerbose();
	p = parseScript("M3D\\SCRIPTS\\m3d_eff.scr","M3D\\SCRIPTS\\m3d.def");
	saveScript("RESOURCE\\m3d_eff.scb",p);

	scrSetVerbose();
	p = parseScript("M3D\\SCRIPTS\\m3d_Xreal.scr","M3D\\SCRIPTS\\m3d.def");
	saveScript("RESOURCE\\m3d_Xreal.scb",p);
	delete p;
}

void mchCompileMainScript(void)
{
/*	XBuffer XBuf;
	scrDataBlock* p;

	scrSetVerbose();
	p = parseScript("RUNTIME\\SCRIPTS\\mechosoma.scr","RUNTIME\\SCRIPTS\\mch_script.def");

	saveScript("RESOURCE\\mechosoma.scb",p);
	delete p;*/
}

void hsSetup(void)
{
	int i,id;
	XBuffer XBuf,XBuf2;

	hsStartAssembled = atoi(getIniKey(mch_hotseatINI,"game parameters","start_assembled"));
	hsTurnSeedsMax = atoi(getIniKey(mch_hotseatINI,"game parameters","seeds_max"));
	hsTurnTime = atoi(getIniKey(mch_hotseatINI,"game parameters","turn_time"));
	hsTurnPrepareTime = atoi(getIniKey(mch_hotseatINI,"game parameters","turn_prepare_time"));
	hsWaitTurnTime = atoi(getIniKey(mch_hotseatINI,"game parameters","wait_turn_time"));
	hsCameraLimit = atoi(getIniKey(mch_hotseatINI,"game parameters","camera_limit"));
	hsSeedsAccumulation = atoi(getIniKey(mch_hotseatINI,"game parameters","seeds_accumulation"));;
	hsReturnMax = atoi(getIniKey(mch_hotseatINI,"game parameters","return_max"));
	hsWorld = atoi(getIniKey(mch_hotseatINI,"game parameters","world"));
	hsTrack = atoi(getIniKey(mch_hotseatINI,"game parameters","track"));

#ifndef _FINAL_VERSION_
	const char* s;
	if((s = check_command_line("hsWorld:")) != 0)
		hsWorld = atoi(s);
	if((s = check_command_line("hsTrack:")) != 0)
		hsTrack = atoi(s);
#endif

	for(i = 0; i < 15; i ++){
		cString p = iGetText(iTXT_NAME00 + i);
		if(strlen(p) > HS_NAME_LEN) p[HS_NAME_LEN] = 0;
		strcpy(mch_playerNames[i],p);
	}

	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		XBuf.init();
		XBuf < "player" <= i + 1;

		hsPlayers[i].SetName(getIniKey(mch_hotseatINI,XBuf.address(),"name"));

		if(strlen(hsPlayers[i].name) > HS_NAME_LEN) hsPlayers[i].name[HS_NAME_LEN] = 0;
		hsPlayers[i].type = atoi(getIniKey(mch_hotseatINI,XBuf.address(),"ai_type"));
		strcpy(hsPlayers[i].configStr,getIniKey(mch_hotseatINI,XBuf.address(),"mechos_config"));

		if(!strlen(hsPlayers[i].name)){
			XBuf2.init();
			XBuf2 < hsPlayers[i].configStr;
			XBuf2.set(0);

			XBuf2 >= id;

			strcpy(hsPlayers[i].name,mch_playerNames[id - 1]);
			putIniKey(mch_hotseatINI,XBuf.address(),"name",hsPlayers[i].name);
		}
	}

	if(!mchPBEM_Game && mchGameMode == MCH_ENTIRE_CONTROL_HS){
		mchCurrentWorld = hsWorld;
		mchCurrentTrack = hsTrack;
	}
}

void hsSaveParams(void)
{
	int i;
	XBuffer XBuf,plBuf(20);

	XBuf <= hsStartAssembled;
	putIniKey(mch_hotseatINI,"game parameters","start_assembled",(char*)XBuf);
	XBuf.init();

	XBuf <= hsTurnSeedsMax;
	putIniKey(mch_hotseatINI,"game parameters","seeds_max",(char*)XBuf);
	XBuf.init();

	XBuf <= hsTurnTime;
	putIniKey(mch_hotseatINI,"game parameters","turn_time",(char*)XBuf);
	XBuf.init();

	XBuf <= hsWaitTurnTime;
	putIniKey(mch_hotseatINI,"game parameters","wait_turn_time",(char*)XBuf);
	XBuf.init();

	XBuf <= hsCameraLimit;
	putIniKey(mch_hotseatINI,"game parameters","camera_limit",(char*)XBuf);
	XBuf.init();

	XBuf <= hsSeedsAccumulation;
	putIniKey(mch_hotseatINI,"game parameters","seeds_accumulation",(char*)XBuf);
	XBuf.init();

	XBuf <= hsReturnMax;
	putIniKey(mch_hotseatINI,"game parameters","return_max",(char*)XBuf);
	XBuf.init();

	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		plBuf < "player" <= i + 1;

		putIniKey(mch_hotseatINI,plBuf.address(),"name",hsPlayers[i].name);

		XBuf <= hsPlayers[i].type;
		putIniKey(mch_hotseatINI,plBuf.address(),"ai_type",(char*)XBuf);
		XBuf.init();

		putIniKey(mch_hotseatINI,plBuf.address(),"mechos_config",hsPlayers[i].configStr);
	}
}

void mchOptionData::Init(void)
{
	const char* p;
	LimitMin = 0;
	LimitMax = 255;
	Value = 128;

	if(!keyName) return;

	XBuffer XBuf;

	XBuf < keyName < "_min";
	LimitMin = atoi(getIniKey(mch_optionsINI,"default",(char*)XBuf));
	XBuf.init();

	XBuf < keyName < "_max";
	LimitMax = atoi(getIniKey(mch_optionsINI,"default",(char*)XBuf));
	XBuf.init();

	p = getIniKey(mch_optionsINI,"current",keyName);
	if(!strlen(p))
		p = getIniKey(mch_optionsINI,"default",keyName);
	Value = atoi(p);
}

void mchOptionData::Reset(void)
{
	Value = atoi(getIniKey(mch_optionsINI,"default",keyName));
}

void mchOptionData::Save(void)
{
	if(!keyName) return;

	XBuffer XBuf;
	XBuf <= Value;

	putIniKey(mch_optionsINI,"current",keyName,(char*)XBuf);
}

void mchOptionsSetup(void)
{
	int i;
	for(i = 0; i < ACS_NUM_OPTIONS; i ++){
		mchOptions[i].ID = i;
		mchOptions[i].SetKey(mchOptionKeys[i]);
		mchOptions[i].Init();
		mchOptions[i].Save();
	}
}

void mchUpdateOptions(void)
{
	int i;
	for(i = 0; i < ACS_NUM_OPTIONS; i ++)
		mchOptions[i].Update();
}

void mchOptionData::SetObjectValue(void)
{
	aciScreenObject* p = (aciScreenObject*)iScreenObj;
	aciScreenScroller* s;
	aciScreenInputField* f;

	if(!p) return;

	switch(p -> type){
		case ACS_INPUT_FIELD_OBJ:
			f = (aciScreenInputField*)p;
			f -> set_state(Value);
			break;
		case ACS_SCROLLER_OBJ:
			s = (aciScreenScroller*)p;
			s -> MaxValue = LimitMax - LimitMin;
			s -> CurValue = Value - LimitMin;
			s -> flags |= ACS_REDRAW_OBJECT;
			break;
	}
}

void mchOptionData::GetObjectValue(void)
{
	aciScreenObject* p = (aciScreenObject*)iScreenObj;
	aciScreenScroller* s;
	aciScreenInputField* f;

	if(!p) return;

	switch(p -> type){
		case ACS_INPUT_FIELD_OBJ:
			f = (aciScreenInputField*)p;
			Value = f -> CurState;
			break;
		case ACS_SCROLLER_OBJ:
			s = (aciScreenScroller*)p;
			Value = s -> CurValue + LimitMin;
			break;
	}
}

void mchOptionData::Update(void)
{
	int v;
	switch(ID){
		case ACS_OPT_SHADOWS:
			if(!Value){
				gb_IVisGeneric->ClearRenderObjectSwitch(RENDER_TUNING_SHADOW);
			}
			else {
				gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_SHADOW);
			}
			break;
		case ACS_OPT_SKY:
			if(!Value){
				gb_IVisGeneric->ClearRenderObjectSwitch(RENDER_TUNING_CLOUD);
			}
			else {
				gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_CLOUD);
			}
			break;
		case ACS_OPT_FURROWS:
			if(!Value){
				gb_IVisGeneric->ClearRenderObjectSwitch(RENDER_TUNING_BASETRAIL);
			}
			else {
				gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_BASETRAIL);
			}
			break;
		case ACS_OPT_CHROME:
			if(!Value){
				gb_IVisGeneric->ClearRenderObjectSwitch(RENDER_TUNING_METAL);
			}
			else {
				gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_METAL);
			}
			break;
		case ACS_OPT_COLOR_DEPTH:
			if(!xgrInitModeSelected) {
				if(mchA_d3dCheckMode(xgrGameMode,Value * 16 + 16)){
					xgrColorDepth = Value * 16 + 16;
				} else {
					Value = (xgrColorDepth - 16) >> 4;
					SetObjectValue();
				}
            }
			break;
		case ACS_OPT_FULLSCREEN:
			if(!xgrInitModeSelected)
				xgrFullscreenMode = Value;
			break;
		case ACS_OPT_RESOLUTION:
//			if(acsPrepareFlag && mch_ShowImages)
//				break;

			if(acsActiveFlag)
				xgrInitModeSelected = 0;

			if(!xgrInitModeSelected){
				if(mchA_d3dCheckMode(Value + 1,xgrColorDepth)){
					xgrGameMode = Value + 1;

					if(!acsPrepareFlag || !mch_ShowImages){
						mchReInitGraph(Value + 1);
						if(mch_iScreen && acsScrD)
							acsScrD -> ChangeCoords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);
						acsExtSetCamera();
						mchInitCamera();

						if(acsActiveFlag)
							acsCreateWater();

						acsMouseLPFlag = acsMouseRPFlag = 0;
					}
				}
				else {
					Value = xgrGameMode - 1;
					SetObjectValue();
				}
			}
			break;
		case ACS_OPT_AUTO_3D:
			break;
		case ACS_OPT_SOUND:
			v = mchSoundMute;
			mchSoundMute = !Value;
			if(mchSoundMute) mchStopSound();
			break;
		case ACS_OPT_SOUND_VOL:
			sndSetVolume(Value);
			break;
		case ACS_OPT_MUSIC:
			v = mchMusicMute;
			mchMusicMute = !Value;
			if(v != mchMusicMute){
				if(mchMusicMute) { 
					mchSetMusicMode(MCH_CD_HIDE);
				}
				else { 
					if(mchResourcesFlag || !mchSkipMainMenu)
						mchSetMusicMode(MCH_CD_SHOW);
				}
			}
			break;
		case ACS_OPT_MUSIC_VOL:
			sndMusicSetVolume(Value);
			mchMusicVolume = Value;
			break;
		case ACS_OPT_AUTO_CENTER:
			mchAutoCenterCamera = Value;
			break;
		case ACS_OPT_GAMMA:
			mchA_d3dSetGamma(((float)Value)/100.0f);
			xgrGammaValue = Value;
			break;
	}
	Save();
}

void hsOnlineSetup(void)
{
#ifdef NETWORK
	int i;
	ogPlayerInfo* p;

	XBuffer buf(30);

	if(og_inP.get_type() == OG_COMPLETED_GAME_PACKET)
		mchPBEM_SetFlag(PBEM_REPLAY_MODE);

	mchPBEM_PlayerUID = og_inP.owner_uid();

	hsTurnSeedsMax = og_inP.turn_seeds();
	hsTurnTime = 0;
	hsTurnPrepareTime = 0;
	hsWaitTurnTime = 0;
	hsSeedsAccumulation = 0;

	hsWorld = og_inP.world();
	hsTrack = og_inP.track();

	hsRndSeed = og_inP.rnd_seed();

	mchNumLaps = mchNumLapsDef = og_inP.num_laps();

	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		hsPlayers[i].type = 0;
	}

	for(i = 0; i < og_inP.num_players(); i ++){
		p = og_inP.get_player(i);
		hsPlayers[i].type = (p -> ROBOT == 'Y') ? 2 : 1;
		hsPlayers[i].SetName(p -> NIC);

		buf.init();

		buf <= p -> PERS_CAR_COMP_ID < " ";
		buf <= p -> FRONT_CAR_COMP_ID < " ";
		buf <= p -> FWHEEL_CAR_COMP_ID < " ";
		buf <= p -> BWHEEL_CAR_COMP_ID;

		strcpy(hsPlayers[i].configStr,buf.address());
	}

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
		mchCurrentWorld = hsWorld;
		mchCurrentTrack = hsTrack;
	}

	mchPBEM_CurPlayer = mchPBEM_CurPlayer0 = og_inP.get_player_id(mchPBEM_PlayerUID);

//	if(mchPBEM_CurPlayer == -1)
//		mchPBEM_CurPlayer = 0;
#endif
}

void cdCheck(char* f0,char* f1)
{
#ifdef CD_CHECK
	char* str = new char[MAX_PATH];
	char* label = new char[64];
	char* system = new char[64];

	int ret = 0;

	sprintf(str,"%s\\",fcPath);

	GetVolumeInformation(str,label,64,NULL,NULL,NULL,system,64);

	if(system[3] != 'S' || system[1] != 'D' || system[0] != 'C' || system[2] != 'F') ret = 0;
	if(ret) if(stricmp(label,iGetText(iTXT_CDLABEL))) ret = 0;

	if(ret){
		sprintf(str,"%s\\%s",fcPath,f0);
		DWORD a = GetFileAttributes(str);
		if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;

		if(ret){
			sprintf(str,"%s\\%s",fcPath,f1);
			DWORD a = GetFileAttributes(str);
			if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;
		}
	}

	if(!ret) ErrH.Abort(iGetText(iTXT_NOCD_MSG));

	delete str;
	delete label;
	delete system;
#endif
}
