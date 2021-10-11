
#ifndef __ARCANE_MENU_H__
#define __ARCANE_MENU_H__

struct mchArcaneScreenElement;
struct mchArcaneRacerSet;
struct mchArcaneStatsDispatcher;
struct mchArcaneMenu;

enum mchA_EndGameModes 
{
	AE_RACE_LOST,
	AE_RACE_WON,
	AE_RACE_NOT_LOST,
	AE_RACE_END
};

#define AE_FLUSH_LINE_SIZE	100

struct mchFlushData
{
	int X;
	int Y;

	int SizeX;
	int SizeY;
};

struct mchFlushLine
{
	int size;
	int first_index;
	int last_index;
	int flags;

	mchFlushData** table;
	char* mem_heap;

	void clear(void);
	void put(int x,int y,int sx,int sy);
	mchFlushData* get(void);

	mchFlushLine(void);
	~mchFlushLine(void);
};

// mchArcaneBMP::flags
#define AE_OPEN_FILE		0x01

struct mchArcaneBMP
{
	int ID;
	int flags;

	int SizeX;
	int SizeY;

	int bound[4];

	char* fileName;
	char* data;

	int numFrames;
	char** frameData;

	void* list;
	mchArcaneBMP* prev;
	mchArcaneBMP* next;

	void Crop(void);
	void Load(char* fname);
	void Load(char* fname,XStream& fh);
	void Save(char* fname);
	void Scale(int sx,int sy);
	void Expand(int sx,int sy);

	void Free(void);

	void CalcBound(void);

	mchArcaneBMP(char* fname);
	mchArcaneBMP(void);
	mchArcaneBMP(int sx,int sy);
	~mchArcaneBMP(void);
};

struct mchArcaneRacerSet
{
	int wndID;
	int mapFlag;

	struct mchRacer* owner;

	mchArcaneScreenElement* starsEl;
	mchArcaneScreenElement* speedEl;
	mchArcaneScreenElement* energyEl;
	mchArcaneScreenElement* arrowEl;
	mchArcaneScreenElement* nameEl;
	mchArcaneScreenElement* placeEl;
	mchArcaneScreenElement* lapEl;
	mchArcaneScreenElement* mapEl;
	mchArcaneScreenElement* lapcntEl;
	mchArcaneScreenElement* cpEl;

	float arrowAngle;

	int dropCp;
	int dropCpLap;
	int dropIdx;
	int dropCnt;
	int dropTimer;

	mchArcaneStatsDispatcher* statD;
	mchArcaneMenu* menu;

	void ToggleMap(void);

	void init(int align = -1);
	void SetState(int st);

	void SetOwner(struct mchRacer* p);

	void Quant(void);

	mchArcaneRacerSet(void);
	~mchArcaneRacerSet(void);
};

enum mchA_ShowModes 
{
	AE_SHOW_SCREEN = 1,
	AE_HIDE_SCREEN,
	AE_BUILD_SCREEN,
	AE_BUILD_RACERS_SCREEN
};

// mchArcaneScreenElement::flags
#define AE_DISABLED		        0x01
#define AE_HIDDEN		        0x02
#define AE_GETFON		        0x04
#define AE_WINDOW_CLIP			0x08
#define AE_NO_FON	                0x10
#define AE_GLOBAL_COORDS		0x20
#define AE_FREE				0x40
#define AE_MOVING			0x80
#define AE_ROLLBACK			0x100
#define AE_NO_REDRAW			0x200
#define AE_CENTERED			0x400
#define AE_BLINK			0x800
#define AE_ROTATE			0x1000
#define AE_END_ROTATE			0x2000
#define AE_SELECTED			0x4000
#define AE_ALIGN_RIGHT			0x8000
#define AE_ALPHA_SCALE			0x10000
#define AE_HALF_ALPHA			0x20000
#define AE_FINISH_SCREEN_EL		0x40000

enum mchAcraneScreenElementType 
{
	AE_BASE_TYPE = 0,
	AE_MENU,		// 1
	AE_SEED_MARK,		// 2
	AE_ENERGY_BAR,		// 3
	AE_SEED_TEXT,		// 4
	AE_SEED_SYMBOL,		// 5
	AE_SEED_COUNTER,	// 6
	AE_TEXT,		// 7
	AE_TIME_TEXT,		// 8

	AE_CMENU,		// 9
	AE_AMENU,		// 10
	AE_SMENU,		// 11
	AE_STATUS_STR,		// 12
	AE_NAME_STATUS_STR,	// 13

	AE_RACER_POINTER,	// 14
	AE_WORLD_MAP,		// 15
	AE_ARCANE_STATS,	// 16

	AE_TIME_STOP_COUNTER,	// 17
	AE_TIME_STOP_MOUSE,	// 18
	AE_STAR_COUNTER,	// 19
	AE_SPEED_COUNTER,	// 20

	AE_LAP_COUNTER,		// 21
	AE_START_COUNTER,	// 22

	AE_PLACE_STR,		// 23
	AE_FINISH_PLACE_STR,	// 24
	AE_LAP_STR,		// 25

	AE_FIGURE_FACE,		// 26
	AE_ARROW,		// 27

	AE_FINISH_FIGURE_FACE,	// 28
	AE_FINISH_STAR_COUNTER,	// 29
	AE_3D_TEXT,		// 30

	AE_BONUS_PART,		// 31

	AE_RECTANGLE,		// 32
	AE_WAITING_TEXT,	// 33

	AE_DROP_TEXT,		// 34
	AE_TIME_SPEED,		// 35

	AE_MAX_TYPE
};

enum mchArcaneScreenElementAlign
{
	AE_LEFT = 1,
	AE_TOP,
	AE_RIGHT,
	AE_BOTTOM,
	AE_CENTER
};

struct mchArcaneScreenElement
{
	int ID;

	int type;
	int flags;

	float Timer;

	int clockTimer;
	int clockPeriod;

	Vect3f R;
	Vect3f R0;
	Vect3f R1;
	int align_x;
	int align_y;

	float phase;
	float dest_phase;

	float Scale;
	float destScale;
	float deltaScale;

	int showDir;
	int showIndex;

	int objDir[4];

	int SizeX;
	int SizeY;

	int color;

	int Alpha;
	int dAlpha;
	int trgAlpha;

	int dataType;

	int exDataParam[5];
	void* exDataPtr;

	mchArcaneRacerSet* owner;

	void* list;
	mchArcaneScreenElement* prev;
	mchArcaneScreenElement* next;

	virtual void Redraw(void);
	virtual void InitRedraw(void);
	virtual void RedrawFnc(int x,int y);
	virtual void Quant(float dt);

	void PhaseQuant(int level,int mode = AE_SHOW_SCREEN);
	int CheckPhase(void){ 
		if(fabs(phase - dest_phase) < 0.1f)
			return 0;
		return 1;
	}

	void ScaleQuant(void);

	virtual void alloc(void);
	virtual void free(void);

	void InitCoords(char* name);
	void ScaleCoords(struct mchGameWindow* src,struct mchGameWindow* dest);
	void ScaleSize(float sc);
	void InitR(void);

	int CheckCoords(int x,int y){ 
		if(x >= R.xi() && x < R.xi() + SizeX && y >= R.yi() && y < R.yi() + SizeY) return 1;
		return 0;
	};

	void GetFon(void);
	void PutFon(void);

	void SetString(int fnt,int sp,void* p);
	void SetString3D(int fnt,int sp,void* p);
	virtual void SetState(int st);
	void SetTimer(int tm){ SetState(1); Timer = tm; }

	virtual void MouseHandler(int bt);
	virtual void KeyHandler(int bt,int id);

	void SetAlphaSeq(int al,int d_al,int t_al){ Alpha = al; dAlpha = d_al; trgAlpha = t_al; }
	int Active(void){ return !(flags & (AE_HIDDEN | AE_DISABLED | AE_FREE)); }

	mchArcaneScreenElement(void);
	~mchArcaneScreenElement(void);
};

enum arcStatEvents {
	AE_SET_BLINK_ON,
	AE_SET_BLINK_OFF,
	AE_SET_ROTATE_ON,
	AE_SET_ROTATE_OFF,
	AE_REMOVE,
	AE_CRITICAL_EVENT,

	AE_MAX_STAT_EVENT
};

//  mchArcaneMenuElement::arcaneType...
#define MCH_AM_RECALL			3
#define MCH_AM_UNIQUE			0
#define MCH_AM_TIME			1
#define MCH_AM_BONUS			2

#define AE_STATS_MAX	8
struct mchArcaneStatsDispatcher
{
	int X;
	int Y;

	int statusFlags[5];
	int racerIDs[5];

	mchArcaneScreenElement** data;

	void Redraw(void);
	void InitRedraw(void);

	void Add(int arcID);
	void Add(int arcID,int objID,int dataType = MCH_AM_UNIQUE,int charge = 0,int col = 2);
	void Remove(int objID);
	void HandleEvent(int objID,int ev_code,void* p = NULL);
	void Clear(void);

	void ScaleCoords(struct mchGameWindow* src,struct mchGameWindow* dest);
	void Scale(float sc);

	void GetCoords(int id,int& x,int& y);

	mchArcaneStatsDispatcher(void);
	~mchArcaneStatsDispatcher(void);
};

//  mchArcaneMenuElement::flags...
#define MCH_AM_DISABLED		0x01

struct mchArcaneMenuElement
{
	int ID;

	int flags;
	int color;

	float PosX;
	float PosY;

	int SizeX;
	int SizeY;

	float scale;

	int arcaneID;
	int arcaneChar;
	int arcaneType;
	int arcaneSeed;
	int arcaneSeedMax;
	unsigned char* arcaneName;

	void redraw(int x,int y,int alpha0 = 0,int rflag = 0);
	int check_coords(int x,int y){
		if(x >= PosX && x < PosX + SizeX && y >= PosY && y < PosY + SizeY) return 1;
		return 0;
	};

	void* list;
	mchArcaneMenuElement * prev;
	mchArcaneMenuElement* next;

	void Enable(void){ color = 1; flags &= ~(AE_DISABLED | AE_HIDDEN | AE_NO_REDRAW); }
	void Disable(void){ color = 0; flags |= AE_DISABLED; }

	mchArcaneMenuElement(void);
	~mchArcaneMenuElement(void);
};

struct mchArcaneMenu : public mchArcaneScreenElement
{
	xtList<mchArcaneMenuElement>* optionsLst;

	mchArcaneStatsDispatcher* statD;

	virtual void InitRedraw(void);
	virtual void MouseHandler(int bt);
	virtual void RedrawFnc(int x,int y);

	virtual void KeyHandler(int id);
	void SetArcane(mchArcaneMenuElement* p);

	void Build(void);

	void addElement(int type,int id);
	void removeElement(int type,int id);

	void ChangeState(void);
	void SetState(int st);

	void Scale(float sc);

	mchArcaneMenu(void);
	~mchArcaneMenu(void);
};

struct mchArcaneRoundMenu : public mchArcaneMenu
{
	int R_;
	int Phase;

	unsigned char* statusStr;

	mchArcaneMenuElement* curEl;
	mchArcaneMenuElement* prevEl;
	mchArcaneMenuElement* mouseEl;

	mchArcaneScreenElement* textEl;

	virtual void InitRedraw(void);
	virtual void RedrawFnc(int x,int y);
	virtual void KeyHandler(int id){ Change(id); }
	virtual void MouseHandler(int bt);
	virtual void Quant(float dt);
	virtual void SetState(int st);

	void SetTextEl(mchArcaneScreenElement* p);

	void Change(void);
	void Change(int id);
	void Set(int id);

	int CheckMouse(void);

	mchArcaneRoundMenu(int tp);
	~mchArcaneRoundMenu(void);
};

enum mchA_ShowActions 
{
	AE_INIT_FINISH_SCREEN = 1,
	AE_FINIT_FINISH_SCREEN,
	AE_EXIT_FINISH_SCREEN,
	AE_FINIT_TRACK_DEMO
};

struct mchArcaneScreenDispatcher
{
	float phase;
	float dest_phase;

	int showMode;
	int showAction;
	int phaseIndex;
	int phaseIndexMax;

	int Visible;

	int screenR;
	int screenG;
	int screenB;
	int screenA;

	int screenR_dest;
	int screenG_dest;
	int screenB_dest;
	int screenA_dest;

	mchArcaneScreenElement* activeEl;

	mchArcaneRacerSet* playerSet1;
	mchArcaneRacerSet* playerSet2;

	xtList<mchArcaneScreenElement>* objList;
	mchFlushLine* flushLine;

	void Quant(void);
	void TimerQuant(void);

	void GetFon(void);
	void PutFon(void);

	void Redraw(void);
	void InitRedraw(void);
	void ToggleVisible(int v = -1);

	void MouseHandler(int bt);
	void KeyHandler(int bt,int id);

	void SetFlush(int x,int y,int sx,int sy);
	void Flush(void);

	void EndGame(void);

	void InitStartupScreen(void);
	void InitEndGameScreen(void);
	void InitRacersFinishScreen(void);
	void InitTurnBasedScreen(void);
	void InitChoiceScreen(void);

	void InitTrackDemoScreen(void);
	void FinitTrackDemoScreen(void);

	void SetPhase(int ph);
	void SetDestPhase(int ph);
	void AdvancePhase(void);
	int CheckBuildLevel(void);

	void SetActiveEl(int id){ activeEl = objList -> search(id); }
	void SetLastEl(int tp);

	mchArcaneScreenElement* getEl_Type(int tp);
	mchArcaneScreenElement* getEl_ID(int id);

	void insert(mchArcaneRacerSet* p);
	void remove(mchArcaneRacerSet* p);

	void prepare(void);

	mchArcaneScreenDispatcher(void);
	~mchArcaneScreenDispatcher(void);
};

extern mchArcaneMenu* mch_AMenu;
extern mchArcaneRoundMenu* mch_CMenu;
extern mchArcaneRoundMenu* mch_SMenu;
extern mchArcaneScreenDispatcher* mch_arcScrD;
extern mchArcaneStatsDispatcher* mch_arcStatD;
extern mchArcaneScreenElement* mchA_StatusStrEl;
extern mchArcaneScreenElement* mchA_EndGameEl;

extern int mchA_TimerMax;
extern int mchA_TimerCur;
extern int mchA_TimerStart;

void mchA_SetTimer(int tm);
void mchA_LoadWorldMap(int m);
void mchA_SetWorldMapCoords(void);
void mchA_StopTimeHandler(void);
void mchA_StartTimeHandler(void);

void mchA_HandleStatsEvent(int objID,int ev_code,void* p = NULL,int n = 0);
void mchA_AddArcaneStats(int arcID,int objID,int n = 0);

void mchInitArcaneScreen(void);
void mchA_CHQuant(int k);

enum mchA_GammaSeqModes 
{
	GS_SHOW_LOADING_IMAGE,
	GS_HIDE_LOADING_IMAGE,
	GS_SHOW_WORLD,
	GS_HIDE_WORLD
};

void mchA_SetGammaSeq(int dir,int mode);
void mchA_EndGammaSeq(void);
void mchA_GammaSeqQuant(void);
int mchA_isGammaSeqActive(void);
int mchA_GetGammaSeqMode(void);

void mchA_SetTurnFlag(int fl = 1,int id = -1);

#endif // __ARCANE_MENU_H__