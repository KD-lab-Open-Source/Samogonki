
#ifndef __MCH_RTO_H__
#define __MCH_RTO_H__

enum xgrModes 
{
	MCH_320x200 = 0,		
	MCH_640x480,			// 1
	MCH_800x600 ,			// 2
	MCH_1024x768,			// 3
	MCH_1152x864,			// 4
	MCH_1280x1024,			// 5
	MCH_1600x1200,			// 6

	MCH_MAX_GRAPH_MODE
};


const int RTO_GAME_QUANT_TIMER	= 0;		// 1000/25;
const int RTO_MAIN_MENU_TIMER	= 1000/30;
const int RTO_IMAGE_TIMER		= 1000/30;
const int RTO_LOADING_TIMER		= 1000/30;
const int RTO_INTRO_MOVIE_TIMER	= 0; // 1000/30;

// Runtime objects...
enum rtoID
{
	RTO_GAME_QUANT_ID  = 1,
	RTO_MAIN_MENU_ID,		// 2
	RTO_SHOW_IMAGE_ID,		// 3
	RTO_LOADING_ID,			// 4
	RTO_INTRO_MOVIE_ID,		// 5

	RTO_MAX_ID
};

struct BaseRTO : XRuntimeObject
{
	int flags;
	int NextID;

	void SetNext(int id){ NextID = id; }
	void SetFlag(int fl){ flags |= fl; }
	void ClearFlag(int fl){ flags &= ~fl; }

	BaseRTO(void){ NextID = XT_TERMINATE_ID; flags = 0; }
};

struct GameQuantRTO : BaseRTO
{
	virtual void Init(int id);
	virtual int Quant(void);
	virtual void Finit(void);

	GameQuantRTO(void){ ID = RTO_GAME_QUANT_ID; Timer = RTO_GAME_QUANT_TIMER; }
};

struct MainMenuRTO : BaseRTO
{
	virtual void Init(int id);
	virtual int Quant(void);
	virtual void Finit(void);

	MainMenuRTO(void){ ID = RTO_MAIN_MENU_ID; Timer = RTO_MAIN_MENU_TIMER; }
};

// ShowImageRTO flags...
#define IMG_RTO_NO_IMAGE		0x01
#define IMG_RTO_TEXT			0x02
#define IMG_RTO_PALETTE			0x04
#define IMG_RTO_AVI			0x08
#define IMG_RTO_AVI_FULLSCREEN		0x10
#define IMG_RTO_AVI_PLAY_ONCE		0x20
#define IMG_RTO_INTRO_IMAGE		0x40
#define IMG_RTO_START_MUSIC		0x80
#define IMG_RTO_SCROLL_TEXT		0x100
#define IMG_RTO_NO_DISABLE		0x200

#define RTO_SHOW_IMAGE_TIME	10000	
#define RTO_SHOW_IMAGE_MAX	5
struct ShowImageRTO : BaseRTO
{
	int count;
	int numFiles;
	int curFile;

	int gammaTimerBeg;
	int gammaDir;

	int txtScrollY;
	float txtScrollDelta;

	int WaitTimer;

	int Flags[RTO_SHOW_IMAGE_MAX];
	int fntData[RTO_SHOW_IMAGE_MAX];
	int spaceData[RTO_SHOW_IMAGE_MAX];
	int colData[RTO_SHOW_IMAGE_MAX];

	char** fileNames;
	char** palNames;
	const char** textPtr;

	int imageSX;
	int imageSY;
	void* imageData;

	virtual void Init(int id);
	virtual int Quant(void);
	virtual void Finit(void);

	void SetNumFiles(int num);
	void SetName(const char* p,int id = 0){ strcpy(fileNames[id],p); count = 0; Flags[id] = 0; }
	void SetPalette(const char* p,int id = 0){ strcpy(palNames[id],p); Flags[id] |= IMG_RTO_PALETTE; }
	void SetText(const char* p,int fnt,int sp,int col,int id = 0){ textPtr[id] = p; fntData[id] = fnt; spaceData[id] = sp; colData[id] = col; Flags[id] |= IMG_RTO_TEXT; }

	void SetGammaSeq(int dir);

	void SetFlag(int id,int fl){ Flags[id] |= fl; }
	void DropFlag(int id,int fl){ Flags[id] &= ~fl; }
	void ClearFlag(int id){ Flags[id] = 0; }

	ShowImageRTO(void);
};

struct LoadingRTO : BaseRTO
{
	int startTimer;

	virtual void Init(int id);
	virtual int Quant(void);
	virtual void Finit(void);

	LoadingRTO(void){ ID = RTO_LOADING_ID; Timer = RTO_LOADING_TIMER; }
};

struct IntroMovieRTO : BaseRTO
{
	virtual void Init(int id);
	virtual int Quant(void);
	virtual void Finit(void);

	IntroMovieRTO(void){ ID = RTO_INTRO_MOVIE_ID; Timer = RTO_INTRO_MOVIE_TIMER; }
};

#endif // __MCH_RTO_H__


