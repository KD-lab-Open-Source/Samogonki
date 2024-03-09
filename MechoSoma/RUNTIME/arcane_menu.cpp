/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "iText.h"
#include "mch_rto.H"

#include "mechosoma.h"
#include "arcane.h"
#include "arcane_menu.h"
#include "ctl_point.h"
#include "race.h"
#include "sound.h"
#include "sound_api.h"

#include "SpriteDispatcher.h"
#include "CameraDispatcher.h"

#include "ResourceDispatcher.h"

#include "LocalVersion.h"

#include "TERRA.H"
#include "Mesh3ds.h"
#include "IGraph3d.h"

#include "HFONT.H"
#include "TGA.H"

#include "SCR_DEFS.H"
#include "ACI_IDS.H"
#include "aci_parser.h"
#include "ACI_SCR.H"
#include "controls.h"

#include "fxlabInterface.h"

#include "savegame.h"

#include "Md3d.h"

#include "arcane_menu_d3d.h"
#include "parts_pool.h"

#include "demo_dispatcher.h"

#include "M3d_effects.h"

#include "KEYS.H"
#include "Chtree.h"

#include "I-World.h"
#include "TrackDispatcher.h"
#include "TrackRecorder.h"

#include "online_game.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */

struct mchA_TextStr 
{
	int X;
	int Y;

	int tr_flag;

	int fnt;
	int space;
	int color;

	char str[256];

	void draw(int alpha);
	void set(int y,int f,int sp,void* p,int col = 2,int tr = 0);
};

struct mchA_TextInfo
{
	int X;
	int Y;

	int SizeX;
	int SizeY;

	int CurX;
	int CurY;

	int font;
	int space;
	int v_space;

	void OutStr(int str_num,void* p,int col,int alpha,int center = 1);
	void GetStringPrm(int str_num,void* p,int center,int& x,int& y,int& sx,int& sy);

	int StrPosY(int str);

	mchA_TextInfo(const char* ini_file,const char* ini_section,const char* ini_key_prefix);
};

struct mchArcaneName
{
	int ID;
	const char* nameStr;
	const char* criticalHitStr;

	void* list;
	mchArcaneName* prev;
	mchArcaneName* next;

	mchArcaneName(void){ ID = -1; list = NULL; nameStr = NULL; criticalHitStr = NULL; }
	mchArcaneName(int id,const char* p){ ID = id; list = NULL; nameStr = p; criticalHitStr = NULL; }
	~mchArcaneName(void){ };
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern SystemResourceDispatcher resource_dispatcher;

extern int mchPBEM_HiddenTime;
extern int mchPBEM_DataFlag;
extern int mchPBEM_Pause;

extern int mchExitOnFinishFlag;
extern int mchSplitScreenMode;

extern int mchEnableTips;
extern int mchFreeResourcesFlag;
extern int MCH_CP_STAR_NUM;

extern int iWorldMode;
extern const char* mch_mainINI;

extern int acsNumFonts;
extern HFont** acsFntTable;

extern int mchRealClockStart;
extern int mchRealClockEnd;
extern int mchRealClockShow;

extern int mchRightMousePress;
extern int mchPartID[];
extern int mch_iScreenID;

extern int mchCurrentTrack;
extern int mchTurn;
extern int mchNumLaps;
extern int mchNumRacers;

extern int acsMaxEssenceID;
extern int mchEnableStartCounter;

extern int mchNextWorld;
extern int mchNextTrack;

extern int xgrGameMode;

extern xtList<mchRacerStats> mch_racerLst;

extern int acsActiveFlag;
extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

extern int mchSelectedWorld;
extern int mchWorldSelectFlag;

extern int mchHS_LoadFlag;
extern int mchTrackDemoStartTimer;
extern int mchTrackDemoCurTimer;

extern float mchTimeSpeedCur;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */


void mchA_DrawTextStr(mchA_TextStr* p,int sz,int alpha);

void mchA_SetOnlinePlaceOrder(int pl_id,int idx);
void mchA_BuildOnlinePlaceOrder(void);
int mchA_GetOnlinePlaceOrder(int idx);
void mchA_ChangeOnlinePlayer(int dir);

void mchA_DrawCharBitmap(int x,int y,int sx,int sy,const void* text,void* bitmap,int fnt,int sp = 0,int vsp = 0,float scale_x = 1.0f,float scale_y = 1.0f);
void mchA_CharBitmapTest(void);

void mchInitChoiceScreen(void);
void mchFinitTrackDemo(void);

void mchDrawScrollbar(int x,int y,int sx,int sy,int v,int max_v);
void mchSetNextQuantRTO(void);

void mchA_DrawTips(void);

int acsGetKeyName(int vkey,int shift,int lng_flag);
const char* mchGetKeyNameText(int code);

void acsStrLen3D(const void* str,int fnt,int space,int& sx,int& sy);
void mchA_StrLen3D(const void* str,int fnt,int space,int& sx,int& sy);
void mchA_OutString3D(float x,float y,const void* str,int fnt,int col,int space,int alpha,float z,float sc);
void mchInitPartIDs(void);

void mchA_DrawM3D(cMesh* p);
void mchA_DrawBonusPart(int id,int x,int y,int r,float alpha);

void mchA_d3dLoadBmp(int x,int y,int idx,int ch_idx,mchArcaneBMP* p);

int mchGetKey(int id,int cfg);
int mchGetKeysConfig(void);

void mchA_DrawRacersInfo(void);
void mchA_DrawRacersEnergy(void);
void mchA_DrawRacersPlace(void);
void mchA_DrawRacerPlace(int x,int y,int id,int active);
void mchA_DrawRacerFinishPlace(int x,int y,int id,int active,float ph,const char* name,int chr,int star,int time);
void mchA_DrawArrow(int x,int y,int sx,mchArcaneRacerSet* p = NULL);
void mchA_DrawMouse(int mode = 0);
void mchA_DrawMouseRect(int sx,int sy,int sz,int delta);
void mchA_DrawSeeds(void);
void mchA_DrawSleep(void);
void mchA_DrawPBEM_FinishScreen(void);
void mchA_DrawWaitingText(int color,int Alpha);

void mchA_DrawStar(int x,int y,float sz = 1.0f,float alpha = 1.0f);
void mchA_DrawSpeedStar(int x,int y,float sz = 1.0f);
void mchA_DrawBonus(int x,int y,int idx);

void mchResetRacers(void);
int mchA_ConvertCoords(int x,int y,int z,int& sx,int& sy,int viewport = 0);
int mchMouse3D_Visible(void);

void fxShowBitmap(const int sx,const int sy,unsigned char* sbuf,int dx,int dy,unsigned char* dbuf,double angle);
void mchFreeResources(int mode = 0);

void mch_scale_sprite(unsigned char* dest, unsigned char* src, int sx_src, int sy_src, int sx_dest, int sy_dest);

void mchPrepareA_MenuPal(void);

void acsOutStr16_a2(int x,int y,int fnt,int col,const unsigned char* str,const unsigned* pal,int space,int alpha);
void acsOutStr16_a2_clip(int x,int y,int fnt,int col,const unsigned char* str,const unsigned* pal,int space,int alpha);

int acsStrLen(int fnt,const unsigned char* str,int space);
int acsStrHeight(int fnt,const unsigned char* str);

void acsPutSpr16sz_a2(int x,int y,int sx,int sy,const void* buf,int col,int col_sz,const unsigned *pal,int alpha);
void acsPutSpr16sz_a2_clip(int x,int y,int sx,int sy,const void* buf,int col,int col_sz,const unsigned *pal,int alpha);
void acsRectangle(int x,int y,int sx,int sy,int col,const unsigned* pal,int alpha);

void mchA_ShowEnergy(int x,int y,float scale,float phase,float phase_scale,int alpha);
void mchA_ShowEnergyZ(int x,int y,int z,int sx,int sy,float phase,int alpha);
void mchA_ShowEssenceEnergy(int x,int y,float phase,int alpha,int id);

void mchGetMouseCoords3D(int& x,int& y,int& z);
void mchA_ShowTime(mchArcaneScreenElement* p,int tm);
void mchEndGameFinit(int mode = 0);

void mchBuyTrackPart(int mode = 1);

void mchA_LoadWorldMap(int m);
void mchA_ShowWorldMap(mchArcaneScreenElement* m);

void mchA_MemPutSpr(int x,int y,int sx,int sy,int bsx,void* p,void* scr);

void mchA_ShowRacerStatus(void);
void mchA_ShowLap(int x,int y,int al,mchArcaneRacerSet* owner);
void mchA_ShowLapEnable(int player);
void mchA_ShowStartCount(int al);
void mchA_ShowCount(int al,int time);
void mchA_ShowStartCountEnable(void);
void mchA_ShowStartCountDisable(void);

void mchA_DropCPDisable(mchRacer* p);

void mchA_ShowRacerPlace(int x,int y,int al,int finish = 0,mchArcaneRacerSet* owner = NULL);
void mchA_ShowRacerLap(int x,int y,int al,mchArcaneRacerSet* owner = NULL);

const char* mchA_GetNumSuffix(int v);

unsigned short* loadJPG(XStream *Xfile, int &IMGwidth, int &IMGheight);

void mchA_PrepareLoadingImage(int wrld,int tr);

void mchA_BuildMapImage(void);
void mchA_MapPutspr(int x,int y,int sx,int sy,char* spr,char* p);

void mchA_DrawRealClock(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define AE_SPRITE_ARCANES

#define AE_END_GAME_TIMER		1

#define AE_MAX_PHASE			2
#define MCH_ARCANE_CMENU_R		42
#define MCH_ARCANE_SMENU_R		40
//#define MCH_ARCANE_SMENU_R		48

#define AE_ARCANE_MENU_TIMER		50

#define AE_CMENU_DELTA		12
#define AE_SMENU_DELTA		20

// IDs
enum mchA_ObjectIDs {
	AE_END_GAME_STR = 512,
	AE_END_GAME_STR2,
	AE_TIME_STR,

	AE_LOST_STR,
	AE_TRY_STR,
	AE_AWAY_STR,
	AE_NOT_LOST_STR,
	AE_CONTINUE_STR,

	AE_FINISH_NSTAR,
	AE_WON_STR0,
	AE_WON_STR1,
	AE_WINNER_STR,
	AE_WINNER2_STR,

	AE_PART_EL,
	AE_PART_NAME,

	AE_BUY_STR,
	AE_PRICE_STR,
	AE_PRICE_STR2,
	AE_BUY_IT_STR,
	AE_NO_THANKS_STR,
	AE_CONTINUE2_STR,

	AE_NOT_ENOUGH_STR0,
	AE_NOT_ENOUGH_STR1,

	AE_TRY_STR2,
	AE_NEXT_TIME_STR,

	AE_FINISH_PLACE,

	AE_SMALL_RECT1,
	AE_SMALL_RECT2,

	AE_BIG_RECT1,
	AE_BIG_RECT2,

	AE_PLAY_AGAIN_STR,
	AE_AWAY_STR2,

	AE_MAX_ID
};

#define AE_PALETTE_SZ		5

const int MCH_AM_SX0	      = 0;
const int MCH_AM_SX1	      = 220;
const int MCH_AM_SX2	      = 60;

const int MCH_AM_SX			= MCH_AM_SX0 + MCH_AM_SX1 + MCH_AM_SX2;
const int MCH_AM_SY			= 51;
const int MCH_AM_FSY		= MCH_AM_SY * 10;

#define MCH_AM_NAME_FNT		1
#define MCH_AM_NAME_SPACE	1
#define MCH_AM_SEED_FNT		2
#define MCH_AM_TIME_FNT		1
#define MCH_AM_SEED_COUNT_FNT	1
#define MCH_AM_STAR_COUNT_FNT	1
#define MCH_AM_CRITICAL_FNT	2

#define MCH_AM_LAP_FNT		0
#define MCH_AM_LAP_SPACE	1

#define MCH_AM_WAIT0_FNT		0
#define MCH_AM_WAIT1_FNT		1
#define MCH_AM_WAIT0_SPACE		1
#define MCH_AM_WAIT1_SPACE		1
#define MCH_AM_CRITICAL_SPACE		1

#define MCH_AM_STATUS_FNT		2
#define MCH_AM_STATUS_SPACE		1

#define MCH_AM_NAME_FNT			1
#define MCH_AM_NAME_SPACE		1

#define MCH_AM_GAME_OVER_FNT		0
#define MCH_AM_GAME_OVER_SPACE		1

unsigned mchA_MenuPal[768];
unsigned mchA_MenuPal2[768];
mchArcaneMenu* mch_AMenu;
mchArcaneRoundMenu* mch_SMenu;
mchArcaneScreenDispatcher* mch_arcScrD;
mchArcaneStatsDispatcher* mch_arcStatD = NULL;

mchArcaneBMP* mchA_CurWorld = NULL;
mchArcaneBMP** mchA_MapPointer = NULL;

mchArcaneBMP** mchA_ShapeBMP;
mchArcaneBMP** mchA_iShapeBMP;

mchArcaneBMP* mchA_MouseCrossBMP;
mchArcaneBMP* mchA_MouseHandBMP;
mchArcaneBMP* mchA_ZzzBMP;

mchArcaneBMP** mchAShop_ShapeBMP;
mchArcaneBMP* mchA_PointerBMP;

int mchEnergyBarSize = 0;
unsigned short* mchEnergyBar = NULL;

int mchA_Alpha = 128;
int mchA_AlphaHideSpeed = 16;
int mchA_AlphaShowSpeed = 16;
int mchA_MenuAlphaHideSpeed = 16;
int mchA_MenuAlphaShowSpeed = 64;

int mchA_PointerPeriod = 4;
int mchA_PointerDelta = 20;
int mchA_PointerPhase = 0;
float mchA_PointerPhaseF = 0.0f;

int mchA_SeedSize = 32;
int mchA_SeedSymSize = 24;
int mchAMenu_SeedSize = 32;
int mchAMenu_SeedSymSize = 24;
int mchAShop_SeedSize = 50;
int mchAShop_SeedSymSize = 30;

int mchGameFinit = 0;
int mchA_SeedCur = 0;
XBuffer mchA_XBuf;
char mchA_String[256];

mchArcaneScreenElement* mchA_WorldMap = NULL;

mchArcaneScreenElement* mchA_NameStrEl;
mchArcaneScreenElement* mchA_CountEl = NULL;
mchArcaneScreenElement* mchA_CpEl = NULL;
mchArcaneScreenElement* mchA_NStarEl = NULL;
mchArcaneScreenElement* mchA_FigureEl[4];
mchArcaneScreenElement* mchA_FinishFigureEl[5];
mchArcaneScreenElement* mchA_FinishNStarEl;

mchArcaneScreenElement* mchA_TimeSpeedEl;

cMesh* mchA_BonusMesh = NULL;

const unsigned char* mchA_SymTempPtr = NULL;
const unsigned char* mchA_SymTempPtr2 = NULL;

int mchA_TimerMax = 0;
int mchA_TimerCur = 0;
int mchA_TimerStart = 0;

const char* mchA_DropStr = NULL;
const char* mchA_TryAgainStr = NULL;
const char* mchA_GoAwayStr = NULL;
const char* mchA_LostStr = NULL;
const char* mchA_ContinueStr = NULL;
const char* mchA_NotLostStr = NULL;
const char* mchA_WonSpellStr0 = NULL;
const char* mchA_WonSpellStr1 = NULL;
const char* mchA_WinnerStr = NULL;
const char* mchA_BuyStr = NULL;
const char* mchA_BuyItStr = NULL;
const char* mchA_NoThanksStr = NULL;
const char* mchA_NotEnoughStr0 = NULL;
const char* mchA_NextTimeStr = NULL;
const char* mchA_PlayAgainStr = NULL;
const char* mchA_PressStr = NULL;

char mchA_NotEnoughStr1[100];
char mchA_PriceStr[100];

cMesh* mchA_ArrowMesh = NULL;
cMesh* mchA_StarMesh = NULL;
cMesh* mchA_SpeedStarMesh = NULL;

//cMesh* mchA_BonusMesh[4];

xtList<mchArcaneName>* mchA_Names = NULL;

const unsigned char* mchNameStr = NULL;

float mchA_FontScaleX[5] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
float mchA_FontScaleY[5] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

float mchA_SeedScale[3];
float mchA_SeedSymScale[3];

int mchA_TypeIdx[3] = { AE_D3DSPR_CIRCLE, AE_D3DSPR_TRIANGLE, AE_D3DSPR_SQUARE };

unsigned mchA_ColorF[16];

int mchA_MAXX = 640;
int mchA_MAXY = 480;

/*
mchA_TextInfo mchA_FinishStats("RESOURCE\\ISCREEN\\iscreen.ini","finish screen","stats");
mchA_TextInfo mchA_FinishPlStats("RESOURCE\\ISCREEN\\iscreen.ini","finish screen","pl_stats");
mchA_TextInfo mchA_FinishPartInfo("RESOURCE\\ISCREEN\\iscreen.ini","finish screen","part_info");
*/

float mchA_ArrowAngleDelta = 0.1f;

aciCHTree* mchA_CHTree;

int mchA_NStarMax = 0;
int mchA_TrackPart = 0;
int mchA_TrackPartPrice = 0;

int mchA_NStarOld = 0;
int mchA_NStarNew = 0;

Mat3f mchA_PartM;
float mchA_Part_Angle = 0.0f;
float mchA_Part_dAngle = 2.0f;
int mchA_TmpTimer = 0;
int mchA_QuantTimer = 0;
Vect3f mchA_PartAxis = Vect3f(0,0,1);

int mchA_PartID[NUM_ACTIVE_PARTS];
int mchA_PartNum[NUM_ACTIVE_PARTS];

int mchA_DropCp = 0;
int mchA_DropCpLap = 0;
int mchA_DropIdx = 0;
int mchA_DropCnt = 0;

int mchA_EndGameMode = 0;

unsigned char mchA_TurnFlags[HS_PLAYERS_MAX];

int mchA_MouseSX;
int mchA_MouseSY;
float mchA_MouseScale;
int mchA_MouseXY_Flag = 0;

int mchFixStartCP = 0;

int mchA_OnlinePlace[HS_PLAYERS_MAX];

void mchA_TextStr::set(int y,int f,int sp,void* p,int col,int tr)
{
	Y = y;
	fnt = f;
	space = sp;
	color = col;

	tr_flag = tr;

	strcpy(str,(char*)p);

	X = (640 - acsStrLen(fnt,(unsigned char*)str,space)) / 2;
}

void mchA_TextStr::draw(int alpha)
{
	int sx;
	mchA_d3dOutString(X,Y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],str,mchA_ColorF[color],256 - alpha,fnt,space);

	if(tr_flag){
		sx = acsStrLen(fnt,(unsigned char*)str,space);
		mchA_d3dOutSprite(X - 30,Y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[color],256 - alpha,0.0f,1.0f);
		mchA_d3dOutSprite(X + sx + 30,Y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[color],256 - alpha,M_PI,1.0f);
	}
}


mchArcaneScreenElement::mchArcaneScreenElement(void)
{
	int i;

	align_x = align_y = 0;

	for(i = 0; i < 4; i ++)
		objDir[i] = 0;

	ID = -1;

	Timer = 0;

	flags = 0;
	type = AE_BASE_TYPE;
	dataType = 0;

	clockTimer = 0;
	clockPeriod = 20;

	list = 0;
	color = 1;

	exDataPtr = NULL;

	dAlpha = trgAlpha = 0;
	Alpha = 256;
	flags |= AE_HIDDEN;
	
	SizeX = SizeY = 0;

	R = R0 = Vect3f::ZERO;
	Scale = destScale = 1.0f;
	deltaScale = 0.02f;

	showDir = -1;
	showIndex = 0;

	phase = dest_phase = 1.0f;

	exDataPtr = NULL;
	for(i = 0; i < 5; i ++)
		exDataParam[i] = 0;

	owner = NULL;
}

mchArcaneScreenElement::~mchArcaneScreenElement(void)
{
	free();
}

void mchArcaneScreenElement::InitCoords(const char* name)
{
	int align;

	const char* p;
	XBuffer XBuf;

	XBuf.init();
	XBuf < name < "_x";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());
	if(strlen(p)) R.x = atoi(p);

	XBuf.init();
	XBuf < name < "_y";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());
	if(strlen(p)) R.y = atoi(p);

	XBuf.init();
	XBuf < name < "_sx";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());
	if(strlen(p)) SizeX = atoi(p);

	XBuf.init();
	XBuf < name < "_sy";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());
	if(strlen(p)) SizeY = atoi(p);

	XBuf.init();
	XBuf < name < "_show_dir";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());

	if(strlen(p)){
		showDir = atoi(p);

		XBuf.init();
		XBuf < name < "_show_idx";
		showIndex = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address()));

		if(showDir == 4){
			XBuf.init();
			XBuf < name < "_x0";
			R0.x = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address()));

			XBuf.init();
			XBuf < name < "_y0";
			R0.y = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address()));
		}
	}

	XBuf.init();
	XBuf < name < "_align_x";
	p = getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface",XBuf.address());
	if(strlen(p)){
		align = atoi(p);
		if(align == 0)
			R.x += SizeX/2;
		if(align == 1)
			R.x -= SizeX/2;
	}

	if(type == AE_3D_TEXT)
		R.x -= SizeX/2;

	if(RenderMode == XGRAPH_HICOLOR && XGR_MAXX != 640){
		R.x = (R.xi() * XGR_MAXX) / 640;
		R.y = (R.yi() * XGR_MAXY) / 480;
	}

	InitR();
}

void mchArcaneScreenElement::InitR(void)
{
	switch(showDir){
		case 0: // from left
			R0 = R1 = R;
			R0.x = -SizeX - 2;
			break;
		case 1: // from up
			R0 = R1 = R;
			R0.y = -SizeY - 2;
			break;
		case 2: // from right
			R0 = R1 = R;
			R0.x = 640 + 20;
			break;
		case 3: // from down
			R0 = R1 = R;
			R0.y = 480 + 2;
			break;
		case 4: // custom
			R1 = R;
			break;
		default:
			R0 = R1 = R;
			break;
	}
}

void mchArcaneScreenElement::InitRedraw(void)
{
	R1 = R;
	if(showDir != -1)
		R = R0 + (R - R0) * phase;
}

#define AE_D3D_CH_SIZE	2.0f
#define AE_D3D_CH_SIZE2 1.0f
void mchArcaneScreenElement::RedrawFnc(int x0,int y0)
{
	int x,y,sx,sy,sy0,ch_sx,al,tm;
	float sc;
	
	const int ds = 1;

	double angle;
	mchArcaneName* nm;
//	mchBonusArcaneData* bd;
	HChar* cp;

	unsigned ch;

	switch(type){
		case AE_ARROW:
			mchA_DrawArrow(R.xi() + SizeX/2,R.yi() + SizeY/2,SizeX,owner);
			break;
		case AE_RECTANGLE:
			mchA_d3dOutSprite(x0,y0,float(SizeX) / 16.0f,float(SizeY) / 16.0f,AE_D3DSPR_DUMMY,0,255);

			mchA_d3dOutSprite(x0,y0 - 1,float(SizeX) / 16.0f,1.0f,AE_D3DSPR_DUMMY,0,255);
			mchA_d3dOutSprite(x0,y0 + SizeY - 16,float(SizeX) / 16.0f,1.0f,AE_D3DSPR_DUMMY,0,255);

			mchA_d3dOutSprite(x0,y0,1.0f,float(SizeY) / 16.0f,AE_D3DSPR_DUMMY,0,255);
			mchA_d3dOutSprite(x0 + SizeX - 16,y0,1.0f,float(SizeY) / 16.0f,AE_D3DSPR_DUMMY,0,255);
			if(exDataPtr){
				if(ID == AE_SMALL_RECT1 && mchTrackDemoCurTimer < mchTrackDemoStartTimer + 1000) break;

				sx = acsStrLen(exDataParam[0],(unsigned char*)exDataPtr,exDataParam[1]);
				sy = acsStrHeight(exDataParam[0],(unsigned char*)exDataPtr);

				x = (SizeX - sx) / 2;
				y = (SizeY - sy) / 2;

				mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],exDataPtr,mchA_ColorF[color],256 - Alpha,exDataParam[0],exDataParam[1],1.0f);
			}
			break;
		case AE_BONUS_PART:
			mchA_DrawBonusPart(exDataParam[0],x0,y0,SizeX,1.0f - (float(Alpha - mchA_Alpha)) / (256.0f - float(mchA_Alpha)));
			break;
		case AE_WAITING_TEXT:
			mchA_DrawWaitingText(color,Alpha);
			break;
		case AE_3D_TEXT:
			mchA_OutString3D(x0 - round((Scale - 1.0f) * float(SizeX)/2.0f),y0 - round((Scale - 1.0f) * float(SizeY)/2.0f),exDataPtr,exDataParam[0],color,exDataParam[1],(256 - Alpha)/4,0.0f,Scale * 0.7f);
/*			mchA_d3dOutSprite(430,10,1.0f/16.0f,460.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - Alpha,0.0f);
			mchA_d3dOutSprite(280,10,1.0f/16.0f,460.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - Alpha,0.0f);
			mchA_d3dOutSprite(580,10,1.0f/16.0f,460.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - Alpha,0.0f);
			mchA_d3dOutSprite(x0 - round((Scale - 1.0f) * float(SizeX)/2.0f),y0 - round((Scale - 1.0f) * float(SizeY)/2.0f),Scale * float(SizeX)/16.0f,Scale * float(SizeY)/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - Alpha,0.0f);*/
			break;
		case AE_FINISH_FIGURE_FACE:
			mchA_DrawRacerFinishPlace(x0,y0,exDataParam[0],exDataParam[1],phase,(char*)exDataPtr,exDataParam[2],exDataParam[3],exDataParam[4]);
			if(phase == 1.0f)
				exDataParam[2] ++;
			break;
		case AE_FIGURE_FACE:
			if(mchSplitScreenGame) break;

			mchA_DrawRacerPlace(x0,y0,exDataParam[0],exDataParam[1]);
			break;
		case AE_START_COUNTER:
			mchA_ShowStartCount(Alpha);
			break;
		case AE_LAP_COUNTER:
			mchA_ShowLap(x0,y0,Alpha,owner);
			break;
		case AE_STAR_COUNTER:
			sc = (mchSplitScreenGame) ? 0.8f : 1.0f;
			mchA_XBuf.init();
			if(!owner){
				if(!iWorldMode)
					mchA_XBuf < "x" <= mch_raceD -> activeRacer -> NStar < "/" <= mchA_NStarMax;
				else
					mchA_XBuf < "x" <= mch_raceD -> activeRacer -> stPtr -> NStar;
			}
			else {
				if(!iWorldMode)
					mchA_XBuf < "x" <= owner -> owner -> NStar < "/" <= mchA_NStarMax;
				else
					mchA_XBuf < "x" <= owner -> owner -> stPtr -> NStar;
			}

			mchA_d3dOutString(x0 + 48,y0 + 8,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha,exDataParam[0],exDataParam[1],sc);
			mchA_DrawStar(x0 + 24,y0 + 24);
			break;
		case AE_SEED_COUNTER:
			mchA_XBuf.init();
			mchA_XBuf < "x" <= mch_raceD -> activeRacer -> numSeeds - mch_raceD -> activeRacer -> seedLst.size();

			mchA_d3dOutString(x0 + 40,y0 + 2,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha,exDataParam[0],exDataParam[1]);
			mchA_d3dOutSprite(x0,y0,1.25f,1.25f,AE_D3DSPR_CIRCLE,mchA_ColorF[color],256 - Alpha,0.0f);
			break;
		case AE_FINISH_STAR_COUNTER:
			mchA_DrawStar(x0 + round(float(SizeX/2) * Scale),y0 + round(float(SizeY/2) * Scale),3.0f * Scale,1.0f - (float(Alpha - mchA_Alpha)) / (256.0f - float(mchA_Alpha)));

			mchA_XBuf.init();

			if(mchA_NStarOld < 100) mchA_XBuf < "0";
			if(mchA_NStarOld < 10) mchA_XBuf < "0";

			sx = (Alpha <= mchA_Alpha) ? 0 : 255;

			mchA_XBuf <= mchA_NStarOld;
			mchA_OutString3D(x0,y0 + round(float(SizeY) * Scale),mchA_XBuf.address(),1,1,1,sx,0.0f,Scale);
	
/*				
			mchA_XBuf <= mchA_NStarOld;
			sx = acsStrLen(0,(unsigned char*)mchA_XBuf.address(),1);
			mchA_d3dOutString(x0 + (SizeX - sx)/2,y0 + SizeY,mchA_FontScaleX[0],mchA_FontScaleY[0],mchA_XBuf.address(),mchA_ColorF[3],256 - Alpha,0,1);
*/				
			if(Alpha == mchA_Alpha && phase == dest_phase && mchA_NStarOld != mchA_NStarNew){
				if(abs(mchA_NStarOld - mchA_NStarNew) > ds)
					mchA_NStarOld += (mchA_NStarOld < mchA_NStarNew) ? ds : -ds;
				else 
					mchA_NStarOld = mchA_NStarNew;

				mchStartSoundEFF(EFF_STARS_COUNTER);
			}
			break;
		case AE_SPEED_COUNTER:
			if(owner){
				sc = (mchSplitScreenGame) ? 0.8f : 1.0f;

				mchA_XBuf.init();
				mchA_XBuf < "x" <= owner -> owner -> SpeedCount();
				mchA_d3dOutString(x0 + 48,y0 + 8,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha,exDataParam[0],exDataParam[1],sc);
				mchA_DrawSpeedStar(x0 + 24,y0 + 24);
			}
			break;
		case AE_ARCANE_STATS:
			mchA_XBuf.init();
			mchA_XBuf <= exDataParam[1];
			sx = 5;
			mchA_d3dOutSprite(x0,y0,AE_D3D_CH_SIZE2,AE_D3D_CH_SIZE2,*mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha);

			if(dataType == MCH_AM_UNIQUE){
				sc = (mchSplitScreenGame) ? 0.8f : 1.0f;

				if(flags & AE_ROTATE)
					angle = 2.0 * M_PI/(double)clockPeriod * (double)clockTimer;
				else
					angle = 0.0;

				mchA_SprD -> DrawArcaneSymbol(x0 + sx,y0,exDataParam[0],(float)exDataParam[2] / (float)AE_D3DSPR_MAX_PHASE,angle,color,200,sc);
			}
			break;
		case AE_WORLD_MAP:
			mchA_ShowWorldMap(this);
			break;
		case AE_RACER_POINTER:
			mchA_d3dOutSpriteZ(x0,y0,R.zi(),1.0f,1.0f,AE_D3DSPR_POINTER,mchA_ColorF[color],256 - Alpha);
			break;
		case AE_TIME_TEXT:
			mchA_ShowTime(this,mchA_TimerMax - mchA_TimerCur);
			break;
		case AE_NAME_STATUS_STR:
			if(!owner) break;

			mchA_XBuf.init();
			mchA_XBuf < owner -> owner -> name < " w" <= mchCurrentWorld < "t" <= mchCurrentTrack;

			sc = 1.0f;

			if(flags & AE_HALF_ALPHA)
				al = Alpha/2;
			else
				al = Alpha;

			if(flags & AE_CENTERED){
				if(flags & AE_ALPHA_SCALE)
					sc = (256.0f - (float)Alpha) / (256.0f - (float)mchA_Alpha);

				sx = acsStrLen(exDataParam[0],(unsigned char*)mchA_XBuf.address(),exDataParam[1]);
				sy = acsStrHeight(exDataParam[0],(unsigned char*)mchA_XBuf.address());

				if(sc < 1.0f){
					sx = round((float)sx * sc);
					sy = round((float)sy * sc);
				}

				x = (SizeX - sx) / 2;
				y = (SizeY - sy) / 2;

				if(flags & AE_ALIGN_RIGHT && sx > SizeX) x = SizeX - sx;
			}
			else {
				if(flags & AE_ALIGN_RIGHT){
					sx = acsStrLen(exDataParam[0],(unsigned char*)mchA_XBuf.address(),exDataParam[1]);
					sy = acsStrHeight(exDataParam[0],(unsigned char*)mchA_XBuf.address());
					x = SizeX - sx;
					y = (SizeY - sy) / 2;
				}
				else
					x = y = 0;
			}
			mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],mchA_XBuf.address(),mchA_ColorF[color],256 - al,exDataParam[0],exDataParam[1],sc);
			break;
		case AE_DROP_TEXT:
			sc = 1.0f;

			if(flags & AE_HALF_ALPHA)
				al = Alpha/2;
			else
				al = Alpha;

			if(flags & AE_CENTERED){
				if(flags & AE_ALPHA_SCALE)
					sc = (256.0f - (float)Alpha) / (256.0f - (float)mchA_Alpha);

				sx = acsStrLen(exDataParam[0],(unsigned char*)exDataPtr,exDataParam[1]);
				sy = acsStrHeight(exDataParam[0],(unsigned char*)exDataPtr);

				if(sc < 1.0f){
					sx = round((float)sx * sc);
					sy = round((float)sy * sc);
				}

				x = (SizeX - sx) / 2;
				y = (SizeY - sy) / 2;

				if(flags & AE_ALIGN_RIGHT && sx > SizeX) x = SizeX - sx;
			}
			else {
				if(flags & AE_ALIGN_RIGHT){
					sx = acsStrLen(exDataParam[0],(unsigned char*)exDataPtr,exDataParam[1]);
					sy = acsStrHeight(exDataParam[0],(unsigned char*)exDataPtr);
					x = SizeX - sx;
					y = (SizeY - sy) / 2;
				}
				else
					x = y = 0;
			}
			mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],exDataPtr,mchA_ColorF[color],256 - al,exDataParam[0],exDataParam[1],sc);

			sy0 = sy;

			if(mchGameMode == MCH_SINGLE_GAME && !mchTurnBasedGame && !(flags & AE_DISABLED)){
				if(owner -> owner -> State() == DestroyedRacer){
					SetState(0);
					break;
				}

				tm = mch_clock() - owner -> dropTimer;
				if(tm >= 5000){
					SetState(0);
					mch_raceD -> CenterRacer(owner -> owner);
				}
				else 
					mchA_ShowCount(Alpha,tm);
			}
			break;
		case AE_TIME_SPEED:
			if(mchTimeSpeedCur < 1.0f)
				sc = -1.0f / mchTimeSpeedCur;
			else 
				sc = mchTimeSpeedCur;

			if(fabs(mchTimeSpeedCur - 1.0f) < 0.09f)
				sprintf(mchA_String,"\C2\F0\E5\EC\FF 1.0");
			else
				sprintf(mchA_String,"\C2\F0\E5\EC\FF %+.1f",sc);

			sx = acsStrLen(1,(unsigned char*)mchA_String,0);
			sy = acsStrHeight(1,(unsigned char*)mchA_String);

			x = (640 - sx) / 2;
			y = 20;
			mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha,1,0);
			break;
		case AE_TEXT:
		case AE_STATUS_STR:
			sc = 1.0f;

			if(flags & AE_HALF_ALPHA)
				al = Alpha/2;
			else
				al = Alpha;

			if(flags & AE_CENTERED){
				if(flags & AE_ALPHA_SCALE)
					sc = (256.0f - (float)Alpha) / (256.0f - (float)mchA_Alpha);

				sx = acsStrLen(exDataParam[0],(unsigned char*)exDataPtr,exDataParam[1]);
				sy = acsStrHeight(exDataParam[0],(unsigned char*)exDataPtr);

				if(sc < 1.0f){
					sx = round((float)sx * sc);
					sy = round((float)sy * sc);
				}

				x = (SizeX - sx) / 2;
				y = (SizeY - sy) / 2;

				if(flags & AE_ALIGN_RIGHT && sx > SizeX) x = SizeX - sx;
			}
			else {
				if(flags & AE_ALIGN_RIGHT){
					sx = acsStrLen(exDataParam[0],(unsigned char*)exDataPtr,exDataParam[1]);
					sy = acsStrHeight(exDataParam[0],(unsigned char*)exDataPtr);
					x = SizeX - sx;
					y = (SizeY - sy) / 2;
				}
				else
					x = y = 0;
			}
			mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[exDataParam[0]],mchA_FontScaleY[exDataParam[0]],exDataPtr,mchA_ColorF[color],256 - al,exDataParam[0],exDataParam[1],sc);
			break;
		case AE_LAP_STR:
			mchA_ShowRacerLap(x0,y0,Alpha,owner);
			break;
		case AE_PLACE_STR:
			mchA_ShowRacerPlace(x0,y0,Alpha,0,owner);
			break;
		case AE_FINISH_PLACE_STR:
			mchA_ShowRacerPlace(x0,y0,Alpha,1);
			break;
		case AE_ENERGY_BAR:
			if(owner && owner -> owner -> Type == MCH_RACER_TYPE_MECHOS)
				mchA_ShowEnergy(R.xi(),R.yi(),1.0f,owner -> owner -> EnergyValue(),owner -> owner -> EnergyValueMax(),200);
			break;
		case AE_SEED_MARK:
			mchA_d3dOutSprite(x0,y0,1.0f,1.0f,mchA_TypeIdx[dataType],mchA_ColorF[color],256 - Alpha);
			if(dataType == MCH_AM_UNIQUE){
			nm = mchA_Names -> search(exDataParam[0]);
				if(nm){
					ch = nm -> nameStr[0];
					cp = acsFntTable[1] -> data[ch];
					ch_sx = cp -> SizeX - (cp -> LeftOffs + cp -> RightOffs);

					sx = (mchA_ShapeBMP[dataType] -> SizeX - ch_sx) / 2 + cp -> LeftOffs;
					sy = (mchA_ShapeBMP[dataType] -> SizeY - cp -> SizeY) / 2;

					mchA_d3dOutSprite(x0 + sx,y0 + sy,mchA_FontScaleX[1],mchA_FontScaleY[1],ch,mchA_ColorF[color],256 - Alpha);
				}
			}
			break;
		case AE_SEED_TEXT:
			mchA_d3dOutString(x0,y0,mchA_FontScaleX[MCH_AM_SEED_FNT],mchA_FontScaleY[MCH_AM_SEED_FNT],exDataPtr,mchA_ColorF[2],256 - Alpha,MCH_AM_SEED_FNT,MCH_AM_NAME_SPACE);
			break;
	}
}

void mchArcaneScreenElement::Redraw(void)
{
	int x,y;
	if(flags & AE_GLOBAL_COORDS){
		mchA_ConvertCoords(R.x,R.y,R.z,x,y);

		x -= SizeX/2;
		y -= SizeY/2;
	}
	else {
		x = R.xi();
		y = R.yi();
	}
	if(flags & AE_WINDOW_CLIP) gameWnd -> set_clipping();
	RedrawFnc(x,y);
	if(flags & AE_WINDOW_CLIP) gameWnd -> remove_clipping();
}

void mchArcaneScreenElement::GetFon(void)
{
/*
	if(!(flags & AE_NO_FON)){
		if(!fonPtr) alloc();
		XGR_GetSpr16(PosX,PosY,SizeX,SizeY,fonPtr);
	}

	flags |= AE_GETFON;
*/
}

void mchArcaneScreenElement::PutFon(void)
{
/*
	if(!(flags & AE_NO_FON) && fonPtr && flags & AE_GETFON)
		XGR_PutSpr16(PosX,PosY,SizeX,SizeY,fonPtr,XGR_BLACK_FON);

	flags &= ~AE_GETFON;
*/
}

mchArcaneMenuElement::mchArcaneMenuElement(void)
{
	ID = -1;
	flags = 0;
	list = 0;

	color = 0;
	PosX = PosY = 0;

	SizeX = MCH_AM_SX;
	SizeY = MCH_AM_SY;

	arcaneName = NULL;
	arcaneChar = 0;
	arcaneType = 0;
	arcaneID = -1;
}

mchArcaneMenuElement::~mchArcaneMenuElement(void)
{
}

void mchArcaneMenuElement::redraw(int x0,int y0,int alpha0,int rflag)
{
	int x,y;
	float sc;

	if(RenderMode == XGRAPH_HICOLOR)
		return;

	if(!rflag){
		if(arcaneName){
			sc = (mchSplitScreenGame) ? 0.8f : 1.0f;

			x = PosX + MCH_AM_SX0;
			y = PosY;

			mchA_d3dOutString(x0 + x,y0 + y,mchA_FontScaleX[MCH_AM_NAME_FNT],mchA_FontScaleY[MCH_AM_NAME_FNT],arcaneName,mchA_ColorF[color],256 - alpha0,MCH_AM_NAME_FNT,MCH_AM_NAME_SPACE,sc);
		}
	}
	else {
		mchA_SprD -> DrawArcaneSymbol(x0 + PosX,y0 + PosY,arcaneID,0.0f,0.0f,2,256 - alpha0,0.8f * scale,1);
		mchA_d3dOutSprite(x0 + PosX,y0 + PosY,1.25f * scale,1.25f * scale,AE_D3DSPR_CIRCLE,mchA_ColorF[7],256 - alpha0,0.0f,1);
	}
}

mchArcaneMenu::mchArcaneMenu(void)
{
	int i;
	flags = AE_HIDDEN;

	optionsLst = new xtList<mchArcaneMenuElement>;

//	addElement(MCH_AM_RECALL,-1);
//	p = optionsLst -> last();
//	p -> arcaneName = (unsigned char*)mchA_RecallStr;

	for(i = 0; i < NUM_ACTIVE_PARTS - 1; i ++)
		addElement(MCH_AM_UNIQUE,-2);
/*
	for(i = 0; i < 3; i ++){
		addElement(MCH_AM_BONUS,-2);
		p = optionsLst -> last();
		p -> flags |= AE_HIDDEN;
	}
*/
	InitCoords("arcane_menu");	

	SizeX = MCH_AM_SX;
	SizeY = MCH_AM_FSY;

	type = AE_AMENU;

	statD = NULL;
}

mchArcaneRoundMenu::mchArcaneRoundMenu(int tp)
{
	R_ = (tp == AE_CMENU) ? MCH_ARCANE_CMENU_R : MCH_ARCANE_SMENU_R;
	Phase = 0;
	curEl = optionsLst -> first();
	prevEl = NULL;
	mouseEl = NULL;

	statusStr = new unsigned char[256];

	textEl = NULL;

	SizeX = SizeY = R_ * 2 + mchAMenu_SeedSize * 2;

	type = tp;
	flags |= AE_NO_FON | AE_WINDOW_CLIP;

	addElement(MCH_AM_RECALL,-1);
}

mchArcaneRoundMenu::~mchArcaneRoundMenu(void)
{
	delete statusStr;
}

void mchArcaneRoundMenu::SetState(int st)
{
	if(textEl){
		if(!st)
			textEl -> SetState(0);
		else {
			if(!Active()) 
				textEl -> SetTimer(AE_ARCANE_MENU_TIMER);
		}
	}
	mchArcaneScreenElement::SetState(st);
}

void mchArcaneMenu::SetState(int st)
{
	if(st){
		if(flags & (AE_HIDDEN | AE_DISABLED)){
			SetAlphaSeq(Alpha,-mchA_AlphaShowSpeed,mchA_Alpha);
			flags &= ~(AE_HIDDEN | AE_DISABLED);
			mchStartSoundEFF(EFF_MENU_SHOW);
		}
	}
	else {
		if(!(flags & (AE_HIDDEN | AE_DISABLED))){
			flags |= AE_DISABLED;
			SetAlphaSeq(Alpha,mchA_AlphaHideSpeed,256);
			mchStartSoundEFF(EFF_MENU_SHOW);
		}
		Timer = 0;
	}
}

mchArcaneMenu::~mchArcaneMenu(void)
{
	mchArcaneMenuElement* p = optionsLst -> first();
	while(p){
		optionsLst -> remove(p);
		delete p;
		p = optionsLst -> first();
	}
	delete optionsLst;
}

void mchArcaneMenu::addElement(int tp,int id)
{
	mchArcaneMenuElement* p = new mchArcaneMenuElement;

	p -> arcaneID = id;
	p -> arcaneType = tp;

	p -> ID = optionsLst -> size();
	optionsLst -> append(p);
}

void mchArcaneMenu::removeElement(int tp,int id)
{
	mchArcaneMenuElement* p = optionsLst -> first();

	while(p){
		if(p -> arcaneType == tp && p -> arcaneID == id){
			optionsLst -> remove(p);
			delete p;
			return;
		}
		p = p -> next;
	}

}

void mchArcaneRoundMenu::InitRedraw(void)
{
	int x,y;
	if(textEl){
		mchA_ConvertCoords(R.xi(),R.yi(),R.zi(),x,y);

		textEl -> R.x = x - textEl -> SizeX/2;
		textEl -> R.y = y + SizeY/2;
		textEl -> R.z = R.z;

		if(type == AE_CMENU) textEl -> R.y -= AE_CMENU_DELTA;
		else textEl -> R.y -= AE_SMENU_DELTA;

		mchA_XBuf.init();
		if(curEl -> arcaneType != MCH_AM_RECALL){
			mchA_XBuf < (char*)curEl -> arcaneName;
			if(curEl -> arcaneType == MCH_AM_UNIQUE){
				mchA_XBuf < " ";
				if(curEl -> arcaneSeed >= curEl -> arcaneSeedMax)
					mchA_XBuf < (unsigned char)215 < "/" <= curEl -> arcaneSeedMax;
				else
					mchA_XBuf <= curEl -> arcaneSeed < "/" <= curEl -> arcaneSeedMax;
			}
		}
		strcpy((char*)statusStr,mchA_XBuf.address());
		x = acsStrLen(textEl -> exDataParam[0],(unsigned char*)textEl -> exDataPtr,textEl -> exDataParam[1]);
		if(x > textEl -> SizeX){
			textEl -> free();
			textEl -> SizeX = x;
			textEl -> alloc();
		}
		textEl -> color = curEl -> color;
	}
}

void mchArcaneRoundMenu::RedrawFnc(int x0,int y0)
{
	int i,sz,mx,my,mx0,my0,mz0;
	float el_sz = 16.0f;

	float angle,x,y,dx,dy,dist,scale;
	mchArcaneMenuElement* p;

	Vect3f dr,r,r0;

	r0 = camera_dispatcher -> observationPoint();
	r0.z = 0.0f;

	r = R;
	r.z = 0.0f;

	dr = getDist(r,r0);
	if(dr.norm() > 900.0f) return;

	sz = optionsLst -> size();

	mchGetMouseCoords3D(mx0,my0,mz0);
	mchA_ConvertCoords(mx0,my0,mz0,mx,my);

	mx -= x0;
	my -= y0;
/*
	mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
	my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

	mx = round((float)mx / mchA_d3dResX) - x0;
	my = round((float)my / mchA_d3dResY) - y0;
*/
	scale = camera_dispatcher -> getScale(R);
	el_sz *= scale;

	mouseEl = NULL;

	p = optionsLst -> first();
	for(i = 0; i < sz; i ++){
		angle = 2.0f * i * M_PI / sz + M_PI / 2.0f;

		x = float(SizeX/2) + R_ * sin(angle) * scale;
		y = float(SizeY/2) + R_ * cos(angle) * scale;

		p -> PosX = x;
		p -> PosY = y;

		p -> scale = scale;
		if(p != curEl && abs(mx - x) <= el_sz && abs(my - y) <= el_sz){
			mchA_MouseXY_Flag = 1;
			mchA_MouseSX = x + x0;
			mchA_MouseSY = y + y0;

			mchA_MouseScale = camera_dispatcher -> getScale(R);

			p -> scale *= 1.1f;
			mouseEl = p;
		}

		p = p -> next;
	}

	if(!(curEl -> flags & AE_DISABLED))
		curEl -> color = 2;

	if(Phase){
		dx = SizeX/2 - curEl -> PosX;
		dy = SizeY/2 - curEl -> PosY;

		dist = sqrt(dx * dx + dy * dy);
		dx /= dist;
		dy /= dist;

		dist *= (float)(AE_MAX_PHASE - Phase) / (float)AE_MAX_PHASE;
		dx *= dist;
		dy *= dist;

		curEl -> PosX += dx;
		curEl -> PosY += dy;

		if(prevEl){
			dx = SizeX/2 - prevEl -> PosX;
			dy = SizeY/2 - prevEl -> PosY;

			dist = sqrt(dx * dx + dy * dy);
			dx /= dist;
			dy /= dist;

			dist *= (float)Phase / (float)AE_MAX_PHASE;
			dx *= dist;
			dy *= dist;

			prevEl -> PosX += dx;
			prevEl -> PosY += dy;
		}
	}
	else {
		curEl -> PosX = SizeX/2;
		curEl -> PosY = SizeY/2;
	}

	p = optionsLst -> first();
	while(p){
		if(!(p -> flags & AE_HIDDEN))
			p -> redraw(x0,y0,Alpha,1);
		p = p -> next;
	}
}

void mchArcaneRoundMenu::Change(void)
{
	Phase = AE_MAX_PHASE;
	prevEl = curEl;
	curEl = curEl -> next;
	if(!curEl) curEl = optionsLst -> first();

	while(curEl -> flags & AE_DISABLED){
		curEl = curEl -> next;
		if(!curEl) curEl = optionsLst -> first();
		if(curEl == prevEl) break;
	}

	if(prevEl == curEl) prevEl = NULL;
	if(textEl) textEl -> SetTimer(AE_ARCANE_MENU_TIMER);
	if(type == AE_SMENU) SetArcane(curEl);

	mchStartSoundEFF(EFF_ROUND_MENU_TOGGLE);
}

void mchArcaneRoundMenu::Change(int id)
{
	if(flags & AE_DISABLED) return;

	Phase = AE_MAX_PHASE;
	prevEl = curEl;

	curEl = optionsLst -> search(id - 1);
	if(!curEl || curEl -> flags & AE_DISABLED){
		curEl = prevEl;
		Phase = 0;
		return;
	}
	if(textEl) textEl -> SetTimer(AE_ARCANE_MENU_TIMER);
	if(type == AE_SMENU) SetArcane(curEl);

//	mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
}

void mchArcaneRoundMenu::Set(int id)
{
	mchArcaneMenuElement* el;

	el = optionsLst -> first();
	while(el){
		if(el -> arcaneID == id){ 
			curEl = el;
			return;
		}
		el = el -> next;
	}
}

void mchArcaneMenu::InitRedraw(void)
{
}

void mchArcaneMenu::RedrawFnc(int x0,int y0)
{
	int y = 0;
	mchArcaneMenuElement* p;

	p = optionsLst -> first();
	while(p){
		p -> PosX = 0;
		p -> PosY = y; 
		if(!(p -> flags & AE_HIDDEN))
			y += p -> SizeY;
		p = p -> next;
	}

	p = optionsLst -> first();
	while(p){
		if(!(p -> flags & (AE_HIDDEN | AE_NO_REDRAW))) p -> redraw(R.xi(),R.yi(),Alpha,0);
		p = p -> next;
	}
}

void mchArcaneRoundMenu::Quant(float dt)
{
	mchArcaneScreenElement::Quant(dt);
	if(Phase){ 
		if(!(--Phase)){
			if(curEl -> flags & AE_DISABLED){
				Change();
			}
		}
	}
}

void mchArcaneScreenElement::ScaleQuant(void)
{
	float d;
	if(!Active()) return;

	d = destScale - Scale;
	if(fabs(d) <= deltaScale)
		Scale = destScale;
	else
		Scale += (destScale > Scale) ? deltaScale : -deltaScale;
}

void mchArcaneScreenElement::PhaseQuant(int level,int mode)
{
	float d;
	int fl = 0;
	const float dp = 0.2f;

	if(!Active()) return;

	switch(mode){
		case AE_BUILD_RACERS_SCREEN:	
			if(level >= showIndex)
				fl = 1;
			break;
		case AE_BUILD_SCREEN:	
			if(level >= showIndex)
				fl = 1;
			break;
		case AE_SHOW_SCREEN:
//			if(level >= showIndex)
				fl = 1;
			break;
		case AE_HIDE_SCREEN:
//			if(level <= showIndex)
				fl = 1;
			break;
	}

	if(fl){
		d = dest_phase - phase;
		if(fabs(d) <= dp){
			phase = dest_phase;
			if(!dest_phase && flags & AE_FINISH_SCREEN_EL){
				flags |= AE_DISABLED | AE_HIDDEN;
				Alpha = 256;
			}
			if(fabs(d) >= 0.1f && phase > 0.95f && flags & AE_FINISH_SCREEN_EL && (type == AE_TEXT || type == AE_3D_TEXT))
				mchStartSoundEFF(EFF_LETTERS_MOVE_ADD);
		}
		else {
			if(phase < 0.1f && flags & AE_FINISH_SCREEN_EL && (type == AE_TEXT || type == AE_3D_TEXT))
				mchStartSoundEFF(EFF_LETTERS_MOVE);

			phase += (dest_phase) ? dp : -dp;
		}
	}
}

void mchArcaneScreenElement::Quant(float dt)
{
/*
	if(Timer){
		Timer --;
		if(!Timer){ 
			if(!Active())
				SetState(1);
			else
				SetState(0);
		}
	}
*/
	const float dtimer = 10.0f;

	if(Timer > 0.01f){
		Timer -= dtimer * dt;
		if(Timer <= 0.01f){ 
			if(!Active())
				SetState(1);
			else
				SetState(0);

			Timer = 0.0f;
		}
	}

	if(++clockTimer >= clockPeriod){ 
		clockTimer = 0;
		if(flags & AE_END_ROTATE)
			flags &= ~(AE_ROTATE | AE_END_ROTATE);
	}

	if(flags & AE_DISABLED && !dAlpha && !Timer){ 
		flags |= AE_HIDDEN;
		flags ^= AE_DISABLED;
	}
	if(dAlpha){
		Alpha += dAlpha;
		if(dAlpha > 0 && Alpha >= trgAlpha){
			Alpha = trgAlpha;
			dAlpha = 0;
		}
		if(dAlpha < 0 && Alpha <= trgAlpha){
			Alpha = trgAlpha;
			dAlpha = 0;
		}

	}
	if(flags & AE_BLINK && !dAlpha){
		if(Alpha == 256)
			SetAlphaSeq(256,-mchA_MenuAlphaShowSpeed,mchA_Alpha);
		else
			SetAlphaSeq(Alpha,mchA_MenuAlphaHideSpeed,256);
	}

	const float sc0 = 1.01f;
	const float sc1 = 1.15f;

	if(type == AE_3D_TEXT){
		if(flags & AE_SELECTED){
			if(Scale == destScale && Scale == sc1)
				destScale = sc0;
			if(Scale == destScale && Scale == sc0)
				destScale = sc1;

			if(destScale == 1.0f)
				destScale = sc1;
	
			color = 2;
		}
		else {
			destScale = 1.0f;
			color = 0;
		}
	}
}

void mchArcaneScreenElement::alloc(void)
{
	free();
/*
	if(!(flags & AE_NO_FON))
		fonPtr = new unsigned short[SizeX * SizeY];
*/
}

void mchArcaneScreenElement::free(void)
{
/*
	if(fonPtr) delete fonPtr;
	fonPtr = NULL;
*/
}

void mchPrepareA_MenuPal(void)
{
	int i;
	unsigned r,g,b;
	unsigned char palBuf[768];
	XStream fh;

	acsOpenResource("a_menu.pal",fh);
	fh.read(palBuf,768);
	for(i = 0; i < 256; i ++){
		r = palBuf[i * 3] >> 1;
		g = (XGR_HighColorMode) ? (palBuf[i * 3 + 1] >> 1) : palBuf[i * 3 + 1];
		b = palBuf[i * 3 + 2] >> 1;

		mchA_MenuPal[i] = XGR_RGB64K(r,g,b);
	}
	fh.close();

	int idx;
	for(i = 0; i < 8; i ++){
		idx = ((i + 1) << 5) - 1;
		r = palBuf[idx * 3] << 2;
		g = palBuf[idx * 3 + 1] << 2;
		b = palBuf[idx * 3 + 2] << 2;
		mchA_ColorF[i] = RGB_MAKE(r,g,b);
	}


	r = g = b = 64;
	mchA_ColorF[8] = RGB_MAKE(r,g,b);

	r = 255;
	g = 100;
	b = 0;
	mchA_ColorF[9] = RGB_MAKE(r,g,b);

	r = 0;
	g = 50;
	b = 255;
	mchA_ColorF[10] = RGB_MAKE(r,g,b);

	r = 255;
	g = 255;
	b = 50;
	mchA_ColorF[11] = RGB_MAKE(r,g,b);

	r = 255;
	g = 50;
	b = 50;
	mchA_ColorF[12] = RGB_MAKE(r,g,b);

	r = 255;
	g = 255;
	b = 255;
	mchA_ColorF[13] = RGB_MAKE(r,g,b);

	r = 255;
	g = 255;
	b = 0;
	mchA_ColorF[14] = RGB_MAKE(r,g,b);

	r = 255;
	g = 0;
	b = 0;
	mchA_ColorF[15] = RGB_MAKE(r,g,b);

	acsOpenResource("a_menu2.pal",fh);
	fh.read(palBuf,768);
	for(i = 0; i < 256; i ++){
		r = palBuf[i * 3] >> 1;
		g = (XGR_HighColorMode) ? (palBuf[i * 3 + 1] >> 1) : palBuf[i * 3 + 1];
		b = palBuf[i * 3 + 2] >> 1;

		mchA_MenuPal2[i] = XGR_RGB64K(r,g,b);
	}
	fh.close();

}

void mchArcaneMenu::ChangeState(void)
{
	mchArcaneMenuElement* p;
	if(flags & AE_HIDDEN){
		flags ^= AE_HIDDEN;
		SetAlphaSeq(256,-mchA_MenuAlphaShowSpeed,mchA_Alpha);
		mchStartSoundEFF(EFF_MENU_SHOW);

		p = optionsLst -> first();
		while(p){
			p -> flags &= ~AE_HIDDEN;
			p = p -> next;
		}
		Build();
		InitRedraw();
		p = optionsLst -> first();
		while(p){
			if(p -> arcaneType == MCH_AM_TIME){
				XGR_MouseSetPos(R.xi() + p -> PosX + p -> SizeX/2,R.yi() + p -> PosY + p -> SizeY/2);
				break;
			}
			p = p -> next;
		}
	}
	else {
		flags |= AE_DISABLED;
		SetAlphaSeq(Alpha,mchA_MenuAlphaHideSpeed,256);
//		mchStartSoundEFF(EFF_MENU_HIDE);
	}
}

void mchArcaneRoundMenu::MouseHandler(int bt)
{
	int i,x,y,mx,my,mx0,my0,mz0;
	float el_sz = 16.0f,scale;
	mchArcaneMenuElement* p;

	if(bt == 1){
		if(Active() && !Phase){
			Change();
		}
	}
	else {
		if(!bt){
			if(Active()){
				scale = camera_dispatcher -> getScale(R);
				el_sz *= scale;

				mchA_ConvertCoords(R.x,R.y,R.z,x,y);
/*
				mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
				my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

				mx = round((float)mx / mchA_d3dResX);
				my = round((float)my / mchA_d3dResY);
*/
				mchGetMouseCoords3D(mx0,my0,mz0);
				mchA_ConvertCoords(mx0,my0,mz0,mx,my);

				x -= SizeX/2;
				y -= SizeY/2;

				p = optionsLst -> first();
				for(i = 0; i < optionsLst -> size(); i ++){
					if(float(abs(mx - p -> PosX - x)) <= el_sz && float(abs(my - p -> PosY - y)) <= el_sz){
						Change(p -> ID + 1);
						return;
					}

					p = p -> next;
				}

			}
		}
/*
		if(bt == -1){
			if(Active() && mchRightMousePress && !Phase){
				Change();
			}
		}
*/
	}
}

void mchArcaneMenu::MouseHandler(int bt)
{
	int mx,my;
	mchArcaneMenuElement* p;

	if(!mchTurnBasedGame) return;

	if(bt == 0){
		if(!mch_raceD -> activeRacer -> seedLst.size()) return;

		p = optionsLst -> first();
		mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2 - R.xi();
		my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2 - R.yi();

		while(p){
			if(!(p -> flags & AE_DISABLED)){
				if(p -> check_coords(mx,my)){ 
//					mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
					p -> color = 2;
					if(p -> arcaneType != MCH_AM_RECALL){
						SetArcane(p);
					}
					else {
						mch_raceD -> activeRacer -> curSeed -> RemoveArcane(1);
					}
				}
			}
			p = p -> next;
		}
	}
	else {
		if(bt == -1 && !(flags & (AE_HIDDEN | AE_DISABLED))){
			p = optionsLst -> first();
			mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2 - R.xi();
			my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2 - R.yi();

			while(p){
				if(!(p -> flags & AE_DISABLED))
					if(p -> check_coords(mx,my)) p -> color = 2;

				p = p -> next;
			}
		}
	}
}

#define AE_BODY_SCALE	1.0f
void mchA_ShowEssenceEnergy(int x,int y,float phase,int alpha,int id)
{
	int col;
	if(phase < 0) phase = 0;

	col = (mch_raceD -> activeRacer -> flags & MCH_ESSENCE_SLIME) ? 4 : 2;

	if(RenderMode == DIRECT3D_HICOLOR){
		mchA_SprD -> DrawEssence(x,y,id,phase,0.0f,2,alpha);
	}
}

void mchInitArcaneScreen(void)
{
	int i,v;
	mchArcaneScreenElement* p;

	mchA_DropStr = iGetText(iTXT_DROP_CP);
	mchA_TryAgainStr = iGetText(iTXT_TRY_AGAIN);
	mchA_GoAwayStr = iGetText(iTXT_GO_AWAY);
	mchA_LostStr = iGetText(iTXT_LOST);
	mchA_ContinueStr = iGetText(iTXT_CONTINUE);
	mchA_NotLostStr = iGetText(iTXT_BONANZA_RACE);
	mchA_WonSpellStr0 = iGetText(iTXT_WON_SPELL0);
	mchA_WonSpellStr1 = iGetText(iTXT_WON_SPELL1);
	mchA_WinnerStr = iGetText(iTXT_WINNER);
	mchA_BuyStr = iGetText(iTXT_BUY_SPELL);
	mchA_BuyItStr = iGetText(iTXT_BUY_IT);
	mchA_NoThanksStr = iGetText(iTXT_NO_THANKS);
	mchA_NotEnoughStr0 = iGetText(iTXT_NOT_ENOUGH);
	mchA_NextTimeStr = iGetText(iTXT_NEXT_TIME);
	mchA_PlayAgainStr = iGetText(iTXT_PLAY_AGAIN);
	mchA_PressStr = iGetText(iTXT_PRESS_KEY);

	XStream fh;
	mchA_CHTree = new aciCHTree;
#ifdef _FINAL_VERSION_
	acsOpenResource("ARCANE\\dummy_f.bmp",fh);
#else
	acsOpenResource("ARCANE\\dummy.bmp",fh);
#endif
	mchA_CHTree -> load(fh);

	mchA_PartM.set(Vect3f(1,0,0),M_PI/2.0f);

	if(RenderMode == XGRAPH_HICOLOR){
		mchA_MAXX = XGR_MAXX;
		mchA_MAXY = XGR_MAXY;
	}

	mchNameStr = new unsigned char[HS_NAME_LEN + 1];

	mchA_SymTempPtr = new unsigned char[128*128];
	mchA_SymTempPtr2 = new unsigned char[128*128];

	XBuffer XBuf;

	for(i = 0; i < 8; i ++){
		mchA_ColorF[i] = RGB_MAKE(255,255,255);
	}
	for(i = 0; i < acsNumFonts; i ++){
		mchA_FontScaleX[i] = (float)acsFntTable[i] -> SizeX / (float)acsFntTable[AE_D3D_FONT] -> SizeX;
		mchA_FontScaleY[i] = (float)acsFntTable[i] -> SizeY / (float)acsFntTable[AE_D3D_FONT] -> SizeY;
	}

	mchA_SeedScale[0] = 1.0f;
	mchA_SeedScale[1] = (float)mchAMenu_SeedSize / (float)mchA_SeedSize;
	mchA_SeedScale[2] = (float)mchAShop_SeedSize / (float)mchA_SeedSize;

	mchA_SeedSymScale[0] = (float)mchA_SeedSymSize / (float)acsFntTable[AE_D3D_FONT] -> SizeY;
	mchA_SeedSymScale[1] = (float)mchAMenu_SeedSymSize / (float)acsFntTable[AE_D3D_FONT] -> SizeY;
	mchA_SeedSymScale[2] = (float)mchAShop_SeedSymSize / (float)acsFntTable[AE_D3D_FONT] -> SizeY;

	mchA_Names = new xtList<mchArcaneName>;

	mchA_Alpha = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","alpha"));

	v = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","alpha_hide_speed"));
	mchA_AlphaHideSpeed = (256 - mchA_Alpha) / v;

	v = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","alpha_show_speed"));
	mchA_AlphaShowSpeed = (256 - mchA_Alpha) / v;

	v = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","menu_alpha_hide_speed"));
	mchA_MenuAlphaHideSpeed = (256 - mchA_Alpha) / v;

	v = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","menu_alpha_show_speed"));
	mchA_MenuAlphaShowSpeed = (256 - mchA_Alpha) / v;

	mchA_MouseCrossBMP = new mchArcaneBMP("ARCANE\\ms_cross.bmp");
	mchA_MouseHandBMP = new mchArcaneBMP("ARCANE\\hand.bmp");

	mchA_ZzzBMP = new mchArcaneBMP("ARCANE\\zzz.bmp");

	mchA_PointerBMP = new mchArcaneBMP("ARCANE\\pointer.bmp");

	mchA_ShapeBMP = new mchArcaneBMP*[3];
//	mchA_ShapeBMP[MCH_AM_UNIQUE] = new mchArcaneBMP("ARCANE\\shape_cr.bmp");
	mchA_ShapeBMP[MCH_AM_UNIQUE] = new mchArcaneBMP("ARCANE\\shp_cr_s.bmp");
	mchA_ShapeBMP[MCH_AM_TIME] = new mchArcaneBMP("ARCANE\\shape_tr.bmp");
	mchA_ShapeBMP[MCH_AM_BONUS] = new mchArcaneBMP("ARCANE\\shape_sq.bmp");

	mchA_iShapeBMP = new mchArcaneBMP*[3];
//	mchA_iShapeBMP[MCH_AM_UNIQUE] = new mchArcaneBMP("ARCANE\\shape_ci.bmp");
	mchA_iShapeBMP[MCH_AM_UNIQUE] = new mchArcaneBMP("ARCANE\\shp_ci_s.bmp");
	mchA_iShapeBMP[MCH_AM_BONUS] = new mchArcaneBMP("ARCANE\\shape_si.bmp");

	mchAShop_ShapeBMP = new mchArcaneBMP*[3];
	mchAShop_ShapeBMP[MCH_AM_UNIQUE] = new mchArcaneBMP("ARCANE\\shape_cr.bmp");
//	mchAShop_ShapeBMP[MCH_AM_TIME] = new mchArcaneBMP("ARCANE\\shape_tr.bmp");
	mchAShop_ShapeBMP[MCH_AM_BONUS] = new mchArcaneBMP("ARCANE\\shape_sq.bmp");

	for(i = 0; i < 3; i ++){
		if(i != MCH_AM_UNIQUE)
			mchA_ShapeBMP[i] -> Scale(mchA_SeedSize,mchA_SeedSize);
	}
//	mchA_iShapeBMP[MCH_AM_UNIQUE] -> Scale(mchA_SeedSize,mchA_SeedSize);
	mchA_iShapeBMP[MCH_AM_BONUS] -> Scale(mchA_SeedSize,mchA_SeedSize);
//	mchA_iShapeBMP[MCH_AM_UNIQUE] -> Crop();
	mchA_iShapeBMP[MCH_AM_BONUS] -> Crop();

	mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> Scale(mchAShop_SeedSize,mchAShop_SeedSize);
//	mchAShop_ShapeBMP[MCH_AM_TIME] -> Scale(mchAShop_SeedSize,mchAShop_SeedSize);
	mchAShop_ShapeBMP[MCH_AM_BONUS] -> Scale(mchAShop_SeedSize,mchAShop_SeedSize);

	mch_arcScrD = new mchArcaneScreenDispatcher;

	mch_SMenu = new mchArcaneRoundMenu(AE_SMENU);
	mch_SMenu -> flags |= AE_GLOBAL_COORDS;
	mch_SMenu -> SetState(0);
	mch_arcScrD -> objList -> append(mch_SMenu);

/*	mch_AMenu = new mchArcaneMenu;
	if(RenderMode == XGRAPH_HICOLOR && XGR_MAXX != 640){
		mch_AMenu -> R.x = (mch_AMenu -> R.xi() * XGR_MAXX) / 640;
		mch_AMenu -> R.y = (mch_AMenu -> R.yi() * XGR_MAXY) / 480;
	}
*/
	mchPrepareA_MenuPal();

	mchA_CurWorld = new mchArcaneBMP(64,64);

	mchA_MapPointer = new mchArcaneBMP*[5];

	mchA_MapPointer[0] = new mchArcaneBMP("ARCANE\\map.bmp");
	mchA_MapPointer[1] = new mchArcaneBMP("ARCANE\\map2.bmp");
	mchA_MapPointer[2] = new mchArcaneBMP("ARCANE\\map3.bmp");
	mchA_MapPointer[3] = new mchArcaneBMP("ARCANE\\map_cp.bmp");
	mchA_MapPointer[4] = new mchArcaneBMP("ARCANE\\map_cp.bmp");
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_WORLD_MAP;
	p -> InitCoords("map");
	p -> color = 2;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
	mchA_WorldMap = p;
	mchA_SetWorldMapCoords();

	p = new mchArcaneScreenElement;
	p -> type = AE_ENERGY_BAR;
	p -> InitCoords("energy");
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_PLACE_STR;
	p -> InitCoords("place_str");
	p -> color = 2;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
*/
	p = new mchArcaneScreenElement;
	p -> type = AE_FINISH_PLACE_STR;
	p -> ID = AE_FINISH_PLACE;
	p -> InitCoords("finish_place_str");
	p -> color = 2;
	p -> SetState(0);
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_LAP_STR;
	p -> InitCoords("lap_str");
	p -> color = 2;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
*/
	p = new mchArcaneScreenElement;
	p -> type = AE_SEED_COUNTER;
	p -> InitCoords("seed_counter");
	const char *t = "999";
	p -> SetString(MCH_AM_SEED_COUNT_FNT,1,t);
	p -> color = 7;
	p -> SetState(0);
	mch_arcScrD -> objList -> append(p);
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_STAR_COUNTER;
	p -> InitCoords("star_counter");
	p -> SetString(MCH_AM_STAR_COUNT_FNT,1,"99x99");
	p -> color = 3;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
	mchA_NStarEl = p;
*/
	p = new mchArcaneScreenElement;
	p -> type = AE_FINISH_STAR_COUNTER;
	p -> ID = AE_FINISH_NSTAR;
	p -> InitCoords("finish_star_counter");
	p -> color = 3;
	p -> SetState(0);
	p -> deltaScale = 0.1f;
	mch_arcScrD -> objList -> append(p);
	mchA_FinishNStarEl = p;

	p = new mchArcaneScreenElement;
	p -> type = AE_RECTANGLE;
	p -> ID = AE_SMALL_RECT1;
	p -> SetString(1,1,mchA_PressStr);
	p -> InitCoords("small_rect1");
	p -> SetState(0);
	p -> color = 2;
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TIME_SPEED;
	p -> SetState(0);
	p -> color = 2;
	mch_arcScrD -> objList -> append(p);
	mchA_TimeSpeedEl = p;

	p = new mchArcaneScreenElement;
	p -> type = AE_RECTANGLE;
	p -> ID = AE_SMALL_RECT2;
	p -> InitCoords("small_rect2");
	p -> SetState(0);
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_ARROW;
	p -> InitCoords("arrow");
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
*/
	for(i = 0; i < 4; i ++){
		p = new mchArcaneScreenElement;
		p -> type = AE_FIGURE_FACE;
		p -> InitCoords("figure");
		p -> R.y += p -> SizeY * i;
		p -> SetState(1);
		p -> InitR();
		p -> showIndex += i;
		mch_arcScrD -> objList -> append(p);
		mchA_FigureEl[i] = p;
	}

	for(i = 0; i < 5; i ++){
		p = new mchArcaneScreenElement;
		p -> type = AE_FINISH_FIGURE_FACE;
		p -> InitCoords("finish_figure");
		p -> R.y += p -> SizeY * i;
		p -> SetState(0);
		p -> InitR();
		p -> showIndex += i;
		mch_arcScrD -> objList -> append(p);
		p -> flags |= AE_FINISH_SCREEN_EL;
		mchA_FinishFigureEl[i] = p;
	}
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_SPEED_COUNTER;
	p -> InitCoords("speed_counter");
	p -> SetString(MCH_AM_STAR_COUNT_FNT,1,"99x99");
	p -> color = 4;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
*/
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_TIME_STOP_COUNTER;
	p -> InitCoords("tsd_counter");
	p -> SetString(MCH_AM_SEED_COUNT_FNT,1,"99x99");
	p -> color = 4;
	p -> SetState(0);
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_NAME_STATUS_STR;
	strcpy((char*)mchNameStr,"  ");
	p -> SetString(MCH_AM_STATUS_FNT,MCH_AM_STATUS_SPACE,mchNameStr);
	p -> InitCoords("name_str");
	p -> flags |= AE_CENTERED | AE_ALIGN_RIGHT;
	p -> color = 2;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
	mchA_NameStrEl = p;
*/
	p = new mchArcaneScreenElement;
	p -> type = AE_RACER_POINTER;
	p -> flags |= AE_NO_FON | AE_WINDOW_CLIP;
	p -> SizeX = mchA_PointerBMP -> SizeX;
	p -> SizeY = mchA_PointerBMP -> SizeY;
	p -> color = 7;
	p -> SetState(1);
	mch_arcScrD -> objList -> append(p);
/*
	p = new mchArcaneScreenElement;
	p -> type = AE_LAP_COUNTER;
	p -> color = 2;
	p -> SetState(0);
	mch_arcScrD -> objList -> append(p);
	mchA_LapEl = p;
*/
	p = new mchArcaneScreenElement;
	p -> type = AE_START_COUNTER;
	p -> color = 2;
	p -> SetState(0);
	mch_arcScrD -> objList -> append(p);
	mchA_CountEl = p;

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(0,0,mchA_DropStr);
	p -> R.x = (640 - p -> SizeX)/2;
	p -> R.y = (480 - p -> SizeY)/2;
	p -> flags |= AE_ALPHA_SCALE | AE_CENTERED | AE_HALF_ALPHA;
	mch_arcScrD -> objList -> append(p);
	mchA_CpEl = p;

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_LOST_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(0,1,mchA_LostStr);
	p -> InitCoords("lost_str");
	p -> flags |= AE_HALF_ALPHA | AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_NOT_LOST_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(0,1,mchA_NotLostStr);
	p -> InitCoords("not_lost_str");
	p -> flags |= AE_HALF_ALPHA | AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_WON_STR0;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_WonSpellStr0);
	p -> InitCoords("won_str0");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_WON_STR1;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_WonSpellStr1);
	p -> InitCoords("won_str1");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_BUY_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_BuyStr);
	p -> InitCoords("buy_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_NOT_ENOUGH_STR0;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_NotEnoughStr0);
	p -> InitCoords("not_enough0_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_NOT_ENOUGH_STR1;
	p -> color = 2;
	p -> SetState(0);
	memset(mchA_NotEnoughStr1,0,100);
	p -> SetString(1,0,mchA_NotEnoughStr1);
	p -> InitCoords("not_enough1_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_PRICE_STR;
	p -> color = 2;
	p -> SetState(0);
	memset(mchA_PriceStr,0,100);
	p -> SetString(1,0,mchA_PriceStr);
	p -> InitCoords("price_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_PRICE_STR2;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_PriceStr);
	p -> InitCoords("price2_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TEXT;
	p -> ID = AE_PART_NAME;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString(1,0,mchA_WinnerStr);
	p -> InitCoords("bonus_part_str");
	p -> flags |= AE_FINISH_SCREEN_EL | AE_CENTERED;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_CONTINUE_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_ContinueStr);
	p -> InitCoords("continue_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_CONTINUE2_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_ContinueStr);
	p -> InitCoords("continue2_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_BUY_IT_STR;
	p -> objDir[0] = p -> objDir[2] = AE_NO_THANKS_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_BuyItStr);
	p -> InitCoords("buy_it_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_NO_THANKS_STR;
	p -> objDir[0] = p -> objDir[2] = AE_BUY_IT_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_NoThanksStr);
	p -> InitCoords("no_thanks_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_TRY_STR;
	p -> color = 2;
	p -> objDir[0] = p -> objDir[2] = AE_AWAY_STR;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_TryAgainStr);
	p -> InitCoords("try_again_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_PLAY_AGAIN_STR;
	p -> color = 2;
	p -> objDir[0] = p -> objDir[1] = p -> objDir[2] = p -> objDir[3] = AE_AWAY_STR2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_PlayAgainStr);
	p -> InitCoords("play_again_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_AWAY_STR2;
	p -> objDir[0] = p -> objDir[1] = p -> objDir[2] = p -> objDir[3] = AE_PLAY_AGAIN_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_GoAwayStr);
	p -> InitCoords("go_away2_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_TRY_STR2;
	p -> objDir[0] = p -> objDir[2] = AE_NEXT_TIME_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_TryAgainStr);
	p -> InitCoords("try_again2_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_NEXT_TIME_STR;
	p -> objDir[0] = p -> objDir[2] = AE_TRY_STR2;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_NextTimeStr);
	p -> InitCoords("next_time_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_WINNER_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_WinnerStr);
	p -> InitCoords("winner_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_WINNER2_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_WinnerStr);
	p -> InitCoords("continue_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_BONUS_PART;
	p -> ID = AE_PART_EL;
	p -> SetState(0);
	p -> InitCoords("bonus_part");
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_3D_TEXT;
	p -> ID = AE_AWAY_STR;
	p -> objDir[0] = p -> objDir[2] = AE_TRY_STR;
	p -> color = 2;
	p -> SetState(0);
	p -> SetString3D(1,0,mchA_GoAwayStr);
	p -> InitCoords("go_away_str");
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_TIME_TEXT;
	p -> Alpha = 256;
	p -> InitCoords("turn_time");

	mchA_XBuf.init();
	mchA_XBuf < " 00:00";
	p -> SetString(MCH_AM_WAIT0_FNT,MCH_AM_WAIT0_SPACE,mchA_XBuf.address());
	p -> color = 2;
	p -> flags |= AE_HIDDEN;
	p -> ID = AE_TIME_STR;
	mch_arcScrD -> objList -> append(p);

//	mch_arcScrD -> objList -> append(mch_AMenu);

//	mch_arcStatD = new mchArcaneStatsDispatcher;

	p = new mchArcaneScreenElement;
	p -> type = AE_RECTANGLE;
	p -> ID = AE_BIG_RECT1;
	p -> InitCoords("big_rect1");
	p -> SetState(0);
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_RECTANGLE;
	p -> ID = AE_BIG_RECT2;
	p -> InitCoords("big_rect2");
	p -> SetState(0);
	p -> flags |= AE_FINISH_SCREEN_EL;
	mch_arcScrD -> objList -> append(p);

	p = new mchArcaneScreenElement;
	p -> type = AE_WAITING_TEXT;
	p -> color = 2;
	p -> SetState(0);
	mch_arcScrD -> objList -> append(p);

	mch_arcScrD -> playerSet1 = new mchArcaneRacerSet;
	mch_arcScrD -> playerSet2 = new mchArcaneRacerSet;

	if(RenderMode == DIRECT3D_HICOLOR)
		mchA_d3dInit();

	mchA_InitSpriteDispatcher();

	mchA_ArrowMesh = MeshLibrary -> Find(M3D_TOTAL_TYPE(503,M3D_iSCREEN));
	mchA_StarMesh = MeshLibrary -> Find(M3D_TOTAL_TYPE(504,M3D_iSCREEN));
	mchA_SpeedStarMesh = MeshLibrary -> Find(M3D_TOTAL_TYPE(100,M3D_iSCREEN));

	mch_arcScrD -> phaseIndexMax = 0;
	p = mch_arcScrD -> objList -> first();
	while(p){
		if(p -> showIndex >= mch_arcScrD -> phaseIndexMax)
			mch_arcScrD -> phaseIndexMax = p -> showIndex;
		p = p -> next;
	}
	mch_arcScrD -> phaseIndexMax ++;
}

void mchA_InitNames(void)
{
	int i,j;
	mchArcaneName* nm;
	cMesh* mp = MeshLibrary -> First();

	mchInitPartIDs();

	for(j = 0; j < NUM_ACTIVE_PARTS; j ++){
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			if(mchA_PartID[i] == mchPartID[j])
				mchA_PartNum[j] = i;
		}
	}

	while(mp){
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
			if(mp -> Type & mchA_PartID[i]){
				if(mp -> arcaneData.ID != -1){
					nm = mchA_Names -> search(mp -> arcaneData.ID);
					if(!nm){
						nm = new mchArcaneName(mp -> arcaneData.ID,mp -> arcaneData.name);
						mchA_Names -> append(nm);
					}
					else {
						nm -> nameStr = mp -> arcaneData.name;
					}
				}
			}
		}
		mp = MeshLibrary -> Next();
	}
}

mchArcaneScreenDispatcher::mchArcaneScreenDispatcher(void)
{
	Visible = 1;
	phase = dest_phase = 1.0f;
	phaseIndex = 0;
	objList = new xtList<mchArcaneScreenElement>;
	flushLine = new mchFlushLine;

	screenR = screenG = screenB = screenA = 0;
	screenR_dest = screenG_dest = screenB_dest = screenA_dest = 0;

	activeEl = NULL;
}

mchArcaneScreenDispatcher::~mchArcaneScreenDispatcher(void)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		objList -> remove(p);
		delete p;
		p = objList -> first();
	}
	delete objList;
	delete flushLine;
}

void mchArcaneScreenDispatcher::GetFon(void)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		if(!(p -> flags & AE_HIDDEN))
			p -> GetFon();
		p = p -> next;
	}
}

void mchArcaneScreenDispatcher::SetFlush(int x,int y,int sx,int sy)
{
	flushLine -> put(x,y,sx,sy);
}

void mchArcaneScreenDispatcher::Flush(void)
{
	mchFlushData* p;
	if(gameWnd -> flags & MCH_WND_FULLSCREEN || RenderMode != XGRAPH_HICOLOR)
		flushLine -> clear();

	while(flushLine -> size){
		p = flushLine -> get();
		XGR_Flush(p -> X,p -> Y,p -> SizeX,p -> SizeY);
	}
}

void mchArcaneScreenDispatcher::Quant(void)
{
	int idt,fl,sx,sy;
	float dt,x,y,z,angle;

	idt = non_stop_global_time() - mchA_QuantTimer;
	mchA_QuantTimer = non_stop_global_time();

	dt = float(idt) / 1000.0f;

	mchArcaneScreenElement* p;

	if(mch_demoD.is_enabled())
		ToggleVisible(0);

	if(mchTurnBasedGame){
		if(mchTimeMode == MCH_TIME_STOPPED && mch_raceD -> activeRacer -> curSeed){
			mchA_ConvertCoords(mch_raceD -> activeRacer -> curSeed -> PosX,mch_raceD -> activeRacer -> curSeed -> PosY,mch_raceD -> activeRacer -> curSeed -> PosZ,sx,sy);

			mch_SMenu -> Build();
			mch_SMenu -> SetState(1);
			mch_SMenu -> Set(mch_raceD -> activeRacer -> curSeed -> arcaneData.ID);

			mch_SMenu -> R.x = mch_raceD -> activeRacer -> curSeed -> PosX;
			mch_SMenu -> R.y = mch_raceD -> activeRacer -> curSeed -> PosY;
			mch_SMenu -> R.z = mch_raceD -> activeRacer -> curSeed -> PosZ;

			mch_SMenu -> R0 = mch_SMenu -> R1 = mch_SMenu -> R;
		}
		else
			mch_SMenu -> SetState(0);
	}
	else
		mch_SMenu -> SetState(0);

	mchA_NStarMax = mch_trackD -> TrackLength() * MCH_CP_STAR_NUM * mchNumLaps;

	if(playerSet1 -> owner != mch_raceD -> activeRacer)
		playerSet1 -> SetOwner(mch_raceD -> activeRacer);
	playerSet1 -> menu -> Build();
	playerSet1 -> statD -> InitRedraw();

	if(mchSplitScreenGame){
		if(playerSet2 -> owner != mch_raceD -> activeRacer2)
			playerSet2 -> SetOwner(mch_raceD -> activeRacer2);
		playerSet2 -> menu -> Build();
		playerSet2 -> statD -> InitRedraw();
	}

	mchA_DrawRacersPlace();

	if(activeEl)
		activeEl -> flags |= AE_SELECTED;

	p = objList -> first();
	while(p){
		if(p != activeEl) p -> flags &= ~AE_SELECTED;
		p -> ScaleQuant();
		p = p -> next;
	}

#if defined(GPX) && defined(EMSCRIPTEN)
    static int oldShowMode = 0;
    if (oldShowMode != showMode) {
        oldShowMode = showMode;
        EM_ASM((
                if (Module.layers) {
                    Module.layers.showMode($0);
                }
                ), showMode);
    }
#endif


	switch(showMode){
		case AE_BUILD_SCREEN:
		case AE_BUILD_RACERS_SCREEN:
		case AE_SHOW_SCREEN:
			fl = 1;
			p = objList -> first();
			while(p){
				p -> PhaseQuant(phaseIndex,showMode);
				if(p -> Active() && p -> showIndex <= phaseIndex && p -> CheckPhase())
					fl = 0;
				p = p -> next;
			}
			if(fl)
				AdvancePhase();
			break;
		case AE_HIDE_SCREEN:
			fl = 1;
			p = objList -> first();
			while(p){
				p -> PhaseQuant(phaseIndex,showMode);
//				if(p -> Active() && p -> showIndex >= phaseIndex && p -> CheckPhase())
				if(p -> Active() && p -> CheckPhase())
					fl = 0;
				p = p -> next;
			}
			if(fl)
				AdvancePhase();
			break;
	}

	p = objList -> first();
	while(p){
		switch(p -> type){
			case AE_SEED_COUNTER:
				if((mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS) && mchTimeMode == MCH_TIME_STOPPED)
					p -> SetState(1);
				else
					p -> SetState(0);
				break;
			case AE_WAITING_TEXT:
				if(mchTimeMode == MCH_TIME_WAITING_TURN && (!mchHS_LoadFlag || mchPBEM_CheckFlag(PBEM_REPLAY_MODE)) && mchPBEM_GameMode != MCH_PBEM_VIEW_TURN && mchPBEM_GameMode != MCH_PBEM_END_TURN && !mchPBEM_HiddenTime)
					p -> SetState(1);
				else
					p -> SetState(0);
				break;
			case AE_RACER_POINTER:
				if(mchTimeMode == MCH_TIME_STOPPED)
					p -> SetState(1);
				else
					p -> SetState(0);
				x = mch_raceD -> activeRacer -> R().x;
				y = mch_raceD -> activeRacer -> R().y;
				z = mch_raceD -> activeRacer -> R().z + mch_raceD -> activeRacer -> radius() + 10.0f;

				if(!(camera_dispatcher -> cameraMode().mapMode && (mchCameraAX || mchCameraAZ)))
					y -= mch_raceD -> activeRacer -> radius();
				else {
					angle = M_PI * (float)mchCameraAZ / 180.0f;
					x -= round(mch_raceD -> activeRacer -> radius() * sin(angle));
					y -= round(mch_raceD -> activeRacer -> radius() * cos(angle));
				}	

				p -> R.z = mchA_ConvertCoords(x,y,z,sx,sy);

				p -> R.x = sx - p -> SizeX/2;
				p -> R.y = sy - p -> SizeY/2 - mchA_PointerPhase;
				break;
		}
		p = p -> next;
	}

	if(mchTimeMode != MCH_TIME_GAME_OVER && mchKeyPressed(MCH_KEY_ARCANE_MENU)){
		playerSet1 -> menu -> SetState(1);
		if(mchSplitScreenGame)
			playerSet2 -> menu -> SetState(1);
	}
	else {
		playerSet1 -> menu -> SetState(0);
		if(mchSplitScreenGame)
			playerSet2 -> menu -> SetState(0);
	}

	MouseHandler(-1);

	p = objList -> first();
	while(p){
		if(!(p -> flags & AE_HIDDEN))
			p -> Quant(dt);
		p = p -> next;
	}
}

extern Vect3f acsPart_dA;

void mchArcaneScreenDispatcher::Redraw(void)
{
	mchArcaneScreenElement* p = objList -> first();

	const int dr = 8;

	int dt = non_stop_global_time() - mchA_TmpTimer;
	mchA_TmpTimer = non_stop_global_time();

	mchA_Part_Angle += mchA_Part_dAngle * float(dt) / 1000.0f;
	if(mchA_Part_Angle > M_PI * 2.0f)
		mchA_Part_Angle -= M_PI * 2.0f;

	mchA_PointerPhaseF += (float)mchA_PointerDelta * float(dt) / 1000.0f;
	mchA_PointerPhase = round(mchA_PointerPhaseF);
	if(mchA_PointerPhase >= mchA_PointerPeriod || mchA_PointerPhase <= 0){ 
		mchA_PointerDelta = -mchA_PointerDelta;
	}

	if(mchA_PointerPhase > mchA_PointerPeriod){
		mchA_PointerPhase = mchA_PointerPeriod;
		mchA_PointerPhaseF = float(mchA_PointerPeriod);
	}

	if(mchA_PointerPhase < 0){
		mchA_PointerPhase = 0;
		mchA_PointerPhaseF = 0.0f;
	}

	if(mchTimeMode == MCH_TIME_WAITING_TURN && (!mchHS_LoadFlag || (mchPBEM_CheckFlag(PBEM_REPLAY_MODE) && mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER))) && mchPBEM_GameMode != MCH_PBEM_VIEW_TURN && mchPBEM_GameMode != MCH_PBEM_END_TURN){
		mchA_DarkenRect(0,0,XGR_MAXX,XGR_MAXY,150);
	}
	else {
		if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE) && mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER))
			mchA_DarkenRect(0,0,XGR_MAXX,XGR_MAXY,150);
	}

	if(mchTimeMode == MCH_TIME_GAME_OVER){
		gb_IGraph3d->SetViewColor(screenR,screenG,screenB,screenA);
		
		if(screenR != screenR_dest){
			if(abs(screenR - screenR_dest) < dr)
				screenR = screenR_dest;
			else
				screenR += (screenR < screenR_dest) ? dr : -dr;
		}
		if(screenG != screenG_dest){
			if(abs(screenG - screenG_dest) < dr)
				screenG = screenG_dest;
			else
				screenG += (screenG < screenG_dest) ? dr : -dr;
		}
		if(screenB != screenB_dest){
			if(abs(screenB - screenB_dest) < dr)
				screenB = screenB_dest;
			else
				screenB += (screenB < screenB_dest) ? dr : -dr;
		}
		if(screenA != screenA_dest){
			if(abs(screenA - screenA_dest) < dr)
				screenA = screenA_dest;
			else
				screenA += (screenA < screenA_dest) ? dr : -dr;
		}
	}
	
	InitRedraw();

	if(!iWorldMode){
		if(mchRealClockShow && mchTimeMode == MCH_TIME_GAME_OVER)
			mchA_DrawRealClock();

		mchA_DrawPBEM_FinishScreen();
		mchA_DrawSeeds();
		mchA_DrawSleep();

		if(Visible){
			while(p){
				if(!(p -> flags & (AE_HIDDEN | AE_FREE))){
					p -> Redraw();
					if(!(p -> flags & AE_GLOBAL_COORDS)) SetFlush(p -> R.xi(),p -> R.yi(),p -> SizeX,p -> SizeY);
				}
				p = p -> next;
			}
		}
/*
		playerSet1 -> Quant();
		if(mchSplitScreenGame)
			playerSet2 -> Quant();
*/
		if(Visible)
			mchA_DrawMouse();
		mchA_DrawRacersEnergy();
		mchA_DrawTips();
	}
	else {
		if(Visible){
			playerSet1 -> starsEl -> Redraw();
/*
			if(!iworld_D -> CurScreen()){
				playerSet1 -> energyEl -> R.x -= 100.0f;
				playerSet1 -> energyEl -> Redraw();
				playerSet1 -> energyEl -> R.x += 100.0f;
			}
*/
			if(!iworld_D -> ElectionaryMode()){
				playerSet1 -> statD -> Redraw();
				if(!iworld_D -> AssemblyMode() && !(playerSet1 -> menu -> flags & (AE_HIDDEN | AE_FREE)))
					playerSet1 -> menu -> Redraw();
			}
		}
		iworld_D -> Redraw();

		if(mchSplitScreenGame){
			if(Visible){
				playerSet2 -> starsEl -> Redraw();
/*
				if(!iworld_D2 -> CurScreen()){
					playerSet2 -> energyEl -> R.x -= 100.0f;
					playerSet2 -> energyEl -> Redraw();
					playerSet2 -> energyEl -> R.x += 100.0f;
				}
*/
				if(!iworld_D2 -> ElectionaryMode()){
					playerSet2 -> statD -> Redraw();
					if(!iworld_D2 -> AssemblyMode() && !(playerSet2 -> menu -> flags & (AE_HIDDEN | AE_FREE)))
						playerSet2 -> menu -> Redraw();
				}
			}
			iworld_D2 -> Redraw();
		}

		mchA_DrawMouse();
	}

	p = objList -> first();
	while(p){
		p -> R = p -> R1;
		p = p -> next;
	}

//	sndDumpStats();

	if(XGR_MouseObj.Visible())
		mchA_DrawMouse(1);
}

void mchArcaneScreenDispatcher::PutFon(void)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		if(!(p -> flags & AE_HIDDEN))
			p -> PutFon();
		p = p -> next;
	}
}

void mchArcaneScreenElement::MouseHandler(int bt)
{
	int mx;
	int my;

	if(!Active()) return;

	mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
	my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

	if(RenderMode == DIRECT3D_HICOLOR){
		mx = round((float)mx / mchA_d3dResX);
		my = round((float)my / mchA_d3dResY);
	}

	if(type == AE_3D_TEXT && bt == -1){
		if(dest_phase == 1.0f && CheckCoords(mx,my))
			mch_arcScrD -> SetActiveEl(ID);
	}

	if(ID == AE_TRY_STR || ID == AE_TRY_STR2 || ID == AE_PLAY_AGAIN_STR){
		if(CheckCoords(mx,my)){
			switch(bt){
				case 0:
				case 1:
					mchEndGameFinit(1);
					mchNextWorld = mchCurrentWorld;
					mchNextTrack = mchCurrentTrack;
					mchResetRacers();
					mchHS_LoadFlag = 0;
					break;
			}
		}
	}
	if(ID == AE_BUY_IT_STR){
		if(CheckCoords(mx,my)){
			switch(bt){
				case 0:
				case 1:
					mchBuyTrackPart();
					break;
			}
		}
	}

	if(ID == AE_NO_THANKS_STR){
		if(CheckCoords(mx,my)){
			switch(bt){
				case 0:
				case 1:
					mchInitChoiceScreen();
					break;
			}
		}
	}
	if(ID == AE_AWAY_STR2){
		if(CheckCoords(mx,my)){
			switch(bt){
				case 0:
				case 1:
					mchEndGameFinit(1);
					if(mchGameMode == MCH_SINGLE_GAME){
						mchNextWorld = 7;
						mchNextTrack = 0;
						mchResetRacers();
					}
					else {
						mch_iScreenID = ACS_SPLIT_HS_SCR;
						mchNextWorld = mchCurrentWorld;
						mchNextTrack = mchCurrentTrack;
					}
					break;
			}
		}
	}

	if(ID == AE_AWAY_STR || ID == AE_CONTINUE_STR || ID == AE_WINNER_STR || ID == AE_WINNER2_STR || ID == AE_CONTINUE2_STR || ID == AE_NEXT_TIME_STR){
		if(CheckCoords(mx,my)){
			switch(bt){
				case 0:
				case 1:
					mchEndGameFinit(1);
					mchNextWorld = 7;
					mchNextTrack = 0;
					break;
			}
		}
	}
}

void mchArcaneScreenDispatcher::MouseHandler(int bt)
{
	mchArcaneScreenElement* p = objList -> first(),*p1;

	if(mchTimeMode == MCH_TIME_GAME_OVER && mchA_EndGameMode != AE_RACE_LOST && phaseIndex < 199 && phaseIndex > 10){
		if(bt != -1) phaseIndex = 199;
	}

	while(p){
		p1 = p -> next;
		p -> MouseHandler(bt);
		p = p1;
	}
}

void mchArcaneMenu::Build(void)
{
	cMesh* m;
	int charge,col,charge_flag;
	mchArcaneMenuElement* p;

	mchMechosRacer* r;

	if(statD)
		statD -> Clear();

	if(owner){
		if(owner -> owner -> Type == MCH_RACER_TYPE_MECHOS)
			r = (mchMechosRacer*)owner -> owner;
		else
			return;
	}
	else {
		if(mch_raceD -> activeRacer -> Type == MCH_RACER_TYPE_MECHOS)
			r = (mchMechosRacer*)mch_raceD -> activeRacer;
	}

	p = optionsLst -> first();
	while(p){
		p -> Enable();
		switch(p -> arcaneType){
			case MCH_AM_UNIQUE:
				if(mch_trackD -> TrackBoss() && p -> ID){
					p -> Disable();
					p -> flags |= AE_HIDDEN;
					break;
				}

				m = r -> part_by_type(mchA_PartID[p -> ID]);

				if(!m){
					m = mchGetPartByType(mchA_PartNum[mchA_PartNum[p -> ID]],r -> stPtr -> last_config[mchA_PartNum[mchA_PartNum[p -> ID]]]);
					charge_flag = 0;
				}
				else 
					charge_flag = 1;

				if(m && m -> arcaneData.ID != -1){
					p -> arcaneID = m -> arcaneData.ID;
					p -> arcaneName = (unsigned char*)m -> arcaneData.name;

					if(charge_flag && m -> arcaneData.Status && m -> arcaneData.Power >= m -> arcaneData.ActivatePower) col = 2;
					else col = 1;

					if(statD){
						charge = (charge_flag) ? round(m -> arcaneData.Power * (float)AE_D3DSPR_MAX_PHASE / m -> arcaneData.MaxPower) : 0;
						if(charge > AE_D3DSPR_MAX_PHASE - 1) charge = AE_D3DSPR_MAX_PHASE - 1;
						statD -> Add(m -> arcaneData.ID,p -> ID,MCH_AM_UNIQUE,charge,col);
					}
				}
				else {
					p -> Disable();
					p -> flags |= AE_HIDDEN;
				}
				break;
			case MCH_AM_BONUS:
				p -> Disable();
				p -> flags |= AE_HIDDEN;
				break;
		}
		if(!mchTurnBasedGame)
			p -> color = 2;
		p = p -> next;
	}
}

mchArcaneBMP::mchArcaneBMP(const char* fname)
{
	ID = -1;
	list = 0;
	flags = 0;
	fileName = NULL;
	Load(fname);
}

mchArcaneBMP::mchArcaneBMP(void)
{
	ID = -1;
	list = 0;
	flags = 0;
	SizeX = SizeY = 0;

	fileName = NULL;
	frameData = NULL;
	data = NULL;
}

mchArcaneBMP::mchArcaneBMP(int sx,int sy)
{
	ID = -1;
	list = 0;
	flags = 0;
	fileName = NULL;
	frameData = NULL;

	SizeX = sx;
	SizeY = sy;

	numFrames = 1;
	data = new char[sx * sy];
}

void mchArcaneBMP::Load(const char* fname)
{
	int i;
	unsigned short sx,sy,sz;
	XStream fh;

	if(!fname) fname = fileName;

	const char *p = fname + strlen(fname) - 3;

	if(!stricmp(p,"bmp")){

		if(flags & AE_OPEN_FILE)
			fh.open(fname,XS_IN);
		else
			acsOpenResource(fname,fh);

		fh > sx > sy;

		numFrames = 1;
		data = new char[sx * sy];
		fh.read(data,sx * sy);

		fh.close();
	}
	else {
		if(!stricmp(p,"bml")){
			acsOpenResource(fname,fh);
			fh > sx > sy > sz;

			numFrames = sz;
			data = new char[sx * sy * sz];
			fh.read(data,sx * sy * sz);

			fh.close();
		}
	}

	SizeX = sx;
	SizeY = sy;

	frameData = new char*[numFrames];
	for(i = 0; i < numFrames; i ++)
		frameData[i] = data + i * SizeX * SizeY;

}

void mchArcaneBMP::Load(const char* fname,XStream& fh)
{
	int i;
	unsigned short sx,sy,sz;

	if(!fname) fname = fileName;

	const char *p = fname + strlen(fname) - 3;

	if(!stricmp(p,"bmp")){
		fh > sx > sy;

		numFrames = 1;
		data = new char[sx * sy];
		fh.read(data,sx * sy);

		fh.close();
	}
	else {
		if(!stricmp(p,"bml")){
			fh > sx > sy > sz;

			numFrames = sz;
			data = new char[sx * sy * sz];
			fh.read(data,sx * sy * sz);

			fh.close();
		}
	}

	SizeX = sx;
	SizeY = sy;

	frameData = new char*[numFrames];
	for(i = 0; i < numFrames; i ++)
		frameData[i] = data + i * SizeX * SizeY;

}

void mchArcaneBMP::Free(void)
{
	if(data){
		delete data;
		data = NULL;
	}
	if(frameData){
		delete frameData;
		frameData = NULL;
	}
}

void mchArcaneMenu::KeyHandler(int id)
{
	mchArcaneMenuElement* p;
	id -= 1;

	if(!mchTurnBasedGame && mchTimeMode != MCH_TIME_RUNNING) return;

	p = optionsLst -> first();
	while(p){
		if(p -> ID == id) break;
		p = p -> next;
	}
	if(p){
		if(!mch_trackD -> TrackBoss() || !p -> ID)
			SetArcane(p);
	}
}

void mchArcaneMenu::SetArcane(mchArcaneMenuElement* p)
{
	cMesh* m;
	if(!p) return;

	mchRacer* r;
	if(!owner) 
		r = mch_raceD -> activeRacer;
	else
		r = owner -> owner;

	if((!mchTurnBasedGame || mchTimeMode == MCH_TIME_RUNNING) && p -> arcaneType == MCH_AM_UNIQUE){
		if(r -> Type == MCH_RACER_TYPE_MECHOS){
			m = ((mchMechosRacer*)r) -> part_by_type(mchA_PartID[p -> ID]);
			if(m){
				if(m -> arcaneData.Status){
					if(m -> arcaneData.Power >= m -> arcaneData.ActivatePower){
						m -> arcaneData.ownerID = r -> ID;
						m -> arcaneData.fxlabStart();
					}
				}
				else
					m -> arcaneData.fxlabStop();
			}
		}
	}

	if(mchTurnBasedGame && mchTimeMode == MCH_TIME_STOPPED && r -> curSeed){
		if(p -> arcaneType == MCH_AM_UNIQUE){
			if(r -> Type == MCH_RACER_TYPE_MECHOS){
				m = ((mchMechosRacer*)r) -> part_by_type(mchA_PartID[p -> ID]);

				if(!m)
					m = mchGetPartByType(mchA_PartNum[mchA_PartNum[p -> ID]],r -> stPtr -> last_config[mchA_PartNum[mchA_PartNum[p -> ID]]]);

				if(m && m -> arcaneData.ID != -1){
					r -> curSeed -> SetArcane(m -> arcaneData,0,0);
					mchStartSoundEFF(EFF_ROUND_MENU_TOGGLE);
				}
			}
		}
		if(p -> arcaneType == MCH_AM_RECALL){
			r -> curSeed -> RemoveArcane();
			mchStartSoundEFF(EFF_ROUND_MENU_TOGGLE);
		}
	}
}

mchFlushLine::mchFlushLine(void)
{
	int i;
	table = new mchFlushData*[AE_FLUSH_LINE_SIZE];
	mem_heap = new char[AE_FLUSH_LINE_SIZE * sizeof(mchFlushData)];
	for(i = 0; i < AE_FLUSH_LINE_SIZE; i ++)
		table[i] = (mchFlushData*)(mem_heap + i * sizeof(mchFlushData));

	clear();
}

mchFlushLine::~mchFlushLine(void)
{
	delete mem_heap;
	delete table;
	clear();
}

void mchFlushLine::put(int x,int y,int sx,int sy)
{
	if(size < AE_FLUSH_LINE_SIZE){
		table[last_index] -> X = x;
		table[last_index] -> Y = y;
		table[last_index] -> SizeX = sx;
		table[last_index] -> SizeY = sy;

		last_index ++;
		if(last_index >= AE_FLUSH_LINE_SIZE)
			last_index = 0;
		size ++;
	}
//	else
//		ErrH.Abort("Flush buffer overflow...");
}

mchFlushData* mchFlushLine::get(void)
{
	mchFlushData* p;
	if(size){
		p = table[first_index];

		first_index ++;
		if(first_index >= AE_FLUSH_LINE_SIZE)
			first_index = 0;

		size --;
		return p;
	}
	return NULL;
}

void mchFlushLine::clear(void)
{
	size = 0;
	first_index = 0;
	last_index = 0;
}

void mch_scale_sprite(unsigned char* dest, unsigned char* src, int sx_src, int sy_src, int sx_dest, int sy_dest)
{
	int x,y;
	unsigned char* line_src;
	int sx_src_power = BitSR(sx_src);
	assert(1 << sx_src_power == sx_src);
	int dx = (sx_src << 16)/sx_dest;
	int dy = (sy_src << 16)/sy_dest;
	int fx = (1 << 15);
	int fy = (1 << 15);
	for(y = 0;y < sy_dest; y++){
		line_src = src + ((fy >> 16) << sx_src_power);
		fy += dy;
		fx = (1 << 15);
		for(x = 0;x < sx_dest; x++){
			*dest++ = line_src[fx >> 16];
			fx += dx;
		}
	}
}

void mchArcaneScreenElement::SetString(int fnt,int sp,const void* p)
{
	free();

	if(!(flags & AE_ALIGN_RIGHT)){
		SizeX = acsStrLen(fnt,(unsigned char*)p,sp);
		SizeY = acsStrHeight(fnt,(unsigned char*)p);
	}
	exDataPtr = p;

	exDataParam[0] = fnt;
	exDataParam[1] = sp;
}

void mchArcaneScreenElement::SetString3D(int fnt,int sp,const void* p)
{
	free();

	mchA_StrLen3D(p,fnt,sp,SizeX,SizeY);
	SizeX = round(float(SizeX) * 0.7f);
	SizeY = round(float(SizeY) * 0.7f);

	exDataPtr = p;

	exDataParam[0] = fnt;
	exDataParam[1] = sp;
}

mchArcaneScreenElement* mchArcaneScreenDispatcher::getEl_Type(int tp)
{
	mchArcaneScreenElement* p;

	p = objList -> first();
	while(p){
		if(p -> type == tp && p -> flags & AE_FREE)
			break;
		p = p -> next;
	}
	if(!p){
		p = new mchArcaneScreenElement;
		p -> type = tp;
		objList -> append(p);
	}
	p -> flags = 0;
	return p;
}

mchArcaneScreenElement* mchArcaneScreenDispatcher::getEl_ID(int id)
{
	mchArcaneScreenElement* p;

	p = objList -> search(id);
	return p;
}

void mchA_ShowTime(mchArcaneScreenElement* p,int tm)
{
	int hr,mn,sc;

	tm /= 1000;
	hr = tm / 3600;
	mn = (tm % 3600) / 60;
	sc = tm % 60;

	mchA_XBuf.init();

	if(mn < 10) mchA_XBuf < "0";
	mchA_XBuf <= mn < ":";

	if(sc < 10) mchA_XBuf < "0";
	mchA_XBuf <= sc;

	if(RenderMode == XGRAPH_HICOLOR)
		acsOutStr16_a2(p -> R.xi(),p -> R.yi(),p -> exDataParam[0],p -> color << AE_PALETTE_SZ,(unsigned char*)mchA_XBuf.address(),mchA_MenuPal,p -> exDataParam[1],p -> Alpha);
	else
		mchA_d3dOutString(p -> R.xi(),p -> R.yi(),mchA_FontScaleX[p -> exDataParam[0]],mchA_FontScaleY[p -> exDataParam[0]],mchA_XBuf.address(),mchA_ColorF[p -> color],256 - p -> Alpha,p -> exDataParam[0],p -> exDataParam[1]);
}

void mchA_SetTimer(int tm)
{
	mchArcaneScreenElement* p;

	mchA_TimerStart = non_stop_global_time();
	mchA_TimerCur = mchA_TimerMax = tm * 1000;
	mchA_QuantTimer = mchA_TmpTimer = non_stop_global_time();

	if(mchGameMode != MCH_SINGLE_GAME){
		p = mch_arcScrD -> objList -> search(AE_TIME_STR);
		if(p){
			if(tm) p -> SetState(1);
			else p -> SetState(0);
		}
	}
}

void mchArcaneBMP::Crop(void)
{
	int i,y,sx,sy;
	int dx00,dy00,dx01,dy01;
	int idx = 0,idx1 = 0;
	char* ptr;

	CalcBound();
	dx00 = bound[0];
	dx01 = bound[1];
	dy00 = bound[2] * SizeX;
	dy01 = bound[3] * SizeX;

	sx = SizeX - (dx00 + dx01);
	sy = SizeY - (dy00 + dy01) / SizeX;
	if(sx < 1) sx = 1;
	if(sy < 1) sy = 1;

	ptr = new char[sx * sy * numFrames];

	for(i = 0; i < numFrames; i ++){
		idx1 = sx * sy * i;
		idx = dx00 + dy00;
		for(y = 0; y < sy; y ++){
			memcpy(ptr + idx1,frameData[i] + idx,sx);
			idx += SizeX;
			idx1 += sx;
		}
	}

	for(i = 0; i < numFrames; i ++){
		frameData[i] = ptr + sx * sy * i;
	}

	delete[] data;
	data = ptr;
	SizeX = sx;
	SizeY = sy;
}

void mchArcaneBMP::Expand(int sx,int sy)
{
	int i,y,idx0,idx1,dx,dy;
	char* ptr;

	dx = (sx - SizeX) / 2;
	dy = (sy - SizeY) / 2;

	if(dx < 0 || dy < 0) return;
	ptr = new char[sx * sy * numFrames];
	memset(ptr,0,sx * sy * numFrames);

	for(i = 0; i < numFrames; i ++){
		idx0 = SizeX * SizeY * i;
		idx1 = sx * sy * i + dx + dy * sx;

		for(y = 0; y < SizeY; y ++){
			memcpy(ptr + idx1,frameData[i] + idx0,SizeX);
			idx0 += SizeX;
			idx1 += sx;
		}
	}

	for(i = 0; i < numFrames; i ++){
		frameData[i] = ptr + sx * sy * i;
	}

	delete data;
	data = ptr;
	SizeX = sx;
	SizeY = sy;
}

void mchArcaneBMP::Save(const char* fname)
{
	XStream fh(fname,XS_OUT);

	if(numFrames <= 1)
		fh < (unsigned short)SizeX < (unsigned short)SizeY;
	else
		fh < (unsigned short)SizeX < (unsigned short)SizeY < (unsigned short)numFrames;

	fh.write(data,SizeX * SizeY * numFrames);
}

mchArcaneBMP::~mchArcaneBMP(void)
{
	delete[] data;
	delete[] frameData;
}

void mchArcaneBMP::Scale(int sx,int sy)
{
	int i,idx = 0;
	char* p = new char[sx * sy * numFrames];
	for(i = 0; i < numFrames; i ++){
		mch_scale_sprite((unsigned char*)p + idx,(unsigned char*)frameData[i],SizeX,SizeY,sx,sy);
		idx += sx * sy;
	}
	for(i = 0; i < numFrames; i ++)
		frameData[i] = p + i * sx * sy;

	delete[] data;
	data = p;

	SizeX = sx;
	SizeY = sy;
}

void mchArcaneScreenElement::SetState(int st)
{
	if(st){
		if(flags & (AE_HIDDEN | AE_DISABLED)){
			SetAlphaSeq(Alpha,-mchA_AlphaShowSpeed,mchA_Alpha);
			flags &= ~(AE_HIDDEN | AE_DISABLED);
		}
	}
	else {
		if(!(flags & (AE_HIDDEN | AE_DISABLED))){
			flags |= AE_DISABLED;
			SetAlphaSeq(Alpha,mchA_AlphaHideSpeed,256);
		}
		Timer = 0;
	}
}

int fxlabWorldIrradiate(void);

void mchEndGame(void)
{
	int sz;
	
	if(mchGameFinit) return;

	_RACE_LOG_STR("mchEndGame()\r\n");

	mchRealClockEnd = clocki();

	if(mchExitOnFinishFlag)
		ErrH.Exit();

	if(mch_demoD.is_enabled()){
		mchFreeResourcesFlag = 1;
		mchSetNextQuantRTO();
		mch_trackD -> GetRndTrack(mchNextWorld,mchNextTrack);
		mchCurrentWorld = mchNextWorld;
		mchCurrentTrack = mchNextTrack;
//		mchResetRacers();
		return;
	}

	mchA_SetTimer(0);

	mch_raceD -> stQuant(1);
	if(mchPBEM_Game){
		if(mchHS_LoadFlag && mchPBEM_HiddenTime){
			resource_dispatcher.break_evolve();
			mchPBEM_HiddenTime = 0;
			mchHS_LoadFlag = 0;

			if(mchPBEM_TargetTurn > mchTurn)
				mchTurn = mchPBEM_TargetTurn;
		}
		mchPBEM_GameMode = MCH_PBEM_EDIT_TURN;

		if(og_inP.express_game() && !mchPBEM_CheckFlag(PBEM_CONTROL_DATA_SENT)){
			mchPBEM_HiddenTime = -1;
			mchPBEM_SetFlag(PBEM_SHOW_GAME);
		}
		mchTurn ++;

		mch_arcScrD -> InitEndGameScreen();

		mchPBEM_SetFlag(PBEM_SET_REPLAY_MODE);
		sgPrepareControlData();
//		KeyBuf -> put('R',KBD_CUR_KEY_PRESSED);
	}
	else {
		fxlabWorldIrradiate();

		mch_arcScrD -> InitEndGameScreen();

		if(mch_raceD -> activeRacer -> stPtr -> place < 3)
			mch_raceD -> activeRacer -> StartEffect(Mechos::Effect::ExultEffect);
		else
			mch_raceD -> activeRacer -> StartEffect(Mechos::Effect::InfluenceOfArcaneEffect);

		if(!(mch_raceD -> activeRacer -> flags & MCH_AI))
			mch_raceD -> AI_Toggle(mch_raceD -> activeRacer);

		mchTurn ++;
	}

	camera_dispatcher -> runFinishScenario();

	if(mchSplitScreenGame){
		if(mch_raceD -> activeRacer2 -> stPtr -> place < 3)
			mch_raceD -> activeRacer2 -> StartEffect(Mechos::Effect::ExultEffect);
		else
			mch_raceD -> activeRacer2 -> StartEffect(Mechos::Effect::InfluenceOfArcaneEffect);

		if(!(mch_raceD -> activeRacer2 -> flags & MCH_AI))
			mch_raceD -> AI_Toggle(mch_raceD -> activeRacer2);

		camera_dispatcher2 -> runFinishScenario();
	}

	mchA_TrackPart = mch_trackD -> TrackPart();
	mchA_TrackPartPrice = mch_trackD -> TrackPrice();

	if(mchA_TrackPart)
		mchA_BonusMesh = mchGetPartByArcane(mchA_TrackPart);
	else
		mchA_BonusMesh = NULL;

	if(!mchSplitScreenGame && mchGameMode == MCH_SINGLE_GAME){
		mchA_EndGameMode = AE_RACE_NOT_LOST;
		if(!mch_raceD -> activeRacer -> stPtr -> place){
			if(mchA_TrackPart && !mch_raceD -> activeRacer -> stPtr -> partFlags[mchA_TrackPart]){
				mchBuyTrackPart(0);
			}
			else
				mchA_TrackPart = 0;

			mch_trackD -> PassTrack();

			if(!mch_trackD -> TrackBoss())
				sz = mch_trackD -> GetPrize(mch_raceD -> activeRacer -> NStar + mch_raceD -> activeRacer -> stPtr -> NStar);
			else {
				sz = 0;
				mch_raceD -> activeRacer -> NStar = 0;
			}

			mch_raceD -> activeRacer -> NStar += sz;
			mchA_EndGameMode = AE_RACE_WON;

			if(mch_trackD -> TrackBoss()){
				if(mch_trackD -> AllBossesDefeated())
					iworld_D -> PutPhrase(IW_DOGGY_LAST_BOSS_WON);
				else
					iworld_D -> PutPhrase(IW_DOGGY_BOSS_RACE_WON);
			}
			else
				iworld_D -> PutPhrase(IW_DOGGY_RACE_WON);
		}
		else {
			if(mch_raceD -> activeRacer -> stPtr -> partFlags[mchA_TrackPart])
				mchA_TrackPart = 0;

			sz = mch_raceD -> racerLst -> size();
			if(sz > 1 && mch_raceD -> activeRacer -> stPtr -> place == sz - 1){
				mchA_EndGameMode = AE_RACE_LOST;

				if(mch_trackD -> TrackBoss())
					iworld_D -> PutPhrase(IW_DOGGY_BOSS_RACE_LOST);
				else
					iworld_D -> PutPhrase(IW_DOGGY_RACE_LOST);
			}
		}

		if(mchA_EndGameMode != AE_RACE_LOST && mchA_TrackPart && mch_raceD -> activeRacer -> isAuthorized(mchA_TrackPart))
			mch_trackD -> PassTrack();
	}
	else
		mchA_EndGameMode = AE_RACE_END;

	mchSetTimeMode(MCH_TIME_GAME_OVER);

	if(!mchMusicMute) mchSetMusicMode(MCH_CD_SHOW);

	mchGameFinit = 1;
}

void mchEndGameFinit(int mode)
{
	mchArcaneScreenElement* p;
	
	if(!mchGameFinit) return;

	mchGameFinit = 0;
/*
	mchA_LapEl -> Timer = 0;
	mchA_LapEl -> flags |= AE_DISABLED | AE_HIDDEN;
*/
	mchA_CountEl -> Timer = 0;
	mchA_CountEl -> flags |= AE_DISABLED | AE_HIDDEN;
/*
	mchA_CpEl -> Timer = 0;
	mchA_CpEl -> flags |= AE_DISABLED | AE_HIDDEN;
*/

	mch_arcScrD -> showMode = AE_HIDE_SCREEN;
	mch_arcScrD -> showAction = AE_FINIT_FINISH_SCREEN;

	mch_arcScrD -> SetDestPhase(0.0f);
	mch_arcScrD -> phaseIndex = 2;

	p = mch_arcScrD -> objList -> search(AE_FINISH_NSTAR);
	if(p) p -> SetState(0);

	p = mch_arcScrD -> objList -> search(AE_PART_EL);
	if(p) p -> SetState(0);
}

void mchArcaneRoundMenu::SetTextEl(mchArcaneScreenElement* p)
{
	textEl = p;
	memset(statusStr,0,256);
	p -> SetString(MCH_AM_SEED_FNT,1,statusStr);
}

void mchA_ShowPartInfo(cMesh* m,int px,int py,int sx,int sy)
{
	HChar* p;
	mchArcaneName* nm;
	int x,y,ssx,ssy,ssx1,ssy1,ch_sx;
	unsigned ch;

	ssx = mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeX;
	ssy = mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeY;

	x = (sx - ssx) / 2 - 3;
	y = (sy - ssy) / 2;

	acsPutSpr16sz_a2(px + x,py + y,mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeX,mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeY,mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> data,2 << AE_PALETTE_SZ,0,mchA_MenuPal,32);

	nm = mchA_Names -> search(m -> arcaneData.ID);
	if(nm){
		ch = nm -> nameStr[0];
		p = acsFntTable[0] -> data[ch];
		ch_sx = p -> SizeX - (p -> LeftOffs + p -> RightOffs);

		ssx1 = (mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeX - ch_sx) / 2 + p -> LeftOffs;
		ssy1 = (mchAShop_ShapeBMP[MCH_AM_UNIQUE] -> SizeY - p -> SizeY) / 2;
		acsPutSpr16sz_a2(px + x + ssx1,py + y + ssy1,p -> SizeX,p -> SizeY,p -> HeightMap,2 << AE_PALETTE_SZ,0,mchA_MenuPal,32);
	}
}

void mchA_LoadWorldMap(int m)
{
	mchA_BuildMapImage();
	mchA_d3dLoadBmp(191,191,0,AE_D3DSPR_MAP,mchA_CurWorld);
/*
	int x,y,idx,sx,sy;
	unsigned char* buf;
	char fname[256];
	tgaFile* tgaPtr;

	sx = mchA_CurWorld -> SizeX;
	sy = mchA_CurWorld -> SizeY;

	buf = new unsigned char[sx * sy * 3];

	idx = 0;
	for(y = 0; y < sy; y ++){
		for(x = 0; x < sx; x ++){
			buf[idx * 3] = buf[idx * 3 + 1] = buf[idx * 3 + 2] = mchA_CurWorld -> data[idx] * 8;
			idx ++;
		}
	}

	tgaPtr = new tgaFile(sx,sy,24,buf,"");
	sprintf(fname,"map_%.2d_%.2d.tga",mchCurrentWorld,mchCurrentTrack);
	tgaPtr -> write(fname);
*/
}

void mchA_SetWorldMapCoords(void)
{
/*
	if(mchA_WorldMap){
		mchA_WorldMap -> PosX = gameWnd -> PosX + gameWnd -> SizeX - mchA_WorldMap -> SizeX - 8;
		mchA_WorldMap -> PosY = gameWnd -> PosY + 8;
	}
*/
}

void mchA_ShowWorldMap(mchArcaneScreenElement* m)
{
	int x,y,col;
	Vect3f v;

	int delta;
	float scale_delta;

	const float mchA_d3dMapBorder = 2.0f;

	mchArcaneBMP* p,*p0 = mchA_MapPointer[3];
	mchCheckpoint* cp,*cp0;
	mchRacer* rp,*rp0;
	if(!m) return;

	rp0 = m -> owner -> owner;
	cp0 = mch_raceD -> getCP(rp0 -> stPtr -> Checkpoint);

	if(!mchSplitScreenGame){
		delta = 0;
		scale_delta = 1.0f;
	}
	else {
		delta = 64;
		scale_delta = 2.0f;
	}

	if(mchA_CurWorld -> data)
		mchA_d3dOutSprite(m -> R.xi() + delta,m -> R.yi() + delta,2.0f / scale_delta,2.0f / scale_delta,AE_D3DSPR_MAP,mchA_ColorF[7],(256 - m -> Alpha) * 2 / 3);

	p = mchA_MapPointer[4];
	cp = mch_raceD -> cpLst -> first();
	while(cp){
		if(cp == cp0)
			col = 3;
		else 
			col = 8;

		v.x = cp -> PosX0 - cp -> PosX1;
		v.y = cp -> PosY0 - cp -> PosY1;
		v.z = 0.0f;

		x = (XCYCL((cp -> PosX0 + cp -> PosX1) >> 1) >> 4) / scale_delta;
		y = (YCYCL((cp -> PosY0 + cp -> PosY1) >> 1) >> 4) / scale_delta;
		mchA_d3dOutSprite(m -> R.xi() + x + delta,m -> R.yi() + y + delta,1.0f,1.5f,AE_D3DSPR_MAP_BAR,mchA_ColorF[col],256 - m -> Alpha,v.psi(),1);
		cp = cp -> next;
	}

	p = mchA_MapPointer[0];
	rp = mch_raceD -> racerLst -> first();
	while(rp){
		if(rp != mch_raceD -> activeRacer){
			x = (int(round(rp -> R().x)) >> 4) / scale_delta - p -> SizeX/2;
			y = (int(round(rp -> R().y)) >> 4) / scale_delta - p -> SizeY/2;
			mchA_d3dOutSprite(m -> R.xi() + x + delta,m -> R.yi() + y + delta,1.0f,1.0f,AE_D3DSPR_MAP_POINTER,mchA_ColorF[4],256 - m -> Alpha);
		}
		rp = rp -> next;
	}

	if(mchTimeMode == MCH_TIME_STOPPED){
		v = camera_dispatcher -> observationPoint();
		p = mchA_MapPointer[0];
		x = (int(round(v.x)) >> 4) / scale_delta;
		y = (int(round(v.y)) >> 4) / scale_delta;
		mchA_d3dOutSprite(m -> R.xi() + x + delta,m -> R.yi() + y + delta,2.0f/16.0f,8.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - m -> Alpha,0,1);
		mchA_d3dOutSprite(m -> R.xi() + x + delta,m -> R.yi() + y + delta,2.0f/16.0f,8.0f/16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],256 - m -> Alpha,M_PI/2.0f,1);
	}

	p = mchA_MapPointer[0];
	x = (int(round(rp0 -> R().x)) >> 4) / scale_delta;
	y = (int(round(rp0 -> R().y)) >> 4) / scale_delta;
	mchA_d3dOutSprite(m -> R.xi() + x + delta,m -> R.yi() + y + delta,1.5f,1.5f,AE_D3DSPR_MAP_POINTER,mchA_ColorF[9],256 - m -> Alpha,0,1);

	mchA_d3dRectangleSq(m -> R.xi() + delta - 2,m -> R.yi() + delta - 2,132/scale_delta,132/scale_delta,1,256 - m -> Alpha);
}

void mchArcaneStatsDispatcher::Scale(float sc)
{
	int i;

	for(i = 0; i < AE_STATS_MAX; i ++){
		data[i] -> SizeX = round(float(mchAShop_ShapeBMP[0] -> SizeX) * sc);
		data[i] -> SizeY = round(float(mchAShop_ShapeBMP[0] -> SizeY) * sc);
	}
}

mchArcaneStatsDispatcher::mchArcaneStatsDispatcher(void)
{
	int i;

	X = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","arc_stats_x"));
	Y = atoi(getIniKey("RESOURCE/ISCREEN/iscreen.ini","world_interface","arc_stats_y"));

	memset(statusFlags,0,5 * sizeof(int));
	memset(racerIDs,0,5 * sizeof(int));

	data = new mchArcaneScreenElement*[AE_STATS_MAX];
	for(i = 0; i < AE_STATS_MAX; i ++){
		data[i] = new mchArcaneScreenElement;
		data[i] -> type = AE_ARCANE_STATS;
		data[i] -> dataType = 0;
		data[i] -> SetState(0);
		data[i] -> color = 2;

		data[i] -> SizeX = mchAShop_ShapeBMP[0] -> SizeX;
		data[i] -> SizeY = mchAShop_ShapeBMP[0] -> SizeY;

		data[i] -> showDir = 0;
		data[i] -> align_x = AE_LEFT;
	}
}

mchArcaneStatsDispatcher::~mchArcaneStatsDispatcher(void)
{
	delete data;
}

void mchArcaneStatsDispatcher::Redraw(void)
{
	int i;
	for(i = 0; i < AE_STATS_MAX; i ++){
		if(!(data[i] -> flags & AE_HIDDEN))
			data[i] -> Redraw();
	}
}

void mchArcaneStatsDispatcher::InitRedraw(void)
{
	int i,x,y;

	x = X;
	y = Y;

	for(i = 0; i < AE_STATS_MAX; i ++){
		data[i] -> R.x = x;
		data[i] -> R.y = y;

		data[i] -> InitR();
		data[i] -> showIndex = 1 + i;

		if(!(data[i] -> flags & AE_HIDDEN))
			y += data[i] -> SizeY + 1;

		if(!mch_arcScrD -> showMode)
			data[i] -> phase = data[i] -> dest_phase = 1.0f;
	}
}

void mchArcaneStatsDispatcher::Add(int arcID)
{
	int objID = arcID >> 24;
	arcID &= 0xFFFFFF;

	Add(arcID,objID);
}

void mchArcaneStatsDispatcher::Add(int arcID,int objID,int dataType,int charge,int col)
{
	int idx = -1;

	mchArcaneScreenElement* p = data[objID];
/*
	for(i = 0; i < AE_STATS_MAX; i ++){
		if(!(data[i] -> flags & AE_HIDDEN) && data[i] -> ID == objID)
			return;

	}
	for(i = 0; i < AE_STATS_MAX; i ++){
		if(data[i] -> flags & AE_HIDDEN){
			idx = i;
			break;
		}
	}
	if(idx == -1){
		p = data[0];
		for(i = 1; i < AE_STATS_MAX; i ++)
			data[i - 1] = data[i];
		data[AE_STATS_MAX - 1] = p;
	}
	else
		p = data[idx];
*/
	p -> dataType = dataType;

	p -> ID = objID;
	p -> exDataParam[0] = arcID;
	p -> exDataParam[1] = objID + 1;
	p -> exDataParam[2] = charge;
	p -> color = col;
//	p -> flags &= ~(AE_BLINK | AE_ROTATE | AE_END_ROTATE);
	p -> SetState(1);
}

void mchArcaneStatsDispatcher::Remove(int objID)
{
	int i;
	for(i = 0; i < AE_STATS_MAX; i ++){
		if(data[i] -> ID == objID){
			data[i] -> SetState(0);
		}
	}
}

void mchArcaneStatsDispatcher::HandleEvent(int objID,int ev_code,void* p)
{
	if(objID < 0 || objID >= AE_STATS_MAX)
		return;

	objID = mchA_PartNum[objID];

	switch(ev_code){
		case AE_SET_BLINK_ON:
			data[objID] -> flags |= AE_BLINK;
			break;
		case AE_SET_BLINK_OFF:
			data[objID] -> flags &= ~AE_BLINK;
			break;
		case AE_SET_ROTATE_ON:
			if(!(data[objID] -> flags & AE_ROTATE)){
				data[objID] -> flags |= AE_ROTATE;
				data[objID] -> clockTimer = 0;
			}
			else
				data[objID] -> flags &= ~AE_END_ROTATE;
			break;
		case AE_SET_ROTATE_OFF:
			if(!(data[objID] -> flags & AE_END_ROTATE)){
				data[objID] -> flags |= AE_END_ROTATE;
			}
			break;
	}
}

void mchArcaneStatsDispatcher::Clear(void)
{
	int i;
	for(i = 0; i < AE_STATS_MAX; i ++){
		if(!(data[i] -> flags & AE_HIDDEN)){
			data[i] -> PutFon();
			data[i] -> flags |= AE_HIDDEN;
		}
	}
}

int mchA_ConvertCoords(int x,int y,int z,int& sx,int& sy,int viewport)
{
	Vect3f pv,pe;
	if(!viewport || !camera_dispatcher2)
		camera_dispatcher -> ConvertorWorldToViewPort(Vect3f(x,y,z), pv, pe);
	else
		camera_dispatcher2 -> ConvertorWorldToViewPort(Vect3f(x,y,z), pv, pe);

	sx=round(pe.x),	sy=round(pe.y);
	if(RenderMode == DIRECT3D_HICOLOR){
		sx = round((float)sx / mchA_d3dResX);
		sy = round((float)sy / mchA_d3dResY);
	}
	return round(pv.z);
}

void mchA_MemPutSpr(int x,int y,int sx,int sy,int bsx,void* p,void* scr)
{
	int yy;
	char* p0,*p1;
	p0 = (char*)p;
	p1 = (char*)scr + x + y * bsx;
	for(yy = 0; yy < sy; yy ++){
		memcpy(p1,p0,sx);
		p0 += sx;
		p1 += bsx;
	}
}

void mchArcaneScreenDispatcher::ToggleVisible(int v)
{
	if(v == -1)
		Visible ^= 1;
	else
		Visible = v;
}

mchA_TextInfo::mchA_TextInfo(const char* ini_file,const char* ini_section,const char* ini_key_prefix)
{
	XBuffer XBuf;

	XBuf < ini_key_prefix < "_x";
	X = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_y";
	Y = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_sx";
	SizeX = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_sy";
	SizeY = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_fnt";
	font = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_sp";
	space = atoi(getIniKey(ini_file,ini_section,XBuf.address()));

	XBuf.init();
	XBuf < ini_key_prefix < "_vsp";
	v_space = atoi(getIniKey(ini_file,ini_section,XBuf.address()));
}

int mchA_TextInfo::StrPosY(int str)
{
	return (Y + str * (v_space + acsFntTable[font] -> SizeY) + acsFntTable[font] -> SizeY/2);
}

void mchA_TextInfo::OutStr(int str_num,void* p,int col,int alpha,int center)
{
	int x,y,sx = acsStrLen(font,(unsigned char*)p,space);

	if(center == -1){
		x = CurX;
		y = CurY;
	}
	else {
		x = (center) ? (X + (SizeX - sx)/2) : X;
		y = Y + str_num * (v_space + acsFntTable[font] -> SizeY);
	}

	if(RenderMode == XGRAPH_HICOLOR)
		acsOutStr16_a2(x,y,font,col << AE_PALETTE_SZ,(unsigned char*)p,mchA_MenuPal,space,alpha);
	else
		mchA_d3dOutString(x,y,mchA_FontScaleX[font],mchA_FontScaleY[font],p,mchA_ColorF[col],256 - alpha,font,space);

	CurX = x + sx;
	CurY = y;
}

void mchA_TextInfo::GetStringPrm(int str_num,void* p,int center,int& x,int& y,int& sx,int& sy)
{
	sx = acsStrLen(font,(unsigned char*)p,space);
	sy = v_space + acsFntTable[font] -> SizeY;

	x = (center) ? (X + (SizeX - sx)/2) : X;
	y = Y + str_num * (v_space + acsFntTable[font] -> SizeY);
}

#define AE_MOUSE_SZ0		32.0f
#define AE_MOUSE_SZ1		8.0f
#define AE_MOUSE_DELTA0		8
#define AE_MOUSE_DELTA1		16
#define AE_MOUSE_DELTA2		20

void mchA_DrawMouse(int mode)
{
	int x,y,z,sx,sy,sz,delta,hand_mode = 0;
	float scale;

	mchControlPoint* p;

	if(!mode && !mchMouse3D_Visible()) return;

	if(!mode){
		if(mch_raceD -> activeRacer -> mouseSeed){
			p = mch_raceD -> activeRacer -> mouseSeed;

			mchA_ConvertCoords(p -> PosX,p -> PosY,p -> PosZ,mchA_MouseSX,mchA_MouseSY);
			mchA_MouseScale = camera_dispatcher -> getScale(Vect3f(p -> PosX,p -> PosY,p -> PosZ));
			mchA_MouseXY_Flag = 1;
		}

		if(mchA_MouseXY_Flag){
			sx = mchA_MouseSX;
			sy = mchA_MouseSY;

			z = 128;
		}
		else {
			mchGetMouseCoords3D(x,y,z);
			mchA_ConvertCoords(x,y,z,sx,sy);

			if(mchRightMousePress) hand_mode = 1;
		}
	}
	else {
		sx = round((float)(XGR_MouseObj.PosX + XGR_MouseObj.SizeX / 2) / mchA_d3dResX);
		sy = round((float)(XGR_MouseObj.PosY + XGR_MouseObj.SizeY / 2) / mchA_d3dResY);
		z = 128;
	}

	sz = AE_MOUSE_SZ1;
	scale = 0.7f + 0.3f * z / 256.0f;
	delta = AE_MOUSE_DELTA2;

	if(mode || !mchA_MouseXY_Flag){
		if(!hand_mode){
			mchA_d3dOutSprite(sx,sy,1.0f,1.0f,AE_D3DSPR_MOUSE_CROSS,mchA_ColorF[7],200,0.0f,1);
			mchA_d3dOutSprite(sx,sy,scale,scale,AE_D3DSPR_CIRCLE,mchA_ColorF[7],200,0.0f,1);
		}
		else 
			mchA_d3dOutSprite(sx,sy,1.0f,1.0f,AE_D3DSPR_MOUSE_HAND,mchA_ColorF[7],200,0.0f,1);
//			mchA_d3dOutSprite(sx,sy,scale,scale,AE_D3DSPR_CIRCLE,mchA_ColorF[7],200,0.0f,1);
	}
	else
		mchA_DrawMouseRect(sx,sy,sz,round(float(delta) * mchA_MouseScale));

	mchA_MouseXY_Flag = 0;
}

void mchA_DrawMouseRect(int sx,int sy,int sz,int delta)
{
	int i,x0[4],y0[4],x1[4],y1[4];

	x0[0] = sx - delta + sz/2;
	y0[0] = sy - delta;
	x1[0] = sx - delta;
	y1[0] = sy - delta + sz/2;

	x0[1] = sx + delta - sz/2;
	y0[1] = sy - delta;
	x1[1] = sx + delta;
	y1[1] = sy - delta + sz/2;

	x0[2] = sx + delta - sz/2;
	y0[2] = sy + delta;
	x1[2] = sx + delta;
	y1[2] = sy + delta - sz/2;

	x0[3] = sx - delta + sz/2;
	y0[3] = sy + delta;
	x1[3] = sx - delta;
	y1[3] = sy + delta - sz/2;

	if(RenderMode == DIRECT3D_HICOLOR){
		for(i = 0; i < 4; i ++){
			mchA_d3dOutSprite(x0[i],y0[i],AE_MOUSE_SZ1 / 16.0f,2.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],200,0.0f,1);
			mchA_d3dOutSprite(x1[i],y1[i],2.0f / 16.0f,AE_MOUSE_SZ1 / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[7],200,0.0f,1);
		}
	}
	else {
		gameWnd -> set_clipping();

		for(i = 0; i < 4; i ++){
			XGR_LineTo16(x0[i] - AE_MOUSE_SZ1/2,y0[i],AE_MOUSE_SZ1,2,mchA_MenuPal[(2 << 5) - 1]);
			XGR_LineTo16(x1[i],y1[i] - AE_MOUSE_SZ1/2,AE_MOUSE_SZ1,3,mchA_MenuPal[(2 << 5) - 1]);
		}

		gameWnd -> remove_clipping();
	}
}

void mchA_DrawSleep(void)
{
	int x,y,z,sx,sy,sz;
	float angle,scale;
	if(mchGameMode != MCH_ENTIRE_CONTROL_HS || (mchTimeMode != MCH_TIME_RUNNING && !mchPBEM_Pause)) return;

	mchRacer* p = mch_raceD -> racerLst -> first();
	while(p){
		if(p -> flags & MCH_SLEEPING){
			x = p -> R().xi();
			y = p -> R().yi();
			z = p -> R().zi() + p -> radius() * 2 + 5;

			if(!(camera_dispatcher -> cameraMode().mapMode && (mchCameraAX || mchCameraAZ)))
				y -= p -> radius();
			else {
				angle = M_PI * (float)mchCameraAZ / 180.0f;
				x -= round(p -> radius() * sin(angle));
				y -= round(p -> radius() * cos(angle));
			}	

			sz = mchA_ConvertCoords(x,y,z,sx,sy);
			scale = camera_dispatcher -> getScale(p -> R());

			mchA_d3dOutSpriteZ(sx,sy,sz,1.0f * scale,1.0f * scale,AE_D3DSPR_ZZZ,mchA_ColorF[7],200,0,1);
		}
		p = p -> next;
	}
}

void mchA_DrawSeeds(void)
{
	int x,y,z,cnt,al,sz,delta;
	Vect3f dr,r,r0;

	float scale;

	sz = AE_MOUSE_SZ1;
	delta = AE_MOUSE_DELTA1;

	mchControlPoint* p;
//	if(RenderMode != DIRECT3D_HICOLOR || !mch_arcScrD -> Visible || !mchTurnBasedGame || mchTimeMode == MCH_TIME_GAME_OVER || mch_raceD -> activeRacer -> flags & MCH_AI) return;
	if(RenderMode != DIRECT3D_HICOLOR || !mch_arcScrD -> Visible || !mchTurnBasedGame || mchTimeMode == MCH_TIME_GAME_OVER) return;

	r0 = camera_dispatcher -> observationPoint();
	r0.z = 0.0f;

	p = mch_raceD -> activeRacer -> seedLst.first();

	al = 200;
	cnt = 0;

	while(p){
		if(p != mch_raceD -> activeRacer -> curSeed || mchTimeMode == MCH_TIME_RUNNING){
			r = Vect3f(*p);
			r.z = 0.0f;

			dr = getDist(r,r0);

			if(mchTimeMode == MCH_TIME_RUNNING || dr.norm() < 900.0f){
				z = mchA_ConvertCoords(p -> PosX,p -> PosY,p -> PosZ,x,y);

				scale = camera_dispatcher -> getScale(*p);

				if(p -> arcaneData.ID != -1)
					mchA_SprD -> DrawArcaneSymbol(x,y,p -> arcaneData.ID,0.0f,0.0f,2,al,0.8f * scale,1);

				mchA_d3dOutSprite(x,y,1.25f * scale,1.25f * scale,AE_D3DSPR_CIRCLE,mchA_ColorF[7],al,0.0f,1);

				if(mchTimeMode == MCH_TIME_RUNNING){
					cnt ++;
					al -= 70;
					if(cnt >= 3) break;
				}
			}
		}

		p = p -> next;
	}
}

void mchA_DrawArrow(int x_,int y_,int sx,mchArcaneRacerSet* p)
{
	Vect3f r;
	int x,y,z,x0,y0,x1,y1,dx,dy;
	float angle,angle_delta,sc;

	mchRacer* racer;

	mchTrackElement* el = NULL;

	if(mchSplitScreenGame && acsActiveFlag) return;

	if(mchTrackDemoMode || mchTimeMode != MCH_TIME_RUNNING || !mch_arcScrD -> Visible || mchTurnBasedGame) return;

	if(!p) 
		return;

	sc = float(sx) / 100.0f;

	racer = p -> owner;

	if(racer -> last_trackP)
		el = mch_raceD -> AI_GetTrackEl(racer -> last_trackP,-1,5,1,0);

	if(el){
		x = XCYCL(el -> Cx);
		y = YCYCL(el -> Cy);
		z = el -> Cz;
		
		mchA_ConvertCoords(x,y,z,x1,y1,p -> wndID);

		dx = getDistX(racer -> R().xi(),x);
		dy = getDistY(racer -> R().yi(),y);

		x += dx;
		y += dy;
		z = racer -> R().z;
		mchA_ConvertCoords(x,y,z,x0,y0,p -> wndID);

		r.y = x1 - x0;
		r.x = -y1 + y0;
		r.z = 0.0f;

		angle = r.psi();
	}
	else
		angle = 0.0f;

	angle_delta = angle - p -> arrowAngle;
	if(fabs(angle_delta) > M_PI)
		angle_delta += (angle_delta < 0) ? (M_PI * 2.0f) : -(M_PI * 2.0f);

	if(p -> arrowAngle < M_PI * 2.0f - mchA_ArrowAngleDelta && fabs(angle_delta) > mchA_ArrowAngleDelta)
		p -> arrowAngle += (angle_delta > 0) ? mchA_ArrowAngleDelta : -mchA_ArrowAngleDelta;
	else
		p -> arrowAngle = angle;

	if(mchA_ArrowMesh){
		mchA_ArrowMesh -> SetColor(1,1,1,1.0f);
		mchA_ArrowMesh -> SetScale(Vect3f(mchA_d3dResX * sc));

		Vect3f v1(float(x_) * mchA_d3dResX,float(y_) * mchA_d3dResY,0.0f);
		Vect3f v2(mchCameraAX + 40.0f,0,p -> arrowAngle * 180.0f / M_PI);
		gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)mchA_ArrowMesh,
			&v1,
			&v2);
		mchA_DrawM3D(mchA_ArrowMesh);
		mchA_ArrowMesh -> SetScale(Vect3f(1,1,1));
		mchA_ArrowMesh -> SetColor(1,1,1,1.0f);
	}
}

void mchA_BuyTrackParts(void)
{
	int i,j;
	mchRacerStats* rp = mch_racerLst.search(0);
	if(!rp) return;

	for(i = 0; i < 20; i ++){
		for(j = 0; j < 10; j ++){
			rp -> partFlags[mch_trackD -> TrackPart(i,j)] = 1;
		}
	}
}

void mchA_BuyHiddenParts(void)
{
	int i;
	mchRacerStats* rp = mch_racerLst.search(0);
	if(!rp) return;

	for(i = 0; i < ARCANE_MAX; i ++)
		rp -> partFlags[i] = 1;
}

void mchA_BuyTracks(void)
{
	int i,j;
	for(i = 0; i < 20; i ++){
		for(j = 0; j < 10; j ++){
			mch_trackD -> OpenTrack(i,j);
		}
	}
#ifndef _FINAL_VERSION_	
	mch_trackD -> SetFlag(MCH_TRACK_DEBUG_FLAG);
#endif
	if(iWorldMode)
		iworld_D -> InitTeleports();
}

void mchA_CHQuant(int k)
{
	int i,ch,code = 0;
	mchRacerStats* rp;
	mchRacer* p;

	if(k & KBD_UNPRESS_FLAG) return;

	if(k > 0 && k < 256){
		ch = acsGetKeyName(k,0,0);
		code = mchA_CHTree -> quant(ch);
	}

	switch(code){
		case 2: // Finish race...
			if(!acsActiveFlag && !iWorldMode){
//				mch_raceD -> activeRacer -> stPtr -> Lap = mchNumLaps + 1;
//				mch_raceD -> activeRacer -> stPtr -> Checkpoint = 1;

				if(!(mch_raceD -> activeRacer -> flags & MCH_AI))
					mch_raceD -> AI_Toggle();

				mch_raceD -> activeRacer -> flags |= MCH_FINISHED;
				mchEndGame();
			}
			mchA_CHTree -> reset();
			break;
		case 3: // Add stars...
			if(!iWorldMode && !acsActiveFlag){
				mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
				mch_raceD -> activeRacer -> NStar += 100;
				if(mchSplitScreenGame)
					mch_raceD -> activeRacer2 -> NStar += 100;
			}
			else {
				rp = mch_racerLst.search(0);
				if(rp) 
					rp -> NStar += 100;

				if(mchSplitScreenGame){
					rp = mch_racerLst.search(1);
					if(rp) 
						rp -> NStar += 100;
				}
			}
			mchA_CHTree -> reset();
			break;
		case 4: // Death...
			if(!acsActiveFlag){
				mch_raceD -> activeRacer -> damage(1000);
				if(mchSplitScreenGame)
					mch_raceD -> activeRacer2 -> damage(1000);
			}
			mchA_CHTree -> reset();
			break;
		case 5: // Damage...
			if(!acsActiveFlag){
				mch_raceD -> activeRacer -> damage(20);
				if(mchSplitScreenGame)
					mch_raceD -> activeRacer2 -> damage(20);
			}
			mchA_CHTree -> reset();
			break;
		case 6: // Charge...
			if(!acsActiveFlag){
				if(mch_raceD -> activeRacer -> Type == MCH_RACER_TYPE_MECHOS)
					(dynamic_cast<mchMechosRacer*>(mch_raceD -> activeRacer)) -> charge(1000);
				if(mchSplitScreenGame && mch_raceD -> activeRacer2 -> Type == MCH_RACER_TYPE_MECHOS)
					(dynamic_cast<mchMechosRacer*>(mch_raceD -> activeRacer2)) -> charge(1000);
			}
			
			mchA_CHTree -> reset();
			break;
		case 7: // Open track parts
			mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
			mchA_BuyTrackParts();
			mchA_CHTree -> reset();
			break;
		case 8: // Open all parts
			mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
			mchA_BuyHiddenParts();
			mchA_CHTree -> reset();
			break;
		case 9: // Open all tracks
			mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
			mchA_BuyTracks();
			mchA_CHTree -> reset();
			break;
		case 10: // mana
			mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
			if(!acsActiveFlag){
				for(i = 0; i < 7; i ++)
					fxlabBonusEvent(ARCANE062,mch_raceD -> activeRacer);

				if(mchSplitScreenGame){
					for(i = 0; i < 7; i ++)
						fxlabBonusEvent(ARCANE062,mch_raceD -> activeRacer2);
				}
			}
			mchA_CHTree -> reset();
			break;
		case 11: // lose race
			if(!acsActiveFlag && !iWorldMode){
				p = mch_raceD -> racerLst -> first();
				while(p){
					if(p -> flags & MCH_ACTIVE_RACER)
						p -> stPtr -> time = 1000 * 3600;
					p -> stPtr -> placeTimer = 0;
					p -> flags |= MCH_FINISHED;
					p = p -> next;
				}
				mchTimeMode = MCH_TIME_GAME_OVER;
				mchEndGame();
			}
			mchA_CHTree -> reset();
			break;
		case 12: // track recorder enable	
			if(mch_trkRec){
				mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
				trkEnableFlag = 1;

				mchFreeResourcesFlag = 1;
				mchSetNextQuantRTO();
				mchNextWorld = mchCurrentWorld;
				mchNextTrack = mchCurrentTrack;
				mchResetRacers();
			}				
			mchA_CHTree -> reset();
			break;
		case 13: // restart race	
			if(!acsActiveFlag && !iWorldMode){
				mchFreeResourcesFlag = 1;
				mchSetNextQuantRTO();
				mchNextWorld = mchCurrentWorld;
				mchNextTrack = mchCurrentTrack;
				mchResetRacers();
			}
			mchA_CHTree -> reset();
			break;
		case 14: // max speed
			if(!acsActiveFlag && !iWorldMode){
				mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
				if(mch_raceD -> activeRacer -> Type == MCH_RACER_TYPE_MECHOS){
					for(i = 0; i < 20; i ++)
						((mchMechosRacer*)mch_raceD -> activeRacer) -> speed_increment();
				}
			}
			mchA_CHTree -> reset();
			break;
		case 15: // all figures
			if(mch_trackD){
				mchStartSoundEFF(EFF_MAIN_MENU_BUTTON);
				mch_trackD -> EnableFigures(10);

				if(iWorldMode)
					iworld_D -> InitFigures();
			}
			mchA_CHTree -> reset();
			break;
		case 16: // end race
			if(!acsActiveFlag && !iWorldMode){
				p = mch_raceD -> racerLst -> first();
				while(p){
					p -> stPtr -> placeTimer = 0;
					p -> flags |= MCH_FINISHED;
					p = p -> next;
				}
				mchTimeMode = MCH_TIME_GAME_OVER;
				mchEndGame();
			}
			mchA_CHTree -> reset();
			break;
		case 17: // boss movie 0
			if(iWorldMode)
				iworld_D -> SetMovie(1);
			mchA_CHTree -> reset();
			break;
		case 18: // boss movie 1
			if(iWorldMode)
				iworld_D -> SetMovie(0);
			mchA_CHTree -> reset();
			break;
		case 19: // fix first checkpoint
			if(!acsActiveFlag && !iWorldMode){
				mchFixStartCP = 1;
				mchFreeResourcesFlag = 1;
				mchSetNextQuantRTO();
				mchNextWorld = mchCurrentWorld;
				mchNextTrack = mchCurrentTrack;
				mchResetRacers();
			}
			mchA_CHTree -> reset();
			break;
		case 20: // end game movie
			if(iWorldMode)
				iworld_D -> SetEndMovie();

			mchA_CHTree -> reset();
			break;
	}
}

void mchBuyTrackPart(int mode)
{
	int arc_id,arc_price;
	mchArcaneScreenElement* p;

	mchRacer* r = mch_raceD -> racerLst -> first();
	while(r){
		if(!r -> stPtr -> ID) break;
		r = r -> next;
	}

	if(!r) return;

	arc_id = mch_trackD -> TrackPart();
	arc_price = mch_trackD -> TrackPrice();

	if(!r -> stPtr -> partFlags[arc_id])
		iworld_D -> PutPhrase(IW_DOGGY_NEW_PART);

	r -> stPtr -> partFlags[arc_id] = 1;
	r -> stPtr -> ReplacePart(arc_id);
	
	mch_trackD -> PassTrack();

	if(mode){
		r -> NStar -= arc_price;
		if(r -> NStar < 0){
			r -> stPtr -> NStar += r -> NStar;
			r -> NStar = 0;
		}
		mchA_NStarNew -= arc_price;

		p = mch_arcScrD -> objList -> search(AE_BUY_STR);
		if(p) p -> dest_phase = 0.0f;
		p = mch_arcScrD -> objList -> search(AE_NO_THANKS_STR);
		if(p) p -> dest_phase = 0.0f;
		p = mch_arcScrD -> objList -> search(AE_BUY_IT_STR);
		if(p) p -> dest_phase = 0.0f;
		p = mch_arcScrD -> objList -> search(AE_PRICE_STR);
		if(p) p -> dest_phase = 0.0f;

		p = mch_arcScrD -> objList -> search(AE_CONTINUE2_STR);
		if(p){
			p -> phase = 0.0f;
			p -> dest_phase = 1.0f;
			p -> SetState(1);
		}
		
		mch_arcScrD -> SetActiveEl(AE_CONTINUE2_STR);

		mchA_FinishNStarEl -> dest_phase = 1.0f;
		mchA_FinishNStarEl -> destScale = 1.0f;

		mchStartSoundEFF(EFF_STAR_MOVE);
	}
}

void mchA_ShowLap(int x,int y,int al,mchArcaneRacerSet* owner)
{
	int sx,sy,lap,lp;

	if(!owner) return;

	lap = owner -> owner -> stPtr -> Lap + 1;

	float sc = (256.0f - (float)al) / (256.0f - (float)mchA_Alpha);
	al /= 2;

	if(lap > 20)
		lp = lap % 10;
	else
		lp = lap;

	mchA_XBuf.init();

	if(lap > mchNumLaps) return;
	
	if(lap < mchNumLaps){
		mchA_XBuf < iGetText(iTXT_LAP) <= lap;
	}
	else
		mchA_XBuf < iGetText(iTXT_FINAL_LAP);

	sx = round((float)acsStrLen(MCH_AM_LAP_FNT,(unsigned char*)mchA_XBuf.address(),1) * sc);
	sy = round((float)acsStrHeight(MCH_AM_LAP_FNT,(unsigned char*)mchA_XBuf.address()) * sc);

	x -= sx/2;
	y -= sy/2;
	mchA_d3dOutString(x,y,mchA_FontScaleX[MCH_AM_LAP_FNT],mchA_FontScaleY[MCH_AM_LAP_FNT],mchA_XBuf.address(),mchA_ColorF[2],256 - al,MCH_AM_LAP_FNT,2,sc);
}

void mchA_ShowStartCountEnable(void)
{
	mchA_CountEl -> SetState(1);
}

int mchA_StartCountEnabled(void)
{
	return mchA_CountEl -> Active();
}

void mchA_ShowStartCountDisable(void)
{
	mchA_SetTimer(0);
	mchA_CountEl -> SetState(0);
}

void mchA_ShowStartCount(int al)
{
	float sc,scale;
	int tm,cur_timer;

	static int last_tm = -1;

	cur_timer = non_stop_global_time() - mchA_TimerStart;

	if(cur_timer > mchA_TimerMax) return;

	tm = (mchA_TimerMax - cur_timer) / 1000;

	sc = (256.0f - (float)al) / (256.0f - (float)mchA_Alpha);
//	sc = (float)al / (256.0f - (float)mchA_Alpha);

	if(!camera_dispatcher -> isInterpolationCompleted())
		return;

	if(tm != last_tm){
		if(tm > 0)
			mchStartSoundEFF(EFF_RACE_COUNTDOWN);
		else
			mchStartSoundEFF(EFF_RACE_GO);
	}
	last_tm = tm;

//	scale = (float)((mchA_TimerMax - cur_timer) % 1000) / (float)1000;
	scale = (float)(cur_timer % 1000) / 1000.0f;

	if(tm > 0){
		al = 255 - round(250.0f * scale);
		scale *= sc * 16.0f;
		mchA_SprD -> DrawSprite(320,240,scale,scale,200 + tm,mchA_ColorF[2],al,0.0f,1);
	}
	else {
#ifdef _LOCAL_VERSION_
		al = 255 - round(250.0f * scale);
		if(al < 0) al = 0;

		scale *= sc * 10.0f;
		mchA_SprD -> DrawSprite(320,240,scale,scale,200,mchA_ColorF[2],al,0.0f,1);

		mchA_SprD -> DrawSprite(320 - 64 * scale,240,scale,scale,211,mchA_ColorF[2],al,0.0f,1);
		mchA_SprD -> DrawSprite(320 + 64 * scale,240,scale,scale,212,mchA_ColorF[2],al,0.0f,1);
#else
		al = 255 - round(320.0f * scale);
		if(al < 0) al = 0;

		scale *= sc * 20.0f;
		mchA_SprD -> DrawSprite(320,240,scale,scale,200,mchA_ColorF[2],al,0.0f,1);
#endif
	}
}

void mchA_ShowCount(int al,int time)
{
	float sc,scale;
	int tm;

	static int last_tm = -1;

	if(time > 5000) return;

	tm = (6000 - time) / 1000;

	sc = (256.0f - (float)al) / (256.0f - (float)mchA_Alpha);
	scale = (float)(time % 1000) / 1000.0f;

	if(tm != last_tm){
		if(tm > 0)
			mchStartSoundEFF(EFF_DROP_COUNTDOWN);
	}
	last_tm = tm;

	if(tm > 0){
		al = 255 - round(250.0f * scale);
		scale *= sc * 16.0f;
		mchA_SprD -> DrawSprite(320,240,scale,scale,200 + tm,mchA_ColorF[2],al,0.0f,1);
	}
}

void mchA_ShowLapEnable(int player)
{
	mchArcaneScreenElement* el = (player) ? mch_arcScrD -> playerSet2 -> lapcntEl : mch_arcScrD -> playerSet1 -> lapcntEl;

	el -> SetState(1);
	el -> Timer = 25;

	mchStartSoundEFF(EFF_RACE_LAP);
}

int mchA_GetEssenceID(void)
{
	if(mch_raceD && mch_raceD -> activeRacer)
		return mch_raceD -> activeRacer -> stPtr -> essenceID;

	return 0;
}

void mchArcaneBMP::CalcBound(void)
{
	int i,x,y,sz,dx0,dx1,dy0,dy1;
	int dx00,dy00,dx01,dy01;
	int idx = 0,idx1 = 0;

	dx00 = dx01 = SizeX/2;
	dy00 = dy01 = SizeY/2 * SizeX;

	for(i = 0; i < numFrames; i ++){
		dx0 = dx1 = SizeX/2;
		dy0 = dy1 = SizeY/2 * SizeX;

		for(y = 0; y < SizeY; y ++){
			x = 0;
			while(x < SizeX && !frameData[i][idx + x]) x ++;
			if(x < dx0) dx0 = x;
			idx += SizeX - 1;

			x = 0;
			while(x < SizeX && !frameData[i][idx - x]) x ++;
			if(x < dx1) dx1 = x;
			idx ++;
		}

		idx = 0;
		sz = SizeX * (SizeY - 1);
		for(x = 0; x < SizeX; x ++){
			y = 0;
			while(y <= sz && !frameData[i][idx + y]) y += SizeX;
			if(y < dy0) dy0 = y;

			y = 0;
			while(y <= sz && !frameData[i][idx - y + sz]) y += SizeX;
			if(y < dy1) dy1 = y;
			idx ++;
		}

		if(dx00 > dx0) dx00 = dx0;
		if(dx01 > dx1) dx01 = dx1;

		if(dy00 > dy0) dy00 = dy0;
		if(dy01 > dy1) dy01 = dy1;
	}

	bound[0] = dx00;
	bound[1] = dx01;
	bound[2] = dy00 / SizeX;
	bound[3] = dy01 / SizeX;
}

void mchA_DrawRacersPlace(void)
{
	int i;
	mchRacer* p;
	for(i = 0; i < 4; i ++){
		mchA_FigureEl[i] -> SetState(0);
		p = mch_raceD -> racerLst -> first();
		while(p){
			if(p -> stPtr -> place == i){
				mchA_FigureEl[i] -> exDataParam[0] = p -> stPtr -> essenceID;
				mchA_FigureEl[i] -> exDataParam[1] = p -> flags & MCH_ACTIVE_RACER;
				mchA_FigureEl[i] -> SetState(1);
			}
			p = p -> next;
		}
	}
}

void mchA_DrawRacerFinishPlace(int x,int y,int id,int active,float ph,const char* name,int chr,int star,int time)
{
	int col = (active) ? 2 : 1,hrs,min,sec,num;
	static int alpha = 0;
	static int d_alpha = 10;
	const int alpha_max = 150;

	static int last_chr = -1;

	if(active && !acsActiveFlag){
		if((alpha += d_alpha) >= alpha_max){
			d_alpha = -abs(d_alpha);
			alpha = alpha_max;
		}
		if((alpha += d_alpha) < 0){
			d_alpha = abs(d_alpha);
			alpha = 0;
		}
	}

	if(active)
		mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,1,mchA_ColorF[7],250 - alpha,0.0f,0);
	else
		mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,1,mchA_ColorF[0],200,0.0f,0);
	
	mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,100 + id,mchA_ColorF[7],200,0.0f,0);

	if(ph == 1.0f && name){
		mchA_XBuf.init();
		mchA_XBuf < name;
		
		if(chr < strlen(name)){
			mchA_XBuf.buf[chr] = 0;

			if(last_chr != chr){
				mchStartSoundEFF(EFF_FINISH_KEY);
				last_chr = chr;
			}
		}

		mchA_d3dOutString(x + 64,y + 20,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[col],200,1,0);

		if(mchSplitScreenGame && chr > strlen(name)){
			mchA_XBuf.init();
			mchA_XBuf < "x" <= star;
			mchA_d3dOutString(x + 210,y + 20,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[3],200,1,0);
			mchA_DrawStar(x + 210 - 24,y + 35);
		}

		if(mchRealClockShow && mchGameMode == MCH_SINGLE_GAME && chr >= strlen(name)){
			mchA_XBuf.init();

			if(time != -1){
				hrs = time / 3600;
				min = (time - hrs * 3600) / 60;
				sec = time % 60;

				if(hrs < 10) mchA_XBuf < "0" <= hrs < ":";
				else mchA_XBuf <= hrs < ":";

				if(min < 10) mchA_XBuf < "0" <= min < ":";
				else mchA_XBuf <= min < ":";

				if(sec < 10) mchA_XBuf < "0" <= sec;
				else mchA_XBuf <= sec;

				num = chr - strlen(name);

				if(num < strlen(mchA_XBuf.address())){
					mchA_XBuf.set(num,XB_BEG);
					mchA_XBuf < (char)0;
				}

				mchA_d3dOutString(x + 160,y + 25,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_XBuf.address(),mchA_ColorF[2],200,2,0);
			}
		}

		if((mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS) && chr >= strlen(name)){
			mchA_XBuf.init();

			if(time != -1){
				hrs = time / 3600;
				min = (time - hrs * 3600) / 60;
				sec = time % 60;

				if(hrs < 10) mchA_XBuf < "0" <= hrs < ":";
				else mchA_XBuf <= hrs < ":";

				if(min < 10) mchA_XBuf < "0" <= min < ":";
				else mchA_XBuf <= min < ":";

				if(sec < 10) mchA_XBuf < "0" <= sec;
				else mchA_XBuf <= sec;

				mchA_XBuf < iGetText(iTXT_TURNS) <= star;
			}
			else
				mchA_XBuf < "--:--:--";

			num = chr - strlen(name);

			if(num < strlen(mchA_XBuf.address())){
				mchA_XBuf.set(num,XB_BEG);
				mchA_XBuf < (char)0;
			}

			mchA_d3dOutString(x + 185,y + 20,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[2],200,1,0);
		}
	}
}

void mchA_DrawRacerPlace(int x,int y,int id,int active)
{
	static int alpha = 0;
	static int d_alpha = 10;
	const int alpha_max = 150;

	if(active && !acsActiveFlag){
		if((alpha += d_alpha) >= alpha_max){
			d_alpha = -abs(d_alpha);
			alpha = alpha_max;
		}
		if((alpha += d_alpha) < 0){
			d_alpha = abs(d_alpha);
			alpha = 0;
		}
	}

	if(active)
		mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,1,mchA_ColorF[7],250 - alpha,0.0f,0);
	else
		mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,1,mchA_ColorF[0],200,0.0f,0);
	
	mchA_SprD -> DrawSprite(x,y,1.0f,1.0f,100 + id,mchA_ColorF[7],200,0.0f,0);
}

void mchA_DrawRacersEnergy(void)
{
	int x,y,z,sx,sy,place;
	Vect3f r,r0,dr;

	mchRacer* p;

	static int place_colors[5] = { 13, 14, 14, 14, 15 };

	if(!mch_arcScrD -> Visible || RenderMode == XGRAPH_HICOLOR) return;
	if(!mchTurnBasedGame || mchTimeMode != MCH_TIME_STOPPED) return;
	if(mch_raceD -> activeRacer -> stPtr -> time < 1000) return;

	mchGetMouseCoords3D(x,y,z);
	r = Vect3f(x,y,0);

	p = mch_raceD -> racerLst -> first();
	while(p){
		r0 = p -> R();
		r0.z = 0;
		dr = getDist(r,r0);
		if(dr.norm() < p -> radius()){
			place = p -> stPtr -> place + 1; 
			mchA_ConvertCoords(x,y,z,sx,sy);
			mchA_ShowEnergy(sx + 10,sy - 10,0.4f,p -> EnergyValue(),p -> EnergyValueMax(),200);

			mchA_d3dOutString(sx + 10,sy - 30,mchA_FontScaleX[2],mchA_FontScaleY[2],p -> name,mchA_ColorF[place_colors[place - 1]],200,2,0);

			mchA_XBuf.init();
			mchA_XBuf <= place < mchA_GetNumSuffix(place);
			mchA_d3dOutString(sx + 15,sy - 5,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_XBuf.address(),mchA_ColorF[place_colors[place - 1]],200,2,0);

			return;
		}
		p = p -> next;
	}
}

void mchA_DrawRacersInfo(void)
{
	int i,j,sx,sy,fl = 1,cnt;
	mchRacer* p;

	if(!mch_arcScrD -> Visible) return;
	if(mchTurnBasedGame && (!mchKeyPressed(MCH_KEY_ARCANE_MENU) && mchTimeMode != MCH_TIME_GAME_OVER)) return;
	if(!mchTurnBasedGame && mchTimeMode != MCH_TIME_GAME_OVER) return;

	static int x[HS_PLAYERS_MAX];
	static int y[HS_PLAYERS_MAX];
	static int z[HS_PLAYERS_MAX];

	static int places[HS_PLAYERS_MAX];
	static int ssx[HS_PLAYERS_MAX];
	static char strings[HS_PLAYERS_MAX][256];

	i = 0;

	p = mch_raceD -> racerLst -> first();
	while(p){
		z[i] = mchA_ConvertCoords(p -> R().xi(),p -> R().yi(),p -> R().zi() + p -> radius(),sx,sy);
		x[i] = sx;
		y[i] = sy;
		places[i] = p -> stPtr -> place;
		i ++;

		p = p -> next;
	}

	i = 0;
	p = mch_raceD -> racerLst -> first();
	while(p){
		mchA_XBuf.init();

		if(mchTimeMode != MCH_TIME_GAME_OVER || p -> flags & MCH_FINISHED){
			mchA_XBuf < p -> stPtr -> name < " ";

			if(!(p -> flags & MCH_FINISHED)){
				mchA_XBuf < "Place: " <= (p -> stPtr -> place + 1) < "/" <= mch_raceD -> racerLst -> size() < " ";
				mchA_XBuf < "Lap: " <= (p -> stPtr -> Lap + 1) < "/" <= mchNumLaps;
			}
			else
				mchA_XBuf <= (p -> stPtr -> place + 1);
		}

		strcpy(strings[i],mchA_XBuf.address());
		ssx[i] = acsStrLen(2,(unsigned char*)strings[i],0);

		i ++;

		p = p -> next;
	}

	if(mchTimeMode == MCH_TIME_GAME_OVER)
		fl = 0;
	else
		fl = 1;

	cnt = 0;
	while(fl){
		fl = 0;
		if(++cnt > 1000) break;

		for(i = 0; i < mch_raceD -> racerLst -> size(); i ++){
			for(j = 0; j < mch_raceD -> racerLst -> size(); j ++){
				if(i != j && abs(y[i] - y[j]) < 10){
					fl = 1;
					if(y[i] < y[j]){
						y[i] --;
						y[j] ++;
					}
					else {
						y[i] ++;
						y[j] --;
					}
				}
			}
		}
	}

	for(i = 0; i < mch_raceD -> racerLst -> size(); i ++){
		if(RenderMode == DIRECT3D_HICOLOR)
//			mchA_d3dOutStringZ(x[i],y[i],z[i],1.0f,1.0f,strings[i],mchA_ColorF[7],200,2,0,0.7);
			mchA_d3dOutString(x[i],y[i],1.0f,1.0f,strings[i],mchA_ColorF[7],200,2,0,0.7);
	}
}

void mchA_ShowRacerStatus(void)
{
	int cp_flag = 0,dist;

	mchRacer* p;
	mchCheckpoint* cp,*cp1;

	int alpha = mchA_Alpha/2;

	if(mchTimeMode != MCH_TIME_RUNNING) return;

	p = mch_raceD -> activeRacer;
	cp = mch_raceD -> getCP(p -> stPtr -> Checkpoint);

	if(p -> last_trackP0){
		cp_flag = 1;
		cp1 = p -> last_trackP0 -> owner -> owner;
		dist = p -> last_trackP0 -> index - p -> last_trackP0 -> owner -> pointLst -> first() -> index;
		if(cp -> ID == mch_raceD -> cpLst -> size() - 1){
			if(!cp1 -> ID && dist >= 3)
				cp_flag = 0;
		}
		else {
			if(cp1 -> ID == cp -> ID + 1 && dist >= 3)
				cp_flag = 0;
		}

		if(!cp_flag && (mchA_DropCp != cp -> ID || mchA_DropCpLap != p -> stPtr -> Lap)){
			if(mchA_DropCnt){
				if(p -> last_trackP0 -> index - mchA_DropIdx >= 3){
					mch_raceD -> activeRacer -> StartEffect(Mechos::Effect::CheckPointOmissionEffect);
					mchA_DropIdx = mchA_DropCnt = 0;

					mchA_CpEl -> SetState(1);
					mchA_CpEl -> Timer = 100;

					mchA_DropCp = cp -> ID;
					mchA_DropCpLap = p -> stPtr -> Lap;
				}
			}
			else {
				mchA_DropIdx = p -> last_trackP0 -> index;
				mchA_DropCnt = 1;
			}
		}
	}
}

void mchA_ShowMechoses(void)
{
	XBuffer XBuf;

	mchRacerStats* p = mch_racerLst.first();
	while(p){
		if(p -> flags & MCH_RACER_MECHOS_CFG)
			XBuf < p -> mechos_config < "\n";
		else
			XBuf < "---\n";
		p = p -> next;
	}
	if(RenderMode == XGRAPH_HICOLOR)
		XGR_OutText(10,5,0xFFFF,XBuf.address());
	else
		mchA_d3dOutString(10,5,1.0f,1.0f,XBuf.address(),mchA_ColorF[7],200,2,0);
}

const float AE_PART_RADIUS = 50.0f;
void mchA_DrawBonusPart(int id,int x,int y,int r,float alpha)
{
	cMesh* m;
	Mat3f mat;

	sColor4f col,col0;


	float sc;

	m = mchGetPartByArcane(id);
	if(m){
		col = col0 = m -> GetDiffuse();

		col.a *= alpha;
		m -> SetColor(col);

		mat.set(mchA_PartAxis,mchA_Part_Angle);
		sc = float(r) / m -> rmaxTotal() * mchA_d3dResX;
		m -> SetPosition(mchA_PartM * mat,Vect3f(float(x) * mchA_d3dResX,float(y) * mchA_d3dResY,0.0f));
		m -> SetScale(Vect3f(sc,sc,sc));
		mchA_DrawM3D(m);
		m -> SetScale(Vect3f(1,1,1));

		m -> SetColor(col0);
	}
}

void mchA_DrawStar(int x,int y,float sz,float alpha)
{
	if(!mchA_StarMesh) return;

	sColor4f col,col0;
	col = col0 = mchA_StarMesh -> GetDiffuse();

	col.a *= alpha;
	mchA_StarMesh -> SetColor(col);

	sz *= mchA_d3dResX;
	mchA_StarMesh -> SetScale(Vect3f(sz,sz,sz));

	Vect3f v1((float)x * mchA_d3dResX,(float)y * mchA_d3dResY,0);
	Vect3f v2(90,0,180.0f * mchA_Part_Angle / M_PI);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)mchA_StarMesh,
		&v1,
		&v2);

	mchA_DrawM3D(mchA_StarMesh);
	mchA_StarMesh -> SetScale(Vect3f(1,1,1));
	mchA_StarMesh -> SetColor(col0);
}

void mchA_DrawSpeedStar(int x,int y,float sz)
{
	if(!mchA_SpeedStarMesh) return;

	sz *= mchA_d3dResX;
	mchA_SpeedStarMesh -> SetScale(Vect3f(sz,sz,sz));

	Vect3f v1((float)x * mchA_d3dResX,(float)y * mchA_d3dResY,0);
	Vect3f v2(0,180.0f * mchA_Part_Angle / M_PI,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)mchA_SpeedStarMesh,
		&v1,
		&v2);

	mchA_DrawM3D(mchA_SpeedStarMesh);
	mchA_SpeedStarMesh -> SetScale(Vect3f(1,1,1));
}

void mchA_DrawBonus(int x,int y,int idx)
{
/*	
	if(!mchA_BonusMesh[idx]) return;
	mchA_BonusMesh[idx] -> SetScale(Vect3f(0.17f * mchA_d3dResX,0.17f * mchA_d3dResX,0.17f * mchA_d3dResX));
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)mchA_BonusMesh[idx],
		&Vect3f((float)x * mchA_d3dResX,(float)y * mchA_d3dResY,0),
		&Vect3f(0,180.0f * mchA_Part_Angle / M_PI,0));
	mchA_DrawM3D(mchA_BonusMesh[idx]);
	mchA_BonusMesh[idx] -> SetScale(Vect3f(1,1,1));
*/	
}

void mchA_ShowEnergy(int x,int y,float scale,float phase,float phase_scale,int alpha)
{
	float sz_x = 1.6f * scale * phase_scale;
	float sz_y = 1.6f * scale;

	if(phase < 0.0f) phase = 0.0f;
	if(phase > 1.0f) phase = 1.0f;

	if(mchSplitScreenGame){
		sz_x *= 0.7f;
		sz_y *= 0.7f;
	}

	mchA_SprD -> DrawSpriteClip(x,y,sz_x,sz_y,0.0f,0.4f,1.0f,0.6f,500,-1,alpha);
	mchA_SprD -> DrawSpriteClip(x,y,sz_x,sz_y,0.0f,0.675f,phase,0.875f,500,-1,alpha);
}

void mchA_ShowEnergyZ(int x,int y,int z,int sx,int sy,float phase,int alpha)
{
	int sx0,sx1;
	if(phase < 0.0f) phase = 0.0f;

	sx0 = round((float)sx * phase);
	sx1 = sx - sx0;

	if(RenderMode == DIRECT3D_HICOLOR){
		if(sx0)
			mchA_d3dRectangleZ(x,y,z,sx0,sy,2,0,alpha);
		if(sx1)
			mchA_d3dRectangleZ(x + sx0 - 1,y,z,sx1,sy,0,0,alpha);
	}
}

int mchA_PlayersArcane[HS_PLAYERS_MAX][NUM_ACTIVE_PARTS];

void mchA_InitLoadingScreen(void)
{
	int i,id,sz = 0;
	XBuffer XBuf;
	mchRacerStats* p,*rs,*rs0,*rs0_,*rs1;
	cMesh* m;
/*
#ifdef _DEBUG

	mchRacerStats* rrs[5];
	rs = mch_racerLst.first();
	for(i = 0; i < 5; i ++){
		rrs[i] = rs;
		rs = rs -> next;
	}
#endif
*/

//	mchA_PrepareLoadingImage(mchCurrentWorld,mchCurrentTrack);

	for(sz = 0; sz < HS_PLAYERS_MAX; sz ++){
		for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
			mchA_PlayersArcane[sz][mchA_PartNum[i]] = 1;
	}

	sz = 0;

	if(mchGameMode == MCH_SINGLE_GAME){
		rs0 = mch_racerLst.search(0);
		if(!rs0) return;

		rs0_ = mch_racerLst.search(1);

		rs = mch_racerLst.first();
		while(rs){
			if(rs -> essenceID >= acsMaxEssenceID)
				rs -> essenceID = rs0 -> essenceID;

			rs -> flags &= ~MCH_RACER_MECHOS_CFG;
			rs = rs -> next;
		}
		mchResetPartsPool(0);
		rs0 -> GetMechos(1);
		if(mchSplitScreenGame)
			rs0_ -> GetMechos(1);

		rs = mch_racerLst.first();
		while(rs){
			if(rs != rs0 && (!mchSplitScreenGame || rs != rs0_) && rs -> essenceID == rs0 -> essenceID){
				for(i = 0; i < acsMaxEssenceID; i ++){
					if(mch_trackD -> FigureEnabled(i)){
						rs1 = mch_racerLst.first();
						while(rs1){
							if(rs1 -> essenceID == i)
								break;
							rs1 = rs1 -> next;
						}
						if(!rs1){
							rs -> essenceID = i;
							rs -> GetMechos();
							break;
						}
					}
				}
			}
			rs = rs -> next;
		}

		rs = mch_racerLst.first();
		while(rs){
			if(!(rs -> flags & MCH_RACER_MECHOS_CFG))
				rs -> GetMechos();

			if(iWorldMode && rs != rs0 && (!mchSplitScreenGame || rs != rs0_))
				rs -> PutMechos();

			rs = rs -> next;
		}
		p = mch_racerLst.first();

		while(p){
			if(sz <= mchNumRacers){ 
				if(!(p -> flags & MCH_RACER_MECHOS_CFG))
					p -> GetMechos();

				XBuf.init();
				XBuf.write(p -> mechos_config,30);
				XBuf.set(0);

				for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
					XBuf >= id;
					m = MeshLibrary -> Find(M3D_TOTAL_TYPE(id,mchPartID[i]));
					if(m)
						mchA_PlayersArcane[p -> ID][mchA_PartNum[i]] = m -> arcaneData.ID;
				}
			}
			p = p -> next;
			sz ++;
		}
	}
	else {
		for(sz = 0; sz < HS_PLAYERS_MAX; sz ++){
			if(hsPlayers[sz].type){
				XBuf.init();
				XBuf.write(hsPlayers[sz].configStr,30);
				XBuf.set(0);

				for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
					XBuf >= id;
					m = MeshLibrary -> Find(M3D_TOTAL_TYPE(id,mchPartID[i]));
					if(m)
						mchA_PlayersArcane[sz][mchA_PartNum[i]] = m -> arcaneData.ID;
				}
			}
			else {
				for(i = 0; i < NUM_ACTIVE_PARTS; i ++)
					mchA_PlayersArcane[sz][mchA_PartNum[i]] = 1;
			}
		}
	}
}

void mchA_DrawLoadingScreen(int x,int y,int sx,int sy,int val,int max_val)
{
	float angle;
	int i,j,id,alpha,d_alpha,n,n0;

	const float r = 150.0f;
	const int num_parts = NUM_ACTIVE_PARTS - 1;
	const int sz = 50;

	const int xc  = 320;
	const int yc  = 240;
	const int xcl = xc - sz;

	const float d_alpha_max = 50.0f;

	n0 = 3 * num_parts * (HS_PLAYERS_MAX - 1) * val / max_val / 2;
	n = 1;

	mchA_d3dPrepare();
	mchA_d3dClear(0);
	mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);

	mchA_d3dBeginScene();

	d_alpha = round(d_alpha_max * (1.0f - (float)val / (float)max_val));
	if(d_alpha < 0) d_alpha = 0;

	angle = 0.0f;
	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		for(j = 0; j < NUM_ACTIVE_PARTS - 1; j ++){
			alpha = 200;	// - j * d_alpha;
			id = (i <= mchNumRacers) ? mchA_PlayersArcane[i][j] : 1;

			if(!i){
				mchA_SprD -> DrawSprite(xcl + j * sz,yc,0.8f,0.8f,id,-1,alpha,0.0f,1);
			}
			else {
				if(n <= n0){
					mchA_SprD -> DrawSprite(xc + round(r * cos(angle)),yc + round(r * sin(angle)),0.8f,0.8f,id,-1,alpha,0.0f,1);
					angle += 2.0f * M_PI / (float)(HS_PLAYERS_MAX - 1) / (float)num_parts;
				}
				n ++;
			}
		}
	}

//	mchA_d3dDrawScrollbar(x,y,sx,sy,val,max_val);
	mchA_d3dEndScene();
//	mchA_d3dFlip();
}

void mchA_DropCPDisable(mchRacer* p)
{
	if(p -> flags & MCH_ACTIVE_RACER2){
		mch_arcScrD -> playerSet2 -> cpEl -> Timer = 0;
		mch_arcScrD -> playerSet2 -> cpEl -> SetState(0);
	}
	else {
		mch_arcScrD -> playerSet1 -> cpEl -> Timer = 0;
		mch_arcScrD -> playerSet1 -> cpEl -> SetState(0);
	}
}

void mchInitPartIDs(void)
{
	mchPartID[0] = M3D_ENGINE;
	mchPartID[1] = M3D_FRONT;
	mchPartID[2] = M3D_RF_WHEEL;
	mchPartID[3] = M3D_RB_WHEEL;

	mchA_PartID[3] = M3D_ENGINE;
	mchA_PartID[1] = M3D_FRONT;
	mchA_PartID[0] = M3D_RF_WHEEL;
	mchA_PartID[2] = M3D_RB_WHEEL;
}

void mchA_ShowRacerPlace(int x,int y,int al,int finish,mchArcaneRacerSet* owner)
{
	float mul,tm;
	int place,col;
	mchA_Sprite* p;

	mchRacer* r = mch_raceD -> activeRacer;
	if(owner) r = owner -> owner;

	place = r -> stPtr -> place + 1;

	const int alpha_max = 250;
	const int alpha_min = 70;

	static int alpha = alpha_min;
	static int alpha_delta = 16;

	static int place_colors[5] = { 13, 14, 14, 14, 15 };
	mchA_XBuf.init();
//	mchA_XBuf <= place < mchA_GetNumSuffix(place);
	mchA_XBuf < mchA_GetNumSuffix(place);

	tm = r -> stPtr -> placeTimer - 250.0f;
	if(tm < 0.0f) tm = 0.0f;
	mul = 1.0f + tm / 250.0f * 0.2f;

	al = 255 - al/2;

	if(finish){
		alpha += alpha_delta;
		if(alpha > alpha_max){
			alpha = alpha_max;
			alpha_delta = -alpha_delta;
		}
		if(alpha < alpha_min){
			alpha = alpha_min;
			alpha_delta = -alpha_delta;
		}
		mul *= 1.5f;
		al = alpha;
	}
	else {
		if(mchSplitScreenGame) mul *= 0.7f;
	}

	col = place_colors[place - 1];
	if(place >= mch_raceD -> racerLst -> size())
		col = 15;

	p = mchA_SprD -> spriteLst.search(200 + place);
	x -= round(float(p -> bound[0]) * mul);
	mchA_SprD -> DrawSprite(x,y,mul,mul,200 + place,mchA_ColorF[col],al,0.0f,0);
	x += round(float(p -> SizeX - p -> bound[0] - p -> bound[1]) * mul) + 5;
	mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[col],al,1,1,mul);
}

void mchA_ShowRacerLap(int x,int y,int al,mchArcaneRacerSet* owner)
{
	int lap;
	mchRacer* r;

	float sc = (mchSplitScreenGame) ? 0.8f : 1.0f;

	if(owner) r = owner -> owner;
	else r = mch_raceD -> activeRacer;

	lap = r -> stPtr -> Lap + 1;
	if(lap > mchNumLaps) lap = mchNumLaps;

	mchA_XBuf.init();
	mchA_XBuf < iGetText(iTXT_LAP) <= lap < "/" <= mchNumLaps;

	if(RenderMode == DIRECT3D_HICOLOR)
		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[2],256 - al,1,1,sc);
}

const char* mchA_GetNumSuffix(int v)
{
	return iGetText(iTXT_PLACE0 + v - 1);
}

void mchArcaneScreenDispatcher::AdvancePhase(void)
{
	mchArcaneScreenElement* p;
	
	switch(showMode){
		case AE_BUILD_SCREEN:
			if(CheckBuildLevel())
				phaseIndex ++;

			if(phaseIndex >= 20)
				showMode = 0;
			break;
		case AE_BUILD_RACERS_SCREEN:	
			if(screenR == screenR_dest && screenG == screenG_dest && screenB == screenB_dest && screenA == screenA_dest)
				phaseIndex ++;
			
			if(mchSplitScreenGame || mchGameMode == MCH_SPLIT_CONTROL_HS || mchGameMode == MCH_ENTIRE_CONTROL_HS){
				if(!mchPBEM_Game && phaseIndex == 6){
					SetActiveEl(AE_PLAY_AGAIN_STR);
					p = objList -> search(AE_PLAY_AGAIN_STR);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
					p = objList -> search(AE_AWAY_STR2);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
				}
				break;
			}

			if(phaseIndex == 11){
				if(mchA_EndGameMode != AE_RACE_LOST && mchA_NStarNew != mchA_NStarOld)
					phaseIndex --;
			}

			if(phaseIndex == 6){
				if(mchA_EndGameMode != AE_RACE_LOST){
					if(!mch_trackD -> TrackBoss()){
						mchA_FinishNStarEl -> SetState(1);
						mchA_FinishNStarEl -> phase = mchA_FinishNStarEl -> dest_phase = 1.0f;
						mchA_FinishNStarEl -> Scale = mchA_FinishNStarEl -> destScale = 1.0f;
					}
					else {
						phaseIndex = 200;
					}
				}
				else {
					SetActiveEl(AE_TRY_STR);
					p = objList -> search(AE_LOST_STR);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
					p = objList -> search(AE_TRY_STR);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
					p = objList -> search(AE_AWAY_STR);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
				}
			}

			if(!mch_trackD -> TrackBoss()){
				if(mchA_EndGameMode != AE_RACE_LOST && phaseIndex == 200){
					mchA_FinishNStarEl -> dest_phase = 0.0f;
					mchA_FinishNStarEl -> destScale = 0.6f;
					mchA_NStarOld = mchA_NStarNew;

					mchStartSoundEFF(EFF_STAR_MOVE);
				}

				if(mchA_EndGameMode != AE_RACE_LOST && phaseIndex > 210 && mchA_NStarOld == mchA_NStarNew){
					mchA_FinishNStarEl -> dest_phase = 0.0f;
					mchA_FinishNStarEl -> destScale = 0.6f;

					if(mchA_FinishNStarEl -> phase >= 0.9f)
						mchStartSoundEFF(EFF_STAR_MOVE);
				}
			}

			if(mchA_EndGameMode != AE_RACE_LOST && phaseIndex == 201){
				if(!mchA_TrackPart){
					if(!mch_trackD -> TrackBoss()){
						SetActiveEl(AE_CONTINUE_STR);
						p = objList -> search(AE_CONTINUE_STR);
					}
					else {
						SetActiveEl(AE_WINNER2_STR);
						p = objList -> search(AE_WINNER2_STR);
					}
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}

					p = objList -> search(AE_NOT_LOST_STR);
					if(p){
						p -> phase = 0.0f;
						p -> dest_phase = 1.0f;
						p -> SetState(1);
					}
				}
				else {
					if(mchA_EndGameMode == AE_RACE_WON){
						SetActiveEl(AE_WINNER_STR);
						p = objList -> search(AE_WON_STR0);
						if(p){
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
						p = objList -> search(AE_WON_STR1);
						if(p){
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
						p = objList -> search(AE_WINNER_STR);
						if(p){
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
						p = objList -> search(AE_PART_EL);
						if(p){
							p -> exDataParam[0] = mchA_TrackPart;
							p -> SetState(1);
						}
						p = objList -> search(AE_PART_NAME);
						if(p){
							if(mchA_BonusMesh && mchA_BonusMesh -> arcaneData.ID != -1)
								p -> exDataPtr = mchA_BonusMesh -> arcaneData.name;
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
					}
					else {
						if(mch_raceD -> activeRacer -> NStar + mch_raceD -> activeRacer -> stPtr -> NStar >= mchA_TrackPartPrice){
							sprintf(mchA_PriceStr,iGetText(iTXT_PRICE),mch_trackD -> TrackPrice());

							SetActiveEl(AE_BUY_IT_STR);
							p = objList -> search(AE_BUY_STR);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_NO_THANKS_STR);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_BUY_IT_STR);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_PRICE_STR);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_PART_EL);
							if(p){
								p -> exDataParam[0] = mchA_TrackPart;
								p -> SetState(1);
							}
							p = objList -> search(AE_PART_NAME);
							if(p){
								if(mchA_BonusMesh && mchA_BonusMesh -> arcaneData.ID != -1)
									p -> exDataPtr = mchA_BonusMesh -> arcaneData.name;
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
						}
						else {
							sprintf(mchA_PriceStr,iGetText(iTXT_COST),mch_trackD -> TrackPrice());

							if(mchA_BonusMesh && mchA_BonusMesh -> arcaneData.ID != -1)
								sprintf(mchA_NotEnoughStr1,iGetText(iTXT_NOT_ENOUGH1),"\"",mchA_BonusMesh -> arcaneData.name,"\"");

							SetActiveEl(AE_TRY_STR2);
							p = objList -> search(AE_NEXT_TIME_STR);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_TRY_STR2);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_NOT_ENOUGH_STR0);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_NOT_ENOUGH_STR1);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							p = objList -> search(AE_PRICE_STR2);
							if(p){
								p -> phase = 0.0f;
								p -> dest_phase = 1.0f;
								p -> SetState(1);
							}
							iworld_D -> PutPhrase(IW_DOGGY_NOT_ENOUGH_MONEY);
						}
					}
				}
			}
			break;
		case AE_SHOW_SCREEN:
			phaseIndex ++;
			if(phaseIndex >= phaseIndexMax)
				showMode = 0;
			break;
		case AE_HIDE_SCREEN:
			phaseIndex --;
			if(phaseIndex < 0){
				if(mchTimeMode == MCH_TIME_GAME_OVER){
					if(showAction == AE_INIT_FINISH_SCREEN)
						InitRacersFinishScreen();
					if(showAction == AE_EXIT_FINISH_SCREEN)
						mchGameFlags |= MCH_START_TIME_FLAG;

					if(showAction == AE_FINIT_FINISH_SCREEN){
						mchStartSoundEFF(EFF_SCREEN_FINISH);
						p = objList -> search(AE_BIG_RECT1);
						if(p){
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
						p = objList -> search(AE_BIG_RECT2);
						if(p){
							p -> phase = 0.0f;
							p -> dest_phase = 1.0f;
							p -> SetState(1);
						}
						phaseIndex = 2;
						showAction = AE_EXIT_FINISH_SCREEN;
					}

				}
				else
					showMode = 0;
			}
			break;
	}
}

void mchArcaneScreenDispatcher::SetDestPhase(int ph)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		if(p -> showDir != -1)
			p -> dest_phase = ph;
		p = p -> next;
	}
}

void mchArcaneScreenDispatcher::SetPhase(int ph)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		if(p -> showDir != -1)
			p -> phase = p -> dest_phase = ph;
		p = p -> next;
	}
}

void mchArcaneScreenDispatcher::InitStartupScreen(void)
{
	mchArcaneScreenElement* p;

	phaseIndex = -2;
	showMode = AE_BUILD_SCREEN;

	SetPhase(0);
	SetDestPhase(1);

	p = objList -> search(AE_BIG_RECT1);
	if(p)
		p -> dest_phase = 0.0f;
	p = objList -> search(AE_BIG_RECT2);
	if(p)
		p -> dest_phase = 0.0f;
}

void mchArcaneScreenDispatcher::InitEndGameScreen(void)
{
	Visible = 1;

	phaseIndex = 2;
	showMode = AE_HIDE_SCREEN;
	showAction = AE_INIT_FINISH_SCREEN;

	screenR = 0;
	screenG = 0;
	screenB = 0;
	screenA = 0;

	screenR_dest = 0;
	screenG_dest = 0;
	screenB_dest = 0;
	screenA_dest = 130;

	SetDestPhase(0);
}

int mchArcaneScreenDispatcher::CheckBuildLevel(void)
{
	mchArcaneScreenElement* p;
	switch(phaseIndex){
		case -2:
			if(mchTrackDemoMode){
				p = objList -> search(AE_BIG_RECT1);
				if(!p) return 1;

				if(!p -> Active())
					return 0;
			}
			break;
		case -1:
			p = objList -> search(AE_BIG_RECT1);
				if(!p) return 1;

			if(mchTrackDemoMode){
				if(p -> phase == 1.0f)
					mchFinitTrackDemo();

				return 0;
			}
			else {
				if(p -> dest_phase){
					p -> dest_phase = 0.0f;
					p = objList -> search(AE_BIG_RECT2);
					if(p && p -> dest_phase)
						p -> dest_phase = 0.0f;
				}

				if(p -> phase > 0.01f)
					return 0;
			}
			break;
		case 0:
			if((mchA_TimerMax - mchA_TimerCur) > 1000 * 4 - 100)
				return 0;
			break;
		case 3:
			if(mchTimeMode == MCH_TIME_STOPPED)
				return 0;
			break;
		case 4:
			if(mch_raceD -> activeRacer -> stPtr -> time < 1000 * 2)
				return 0;
			break;
	}
	return 1;
}

void mchArcaneScreenDispatcher::InitRedraw(void)
{
	mchArcaneScreenElement* p = objList -> first();
	while(p){
		p -> InitRedraw();
		p = p -> next;
	}
}

void mchArcaneScreenDispatcher::InitRacersFinishScreen(void)
{
	int i,sz;
	mchRacer* p;
	mchArcaneScreenElement* el;

	sz = mch_raceD -> racerLst -> size();

	p = mch_raceD -> racerLst -> first();
	while(p){
		if(p -> flags & MCH_ACTIVE_RACER){
			if(sz == 1 || p -> stPtr -> place < sz - 1){
				mchA_NStarOld = p -> stPtr -> NStar;
				mchA_NStarNew = p -> stPtr -> NStar + p -> NStar;
			}
			else {
				mchA_NStarOld = mchA_NStarNew = 0;
			}
			break;
		}
		p = p -> next;
	}

	for(i = 0; i < 5; i ++){
		p = mch_raceD -> racerLst -> first();
		while(p){
			if(p -> stPtr -> place == i){
				mchA_FinishFigureEl[i] -> exDataParam[0] = p -> stPtr -> essenceID;
				mchA_FinishFigureEl[i] -> exDataParam[1] = p -> flags & MCH_ACTIVE_RACER;
				mchA_FinishFigureEl[i] -> exDataParam[2] = 0;

				if(mchGameMode != MCH_SPLIT_CONTROL_HS && mchGameMode != MCH_ENTIRE_CONTROL_HS){
					mchA_FinishFigureEl[i] -> exDataParam[3] = p -> NStar;
					mchA_FinishFigureEl[i] -> exDataParam[4] = p -> stPtr -> time / 1000;

					if(!(p -> flags & MCH_FINISHED))
						mchA_FinishFigureEl[i] -> exDataParam[4] = -1;
				}
				else {
					mchA_FinishFigureEl[i] -> exDataParam[3] = p -> stPtr -> Turn + 1;
					mchA_FinishFigureEl[i] -> exDataParam[4] = p -> stPtr -> time / 1000;

					if(!(p -> flags & MCH_FINISHED))
						mchA_FinishFigureEl[i] -> exDataParam[4] = -1;
				}

				if(!mchSplitScreenGame || p -> stPtr -> ID < 2)
					mchA_FinishFigureEl[i] -> exDataPtr = p -> name;
				else
					mchA_FinishFigureEl[i] -> exDataPtr = NULL;

				mchA_FinishFigureEl[i] -> SetState(1);
				break;
			}
			p = p -> next;
		}
		mchA_FinishFigureEl[i] -> phase = 0.0f;
		mchA_FinishFigureEl[i] -> dest_phase = 1.0f;
	}
	phaseIndex = -25;
	showMode = AE_BUILD_RACERS_SCREEN;
	XGR_MouseSetPos(20,20);

	if(!mchSplitScreenGame){
		el = objList -> search(AE_FINISH_PLACE);
		if(el){
			el -> phase = 0.0f;
			el -> dest_phase = 1.0f;
			el -> SetState(1);
		} 
	}
}

void mchArcaneScreenDispatcher::KeyHandler(int bt,int id)
{
	mchArcaneScreenElement* p;
//	if(mchTimeMode == MCH_TIME_GAME_OVER && mchA_EndGameMode != AE_RACE_LOST && phaseIndex < 199 && phaseIndex >= 10 && phaseIndex < 199 && id != MCH_KEY_EXIT && !mchSplitScreenGame && mchGameMode != MCH_SPLIT_CONTROL_HS && mchGameMode != MCH_ENTIRE_CONTROL_HS){
	if(mchTimeMode == MCH_TIME_GAME_OVER && mchA_EndGameMode != AE_RACE_LOST && phaseIndex < 199 && phaseIndex >= 10 && phaseIndex < 199 && !mchSplitScreenGame && mchGameMode != MCH_SPLIT_CONTROL_HS && mchGameMode != MCH_ENTIRE_CONTROL_HS){
		phaseIndex = 199;
		return;
	}
	if(activeEl){
		switch(bt){
			case VK_LEFT:
				if(activeEl -> objDir[0]){
					SetActiveEl(activeEl -> objDir[0]);
					return;
				}
				break;
			case VK_UP:
				if(activeEl -> objDir[1]){
					SetActiveEl(activeEl -> objDir[1]);
					return;
				}
				break;
			case VK_RIGHT:
				if(activeEl -> objDir[2]){
					SetActiveEl(activeEl -> objDir[2]);
					return;
				}
				break;
			case VK_DOWN:
				if(activeEl -> objDir[3]){
					SetActiveEl(activeEl -> objDir[3]);
					return;
				}
				break;
		}
	}

	p = objList -> first();
	while(p){
		p -> KeyHandler(bt,id);
		p = p -> next;
	}
}

void mchArcaneScreenElement::KeyHandler(int bt,int id)
{
	float x,y;
	if(!(flags & AE_SELECTED) || phase < 0.9f) return;

	if(bt == VK_SPACE || bt == VK_RETURN){
		x = (R.x + SizeX/2 - XGR_MouseObj.SizeX/2) * mchA_d3dResX;
		y = (R.y + SizeY/2 - XGR_MouseObj.SizeY/2) * mchA_d3dResY;

		XGR_MouseSetPos(round(x),round(y));
		MouseHandler(1);
	}
}

void mchArcaneScreenDispatcher::InitTrackDemoScreen(void)
{
	mchArcaneScreenElement* p;
	p = objList -> search(AE_SMALL_RECT1);
	if(p){
		p -> SetState(1);
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
	}
	p = objList -> search(AE_SMALL_RECT2);
	if(p){
		p -> SetState(1);
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
	}
}

void mchArcaneScreenDispatcher::InitTurnBasedScreen(void)
{
	mchArcaneScreenElement* p;

	p = objList -> search(AE_SMALL_RECT1);
	if(p){
		p -> phase = p -> dest_phase = 0.0f;
		p -> SetState(0);
	}
	p = objList -> search(AE_SMALL_RECT2);
	if(p){
		p -> phase = p -> dest_phase = 0.0f;
		p -> SetState(0);
	}
	p = objList -> search(AE_BIG_RECT1);
	if(p){
		p -> phase = p -> dest_phase = 0.0f;
		p -> SetState(0);
	}
	p = objList -> search(AE_BIG_RECT2);
	if(p){
		p -> phase = p -> dest_phase = 0.0f;
		p -> SetState(0);
	}
}

void mchArcaneScreenDispatcher::FinitTrackDemoScreen(void)
{
	mchArcaneScreenElement* p;

	if(mchTrackDemoMode == -1) return;

	mchTrackDemoMode = -1;

	p = objList -> search(AE_SMALL_RECT1);
	if(p) p -> dest_phase = 0.0f;
	p = objList -> search(AE_SMALL_RECT2);
	if(p) p -> dest_phase = 0.0f;

	p = objList -> search(AE_BIG_RECT1);
	if(p){
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
		p -> SetState(1);
	}
	p = objList -> search(AE_BIG_RECT2);
	if(p){
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
		p -> SetState(1);
	}
}

mchArcaneRacerSet::mchArcaneRacerSet(void)
{
	cpEl = new mchArcaneScreenElement;
	cpEl -> type = AE_DROP_TEXT;
	cpEl -> align_x = AE_CENTER;
	cpEl -> align_y = AE_CENTER;
	cpEl -> color = 2;
	cpEl -> SetState(0);
	cpEl -> SetString(0,1,mchA_DropStr);
	cpEl -> R.x = (640 - cpEl -> SizeX)/2;
	cpEl -> R.y = (480 - cpEl -> SizeY)/2;
	cpEl -> flags |= AE_ALPHA_SCALE | AE_CENTERED | AE_HALF_ALPHA;

	lapcntEl = new mchArcaneScreenElement;
	lapcntEl -> type = AE_LAP_COUNTER;
	lapcntEl -> color = 2;
	lapcntEl -> align_x = lapcntEl -> align_y = AE_CENTER;
	lapcntEl -> SetState(0);

	energyEl = new mchArcaneScreenElement;
	energyEl -> align_x = AE_LEFT;
	energyEl -> align_y = AE_BOTTOM;
	energyEl -> type = AE_ENERGY_BAR;

	placeEl = new mchArcaneScreenElement;
	placeEl -> type = AE_PLACE_STR;
	placeEl -> align_x = AE_LEFT;
	placeEl -> align_y = AE_TOP;
	placeEl -> color = 2;

	lapEl = new mchArcaneScreenElement;
	lapEl -> align_x = AE_LEFT;
	lapEl -> align_y = AE_BOTTOM;
	lapEl -> type = AE_LAP_STR;
	lapEl -> color = 2;

	mapEl = new mchArcaneScreenElement;
	mapEl -> type = AE_WORLD_MAP;
	mapEl -> align_x = AE_RIGHT;
	mapEl -> align_y = AE_BOTTOM;
	mapEl -> color = 2;

	starsEl = new mchArcaneScreenElement;
	starsEl -> align_x = AE_LEFT;
	starsEl -> align_y = AE_TOP;
	starsEl -> type = AE_STAR_COUNTER;
	const char* t2 = "99x99";
	starsEl -> SetString(MCH_AM_STAR_COUNT_FNT,1,t2);
	starsEl -> color = 3;

	arrowEl = new mchArcaneScreenElement;
	arrowEl -> align_x = AE_CENTER;
	arrowEl -> align_y = AE_TOP;
	arrowEl -> type = AE_ARROW;

	speedEl = new mchArcaneScreenElement;
	speedEl -> type = AE_SPEED_COUNTER;
	speedEl -> align_x = AE_RIGHT;
	speedEl -> align_y = AE_TOP;
	const char* t3 = "99x99";
	speedEl -> SetString(MCH_AM_STAR_COUNT_FNT,1,t3);
	speedEl -> color = 4;

	nameEl = new mchArcaneScreenElement;
	nameEl -> type = AE_NAME_STATUS_STR;
	nameEl -> align_x = AE_RIGHT;
	nameEl -> align_y = AE_BOTTOM;
	strcpy((char*)mchNameStr,"  ");
	nameEl -> SetString(MCH_AM_STATUS_FNT,MCH_AM_STATUS_SPACE,mchNameStr);
	nameEl -> flags |= AE_CENTERED | AE_ALIGN_RIGHT;
	nameEl -> color = 2;

	arrowAngle = 0.0f;

	owner = NULL;

	wndID = 0;
	mapFlag = 1;

	statD = new mchArcaneStatsDispatcher;
	menu = new mchArcaneMenu;
	menu -> align_x = AE_LEFT;

	menu -> statD = statD;
	menu -> align_x = AE_LEFT;
	menu -> align_y = AE_TOP;
}

void mchArcaneRacerSet::init(int align)
{
	int i;
	mchGameWindow* wnd;

	lapcntEl -> R.x = 320.0f;
	lapcntEl -> R.y = 240.0f;
	lapcntEl -> SizeX = lapcntEl -> SizeY = 0;

	cpEl -> SetString(0,1,mchA_DropStr);
	cpEl -> R.x = (640 - cpEl -> SizeX)/2;
	cpEl -> R.y = (480 - cpEl -> SizeY)/2;

	energyEl -> InitCoords("energy");
	placeEl -> InitCoords("place_str");
	lapEl -> InitCoords("lap_str");
	mapEl -> InitCoords("map");
	starsEl -> InitCoords("star_counter");
	arrowEl -> InitCoords("arrow");
	speedEl -> InitCoords("speed_counter");
	nameEl -> InitCoords("name_str");
	menu -> InitCoords("arcane_menu");

	statD -> X = atoi(getIniKey("RESOURCE\\ISCREEN\\iscreen.ini","world_interface","arc_stats_x"));
	statD -> Y = atoi(getIniKey("RESOURCE\\ISCREEN\\iscreen.ini","world_interface","arc_stats_y"));
	statD -> Scale(1.0f);

	menu -> Scale(1.0f);

	wndID = 0;

	for(i = 0; i < AE_STATS_MAX; i ++)
		statD -> data[i] -> showDir = 0;

	if(align != -1){
		cpEl -> SetString(1,1,mchA_DropStr);
		cpEl -> R.x = (640 - cpEl -> SizeX)/2;
		cpEl -> R.y = (480 - cpEl -> SizeY)/2;

		lapEl -> R.y += 17.0f;

		starsEl -> R.y -= 10.0f;
		speedEl -> R.x += 30.0f;
		speedEl -> R.y -= 10.0f;

		energyEl -> R.x -= 30.0f;
		energyEl -> R.y += 10.0f;
		energyEl -> ScaleSize(0.7f);

		arrowEl -> ScaleSize(0.7f);

		statD -> Scale(0.75f);
		menu -> Scale(0.75f);

		if(mchSplitScreenMode){
			statD -> Y -= 17;
			menu -> R.y -= 17;
			placeEl -> R.y -= 50.0f;
		}
		else {
			placeEl -> showDir = 3;
			for(i = 0; i < AE_STATS_MAX; i ++)
				statD -> data[i] -> showDir = 1;
		}

		wnd = (align) ? iWnd1 : iWnd0;

		lapcntEl -> ScaleCoords(iWnd,wnd);
		energyEl -> ScaleCoords(iWnd,wnd);
		placeEl -> ScaleCoords(iWnd,wnd);
		lapEl -> ScaleCoords(iWnd,wnd);
		mapEl -> ScaleCoords(iWnd,wnd);
		starsEl -> ScaleCoords(iWnd,wnd);
		arrowEl -> ScaleCoords(iWnd,wnd);
		speedEl -> ScaleCoords(iWnd,wnd);
		nameEl -> ScaleCoords(iWnd,wnd);
		cpEl -> ScaleCoords(iWnd,wnd);
		menu -> ScaleCoords(iWnd,wnd);
		statD -> ScaleCoords(iWnd,wnd);

		wndID = align;
	}
}

mchArcaneRacerSet::~mchArcaneRacerSet(void)
{
	delete starsEl;
	delete speedEl;
	delete energyEl;
	delete arrowEl;
	delete nameEl;
	delete placeEl;
	delete lapEl;
	delete mapEl;
	delete lapcntEl;
	delete cpEl;
}

void mchArcaneScreenDispatcher::insert(mchArcaneRacerSet* p)
{
	int i;

	if(!p -> lapcntEl -> list)
		objList -> append(p -> lapcntEl);
	if(!p -> starsEl -> list)
		objList -> append(p -> starsEl);
	if(!p -> speedEl -> list)
		objList -> append(p -> speedEl);
	if(!p -> energyEl -> list)
		objList -> append(p -> energyEl);
	if(!p -> arrowEl -> list)
		objList -> append(p -> arrowEl);
	if(!p -> nameEl -> list)
		objList -> append(p -> nameEl);
	if(!p -> placeEl -> list)
		objList -> append(p -> placeEl);
	if(!p -> lapEl -> list)
		objList -> append(p -> lapEl);
	if(!p -> mapEl -> list)
		objList -> append(p -> mapEl);

	mch_AMenu = p -> menu;
	if(!p -> menu -> list)
		objList -> append(p -> menu);

	if(!p -> cpEl -> list)
		objList -> append(p -> cpEl);

	mch_arcStatD = p -> statD;

	for(i = 0; i < AE_STATS_MAX; i ++){
		if(!p -> statD -> data[i] -> list)
			objList -> append(p -> statD -> data[i]);
	}

	p -> dropCp = p -> dropCpLap = p -> dropIdx = p -> dropCnt = 0;
	p -> lapcntEl -> SetState(0);
	p -> cpEl -> SetState(0);

	p -> SetState(1);
}

void mchArcaneScreenDispatcher::remove(mchArcaneRacerSet* p)
{
	int i;

	if(p -> starsEl -> list)
		objList -> remove(p -> starsEl);
	if(p -> speedEl -> list)
		objList -> remove(p -> speedEl);
	if(p -> energyEl -> list)
		objList -> remove(p -> energyEl);
	if(p -> arrowEl -> list)
		objList -> remove(p -> arrowEl);
	if(p -> nameEl -> list)
		objList -> remove(p -> nameEl);
	if(p -> placeEl -> list)
		objList -> remove(p -> placeEl);
	if(p -> lapEl -> list)
		objList -> remove(p -> lapEl);
	if(p -> mapEl -> list)
		objList -> remove(p -> mapEl);
	if(p -> lapcntEl -> list)
		objList -> remove(p -> lapcntEl);
	if(p -> cpEl -> list)
		objList -> remove(p -> cpEl);
	if(p -> menu -> list)
		objList -> remove(p -> menu);

	for(i = 0; i < AE_STATS_MAX; i ++){
		if(p -> statD -> data[i] -> list)
			objList -> remove(p -> statD -> data[i]);
	}
}

void mchArcaneRacerSet::SetState(int st)
{
	starsEl -> SetState(st);
	speedEl -> SetState(st);
	energyEl -> SetState(st);
	arrowEl -> SetState(st);
	nameEl -> SetState(st);
	placeEl -> SetState(st);
	lapEl -> SetState(st);

	if(mapFlag && st)
		mapEl -> SetState(1);
	else
		mapEl -> SetState(0);
}

void mchArcaneRacerSet::ToggleMap(void)
{
	mapFlag ^= 1;

	mapEl -> SetState(mapFlag);
	mapEl -> phase = mapEl -> dest_phase = 1.0f;
}

void mchArcaneStatsDispatcher::ScaleCoords(struct mchGameWindow* src,struct mchGameWindow* dest)
{
	int dx;
	if(dest -> SizeX < src -> SizeX){
		dx = X - src -> PosX;
		X = dest -> PosX + dx;
	}
	else {
		Y += dest -> PosY;
	}
}

void mchArcaneScreenElement::ScaleCoords(struct mchGameWindow* src,struct mchGameWindow* dest)
{
	int dx,dy;
	if(dest -> SizeX < src -> SizeX){
		switch(align_x){
			case AE_LEFT:
				dx = R.x - src -> PosX;
				R.x = dest -> PosX + dx;
				break;
			case AE_RIGHT:
				dx = R.x - (src -> PosX + src -> SizeX);
				R.x = dest -> PosX + dest -> SizeX + dx;
				break;
			case AE_CENTER:
				R.x = dest -> CenterX - SizeX/2;
				break;
		}
	}
	else {
		switch(align_y){
			case AE_TOP:
				dy = R.y - src -> PosY;
				R.y = dest -> PosY + dy;
				break;
			case AE_BOTTOM:
				dy = R.y - (src -> PosY + src -> SizeY);
				R.y = dest -> PosY + dest -> SizeY + dy;
				break;
			case AE_CENTER:
				R.y = dest -> CenterY - SizeY/2;
				break;
		}
	}
	InitR();
}

void mchArcaneScreenDispatcher::prepare(void)
{
	if(mchSplitScreenGame){
		playerSet1 -> init(0);
		playerSet2 -> init(1);

		insert(playerSet1);
		insert(playerSet2);

		playerSet1 -> SetOwner(mch_raceD -> activeRacer);
		playerSet2 -> SetOwner(mch_raceD -> activeRacer2);
	}
	else {
		playerSet1 -> init();

		insert(playerSet1);
		remove(playerSet2);

		SetLastEl(AE_WAITING_TEXT);

		playerSet1 -> SetOwner(mch_raceD -> activeRacer);
	}
}

void mchArcaneRacerSet::SetOwner(struct mchRacer* p)
{
	owner = p;
	starsEl -> owner = this;
	speedEl -> owner = this;
	energyEl -> owner = this;
	arrowEl -> owner = this;
	nameEl -> owner = this;
	placeEl -> owner = this;
	lapEl -> owner = this;
	mapEl -> owner = this;
	lapcntEl -> owner = this;
	cpEl -> owner = this;
	menu -> owner = this;
}

void mchArcaneScreenElement::ScaleSize(float sc)
{
	int sx,sy;

	sx = round(float(SizeX) * sc);
	sy = round(float(SizeY) * sc);

	switch(align_x){
		case AE_RIGHT:
			R.x += SizeX - sx;
			break;
		case AE_CENTER:
			R.x += (SizeX - sx)/2; 
			break;
	}
	switch(align_y){
		case AE_BOTTOM:
			R.y += SizeY - sy;
			break;
		case AE_CENTER:
			R.y += (SizeY - sy)/2; 
			break;
	}

	SizeX = sx;
	SizeY = sy;
}

void mchArcaneMenu::Scale(float sc)
{
	mchArcaneMenuElement* p = optionsLst -> first();

	while(p){
		p -> SizeX = round(float(MCH_AM_SX) * sc);
		p -> SizeY = round(float(MCH_AM_SY) * sc);
		p = p -> next;
	}
}

void mchArcaneRacerSet::Quant(void)
{
	int cp_flag = 0,dist;

	mchRacer* p;
	mchCheckpoint* cp,*cp1;

	if(!owner) return;

	if(mchTimeMode != MCH_TIME_RUNNING) return;

	p = owner;
	cp = mch_raceD -> getCP(p -> stPtr -> Checkpoint);

	if(p -> last_trackP0){
		cp_flag = 1;
		cp1 = p -> last_trackP0 -> owner -> owner;
		dist = p -> last_trackP0 -> index - p -> last_trackP0 -> owner -> pointLst -> first() -> index;
		if(cp -> ID == mch_raceD -> cpLst -> size() - 1){
			if(!cp1 -> ID && dist >= 3)
				cp_flag = 0;
		}
		else {
			if(cp1 -> ID == cp -> ID + 1 && dist >= 3)
				cp_flag = 0;
		}

		if(!cp_flag && (dropCp != cp -> ID || dropCpLap != p -> stPtr -> Lap)){
			if(dropCnt){
				if(p -> last_trackP0 -> index - dropIdx >= 3){
					mch_raceD -> activeRacer -> StartEffect(Mechos::Effect::CheckPointOmissionEffect);
					dropIdx = dropCnt = 0;

					if(!mchPBEM_HiddenTime){
						mchStartSoundEFF(EFF_DROP);

						cpEl -> SetState(1);

						if(mchGameMode != MCH_SINGLE_GAME || mchTurnBasedGame)
							cpEl -> Timer = 50;
					}

					dropCp = cp -> ID;
					dropCpLap = p -> stPtr -> Lap;

					dropTimer = mch_clock();
				}
			}
			else {
				dropIdx = p -> last_trackP0 -> index;
				dropCnt = 1;
			}
		}
	}
}

void mchA_HandleStatsEvent(int objID,int ev_code,void* p,int n)
{
	if(n)
		mch_arcScrD -> playerSet2 -> statD -> HandleEvent(objID,ev_code,p);
	else
		mch_arcScrD -> playerSet1 -> statD -> HandleEvent(objID,ev_code,p);
}

void mchA_AddArcaneStats(int arcID,int objID,int n)
{
	if(n)
		mch_arcScrD -> playerSet2 -> statD -> Add(arcID,objID);
	else
		mch_arcScrD -> playerSet1 -> statD -> Add(arcID,objID);
}

void mchArcaneScreenDispatcher::InitChoiceScreen(void)
{
	mchArcaneScreenElement* p;

	SetActiveEl(AE_TRY_STR2);
	p = objList -> search(AE_NEXT_TIME_STR);
	if(p){
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
		p -> SetState(1);
	}
	p = objList -> search(AE_TRY_STR2);
	if(p){
		p -> phase = 0.0f;
		p -> dest_phase = 1.0f;
		p -> SetState(1);
	}

	p = objList -> search(AE_BUY_STR);
	if(p) p -> dest_phase = 0.0f;

	p = objList -> search(AE_NO_THANKS_STR);
	if(p) p -> dest_phase = 0.0f;

	p = objList -> search(AE_BUY_IT_STR);
	if(p) p -> dest_phase = 0.0f;

	p = objList -> search(AE_PRICE_STR);
	if(p) p -> dest_phase = 0.0f;

	p = objList -> search(AE_PART_EL);
	if(p)
		p -> SetState(0);

	p = objList -> search(AE_PART_NAME);
	if(p) p -> dest_phase = 0.0f;
}

void mchInitChoiceScreen(void)
{
	mch_arcScrD -> InitChoiceScreen();
}

void mchA_DrawTips(void)
{
	int x,y,fnt,dy;
	if(!mchEnableTips || !mchTurnBasedGame || !mch_arcScrD -> Visible) return;

	const char* enterStr,*spaceStr,*insStr,*altStr,*delStr,*pgupStr,*pgdnStr;

	const float sc = 0.75f;
/*
	x = 225;
	y = 415;
*/
	x = 10;
	y = 375;

	fnt = 2;
	dy = 12;

	if(mchTimeMode == MCH_TIME_STOPPED){
		if(mchPBEM_Pause) return;

		enterStr = mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig()));
		spaceStr = mchGetKeyNameText(mchGetKey(MCH_KEY_ROTATE_CAM,mchGetKeysConfig()));
		insStr = mchGetKeyNameText(mchGetKey(MCH_KEY_CENTER_VIEW,mchGetKeysConfig()));
		altStr = mchGetKeyNameText(mchGetKey(MCH_KEY_ARCANE_MENU,mchGetKeysConfig()));
		delStr = mchGetKeyNameText(mchGetKey(MCH_KEY_REMOVE_SEED,mchGetKeysConfig()));
		pgupStr = mchGetKeyNameText(mchGetKey(MCH_KEY_ZOOM_IN,mchGetKeysConfig()));
		pgdnStr = mchGetKeyNameText(mchGetKey(MCH_KEY_ZOOM_OUT,mchGetKeysConfig()));

		sprintf(mchA_String,iGetText(iTXT_TIPS00));
		mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		y += dy;

		sprintf(mchA_String,iGetText(iTXT_TIPS01));
		mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		y += dy;

		sprintf(mchA_String,iGetText(iTXT_TIPS02),enterStr);
		mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		y += dy;

		sprintf(mchA_String,iGetText(iTXT_TIPS03),spaceStr,insStr);
		mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		y += dy;

		sprintf(mchA_String,iGetText(iTXT_TIPS04),delStr,altStr);
		mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		y += dy;
	}
	if(mchTimeMode == MCH_TIME_RUNNING){
		enterStr = mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig()));

		if(enterStr){
			y += dy * 4;

			sprintf(mchA_String,iGetText(iTXT_TIPS1),enterStr);
			mchA_d3dOutString(x,y,mchA_FontScaleX[fnt],mchA_FontScaleY[fnt],mchA_String,mchA_ColorF[2],200,fnt,0,sc);
		}
	}
}

void mchArcaneScreenDispatcher::TimerQuant(void)
{
	if(mchA_TimerMax){
		if(mchGameMode == MCH_SINGLE_GAME){
/*
			if(mchTimeMode == MCH_TIME_RUNNING)
				mchA_TimerCur = mchA_TimerMax;
			else {
				if(!camera_dispatcher -> isInterpolationCompleted())
					mchA_TimerStart = non_stop_global_time();

				mchA_TimerCur = non_stop_global_time() - mchA_TimerStart;
			}

			if(mchA_TimerCur >= mchA_TimerMax){
				mchA_TimerCur = mchA_TimerMax = 0;
				mchGameFlags |= MCH_TIMEOUT;
				mchGameFlags |= MCH_START_TIME_FLAG;
				if(mchA_CountEl -> Active()){ 
					mchA_CountEl -> Timer = 0;
				}
				mch_raceD -> SetBrake(0);
				mch_raceD -> SetTime(mch_clock());
			}
*/
			if(mchTimeMode != MCH_TIME_RUNNING){
				if(!camera_dispatcher -> isInterpolationCompleted())
					mchA_TimerStart = non_stop_global_time();
			}

			mchA_TimerCur = non_stop_global_time() - mchA_TimerStart;

			if(mchTimeMode != MCH_TIME_RUNNING && mchA_TimerCur >= mchA_TimerMax - 999){
//				mchA_TimerCur = mchA_TimerMax = 0;
				mchGameFlags |= MCH_TIMEOUT;
				mchGameFlags |= MCH_START_TIME_FLAG;
				if(mchA_CountEl -> Active()){ 
					mchA_CountEl -> Timer = 0;
				}
				mch_raceD -> SetBrake(0);
				mch_raceD -> SetTime(mch_clock());
			}

			if(mchA_TimerCur >= mchA_TimerMax)
				mchA_TimerCur = mchA_TimerMax = 0;
		}
		else {
			if(mchTimeMode != MCH_TIME_RUNNING){
				if(!camera_dispatcher -> isInterpolationCompleted())
					mchA_TimerStart = non_stop_global_time();

				mchA_TimerCur = non_stop_global_time() - mchA_TimerStart;

				if(mchTimeMode == MCH_TIME_WAITING_TURN && mch_raceD -> activeRacer -> flags & MCH_AI && mchA_TimerCur >= 1000 * 2)
					mchA_TimerCur = mchA_TimerMax;
			}
			else
				mchA_TimerCur = non_stop_global_time() - mchA_TimerStart;

			if(mchA_TimerCur >= mchA_TimerMax){
				mchA_TimerCur = mchA_TimerMax = 0;
				mchGameFlags |= MCH_TIMEOUT;

				if(mchTimeMode == MCH_TIME_RUNNING)
					mchGameFlags |= MCH_STOP_TIME_FLAG;
				else
					mchGameFlags |= MCH_START_TIME_FLAG;
			}
		}
	}

	playerSet1 -> Quant();
	if(mchSplitScreenGame)
		playerSet2 -> Quant();
}

void mchA_DrawCharBitmap(int x,int y,int sx,int sy,const void* text,void* bitmap,int fnt,int sp,int vsp,float scale_x,float scale_y)
{
	int i,j,idx = 0;
	float _x,_y,fsx,fsy,ssx,ssy;
	unsigned r,g,b,col;

	unsigned char* bp = (unsigned char*)bitmap;
	const unsigned char* tp = (const unsigned char*)text;

	_x = x;
	_y = y;

	fsx = float(acsFntTable[fnt] -> SizeX + sp) * scale_x;
	fsy = float(acsFntTable[fnt] -> SizeY + vsp) * scale_y;

	ssx = mchA_FontScaleX[fnt] * scale_x;
	ssy = mchA_FontScaleY[fnt] * scale_y;

	for(i = 0; i < sy; i ++){
		for(j = 0; j < sx; j ++){
			r = bp[idx * 3 + 0];
			g = bp[idx * 3 + 1];
			b = bp[idx * 3 + 2];

			if(r && g && b){
				col = mchA_d3dMakeColor(r,g,b);
				mchA_d3dOutSprite(round(_x),round(_y),ssx,ssy,tp[idx],col,200,0.0f,0);
			}

			_x += fsx;
			idx ++;
		}
		_x = x;
		_y += fsy;
	}
}

hbm_Image* testHBM = NULL;
unsigned char* testTXT = NULL;

void mchA_CharBitmapTest(void)
{
	int i,sz,x,y;
	XStream fh;

	float fsx,fsy;

	const int fnt = 2;
	const float scale_x = 0.7f;
	const float scale_y = 0.5f;
	const int sp = -5;
	const int vsp = -2;

	fsx = float(acsFntTable[fnt] -> SizeX + sp) * scale_x;
	fsy = float(acsFntTable[fnt] -> SizeX + vsp) * scale_y;

	if(!testHBM){
		testHBM = new hbm_Image;
		testHBM -> load("RESOURCE\\ISCREEN\\logo.hbm",0);

		sz = testHBM -> SizeX * testHBM -> SizeY;
		testTXT = new unsigned char[sz];

		for(i = 0; i < sz; i ++)
			testTXT[i] = 'A' + XRnd('Z' - 'A');
	}

	x = (640 - round(fsx * float(testHBM -> SizeX)))/2;
	y = (480 - round(fsy * float(testHBM -> SizeY)))/2;

	mchA_DrawCharBitmap(x,y,testHBM -> SizeX,testHBM -> SizeY,testTXT,testHBM -> frameTable[0] -> dataPtr,fnt,sp,vsp,scale_x,scale_y);
}

void mchA_PrepareLoadingImage(int wrld,int tr)
{
	int x,y,img_sx,img_sy;
	unsigned short* img_ptr;

	XStream fh(0);

	mchA_XBuf.init();

	mchA_XBuf < "SCREENS\\";

	if(xgrGameMode <= MCH_640x480)
		mchA_XBuf < "LOW\\";

	if(wrld < 10) mchA_XBuf < "s0" <= wrld < "_";
	else mchA_XBuf < "shot" <= wrld < "_";

	if(tr < 10) mchA_XBuf < "0" <= tr < ".jpg";
	else mchA_XBuf <= tr < ".jpg";

	if(acsOpenResource(mchA_XBuf.address(),fh)){
		img_ptr = loadJPG(&fh,img_sx,img_sy);

		if(img_ptr){
			x = (XGR_MAXX - img_sx)/2;
			y = (XGR_MAXY - img_sy)/2;

			mchA_d3dLockBackBuffer();
			XGR_Obj.putspr16(x,y,img_sx,img_sy,img_ptr,XGR_BLACK_FON);
			mchA_d3dUnlockBackBuffer();

			delete[] img_ptr;
		}
	}
	else {
		if(wrld != 7)
			mchA_PrepareLoadingImage(7,0);
	}
}

int mchA_GammaSeqDir = 0;
int mchA_GammaSeqMode = 0;
int mchA_GammaSeqStartTime = 0;

void mchA_SetGammaSeq(int dir,int img_mode)
{
	mchA_GammaSeqDir = dir;
	mchA_GammaSeqMode = img_mode;
	mchA_GammaSeqStartTime = clocki();
}

void mchA_EndGammaSeq(void)
{
	mchA_GammaSeqDir = 0;
	mchA_GammaSeqMode = 0;
	mchA_GammaSeqStartTime = 0;
}

void mchA_GammaSeqQuant(void)
{
	int v = (clocki() - mchA_GammaSeqStartTime) >> 1;

	if(v < 0) v = 0;
	if(v >= 255) v = 255;

	if(mchA_GammaSeqDir > 0){
		if(mchA_GammaSeqMode == GS_SHOW_LOADING_IMAGE){
			mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);
			gb_IGraph3d->BeginScene();
		}

		gb_IGraph3d->SetViewColor(0,0,0,255 - v);
		if(v == 255)
			mchA_GammaSeqDir = 0;
	}
	else {
		if(mchA_GammaSeqMode == GS_HIDE_LOADING_IMAGE){
			gb_IGraph3d->EndScene();
			mchA_DrawLoadingScreen(0,0,10,10,100,100);
			gb_IGraph3d->BeginScene();
		}

		gb_IGraph3d->SetViewColor(0,0,0,v);
		if(v == 255){
			mchA_GammaSeqDir = 0;

			switch(mchA_GammaSeqMode){
				case GS_SHOW_LOADING_IMAGE:
					gb_IGraph3d->EndScene();
					mchA_d3dCreateBackBuffer();
					mchA_PrepareLoadingImage(mchCurrentWorld,mchCurrentTrack);
					gb_IGraph3d->BeginScene();
					mchA_SetGammaSeq(1,GS_SHOW_LOADING_IMAGE);
					break;
				case GS_HIDE_LOADING_IMAGE:
					mchA_SetGammaSeq(1,GS_SHOW_WORLD);
					break;
			}
		}
	}
}

int mchA_isGammaSeqActive(void)
{
	return mchA_GammaSeqDir;
}

int mchA_GetGammaSeqMode(void)
{
	return mchA_GammaSeqMode;
}

void mchA_ShowLayoutID(void)
{
#ifdef _LOCAL_VERSION_
	const int x = 610;
	const int y = 2;
//	const int y = 460;

	const char* str = iGetText(iTXT_OTHER_LAYOUT);
	int id = win32_GetKeybLayoutID();

	if(id == 0){
		str = iGetText(iTXT_ENG_LAYOUT);
	}
	else {
		if(id == 1)
			str = iGetText(iTXT_LOCAL_LAYOUT);
	}

	mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],str,mchA_ColorF[2],200,2,0,1.0f);
#endif
}

void mchA_BuildMapImage(void)
{
	int x,y,dx,dy,cl;

	mchTrackBranch* br;
	mchTrackElement* el0,*el;
	mchCheckpoint* cp0,*cp;

	mchArcaneBMP* bp = mchA_MapPointer[0];

	char* p = (char*)mchA_CurWorld -> data;

	memset(p,0,64*64);

	el0 = NULL;

	cp = mch_raceD -> cpLst -> first();

	if(!cp) return;

	cp0 = cp -> prev;

	br = cp0 -> trackPtr -> first();
	if(br)
		el0 = br -> pointLst -> last();

	while(cp){
		x = (XCYCL((cp -> PosX1 + cp -> PosX0)/2) >> 5) - bp -> SizeX/2;
		y = (YCYCL((cp -> PosY1 + cp -> PosY0)/2) >> 5) - bp -> SizeY/2;

		mchA_MapPutspr(x,y,bp -> SizeX,bp -> SizeY,(char*)bp -> data,p);

		br = cp -> trackPtr -> first();
		while(br){
			el = br -> pointLst -> first();

			if(el0){
				dx = getDistX(el -> Cx,el0 -> Cx);
				dy = getDistY(el -> Cy,el0 -> Cy);

				if(dx <= (bp -> SizeX << 5) && dy <= (bp -> SizeY << 5)){
					x = (XCYCL(el0 -> Cx + dx/2) >> 5) - bp -> SizeX/2;
					y = (YCYCL(el0 -> Cy + dy/2) >> 5) - bp -> SizeY/2;
					
//					mchA_MapPutspr(x,y,bp -> SizeX,bp -> SizeY,(char*)bp -> data,p);
				}
			}

			while(el){
				x = (XCYCL(el -> Cx) >> 5) - bp -> SizeX/2;
				y = (YCYCL(el -> Cy) >> 5) - bp -> SizeY/2;

				mchA_MapPutspr(x,y,bp -> SizeX,bp -> SizeY,(char*)bp -> data,p);

				el = el -> next;
			}
			el0 = br -> pointLst -> last();

			br = br -> next;
		}
		cp = cp -> next;
	}

	cp = mch_raceD -> cpLst -> first();
	while(cp){
		br = cp -> trackPtr -> first();
		while(br){
			br -> clean();
			br = br -> next;
		}
		cp = cp -> next;
	}

	dx = 0;
	for(y = 0; y < 64; y ++){
		for(x = 0; x < 64; x ++){
			if(x && y && x < 63 && y < 63) {
				cl = (p[dx - 1] + p[dx - 64] + p[dx + 1] + p[dx + 64]) >> 2;
				p[dx] = cl;
			}
			dx ++;
		}
	}
}

void mchA_MapPutspr(int x,int y,int sx,int sy,char* spr,char* p)
{
	int i,j,spr_idx,p_idx,cl;

	spr_idx = 0;
	p_idx = x + (y << 6);

	for(i = 0; i < sy; i ++){
		for(j = 0; j < sx; j ++){
			if(i + y > 0 && i + y < 64 && j + x > 0 && j + x < 64){
				cl = p[p_idx + j] + spr[spr_idx + j];
				if(cl > 31) cl = 31;
				p[p_idx + j] = cl;
			}
		}
		spr_idx += sx;
		p_idx += 64;
	}
}

int mchArcaneRoundMenu::CheckMouse(void)
{
	if(mouseEl) return 1;

	return 0;
}


void mchA_DrawOnlineScreen(void)
{
	int x,y,sx;
	const char* message;

	y = 200;

	if(mchPBEM_CheckFlag(PBEM_ERROR)){
		ogGetErrorStr();
		sx = acsStrLen(0,(unsigned char*)ogErrorStr,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[0],mchA_FontScaleY[0],ogErrorStr,mchA_ColorF[2],200,0,1,1.0f);
		y += 60;

		sprintf(mchA_String,iGetText(iTXT_RETRY));

		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[2],200,1,1,1.0f);
	}

	if(mchPBEM_CheckFlag(PBEM_CONNECTED))
		message = (mchPBEM_DataFlag == -1) ? iGetText(iTXT_SEND) : iGetText(iTXT_RECEIVE);
	else
		message = iGetText(iTXT_CONNECT);

	mchA_d3dOutString(5,460,mchA_FontScaleX[2],mchA_FontScaleY[2],message,mchA_ColorF[6],160,2,0,0.8f);
}

void mchA_SetTurnFlag(int fl,int id)
{
	if(id != -1)
		mchA_TurnFlags[id] = fl;
	else
		memset(mchA_TurnFlags,fl,HS_PLAYERS_MAX);
}

void mchArcaneStatsDispatcher::GetCoords(int id,int& x,int& y)
{
	int i;

	for(i = 0; i < AE_STATS_MAX; i ++){
		if(!(data[i] -> flags & AE_HIDDEN) && data[i] -> exDataParam[0] == id){
			x = data[i] -> R.xi() + data[i] -> SizeX/2;
			y = data[i] -> R.yi() + data[i] -> SizeY/2;

			return;
		}
	}
}

void mchA_DrawPBEM_FinishScreen(void)
{
	int x,y,sx,color = 2;

	if(mchPBEM_Game && mchTimeMode == MCH_TIME_GAME_OVER && mchTurn){
		y = 430;
		sprintf(mchA_String,iGetText(iTXT_REPLAY_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY,mchGetKeysConfig())));

		sx = acsStrLen(2,(unsigned char*)mchA_String,0);
		x = (640 - sx) / 2;

		if(!mchPBEM_CheckFlag(PBEM_REPLAY_MODE))
			mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_String,mchA_ColorF[color],200,2,0,1.0f);

		y += 20;
		sprintf(mchA_String,iGetText(iTXT_REPLAY_GAME),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY_ALL,mchGetKeysConfig())));

		sx = acsStrLen(2,(unsigned char*)mchA_String,0);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_String,mchA_ColorF[color],200,2,0,1.0f);
	}
}

void mchA_DrawRealClock(void)
{
	int col = 2,sx,time,hrs,min,sec;

	time = (mchRealClockEnd - mchRealClockStart) / 1000;

	mchA_XBuf.init();

	hrs = time / 3600;
	min = (time - hrs * 3600) / 60;
	sec = time % 60;

	if(hrs < 10) mchA_XBuf < "0" <= hrs < ":";
	else mchA_XBuf <= hrs < ":";

	if(min < 10) mchA_XBuf < "0" <= min < ":";
	else mchA_XBuf <= min < ":";

	if(sec < 10) mchA_XBuf < "0" <= sec;
	else mchA_XBuf <= sec;

	sx = acsStrLen(2,(unsigned char*)mchA_XBuf.address(),0);

	mchA_d3dOutString(635 - sx,460,mchA_FontScaleX[2],mchA_FontScaleY[2],mchA_XBuf.address(),mchA_ColorF[2],200,2,0);
}
/*
void mchA_DrawWaitingText(int color,int Alpha)
{
	int x,y,yy,sx,ch_sx,col;
	
	const int ds = 1;

	mchRacerStats* rp,*rp0,*rp1;

	y = 70;

	if((mchPBEM_Game && !mch_raceD -> online_checkEndGame()) || (!mchPBEM_Game && !(mch_raceD -> activeRacer -> flags & MCH_FINISHED))){
		if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_DATA_SENT) && og_inP.express_game() && !(mch_raceD -> activeRacer -> flags & MCH_FINISHED)){
			y = 20;
		}
	}

	mchA_XBuf.init();

	if(mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER)){
		y = 70;

		sprintf(mchA_String,iGetText(iTXT_CHANGE_PLAYER_T));
		sx = acsStrLen(0,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[0],mchA_FontScaleY[0],mchA_String,mchA_ColorF[color],256 - Alpha/2,0,1,1.0f);
		y += 50;

		yy = y;

		rp0 = mch_raceD -> get_firstRacer(mchPBEM_CurPlayer0);
		rp1 = mch_raceD -> get_firstRacer(mchPBEM_CurPlayer);
		rp = mch_racerLst.first();

		for(ch_sx = 0; ch_sx < og_inP.num_players(); ch_sx ++){
			rp = mch_raceD -> get_firstRacer(mchA_OnlinePlace[ch_sx]);

			if(rp && hsPlayers[rp -> ID].type >= 1){
				col = (rp == rp0 || mchPBEM_CheckFlag(PBEM_REPLAY_MODE)) ? 2 : 1;

				sprintf(mchA_String,rp -> name);
				sx = acsStrLen(1,(unsigned char*)mchA_String,1);
				x = (640 - sx) / 2;

				mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[col],256 - Alpha/2,1,1,1.0f);

				if(rp == rp1){
					mchA_d3dOutSprite(x - 30,y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[col],256 - Alpha/2,0.0f,1.0f);
					mchA_d3dOutSprite(x + sx + 30,y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[col],256 - Alpha/2,M_PI,1.0f);
				}

				y += 30;
			}
		}

		y = yy + 30 * 5;

		sprintf(mchA_String,iGetText(iTXT_USE_ARROWS));
		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
		y += 30;

		sprintf(mchA_String,iGetText(iTXT_CHANGE_CONTINUE),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));
		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);

		return;
	}

	if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE) && (mchGameFlags & MCH_START_TIME_FLAG || mchTimeMode == MCH_TIME_RUNNING)) return;

	if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT) || mchPBEM_CurPlayer != mchPBEM_CurPlayer0){
		mchA_XBuf < mch_raceD -> activeRacer -> name;
	}
	else {
		if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
			mchA_XBuf < iGetText(iTXT_SENT_OK);
		else
			mchA_XBuf < iGetText(iTXT_LATE_TURN);
	}

	sx = acsStrLen(0,(unsigned char*)mchA_XBuf.address(),1);
	x = (640 - sx) / 2;

	mchA_d3dOutString(x,y,mchA_FontScaleX[0],mchA_FontScaleY[0],mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha/2,0,1,1.0f);
	y += 50;

	if(!(mch_raceD -> activeRacer -> flags & MCH_FINISHED)){
		if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT))
			sprintf(mchA_String,iGetText(iTXT_WAIT_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));
		else 
			sprintf(mchA_String,iGetText(iTXT_EDIT_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));

		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		if(!mchPBEM_CheckFlag(PBEM_LATE_TURN) && mchPBEM_CurPlayer == mchPBEM_CurPlayer0)
			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);

		y += 30;

		if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_DATA_SENT) && og_inP.express_game() && !(mch_raceD -> activeRacer -> flags & MCH_FINISHED)){
			yy = y;

			sprintf(mchA_String,iGetText(iTXT_WAITING_FOR));
			sx = acsStrLen(1,(unsigned char*)mchA_String,1);
			x = (640 - sx) / 2;

			if(mchPBEM_CurPlayer == mchPBEM_CurPlayer0){
				if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
					mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);

				y += 30;

				rp = mch_racerLst.first();
				for(ch_sx = 0; ch_sx < 5; ch_sx ++){
					if(hsPlayers[rp -> ID].type && rp && !mchA_TurnFlags[ch_sx] && ch_sx != mchPBEM_CurPlayer){
						sprintf(mchA_String,rp -> name);
						sx = acsStrLen(1,(unsigned char*)mchA_String,1);
						x = (640 - sx) / 2;

						if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
							mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
						y += 30;
					}
					if(rp)
						rp = rp -> next;
				}

				y = yy + 30 * 5;

				sprintf(mchA_String,iGetText(iTXT_REFRESH));
				sx = acsStrLen(1,(unsigned char*)mchA_String,1);
				x = (640 - sx) / 2;

				mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
				y += 30;

				if(ogCheckRefreshTime())
					KeyBuf -> put(VK_SPACE,KBD_CUR_KEY_PRESSED);
			}
		}

		if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT) || mchPBEM_CurPlayer != mchPBEM_CurPlayer0){
			mchA_XBuf.init();
			mchA_XBuf < iGetText(iTXT_TURN) <= mchTurn + 1;
			sx = acsStrLen(1,(unsigned char*)mchA_XBuf.address(),1);
			x = (640 - sx) / 2;
			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_XBuf.address(),mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
		}
	}
	else {
		sprintf(mchA_String,iGetText(iTXT_WAIT_TURN_FINISHED));
		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
		y += 30;
	}

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchTurn){
		y += 50;
		sprintf(mchA_String,iGetText(iTXT_REPLAY_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY,mchGetKeysConfig())));

		sx = acsStrLen(1,(unsigned char*)mchA_String,1);
		x = (640 - sx) / 2;

		if(!mchPBEM_CheckFlag(PBEM_REPLAY_MODE))
			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);

		if(mchTurn > 1 || mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
			y += 30;
			sprintf(mchA_String,iGetText(iTXT_REPLAY_GAME),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY_ALL,mchGetKeysConfig())));

			sx = acsStrLen(1,(unsigned char*)mchA_String,1);
			x = (640 - sx) / 2;

			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
		}

		if(mchPBEM_Game){
			y += 30;
			sprintf(mchA_String,iGetText(iTXT_CHANGE_PLAYER));
			sx = acsStrLen(1,(unsigned char*)mchA_String,1);
			x = (640 - sx) / 2;

			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],mchA_String,mchA_ColorF[color],256 - Alpha/2,1,1,1.0f);
		}
	}
}
*/
void mchA_DrawWaitingText(int color,int Alpha)
{
	int y,yy,ch_sx,col;

	static mchA_TextStr text[20];
	int text_idx = 0;
	
	const int ds = 1;

	mchRacerStats* rp,*rp0,*rp1;

	y = 0;

	mchA_XBuf.init();

	if(mchPBEM_CheckFlag(PBEM_CHANGE_PLAYER)){
		sprintf(mchA_String,iGetText(iTXT_CHANGE_PLAYER_T));
		text[text_idx ++].set(y,0,1,mchA_String);
		y += 50;

		yy = y;

		rp0 = mch_raceD -> get_firstRacer(mchPBEM_CurPlayer0);
		rp1 = mch_raceD -> get_firstRacer(mchPBEM_CurPlayer);
		rp = mch_racerLst.first();

		for(ch_sx = 0; ch_sx < og_inP.num_players(); ch_sx ++){
			rp = mch_raceD -> get_firstRacer(mchA_OnlinePlace[ch_sx]);

			if(rp && hsPlayers[rp -> ID].type >= 1){
				col = (rp == rp0 || mchPBEM_CheckFlag(PBEM_REPLAY_MODE)) ? 2 : 1;

				sprintf(mchA_String,rp -> name);
				text[text_idx ++].set(y,1,1,mchA_String,col,(rp == rp1));
/*
				if(rp == rp1){
					mchA_d3dOutSprite(x - 30,y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[col],256 - Alpha/2,0.0f,1.0f);
					mchA_d3dOutSprite(x + sx + 30,y + 18,0.7f,0.6f,AE_D3DSPR_TRIANGLE,mchA_ColorF[col],256 - Alpha/2,M_PI,1.0f);
				}
*/
				y += 30;
			}
		}

		y = yy + 30 * 5;

		sprintf(mchA_String,iGetText(iTXT_USE_ARROWS));
		text[text_idx ++].set(y,1,1,mchA_String);
		y += 30;

		sprintf(mchA_String,iGetText(iTXT_CHANGE_CONTINUE),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));
		text[text_idx ++].set(y,1,1,mchA_String);

		mchA_DarkenRect(0,0,XGR_MAXX,XGR_MAXY,100);
		mchA_DrawTextStr(text,text_idx,Alpha/2);
		return;
	}

	if(mchPBEM_CheckFlag(PBEM_REPLAY_MODE) && (mchGameFlags & MCH_START_TIME_FLAG || mchTimeMode == MCH_TIME_RUNNING)) return;

	if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT) || mchPBEM_CurPlayer != mchPBEM_CurPlayer0){
		mchA_XBuf < mch_raceD -> activeRacer -> name;
	}
	else {
		if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
			mchA_XBuf < iGetText(iTXT_SENT_OK);
		else
			mchA_XBuf < iGetText(iTXT_LATE_TURN);
	}

	text[text_idx ++].set(y,0,1,mchA_XBuf.address());
	y += 50;

	if(!(mch_raceD -> activeRacer -> flags & MCH_FINISHED)){
		if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT))
			sprintf(mchA_String,iGetText(iTXT_WAIT_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));
		else 
			sprintf(mchA_String,iGetText(iTXT_EDIT_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_START_TIME,mchGetKeysConfig())));

		if(!mchPBEM_CheckFlag(PBEM_LATE_TURN) && mchPBEM_CurPlayer == mchPBEM_CurPlayer0)
			text[text_idx ++].set(y,1,1,mchA_String);

		y += 30;

		if(mchPBEM_Game && mchPBEM_CheckFlag(PBEM_DATA_SENT) && og_inP.express_game() && !(mch_raceD -> activeRacer -> flags & MCH_FINISHED)){
			yy = y;

			sprintf(mchA_String,iGetText(iTXT_WAITING_FOR));

			if(mchPBEM_CurPlayer == mchPBEM_CurPlayer0){
				if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
					text[text_idx ++].set(y,1,1,mchA_String);

				y += 30;

				rp = mch_racerLst.first();
				for(ch_sx = 0; ch_sx < 5; ch_sx ++){
					if(hsPlayers[rp -> ID].type && rp && !mchA_TurnFlags[ch_sx] && ch_sx != mchPBEM_CurPlayer){
						sprintf(mchA_String,rp -> name);

						if(!mchPBEM_CheckFlag(PBEM_LATE_TURN))
							text[text_idx ++].set(y,1,1,mchA_String);

						y += 30;
					}
					if(rp)
						rp = rp -> next;
				}

				y = yy + 30 * 5;

				sprintf(mchA_String,iGetText(iTXT_REFRESH));
				text[text_idx ++].set(y,1,1,mchA_String);

				y += 30;

				if(ogCheckRefreshTime())
					KeyBuf -> put(VK_SPACE,KBD_CUR_KEY_PRESSED);
			}
		}

		if(!mchPBEM_Game || !mchPBEM_CheckFlag(PBEM_DATA_SENT) || mchPBEM_CurPlayer != mchPBEM_CurPlayer0){
			mchA_XBuf.init();
			mchA_XBuf < iGetText(iTXT_TURN) <= mchTurn + 1;
			text[text_idx ++].set(y,1,1,mchA_XBuf.address());
		}
	}
	else {
		sprintf(mchA_String,iGetText(iTXT_WAIT_TURN_FINISHED));
		text[text_idx ++].set(y,1,1,mchA_String);
		y += 30;
	}

	if(mchGameMode == MCH_ENTIRE_CONTROL_HS && mchTurn){
		y += 50;
		sprintf(mchA_String,iGetText(iTXT_REPLAY_TURN),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY,mchGetKeysConfig())));

		if(!mchPBEM_CheckFlag(PBEM_REPLAY_MODE))
			text[text_idx ++].set(y,1,1,mchA_String);

		if(mchTurn > 1 || mchPBEM_CheckFlag(PBEM_REPLAY_MODE)){
			y += 30;
			sprintf(mchA_String,iGetText(iTXT_REPLAY_GAME),mchGetKeyNameText(mchGetKey(MCH_KEY_REPLAY_ALL,mchGetKeysConfig())));
			text[text_idx ++].set(y,1,1,mchA_String);
		}

		if(mchPBEM_Game){
			y += 30;
			sprintf(mchA_String,iGetText(iTXT_CHANGE_PLAYER));
			text[text_idx ++].set(y,1,1,mchA_String);
		}
	}

	mchA_DarkenRect(0,0,XGR_MAXX,XGR_MAXY,100);
	mchA_DrawTextStr(text,text_idx,Alpha/2);
}

void mchA_SetOnlinePlaceOrder(int pl_id,int idx)
{
	mchA_OnlinePlace[idx] = pl_id;
}

int mchA_GetOnlinePlaceOrder(int idx)
{
	return mchA_OnlinePlace[idx];
}

void mchA_ChangeOnlinePlayer(int dir)
{
	int i;

	for(i = 0; i < og_inP.num_players(); i ++){
		if(mchA_OnlinePlace[i] == mchPBEM_CurPlayer) break;
	}

	if(dir){
		if(++i >= og_inP.num_players())
			i = 0;
	}
	else {
		if(--i < 0)
			i = og_inP.num_players() - 1;
	}

	mchPBEM_CurPlayer = mchA_OnlinePlace[i];
}

void mchA_BuildOnlinePlaceOrder(void)
{
	mchRacer* p = mch_raceD -> racerLst -> first();
	while(p){
		mchA_SetOnlinePlaceOrder(p -> stPtr -> ID,p -> stPtr -> place);
		p = p -> next;
	}
}

void mchA_EnableTimeSpeedShow(void)
{
	mchA_TimeSpeedEl -> SetTimer(50);
}

void mchA_DrawTextStr(mchA_TextStr* p,int sz,int alpha)
{
	int i,y0;

	if(!sz) return;

	y0 = (480 - p[sz - 1].Y - 60)/2;

	for(i = 0; i < sz; i ++){
		p[i].Y += y0;
		p[i].draw(alpha);
	}
}

void mchArcaneScreenDispatcher::SetLastEl(int tp)
{
	mchArcaneScreenElement* p = objList -> first();

	while(p){
		if(p -> type == tp){
			objList -> remove(p);
			objList -> append(p);
		}
		p = p -> next;
	}
}

void mchA_DrawTextWindow(int x,int y,int sx,int sy,int wnd_alpha,int border_col,int border_alpha)
{
	int wx,wy,wsx,wsy;

	wx = round(float(x) * mchA_d3dResX);
	wy = round(float(y) * mchA_d3dResY);
	wsx = round(float(x + sx) * mchA_d3dResX);
	wsy = round(float(y + sy) * mchA_d3dResY);

	mchA_DarkenRect(wx,wy,wsx,wsy,wnd_alpha);

	if(border_alpha)
		mchA_d3dRectangleSq(x - 4,y - 4,sx + 8,sy + 8,border_col,border_alpha);
}
