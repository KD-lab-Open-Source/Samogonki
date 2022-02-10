
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"
#include "PlayWSC.h"
#include "WinVideo.h"

#include "iText.h"
#include "mch_rto.H"
#include "KEYS.H"
#include "controls.h"
#include "ctl_point.h"

#include "TERRA.H"
#include "TGA.H"

#include "Mesh3ds.h"
#include "IGraph3d.h"
#include "Xreal.h"

#include "race.h"
#include "intro.h"

#include "xsound.h"
#include "sound.h"
#include "sound_api.h"

#include "ACI_IDS.H"
#include "ACI_SCR.H"
#include "arcane_menu.h"
#include "arcane_menu_d3d.h"
#include "savegame.h"
#include "intro_movie.h"

#include "demo_dispatcher.h"

#include "I-World.h"
#include "LocalVersion.h"

#include "CameraDispatcher.h"
#include "TrackDispatcher.h"
#include "TrackRecorder.h"

#include "XJoystick.h"

#include "mechosoma.h"

#ifdef _WIN32
#include "win32f.h"
#endif

#include "ResourceDispatcher.h"
#include "Xreal_utl.h"
#include "TexMgr.h"
#include "GameClient.h"
#include "Params.h"
#include "Statistics.h"
#include "AllocationTracking.h"

#include "PlayMpeg.h"

#include "wininet_api.h"
#include "online_game.h"

#include "cdcheck.h"

#include "mch_common.h" // For far target

#ifndef _WIN32
#define TRUE 1
#define FALSE 0
#define MAX_PATH 1024

#define MK_LBUTTON 0x0001
#define MK_RBUTTON 0x0002

#define DBGCHECK

#include "Md3d.h"

#endif

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern vrtMap* vMap;
extern int mchPlayerRecorder;
extern int mchNumRacers;
extern int mchNumRacersDef;
extern int mchNumLaps;
extern int mchNumLapsDef;
extern int mchNumLapsDefHS;
extern int mchBotArcaneUsing;
extern int acsActiveFlag;
extern int acsMouseLPFlag;
extern int acsMouseRPFlag;
extern int acsLoadFlag;
extern int mchTurn;
extern int iwElectionaryFlag;
extern int iwEnableIntroMovie;

extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];

extern int fxlabStopTime;

extern xtList<mchRacerStats> mch_racerLst;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchGraphicsSetup(void);

void mchInitOutro(void);
void mchChangeTimeSpeed(int dir);

void mchA_EnableTimeSpeedShow(void);
void mchA_PrepareLoadingImage(int wrld,int tr);
void mchA_DrawOnlineScreen(void);
void mchRecorderPrepare(void);
void mchRecorderDone(void);
void acsBackupControls(void);
void acsRestoreControls(void* buf = NULL);
void* acsGetControlsBackupBuf(int& sz);
void* mchSaveGameParams(int& sz);
void mchLoadGameParams(void* p,int sz);
void mchRestoreGameParams(void);
int acsInitSaveSlots(void);

void mchInitApplication(void);
void mchReInitGraph(int mode);

void mchPBEM_Init(void);
void mchPBEM_LoadingQuant(void);

void acsLoadGame(void);
void acsCreateWater(void);
void acsExtSetCamera(int zbuf_flag = 0);
void mchInitCamera(void);
void mchSetNextQuantRTO(void);

void acsJoyHandler(eXJoystickState state, int button_or_axis_value, int joystick_ID);
void BaseObject_SurfaceReflection_Pos(Vect3f &Pos,Vect3f &ReflectionPos);

void mchRestore(void);
void mchInitWorldsPrm(void);
void mchInitRacers(void);

void mchLoadTrack(void);
void mchFreeTrack(void);

void mchInitTrackDemo(void);

void mchShowImageMousePress(int fl, int x, int y);
void mchShowImageKeyPress(int k);

void mchEndGameFinit(int mode = 0);
void mchA_d3dPrepare(void);
void mchA_ShowStartCountEnable(void);
void mchA_ShowStartCountDisable(void);

void mchGlobalQuant(void);
void mchScreenShot(int mode = 0);

void mchSetup(void);
void mchComline(void);
void mchKeyTrap(void);

void mchWorldsInit(void);

void mchFPS_Init(void);
void mchFPS_Quant(void);

int mchMouse3D_Visible(void);
void mchInitMouse(void);
void mchMouseMove(int fl,int x,int y);
void mchMouseLP(int fl,int x,int y);
void mchMouseLU(int fl,int x,int y);
void mchMouseRP(int fl,int x,int y);
void mchMouseRU(int fl,int x,int y);

void mchMouseQuant3D(int fl,int x,int y);
void mchLightCPoint(int& x,int& y,int& z);

void mchInitMouse3D(void);
void mchShowMouse3D(void);
void mchSetMouse3D(const Vect3f& pos);
void mchGetMouseCoords3D(int& x,int& y,int& z);

void mchSetControlPoint(void);
void mchDelControlPoint(void);
void mchDelControlPointAll(void);

void mchInitLoadTimer(void);
void mchFinitLoadTimer(void);
void mchSetLoadTimeMax(unsigned v);
void mchLoadTimerFunc(unsigned sz);

void acsPrepare(void);
void acsInit(void);
int acsQuant(void);
void acsFinit(void);
void acsInitMouse(void);
void acsClearStack(void);
void acsExtInitPrm(void);
void acsInitFont3D(void);
void acsInitOptions(void);
void acsJoyQuant(void);

void acsMouseMove(int fl,int x,int y);

void mchFreeResources(int mode = 0);

void acsLoadFonts(void);

void mchPrepareA_MenuPal(void);
void mchA_InitNames(void);

void mchUpdateOptions(void);

void resource_dispatcher_init();

void fxlabOpen(void);
void fxlabClose(void);
void fxlabCreateController(void);
void fxlabClientClear(void);

void fxlabInit(void);
void fxlabFinit(void);

void fxlabServerQuant(void);
void fxlabClientQuant(void);
void fxlabClientShow(void);

unsigned short* loadJPG(XStream *Xfile, int &IMGwidth, int &IMGheight);

void mchInitLogs(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

//#define _ONLINE_TEST_
//#define _MCH_INIT_LOGS_

unsigned int XRndVal = 83838383;

int mchTimeMode = MCH_TIME_STOPPED;
int mchGameMode = MCH_SINGLE_GAME;
int mchGameFlags = 0;
int mchExitOnFinishFlag = 0;

int mchGraphicsSetupFlag = 0;

int mchTurnBasedGame = 0;
int mchSplitScreenGame = 0;
int mchPBEM_Game = 0;
int mchPBEM_GameID = 0;
int mchPBEM_PlayerUID = 0;
int mchPBEM_DataFlag = 0;
int mchPBEM_ConnectFlag = 0;
int mchPBEM_GameMode = MCH_PBEM_EDIT_TURN;

int mchLaunchOnlineFlag = 0;

int mchPBEM_CurPlayer = 0;
int mchPBEM_CurPlayer0 = 0;
int mchPBEM_TargetTurn = 0;
int mchPBEM_HiddenTime = 0;
int mchPBEM_ViewEntireGame = 0;
int mchPBEM_Pause = 0;
int mchPBEM_EndFlag = 0;
int mchPBEM_SaveFlag = 0;
int mchPBEM_DisableReturnFlag = 0;
int mchPBEM_Flags = 0;

char* mchPBEM_PacketFile = NULL;
char mchPBEM_PacketPath[MAX_PATH];

int mchHS_LoadFlag = 0;
int mchHS_SaveFlag = 0;

int mchImgRTO_ActiveFlag = 0;

int ErrHExcept = 1;
int pause;

int xgrInitFlags = XGR_EXCLUSIVE | XGR_HICOLOR;
int xgrInitMode = MCH_800x600;		// startup videomode
int xgrGameMode = MCH_800x600;		// in-game videomode

int xgrFullscreenMode = 1;
int xgrColorDepth = 16;

int mchOpenNewChar = 0;

int xgrInitModeSelected = 0;
int xgrGammaValue = 100;
GameQuantRTO* quantRTO = NULL;

int mchRealClockStart = 0;
int mchRealClockEnd = 0;
int mchRealClockShow = 0;

mchGameWindow* gameWnd;			// fullscreen window
mchGameWindow* gameWnd0;		// player1 window
mchGameWindow* gameWnd1;		// player2 window

mchGameWindow* iWnd;			// fullscreen interface window
mchGameWindow* iWnd0;			// player1 interface window
mchGameWindow* iWnd1;			// player2 interface window

char* mapFName = "worlds.prm";
char* mchWorldPrm = "worlds.prm";

const int mchFPS_Period = 50;
static char mchFPS_String[100];
int mchFPS_FrameCount = 0;
int mchFPS_StartTime = -1;
int mchFPS_Log = 0;
float mchFPS_Value = -1.0f;

int mch_iScreen = -1;			// Main Menu ON/OFF...
int mch_ShowImages = -1;		// Show Logos ON/OFF...
int mch_RealRnd = -1;

int mchCurrentWorld = 1;
int mchCurrentTrack = 0;

int mchNextWorld = 1;
int mchNextTrack = 0;

int mchFreezeTime = 0;

int mchSelectedWorld = 0;
int mchWorldsFlag = 0;
int mchWorldSelectFlag = 0;
int mchTrackSelectFlag = 0;
int DrawMask=0xFFFFFFFF;

// 2 - New Single Game
// 3 - New Split-Control HotSeat Game
// 4 - New Entire-Control HotSeat Game
// 5 - Continue Game
int mchNewGameMode = 2;
int mchResourcesFlag = 0;

int mchEnableTips = 0;
int mchEnableTrackDemo = 0;
int mchEnableStartCounter = 0;
int mchAutoCenterCamera = 1;
int mchCenteringFlag = 0;
int mchSplitScreenMode = 0; // 0 - V, 1 - H

int mchFreeResourcesFlag = 0;

int mchLeftMousePress = 0;
int mchRightMousePress = 0;

int mch_iScreenID = -1;

int mchFeedbackFlag = 0;

int mchLoadingRTO_NextID = -1;

int mchShowImageMouseFlag = 0;
int mchShowImageKeyFlag = 0;
ShowImageRTO* mch_imageRTO = NULL;
LoadingRTO* mch_loadingRTO = NULL;
sWinVideo mch_imgVideoObj;

int idOS,inHighPriority;

int mchXRecorderMode = 0;
char* mchXRecorderName = NULL;

unsigned mchFrameNumber = 0;

int mchCamera3D = 0;
int mchCameraSlope = 1;

int mchTrackDemoMode = 0;
int mchTrackDemoStartTimer = 0;
int mchTrackDemoCurTimer = 0;

int mchEnableJoystick = 0;
int mchDetectJoystick = 0;

int mch_readonlyINI = 0;
char* mch_mainINI = "game.ini";
char* mch_hotseatINI = "RESOURCE/hotseat.ini";
char* mch_optionsINI = "RESOURCE/options.ini";

int mchDebugMode = 0;
int mchDemoMode = 0;
int mchSkipMainMenu = 0;
int mchScreenShotMode = 0;
int mchScreenShotLetter = 97;
int mchScreenShotCount = 0;
int mch_ChangeRacer = 0;

// For Direct3D
int mch_d3dDebugMode = FALSE;
int mch_d3dAltDevice = FALSE;

int mchMaxFPS = RTO_GAME_QUANT_TIMER;

int setTimerToProfile = 0;

SystemResourceDispatcher resource_dispatcher;

cInterfaceVisGeneric	*gb_IVisGeneric=0;		// интерфейс для работы с библиотекой вывода
cInterfaceGraph3d		*gb_IGraph3d=0;			// интерфейс для работы графикой
cUnknownClass			*gb_URenderDevice=0;	// устройство вывода
cUnknownClass			*gb_UScene=0;			// текущая активная сцена
cUnknownClass			*iCamera=0;			// интерфейсная камера

#ifdef _MCH_LOG_
XStream __mch_logFile;
#endif

char* mchWndTitle = " ";

float mchTimeSpeedCur = 1.0f;
float mchTimeSpeedMin = 0.33f;
float mchTimeSpeedMax = 3.0f;
float mchTimeSpeedDelta = 1.05f;

int xtInitApplication(void)
{
	allocation_tracking("Start");
	ErrH.SetRestore(mchRestore);
	CONTROL_FP();
	typedef void (*PV)();
	SetAssertRestoreGraphicsFunction((PV)&d3dFlipToGdiSurface);
#ifndef _FINAL_VERSION_	
	if(check_command_line("dont_sleep"))
		d3dSetFocusLossBehavior(0);
#endif
							
	mchComline();
/*
	xt_get_cpuid();
	xt_getMMXstatus();

	XCon < "\n" < getOS(idOS) < " detected...\n";
*/
	lvInit();

	if(mchGraphicsSetupFlag){
		mchGraphicsSetup();
	}

	if(mchPBEM_Game)
		wiInit();
#ifdef _WIN32
	if(inHighPriority)
		win32_SetPriorityProcess(HIGH_PRIORITY_CLASS);
	else
		win32_SetPriorityProcess(NORMAL_PRIORITY_CLASS);
#endif
//	XCon < "MMX support ";
//	if(xt_mmxUse) XCon < "detected";
//	else XCon < "is absent";
//	XCon < "...\n";
//	XCon < "Process priority: " <= win32_GetProcessPriority() < "\n";
	
	mchPrepareSound();
	mchInitControls();

	iTextInit();

//	if(!win32_SingleCheck())
//		ErrH.Abort(iGetText(iTXT_EXE_ACTIVE));

#ifdef _FINAL_VERSION_
	mchWndTitle = iGetText(iTXT_GAME_TITLE);
#else
	char wnd_title[256];
	sprintf(wnd_title," %s / %s %s",iGetText(iTXT_GAME_TITLE),__DATE__,__TIME__);

	mchWndTitle = strdup(wnd_title);
#endif

	mchSetup();

	if(mch_RealRnd) XRndVal = clock();

	mchXResourceInit();
	allocation_tracking("mchXResourceInit");

	GameQuantRTO* gPtr = new GameQuantRTO;
	gPtr -> SetTimer(mchMaxFPS);

	MainMenuRTO* mPtr = new MainMenuRTO;
	mch_imageRTO = new ShowImageRTO;
	mch_loadingRTO = new LoadingRTO;
	IntroMovieRTO* iPtr = new IntroMovieRTO;
	iPtr -> SetNext(RTO_LOADING_ID);
//	iPtr -> SetNext(-1);

	xtCreateRuntimeObjectTable(RTO_MAX_ID);

	xtRegisterRuntimeObject(gPtr);
	xtRegisterRuntimeObject(mPtr);
	xtRegisterRuntimeObject(iPtr);
	xtRegisterRuntimeObject(mch_imageRTO);
	xtRegisterRuntimeObject(mch_loadingRTO);

	mch_imageRTO -> SetNumFiles(2);
	mch_imageRTO -> SetFlag(0,IMG_RTO_INTRO_IMAGE | IMG_RTO_NO_IMAGE);
//	mch_imageRTO -> SetName("INTRO\\1c_logo.jpg",1);
	mch_imageRTO -> SetName("INTRO/splash.jpg",1);
	mch_imageRTO -> SetFlag(1,IMG_RTO_START_MUSIC);

	quantRTO = gPtr;
	quantRTO -> NextID = 0;

#ifndef _ONLINE_TEST_
	mch_loadingRTO -> NextID = quantRTO -> ID;
#else
	mch_loadingRTO -> NextID = mchLoadingRTO_NextID;
#endif

	int dwGraphMode,dwScrX,dwScrY;
	switch(xgrInitMode)
	{
		case MCH_320x200:
			dwScrX=320,dwScrY=200;
			break;
		case MCH_640x480:
			dwScrX=640,dwScrY=480;
			break;
		case MCH_800x600:
			dwScrX=800,dwScrY=600;
			break;
		case MCH_1024x768:
			dwScrX=1024,dwScrY=768;
			break;
		case MCH_1152x864:
			dwScrX=1152,dwScrY=864;
			break;
		case MCH_1280x1024:
			dwScrX=1280,dwScrY=1024;
			break;
		case MCH_1600x1200:
			dwScrX=1600,dwScrY=1200;
			break;
	}

	if(xgrInitFlags&DIRECT3D_HICOLOR){
		dwGraphMode = 1;
		RenderMode = DIRECT3D_HICOLOR;
		XGR_Init(dwScrX,dwScrY,xgrInitFlags);
	}
	else {
		dwGraphMode = 0;
		RenderMode = XGRAPH_HICOLOR;
		XGR_Init(dwScrX,dwScrY,xgrInitFlags);
	}

	gb_IVisGeneric=CreateIVisGeneric();
	gb_URenderDevice=gb_IVisGeneric->CreateGraph(dwScrX,dwScrY,dwGraphMode,xgrFullscreenMode,xgrColorDepth);
	gb_IGraph3d=gb_IVisGeneric->GetIGraph3d(gb_URenderDevice);

	{
		sRectangle4f r1(dwScrX*0.001f,dwScrX*0.001f,dwScrX*0.999f,dwScrY*0.999f);
		gb_IVisGeneric->SetGraphClipping(gb_URenderDevice,&r1);
	}
	

#ifndef _DEBUG
#ifdef _WIN32
	FreeConsole();
#endif
#endif

	allocation_tracking("xgrInit");

	CONTROL_FP();

	gb_UScene=gb_IVisGeneric->CreateScene();
	gb_IVisGeneric->SetScene(gb_UScene);			// установка активной сцены

	iCamera=gb_IVisGeneric->CreateCamera();

	{
		Vect3f v1(0,0,512);
		Vect3f v2(0,0,0);
		Vect3f v3(0,0,512);
		gb_IVisGeneric->SetCameraPosition(iCamera,&v1,&v2,&v3);
	}

	{
		Vect2f v1(0.5f,0.5f);
		sRectangle4f r1(-0.499f,-0.499f,0.499f,0.499f);
		Vect2f v2(1.0f,1.0f);
		Vect2f v3(10.0f,3000.0f);
		Vect2f v4(0.2f,0.90f);
		gb_IVisGeneric->SetCameraFrustum(iCamera,	// устанавливается пирамида видимости
			&v1, // центр камеры
			&r1, // видимая область камеры
			&v2, // фокус камеры
			&v3, // ближайший и дальний z-плоскости отсечения
			&v4); // zNear и zFar для мапирования в zBuffer
	}
	
	gb_IVisGeneric->AttachCameraViewPort(iCamera,gb_URenderDevice);

	gameWnd = new mchGameWindow;
	gameWnd0 = new mchGameWindow;
	gameWnd1 = new mchGameWindow;

	iWnd = new mchGameWindow;
	iWnd0 = new mchGameWindow;
	iWnd1 = new mchGameWindow;

	cdCheck_Silent(-1);

	mchDetectJoystick = XJoystickInit();

	if(!mchDetectJoystick) 
		mchEnableJoystick = 0;
	else
		XJoystickSetHandler(&acsJoyHandler);

	allocation_tracking("XJoystickInit");

	mchInitSound();
	allocation_tracking("mchInitSound");
				     
	CONTROL_FP();

	if(mchOpenNewChar)
		gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_n.scb");
	else
		gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d.scb");

	gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_eff.scb");
	gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_Xreal.scb");
	gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_font.scb");
	gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_font_add.scb");
	gb_IVisGeneric->LoadObjectLibrary("RESOURCE/m3d_iscreen.scb");
	allocation_tracking("LoadObjectLibrary");

	camera_dispatcher = new CameraDispatcher(gb_URenderDevice);
	camera_dispatcher2 = new CameraDispatcher(gb_URenderDevice);

	mchInitCamera();
//	iTextInit();

	if(mch_iScreen){
		allocation_tracking("acsPrepare() beg");
		acsPrepare();
		allocation_tracking("acsPrepare() end");
	}
	else {
		XGR_MouseInit(XGR_MAXX/2,XGR_MAXY/2,0,0,1,NULL);
		acsInitOptions();
		acsLoadFonts();
	}

	cdCheck("Resource\\Forest0\\output.vmp","autorun.exe");

	mchInitArcaneScreen();

	mchInitLogs();

	return RTO_SHOW_IMAGE_ID;
}

void xtDoneApplication(void)
{	
	mchFreeResources();
	fxlabFinit();
	if(gb_IVisGeneric)
	{
		gb_IVisGeneric->ReleaseWorld(); // выгрузка мира
		if(camera_dispatcher){
			delete camera_dispatcher;
			camera_dispatcher = 0;
			}
		if(camera_dispatcher2){
			delete camera_dispatcher2;
			camera_dispatcher2 = 0;
			}
		if(iCamera)	gb_IVisGeneric->ReleaseCamera(iCamera); // удаление ненужной камеры
		if(gb_UScene) gb_IVisGeneric->ReleaseScene(gb_UScene); // удаление сцены
		gb_IVisGeneric->ReleaseGraph(gb_URenderDevice); // закрытие окна вывода
		gb_IVisGeneric->Release();	// закрытие библиотеки
		gb_UScene=0; gb_URenderDevice=0; gb_IGraph3d=0; gb_IVisGeneric=0;
	}
	if(!mchMusicMute) sndMusicStop();
	mchFinitSound();

#ifdef _WIN32
#ifdef _FINAL_VERSION_
	if(mchFeedbackFlag)
		win32_shell_execute(iGetText(iTXT_MAILTO));
#endif
	if(mchPBEM_Game){
		wiFinit();

		if(!mchPBEM_DisableReturnFlag && og_inP.return_url())
			win32_shell_execute(og_inP.return_url());
	}

	if(mchLaunchOnlineFlag){
		if(strlen(iGetText(iTXT_ONLINE_URL)))
			win32_shell_execute(iGetText(iTXT_ONLINE_URL));
	}
#endif
}

void GameQuantRTO::Init(int id)
{
	CONTROL_FP();
	int resources_flag = mchResourcesFlag;
	NextID = 0;

	if(mchXRecorderMode){
		xtSysQuantDisable(1);
		mchRecorderPrepare();
	}

	if(!mchResourcesFlag){
		cdCheck("Resource\\Forest5\\output.vmp","Resource\\Lava2\\output.vmp");

		if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
			mchTurnBasedGame = 1;

			if(!mchPBEM_Game)
				mchNumLaps = mchNumLapsDefHS;
		}

		if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
			XRndVal = hsRndSeed;
			if(!mchPBEM_Game){
				mchCurrentWorld = hsWorld;
				mchCurrentTrack = hsTrack;
			}
		}

		mchNextWorld = 7;
		mchNextTrack = 0;

		mchGameFlags = 0;

		mchResourcesFlag = 1;
		gb_IGraph3d->Fill(0,0,0);
		gb_IVisGeneric->ReleaseObject(0xFFFFFFFF,0xFFFFFFFF);

		if(!mchMusicMute) mchSetMusicMode(MCH_CD_STOPPED);

		mchInitLoadTimer();

		global_time.setTime(1);
		non_stop_global_time.setTime(1);
		xm_random_generator.set(1);
		ArcaneStatisticsService::setCurrentArcane(ArcaneStatisticsInfo());
				
		mchWorldsInit();

		iwInit();

		if(acsLoadFlag)
			acsLoadGame();

		iworld_D -> MovieQuant();

		camera_dispatcher -> clear();
		if(mchSplitScreenGame && camera_dispatcher2) camera_dispatcher2 -> clear();

		Xreal_init();

		mchLoadTimerFunc(8000);

		allocation_tracking("before fxlabInit");
		fxlabOpen();
		allocation_tracking("fxlabInit");

		mchLoadTimerFunc(9100);

		mchInitMouse3D();
		mchInitRaceDispatcher();

		fxlabCreateController();

		mchLoadTimerFunc(9200);

		mch_arcScrD -> prepare();
		mch_arcScrD -> ToggleVisible(1);

		cdCheck_Silent(-1);

		mchLoadTimerFunc(9300);

		if(iWorldMode){
			iwInitWorld();

			iworld_D -> SetOwner(mch_raceD -> activeRacer);
			iworld_D -> InitWorld();
			if(mchSplitScreenGame){
				iworld_D2 -> SetOwner(mch_raceD -> activeRacer2);
				iworld_D2 -> InitWorld();
			}
		}
		else {
			if(iworld_D -> TurnbasedMode())
				mchTurnBasedGame = 1;

			iworld_D -> ClearPhrases();
		}

		mchLoadTimerFunc(10000);

  		mchFinitLoadTimer();

		if(!mch_iScreen) KBD_init();

		if(mchGameMode == MCH_SINGLE_GAME){
			if(mchTurnBasedGame){
				mchA_ShowStartCountDisable();
				mchSetTimeMode(MCH_TIME_STOPPED);

				if(mchEnableStartCounter)
					camera_dispatcher -> runTurnBasedStartScenario();

				mch_arcScrD -> InitStartupScreen();
				mch_arcScrD -> InitTurnBasedScreen();

				if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
			}
			else {
				if(!iWorldMode && mchEnableTrackDemo){
					mchInitTrackDemo();
				}
				else {
					if(!iWorldMode && mchEnableStartCounter){
						mch_raceD -> SetBrake(1);
						mchSetTimeMode(MCH_TIME_STOPPED);
						mchA_SetTimer(4);
						mchA_ShowStartCountEnable();
						camera_dispatcher -> runStartScenario();
						if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
					}
					else {
						mchSetTimeMode(MCH_TIME_RUNNING);
						camera_dispatcher -> setMode(CAM_MODE_SLOPE_ROTATE);
						if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
					}
				}
				mch_arcScrD -> InitStartupScreen();
			}
		}
		if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
			mchA_ShowStartCountDisable();
/*
			mchSetTimeMode(MCH_TIME_RUNNING);
			mchGameFlags |= MCH_STOP_TIME_FLAG;
*/
			mchSetTimeMode(MCH_TIME_WAITING_TURN);

			mch_arcScrD -> InitStartupScreen();
			mch_arcScrD -> InitTurnBasedScreen();

			if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
		}

		mchSoundStartup();
		mchA_TimerCur = 0;

		resource_dispatcher.reset();
		mchA_SetGammaSeq(-1,GS_HIDE_LOADING_IMAGE);

		mchRealClockStart = clocki();
	}
	else 
		if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);

	mchInitMouse();

	if(mchNewGameMode == 5 && mchTimeMode == MCH_TIME_RUNNING) mchResumeSound();
	mchA_TimerStart = non_stop_global_time() - mchA_TimerCur;
	mchTrackDemoStartTimer = mchTrackDemoCurTimer = mch_clock();
	
	allocation_tracking("GameQuant Init");

#ifdef _RT_PROFILER
	XBuffer header;
	header < "World: " < vMap -> wTable[vMap -> cWorld].name < "\n";
	header < "Racers: " <= mchNumRacers < "\n";
//	profiler_start_auto_shoot(header);
	allocation_tracking_message(header);
#endif
	
#ifndef _FINAL_VERSION_
	const char* dir_str = check_command_line("hide_evolve:");
	if(dir_str){
		mch_sndD -> Disable(1);
		resource_dispatcher.evolve(atoi(dir_str)*1000, 1);
		append_message_to_file("OK", "diff");
		ErrH.Exit();
		}
#endif

	if(!mchPBEM_Game || !resources_flag)
		resource_dispatcher.start();
	else
		resource_dispatcher.skip_time();

	if(mchHS_LoadFlag && !resources_flag)
		mchPBEM_Init();

	XGR_MouseSetMoveHandler(mchMouseMove);
}

int GameQuantRTO::Quant(void)
{
	static int is_active = -1;

	if(is_active != d3dIsActive()){
		XKey.clear();
		KeyBuf -> clear();
	}

	is_active = d3dIsActive();

	if(mchA_isGammaSeqActive() && mchA_GetGammaSeqMode() == GS_HIDE_LOADING_IMAGE){
//		gb_IGraph3d->BeginScene();
		if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_HiddenTime){
			mchA_EndGammaSeq();
		}
		else {
			mchA_GammaSeqQuant();
			gb_IGraph3d->EndScene();
			gb_IGraph3d->Flush();

			return NextID;
		}
	}

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_HiddenTime){
		mchPBEM_LoadingQuant();
		fxlabClientClear();
		return NextID;
	}

	create_timer(Out_of_GameQuant, STAT_TOTAL);
	stop_timer(Out_of_GameQuant, STAT_TOTAL);

	if(!mchXRecorderMode)
		mchMouseQuant3D(0,XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2,XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2);

	start_timer(mchGlobalQuant, STAT_TOTAL);
	acsJoyQuant();

	if(pause == 1) pause = 2;
	else if(pause){	mchKeyTrap(); return NextID; }

	mchA_d3dPrepare();
//	mch_arcScrD -> TimerQuant();

	stop_timer(mchGlobalQuant, STAT_TOTAL);
	
	start_timer(Sound_Quant, STAT_TOTAL);
	mchSoundQuant();
	stop_timer(Sound_Quant, STAT_TOTAL);

	start_timer(IVG_Fill, STAT_TOTAL);
	gb_IGraph3d->Fill(0,0,0);
	stop_timer(IVG_Fill, STAT_TOTAL);

	/////////////////////////////////////
	//  Logical calculations	
	start_timer(physics, STAT_TOTAL);

	resource_dispatcher.quant();

	if(!mchXRecorderMode && iWorldMode){
		iworld_D -> Quant();
		if(mchSplitScreenGame)
			iworld_D2 -> Quant();
	}
	stop_timer(physics, STAT_TOTAL);

	fxlabStopTime = 0;

	start_timer(fxlabClientQuant, STAT_TOTAL);
	fxlabClientQuant();
	stop_timer(fxlabClientQuant, STAT_TOTAL);

	mch_demoD.quant();

	start_timer(IVG_Draw, STAT_TOTAL);
// действия до начала сцены (в частности кэширование)
	gb_IVisGeneric->SetTime(global_time());
	TestWorldScriptPlay.Animate();

	if(iWorldMode){
		if(iworld_D -> PostMovieQuant()){
			return NextID;
		}
	}

	gb_IVisGeneric->PreDraw(DrawMask&0x7FFFFFFF);

//////////////////////////////////////// НАЧАЛО СЦЕНЫ /////////////////////////////////////////
	gb_IGraph3d->BeginScene();
///////////////////////////////////////////////////////////////////////////////////////////////

	gb_IVisGeneric->Draw(DrawMask);	
	stop_timer(IVG_Draw, STAT_TOTAL);

	start_timer(fxlabClientShow, STAT_TOTAL);
	fxlabClientShow();
	stop_timer(fxlabClientShow, STAT_TOTAL);

	start_timer(mch_raceD, STAT_TOTAL);
	mch_raceD -> showStats(); 

	mch_raceD -> redraw();    
	stop_timer(mch_raceD, STAT_TOTAL);

	start_timer(mch_arcScrD, STAT_TOTAL);
	mch_arcScrD -> Quant(); 

	mch_arcScrD -> Redraw(); 

	if(mch_trkRec)
		mch_trkRec -> Redraw();

	if(mchFPS_Log) mchFPS_Quant(); 
	stop_timer(mch_arcScrD, STAT_TOTAL);

	start_timer(Flush, STAT_TOTAL);
	Xreal_draw_handler();
	TestWorldScriptPlay.Draw();

	if(mchA_isGammaSeqActive()){
		mchA_GammaSeqQuant();
/*
		if(iWorldMode && iworld_D -> CheckFlag(IW_EXIT_FLAG) && !mchA_isGammaSeqActive()){
			mchFreeResourcesFlag = 1;
			mchSetNextQuantRTO();
		}
*/
	}

///////////////////////////////////////// КОНЕЦ СЦЕНЫ /////////////////////////////////////////
	gb_IGraph3d->EndScene();
	gb_IGraph3d->Flush();
///////////////////////////////////////////////////////////////////////////////////////////////

	mch_arcScrD -> Flush();

	if(mchScreenShotMode)
		mchScreenShot(1);
// прочие действия после отрисовки сцены
	gb_IVisGeneric->PostDraw(0x7FFFFFFF);

	if(!mchXRecorderMode)
		mchKeyTrap();

	stop_timer(Flush, STAT_TOTAL);

	start_created_timer(Out_of_GameQuant, STAT_TOTAL);

	return NextID;
}

void GameQuantRTO::Finit(void)
{
	if(mchXRecorderMode)
		mchRecorderDone();

	if(NextID != RTO_LOADING_ID)
		mchFinitDemo();

	xtSysQuantDisable(0);

//	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchPBEM_SaveFlag)
//		sgD -> save("game.dat");

	if(mch_iScreen)
		XGR_MouseSetMoveHandler(acsMouseMove);
	else
		XGR_MouseSetMoveHandler(mchMouseMove);

	mchPauseSound();

//	profiler_write_log();
	allocation_tracking_message("Leave World");

	if(mchFreeResourcesFlag){
		mchFreeResources(1);
		iWorldMode = 0;
		mchFreeResourcesFlag = 0;
	}
	else {
		if(mchTimeMode == MCH_TIME_GAME_OVER && (!mchPBEM_Game || NextID == ID)){
			if(mchGameMode == MCH_SINGLE_GAME)
				mchFreeResources(1);
			else
				mchFreeResources();
		}
	}
}

void MainMenuRTO::Init(int id)
{
	mchInitApplication();

	acsCreateWater();

	if(mchDemoMode)
		mchInitDemo();

	if(mch_iScreen){
		mchPrepareA_MenuPal();

		if(mch_iScreenID == -1){
			if(id == RTO_GAME_QUANT_ID){
				if(mchGameMode == MCH_SINGLE_GAME)
					acsScrD -> curScrID = (iWorldMode) ? ACS_MAIN_MENU_SCR4 : ACS_MAIN_MENU_SCR3;
				else {
					if(!mchPBEM_Game)
						acsScrD -> curScrID = ACS_MAIN_MENU_SCR5;
					else
						acsScrD -> curScrID = ACS_MAIN_MENU_SCR6;
				}
			}
			else
				acsScrD -> curScrID = ACS_MAIN_MENU_DUMMY_SCR0;
		}
		else
			acsScrD -> curScrID = mch_iScreenID;

		acsScrD -> SetFlush();

		allocation_tracking("acsInit() beg");
		acsInit();
		allocation_tracking("acsInit() end");

		XGR_MouseShow();
		mch_iScreenID = -1;
	}
}

int MainMenuRTO::Quant(void)
{
	int ret;
	if(mch_iScreen){
		if(mchSkipMainMenu)
			ret = (mchPBEM_Game) ? 4 : 2;
		else
			ret = acsQuant();

		if(ret > 1){
			mchNewGameMode = ret;
			if(mchNewGameMode == 2)
				mchGameMode = MCH_SINGLE_GAME;
			if(mchNewGameMode == 3)
				mchGameMode = MCH_SPLIT_CONTROL_HS;
			if(mchNewGameMode == 4)
				mchGameMode = MCH_ENTIRE_CONTROL_HS;

			if(!mchResourcesFlag){
				if(iwElectionaryFlag && !mchSplitScreenGame && mchGameMode == MCH_SINGLE_GAME && !acsLoadFlag && iwEnableIntroMovie && !mchPlayerRecorder)
					return RTO_INTRO_MOVIE_ID;

				return RTO_LOADING_ID;
			}

			return RTO_GAME_QUANT_ID;
		}
		if(ret == 1){
			mch_imageRTO -> SetNumFiles(1);
//			mch_imageRTO -> SetName("RESOURCE\\ISCREEN\\IMG\\img01.hbm",0);

			mch_imageRTO -> ClearFlag(0);
			mch_imageRTO -> SetText(iGetText(iTXT_CONTACTS),1,1,0,0);
			mch_imageRTO -> SetFlag(0,IMG_RTO_NO_IMAGE | IMG_RTO_START_MUSIC);
			mch_imageRTO -> SetNext(XT_TERMINATE_ID);
			mch_imageRTO -> WaitTimer = RTO_SHOW_IMAGE_TIME * 3;

			return RTO_SHOW_IMAGE_ID;
		}
		if(ret == -1){
			mchLaunchOnlineFlag = 1;
			return XT_TERMINATE_ID;
		}

		return 0;
	}
	return RTO_LOADING_ID;

//	return RTO_GAME_QUANT_ID;
}

void MainMenuRTO::Finit(void)
{
	if(mch_iScreen){ 
		if(mchSkipMainMenu){
			mchDemoMode = mchSkipMainMenu = 0;
			acsActiveFlag = 0;
			return;
		}

		acsFinit();

		if(RenderMode == XGRAPH_HICOLOR){
			gb_IGraph3d->Fill(0,0,0);
			gb_IGraph3d->Flush();
			xtClearMessageQueue();
		}
		else {
			if(!mchResourcesFlag){
				mchA_d3dClear(0);
				mchA_d3dFlip();
				mchA_d3dClear(0);
			}
		}
	}
}

void mchComline(void)
{
#ifdef _WIN32
	int i,num = __argc;
	char** p = __argv;

	XBuffer str;

	for(i = 0; i < num; i ++){
		if(*p[i] == '-' || *p[i] == '/'){
			switch(p[i][1]){
				case 'n':
					if(!stricmp(p[i] + 1,"newchar"))
						mchOpenNewChar = 1;
				case 'N':
					break;
				case 'm':
				case 'M':
					if(!strnicmp(p[i] + 1,"maxfps",strlen("maxfps"))){
						if(strlen(p[i] + strlen("maxfps") + 1))
							mchMaxFPS = round(1000.0f / (float)atoi(p[i] + strlen("maxfps") + 1));
						else 
							mchMaxFPS = 0;

						XCon < "Max FPS = " <= mchMaxFPS < "\n";
					}
					break;
				case 'D':
				case 'd':
					if(!stricmp(p[i] + 1,"demo")){
						mchSkipMainMenu = mchDemoMode = 1;
						break;
					}
					if(p[i][2] == 'W' || p[i][2] == 'w'){
						mchXRecorderMode = XRC_RECORD_MODE;
						mchXRecorderName = strdup(p[i] + 3);
						mch_iScreen = 0;
					}
					else if(p[i][2] == 'P' || p[i][2] == 'p'){
						mchXRecorderMode = XRC_PLAY_MODE;
						mchXRecorderName = strdup(p[i] + 3);
						mch_iScreen = 0;
					}
					break;
				case '0':
					xgrGameMode = MCH_320x200;
					xgrInitModeSelected = 1;
					break;
				case '1':
					xgrGameMode = MCH_640x480;
					xgrInitModeSelected = 1;
					break;
				case '2':
					xgrGameMode = MCH_800x600;
					xgrInitModeSelected = 1;
					break;
				case '3':
					xgrGameMode = MCH_1024x768;
					xgrInitModeSelected = 1;
					break;
				case '4':
					xgrGameMode = MCH_1152x864;
					xgrInitModeSelected = 1;
					break;
				case '5':
					xgrGameMode = MCH_1280x1024;
					xgrInitModeSelected = 1;
					break;
				case '6':
					xgrGameMode = MCH_1600x1200;
					xgrInitModeSelected = 1;
					break;
				case 'e':
				case 'E':
					if(!stricmp(p[i] + 1, "end_turn"))
						mchPBEM_GameMode = MCH_PBEM_END_TURN;
					else
						if(!stricmp(p[i] + 1, "edit_turn"))
							mchPBEM_GameMode = MCH_PBEM_EDIT_TURN;
						else
							if(!stricmp(p[i] + 1, "exit_on_finish"))
								mchExitOnFinishFlag = 1;
							else
								xgrColorDepth = 32;
					break;
				case 't':
				case 'T':
					mchCurrentTrack = atoi(p[i] + 2);
					mchTrackSelectFlag = 1;
					break;
				case 'a':
				case 'A':
					xgrFullscreenMode = 0;
					xgrInitModeSelected = 1;
					break;
				case 'w':
				case 'W':
					mchCurrentWorld = mchSelectedWorld = atoi(p[i] + 2);
					mchWorldSelectFlag = 1;
					break;
				case 'H':
				case 'h':
					if(!stricmp(p[i] + 1,"hotseat")){
						mchGameMode = MCH_ENTIRE_CONTROL_HS;
						mch_iScreen = 0;
					}
					else 
						inHighPriority = 1 - inHighPriority;
					break;
				case 'S':
				case 's':
					if(!stricmp(p[i] + 1,"skipmenu")){
						mchSkipMainMenu = 1;
					}
					if(!stricmp(p[i] + 1,"savegame"))
						mchHS_SaveFlag = 1;
					if(!stricmp(p[i] + 1,"showtime"))
						mchRealClockShow = 1;
					break;
				case 'C':
				case 'c':
					mch_ChangeRacer = 1;
					break;
				case 'L':
				case 'l':
					if(!stricmp(p[i] + 1,"loadgame"))
						mchHS_LoadFlag = 1;

					if(!stricmp(p[i] + 1,"LOWMEM")){
						acsLowMemFlag = 1;
					}
					break;
				case 'I':
				case 'i':
					str < p[i] + 2 < "\\game.ini";
					mch_mainINI = strdup(str.address());

					str.init();
					str < p[i] + 2 < "\\hotseat.ini";
					mch_hotseatINI = strdup(str.address());

					str.init();
					str < p[i] + 2 < "\\options.ini";
					mch_optionsINI = strdup(str.address());
					break;
				case 'R':
				case 'r':
					if(!stricmp(p[i] + 1,"race_log"))
						mchRaceLogFlag = 1;
					else
						XRndVal = clock();
					break;
				case 'v':
				case 'V':
					if(!stricmp(p[i] + 1, "vsetup"))
						mchGraphicsSetupFlag = 1;
					if(!stricmp(p[i] + 1, "view_turn"))
						mchPBEM_GameMode = MCH_PBEM_VIEW_TURN;
					break;
				case 'o':
				case 'O':
					if(!stricmp(p[i] + 1, "online")){
						mchGameMode = MCH_ENTIRE_CONTROL_HS;
						mchHS_LoadFlag = 1;
						mch_iScreen = 1;
						mch_ShowImages = 0;
						mchPBEM_Game = 1;
						mchPBEM_DataFlag = 1;
						mchSkipMainMenu = 1;
					}
					break;
				case 'g':
				case 'G':
					if(!stricmp(p[i] + 1, "game") && i < num - 1)
						mchPBEM_GameID = atoi(p[i + 1]);
					break;
				case 'P':
				case 'p':
					if(!stricmp(p[i] + 1, "path") && i < num - 1)
						SetCurrentDirectory(p[i + 1]);

					if(!stricmp(p[i] + 1, "player") && i < num - 1)
						mchPBEM_PlayerUID = atoi(p[i + 1]);

					if(!stricmp(p[i] + 1, "packet") && i < num - 1){
						mchPBEM_PacketFile = strdup(p[i + 1]);
						GetCurrentDirectory(MAX_PATH,mchPBEM_PacketPath);

						if(sgCheckPacket(mchPBEM_PacketFile))
							mchLoadingRTO_NextID = RTO_GAME_QUANT_ID;

						if(mchPBEM_Game)
							sgGetWorldID(mchPBEM_PacketFile);
					}
#ifndef _FINAL_VERSION_
					if(!stricmp(p[i] + 1, "profile")){
						XCon < "Set timer mode for profiling\n";
						setTimerToProfile = 1;
						}
#endif
					break;
			}
		}
	}
#ifndef _FINAL_VERSION_
	if(check_command_line("hide_evolve:")){
		xgrGameMode = MCH_640x480;
		xgrInitModeSelected = 1;
		//xgrInitFlags &= ~XGR_EXCLUSIVE;
		mch_iScreen = 0;
		mch_ShowImages = 0;
		//mch_RealRnd = 0;
		}
#endif
#endif
}

void mchKeyTrap(void)
{
	int k,sz,id;

//	if(mchEnableJoystick)
//		XJoystickQuant();

	if(iWorldMode && TestWorldScriptPlay.isPlay()){
		sz = KeyBuf -> size;
		while(KeyBuf -> size){
			k = KeyBuf -> get();
			id = mchGetKeyID_First(k);

			if(k == VK_SPACE || k == VK_ESCAPE || k == VK_RETURN){
				TestWorldScriptPlay.CloseWorldScript();
				KeyBuf -> clear();
				return;
			}
			if(k == VK_F12 && !XKey.Pressed(VK_CONTROL))
				mchScreenShot();
		}
		return;
	}

	if(mchTrackDemoMode){
		mchTrackDemoCurTimer = mch_clock();

		sz = KeyBuf -> size;
		while(KeyBuf -> size){
			k = KeyBuf -> get();
			id = mchGetKeyID_First(k);

			while(id != -1){
				if(id == MCH_KEY_EXIT){
					if(mch_iScreen)
						quantRTO -> NextID = RTO_MAIN_MENU_ID;
					else
						quantRTO -> NextID = XT_TERMINATE_ID;
					KeyBuf -> clear();
					return;
				}
				id = mchGetKeyID_Next();
			}
		}
		if(mchTrackDemoCurTimer > mchTrackDemoStartTimer + 1000 && sz)
			mch_arcScrD -> FinitTrackDemoScreen();
		return;
	}
	
	if(mch_demoD.is_enabled()){
		sz = KeyBuf -> size;
		while(KeyBuf -> size){
			k = KeyBuf -> get();
			if(k == VK_SPACE || k == VK_ESCAPE || k == VK_RETURN){
				if(mch_iScreen)
					quantRTO -> NextID = RTO_MAIN_MENU_ID;
				else
					quantRTO -> NextID = XT_TERMINATE_ID;
			}
			if(k == VK_F12 && !XKey.Pressed(VK_CONTROL))
				mchScreenShot();
		}
		return;
	}

	if(mchTimeMode == MCH_TIME_GAME_OVER){
		sz = KeyBuf -> size;
		while(KeyBuf -> size){
			k = KeyBuf -> get();

			id = mchGetKeyID_First(k);

			while(id != -1){
				if(id == MCH_KEY_EXIT){
					if(!mchPBEM_Game){
						if(mchGameMode != MCH_SINGLE_GAME || mchSplitScreenGame){
							if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
								mch_iScreenID = ACS_SPLIT_HS_SCR;
								mchNextWorld = mchCurrentWorld;
								mchNextTrack = mchCurrentTrack;
							}

							mchEndGameFinit(1);

							KeyBuf -> clear();
							return;
						}
					}
					else {
						if(mch_iScreen)
							quantRTO -> NextID = RTO_MAIN_MENU_ID;
						else
							quantRTO -> NextID = XT_TERMINATE_ID;

						KeyBuf -> clear();
						return;
					}
				}
				if(id == MCH_KEY_REPLAY)
					mch_raceD -> InitReplay(0);
				if(id == MCH_KEY_REPLAY_ALL)
					mch_raceD -> InitReplay(1);

				id = mchGetKeyID_Next();
			}

			if(k == VK_MOUSE_LBUTTON){
				mch_arcScrD -> MouseHandler(0);
			}
			else {
				if(k == VK_MOUSE_RBUTTON)
					mch_arcScrD -> MouseHandler(1);
				else
					mch_arcScrD -> KeyHandler(k,id);
			}
		}
		return;
	}

	if(iWorldMode)
		iwKeyTrap();

	mch_raceD -> KeyTrap();

	while(KeyBuf -> size){
		k = KeyBuf -> get();
		Xreal_key_handler(k);

		mchA_CHQuant(k);

		id = mchGetKeyID_First(k);
		
		while(id != -1){
			switch(id){
				case MCH_KEY_EXIT:
					if(mch_iScreen)
						quantRTO -> NextID = RTO_MAIN_MENU_ID;
					else
						quantRTO -> NextID = XT_TERMINATE_ID;
					break;
				case MCH_KEY_STATUS_STRING:
					mch_arcScrD -> ToggleVisible();
					break;
				case MCH_KEY_SLOW_TIME:
					if(mchTurnBasedGame && mchTimeMode == MCH_TIME_RUNNING){
						mchChangeTimeSpeed(-1);
						mchA_EnableTimeSpeedShow();
					}
					break;
				case MCH_KEY_FAST_TIME:
					if(mchTurnBasedGame && mchTimeMode == MCH_TIME_RUNNING){
						mchChangeTimeSpeed(1);
						mchA_EnableTimeSpeedShow();
					}
					break;
			}
			id = mchGetKeyID_Next();
		}

		switch(k){
#ifndef _FINAL_VERSION_
			case VK_F10:
				if(mchTimeMode == MCH_TIME_RUNNING)
					mch_raceD -> nextRacer();
				break;
			case VK_F11:
				mch_raceD -> AI_Toggle();
				break;
#endif
			case VK_F9:
				mchFPS_Init();
				break;
			case VK_F12:
				if(XKey.Pressed(VK_CONTROL))
					camera_dispatcher -> setFreeDirectionFly();
				else
					mchScreenShot();
				break;
			case VK_MOUSE_LBUTTON:
				if(!mchPBEM_Pause){
					if(!mch_AMenu -> Active())
						mchSetControlPoint();
					else 
						mch_arcScrD -> MouseHandler(0);
				}
				break;
			case VK_MOUSE_RBUTTON:
/*
				if(!mchPBEM_Pause){
					if(mchTimeMode == MCH_TIME_STOPPED && mch_raceD -> activeRacer -> mouseSeed){
						mch_raceD -> activeRacer -> RemoveSelectedSeed();
					}
					else 
						mch_arcScrD -> MouseHandler(1);
				}
*/
				break;
#ifndef _FINAL_VERSION_
			case VK_PAUSE:
				if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
					if(!mchPBEM_Pause){
						mchGameFlags |= MCH_STOP_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
						mchPBEM_SetFlag(PBEM_CHANGE_PLAYER);
					}
					else
						mchGameFlags |= MCH_START_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
				}
				else {
					if(mchFreezeTime){
						mchFreezeTime = 0;
						mchResumeSound();
					}
					else {
						mchFreezeTime = 1;
						mchPauseSound();
					}
				}

/*
				if(XKey.Pressed(VK_SHIFT))
					if(pause == 2) pause = 1;
				else
					if(pause){
						pause = 0;
						mchResumeSound();
					}
					else {
						pause = 2;
						mchPauseSound();
					}
*/
				break;	
/*
			case 'A': // только отраженные объекты 
				if(DrawMask&0x0000000F)
					DrawMask&=~0x0000000F;
				else
					DrawMask|=0x0000000F; 
				break;
			case 'S': // глобальные не прозрачные объекты мира
				if(DrawMask&0x000000F0)
					DrawMask&=~0x000000F0; 
				else
					DrawMask|=0x000000F0;
				break;
			case 'D': // динамические не прозрачные объекты
				if(DrawMask&0x00000F00)
					DrawMask&=~0x00000F00;
				else
					DrawMask|=0x00000F00;
				break;
			case 'F': // глобальные прозрачные объекты мира
				if(DrawMask&0x0000F000)
					DrawMask&=~0x0000F000;
				else
					DrawMask|=0x0000F000;
				break;
			case 'G': // установка цвета экрана под водой
				if(DrawMask&0x00F00000)
					DrawMask&=~0x00F00000;
				else
					DrawMask|=0x00F00000;
				break;
			case 'K': // установка цвета экрана под водой
				extern float gb_LodValue;
				gb_LodValue*=0.5f;
				break;
			case 'J': // установка цвета экрана под водой
				extern float gb_LodValue;
				gb_LodValue*=2.0f;
				break;
*/
			case 'T':

				break;
			case 'A':
				extern int gb_InitKeyMode;
				if(gb_InitKeyMode)
				{
					TestWorldScriptPlay.SetCameraDispatcher(camera_dispatcher);
					TestWorldScriptPlay.SetVisGeneric(gb_IVisGeneric);
					TestWorldScriptPlay.OpenWorldScript((char*)"resource\\WorldScript\\LavaBeginWS.scb");
				}
				break;
			case 'S':
				if(gb_InitKeyMode){
					TestWorldScriptPlay.CloseWorldScript();
					camera_dispatcher -> setObserver(0);
					}
				break;
			case VK_MULTIPLY:
				mchScreenShotMode ^= 1;
				if(!mchScreenShotMode){
					mchScreenShotLetter ++;
					mchScreenShotCount = 0;
				}
				break;
#endif
		}
	}
}

void mchWorldsInit(void)
{
	allocation_tracking("mchWorldsInit() beg");

	if(!mchWorldsFlag){
		vMapPrepare(mchWorldPrm,mchCurrentWorld,mchCurrentTrack);
		mchWorldsFlag = 1;
	}
	else {
		mchFreeTrack();
		vMap -> reload(mchCurrentWorld,mchCurrentTrack);
	}

	mchLoadTimerFunc(2000);
	DBGCHECK;

	vMap -> load3Buf(0,0,0);//mchCurrentTrack
	//if(RenderMode!=DIRECT3D_HICOLOR)
	//	WorldRender();
	//RenderShadovM3DAll();	

	mchLoadTimerFunc(4000);

	vMap -> loadTrack(0);//mchCurrentTrack

	// создание полигонального мира, воды, неба и т.д.
	gb_IVisGeneric->CreateWorld("RESOURCE\\world.scb",mchCurrentWorld,mchCurrentTrack);

	mchLoadTrack();
	mchFreeWorldSound();
	mchLoadWorldSound();

	allocation_tracking("mchWorldsInit() end");

	mchLoadTimerFunc(6000);
}

void mchFPS_Init(void)
{
	mchFPS_Log = ++mchFPS_Log % 2;
	mchFPS_FrameCount = 0;
	mchFPS_StartTime = clock();
	if(!mchFPS_Log) mchFPS_Value = -1.0f;
}

void mchFPS_Quant(void)
{
	float x,y,z;
	if(++mchFPS_FrameCount == mchFPS_Period){
		mchFPS_Value = (float)mchFPS_Period/(float)(((int)clock() - (int)mchFPS_StartTime))*1000.0f;
		mchFPS_FrameCount = 0;
		mchFPS_StartTime = clock();
	}

	x = mch_raceD -> activeRacer -> R().x;
	y = mch_raceD -> activeRacer -> R().y;
	z = mch_raceD -> activeRacer -> R().z;

	if(mchFPS_Value >= 0.0f)
		sprintf(mchFPS_String,"%.1f fps [%.1f %.1f %.1f]",(float)mchFPS_Value,x,y,z);
	else
		sprintf(mchFPS_String,"fps init [%.1f %.1f %.1f]",x,y,z);

	char* str = mchFPS_String;

	if(RenderMode == XGRAPH_HICOLOR)
		XGR_OutText(gameWnd -> PosX + 5,gameWnd -> PosY + 5,0xFFFF,str,1,0,0,0);
	else
		mchA_d3dOutString(gameWnd -> PosX + 5,gameWnd -> PosY + 5,0.8f,0.8f,str,mchA_ColorF[6],160,2,0);
}

void mchInitMouse(void)
{
	if(!mch_iScreen)
		acsInitMouse();

	XGR_MouseSetPressHandler(XGM_LEFT_BUTTON,mchMouseLP);
	XGR_MouseSetUnPressHandler(XGM_LEFT_BUTTON,mchMouseLU);

	XGR_MouseSetPressHandler(XGM_RIGHT_BUTTON,mchMouseRP);
	XGR_MouseSetUnPressHandler(XGM_RIGHT_BUTTON,mchMouseRU);
}

void mchMouseMove(int fl,int x,int y)
{
	if(fl & MK_LBUTTON){
		XGR_MouseObj.lBt.Pressed = 1;
		mchLeftMousePress = 1;
	}
	else {
		XGR_MouseObj.lBt.Pressed = 0;
		mchLeftMousePress = 0;
	}

	if(fl & MK_RBUTTON){
		XGR_MouseObj.rBt.Pressed = 1;
		mchRightMousePress = 1;
	}
	else {
		XGR_MouseObj.rBt.Pressed = 0;
		mchRightMousePress = 0;
	}
}

void mchMouseLP(int fl,int x,int y)
{
	KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_PRESSED);
	mchLeftMousePress = 1;
}

void mchMouseLU(int fl,int x,int y)
{
//	KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_UNPRESSED);
	mchLeftMousePress = 0;
}

void mchMouseRP(int fl,int x,int y)
{
	KeyBuf -> put(VK_MOUSE_RBUTTON,KBD_CUR_KEY_PRESSED);
	mchRightMousePress = 1;
}

void mchMouseRU(int fl,int x,int y)
{
//	KeyBuf -> put(VK_MOUSE_RBUTTON,KBD_CUR_KEY_UNPRESSED);
	mchRightMousePress = 0;
}

void mchMouseQuant3D(int fl,int x,int y)
{
	Vect3f mg = camera_dispatcher -> Screen2World(x,y);
	mchSetMouse3D(mg);

	if(mchTurnBasedGame && mchMouse3D_Visible()){
		if(mch_raceD -> activeRacer -> mouseSeed){
			mchControlPoint* p = mch_raceD -> activeRacer -> mouseSeed;
			Vect3f pv,pe;
			camera_dispatcher -> ConvertorWorldToViewPort(Vect3f(p->PosX,p->PosY,p->PosZ), pv, pe);
			if(!gameWnd -> check(pe.xi(), pe.yi()))
				mch_raceD -> activeRacer -> mouseSeed = NULL;
			}
		else
			mchSetMouse3D(mg);

		int mx = mg.xi();
		int my = mg.yi();
		int mz = mg.zi();
		int mx0 = mx;
		int my0 = my;
		int mz0 = mz;

		if(!mchPBEM_Pause && mch_raceD -> activeRacer -> mouseSeed && mchLeftMousePress){
			mch_raceD -> activeRacer -> MoveSeed(mx,my,mz);
			mch_raceD -> activeRacer -> curSeed = mch_raceD -> activeRacer -> mouseSeed;
			mchLightCPoint(mx,my,mz);
			mchSetMouse3D(mg);
		}
		else {
			mchLightCPoint(mx,my,mz);
			mchSetMouse3D(mg);
		}
/*
		if(mx0 != mx || my0 != my || mz0 != mz){
			ConvertorPoint3d(mx,my,mz,&x,&y);
			if(gameWnd -> check(x,y))
				XGR_MouseInitPos(x - XGR_MouseObj.SizeX/2,y - XGR_MouseObj.SizeY/2);
		}
*/

	}

	if(mchTimeMode == MCH_TIME_GAME_OVER)
		XGR_MouseShow();
	else
		XGR_MouseHide();
}

void mchSetControlPoint(void)
{

	int mx,my,mz,tp = 0;

	if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE))
		return;

	if(mch_SMenu -> Active() && mch_SMenu -> CheckMouse()){
		mch_SMenu -> MouseHandler(0);
		return;
	}

	if(!mch_raceD -> activeRacer -> mouseSeed){
		if(mchMouse3D_Visible() && mch_raceD -> activeRacer -> CheckMouse()){
			mchGetMouseCoords3D(mx,my,mz);
			if(mchKeyPressed(MCH_KEY_TS_MOUSE) && !mchKeyPressed(MCH_KEY_ARCANE_MENU) && !mchKeyPressed(MCH_KEY_MOUSE_ARCANE_MENU)){
				tp = 1;
			}
			mchStartSoundEFF(EFF_SEED,mx,my);
			mch_raceD -> activeRacer -> AddSeed(Vect3f(mx,my,mz + 10), tp);
		}
	}
	else {
		mch_raceD -> activeRacer -> curSeed = mch_raceD -> activeRacer -> mouseSeed;
	}
}

void mchDelControlPoint(void)
{
	mch_raceD -> activeRacer -> RemoveLastSeed();
}

void mchDelControlPointAll(void)
{
	mch_raceD -> activeRacer -> RemoveSeeds();
}

void mchGameWindow::set(int cx,int cy,int sx,int sy)
{
	CenterX = cx;
	CenterY = cy;

	SizeX = sx;
	SizeY = sy;

	SizeX2 = SizeX/2;
	SizeY2 = SizeY/2;
}

void mchGameWindow::set(mchGameWindow* p)
{
	flags = p -> flags;

	SizeX = p -> SizeX;
	SizeY = p -> SizeY;

	CenterX = p -> CenterX;
	CenterY = p -> CenterY;

	SizeX2 = SizeX/2;
	SizeY2 = SizeY/2;

	PosX = p -> PosX;
	PosY = p -> PosY;
}

void mchGameWindow::init(int mode)
{
	int sx,sy,delta = 0;

	if(mode != -1)
		flags = mode;

	if(flags & MCH_WND_640x480){
		sx = 640;
		sy = 480;
	}
	else {
		sx = XGR_MAXX;
		sy = XGR_MAXY;

		delta = 2;
	}

	if(flags & MCH_WND_FULLSCREEN){
		CenterX = sx/2;
		CenterY = sy/2;

		SizeX = sx;
		SizeY = sy;
	}
	if(flags & MCH_WND_ALIGN_LEFT){
		CenterX = sx/4;
		CenterY = sy/2;

		SizeX = sx/2 - delta;
		SizeY = sy;
	}
	if(flags & MCH_WND_ALIGN_RIGHT){
		CenterX = sx/4 + sx/2;
		CenterY = sy/2;

		SizeX = sx/2 - delta;
		SizeY = sy;
	}
	if(flags & MCH_WND_ALIGN_TOP){
		CenterX = sx/2;
		CenterY = sy/4;

		SizeX = sx;
		SizeY = sy/2 - delta;
	}
	if(flags & MCH_WND_ALIGN_BOTTOM){
		CenterX = sx/2;
		CenterY = sy/4 + sy/2;

		SizeX = sx;
		SizeY = sy/2 - delta;
	}
	if(flags & MCH_WND_CENTERED){
		CenterX = sx/2;
		CenterY = sy/2;
	}
	SizeX2 = SizeX/2;
	SizeY2 = SizeY/2;

	PosX = CenterX - SizeX2;
	PosY = CenterY - SizeY2;
}

void mchGameWindow::set_clipping(void)
{
	if(RenderMode==DIRECT3D_HICOLOR) return;
	XGR_SetClipMode(XGR_CLIP_ALL);
	XGR_SetClip(PosX,PosY,PosX + SizeX - 1,PosY + SizeY - 1);
}

void mchGameWindow::remove_clipping(void)
{
	if(RenderMode==DIRECT3D_HICOLOR) return;
	XGR_SetClipMode(XGR_CLIP_PUTSPR);
	XGR_SetClip(0,0,XGR_MAXX,XGR_MAXY);
}

void mchScreenShot(int mode)
{
	int x,y,index,r,g,b,xg;
	XBuffer fname,ID;
	tgaFile* tgaPtr;
	unsigned char* buf;
	unsigned short* xbuf;

	index = 0;
	xg = (XGR_MASK_R == XGR_MASK_R0) ? 1 : 0;

#ifndef _WRLD_SCREENSHOTS_
	if(!mode){
		fname < "shot";
	}
	else {
		fname < "scr_" < (char)mchScreenShotLetter;
	}

	if(mchScreenShotCount < 10) fname < "0";
	if(mchScreenShotCount < 100) fname < "0";
	if(mchScreenShotCount < 1000) fname < "0";
	fname <= mchScreenShotCount < ".tga";
#else
	char str[256];
	sprintf(str,"s%.2d_%.2d.tga",mchCurrentWorld,mchCurrentTrack);
	fname < str;
#endif

	ID < mchWndTitle < " (c) 2001 by K-D Lab\n";
	ID < "Compilation: DATE: " < __DATE__ < " TIME: " < __TIME__ < "\n";

	buf = new unsigned char[XGR_MAXX * XGR_MAXY * 3];
	if(RenderMode == XGRAPH_HICOLOR){
		xbuf = (unsigned short*)XGR_VIDEOBUF;

		for(y = 0; y < XGR_MAXY; y ++){
			for(x = 0; x < XGR_MAXX; x ++){
				r = XGR_64KR(xbuf[index]);
				g = (XGR_64KG(xbuf[index])) >> xg;
				b = XGR_64KB(xbuf[index]);

				buf[index * 3 + 0] = b<<3;
				buf[index * 3 + 1] = g<<3;
				buf[index * 3 + 2] = r<<3;
				index ++;
			}
		}
	}
	else
		mchA_d3dScreenShot(buf,XGR_MAXX * XGR_MAXY * 3);

	tgaPtr = new tgaFile(XGR_MAXX,XGR_MAXY,24,buf,ID);
	tgaPtr -> write(fname);
	delete tgaPtr;
	delete buf;

	mchScreenShotCount ++;
}

void mchFreeResources(int mode)
{
	fxlabClose();

	mchTrackDemoMode = 0;

	if(!mode){
		mchNumLaps = mchNumLapsDef;
		mchNumRacers = mchNumRacersDef;
		hsSetup();
		mchInitWorldsPrm();
		mchFinitRacers();
		mch_trackD -> Reset();

		if(iworld_D)
			iworld_D -> Clear();

		iWorldMode = 0;
	}

	if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);

	if(!mchResourcesFlag) return;
	mchResourcesFlag = 0;

	mchCamera3D = 0;
	mchCameraAX = mchCameraAY = mchCameraAZ = 0.0f;

	if(!mode && mch_iScreen) 
		acsClearStack();

	mchStopSound();
	mchFreeWorldSound();
	mchEndGameFinit(1);

	mchFinitRaceDispatcher(mode);
	camera_dispatcher -> clear(); // to save camera params

	Xreal_finit();

	mchTimeMode = MCH_TIME_RUNNING;
	
	gb_IVisGeneric->ReleaseObject(0xFFFFFFFF,0xFFFFFFFF);
	gb_IVisGeneric->ReleaseOmni(0xFFFFFFFF);
}

void mchSetTimeMode(int m)
{
	switch(m){
		case MCH_TIME_WAITING_TURN:
			if(mchTurnBasedGame){
				mchFreezeTime = 1;
				camera_dispatcher -> setMakeTurnInvitation();
			}

			XGR_MouseSetPos(20,20);

			mchPauseSound();
			mchStartSoundEFF(EFF_STOP_TIME);

			if(!mchPBEM_Game){
				if(!(mch_raceD -> activeRacer -> flags & MCH_AI))
					mchA_SetTimer(hsWaitTurnTime);
				else
					mchA_SetTimer(2);
			}
			resource_dispatcher.set_speed(1.0f);
			camera_dispatcher -> setTimeSpeed(1.0f);
			break;
		case MCH_TIME_STOPPED:
			if(mchTurnBasedGame){
				mchFreezeTime = 1;
				camera_dispatcher -> setStopTime();

				if(mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS)
					mchA_SetTimer(hsTurnPrepareTime);

				mch_raceD -> initCheckpoints(NULL);
			}

			XGR_MouseSetPos(20,20);

			mchPauseSound();
			mchStartSoundEFF(EFF_STOP_TIME);

//			if(mchHS_LoadFlag)
//				mchGameFlags |= MCH_START_TIME_FLAG;

			resource_dispatcher.set_speed(1.0f);
			camera_dispatcher -> setTimeSpeed(1.0f);
			break;
		case MCH_TIME_RUNNING:
			if(!mchPBEM_Pause)
				resource_dispatcher.reset();

			mchFreezeTime = 0;

			if(!mchPBEM_Pause){ 
				mch_raceD -> ResetSeeds();

				if(mchGameMode == MCH_SPLIT_CONTROL_HS)
					mch_raceD -> activeRacer -> SetTime(mch_clock());
				if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
					mch_raceD -> SetTime(mch_clock());
					mch_raceD -> prepareStartTime();
				}

				if(mchGameMode != MCH_SINGLE_GAME)
					mchA_SetTimer(hsTurnPrepareTime);
			}

			if(mchTimeMode != MCH_TIME_RUNNING){
				mchResumeSound();
				mchStartSoundEFF(EFF_START_TIME);

				if(mchTurnBasedGame)
					mch_raceD -> initCheckpoints(NULL);
			}

			if(mchTurnBasedGame){
				resource_dispatcher.set_speed(mchTimeSpeedCur);
				camera_dispatcher -> setTimeSpeed(mchTimeSpeedCur);
			}
			break;
		case MCH_TIME_GAME_OVER:
			if(mchFreezeTime){
				mchFreezeTime = 0;
				camera_dispatcher -> setStartTime();
			}
			resource_dispatcher.set_speed(1.0f);
			camera_dispatcher -> setTimeSpeed(1.0f);
			break;
	}
	mchTimeMode = m;
}

void mchSetGameMode(int m)
{
	mchGameMode = m;
}

void mchGlobalQuant(void)
{
	start_autostop_timer(mchGlobalQuant, STAT_RESDISP);
	CONTROL_FP();
	int fl = mchGameFlags;
	mchGameFlags = 0;

//	char* header;

	trkRecQuant();

	if(mchXRecorderMode){
		if(!(XRec.flags & mchXRecorderMode))
			KeyBuf -> put(VK_ESCAPE,KBD_CUR_KEY_PRESSED);

		if(iWorldMode){
			iworld_D -> Quant();
			if(mchSplitScreenGame)
				iworld_D2 -> Quant();
		}

		XRec.Quant();
		mchMouseQuant3D(0,XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2,XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2);
		mchKeyTrap();
	}

	mch_arcScrD -> TimerQuant();

	if(fl & MCH_TOGGLE_PAUSE_FLAG){
		if(fl & MCH_STOP_TIME_FLAG){
			if(!mchPBEM_Pause){
				if(mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER))
					mchSetTimeMode(MCH_TIME_WAITING_TURN);
				else
					mchSetTimeMode(MCH_TIME_STOPPED);
				mchPBEM_Pause = 1;
			}
		}
		if(fl & MCH_START_TIME_FLAG){
			if(mchTimeMode == MCH_TIME_WAITING_TURN){
				mchSetTimeMode(MCH_TIME_RUNNING);
				mchCenteringFlag = 0;
				mchPBEM_Pause = 0;

				if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
			}
			if(mchTimeMode == MCH_TIME_STOPPED){
				if(mchCenteringFlag){
					if(camera_dispatcher -> isInterpolationCompleted()){
						mchSetTimeMode(MCH_TIME_RUNNING);
						mchCenteringFlag = 0;
						mchPBEM_Pause = 0;

						if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
					}
					else
						mchGameFlags |= MCH_START_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
				}
				else {
					if(!mchPBEM_EndFlag){
						camera_dispatcher -> setStartTime();

						mchCenteringFlag = 1;
						mchGameFlags |= MCH_START_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
					}
					else
						quantRTO -> NextID = XT_TERMINATE_ID;

				}
			}
		}
		return;
	}
	else {
		if(mchPBEM_Pause)
			return;
	}

	if(fl & MCH_STOP_TIME_FLAG){
		_RACE_LOG_STR("StopTime -> ");
		_RACE_LOG_TXT(mch_clock());
		_RACE_LOG_STR("\r\n");

		if(mchGameMode == MCH_SPLIT_CONTROL_HS){
			if(mchTimeMode == MCH_TIME_RUNNING){
				mch_raceD -> activeRacer -> RemoveSeeds();
				mch_raceD -> nextRacer();
				mch_raceD -> AddSeeds(1);
				mchSetTimeMode(MCH_TIME_WAITING_TURN);
			}
		}
		if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
			mch_raceD -> SetTime(mch_clock());

			if(mchTimeMode == MCH_TIME_RUNNING){
				if(!mchPBEM_Game && !mchHS_LoadFlag)
					mch_raceD -> firstRacer(mchPBEM_CurPlayer);


				mch_raceD -> AddSeeds(0);

				if(mchHS_SaveFlag && !mchPBEM_Game)
					mch_raceD -> EndTurn();

				if(mchPBEM_GameMode != MCH_PBEM_EDIT_TURN){
					if(mchHS_LoadFlag && mchPBEM_HiddenTime && mchTurn >= mchPBEM_TargetTurn - 1){
						resource_dispatcher.break_evolve();
						mchPBEM_HiddenTime = 0;
					}
				}
				else {
					if(mchHS_LoadFlag && mchPBEM_HiddenTime && mchTurn >= mchPBEM_TargetTurn){
						resource_dispatcher.break_evolve();
						mchPBEM_HiddenTime = 0;
					}
				}

				if(mchHS_LoadFlag && mchTurn >= mchPBEM_TargetTurn){
					mchHS_LoadFlag = 0;
					ogSetRefreshTime();

					switch(mchPBEM_GameMode){
						case MCH_PBEM_EDIT_TURN:
							mchHS_SaveFlag = 1;
							mchTurn ++;
							mch_raceD -> LoadAllSeeds();
							break;
						case MCH_PBEM_VIEW_TURN:
							mchTurn ++;
							mchGameFlags |= MCH_STOP_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
							mchPBEM_EndFlag = 1;
							break;
						case MCH_PBEM_REPLAY_TURN:
							mchHS_LoadFlag = 0;
							mchHS_SaveFlag = 1;
							mchTurn ++;
							mch_raceD -> LoadAllSeeds();

							if(mchPBEM_Game){
								mchPBEM_GameMode = MCH_PBEM_EDIT_TURN;
								if(og_inP.express_game() && !mchPBEM_CheckFlag(PBEM_CONTROL_DATA_SENT) && !mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
									mchPBEM_HiddenTime = -1;
									mchPBEM_SetFlag(PBEM_SHOW_GAME);
								}
							}
							else {
								mchPBEM_GameMode = 0;
								mchPBEM_CurPlayer = 0;
							}
							break;
						case MCH_PBEM_END_TURN:
							mch_raceD -> EndTurn();
							mchHS_SaveFlag = 1;
							quantRTO -> NextID = XT_TERMINATE_ID;
							mchTurn ++;
							break;
					}
				}
				else 
					mchTurn ++;

				if(mchHS_LoadFlag){
//					if(!sgOnlineControlCheck() && !mchPBEM_CheckFlag(PBEM_REPLAY_MODE))
					if(!sgOnlineControlCheck())
						ErrH.Abort("sgOnlineControlCheck() failed");

					mch_raceD -> LoadAllSeeds();
					resource_dispatcher.reset();
					mch_raceD -> ResetSeeds();
					mch_raceD -> SetTime(mch_clock());
					mch_raceD -> prepareStartTime();
				}
				else {
					mchSetTimeMode(MCH_TIME_WAITING_TURN);
				}
			}
		}
		if(mchGameMode == MCH_SINGLE_GAME){
			if(mchTimeMode != MCH_TIME_GAME_OVER){
				mchSetTimeMode(MCH_TIME_STOPPED);
#ifndef _FINAL_VERSION_
				mchFreezeTime = 1;
#endif
			}
		}
	}
	if(fl & MCH_START_TIME_FLAG){
		if(mchTimeMode == MCH_TIME_WAITING_TURN){
			if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_START_TIME)){
				resource_dispatcher.reset();
				mch_raceD -> ResetSeeds();
				mch_raceD -> firstRacer(mchPBEM_CurPlayer);
				mchSetTimeMode(MCH_TIME_RUNNING);
				mchCenteringFlag = 0;

				if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);

				mchPBEM_DropFlag(PBEM_START_TIME);
				mchPBEM_DropFlag(PBEM_CONTROL_DATA_SENT);
			}
			else {
				mchSetTimeMode(MCH_TIME_STOPPED);

				if(mch_raceD -> activeRacer -> flags & MCH_AI)
					mchGameFlags |= MCH_START_TIME_FLAG;
			}
		}
		else {
			if(mchTimeMode == MCH_TIME_GAME_OVER){
				mchFreeResourcesFlag = 1;
				if((mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS) && mch_iScreenID != -1){
					if(!mchPBEM_Game && mch_iScreen)
						quantRTO -> NextID = RTO_MAIN_MENU_ID;
					else
						quantRTO -> NextID = -1;
				}
				else
					mchSetNextQuantRTO();

				mchCurrentWorld = mchNextWorld;
				mchCurrentTrack = mchNextTrack;

				if(mchNextWorld == 7 && !mch_iScreen)
					quantRTO -> NextID = -1;
			}
			else {
				if(mchGameMode != MCH_ENTIRE_CONTROL_HS || mch_raceD -> isActiveRacerLast()){
					if(mchCenteringFlag){
						if(camera_dispatcher -> isInterpolationCompleted()){
							if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
								if(mchHS_SaveFlag){
									mch_raceD -> SaveAllSeeds();

									if(mchPBEM_Game){
										mchSetTimeMode(MCH_TIME_WAITING_TURN);
										mchPBEM_DropFlag(PBEM_ERROR);
										mchPBEM_SetFlag(PBEM_SHOW_GAME | PBEM_DATA_SENT);

										mchPBEM_DataFlag = -1;
										mchPBEM_HiddenTime = 1;

										sgPrepareSeedsData();

										if(og_inP.express_game())
											mchPBEM_SetFlag(PBEM_NEED_REFRESH);

//										header = "Content-type: application/x-www-form-urlencoded\r\n";
//										wi_D.open_request(WI_POST,wiGameURL,header,strlen(header),wi_D.output_buffer(),wi_D.output_size());
									}
								}
							}

							if(!mchPBEM_Game || !mchHS_SaveFlag){
/*
								_RACE_LOG_STR("StartTime -> ");
								_RACE_LOG_TXT(mch_clock());
								_RACE_LOG_STR("\r\n");

								_RACE_LOG_STR("Rnd -> ");
								_RACE_LOG_TXT(XRndVal);
								_RACE_LOG_STR("\r\n");
*/
/*
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
*/
								mchSetTimeMode(MCH_TIME_RUNNING);
								mchCenteringFlag = 0;

								if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);
							}

							mchCenteringFlag = 0;
						}
						else
							mchGameFlags |= MCH_START_TIME_FLAG;
					}
					else {
						if(mchTurnBasedGame){
							camera_dispatcher -> setStartTime();

//							if(mchPBEM_Game)
//								mch_raceD -> firstRacer(mchPBEM_CurPlayer);
						}

						mchCenteringFlag = 1;
						mchGameFlags |= MCH_START_TIME_FLAG;

						if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
							mch_raceD -> SetTime(mch_clock());
							mch_raceD -> prepareStartTime();
						}
					}
				}
				else {
					mch_raceD -> nextRacerHS();

					if(hsSeedsAccumulation)
						mch_raceD -> activeRacer -> numSeeds += hsTurnSeedsMax;
					else
						mch_raceD -> activeRacer -> numSeeds = hsTurnSeedsMax;

					mchSetTimeMode(MCH_TIME_WAITING_TURN);
				}
			}
		}
	}
}

void mchA_CharBitmapTest(void);

XGR_Pal64K imgRTO_Pal;

void ShowImageRTO::Init(int id)
{
	int x,y,sx,sy;
	char palBuf[768];
//	XGR_Pal64K pal16;

	unsigned short* ptr;

	HWND hWnd;

	txtScrollY = 0;
	txtScrollDelta = 10.0f;

	mchImgRTO_ActiveFlag = 1;

	if(!mch_ShowImages && !(Flags[0] & IMG_RTO_NO_DISABLE)) return;

	XStream fh(0);

	if(!curFile){
		mchA_d3dReleaseBackBuffer();
#ifndef _MAKE_INTRO_MOVIE_
		mchReInitGraph(MCH_800x600);
#else
		mchReInitGraph(MCH_640x480);
#endif
		mchA_d3dCreateBackBuffer();
		sWinVideo::Init();
		mchStopSound();
	}
	mchA_d3dLockBackBuffer();

	XKey.init((void *)&mchShowImageKeyPress,NULL);
	XGR_MouseInit(XGR_MAXX/2,XGR_MAXY/2,0,0,1,NULL);
	XGR_MouseHide();
	XGR_MouseSetPressHandler(XGM_LEFT_BUTTON,mchShowImageMousePress);
	XGR_MouseSetPressHandler(XGM_RIGHT_BUTTON,mchShowImageMousePress);
	XGR_MouseSetPressHandler(XGM_RIGHT_BUTTON,NULL);
	XGR_MouseSetUnPressHandler(XGM_RIGHT_BUTTON,NULL);

	if(!(Flags[curFile] & IMG_RTO_NO_IMAGE) && !(Flags[curFile] & IMG_RTO_AVI)){
		if(acsOpenResource(fileNames[curFile],fh)){
			ptr = loadJPG(&fh,sx,sy);
			x = (XGR_MAXX - sx)/2;
			y = (XGR_MAXY - sy)/2;
			XGR_Obj.putspr16(x,y,sx,sy,ptr,XGR_BLACK_FON);
			delete[] ptr;
		}
		else
			XGR_Obj.fill(0);
	}
	else {
		XGR_Obj.fill(0);
	}

	if(Flags[curFile] & IMG_RTO_AVI){
		if(mch_imgVideoObj.IsPlay()){
			mch_imgVideoObj.Stop();
			mch_imgVideoObj.Close();
		}
		mch_imgVideoObj.Open(fileNames[curFile]);

		hWnd=(HWND)XGR_hWnd;
		if(!hWnd) d3dGetWindowHandle(&hWnd);

		if(Flags[curFile] & IMG_RTO_AVI_FULLSCREEN){
			mch_imgVideoObj.SetWin(hWnd,0,0,XGR_MAXX,XGR_MAXY);
		}
		else {
			mch_imgVideoObj.GetSize(&sx,&sy);
			x = (XGR_MAXX - sx)/2;
			y = (XGR_MAXY - sy)/2;

			mch_imgVideoObj.SetWin(hWnd,x,y,sx,sy);
		}

		mch_imgVideoObj.Play();
	}

	if(Flags[curFile] & IMG_RTO_TEXT){
		if(Flags[curFile] & IMG_RTO_PALETTE && acsOpenResource(palNames[curFile],fh)){
			fh.read(palBuf,768);
			fh.close();
		}
		else {
			memset(palBuf,0,768);
			for(x = 0; x < 32; x ++){
				palBuf[x * 3] = palBuf[x * 3 + 1] = palBuf[x * 3 + 2] = x * 2;
			}
		}
		imgRTO_Pal.prepare(palBuf);

		sx = acsTextStrLenMax(fntData[curFile],(unsigned char*)textPtr[curFile],spaceData[curFile]);
		sy = acsTextHeight(fntData[curFile],(unsigned char*)textPtr[curFile],spaceData[curFile]);

		x = (XGR_MAXX - sx)/2;
		y = (XGR_MAXY - sy)/2;
		acsOutText16(x,y,fntData[curFile],colData[curFile],(unsigned char*)textPtr[curFile],imgRTO_Pal.data,spaceData[curFile]);
	}

	if(RenderMode == DIRECT3D_HICOLOR){
		mchA_d3dUnlockBackBuffer();
/*
		mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);

		if(!(Flags[curFile] & IMG_RTO_AVI))
			mchA_d3dFlip();
*/
	}
	else
		gb_IGraph3d->Flush();
//		XGR_Flush(0,0,XGR_MAXX,XGR_MAXY);

	if(Flags[curFile] & IMG_RTO_INTRO_IMAGE)
		mchInitIntro();

	if(!curFile){
		gb_IGraph3d->Fill(0,0,0);
		gb_IGraph3d->Flush();
		gb_IGraph3d->Fill(0,0,0);
	}

	count = clocki();

	mchShowImageMouseFlag = 0;
	mchShowImageKeyFlag = 0;

	mchCurrentWorld = mchCurrentTrack = -1;

	if(Flags[curFile] & IMG_RTO_START_MUSIC)
		if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);

	SetGammaSeq(1);
}

int ShowImageRTO::Quant(void)
{
	int v,x,y,sx,sy,gd,ret_flag = 1;

	if(mch_ShowImages || Flags[0] & IMG_RTO_NO_DISABLE){
		if(Flags[curFile] & (IMG_RTO_INTRO_IMAGE | IMG_RTO_SCROLL_TEXT) || (count + WaitTimer) > clocki())
			ret_flag = 0;
		else 
			SetGammaSeq(-1);

		if(d3dIsActive()){
			if(!(Flags[curFile] & IMG_RTO_NO_IMAGE) || (Flags[curFile] & IMG_RTO_TEXT)){
				if(Flags[curFile] & IMG_RTO_SCROLL_TEXT){
					mchA_d3dLockBackBuffer();
					XGR_Obj.fill16(0);

					sx = acsTextStrLenMax(fntData[curFile],(unsigned char*)textPtr[curFile],spaceData[curFile]);
					sy = acsTextHeight(fntData[curFile],(unsigned char*)textPtr[curFile],spaceData[curFile]);

					x = (XGR_MAXX - sx)/2;
					y = XGR_MAXY - txtScrollY;

					txtScrollY += 1;

					if(txtScrollY >= XGR_MAXY + sy + 35)
						txtScrollY = 0;

					acsOutText16(x,y,fntData[curFile],colData[curFile],(unsigned char*)textPtr[curFile],imgRTO_Pal.data,spaceData[curFile]);

					mchA_d3dUnlockBackBuffer();
				}
				mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);
			}
		}

		gb_IGraph3d -> BeginScene();

		if(Flags[curFile] & IMG_RTO_INTRO_IMAGE){
			mch_introD -> Quant();
		}

		gd = gammaDir;

		if(gammaDir){
			ret_flag = 0;
			v = ((clocki() - gammaTimerBeg) >> 1);
			if(v >= 255){ 
				if(gammaDir < 0) ret_flag = 1;
				gammaDir = 0;
			}

			if(v < 0) v = 0;
			if(v > 255) v = 255;

			if(gd > 0) v = 255 - v;

			gb_IGraph3d->SetViewColor(0,0,0,v);
		}

		gb_IGraph3d->EndScene();

		gb_IGraph3d->Flush();
		gb_IGraph3d->Fill(0,0,0);

		if(Flags[curFile] & IMG_RTO_AVI && Flags[curFile] & IMG_RTO_AVI_PLAY_ONCE){
			if(mch_imgVideoObj.IsComplete())
				SetGammaSeq(-1);
		}

		if(Flags[curFile] & IMG_RTO_INTRO_IMAGE && mch_introD -> isEnd())
			SetGammaSeq(-1);

		if(mchShowImageKeyFlag || mchShowImageMouseFlag){
			if(Flags[curFile] & IMG_RTO_INTRO_IMAGE){
				if(!mch_introD -> isEnd())
					mch_introD -> AdvancePhase();
				else
					SetGammaSeq(-1);

				mchShowImageKeyFlag = mchShowImageMouseFlag = 0;
			}
			else
				SetGammaSeq(-1);
		}
	}

	mchSoundQuant();

	if(ret_flag){
		curFile ++;
		if(curFile >= numFiles)
			return NextID;
	}
	else
		return 0;

	return ID;
}

void ShowImageRTO::Finit(void)
{
	mchImgRTO_ActiveFlag = 0;

	if(mch_ShowImages || Flags[0] & IMG_RTO_NO_DISABLE){
		if(Flags[curFile] & IMG_RTO_INTRO_IMAGE)
			mchFinitIntro();

//		ErrH.Exit();

		if(curFile >= numFiles && RenderMode == DIRECT3D_HICOLOR){
			mchA_d3dReleaseBackBuffer();

			if(NextID != XT_TERMINATE_ID)
				mchReInitGraph(xgrGameMode);
			else {
				if(!mchMusicMute){ 
					sndMusicStop();
					mchMusicMute = 1;
				}
			}

			sWinVideo::Done();
		}

		if(Flags[curFile - 1] & IMG_RTO_AVI){
			if(mch_imgVideoObj.IsPlay()){
				mch_imgVideoObj.Stop();
				mch_imgVideoObj.Close();
			}
		}

		if(imageData) delete imageData;

		KBD_init();
		XGR_MouseSetPressHandler(XGM_LEFT_BUTTON,mchMouseLP);
		XGR_MouseSetUnPressHandler(XGM_LEFT_BUTTON,mchMouseLU);

		XGR_MouseSetPressHandler(XGM_RIGHT_BUTTON,mchMouseRP);
		XGR_MouseSetUnPressHandler(XGM_RIGHT_BUTTON,mchMouseRU);

		if(mch_iScreen && acsScrD)
			acsScrD -> ChangeCoords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);
		acsExtSetCamera();
		mchInitCamera();

		if(acsActiveFlag)
			acsCreateWater();

		acsMouseLPFlag = acsMouseRPFlag = 0;
	}
	else {
		if(NextID != XT_TERMINATE_ID)
			mchReInitGraph(xgrGameMode);
	}
}

void ShowImageRTO::SetGammaSeq(int dir)
{
	if(gammaDir == dir) return;

	gammaDir = dir;
	gammaTimerBeg = clocki();
}

void mchShowImageMousePress(int fl, int x, int y)
{
	mchShowImageMouseFlag = 1;
}

void mchShowImageKeyPress(int k)
{
	mchShowImageKeyFlag = 1;
}

ShowImageRTO::ShowImageRTO(void)
{
	int i;
	ID = RTO_SHOW_IMAGE_ID;
	Timer = RTO_IMAGE_TIMER;
	numFiles = 0;

	imageData = NULL;

	memset(Flags,0,sizeof(int) * RTO_SHOW_IMAGE_MAX);
	memset(fntData,0,sizeof(int) * RTO_SHOW_IMAGE_MAX);
	memset(spaceData,0,sizeof(int) * RTO_SHOW_IMAGE_MAX);

	textPtr = new char*[RTO_SHOW_IMAGE_MAX];
	fileNames = new char*[RTO_SHOW_IMAGE_MAX];
	palNames = new char*[RTO_SHOW_IMAGE_MAX];
	for(i = 0; i < RTO_SHOW_IMAGE_MAX; i ++){
		fileNames[i] = new char[256];
		palNames[i] = new char[256];
		textPtr[i] = NULL;
	}

	WaitTimer = RTO_SHOW_IMAGE_TIME;

	NextID = RTO_MAIN_MENU_ID;
}

void mchRestore(void)
{
	d3dFlipToGdiSurface();
	mchFinitSound();
	if(gb_IVisGeneric)
	{
		if(gb_URenderDevice) 
			gb_IVisGeneric->ReleaseGraph(gb_URenderDevice); // закрытие окна вывода
		gb_UScene=0; gb_URenderDevice=0; gb_IGraph3d=0; gb_IVisGeneric=0;
	}
	XJoystickCleanup();
}

void racerQuant()
{
	start_autostop_timer(racerQuant, STAT_RESDISP);
	CONTROL_FP();
	if(!mchFreezeTime){
		#ifndef _FINAL_VERSION_
		if(xreal_log)
			bout < "XrealEvolveQuant: " <= global_time() < "\n";
		#endif
		mch_raceD -> quant();
		}
}

void XrealPreQuant()
{
	start_autostop_timer(XrealPreQuant, STAT_RESDISP);
	CONTROL_FP();
	if(!mchFreezeTime){
		#ifndef _FINAL_VERSION_
		if(xreal_log)
			bout < "XrealPreQuant: " <= global_time() < "\n";
		#endif
		Mdisp -> pre_quant();
		}
	
#ifndef _FINAL_VERSION_
	const char* dir_str = check_command_line("shown_evolve:");
	if(dir_str && atoi(dir_str) < global_time()){
		append_message_to_file("OK", "diff");
		KeyBuf -> put(VK_ESCAPE, KBD_CUR_KEY_PRESSED);
		}
#endif // _FINAL_VERSION_
}

void XrealEvolveQuant()
{
	start_autostop_timer(XrealEvolveQuant, STAT_RESDISP);
	CONTROL_FP();
	non_stop_global_time.next_frame();

	XJoystickQuant();

	if(!mchFreezeTime){
		global_time.next_frame();
		#ifndef _FINAL_VERSION_
		if(xreal_log){
#ifdef _WIN32
			bout < "controlfp: " <= _controlfp( 0, 0 ) < "\n";
			bout < "XrealEvolveQuant: " <= global_time() < "\n";
#endif
			}
		#endif
		Mdisp -> evolve_quant();
		}

	camera_dispatcher -> quant();
	if(mchSplitScreenGame && camera_dispatcher2) camera_dispatcher2 -> quant();
}

void resource_dispatcher_init()
{
	resource_dispatcher.attach(&mchGlobalQuant, racer_quant_period);
	resource_dispatcher.attach(&racerQuant, racer_quant_period);
	resource_dispatcher.attach(&XrealPreQuant, xreal_quant_period);
	resource_dispatcher.attach(&XrealEvolveQuant, xreal_evolve_period);
	resource_dispatcher.attach(&fxlabServerQuant, fxlabServerQuantPeriod);

	global_time.set(0, xreal_evolve_period, 10000);
	non_stop_global_time.set(0, xreal_evolve_period, 10000);
	resource_dispatcher.setTimer(syncro_by_clock && !setTimerToProfile, STANDART_FRAME_PERIOD, MAX_TIME_INTERVAL);
}

void mchDrawQuant(void)
{
	if(!mchResourcesFlag) return;
/*
	float u,v,us,vs,uc,vc;
	u = tMap -> duOfsPolyGrid;
	v = tMap -> dvOfsPolyGrid;

	us = tMap -> Sky -> du;
	vs = tMap -> Sky -> dv;

	uc = tMap -> Cloud -> du;
	vc = tMap -> Cloud -> dv;

	tMap -> Sky -> du = tMap -> Sky -> dv = 0.0f;
	tMap -> Cloud -> du = tMap -> Cloud -> dv = 0.0f;
	tMap -> duOfsPolyGrid = tMap -> dvOfsPolyGrid = 0.0f;
*/

	gb_IGraph3d->Fill(0,0,0);

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS){
		mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);
		return;
	}

	fxlabStopTime = 1;

	fxlabClientQuant();

// действия до начала сцены (в частности кэширование)
	if(!mchFreezeTime) gb_IVisGeneric->dSetTime(0);
	gb_IVisGeneric->PreDraw(0x7FFFFFFF);
//////////////////////////////////////// НАЧАЛО СЦЕНЫ /////////////////////////////////////////
	gb_IGraph3d->BeginScene();
///////////////////////////////////////////////////////////////////////////////////////////////

	if(RenderMode==XGRAPH_HICOLOR) mch_arcScrD -> PutFon(); // !
	gb_IVisGeneric->Draw();

	fxlabClientShow();

	mch_raceD -> showStats(); 

	mch_raceD -> redraw();    
	mch_arcScrD -> Redraw(); 
	Xreal_draw_handler();

	if(mchFPS_Log) mchFPS_Quant(); 

	mchA_DarkenRect(0,0,XGR_MAXX,XGR_MAXY,150);
/*
	tMap -> Sky -> du = us;
	tMap -> Sky -> dv = vs;

	tMap -> Cloud -> du = uc;
	tMap -> Cloud -> dv = vc;

	tMap -> duOfsPolyGrid = u;
	tMap -> dvOfsPolyGrid = v;
*/
///////////////////////////////////////// КОНЕЦ СЦЕНЫ /////////////////////////////////////////
	gb_IGraph3d->EndScene();
///////////////////////////////////////////////////////////////////////////////////////////////
	gb_IVisGeneric->PostDraw();
}

void mchSetNextQuantRTO(void)
{
//	quantRTO -> NextID = RTO_GAME_QUANT_ID;
	quantRTO -> NextID = RTO_LOADING_ID;
}

void mchInitCamera(void)
{
	if(!camera_dispatcher) return;

	iWnd -> init(MCH_WND_FULLSCREEN | MCH_WND_640x480);
	gameWnd -> init(MCH_WND_FULLSCREEN);

	if(mchSplitScreenGame){
		camera_dispatcher2 -> activate();
		if(mchSplitScreenMode){
			iWnd0 -> init(MCH_WND_ALIGN_TOP | MCH_WND_640x480);
			gameWnd0 -> init(MCH_WND_ALIGN_TOP);
			camera_dispatcher -> setWindow(gameWnd0);
			gb_IVisGeneric->SetCameraAttribute(camera_dispatcher->camera(),ATTRIBUTE_CAMERA_MASK_B);

			iWnd1 -> init(MCH_WND_ALIGN_BOTTOM | MCH_WND_640x480);
			gameWnd1 -> init(MCH_WND_ALIGN_BOTTOM);
			camera_dispatcher2 -> setWindow(gameWnd1);
			gb_IVisGeneric->SetCameraAttribute(camera_dispatcher2->camera(),ATTRIBUTE_CAMERA_MASK_A);
		}
		else {
			iWnd0 -> init(MCH_WND_ALIGN_LEFT | MCH_WND_640x480);
			gameWnd0 -> init(MCH_WND_ALIGN_LEFT);
			camera_dispatcher -> setWindow(gameWnd0);
			gb_IVisGeneric->SetCameraAttribute(camera_dispatcher->camera(),ATTRIBUTE_CAMERA_MASK_B);

			iWnd1 -> init(MCH_WND_ALIGN_RIGHT | MCH_WND_640x480);
			gameWnd1 -> init(MCH_WND_ALIGN_RIGHT);
			camera_dispatcher2 -> setWindow(gameWnd1);
			gb_IVisGeneric->SetCameraAttribute(camera_dispatcher2->camera(),ATTRIBUTE_CAMERA_MASK_A);
		}
	}
	else {
		camera_dispatcher2 -> deactivate();
		camera_dispatcher -> setWindow(gameWnd);
	}
}

void mchReInitGraph(int mode)
{
	int dwScrX,GraphMode,dwScrY,FullScr,ColorBit;

	gb_IVisGeneric -> GetGraphInfo(gb_URenderDevice,&dwScrX,&dwScrY,&GraphMode,&FullScr,&ColorBit);

	if(xgrInitMode == mode && FullScr == xgrFullscreenMode && ColorBit == xgrColorDepth) return;

	switch(mode)
	{
		case MCH_320x200:
			dwScrX=320,dwScrY=200;
			break;
		case MCH_640x480:
			dwScrX=640,dwScrY=480;
			break;
		case MCH_800x600:
			dwScrX=800,dwScrY=600;
			break;
		case MCH_1024x768:
			dwScrX=1024,dwScrY=768;
			break;
		case MCH_1152x864:
			dwScrX=1152,dwScrY=864;
			break;
		case MCH_1280x1024:
			dwScrX=1280,dwScrY=1024;
			break;
		case MCH_1600x1200:
			dwScrX=1600,dwScrY=1200;
			break;
	}
	xgrInitMode = mode;

	gb_IVisGeneric -> ReInitGraph(gb_URenderDevice,dwScrX,dwScrY,DIRECT3D_HICOLOR,xgrFullscreenMode,xgrColorDepth);

	xgrScreenSizeX = dwScrX;
	xgrScreenSizeY = dwScrY;

	mchA_d3dResX = (float)dwScrX / 640.0f;
	mchA_d3dResY = (float)dwScrY / 480.0f;

	XGR_MouseObj.SetClipAuto();

	mchA_d3dSetGamma(((float)xgrGammaValue)/100.0f);
}

void mchInitApplication(void)
{
	static int init_flag = 0;

	if(init_flag) return;

	init_flag = 1;

	if(!mch_iScreen){
		mchInitWorldsPrm();
		mchInitRacers();
		acsInitFont3D();
		acsExtInitPrm();
	}
	else {
		if(mchPBEM_Game)
			mchInitRacers();

		acsActiveFlag = 1;
		if(!mchMusicMute && !mchSkipMainMenu) mchSetMusicMode(MCH_CD_SHOW);
		acsActiveFlag = 0;
	}

	mchA_InitNames();

	allocation_tracking("before fxlabInit");
	fxlabInit();
	allocation_tracking("fxlabInit");

	if(!mch_iScreen) mchUpdateOptions();

	resource_dispatcher_init();
}

void mchPBEM_Init(void)
{
	int tm = -1,turn;
//	sgRecord* r;

	mchPBEM_HiddenTime = 0;

	if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
		mchPBEM_GameMode = MCH_PBEM_REPLAY_TURN;
		mchPBEM_ViewEntireGame = 1;
	}

	if(mchPBEM_GameMode == MCH_PBEM_VIEW_TURN || mchPBEM_GameMode == MCH_PBEM_REPLAY_TURN)
		turn = mchPBEM_TargetTurn - 1;
	else
		turn = mchPBEM_TargetTurn;

	if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
		mchSetTimeMode(MCH_TIME_WAITING_TURN);
		mchPBEM_SetFlag(PBEM_CHANGE_PLAYER);
//		mchSetTimeMode(MCH_TIME_RUNNING);
//		if(++mchPBEM_CurPlayer >= 4) mchPBEM_CurPlayer = 0;
	}
	else
		mchSetTimeMode(MCH_TIME_RUNNING);
	mch_raceD -> firstRacer(mchPBEM_CurPlayer);

	if(turn <= -1){ 
		if(mchPBEM_GameMode == MCH_PBEM_EDIT_TURN)
			mchPBEM_HiddenTime = -1;
		return;
	}
/*
	r = sgD -> get_first(turn);
	while(r){
		if(r -> Type() == SG_END_TURN_RECORD){
			tm = *((int*)r -> Data());
			break;
		}
		r = sgD -> get_next();
	}

	if(tm <= -1) return;
	mchPBEM_HiddenTime = tm;
*/

	mchPBEM_HiddenTime = 1;
	mchSetLoadTimeMax(mchPBEM_TargetTurn + 1);
	mchLoadTimerFunc(0);

	if(mchPBEM_GameMode == MCH_PBEM_REPLAY_TURN && mchPBEM_ViewEntireGame)
		mchPBEM_HiddenTime = 0;
	else
		mchPBEM_HiddenTime = 1;
}

void mchPBEM_LoadingQuant(void)
{
	int k,tm;	//,dt,dt1;
	const int dt0 = 100;

	static XBuffer obj_buf;

#ifndef _OFFLINE_DEBUG_
	char* header,*url;
#endif

	if(!mchPBEM_DataFlag){
		tm = mchPBEM_HiddenTime;

		if(mchPBEM_HiddenTime != -1){
			mch_sndD -> Disable(1);

			resource_dispatcher.evolve(dt0);
			mchLoadTimerFunc(mchTurn);
			
			mch_sndD -> Disable(0);
		}
		else
			mchPBEM_HiddenTime = 0;

		if(mchPBEM_Game && !mchPBEM_HiddenTime && !mchPBEM_CheckFlag(PBEM_CONTROL_DATA_SENT) && !mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
			mchPBEM_HiddenTime = 1;
			mchPBEM_DataFlag = -1;

			if(mchTimeMode != MCH_TIME_GAME_OVER)
				sgPrepareControlData();

			mchPBEM_SetFlag(PBEM_CONTROL_DATA_SENT);
		}
	}
	else {
		if(mchPBEM_CheckFlag(PBEM_CONNECTED) && !mchPBEM_CheckFlag(PBEM_ERROR)){
			ogQuant();

			switch(wi_D.get_status()){
				case WI_ERROR:
					mchPBEM_SetFlag(PBEM_ERROR);
					break;
				case WI_FINISHED:
					if(mchPBEM_CheckFlag(PBEM_CHECK_RETURN)){
						mchPBEM_DataFlag = 0;
						mchPBEM_HiddenTime = 0;
						mchPBEM_DropFlag(PBEM_CONNECTED | PBEM_ERROR | PBEM_SHOW_GAME | PBEM_CHECK_RETURN);

						if(og_inP2.decode(wi_D.input_buffer(),strlen(wi_D.input_buffer()))){
							sgProcessRefreshData();
						}
					}
					else {
#ifndef _OFFLINE_DEBUG_
						if(og_inP.check_return(wi_D.input_buffer())){
							mchPBEM_DataFlag = 0;
							mchPBEM_HiddenTime = 0;
							mchPBEM_DropFlag(PBEM_CONNECTED | PBEM_ERROR | PBEM_SHOW_GAME);

							if(og_inP.check_return(wi_D.input_buffer()) == 2){
								mchPBEM_SetFlag(PBEM_LATE_TURN);
								mchPBEM_DropFlag(PBEM_NEED_REFRESH);
							}
							else 
								mchPBEM_DropFlag(PBEM_LATE_TURN);
						}
						else {
							mchPBEM_SetFlag(PBEM_ERROR);
						}
#else
						mchPBEM_DataFlag = 0;
						mchPBEM_HiddenTime = 0;
						mchPBEM_DropFlag(PBEM_CONNECTED | PBEM_ERROR | PBEM_SHOW_GAME);
#endif
					}
					if(!mchPBEM_CheckFlag(PBEM_ERROR) && mchPBEM_CheckFlag(PBEM_SET_REPLAY_MODE)){
						mchPBEM_SetFlag(PBEM_REPLAY_MODE);
						mchPBEM_DropFlag(PBEM_SET_REPLAY_MODE);
					}
					if(!mchPBEM_CheckFlag(PBEM_ERROR | PBEM_LATE_TURN) && mchPBEM_CheckFlag(PBEM_NEED_REFRESH)){
						mchPBEM_DropFlag(PBEM_ERROR);
						mchPBEM_SetFlag(PBEM_SHOW_GAME | PBEM_DATA_SENT | PBEM_CHECK_RETURN);

						mchPBEM_DataFlag = -1;
						mchPBEM_HiddenTime = 1;

						sgPrepareRefreshData();

						mchA_SetTurnFlag(0,-1);

						mchPBEM_DropFlag(PBEM_NEED_REFRESH);
					}
					return;
			}
		}

		if(d3dIsActive()){
/*			if(mchPBEM_CheckFlag(PBEM_SHOW_GAME)){
				mchDrawQuant();
			}
			else
				mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);*/

			mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);

			gb_IGraph3d->BeginScene();

			mchA_DrawOnlineScreen();

			gb_IGraph3d->EndScene();
			gb_IGraph3d->Flush();
		}
		if(!mchPBEM_CheckFlag(PBEM_CONNECTED)){
#ifndef _OFFLINE_DEBUG_
			if(mchPBEM_DataFlag == -1){
				if(og_inP.post_url()){
					wi_D.connect(og_inP.post_url(),og_inP.post_port());
					url = og_inP.post_path_url();
				}
				else {
					wi_D.connect(wiServerName,wiServerPort);
					url = wiGameURL;
				}

				header = "Content-type: application/x-www-form-urlencoded\r\n";
				wi_D.open_request(WI_POST,url,header,strlen(header),wi_D.output_buffer(),wi_D.output_size());
			}
			else {
				wi_D.connect(wiServerName,wiServerPort);
				obj_buf < wiGameURL < "?ID=" <= mchPBEM_GameID < "&USERID=" <= mchPBEM_PlayerUID;
				wi_D.open_request(WI_GET,obj_buf);
			}
#endif
			mchPBEM_SetFlag(PBEM_CONNECTED);
		}
	}

	while(KeyBuf -> size){
		k = KeyBuf -> get();

		if(k == VK_ESCAPE)
			quantRTO -> NextID = XT_TERMINATE_ID;

		if(mchPBEM_CheckFlag(PBEM_ERROR) && k == 'R')
			mchPBEM_DropFlag(PBEM_CONNECTED | PBEM_ERROR);
	}

	if(mchPBEM_CheckFlag(PBEM_ERROR) && ogCheckRefreshTime()){
		mchPBEM_DropFlag(PBEM_CONNECTED | PBEM_ERROR);
	}
}

void mchRecorderPrepare(void)
{
	int sz;
	char* buf;

	mchEnableTrackDemo = 0;
	mchEnableStartCounter = 0;

	XRec.Open(mchXRecorderName,mchXRecorderMode);
	acsBackupControls();
	buf = (char*)mchSaveGameParams(sz);

	switch(mchXRecorderMode){
		case XRC_RECORD_MODE:
			XRec.PutMessage(XRC_USER_MESSAGE,sz / sizeof(int),buf);
			buf = (char*)acsGetControlsBackupBuf(sz);
			XRec.PutMessage(XRC_USER_MESSAGE,sz / sizeof(int),buf);
			break;
		case XRC_PLAY_MODE:
			XRec.GetMessage();
			mchLoadGameParams(XRec.nextMsg -> data,XRec.nextMsg -> DataSize);
			XRec.GetMessage();
			acsRestoreControls(XRec.nextMsg -> data);
			XRec.GetMessage();
			break;
	}
	XKey.clear();
}

void mchRecorderDone(void)
{
	int num;
	XRec.Close();
	acsRestoreControls();
	mchRestoreGameParams();

	mchFreeResources();

	if(mch_iScreen){
		num = acsInitSaveSlots();
		if(num < 1) mch_iScreenID = ACS_MAIN_MENU_SCR0;
		if(num == 1) mch_iScreenID = ACS_MAIN_MENU_SCR1;
		if(num > 1) mch_iScreenID = ACS_MAIN_MENU_SCR2;
	}

	switch(mchXRecorderMode){
		case XRC_RECORD_MODE:
			break;
		case XRC_PLAY_MODE:
			break;
	}

	XKey.clear();
	mchXRecorderMode = 0;
}

void LoadingRTO::Init(int id)
{
	int sz;

	XStream fh;
	XBuffer obj_buf;

	char* header;

	mchA_d3dCreateBackBuffer();

	mchA_PrepareLoadingImage(mchCurrentWorld,mchCurrentTrack);
	startTimer = clocki();

#ifdef _WIN32
	if(mchPBEM_DataFlag){
		wi_D.connect(wiServerName,wiServerPort);

		if(mchPBEM_DataFlag == 1){
			if(!mchPBEM_PacketFile){
				obj_buf < wiGameURL < "?ID=" <= mchPBEM_GameID < "&USERID=" <= mchPBEM_PlayerUID;
				wi_D.open_request(WI_GET,obj_buf);
			}
			else {
				fh.open(mchPBEM_PacketFile);
				sz = fh.size();
				wi_D.alloc_inbuf(sz);
				fh.read(wi_D.input_buffer(),sz);
				fh.close();

				wi_D.set_status(WI_LOAD_FILE);
			}
		}
		else {
			header = "Content-type: application/x-www-form-urlencoded\r\n";
			wi_D.open_request(WI_POST,wiGameURL,header,strlen(header),wi_D.output_buffer(),wi_D.output_size());
		}
	}
#endif
}

int LoadingRTO::Quant(void)
{
	int v,ret = 0;

	v = ((clocki() - startTimer) >> 1);
	if(v >= 255 && !mchPBEM_DataFlag){ 
		ret = NextID;
	}

	if(v < 0) v = 0;
	if(v > 255) v = 255;

	v = 255 - v;

	if(d3dIsActive())
		mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);

#ifdef _WIN32
	if(mchPBEM_DataFlag){
		ogQuant();

		switch(wi_D.get_status()){
			case WI_ERROR:
				if(ogGetErrorStr())
					ErrH.Abort(ogErrorStr);
				else
					ErrH.Abort(iGetText(iTXT_BAD_PACKET));
				break;
			case WI_FINISHED:
				ret = NextID;
				break;
		}

		gb_IGraph3d->BeginScene();
		mchA_d3dOutString(5,460,mchA_FontScaleX[2],mchA_FontScaleY[2],ogStatus(),mchA_ColorF[6],160,2,0,0.8f);
		gb_IGraph3d->EndScene();
		if(v >= 255)
			gb_IGraph3d->Flush();
	}
#endif

	if(v < 255){
		gb_IGraph3d->BeginScene();
		gb_IGraph3d->SetViewColor(0,0,0,v);
		gb_IGraph3d->EndScene();
		gb_IGraph3d->Flush();
	}

	return ret;
}

void LoadingRTO::Finit(void)
{
	mchPBEM_DataFlag = 0;
}

void ShowImageRTO::SetNumFiles(int num)
{ 
	numFiles = num;
	curFile = 0; 
}

void mchInitLogs(void)
{
#ifdef _MCH_INIT_LOGS_
	static int flag = 0;

	if(flag) return;

	flag = 1;

	if(!SetCurrentDirectory("LOGS")){
		CreateDirectory("LOGS",NULL);
		return;
	}

	char* fname = XFindFirst("*.*");

	while(fname){
		DeleteFile(fname);
		fname = XFindNext();
	}

	SetCurrentDirectory("..\\");
#endif
}

void mchChangeTimeSpeed(int dir)
{
	if(!dir)
		mchTimeSpeedCur = 1.0f;

	if(dir > 0)
		mchTimeSpeedCur *= mchTimeSpeedDelta;
	if(dir < 0)
		mchTimeSpeedCur /= mchTimeSpeedDelta;

	if(mchTimeSpeedCur < mchTimeSpeedMin)
		mchTimeSpeedCur = mchTimeSpeedMin;
	if(mchTimeSpeedCur > mchTimeSpeedMax)
		mchTimeSpeedCur = mchTimeSpeedMax;

	resource_dispatcher.set_speed(mchTimeSpeedCur);
	camera_dispatcher -> setTimeSpeed(mchTimeSpeedCur);
}

void mchInitOutro(void)
{
	int num;

	mch_imageRTO -> SetNumFiles(1);

	mch_imageRTO -> ClearFlag(0);
	mch_imageRTO -> SetText(iGetText(iTXT_OUTRO),1,0,0,0);
	mch_imageRTO -> SetFlag(0,IMG_RTO_NO_IMAGE | IMG_RTO_START_MUSIC | IMG_RTO_SCROLL_TEXT | IMG_RTO_NO_DISABLE);

	mch_imageRTO -> SetNext(RTO_MAIN_MENU_ID);
	quantRTO -> SetNext(RTO_SHOW_IMAGE_ID);

	if(mch_iScreen){
		num = acsInitSaveSlots();
		if(num < 1) mch_iScreenID = ACS_MAIN_MENU_SCR0;
		if(num == 1) mch_iScreenID = ACS_MAIN_MENU_SCR1;
		if(num > 1) mch_iScreenID = ACS_MAIN_MENU_SCR2;
	}

	mchFreeResourcesFlag = 1;
}

void IntroMovieRTO::Init(int id)
{
	mchIntroMovieActive = 1;

	mchStopSound();
	sndMusicStop();
	sndMusicSetVolume(mchSoundVolume);

	mchLoadIntroMovieSound();

	XGR_Obj.set_clip(0,0,XGR_MAXX,XGR_MAXY);

	if(!mch_introMovieD){
		mch_introMovieD = new mchIntroMovieDispatcher();
		mch_introMovieD -> init();
	}

	mch_introMovieD -> start();

	mchStartSoundEFF(EFF_INTRO_AMBIENCE);
}

int IntroMovieRTO::Quant(void)
{
	int k,ret = 0;

	if(d3dIsActive()){
		ret = mch_introMovieD -> quant();

		gb_IGraph3d->Flush();
		gb_IGraph3d->Fill(0,0,0);
	}
	else
		mch_introMovieD -> set_time();
	
	while(KeyBuf -> size){
		k = KeyBuf -> get();
		if(k == VK_ESCAPE){
			ret = 1;
			KeyBuf -> clear();
			break;
		}
		if(k == VK_SPACE || k == VK_MOUSE_LBUTTON || k == VK_MOUSE_RBUTTON){
			ret = mch_introMovieD -> skip();
			KeyBuf -> clear();
			break;
		}
	}
	
	if(ret)
		return NextID;

	return 0;
}

void IntroMovieRTO::Finit(void)
{
	mchStopSoundEFF(EFF_INTRO_AMBIENCE);
	sndMusicStop();
	sndMusicSetVolume(mchMusicVolume);

	delete mch_introMovieD;
	mch_introMovieD = NULL;

//	mchReInitGraph(xgrGameMode);

	mchIntroMovieActive = 0;
}
