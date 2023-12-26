
#ifndef	__I_WORLD_H__
#define __I_WORLD_H__

#if defined(GPX) and defined(EMSCRIPTEN)
#include <emscripten.h>
#endif

#include "I-WorldEvent.h"
#include "doggy.h"

class iwBody 
{
	float x;
	float v;

	float dx;

	float calc_force(void);

public:
	void reset(void){ x = v = 0.0f; }
	void init(float _dx);

	void quant(float dt);
	float X(void) const { return x; }

	iwBody(void);
	~iwBody(void);
};

class iwDoggyPhrase 
{
	int flags;

	int priority;
	char* message;

public:
	int ID;
	void* list;
	iwDoggyPhrase* prev;
	iwDoggyPhrase* next;

	char* Message(void) const { return message; }
	void SetMessage(char* msg);

	void SetPriority(int pr){ priority = pr; }
	int Priority(void) const { return priority; }

	void SetFlag(int f){ flags |= f; }
	void DropFlag(int f){ flags &= ~f; }
	void ToggleFlag(int f){ flags ^= f; }
	void ClearFlags(void){ flags = 0; }

	int CheckFlag(int f) const { return (flags & f); }

	int Flags(void) const { return flags; }

	void Init(struct scrDataBlock* p);

	iwDoggyPhrase(void);
	~iwDoggyPhrase(void);
};

const int IW_DOGGY_MAX_PHRASE	= 10;

class iwDoggyPhraseQueue
{
	int length;
	iwDoggyPhrase* queue[IW_DOGGY_MAX_PHRASE];
public:
	void Put(iwDoggyPhrase* p);
	iwDoggyPhrase* Get(void);
	void Remove(void);
	void Sort(void);

	void Clear(void){ length = 0; }
	int NewMessages(void) const;

	iwDoggyPhraseQueue(void){ length = 0; }
	~iwDoggyPhraseQueue(void){ };
};

struct iwPlace
{
	int ID;

	int X;
	int Y;
	int angle;

	void* list;
	iwPlace* prev;
	iwPlace* next;

	void read(XBuffer& b)
	{ 
		b >= X >= Y >= angle >= ID;
	}

	iwPlace(void){ list = NULL; }
};

struct iwTeleport
{
	int PosX;
	int PosY;

	int World;
	int Track;

	cMesh* arrow;

	void* list;
	iwTeleport* prev;
	iwTeleport* next;

	void read(XBuffer& b)
	{ 
		b >= PosX >= PosY;
		b >= World >= Track;
	}

	iwTeleport(void){ list = NULL; arrow = NULL; }
};

enum iwSensors {
	IW_ELECTIONARY_SENSOR = 1, 
	IW_ASSEMBLY_SENSOR,		// 2
	IW_CREDITS_SENSOR,		// 3
	IW_DOGGY_SENSOR			// 4
};

enum iWorldModes 
{
	IW_FREE,	
	IW_ELECTIONARY,
	IW_ASSEMBLY,
	IW_TELEPORT,
	IW_ADVISOR,

	IW_MODE_MAX
};

// iWorldDispatcher::flags
const int IW_ELECTIONARY_FLAG	= 0x01;
const int IW_FIRST_QUANT_FLAG	= 0x02;
const int IW_CANCEL_INPUT_FLAG	= 0x04;
const int IW_FIRST_ELECTIONARY_FLAG	= 0x08;
const int IW_TURNBASED_FLAG		= 0x10;
const int IW_INIT_ELECTIONARY_FLAG	= 0x20;
const int IW_DISABLED_FLAG		= 0x40;
const int IW_EXIT_FLAG		= 0x80;
const int IW_DOGGY_MODE_FLAG	= 0x100;
const int IW_EXIT_TELEPORT_FLAG	= 0x200;

const int IW_MAX_MOVIE		= 5;

class iWorldDispatcher 
{
	int ID;

	int mode;
	int flags;

	struct KeyBuffer* keyBuf;

	struct mchGameWindow* viewport;
	struct mchRacer* owner;

	int curMovie;
	int curMovieSize;
	const char* movies[IW_MAX_MOVIE];

	// Advisor
	iwDoggyPhrase* curPhrase;
	iwDoggyPhraseQueue phrases;

	// World select
	int nextWorld;
	int nextTrack;
	int nextTrackStatus;
	int nextTrackPrice;

	Vect3f teleportR;

	// Electionary
	int electionaryScrID;
	int nameScrID;
	int curFigure;
	int numActiveFigures;
	class cMesh** figures;
	int figureBackup;

	// Assembly
	int curPart;
	int curPartType;
	int partSize[4];
	class cMesh** parts[4];
	int configBackup[4];

	xtList<iwTeleport> tpLst;

	int lastPlace;
	xtList<iwPlace> plLst;

	float curScreenPhase;
	float destScreenPhase;
	struct aciScreen* curScreen;

	char backupStr[256];
	struct aciScreenInputField* activeInput;

	void draw_iScreen(struct aciScreen* scr);
	void set_iScreen(int id);

	void draw_AdvisorScreen(void);
	void draw_DisabledScreen(void);

	void prepareTeleportScreen(void);
	void prepareTeleport2Screen(void);
	void draw_TeleportScreen(void);
	void draw_Teleport2Screen(void);
	void TeleportQuant(void);

	void PrepareInput(int obj_id);
	void InputQuant(int code,int chr);
	void CancelInput(void);
	void DoneInput(void);
	void ActiveInputQuant(void);

	void SetElectionaryCamera(void);
	void SetAssemblyCamera(void);

	void draw_AssemblyScreen(void);
	void AssemblyQuant(void);
	void AssemblyPartsQuant(int mode = 1);
	void AssemblyCoordsQuant(void);

	void draw_ElectionaryScreen(void);
	void ElectionaryQuant(void);

	void ChangePart(int dir);
	void SetCurPart(void);
	void SetCurPartType(int tp);

	void SetFigureAnimation(int id,int mode);
	int CheckFigureName(void);

	void SetLastPlace(int id){ lastPlace = id; }
	void Move2LastPlace(int rev = 0);

	void BackupElectionary(void);
	void RestoreElectionary(void);

	void BackupAssembly(void);
	void RestoreAssembly(void);

	int GetY0(void) const;
	int GetScrY(void) const;
	void ScaleScreen(struct aciScreen* scr);

	void HideObjects(int id,int del_mode);
	void ShowObjects(int id);

	void HideAssembly(void);
	void ShowAssembly(void);

	void HideElectionary(void);
	void ShowElectionary(void);

	iwTeleport* getTeleport(int wr,int tr);

public:
	void CreateTeleports(void);
	void CreatePlaces(void);
	int NumPlaces(void){ return plLst.size(); }

	void InitFigures(void);

	void InitElectionary(void);
	void FinitElectionary(void);

	void InitDoggy(int get_mode = 1);
	void FinitDoggy(void);

	void NextFigure(void);
	void PrevFigure(void);
	void ChooseFigure(void);
	void InitFigureName(void);

	void InitAssembly(void);
	void FinitAssembly(void);

	void Quant(void);
	void Redraw(void);

	void KeyTrap(int code);
	void KeyQuant(void);

	void handleEvent(int code,int data0 = 0,int data1 = 0);

	int Input(void) const { if(activeInput) return 1; return 0; }

	int ElectionaryMode(void) const { return (mode == IW_ELECTIONARY); }
	int AssemblyMode(void) const { return (mode == IW_ASSEMBLY); }
	int TeleportMode(void) const { return (mode == IW_TELEPORT); }
	int TurnbasedMode(void) const { return (flags & IW_TURNBASED_FLAG); }
	int AdvisorMode(void) const { return (mode == IW_ADVISOR); }

	void InitWorld(void);
	void InitTeleports(int mode = 1);

	int NextWorld(void) const { return nextWorld; }
	int NextTrack(void) const { return nextTrack; }

	void LastPosition(class Vect3f& r,int& angle);
	struct aciScreen* CurScreen(void) const { return curScreen; }
	int CurScreenPhase(void) const { return curScreenPhase; }

	void Save(XStream& fh);
	void Load(XBuffer& xb,int ver);

	void SetOwner(struct mchRacer* p);
	struct mchRacer* GetOwner(void) const { return owner; }

#if defined(GPX) and defined(EMSCRIPTEN)
    void updateLayers(int f) {
        if (f == 0x10 /* IW_TURNBASED_FLAG */) {
            EM_ASM(({
                        if (Module.layers) {
                            Module.layers.setTurnBasedFlag($0 === 1);
                        }
                    }), (flags & f) ? 1 : 0);
        }
    }
#endif
	void SetFlag(int f) {
        flags |= f;
#if defined(GPX) and defined(EMSCRIPTEN)
        updateLayers(f);
#endif
    }
	void DropFlag(int f) {
        flags &= ~f;
#if defined(GPX) and defined(EMSCRIPTEN)
        updateLayers(f);
#endif
    }
	void ToggleFlag(int f) {
        flags ^= f;
#if defined(GPX) and defined(EMSCRIPTEN)
        updateLayers(f);
#endif
    }
	int CheckFlag(int f) const { return (flags & f); }

	void Disable(void){ flags |= IW_DISABLED_FLAG; }
	void Enable(void){ flags &= ~IW_DISABLED_FLAG; }
	int Disabled(void) const { return (flags & IW_DISABLED_FLAG); }

	int isScreenSelected(void) const { if(curScreen) return 1; return 0; }
	int isInInputMode(void) const { if(activeInput) return 1; return 0; }

	void PutKeys(struct KeyBuffer* p);

	void PutPhrase(int id);
	void ClearPhrases(void);
	void ResetPhrases(void);

	void ShowNewPhrase(void);

	void SetMovie(const char* m);
	void SetMovie(int op_mode);
	void SetEndMovie(void);
	const char* GetMovie(void){ if(curMovieSize > curMovie) return movies[curMovie ++]; return NULL; }
	void MovieQuant(void);
	int PostMovieQuant(void);

	void Clear(void);

	iwTeleport* getFirstTeleport(void){ return tpLst.first(); }

	iWorldDispatcher(int id);
	~iWorldDispatcher(void);
};

extern int iWorldMode;
extern iWorldDispatcher* iworld_D;
extern iWorldDispatcher* iworld_D2;

extern iWorldDispatcher* iworld_Cur;

void iwInitWorld(void);
void iwSelect(int n);
void iwDisable(int n);
void iwEnable(int n);

void iwKeyTrap(void);
void iwJoyKeyTrap(int k);

inline void iwHandleEvent(int code,int data0 = 0,int data1 = 0)
{
	if(iworld_Cur) iworld_Cur -> handleEvent(code,data0,data1);
}

void iwInit(void);

#endif	/* __I_WORLD_H__ */
