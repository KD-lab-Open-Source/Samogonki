/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "hbm.h"

#include "HFONT.H"
#include "mechosoma.h"
#ifdef _WIN32
#include "win32f.h"
#endif

#include "arcane_menu.h"

#include "ACI_IDS.H"
#include "ACI_EVNT.H"
#include "ACI_SCR.H"
#include "KEYS.H"
#include "sound.h"
#include "sound_api.h"

#include "iText.h"

#include "race.h"

#include "xsound.h"

#include "controls.h"
#include "XJoystick.h"

#include "I-World.h"
#include "GameClient.h"

#include "TrackDispatcher.h"

#include "mch_common.h" // For far target
#include "IGraph3d.h" // For far target

#ifndef _WIN32
#define MK_LBUTTON 0x0001
#endif

/* ----------------------------- STRUCT SECTION ----------------------------- */

const int KEY_CONFIG_MAX = 3;

class mchControlKey
{
	int code[KEY_CONFIG_MAX];
	int default_code[KEY_CONFIG_MAX];
public:

	int ID;
	void* list;
	mchControlKey* next;
	mchControlKey* prev;

	int Code(int cfg = -1) const;
	int defCode(int cfg = -1) const;

	void SetCode(int cd,int cfg = -1,int def_mode = 0);
	void SetDefCode(int cd,int cfg = -1);
	void ResetCode(int cfg = -1);

	mchControlKey(int id,int c);
	~mchControlKey(void);
};

// aciMainMenuOption::type
#define ACI_TRIGGER		0
#define ACI_NUMBER		1
#define ACI_STRING		2
#define ACI_KEY			3

// aciMainMenuOption::flags
#define ACI_LIMITS		0x01
#define ACI_MANUAL_SWITCHING	0x02
#define ACI_MULTIVALUE		0x04

struct aciMainMenuOption
{
	int ID;
	int type;

	int flags;

	int MinValue;
	int MaxValue;
	int ValueDelta;

	int ScreenID;
	int ObjectID;
	aciScreenObject* objPtr;

	int Value;
	int mValue[KEY_CONFIG_MAX];

	char* ValueStr;

	char* iniFile;
	char* iniSection;
	char* iniKey;

	void Load(void);
	void Save(void);

	void SetObjectValue(void);
	void GetObjectValue(void);
	void SetObjectVisible(int v = 1);

	void SetLimits(int min,int max,int delta);

	void SetMaxValue(void){ Value = MaxValue; SetObjectValue(); Update(); }
	void SetMinValue(void){ Value = MinValue; SetObjectValue(); Update(); }

	void incValue(void);
	void decValue(void);

	void Update(int mode = 0);

	void* list;
	aciMainMenuOption* prev;
	aciMainMenuOption* next;

	aciMainMenuOption(int tp,int id,int scr_id,int obj_id,char* ini_file,char* ini_sec,char* ini_key);
	~aciMainMenuOption(void);
};

#define ACI_SCREEN_STACK_SZ		20
struct aciScreenStack
{
	int curIndex;
	int data[ACI_SCREEN_STACK_SZ];

	void Put(int id);
	void Clear(void){ curIndex = 0; data[0] = 0; }
	int Get(void);

	aciScreenStack(void);
	~aciScreenStack(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int mch_iScreen;
extern int mchSkipMainMenu;

extern int mchDetectJoystick;

extern int mchXRecorderMode;
extern char* mchXRecorderName;

extern int mchSplitScreenMode;
extern int mchEnableTips;
extern int iwElectionaryFlag;
extern int iwEnableIntroMovie;
extern int mchTrackSelectFlag;

extern int mchEnableJoystick;
extern int mch_iScreenID;

extern int mchWorldSelectFlag;
extern int mchOptionObjIDs[];

extern int acsLastKey;
extern int acsCancelInputFlag;

extern int mchBotArcaneUsing;
extern int mchNumRacersDef;
extern int mchNumLapsDef;
extern int mchNumLapsDefHS;

extern int mchResourcesFlag;
extern int mchSplitScreenBots;

extern int acsLoadFlag;
extern int acsDisableEvents;
extern int acsShopFlag;
extern int acsHighColor;
extern int acsCurrentWorld;
extern int mchCurrentWorld;
extern int mchCurrentTrack;

extern int mchRacersInitFlag;

extern int acsNumFonts;
extern char* acsFontDir;
extern char* acsFontPrefix;
extern HFont** acsFntTable;

extern int acsCurrentSave;
extern int acsNumSaves;

#ifdef _ACS_MOVE_OBJECTS_
extern aciScreenObject* acsMoveObj;
#endif

extern cInterfaceGraph3d	*gb_IGraph3d;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchA_BuildOnlinePlaceOrder(void);

void acsExtSetCamera(int zbuf_flag = 0);
aciScreenObject* acsGetObject(int scr_id,int obj_id);

void mchInitDemo(void);
void acsInitDemo(int tb_mode);

void acsInitOnlineMenu(void);

void acsInitShop(int pl);
void acsFinitShop(int mode = 0);
void acsShopChangePart(int tp,int dir);
void acsShopMoveMouse(int dx,int dy);

void acsSetTurnBased(int mode,int value);

void mchInitCamera(void);
void acsInitControls(void);

void acsInitSaveScreen(void);
void acsSelectSave(int n);
void acsDeleteSave(int n);
void acsScrollSaveSlots(int dir);

int acsInitSaveSlots(void);

char* mchGetKeyNameText(int code);
void acsInitEssenceScreen(void);
void acsEssenceEvent(int id);

void mchInitRacers(void);
void mchResetRacers(void);

void acsInitWorldObjs(void);

void acsPutInStack(int id);
void acsSetNextScr(int id);
void acsClearStack(void);

void acsInitOptions(void);

void mchSetKey(int id,int code,int cfg = -1);
int mchGetKey(int id);
int mchGetKey(int id,int cfg);

void mchSelectKeysConfig(int cfg_id);
void mchRestoreKeysConfig(void);
int mchGetKeysConfig(void);

void acsPrepare(void);
void acsInit(void);
int acsQuant(void);
void acsFinit(void);

void acsJoyQuant(void);
void acsJoyHandler(eXJoystickState state, int button_or_axis_value, int joystick_ID);

void acsExtInit(void);

void acsInitMouse(void);
void acsSetMouse(void);
void acsMouseLP(int fl,int x,int y);
void acsMouseLU(int fl,int x,int y);
void acsMouseRP(int fl,int x,int y);
void acsMouseRU(int fl,int x,int y);
void acsMouseMove(int fl,int x,int y);

void acsSendEvent(int code,int data0,int data1,int data2,int data3);

void acsLoadScript(char* fname);

int acsGetKeyName(int vkey,int shift,int lng_flag);
void ibsout(int x,int y,void* ptr);

void acsOutStr(int x,int y,int fnt,int col,unsigned char* str,int space);
void acsOutStr16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space);
void acsOutStr16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutStr16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutStr16_a2_clip(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutText16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space);
void acsOutText16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutText16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutStr16buf(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int bsx,void* buf,int space,void* alpha_buf = NULL,int alpha0 = 0);
int acsStrLen(int fnt,unsigned char* str,int space);
int acsTextStrLen(int fnt,unsigned char* str,int space);
int acsTextStrLenMax(int fnt,unsigned char* str,int space);
int acsStrHeight(int fnt,unsigned char* str);
int acsTextHeight(int fnt,unsigned char* str,int space);

void acsPutSpr(int x,int y,int sx,int sy,void* buf,int col,int col_sz);
void acsPutSpr16sz(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal);
void acsPutSpr16sz_a(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha);
void acsPutSpr16sz_a2(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha);
void acsPutSpr16sz_a2_clip(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha);
void acsPutSpr16sz_buf(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int bsx,void* scr_buf,void* alpha_buf = NULL,int alpha0 = 0);
void acsPutSpr16sz_buf_clip(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int bsx,int bsy,void* scr_buf,void* alpha_buf,int alpha0);

void acsChangeStrState(int id);

void acsLoadFonts(void);
void acsFreeFonts(void);

void acsHandleExtEvent(int code,int data0 = 0,int data1 = 0,int data2 = 0);

void acsPrepareSprite(int sx,int sy,void* dst_buf,void* src_buf,void* pal);

void mchFreeResources(int mode = 0);

void acsInitMM_Options(void);
void acsSetMM_Option(int scr,int obj,int val);
void acsUpdateMM_Option(int scr,int obj);
void acsIncMM_Option(int scr,int obj);
void acsDecMM_Option(int scr,int obj);

void mchInitWorldsPrm(void);

void acsResetControls(int mode = 0);
void acsBackupControls(void);
void acsRestoreControls(void* buf = NULL);
void* acsGetControlsBackupBuf(int& sz);

void acsInitFont3D(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define ACS_NUM_SAVES		7
#define ACS_MAX_CD_TRACK	21

hbm_Image* acsMouseData;
aciScreenStack* acsStack;

xtList<aciMainMenuOption>* acsMM_Options = NULL;
xtList<mchControlKey>* mchControls = NULL;

int acsNextScreenID = -1;
int acsGameMode = 0;

int acsCurSave = 0;

int acsCurNumTracks = 9;

int acsMaxCDTrack = 0;
int acsCurCDTrack = 0;

int acsMouseLPFlag = 0;
int acsMouseRPFlag = 0;

int acsActiveFlag = 0;
int iWorldMode = 0;

int acsCurKeyConfig = 0;
int acsCurKeyConfigPrev = 0;

int acsPrepareFlag = 0;

mchControlKey* __acsKey;
char* mchControlsBackupBuf = NULL;

char acsJoyUse[3] = { 1, 1, 0};

unsigned iRndVal = 83838383;

void acsPrepare(void)
{
	acsPrepareFlag = 1;

	XStream fh;

	iRndVal = clock();

	KBD_init();
	acsInitMouse();

	acsLoadScript("iscreen.scb");

	acsLoadFonts();

	acsScrD -> ChangeCoords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);

	acsStack = new aciScreenStack;

	acsInitOptions();

	mchInitWorldsPrm();

	acsInitSaveSlots();
	if(acsNumSaves == 1)
		mch_iScreenID = ACS_MAIN_MENU_SCR1;
	if(acsNumSaves > 1)
		mch_iScreenID = ACS_MAIN_MENU_SCR2;

	acsPrepareFlag = 0;
}

int acsGetKeyName(int vkey,int shift,int lng_flag)
{
	if(vkey > 0 && vkey < 256)
		return xtGetKeyName(vkey,shift,lng_flag);

	return -1;
}

void acsOutStr(int x,int y,int fnt,int col,unsigned char* str,int space)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutStr16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutStr16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_a(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}


void acsOutStr16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_a2(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutStr16_a2_clip(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_a2_clip(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutText16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz,ssx;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	ssx = acsTextStrLenMax(fnt,str,space);

	_x = x;
	_y = y;

	_x += (ssx - acsTextStrLen(fnt,str,space))/2;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			if(i < sz - 1)
				_x += (ssx - acsTextStrLen(fnt,str + i + 1,space))/2;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutText16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz,ssx;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	ssx = acsTextStrLenMax(fnt,str,space);

	_x = x;
	_y = y;

	_x += (ssx - acsTextStrLen(fnt,str,space))/2;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			if(i < sz - 1)
				_x += (ssx - acsTextStrLen(fnt,str + i + 1,space))/2;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_a(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutText16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz,ssx;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	ssx = acsTextStrLenMax(fnt,str,space);

	_x = x;
	_y = y;

	_x += (ssx - acsTextStrLen(fnt,str,space))/2;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			if(i < sz - 1)
				_x += (ssx - acsTextStrLen(fnt,str + i + 1,space))/2;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_a2(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

void acsOutStr16buf(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int bsx,void* buf,int space,void* alpha_buf,int alpha0)
{
	int _x,_y,i,sx,sy,ss,sz = strlen((char*)str),col_sz;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr;

	col_sz = (col >> 8) & 0xFF;
	col &= 0xFF;

	_x = x;
	_y = y;
	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str[i] == '\n'){
			_x = x;
			_y += sy + space;
		}
		else {
			acsPutSpr16sz_buf(_x - ch -> LeftOffs,_y,sx,sy,ptr,col,col_sz,pal,bsx,buf,alpha_buf,alpha0);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
}

int acsStrHeight(int fnt,unsigned char* str)
{
	return acsFntTable[fnt] -> SizeY;
}

int acsStrLen(int fnt,unsigned char* str,int space)
{
	int i,sx,sz = strlen((char*)str),len = 0;
	HFont* p = acsFntTable[fnt];
	HChar* ch;

	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		sx = ch -> SizeX;

		if(str[i] != '\n'){
			len += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
	return len;
}

int acsTextStrLen(int fnt,unsigned char* str,int space)
{
	int i,sx,sz = strlen((char*)str),len = 0;
	HFont* p = acsFntTable[fnt];
	HChar* ch;

	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		sx = ch -> SizeX;

		if(str[i] == '\n') break;
		len += sx - ch -> RightOffs - ch -> LeftOffs + space;
	}
	return len;
}

int acsTextStrLenMax(int fnt,unsigned char* str,int space)
{
	int i,sx,sz = strlen((char*)str),len = 0,max_len = 0;
	HFont* p = acsFntTable[fnt];
	HChar* ch;

	for(i = 0; i < sz; i ++){
		ch = p -> data[str[i]];
		sx = ch -> SizeX;

		if(str[i] == '\n'){
			if(len > max_len) max_len = len;
			len = 0;
		}
		else {
			if(str[i] != '\r')
				len += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}
	if(len > max_len) max_len = len;
	return max_len;
}

int acsTextHeight(int fnt,unsigned char* str,int space)
{
	int i,sz = strlen((char*)str),len;
	HFont* p = acsFntTable[fnt];
	
	len = p -> SizeY;
	for(i = 0; i < sz; i ++){
		if(str[i] == '\n')
			len += p -> SizeY + space;
	}
	return len;
}

int acsExtEventActive(aciScreenEventCommand* p)
{
	return 1;
}

void acsHandleExtEvent(int code,int data0,int data1,int data2)
{
	int v;
	mchTrackInfo* p;

	switch(code){
		case ACS_EXT_CHANGE_PLAYER:
			mchGameFlags |= MCH_STOP_TIME_FLAG | MCH_TOGGLE_PAUSE_FLAG;
			mchPBEM_SetFlag(PBEM_CHANGE_PLAYER);
			mchA_BuildOnlinePlaceOrder();
			break;
		case ACS_EXT_INIT_ONLINE_MENU:
			acsInitOnlineMenu();
			break;
		case ACS_EXT_SET_ZBUF:
			acsExtSetCamera(data0);
			break;
		case ACS_EXT_SET_TURNBASED:
			acsSetTurnBased(data0,data1);
			break;
		case ACS_EXT_PLAY_DEMO:
			mchInitDemo();
//			acsInitDemo(data0);
			break;
		case ACS_EXT_CHANGE_PART:
			acsShopChangePart(data0,data1);
			break;
		case ACS_EXT_INIT_SHOP:
			acsInitShop(data0);
			break;
		case ACS_EXT_FINIT_SHOP:
			acsFinitShop(data0);
			break;
		case ACS_EXT_CHANGE_HS_WORLD:
			if(data0)
				p = mch_trackD -> NextTrack(hsWorld,hsTrack);
			else
				p = mch_trackD -> PrevTrack(hsWorld,hsTrack);

			if(p){
				mchCurrentWorld = hsWorld = p -> World();
				mchCurrentTrack = hsTrack = p -> Track();

				putIniKey(mch_hotseatINI,"game parameters","world",hsWorld);
				putIniKey(mch_hotseatINI,"game parameters","track",hsTrack);
			}
			break;
		case ACS_EXT_SELECT_CONTROLLER:
			acsCurKeyConfig = data0;
			acsInitControls();
			break;
		case ACS_EXT_QUIT:
			acsInitSaveSlots();
			if(acsNumSaves < 1)
				acsScrD -> PrepareChangeScreen(ACS_MAIN_MENU_SCR0);
			if(acsNumSaves == 1)
				acsScrD -> PrepareChangeScreen(ACS_MAIN_MENU_SCR1);
			if(acsNumSaves > 1)
				acsScrD -> PrepareChangeScreen(ACS_MAIN_MENU_SCR2);
			break;
		case ACS_EXT_SCROLL_SAVES:
			acsScrollSaveSlots(data0);
			break;
		case ACS_EXT_SELECT_SAVE:
			acsSelectSave(data0);
			break;
		case ACS_EXT_DELETE_SAVE:
			acsDeleteSave(data0);
			break;
		case ACS_EXT_INIT_SAVE_SCR:
			acsInitSaveScreen();
			break;
		case ACS_EXT_ELECTIONARY_MODE:
			iwElectionaryFlag = 1;
			break;
		case ACS_EXT_DONE_INPUT:
			if(acsScrD -> activeInput)
				acsScrD -> DoneInput();
			break;
		case ACS_EXT_RESET_CONTROLS:
			acsResetControls(data0);
			break;
		case ACS_EXT_INIT_ESSENCES:
			acsInitEssenceScreen();
			break;
		case ACS_EXT_ESSENCE_EVENT:
			acsEssenceEvent(data0);
			break;
		case ACS_EXT_TURN_BASED_MODE:
			mchTurnBasedGame = data0;
			break;
		case ACS_EXT_INIT_RACERS:
			mchInitRacers();
			break;
		case ACS_EXT_SAVE_GAME:
			acsCurSave = data1;
			break;
		case ACS_EXT_LOAD_GAME:
			acsLoadFlag = 1;
			break;
		case ACS_EXT_CHOOSE_SLOT:
			acsCurSave = data0;
			break;
		case ACS_EXT_INIT_MM_OPTIONS:
			acsInitMM_Options();
			break;
		case ACS_EXT_INC_MM_OPTION:
			acsIncMM_Option(data0,data1);
			break;
		case ACS_EXT_UPDATE_MM_OPTION:
			acsUpdateMM_Option(data0,data1);
			break;
		case ACS_EXT_DEC_MM_OPTION:
			acsDecMM_Option(data0,data1);
			break;
		case ACS_EXT_SET_NUM_TRACKS:
			acsCurNumTracks = data0;
			break;
		case ACS_EXT_INIT_WORLDS:
			mchInitRacers();
			break;
		case ACS_EXT_ABORT_RACE:
			mchResetRacers();
			mchFreeResources(1);
			acsStack -> Clear();
			iworld_D -> PutPhrase(IW_DOGGY_ABORT_RACE);
			break;
		case ACS_EXT_CLEAR_SCREEN:
			if(RenderMode == XGRAPH_HICOLOR){
				gb_IGraph3d->Fill(0,0,0);
				acsScrD -> flags |= ACS_FULLSCR_FLUSH;
			}
			break;
		case ACS_EXT_SAVE_OPTIONS:
			for(v = 0; v < ACS_NUM_OPTIONS; v ++){
				mchOptions[v].Save();
			}
			break;
		case ACS_EXT_SET_OPTION:
			if(data0 >= 0 && data0 < ACS_NUM_OPTIONS){
				mchOptions[data0].SetObjectValue();
			}
			break;
		case ACS_EXT_GET_OPTION:
			if(data0 >= 0 && data0 < ACS_NUM_OPTIONS){
				mchOptions[data0].GetObjectValue();
			}
			break;
		case ACS_EXT_RESTORE_OPTION:
			if(data0 >= 0 && data0 < ACS_NUM_OPTIONS){
				mchOptions[data0].Reset();
				mchOptions[data0].SetObjectValue();
				mchOptions[data0].Update();
			}
			break;
		case ACS_EXT_UPDATE_OPTION:
			if(data0 >= 0 && data0 < ACS_NUM_OPTIONS){
				mchOptions[data0].Update();
			}
			break;
		case ACS_EXT_SET_GAME_MODE:
			acsGameMode = data0;
			mchGameMode = data0 - 1;

			if(data1 != -1)
				mchSplitScreenGame = data1;

			if(mchGameMode != MCH_SPLIT_CONTROL_HS && mchGameMode != MCH_ENTIRE_CONTROL_HS){
				acsSetTurnBased(0,0);
				mchTurnBasedGame = 0;
			}
			else {
				mchTurnBasedGame = 1;
				mchCurrentWorld = hsWorld;
				mchCurrentTrack = hsTrack;
			}

			mchInitCamera();
			break;
		case ACS_EXT_NEW_GAME:
			acsScrD -> QuantCode = acsGameMode;
			acsScrD -> flags |= ACS_NEED_EXIT;
			break;
		case ACS_EXT_SET_NEXT_SCREEN:
			acsNextScreenID = data0;
			break;
		case ACS_EXT_NEXT_SCREEN:
			acsStack -> Get();
			acsScrD -> PrepareChangeScreen(acsNextScreenID);
			break;
		case ACS_EXT_SAVE_SCREEN:
			if(data0)
				acsStack -> Put(data0);
			else
				acsStack -> Put(acsScrD -> curScr -> ID);
			break;
		case ACS_EXT_RESTORE_SCREEN:
			acsScrD -> PrepareChangeScreen(acsStack -> Get());
			break;
		case ACS_EXT_FREE_RESOURCES:
			mchFreeResources();
			break;
		case ACS_EXT_CHOOSE_WORLD:
//			if(!mchWorldSelectFlag)
//				mchCurrentWorld = data0;
			break;
		case ACS_EXT_CHOOSE_TRACK:
			if(!mchTrackSelectFlag)
				mchCurrentTrack = data0;
			break;
		case ACS_EXT_START_SOUND:
			mchStartSoundEFF(data0);
			break;
		case ACS_EXT_STOP_SOUND:
			mchStopSoundEFF(data0);
			break;
		case ACS_EXT_RND_SOUND2:
			if(XRnd(2))
				mchStartSoundEFF(data0);
			else
				mchStartSoundEFF(data1);
			break;
		case ACS_EXT_RND_SOUND3:
			v = XRnd(3);
			if(v == 2)
				mchStartSoundEFF(data0);
			else {
				if(v == 1)
					mchStartSoundEFF(data1);
				else
					mchStartSoundEFF(data2);
			}
			break;
		case ACS_EXT_SET_NEXT_SCR_ID:
			acsScrD -> SetNextScrID(data0,data1);
			break;
		case ACS_EXT_SET_PREV_SCR_ID:
			acsScrD -> SetPrevScrID(data0,data1);
			break;
		case ACS_EXT_SELECT_WORLD:
			mchCurrentWorld = data0;
			mchCurrentTrack = data1;
			break;
		case ACS_EXT_KEYTRAP:
			KeyBuf -> put(data0,KBD_CUR_KEY_PRESSED);
			break;
	}
}

void acsPutSpr(int x,int y,int sx,int sy,void* buf,int col,int col_sz)
{
	int i,j,index = 0;
	unsigned cl;
	unsigned char* p = (unsigned char*)buf;

	if(col_sz){
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				cl >>= col_sz;
				if(cl)
					XGR_SetPixelFast(x + j,y + i,col + cl);
				index ++;
			}
		}
	}
	else {
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				if(cl)
					XGR_SetPixelFast(x + j,y + i,col + cl);
				index ++;
			}
		}
	}
}

void acsPutSpr16sz(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal)
{
	int i,j,index = 0;
	unsigned cl;
	unsigned char* p = (unsigned char*)buf;

	if(col_sz){
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				cl >>= col_sz;
				if(cl && (x + j) >= 0 && (x + j) < XGR_MAXX && (y + i) >= 0 && (y + i) < XGR_MAXY)
					XGR_SetPixel16Fast(x + j,y + i,pal[col + cl]);
				index ++;
			}
		}
	}
	else {
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				if(cl && (x + j) >= 0 && (x + j) < XGR_MAXX && (y + i) >= 0 && (y + i) < XGR_MAXY)
					XGR_SetPixel16Fast(x + j,y + i,pal[col + cl]);
				index ++;
			}
		}
	}
}

void acsPutSpr16sz_a(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha)
{
	int i,j,index = 0,r,g,b;
	unsigned cl,cl0,cl1;
	int alpha1 = 256 - alpha;
	unsigned char* p = (unsigned char*)buf;
	unsigned short* scrBuf;

	if(col_sz){
		for(i = 0; i < sy; i ++){
			scrBuf = (unsigned short*)XGR_GetVideoLine(y + i) + x;
			for(j = 0; j < sx; j ++){
				cl = p[index];
				cl >>= col_sz;
				if(cl){
					cl0 = scrBuf[j];
					cl1 = pal[col + cl];
					r = (((cl0 & XGR_MASK_R) * alpha + (cl1 & XGR_MASK_R) * alpha1)) & (XGR_MASK_R << 8);
					g = (((cl0 & XGR_MASK_G) * alpha + (cl1 & XGR_MASK_G) * alpha1)) & (XGR_MASK_G << 8);
					b = (((cl0 & XGR_MASK_B) * alpha + (cl1 & XGR_MASK_B) * alpha1)) & (XGR_MASK_B << 8);
					scrBuf[j] = (r + g + b) >> 8;
				}
				index ++;
			}
		}
	}
	else {
		for(i = 0; i < sy; i ++){
			scrBuf = (unsigned short*)XGR_GetVideoLine(y + i) + x;
			for(j = 0; j < sx; j ++){
				cl = p[index];
				if(cl){
					cl0 = scrBuf[j];
					cl1 = pal[col + cl];
					r = (((cl0 & XGR_MASK_R) * alpha + (cl1 & XGR_MASK_R) * alpha1)) & (XGR_MASK_R << 8);
					g = (((cl0 & XGR_MASK_G) * alpha + (cl1 & XGR_MASK_G) * alpha1)) & (XGR_MASK_G << 8);
					b = (((cl0 & XGR_MASK_B) * alpha + (cl1 & XGR_MASK_B) * alpha1)) & (XGR_MASK_B << 8);
					scrBuf[j] = (r + g + b) >> 8;
				}
				index ++;
			}
		}
	}
}

void acsPutSpr16sz_a2(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha)
{
	int i,j,index = 0,r,g,b,al;
	unsigned cl,cl0,cl1;
	alpha = 256 - alpha;
	unsigned char* p = (unsigned char*)buf;
	unsigned short* scrBuf;

	for(i = 0; i < sy; i ++){
		scrBuf = (unsigned short*)XGR_GetVideoLine(y + i) + x;
		for(j = 0; j < sx; j ++){
			cl = p[index];
			cl >>= col_sz;
			if(cl){
				cl0 = scrBuf[j];
				cl1 = pal[col + cl];

				al = ((cl << 3) * alpha) >> 8;

				r = (((cl0 & XGR_MASK_R) * (256 - al) + (cl1 & XGR_MASK_R) * al)) & (XGR_MASK_R << 8);
				g = (((cl0 & XGR_MASK_G) * (256 - al) + (cl1 & XGR_MASK_G) * al)) & (XGR_MASK_G << 8);
				b = (((cl0 & XGR_MASK_B) * (256 - al) + (cl1 & XGR_MASK_B) * al)) & (XGR_MASK_B << 8);
				scrBuf[j] = (r + g + b) >> 8;
			}
			index ++;
		}
	}
}

void acsPutSpr16sz_a2_clip(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int alpha)
{
	int i,j,index = 0,r,g,b,al,_x,_x1,_y,_y1,_sx,_sy;
	unsigned cl,cl0,cl1;
	alpha = 256 - alpha;
	unsigned char* p = (unsigned char*)buf;
	unsigned short* scrBuf;

	_x = (x > XGR_Obj.clipLeft) ? x : XGR_Obj.clipLeft;
	_y = (y > XGR_Obj.clipTop) ? y : XGR_Obj.clipTop;

	_x1 = (x + sx <= XGR_Obj.clipRight) ? (x + sx) : XGR_Obj.clipRight;
	_y1 = (y + sy <= XGR_Obj.clipBottom) ? (y + sy) : XGR_Obj.clipBottom;

	_sx = _x1 - _x;
	_sy = _y1 - _y;

	index = sx * (_y - y) + _x - x;

	for(i = 0; i < _sy; i ++){
		scrBuf = (unsigned short*)XGR_GetVideoLine(_y + i) + _x;
		for(j = 0; j < _sx; j ++){
			cl = p[index + j];
			cl >>= col_sz;
			if(cl){
				cl0 = scrBuf[j];
				cl1 = pal[col + cl];

				al = ((cl << 3) * alpha) >> 8;

				r = (((cl0 & XGR_MASK_R) * (256 - al) + (cl1 & XGR_MASK_R) * al)) & (XGR_MASK_R << 8);
				g = (((cl0 & XGR_MASK_G) * (256 - al) + (cl1 & XGR_MASK_G) * al)) & (XGR_MASK_G << 8);
				b = (((cl0 & XGR_MASK_B) * (256 - al) + (cl1 & XGR_MASK_B) * al)) & (XGR_MASK_B << 8);
				scrBuf[j] = (r + g + b) >> 8;
			}
		}
		index += sx;
	}
}

void acsPutSpr16sz_buf(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int bsx,void* scr_buf,void* alpha_buf,int alpha0)
{
	int i,j,index = 0;
	unsigned cl,cl1;
	unsigned char* p = (unsigned char*)buf;
	unsigned short* scr = (unsigned short*)scr_buf;
	unsigned char* al = (unsigned char*)alpha_buf;

	alpha0 = 256 - alpha0;

	if(col_sz){
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				cl1 = cl >> col_sz;
				if(cl1){
					scr[(y + i) * bsx + x + j] = pal[col + cl1];
					if(al) al[(y + i) * bsx + x + j] = ((cl << 3) * alpha0) >> 8;
				}
				index ++;
			}
		}
	}
	else {
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				if(cl){
					scr[(y + i) * bsx + x + j] = pal[col + cl];
					if(al) al[(y + i) * bsx + x + j] = ((cl << 3) * alpha0) >> 8;
				}
				index ++;
			}
		}
	}
}

void acsPutSpr16sz_buf_clip(int x,int y,int sx,int sy,void* buf,int col,int col_sz,unsigned *pal,int bsx,int bsy,void* scr_buf,void* alpha_buf,int alpha0)
{
	int i,j,index = 0;
	unsigned cl,cl1;
	unsigned char* p = (unsigned char*)buf;
	unsigned short* scr = (unsigned short*)scr_buf;
	unsigned char* al = (unsigned char*)alpha_buf;

	alpha0 = 256 - alpha0;

	if(col_sz){
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				cl1 = cl >> col_sz;
				if(cl1 && x + j >= 0 && x + j < bsx && y + i >= 0 && y + i < bsy){
					scr[(y + i) * bsx + x + j] = pal[col + cl1];
					if(al) al[(y + i) * bsx + x + j] = ((cl << 3) * alpha0) >> 8;
				}
				index ++;
			}
		}
	}
	else {
		for(i = 0; i < sy; i ++){
			for(j = 0; j < sx; j ++){
				cl = p[index];
				if(cl && x + j >= 0 && x + j < bsx && y + i >= 0 && y + i < bsy){
					scr[(y + i) * bsx + x + j] = pal[col + cl];
					if(al) al[(y + i) * bsx + x + j] = ((cl << 3) * alpha0) >> 8;
				}
				index ++;
			}
		}
	}
}

void ibsout(int x,int y,void* ptr)
{
	int _x,_y,cnt;
	int* offs_table = XGR_Obj.yOffsTable;

	unsigned char* vbuf = XGR_VIDEOBUF;
	unsigned char* pbuf = (unsigned char*)ptr;

	cnt = *((int*)pbuf);
	pbuf += 4;
	while(cnt){
		_x = *((int*)pbuf);
		pbuf += 4;
		_y = *((int*)pbuf);
		pbuf += 4;

		memcpy(vbuf + offs_table[_y + y] + _x + x,pbuf,cnt);
		pbuf += cnt;

		cnt = *((int*)pbuf);
		pbuf += 4;
	}
}

void acsInit(void)
{
	acsScrD -> ChangeCoords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);

	acsScrD -> QuantCode = 0;
	acsDisableEvents = 0;
	acsSetMouse();
	acsScrD -> alloc_mem();
	acsScrD -> flags |= ACS_FULL_REDRAW;
	acsExtInit();
	XGR_MouseShow();

	acsSendEvent(ACS_EXT_EVENT,ACS_EXT_CLEAR_SCREEN,0,0,0);
	mchStartSoundEFF(EFF_INTERFACE_AMBIENT);

	acsActiveFlag = 1;

	if(!mchMusicMute && !mchResourcesFlag && !mchSkipMainMenu) mchSetMusicMode(MCH_CD_SHOW);
}

int acsQuant(void)
{
	int ret,k;

	acsJoyQuant();

	while(KeyBuf -> size){
		k = KeyBuf -> get();
		acsScrD -> KeyTrap(k);
	}
	//mchA_d3dPrepare();
	gb_IGraph3d->Fill(0,0,0);
	//resource_dispatcher.quant();
	gb_IGraph3d->BeginScene();

	ret = acsScrD -> Quant();
	acsScrD -> RedrawQuant();
		gb_IGraph3d->EndScene();
	gb_IGraph3d->Flush();
	//gb_IVisGeneric->PostDraw(0x7FFFFFFF);
	return ret;
}

void acsFinit(void)
{
	mchStopSoundEFF(EFF_INTERFACE_AMBIENT);
	acsScrD -> free_mem();

	if(iWorldMode && iworld_D && iworld_D -> CurScreen()){
		acsScrD -> curScr = iworld_D -> CurScreen();
		acsScrD -> curScrID = iworld_D -> CurScreen() -> ID;
	}

	acsActiveFlag = 0;
	acsCurKeyConfig = 0;
}

void acsInitMouse(void)
{
/*
	XStream fh;
	short sx,sy;

	if(RenderMode == XGRAPH_HICOLOR){
		acsMouseData = new hbm_Image;

		acsOpenResource("mouse.hbm",fh);

		acsMouseData -> load(fh,1);
		XGR_MouseInit(XGR_MAXX/2,XGR_MAXY/2,acsMouseData -> SizeX,acsMouseData -> SizeY,1,acsMouseData -> scr_frameTable[0] -> dataPtr);

		acsOpenResource("mouse_a.bmp",fh);

		fh > sx > sy;
		XGR_MouseObj.AlphaData = new unsigned char[sx * sy];
		fh.read(XGR_MouseObj.AlphaData,sx * sy);
		fh.close();
	}
	else 
		XGR_MouseInit(XGR_MAXX/2,XGR_MAXY/2,0,0,1,NULL);
*/
	XGR_MouseInit(XGR_MAXX/2,XGR_MAXY/2,0,0,1,NULL);
	acsSetMouse();
}

void acsSetMouse(void)
{
	XGR_MouseSetPressHandler(XGM_LEFT_BUTTON,acsMouseLP);
	XGR_MouseSetUnPressHandler(XGM_LEFT_BUTTON,acsMouseLU);

	XGR_MouseSetPressHandler(XGM_RIGHT_BUTTON,acsMouseRP);
	XGR_MouseSetUnPressHandler(XGM_RIGHT_BUTTON,acsMouseRU);

	XGR_MouseSetMoveHandler(acsMouseMove);
}

void acsMouseLP(int fl,int x,int y)
{
	KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_PRESSED);
	acsMouseLPFlag = 1;
}

void acsMouseLU(int fl,int x,int y)
{
//	KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_UNPRESSED);
	acsMouseLPFlag = 0;
}

void acsMouseRP(int fl,int x,int y)
{
#ifndef _ACS_MOVE_OBJECTS_
	KeyBuf -> put(VK_MOUSE_RBUTTON,KBD_CUR_KEY_PRESSED);
	acsMouseRPFlag = 1;
#else
	if(acsMoveObj) acsMoveObj -> flags |= ACS_REDRAW_OBJECT;
	acsMoveObj = acsScrD -> GetMouseObject();
	if(acsMoveObj) acsMoveObj -> flags |= ACS_REDRAW_OBJECT;
#endif
}

void acsMouseRU(int fl,int x,int y)
{
//	KeyBuf -> put(VK_MOUSE_RBUTTON,KBD_CUR_KEY_UNPRESSED);
	acsMouseRPFlag = 0;
}

void acsMouseMove(int fl,int x,int y)
{
	if(acsShopFlag && fl & MK_LBUTTON)
		acsShopMoveMouse(XGR_MouseObj.MovementX,XGR_MouseObj.MovementY);
}

void acsLoadFonts(void)
{
	int i;
	XBuffer XBuf;
	if(acsFntTable) acsFreeFonts();
	if(!acsNumFonts) return;

	acsFntTable = new HFont*[acsNumFonts];
	for(i = 0; i < acsNumFonts; i ++){
		XBuf.init();
		XBuf < acsFontDir < "\\" < acsFontPrefix;
		if(i < 10) XBuf < "0";
		XBuf <= i < ".fnc";
		acsFntTable[i] = new HFont;
		acsFntTable[i] -> Flags |= FONT_COLORS;
		acsFntTable[i] -> load(XBuf);

		acsFntTable[i] -> data[32] -> SizeX = acsFntTable[i] -> data[95] -> SizeX;
		acsFntTable[i] -> data[32] -> LeftOffs = acsFntTable[i] -> data[95] -> LeftOffs;
		acsFntTable[i] -> data[32] -> RightOffs = acsFntTable[i] -> data[95] -> RightOffs;
	}
	acsInitFont3D();
}

void acsFreeFonts(void)
{
	int i;
	if(!acsFntTable) return;
	for(i = 0; i < acsNumFonts; i ++)
		delete acsFntTable[i];
	delete acsFntTable;
	acsFntTable = NULL;
}


void acsPrepareSprite(int sx,int sy,void* dst_buf,void* src_buf,void* pal)
{
	int i,j,index = 0;
	unsigned char* src = (unsigned char*)src_buf;
	unsigned short* dst = (unsigned short*)dst_buf;
	unsigned*  data = (unsigned*)pal;

	for(i = 0; i < sy; i ++){
		for(j = 0; j < sx; j ++){
			dst[index] = data[src[index]];
			index ++;
		}
	}
}

void acsPutSpr16(int x,int y,int sx,int sy,void* p,void* pal,int mode)
{
	int i,j,_x,_y,_x2,_x1,_y1,_sx,_sy,_sx2,sx2,dx = 0,dy = 0,yoffs;

	unsigned short* s_scrBuf;
	unsigned char* c_pBuf;
	unsigned* pal_data = (unsigned*)pal;

	if(XGR_Obj.ClipMode == XGR_CLIP_ALL) mode |= XGR_CLIPPED;

	if(mode & XGR_CLIPPED && !(mode & XGR_NOCLIP)){
		_x = (x > XGR_Obj.clipLeft) ? x : XGR_Obj.clipLeft;
		_y = (y > XGR_Obj.clipTop) ? y : XGR_Obj.clipTop;

		dx = _x - x;
		dy = _y - y;

		_x1 = (x + sx < XGR_Obj.clipRight) ? (x + sx) : XGR_Obj.clipRight;
		_y1 = (y + sy < XGR_Obj.clipBottom) ? (y + sy) : XGR_Obj.clipBottom;

		_sx = _x1 - _x;
		_sy = _y1 - _y;
	}
	else {
		_x = x;
		_y = y;
		_sx = sx;
		_sy = sy;
	}
	if(_sx <= 0 || _sy <= 0) return;

	_x2 = _x << 1;
	_sx2 = _sx << 1;
	sx2 = sx << 1;

	if(mode & XGR_BLACK_FON){
		yoffs = XGR_Obj.yStrOffs >> 1;
		s_scrBuf = (unsigned short*)(XGR_Obj.ScreenBuf + XGR_Obj.yOffsTable[_y]) + _x;
		c_pBuf = ((unsigned char*)p) + dx + dy * sx;
		for(i = 0; i < _sy; i ++){
			for(j = 0; j < _sx; j ++){
				s_scrBuf[j] = pal_data[c_pBuf[j]];
			}
			s_scrBuf += yoffs;
			c_pBuf += sx;
		}
	}
	else {
		yoffs = XGR_Obj.yStrOffs >> 1;
		s_scrBuf = (unsigned short*)(XGR_Obj.ScreenBuf + XGR_Obj.yOffsTable[_y]) + _x;
		c_pBuf = ((unsigned char*)p) + dx + dy * sx;
		for(i = 0; i < _sy; i ++){
			for(j = 0; j < _sx; j ++){
				if(c_pBuf[j]){
					s_scrBuf[j] = pal_data[c_pBuf[j]];
				}
			}
			s_scrBuf += yoffs;
			c_pBuf += sx;
		}
	}
}

aciScreenStack::aciScreenStack(void)
{
	memset((char*)data,0,ACI_SCREEN_STACK_SZ * sizeof(int));
	curIndex = 0;
}

aciScreenStack::~aciScreenStack(void)
{
}

void aciScreenStack::Put(int id)
{
	data[curIndex] = id;
	if(++curIndex >= ACI_SCREEN_STACK_SZ)
		ErrH.Abort("aciScreenStack overflow...");
}

int aciScreenStack::Get(void)
{
	if(curIndex)
		return data[--curIndex];
	else
		return -1;
}

void acsInitOptions(void)
{
	int i;
	XBuffer XBuf;
	aciScreenObject* p;

	acsMaxCDTrack = sndMusicNumTracks();

	aciMainMenuOption* mp;
	acsMM_Options = new xtList<aciMainMenuOption>;

	for(i = 0; i < ACS_MAX_CD_TRACK - 1; i ++){
		XBuf < "track" <= i;
		mp = new aciMainMenuOption(ACI_TRIGGER,ACS_CD_TRACK00_STATUS + i,ACS_CD_SCR,ACS_CD_TRACK00_STATUS + i,mch_optionsINI,"soundtrack",XBuf);
		mp -> SetLimits(0,1,1);
		acsMM_Options -> append(mp);
		XBuf.init();
	}

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_HORN,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_HORN,mch_optionsINI,"controls","horn");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_HORN,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_MAP,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_MAP,mch_optionsINI,"controls","map");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_MAP,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_OK,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_OK,mch_optionsINI,"controls","ok");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_OK,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER_NAME_PRM,ACS_ESSENCE_SCR,ACS_PLAYER_NAME_PRM,NULL,"player1","name");
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_NUMBER,ACS_SEEDS_MAX_PRM,ACS_HOTSEAT_CFG_SCR,ACS_SEEDS_MAX_PRM,mch_hotseatINI,"game parameters","seeds_max");
	mp -> SetLimits(5,10,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_NUMBER,ACS_TURN_TIME_PRM,ACS_HOTSEAT_CFG_SCR,ACS_TURN_TIME_PRM,mch_hotseatINI,"game parameters","turn_time");
	mp -> SetLimits(0,180,30);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_NUMBER,ACS_WAIT_TURN_TIME_PRM,ACS_HOTSEAT_CFG_SCR,ACS_WAIT_TURN_TIME_PRM,mch_hotseatINI,"game parameters","wait_turn_time");
	mp -> SetLimits(0,180,30);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_SEEDS_ACCUMULATION_PRM,ACS_HOTSEAT_CFG_SCR,ACS_SEEDS_ACCUMULATION_PRM,mch_hotseatINI,"game parameters","seeds_accumulation");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER0_NAME,ACS_SPLIT_HS_SCR,ACS_PLAYER0_NAME,mch_hotseatINI,"player1","name");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER1_NAME,ACS_SPLIT_HS_SCR,ACS_PLAYER1_NAME,mch_hotseatINI,"player2","name");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER2_NAME,ACS_SPLIT_HS_SCR,ACS_PLAYER2_NAME,mch_hotseatINI,"player3","name");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER3_NAME,ACS_SPLIT_HS_SCR,ACS_PLAYER3_NAME,mch_hotseatINI,"player4","name");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_STRING,ACS_PLAYER4_NAME,ACS_SPLIT_HS_SCR,ACS_PLAYER4_NAME,mch_hotseatINI,"player5","name");
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_PLAYER0_TYPE,ACS_SPLIT_HS_SCR,ACS_PLAYER0_TYPE,mch_hotseatINI,"player1","ai_type");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_PLAYER1_TYPE,ACS_SPLIT_HS_SCR,ACS_PLAYER1_TYPE,mch_hotseatINI,"player2","ai_type");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_PLAYER2_TYPE,ACS_SPLIT_HS_SCR,ACS_PLAYER2_TYPE,mch_hotseatINI,"player3","ai_type");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_PLAYER3_TYPE,ACS_SPLIT_HS_SCR,ACS_PLAYER3_TYPE,mch_hotseatINI,"player4","ai_type");
	acsMM_Options -> append(mp);
	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_PLAYER4_TYPE,ACS_SPLIT_HS_SCR,ACS_PLAYER4_TYPE,mch_hotseatINI,"player5","ai_type");
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_INTRO_PRM,ACS_GRAPHICS_SCR,ACS_INTRO_PRM,mch_mainINI,"general","intro_movie");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_JOY_PRM2,ACS_CONTROLS_SCR,ACS_JOY_PRM2,mch_mainINI,"general","joystick");
	mp -> Value = mchEnableJoystick;
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	if(!mchDetectJoystick && mch_iScreen){
		p = acsGetObject(ACS_CONTROLS_SCR,ACS_JOY_TXT2);
		if(p) p -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
		p = acsGetObject(ACS_CONTROLS_SCR,ACS_JOY_PRM2);
		if(p) p -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
	}

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_JOY_PRM,ACS_CONTROLS_SINGLE_SCR,ACS_JOY_PRM,mch_optionsINI,"player_controller","enable_joystick");
	mp -> flags |= ACI_MULTIVALUE;
	mp -> Value = mp -> mValue[0] = mp -> mValue[1] = 1;
	mp -> mValue[2] = 0;
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_NUMBER,ACS_NUM_LAPS_PRM,ACS_HOTSEAT_CFG_SCR,ACS_NUM_LAPS_PRM,mch_mainINI,"race","num_laps_hs");
	mp -> SetLimits(1,10,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_TIPS_PRM,ACS_TB_CFG_SCR,ACS_TIPS_PRM,mch_mainINI,"general","tips");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_SPLIT_MODE_PRM,ACS_SPLIT_CFG_SCR,ACS_SPLIT_MODE_PRM,mch_optionsINI,"split_screen","mode");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_SPLIT_BOTS_PRM,ACS_SPLIT_CFG_SCR,ACS_SPLIT_BOTS_PRM,mch_optionsINI,"split_screen","bots");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);
/*
	mp = new aciMainMenuOption(ACI_NUMBER,ACS_TS_COUNT_PRM,ACS_RACE_CFG_SCR,ACS_TS_COUNT_PRM,mch_mainINI,"race","num_ts");
	mp -> SetLimits(5,50,5);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_NUMBER,ACS_NUM_ROBOTS_PRM,ACS_RACE_CFG_SCR,ACS_NUM_ROBOTS_PRM,mch_mainINI,"race","num_bots");
	mp -> SetLimits(0,4,1);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_TRIGGER,ACS_BOT_ARCANES_PRM,ACS_RACE_CFG_SCR,ACS_BOT_ARCANES_PRM,mch_mainINI,"race","ai_arcanes");
	mp -> SetLimits(0,1,1);
	acsMM_Options -> append(mp);
*/
	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_MOVE_FORWARD,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_MOVE_FORWARD,mch_optionsINI,"controls","move_fwd");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_MOVE_FORWARD,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_MOVE_BACKWARD,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_MOVE_BACKWARD,mch_optionsINI,"controls","move_bwd");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_MOVE_BACKWARD,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_TURN_LEFT,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_TURN_LEFT,mch_optionsINI,"controls","turn_left");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_TURN_LEFT,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_TURN_RIGHT,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_TURN_RIGHT,mch_optionsINI,"controls","turn_right");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_TURN_RIGHT,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_ROTATE_CAM,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_ROTATE_CAM,mch_optionsINI,"controls","rotate_camera");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_ROTATE_CAM,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_REPLAY,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_REPLAY,mch_optionsINI,"controls","replay");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_REPLAY,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_REPLAY_ALL,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_REPLAY_ALL,mch_optionsINI,"controls","replay_all");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_REPLAY_ALL,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_SLOPE_CAM,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_SLOPE_CAM,mch_optionsINI,"controls","slope_camera");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_SLOPE_CAM,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_CANCEL,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_CANCEL,mch_optionsINI,"controls","cancel");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_CANCEL,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_START_TIME,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_START_TIME,mch_optionsINI,"controls","start_time");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_START_TIME,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_CENTER_VIEW,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_CENTER_VIEW,mch_optionsINI,"controls","center_view");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_CENTER_VIEW,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_SLOW_TIME,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_SLOW_TIME,mch_optionsINI,"controls","slow_time");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_SLOW_TIME,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_FAST_TIME,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_FAST_TIME,mch_optionsINI,"controls","fast_time");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_FAST_TIME,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_REMOVE_SEED,ACS_CONTROLS_HOTSEAT_SCR,MCH_KEY_REMOVE_SEED,mch_optionsINI,"controls","remove_seed");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_REMOVE_SEED,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_SET_ARCANE0,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_SET_ARCANE0,mch_optionsINI,"controls","set_arcane0");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_SET_ARCANE0,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_SET_ARCANE1,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_SET_ARCANE1,mch_optionsINI,"controls","set_arcane1");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_SET_ARCANE1,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_SET_ARCANE2,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_SET_ARCANE2,mch_optionsINI,"controls","set_arcane2");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_SET_ARCANE2,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_ZOOM_IN,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_ZOOM_IN,mch_optionsINI,"controls","zoom_in");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_ZOOM_IN,i);
	acsMM_Options -> append(mp);

	mp = new aciMainMenuOption(ACI_KEY,MCH_KEY_ZOOM_OUT,ACS_CONTROLS_SINGLE_SCR,MCH_KEY_ZOOM_OUT,mch_optionsINI,"controls","zoom_out");
	for(i = 0; i < KEY_CONFIG_MAX; i ++) mp -> Value = mp -> mValue[i] = mchGetKey(MCH_KEY_ZOOM_OUT,i);
	acsMM_Options -> append(mp);

	mp = acsMM_Options -> first();
	while(mp){
		mp -> Load();
		mp -> SetObjectValue();
		mp -> Update(1);
		mp = mp -> next;
	}

	aciScreen* scr = (acsScrD) ? (aciScreen*)acsScrD -> scrList -> fPtr : NULL;
	while(scr){
		for(i = 0; i < ACS_NUM_OPTIONS; i ++){
			if(!mchOptions[i].iScreenObj){
				p = scr -> GetObject(mchOptionObjIDs[i]);
				if(p)
					mchOptions[i].iScreenObj = p;
			}
		}
		scr = (aciScreen*)scr -> next;
	}
	for(i = 0; i < ACS_NUM_OPTIONS; i ++){
//		if(!mchOptions[i].iScreenObj) XCon < "Option object not found -> " <= i < "\n";
		mchOptions[i].SetObjectValue();
		mchOptions[i].Update();
	}
}

void acsRectangle(int x,int y,int sx,int sy,int col,unsigned* pal,int alpha)
{
	int i,j,r,g,b;
	unsigned cl0,cl1;
	unsigned short* scrBuf;

	int alpha1 = 256 - alpha;

	cl1 = pal[col];
	for(i = 0; i < sy; i ++){
		scrBuf = (unsigned short*)XGR_GetVideoLine(y + i) + x;
		for(j = 0; j < sx; j ++){
			cl0 = scrBuf[j];
			r = (((cl0 & XGR_MASK_R) * alpha + (cl1 & XGR_MASK_R) * alpha1)) & (XGR_MASK_R << 8);
			g = (((cl0 & XGR_MASK_G) * alpha + (cl1 & XGR_MASK_G) * alpha1)) & (XGR_MASK_G << 8);
			b = (((cl0 & XGR_MASK_B) * alpha + (cl1 & XGR_MASK_B) * alpha1)) & (XGR_MASK_B << 8);
			scrBuf[j] = (r + g + b) >> 8;
		}
	}
}

void acsPutInStack(int id)
{
	acsStack -> Put(id);
}

void acsSetNextScr(int id)
{
	acsNextScreenID = id;
}

void acsClearStack(void)
{
	acsStack -> Clear();
}

void acsInitWorldObjs(void)
{
	int i;
	XBuffer XBuf;
	aciScreenInputField* p;

	for(i = 0; i < 10; i ++){
		p = (aciScreenInputField*)acsScrD -> GetObject(ACS_TRACK0 + i);
		if(p){
			if(i < acsCurNumTracks)
				p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			else
				p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

		}
	}
}

aciMainMenuOption::aciMainMenuOption(int tp,int id,int scr_id,int obj_id,char* ini_file,char* ini_sec,char* ini_key)
{
	aciScreen* scr;

	flags = 0;
	MinValue = MaxValue = ValueDelta = 0;

	type = tp;
	ID = id;
	ScreenID = scr_id;
	ObjectID = obj_id;

	Value = 0;
	ValueStr = new char[256];
	memset(ValueStr,0,256);

	scr = (acsScrD) ? acsScrD -> GetScreen(ScreenID) : NULL;
	if(scr)
		objPtr = scr -> GetObject(ObjectID);
	else
		objPtr = NULL;

	if(ini_file)
		iniFile = strdup(ini_file);
	else
		iniFile = NULL;

	iniSection = strdup(ini_sec);
	iniKey = strdup(ini_key);

	if(tp == ACI_KEY) flags |= ACI_MULTIVALUE;

	list = NULL;
}

aciMainMenuOption::~aciMainMenuOption(void)
{
	free(iniFile);
	free(iniSection);
	free(iniKey);

	delete ValueStr;
}

void aciMainMenuOption::Load(void)
{
	int i;

	XBuffer sec;
	char* ini_str;

	if(!iniFile) return;

	for(i = 0; i < KEY_CONFIG_MAX; i ++){
		sec.init();
		sec < iniSection;
		if(i) sec <= i;

		ini_str = getIniKey(iniFile,sec,iniKey);

		if(strlen(ini_str)){
			switch(type){
				case ACI_TRIGGER:
				case ACI_NUMBER:
				case ACI_KEY:
					mValue[i] = atoi(ini_str);
					if(!i)
						Value = mValue[i];
					break;
				case ACI_STRING:
					strcpy(ValueStr,ini_str);
					break;
			}
		}
		if(!(flags & ACI_MULTIVALUE)) break;
	}
}

void aciMainMenuOption::SetObjectValue(void)
{
	char* p;
	if(!objPtr) return;

	switch(type){
		case ACI_TRIGGER:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ){
				((aciScreenInputField*)objPtr) -> set_state(Value);
			}
			break;
		case ACI_NUMBER:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ){
				XBuffer XBuf(((aciScreenInputField*)objPtr) -> string,((aciScreenInputField*)objPtr) -> MaxStrLen);
				XBuf <= Value;
				objPtr -> flags |= ACS_REDRAW_OBJECT;
			}
			break;
		case ACI_STRING:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ){
				strcpy(((aciScreenInputField*)objPtr) -> string,ValueStr);
				objPtr -> flags |= ACS_REDRAW_OBJECT;
			}
			break;
		case ACI_KEY:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ){
				p = mchGetKeyNameText(mValue[acsCurKeyConfig]);
				if(!p) p = iGetText(iTXT_NONE);
				strcpy(((aciScreenInputField*)objPtr) -> string,p);
				objPtr -> flags |= ACS_REDRAW_OBJECT;
			}
			break;
	}
}

void aciMainMenuOption::GetObjectValue(void)
{
	if(!objPtr) return;

	switch(type){
		case ACI_TRIGGER:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ){
				if(flags & ACI_MULTIVALUE)
					mValue[acsCurKeyConfig] = ((aciScreenInputField*)objPtr) -> CurState;
				else
					Value = ((aciScreenInputField*)objPtr) -> CurState;
			}
			break;
		case ACI_NUMBER:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ)
				Value = atoi(((aciScreenInputField*)objPtr) -> string);
			break;
		case ACI_STRING:
			if(objPtr -> type == ACS_INPUT_FIELD_OBJ)
				strcpy(ValueStr,((aciScreenInputField*)objPtr) -> string);
			break;
		case ACI_KEY:
			if(!acsCancelInputFlag)
				Value = mValue[acsCurKeyConfig] = acsLastKey;
			break;
	}
	Update();
	Save();
}

void aciMainMenuOption::Save(void)
{
	int i;
	if(!iniFile) return;

	XBuffer sec;
	XBuffer XBuf(ValueStr,256);

	if(!(flags & ACI_MULTIVALUE)){
		switch(type){
			case ACI_TRIGGER:
			case ACI_NUMBER:
			case ACI_KEY:
				XBuf <= Value;
				putIniKey(iniFile,iniSection,iniKey,XBuf);
				break;
			case ACI_STRING:
				putIniKey(iniFile,iniSection,iniKey,ValueStr);
				break;
		}
		return;
	}

	for(i = 0; i < KEY_CONFIG_MAX; i ++){
		sec.init();
		sec < iniSection;
		if(i) sec <= i;

		switch(type){
			case ACI_TRIGGER:
			case ACI_NUMBER:
			case ACI_KEY:
				XBuf.init();
				XBuf <= mValue[i];
				putIniKey(iniFile,sec,iniKey,XBuf);
				break;
			case ACI_STRING:
				putIniKey(iniFile,sec,iniKey,ValueStr);
				break;
		}
	}
}

void aciMainMenuOption::Update(int mode)
{
	int i;
	mchRacerStats* p;

	switch(ID){
		case ACS_START_ASSEMBLED_PRM:
			hsStartAssembled = Value;
			break;
		case ACS_SEEDS_MAX_PRM:
			hsTurnSeedsMax = Value;
			break;
		case ACS_TURN_TIME_PRM:
			hsTurnTime = Value;
			break;
		case ACS_WAIT_TURN_TIME_PRM:
			hsWaitTurnTime = Value;
			break;
		case ACS_CAMERA_LIMIT_PRM:
			hsCameraLimit = Value;
			break;
		case ACS_SEEDS_ACCUMULATION_PRM:
			hsSeedsAccumulation = Value;
			break;
		case ACS_RETURN_MAX_PRM:
			hsReturnMax = Value;
			break;
		case ACS_PLAYER_NAME_PRM:
//			strcpy(hsPlayers[0].name,ValueStr);
			break;
		case ACS_BOT_ARCANES_PRM:
			mchBotArcaneUsing = Value;
			break;
		case ACS_NUM_ROBOTS_PRM:
			mchNumRacersDef = Value;
			break;
		case ACS_TIPS_PRM:
			mchEnableTips = Value;
			break;
		case ACS_NUM_LAPS_PRM:
			mchNumLapsDefHS = Value;
			break;
		case ACS_INTRO_PRM:
			iwEnableIntroMovie = Value;
			break;
		case ACS_JOY_PRM2:
			mchEnableJoystick = Value;
			break;
		case ACS_JOY_PRM:
			if(mode){
				for(i = 0; i < 3; i ++)
					acsJoyUse[i] = mValue[i];
			}
			else
				acsJoyUse[acsCurKeyConfig] = mValue[acsCurKeyConfig];
			break;
		case ACS_SPLIT_MODE_PRM:
			mchSplitScreenMode = Value;
			mchInitCamera();
			if(mchResourcesFlag && mchSplitScreenGame){
				mch_arcScrD -> playerSet1 -> init(0);
				mch_arcScrD -> playerSet2 -> init(1);
			}
			break;
		case ACS_SPLIT_BOTS_PRM:
			mchSplitScreenBots = Value;
			break;
		case ACS_TS_COUNT_PRM:
//			mchTimeStopArcaneCount = Value;
			break;
		case MCH_KEY_MOVE_FORWARD:
		case MCH_KEY_MOVE_BACKWARD:
		case MCH_KEY_TURN_LEFT:
		case MCH_KEY_TURN_RIGHT:
		case MCH_KEY_EXIT:
		case MCH_KEY_START_TIME:
		case MCH_KEY_CENTER_VIEW:
		case MCH_KEY_ARCANE_MENU:
		case MCH_KEY_MOUSE_ARCANE_MENU:
		case MCH_KEY_REMOVE_SEED:
		case MCH_KEY_NEXT_SEED:
		case MCH_KEY_SET_ARCANE0:
		case MCH_KEY_SET_ARCANE1:
		case MCH_KEY_SET_ARCANE2:
		case MCH_KEY_SET_ARCANE3:
		case MCH_KEY_SET_ARCANE4:
		case MCH_KEY_REMOVE_ARCANE:
		case MCH_KEY_VIS_MODE:
		case MCH_KEY_STATUS_STRING:
		case MCH_KEY_FULLSCREEN:
		case MCH_KEY_OK:
		case MCH_KEY_HORN:
		case MCH_KEY_MAP:
		case MCH_KEY_ROTATE_CAM:
		case MCH_KEY_SLOPE_CAM:
		case MCH_KEY_CANCEL:
		case MCH_KEY_SET_TS:
		case MCH_KEY_TS_MOUSE:
		case MCH_KEY_SET_BONUS0:
		case MCH_KEY_SET_BONUS1:
		case MCH_KEY_SET_BONUS2:
		case MCH_KEY_ZOOM_IN:
		case MCH_KEY_ZOOM_OUT:
		case MCH_KEY_REPLAY:
		case MCH_KEY_REPLAY_ALL:
		case MCH_KEY_SLOW_TIME:
		case MCH_KEY_FAST_TIME:
			if(mode){
				for(i = 0; i < KEY_CONFIG_MAX; i ++)
					mchSetKey(ID,mValue[i],i);
			}
			else
				mchSetKey(ID,mValue[acsCurKeyConfig]);
			break;
		case ACS_PLAYER0_NAME:
		case ACS_PLAYER1_NAME:
		case ACS_PLAYER2_NAME:
		case ACS_PLAYER3_NAME:
		case ACS_PLAYER4_NAME:
			if(!mchPBEM_Game)
				strcpy(hsPlayers[ID - ACS_PLAYER0_NAME].name,ValueStr);
			break;
		case ACS_PLAYER0_TYPE:
		case ACS_PLAYER1_TYPE:
		case ACS_PLAYER2_TYPE:
		case ACS_PLAYER3_TYPE:
		case ACS_PLAYER4_TYPE:
			hsPlayers[ID - ACS_PLAYER0_TYPE].type = Value;
			if(mchRacersInitFlag){
				p = mchGetRacer(ID - ACS_PLAYER0_TYPE);
				p -> PutMechos();
				if(Value == 1){
					p -> poolID = p -> ID;
				}
				else {
					if(Value > 1)
						p -> poolID = HS_PLAYERS_MAX;
					else 
						p -> poolID = HS_PLAYERS_MAX + 1;
				}
				p -> GetMechos();
			}
			break;
		default:
			if(ID >= ACS_CD_TRACK00_STATUS && ID < ACS_CD_TRACK00_STATUS + ACS_MAX_CD_TRACK){
			}
			break;
	}
}

void aciMainMenuOption::SetLimits(int min,int max,int delta)
{
	MinValue = min;
	MaxValue = max;
	ValueDelta = delta;
	flags |= ACI_LIMITS;
}

void aciMainMenuOption::incValue(void)
{
	if(flags & ACI_LIMITS){
		if(flags & ACI_MANUAL_SWITCHING && (Value > MaxValue || Value < MinValue))
			return;

		Value += ValueDelta;
		if(Value > MaxValue) Value = MinValue;
	}
	else
		Value ++;

	Update();
	SetObjectValue();
	Save();
}

void aciMainMenuOption::decValue(void)
{
	if(flags & ACI_LIMITS){
		if(flags & ACI_MANUAL_SWITCHING && (Value > MaxValue || Value < MinValue))
			return;

		Value -= ValueDelta;
		if(Value < MinValue) Value = MaxValue;
	}
	else
		Value --;

	Update();
	SetObjectValue();
	Save();
}

void acsInitMM_Options(void)
{
	aciMainMenuOption* p = acsMM_Options -> first();
	while(p){
		p -> Load();
		p -> SetObjectValue();
		p = p -> next;
	}
}

void acsIncMM_Option(int scr,int obj)
{
	aciMainMenuOption* p = acsMM_Options -> first();
	while(p){
		if(p -> ScreenID == scr && p -> ObjectID == obj){
			p -> incValue();
			return;
		}
		p = p -> next;
	}
}

void acsUpdateMM_Option(int scr,int obj)
{
	aciMainMenuOption* p = acsMM_Options -> first();
	while(p){
		if(p -> ScreenID == scr && p -> ObjectID == obj){
			p -> GetObjectValue();
			p -> Update();
			p -> Save();
			return;
		}
		p = p -> next;
	}
}

void acsSetMM_Option(int scr,int obj,int val)
{
	aciMainMenuOption* p = acsMM_Options -> first();
	while(p){
		if(p -> ScreenID == scr && p -> ObjectID == obj){
			p -> Value = val;
			p -> Update();
			p -> Save();
			return;
		}
		p = p -> next;
	}
}

void acsDecMM_Option(int scr,int obj)
{
	aciMainMenuOption* p = acsMM_Options -> first();
	while(p){
		if(p -> ScreenID == scr && p -> ObjectID == obj){
			p -> decValue();
			return;
		}
		p = p -> next;
	}
}

void mchInitControls(void)
{
	mchControlKey* p;
	mchControls = new xtList<mchControlKey>;

	p = new mchControlKey(MCH_KEY_EXIT,VK_ESCAPE);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_ROTATE_CAM,VK_SPACE);
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_SLOPE_CAM,VK_CONTROL);
	p -> SetCode(VK_CONTROL,1,1);
	p -> SetCode(VK_CONTROL,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_HORN,VK_SPACE);
	p -> SetCode(VK_SPACE,1,1);
	p -> SetCode(VK_RETURN,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_REPLAY,'V');
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_REPLAY_ALL,'R');
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_MAP,VK_SLASH);
	p -> SetCode(VK_SLASH,1,1);
	p -> SetCode(VK_SLASH,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_OK,VK_RETURN);
	p -> SetCode(VK_SPACE,1,1);
	p -> SetCode(VK_RETURN,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_CANCEL,VK_BACK);
	p -> SetCode(VK_TILDE,1,1);
	p -> SetCode(VK_BACK,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_START_TIME,VK_RETURN);
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_CENTER_VIEW,VK_INSERT);
	p -> SetCode(VK_TAB,1,1);
	p -> SetCode(VK_INSERT,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_SLOW_TIME,VK_SUBTRACT);
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_FAST_TIME,VK_ADD);
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_ARCANE_MENU,VK_OEM_ALT);
	p -> SetCode(VK_SHIFT,1,1);
	p -> SetCode(VK_OEM_ALT,2,1);
	mchControls -> append(p);

//	p = new mchControlKey(MCH_KEY_MOUSE_ARCANE_MENU,VK_SHIFT);
//	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_REMOVE_SEED,VK_DELETE);
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_SET_ARCANE0,'1');
	p -> SetCode('1',1,1);
	p -> SetCode('8',2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_SET_ARCANE1,'2');
	p -> SetCode('2',1,1);
	p -> SetCode('9',2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_SET_ARCANE2,'3');
	p -> SetCode('3',1,1);
	p -> SetCode('0',2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_ZOOM_IN,VK_PRIOR);
	p -> SetCode('Z',1,1);
	p -> SetCode(VK_PRIOR,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_ZOOM_OUT,VK_NEXT);
	p -> SetCode('X',1,1);
	p -> SetCode(VK_NEXT,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_REMOVE_ARCANE,'0');
	p -> SetCode(0,1,1);
	p -> SetCode(0,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_VIS_MODE,VK_F8);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_STATUS_STRING,VK_F7);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_MOVE_FORWARD,VK_UP);
	p -> SetCode('W',1,1);
	p -> SetCode(VK_UP,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_MOVE_BACKWARD,VK_DOWN);
	p -> SetCode('S',1,1);
	p -> SetCode(VK_DOWN,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_TURN_LEFT,VK_LEFT);
	p -> SetCode('A',1,1);
	p -> SetCode(VK_LEFT,2,1);
	mchControls -> append(p);

	p = new mchControlKey(MCH_KEY_TURN_RIGHT,VK_RIGHT);
	p -> SetCode('D',1,1);
	p -> SetCode(VK_RIGHT,2,1);
	mchControls -> append(p);
}

int mchGetKeyID(int key,int config)
{
	mchControlKey* p = mchControls -> first();
	while(p){
		if(p -> Code(config) == key) return p -> ID;
		p = p -> next;
	}
	return -1;
}

int acsCurSearchKey = -1;
int acsCurSearchKeyCfg = -1;
mchControlKey* acsCurKey;

int mchGetKeyID_First(int key,int config)
{
	mchControlKey* p = mchControls -> first();

	acsCurSearchKey = key;
	acsCurSearchKeyCfg = config;

	while(p){
		if(p -> Code(config) == key){ 
			acsCurKey = p;
			return p -> ID;
		}
		p = p -> next;
	}
	return -1;
}

int mchGetKeyID_Next(void)
{
	int key = acsCurSearchKey;
	int config = acsCurSearchKeyCfg;
	mchControlKey* p = acsCurKey;

	if(p) p = p -> next;

	while(p){
		if(p -> Code(config) == key){ 
			acsCurKey = p;
			return p -> ID;
		}
		p = p -> next;
	}
	return -1;
}

int mchKeyPressed(int id,int config)
{
	mchControlKey* p = mchControls -> search(id);
	if(p){
		if(p -> Code(config) > 0 && p -> Code(config) < 256){
			return XKey.Pressed(p -> Code(config));
		}
		else {
			if(p -> Code(config) >= VK_BUTTON_1 && p -> Code(config) <= VK_BUTTON_32){
				return XJoystickIsButtonPressed(p -> Code(config));
			}
		}
	}
	return 0;
}

void mchSetKey(int id,int code,int cfg)
{
	mchControlKey* p = mchControls -> search(id);
	if(p)
		p -> SetCode(code,cfg);
}

int mchGetKey(int id)
{
	mchControlKey* p = mchControls -> search(id);
	if(p)
		return p -> Code();

	return 0;
}

int mchGetKey(int id,int cfg)
{
	mchControlKey* p = mchControls -> search(id);
	if(p)
		return p -> Code(cfg);

	return 0;
}

void acsLockObject(int scr_id,int obj_id,int mode)
{
	aciScreen* scr;
	aciScreenObject* p;

	scr = acsScrD -> GetScreen(scr_id);
	if(scr){
		p = scr -> GetObject(obj_id);
		if(p){
			if(mode) 
				p -> flags |= ACS_BLOCKED | ACS_HIDDEN;
			else
				p -> flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
		}
	}
}

aciScreenObject* acsGetObject(int scr_id,int obj_id)
{
	aciScreen* scr;
	aciScreenObject* p;

	scr = acsScrD -> GetScreen(scr_id);
	if(scr){
		p = scr -> GetObject(obj_id);
		if(p)
			return p;
	}
	return NULL;
}

void acsResetControls(int mode)
{
	int i;
	aciMainMenuOption* mp;
	mchControlKey* p = mchControls -> first();
	while(p){
		if(!mode){
			p -> ResetCode();
			mp = acsMM_Options -> search(p -> ID);
			if(mp){
				for(i = 0; i < KEY_CONFIG_MAX; i ++)
					mp -> mValue[i] = p -> Code(i);
				mp -> Value = p -> Code(acsCurKeyConfig);
				mp -> SetObjectValue();
				mp -> Save();
			}
		}
		if(mode == ACS_CONTROLS_SINGLE_SCR){
			switch(p -> ID){
				case MCH_KEY_CENTER_VIEW:
				case MCH_KEY_ARCANE_MENU:
				case MCH_KEY_SET_ARCANE0:
				case MCH_KEY_SET_ARCANE1:
				case MCH_KEY_SET_ARCANE2:
				case MCH_KEY_ZOOM_IN:
				case MCH_KEY_ZOOM_OUT:
				case MCH_KEY_MOVE_FORWARD:
				case MCH_KEY_MOVE_BACKWARD:
				case MCH_KEY_TURN_LEFT:
				case MCH_KEY_TURN_RIGHT:
				case MCH_KEY_OK:
				case MCH_KEY_HORN:
				case MCH_KEY_MAP:
				case MCH_KEY_CANCEL:
				case MCH_KEY_SLOPE_CAM:
					p -> ResetCode();
					mp = acsMM_Options -> search(p -> ID);
					if(mp){
						for(i = 0; i < KEY_CONFIG_MAX; i ++)
							mp -> mValue[i] = p -> Code(i);
						mp -> Value = p -> Code(acsCurKeyConfig);
						mp -> SetObjectValue();
						mp -> Save();
					}
					break;
			}
		}

		if(mode == ACS_CONTROLS_HOTSEAT_SCR){
			switch(p -> ID){
				case MCH_KEY_ROTATE_CAM:
				case MCH_KEY_START_TIME:
				case MCH_KEY_REMOVE_SEED:
				case MCH_KEY_NEXT_SEED:
				case MCH_KEY_MOUSE_ARCANE_MENU:
				case MCH_KEY_REMOVE_ARCANE:
				case MCH_KEY_REPLAY:
				case MCH_KEY_REPLAY_ALL:
				case MCH_KEY_SLOW_TIME:
				case MCH_KEY_FAST_TIME:
					p -> ResetCode();
					mp = acsMM_Options -> search(p -> ID);
					if(mp){
						for(i = 0; i < KEY_CONFIG_MAX; i ++)
							mp -> mValue[i] = p -> Code(i);
						mp -> Value = p -> Code(acsCurKeyConfig);
						mp -> SetObjectValue();
						mp -> Save();
					}
					break;
			}
		}

		p = p -> next;
	}
	acsInitControls();
}

void acsInitControls(void)
{
	int i;
	aciScreenObject* p;
	aciMainMenuOption* op;

	for(i = MCH_KEY_EXIT; i < MCH_KEY_MAX; i ++){
		op = acsMM_Options -> search(i);
		if(op){ 
			op -> Value = mchGetKey(i);
			op -> SetObjectValue();
		}
	}

	op = acsMM_Options -> search(ACS_JOY_PRM);
	if(op){ 
		op -> Value = op -> mValue[acsCurKeyConfig];
		op -> SetObjectValue();

		if(!mchEnableJoystick){
			p = acsGetObject(ACS_CONTROLS_SINGLE_SCR,ACS_JOY_TXT);
			if(p) p -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
			p = acsGetObject(ACS_CONTROLS_SINGLE_SCR,ACS_JOY_PRM);
			if(p) p -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
		}
		else {
			p = acsGetObject(ACS_CONTROLS_SINGLE_SCR,ACS_JOY_TXT);
			if(p) p -> flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
			p = acsGetObject(ACS_CONTROLS_SINGLE_SCR,ACS_JOY_PRM);
			if(p) p -> flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
		}
	}
}

int acsJoyX_Axis = 0;
int acsJoyY_Axis = 0;

void acsJoyHandler(eXJoystickState state, int button_or_axis_value, int joystick_ID)
{
	switch(state){
		case XJ_PressButton:
			if(acsActiveFlag && !acsScrD -> activeInput){
				if(button_or_axis_value == VK_BUTTON_1)
					KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_PRESSED);
				if(button_or_axis_value == VK_BUTTON_2)
					KeyBuf -> put(VK_ESCAPE,KBD_CUR_KEY_PRESSED);
			}
			else {
				if(mchTimeMode == MCH_TIME_GAME_OVER)
					KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_PRESSED);

				if(iWorldMode){
					if(button_or_axis_value == VK_BUTTON_1)
						iwJoyKeyTrap(MCH_KEY_OK);
					if(button_or_axis_value == VK_BUTTON_2)
						iwJoyKeyTrap(MCH_KEY_CANCEL);
				}

				KeyBuf -> put(button_or_axis_value,KBD_CUR_KEY_PRESSED);
			}
			break;
		case XJ_UnPressButton:
			break;
		case XJ_ChangeXAxis:
			acsJoyX_Axis = button_or_axis_value;
			break;
		case XJ_ChangeYAxis:
			acsJoyY_Axis = button_or_axis_value;
			break;
	}
}

void acsJoyQuant(void)
{
	int dx,dy,key_x,key_y;

	const int delay_max = 150;

	static int delay_x = 0;
	static int delay_y = 0;

	if(!mchEnableJoystick)
		return;

	XJoystickQuant();

	dx = acsJoyX_Axis * 32 / (RANGE_MAX - RANGE_MIN);
	dy = acsJoyY_Axis * 32 / (RANGE_MAX - RANGE_MIN);

	key_x = key_y = 0;

	if(abs(dx) >= 12){
		if(clocki() - delay_x >= delay_max){
			key_x = (dx < 0) ? VK_LEFT : VK_RIGHT;
			delay_x = clocki();
		}
	}
	if(abs(dy) >= 12){
		if(clocki() - delay_y >= delay_max){
			key_y = (dy < 0) ? VK_UP : VK_DOWN;
			delay_y = clocki();
		}
	}

	if((acsActiveFlag && !acsScrD -> activeInput) || mchTimeMode == MCH_TIME_GAME_OVER){
		if(key_x)
			KeyBuf -> put(key_x,KBD_CUR_KEY_PRESSED);
		if(key_y)
			KeyBuf -> put(key_y,KBD_CUR_KEY_PRESSED);
	}

	if(!acsActiveFlag && iWorldMode){
		if(key_x == VK_LEFT) iwJoyKeyTrap(MCH_KEY_TURN_LEFT);
		if(key_x == VK_RIGHT) iwJoyKeyTrap(MCH_KEY_TURN_RIGHT);

		if(key_y == VK_UP) iwJoyKeyTrap(MCH_KEY_MOVE_FORWARD);
		if(key_y == VK_DOWN) iwJoyKeyTrap(MCH_KEY_MOVE_BACKWARD);
	}

	acsJoyX_Axis = 0;
	acsJoyY_Axis = 0;
}

char* mchGetKeyNameText(int code)
{
	static XBuffer XBuf;

	if(code > 0 && code < 256)
		return xtGetKeyNameText(code);

	if(code >= VK_BUTTON_1 && code <= VK_BUTTON_32){
		XBuf.init();
		XBuf < "JOY" <= code - VK_BUTTON_1 + 1;
		return XBuf.address();
	}

	return NULL;
}

int mchControlKey::Code(int cfg) const 
{
	if(cfg == -1) cfg = acsCurKeyConfig;
	return code[cfg];
}

int mchControlKey::defCode(int cfg) const
{
	if(cfg == -1) cfg = acsCurKeyConfig;
	return default_code[cfg];
}

void mchControlKey::SetCode(int cd,int cfg,int def_mode)
{ 
	if(cfg == -1) cfg = acsCurKeyConfig;
	code[cfg] = cd;

	if(def_mode)
		default_code[cfg] = cd;
}

void mchControlKey::SetDefCode(int cd,int cfg)
{ 
	if(cfg == -1) cfg = acsCurKeyConfig;
	default_code[cfg] = cd;
}

void mchControlKey::ResetCode(int cfg)
{ 
	if(cfg == -1) cfg = acsCurKeyConfig;
	code[cfg] = default_code[cfg]; 
}

void acsSaveScreenState(XBuffer* xb,aciScreen* scr)
{
	int sz = 0;
	aciScreenEvent* ev;
	aciScreenEventCommand* cm;
	aciScreenObject* obj;

	xb -> init();

	ev = (aciScreenEvent*)scr -> events -> fPtr;
	while(ev){
		if(ev -> flags & ACS_EVENT_ACTIVE)
			sz ++;
		ev = (aciScreenEvent*)ev -> next;
	}

	obj = (aciScreenObject*)scr -> objList -> fPtr;
	while(obj){
		ev = (aciScreenEvent*)obj -> events -> fPtr;
		while(ev){
			if(ev -> flags & ACS_EVENT_ACTIVE)
				sz ++;
			ev = (aciScreenEvent*)ev -> next;
		}
		obj = (aciScreenObject*)obj -> next;
	}

	*xb < sz;

	ev = (aciScreenEvent*)scr -> events -> fPtr;
	while(ev){
		if(ev -> flags & ACS_EVENT_ACTIVE){
			*xb < (long)ev < ev -> flags < ev -> CurTimer;
			cm = (aciScreenEventCommand*)ev -> commSeq -> fPtr;
			while(cm){
				*xb < cm -> flags;
				cm = (aciScreenEventCommand*)cm -> next;
			}
		}
		ev = (aciScreenEvent*)ev -> next;
	}

	obj = (aciScreenObject*)scr -> objList -> fPtr;
	while(obj){
		ev = (aciScreenEvent*)obj -> events -> fPtr;
		while(ev){
			if(ev -> flags & ACS_EVENT_ACTIVE){
				*xb < (long)ev < ev -> flags < ev -> CurTimer;
				cm = (aciScreenEventCommand*)ev -> commSeq -> fPtr;
				while(cm){
					*xb < cm -> flags;
					cm = (aciScreenEventCommand*)cm -> next;
				}
			}
			ev = (aciScreenEvent*)ev -> next;
		}
		obj = (aciScreenObject*)obj -> next;
	}
}

void acsRestoreScreenState(XBuffer* xb,aciScreen* scr)
{
	int i,id,sz;
	aciScreenEvent* ev;
	aciScreenObject* obj;
	aciScreenEventCommand* cm;

	ev = (aciScreenEvent*)scr -> events -> fPtr;
	while(ev){
		ev -> Stop();
		ev = (aciScreenEvent*)ev -> next;
	}

	obj = (aciScreenObject*)scr -> objList -> fPtr;
	while(obj){
		ev = (aciScreenEvent*)obj -> events -> fPtr;
		while(ev){
			ev -> Stop();
			ev = (aciScreenEvent*)ev -> next;
		}
		obj = (aciScreenObject*)obj -> next;
	}

	xb -> set(0);

	*xb > sz;

	for(i = 0; i < sz; i ++){
		*xb > id;
		ev = (aciScreenEvent*)id;

		*xb > ev -> flags > ev -> CurTimer;
		cm = (aciScreenEventCommand*)ev -> commSeq -> fPtr;
		while(cm){
			*xb > cm -> flags;
			cm = (aciScreenEventCommand*)cm -> next;
		}
	}
}

mchControlKey::mchControlKey(int id,int c)
{ 
	int i;
	ID = id; 

	for(i = 0; i < KEY_CONFIG_MAX; i ++)
		default_code[i] = code[i] = c; 

	list = NULL; 
}

void mchSelectKeysConfig(int cfg_id)
{
	if(cfg_id == -1){
		mchRestoreKeysConfig();
	}
	else {
		acsCurKeyConfigPrev = acsCurKeyConfig;
		acsCurKeyConfig = cfg_id;
	}
}

void mchRestoreKeysConfig(void)
{
	acsCurKeyConfig = acsCurKeyConfigPrev;
}

int mchGetKeysConfig(void)
{
	return acsCurKeyConfig;
}

cSurfaceReflectionMultiMaterial *GlobalMainMenuSurfaceReflection=0;

void acsCreateWater(void)
{
	const int xTextureSize=4,yTextureSize=4;
	char *VectorMenuName[xTextureSize*yTextureSize]={
		"Menu00.bmp","Menu01.bmp","Menu02.bmp","Menu03.bmp",
		"Menu10.bmp","Menu11.bmp","Menu12.bmp","Menu13.bmp",
		"Menu20.bmp","Menu21.bmp","Menu22.bmp","Menu23.bmp",
		"Menu30.bmp","Menu31.bmp","Menu32.bmp","Menu33.bmp"		};
	float xCamera=XGR_MAXX/2,yCamera=XGR_MAXY/2,zCamera=1000,fCamera=1000;
	float SurfaceReflection_zpos=-20,
		SurfaceReflection_xpos=xCamera*(1.f-(zCamera-SurfaceReflection_zpos)/fCamera)-15,
		SurfaceReflection_ypos=yCamera*(1.f-(zCamera-SurfaceReflection_zpos)/fCamera)-15;
	GlobalMainMenuSurfaceReflection=BaseObject_SurfaceReflection_Create(SurfaceReflection_xpos,SurfaceReflection_ypos,SurfaceReflection_zpos,
		(xCamera-SurfaceReflection_xpos)*2,(yCamera-SurfaceReflection_ypos)*2,
		xTextureSize,yTextureSize,0.f,0.f,
		VectorMenuName,0,"RESOURCE\\FX\\WAVE",40);
}

void acsSetTurnBased(int mode,int value)
{
	aciScreenInputField* p = (aciScreenInputField*)acsGetObject(ACS_MAIN_MENU_SCR4,1);

	switch(mode){
		case -1: // disable
			if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

			mchTurnBasedGame = 0;
			return;
		case 0: // set
			if(p) p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);

			if(mchResourcesFlag){
				if(value)
					iworld_D -> SetFlag(IW_TURNBASED_FLAG);
				else
					iworld_D -> DropFlag(IW_TURNBASED_FLAG);
			}
			else
				return;
			break;
		case 1: // toggle
			if(!mchSplitScreenGame)
				iworld_D -> ToggleFlag(IW_TURNBASED_FLAG);

			if(p) p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
			else return;
			break;
	}

	if(p){
		if(iworld_D -> CheckFlag(IW_TURNBASED_FLAG))
			p -> set_state(1);
		else
			p -> set_state(0);
	}
}

void acsBackupControls(void)
{
	int i,sz;
	mchControlKey* p;
	
	if(!mchControls)
		return;

	sz = KEY_CONFIG_MAX * 2 * (MCH_KEY_MAX - MCH_KEY_EXIT) * sizeof(int);

	if(!mchControlsBackupBuf)
		mchControlsBackupBuf = new char[sz];

	XBuffer XBuf(mchControlsBackupBuf,sz);

	p = mchControls -> first();
	while(p){
		for(i = 0; i < KEY_CONFIG_MAX; i ++)
			XBuf < p -> Code(i) < p -> defCode(i);
		p = p -> next;
	}
}

void acsRestoreControls(void* buf)
{
	int i,sz,cd,dcd;
	mchControlKey* p;
	
	if(!mchControls)
		return;

	if(!buf)
		buf = mchControlsBackupBuf;

	sz = KEY_CONFIG_MAX * 2 * (MCH_KEY_MAX - MCH_KEY_EXIT) * sizeof(int);

	XBuffer XBuf(buf,sz);

	p = mchControls -> first();
	while(p){
		for(i = 0; i < KEY_CONFIG_MAX; i ++){
			XBuf > cd > dcd;

			p -> SetCode(cd,i);
			p -> SetDefCode(dcd,i);
		}
		p = p -> next;
	}
}

void* acsGetControlsBackupBuf(int& sz)
{
	sz = KEY_CONFIG_MAX * 2 * (MCH_KEY_MAX - MCH_KEY_EXIT) * sizeof(int);
	return mchControlsBackupBuf;
}

void acsInitDemo(int tb_mode)
{
	XBuffer fname;
	const int num_demos = 1;

	if(tb_mode)
		fname < "RESOURCE\\DEMO\\TB\\demo0" <= XRnd(num_demos) < ".dem";
	else
		fname < "RESOURCE\\DEMO\\RT\\demo0" <= XRnd(num_demos) < ".dem";

	mchXRecorderMode = XRC_PLAY_MODE;
	if(mchXRecorderName)
		free(mchXRecorderName);

	mchXRecorderName = strdup(fname.address());
}

void aciMainMenuOption::SetObjectVisible(int v)
{
	if(!objPtr) return;

	if(v)
		objPtr -> flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
	else
		objPtr -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
}

void acsInitOnlineMenu(void)
{
	aciScreenObject* p = acsGetObject(ACS_MAIN_MENU_SCR6,ACS_LOAD_OPTION);
	if(!p) return;

	if(!mchPBEM_CheckFlag(PBEM_REPLAY_MODE) || mchTimeMode != MCH_TIME_RUNNING)
		p -> flags |= (ACS_BLOCKED | ACS_HIDDEN);
	else
		p -> flags &= ~(ACS_BLOCKED | ACS_HIDDEN);
}
