#ifndef	__MECHOSOMA_H__
#define __MECHOSOMA_H__

// mchGameWindow::flags...
#define MCH_WND_FULLSCREEN		0x01
#define MCH_WND_CENTERED		0x02
#define MCH_WND_ALIGN_LEFT		0x04
#define MCH_WND_ALIGN_RIGHT		0x08
#define MCH_WND_ALIGN_TOP		0x10
#define MCH_WND_ALIGN_BOTTOM		0x20
#define MCH_WND_640x480			0x40

struct mchGameWindow
{
	int flags;

	int PosX;
	int PosY;

	int CenterX;
	int CenterY;

	int SizeX;
	int SizeY;

	int SizeX2;	// SizeX / 2...
	int SizeY2;	// SizeY / 2...

	void set(int cx,int cy,int sx,int sy);
	void set(mchGameWindow* p);

	void set_clipping(void);
	void remove_clipping(void);

	void init(int mode = -1);

	int check(int x,int y){ 
		if(x >= (CenterX - SizeX2) && x < (CenterX + SizeX2) && y >= (CenterY - SizeY2) && y < (CenterY + SizeY2)) 
			return 1;
		return 0;
	};

	mchGameWindow(void){ flags = 0 ; };
	~mchGameWindow(void){ };
};

extern mchGameWindow* gameWnd;
extern mchGameWindow* gameWnd0;
extern mchGameWindow* gameWnd1;

extern mchGameWindow* iWnd;
extern mchGameWindow* iWnd0;
extern mchGameWindow* iWnd1;

// Camera's params
extern float fCamera;
extern float mchCameraX;
extern float mchCameraY;
extern float hCamera;
extern float mchCameraAX;
extern float mchCameraAY;
extern float mchCameraAZ;


extern int mchCurrentWorld;
extern int mchCurrentTrack;
extern int mchFreezeTime;
extern int mchTrackDemoMode;

extern int mchDebugMode;

extern char* mch_mainINI;
extern char* mch_hotseatINI;
extern char* mch_optionsINI;

// Global flags...
#define MCH_STOP_TIME_FLAG		0x01
#define MCH_START_TIME_FLAG		0x02
#define MCH_TOGGLE_PAUSE_FLAG		0x04
#define MCH_TIMEOUT			0x08
extern int mchGameFlags;

/* ------------------------ HotSeat ---------------------------- */

enum mchTimeModes
{
	MCH_TIME_STOPPED = 1,
	MCH_TIME_WAITING_TURN,	// 2
	MCH_TIME_RUNNING,	// 3
	MCH_TIME_GAME_OVER	// 4
};

extern int mchTimeMode;

enum mchPBEM_GameModes {
	MCH_PBEM_EDIT_TURN = 1,
	MCH_PBEM_REPLAY_TURN,
	MCH_PBEM_VIEW_TURN,
	MCH_PBEM_END_TURN
};

enum mchGameModes
{
	MCH_SINGLE_GAME = 1,
	MCH_SPLIT_CONTROL_HS,
	MCH_ENTIRE_CONTROL_HS
};

// mchPBEM_Flags
const int PBEM_CONTROL_DATA_SENT	= 0x01;
const int PBEM_SHOW_GAME		= 0x02;
const int PBEM_CONNECTED		= 0x04;
const int PBEM_ERROR			= 0x08;
const int PBEM_DATA_SENT		= 0x10;
const int PBEM_CHECK_RETURN		= 0x20;
const int PBEM_START_TIME		= 0x40;
const int PBEM_CHANGE_PLAYER		= 0x80;
const int PBEM_REPLAY_MODE		= 0x100;
const int PBEM_SET_REPLAY_MODE		= 0x200;
const int PBEM_NEED_REFRESH		= 0x400;
const int PBEM_LATE_TURN		= 0x800;

extern int mchGameMode;
extern int mchTurnBasedGame;
extern int mchSplitScreenGame;

extern int mchPBEM_Game;
extern int mchPBEM_GameMode;	// edit/view turn, etc.
extern int mchPBEM_CurPlayer;
extern int mchPBEM_CurPlayer0;
extern int mchPBEM_PlayerUID;
extern int mchPBEM_TargetTurn;
extern int mchPBEM_Flags;

inline void mchPBEM_SetFlag(int fl){ mchPBEM_Flags |= fl; }
inline void mchPBEM_DropFlag(int fl){ mchPBEM_Flags &= ~fl; }
inline int mchPBEM_CheckFlag(int fl){ return (mchPBEM_Flags & fl); }

#define HS_NAME_LEN		7
#define HS_PLAYERS_MAX		5

struct hsPlayerData 
{
	char name[256];
	int type; // 0 - disabled, 1 - human, 2 - AI-1, ..., N - AI-N-1
	int TSD_Count;
	char configStr[256];

	void SetName(char* p){ strcpy(name,p); TSD_Count = 0; }

	hsPlayerData(void){ type = 0; }
	~hsPlayerData(void){ }
};

extern hsPlayerData hsPlayers[];

extern int hsStartAssembled;
extern int hsTurnSeedsMax;
extern int hsWaitTurnTime;
extern int hsTurnPrepareTime;
extern int hsTurnTime;
extern int hsCameraLimit;
extern int hsSeedsAccumulation;
extern int hsReturnMax;
extern int hsWorld;
extern int hsTrack;
extern int hsRndSeed;

void hsSetup(void);
void hsSaveParams(void);
void mchSetTimeMode(int m);
void mchSetGameMode(int m);

/*----------------------- Options ------------------------ */

struct mchOptionData
{
	int ID;

	int LimitMin;
	int LimitMax;
	int Value;

	char* keyName;
	void* iScreenObj;

	int GetValue(void){ return Value; }

	void Update(void);

	void GetObjectValue(void);
	void SetObjectValue(void);

	void InitValue(void);

	void SetKey(char* p){ keyName = strdup(p); }
	void Init(void);
	void Save(void);
	void Reset(void);

	mchOptionData(void){ keyName = NULL; iScreenObj = NULL; }
};

extern mchOptionData mchOptions[];

extern int xgrInitMode;

/*-------------------- end of Options -------------------- */

void mchInitControls(void);
int mchGetKeyID(int key,int config = -1);

int mchGetKeyID_First(int key,int config = -1);
int mchGetKeyID_Next(void);

int mchKeyPressed(int id,int config = -1);

char* getIniKey(char* fname,char* section,char* key);
void putIniKey(char* fname,char* section,char* key,char* val);
void putIniKey(char* fname,char* section,char* key,int val);

unsigned mch_clock(void);

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);
int m3dOpenResource(char* fname,XStream& fh);
void mchXResourceInit(void);
int vmapOpenResource(char* fname,XStream& fh);
int mch_vmapResourceInit(char * Path2World=NULL);


void mchCreateBonusArcane(void* p);

#define _MCH_LOG_

#ifdef _MCH_LOG_
extern XStream __mch_logFile;
#define __MCH_LOG_STR(s) { __mch_logFile.open("game.log",XS_OUT | XS_APPEND | XS_NOREPLACE); __mch_logFile < s < "\r\n"; __mch_logFile.close(); }
#else
#define __MCH_LOG_STR(s) 
#endif

// User-defined groups for statistics
enum {
	STAT_TOTAL,
	STAT_RESDISP,
	STAT_BODYDISP,
	STAT_BODYDISPPRE,
	STAT_MECHOS,
	STAT_CAMERA,
	STAT_M3D,
	STAT_RACEDISP,
	STAT_NUMERIC
};

#endif