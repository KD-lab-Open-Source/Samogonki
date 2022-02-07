
#ifndef __ACI_SCR_H__
#define __ACI_SCR_H__

//#define _ACS_MOVE_OBJECTS_
#include "ACI_EVNT.H"
#include "hbm.h"

// aciScreenResource::type values...
#define ACS_NONE	0
#define ACS_BMP 	1
#define ACS_BML 	2
#define ACS_BMO 	3
#define ACS_XBM 	4
#define ACS_PAL 	5
#define ACS_HBM 	6

// aciScreenResource::flags...
#define ACS_LOADED		0x01
#define ACS_NO_FLUSH		0x02
#define ACS_BUILD_RESOURCE	0x04
#define ACS_BML_FILE		0x08
#define ACS_TRUECOLOR		0x10
#define ACS_MEMORY_FLAG		0x20

struct aciScreenResource : XListElement
{
	int ID;
	int type;
	int flags;

	char* fname;

	int palID;
	aciScreenResource* palPtr;

	char* dataPtr;

	void set_name(char* p);

	virtual void load(void){ };
	virtual void free(void){ if(dataPtr) delete dataPtr; dataPtr = NULL; }

	virtual void change_coords(int dx,int dy){ };
	virtual void redraw(int x,int y,int frame,int mode){ };

	virtual void buildData(void){ };

	virtual unsigned getColor(int x,int y,int frame = 0){ return 1; };

	aciScreenResource(void);
	~aciScreenResource(void);
};

struct aciScreenResourcePAL : aciScreenResource
{
	unsigned char* data;
	unsigned* data16;

	void prepare(void);

	void load(void);
	void free(void);

	aciScreenResourcePAL(void);
	~aciScreenResourcePAL(void);
};

struct aciScreenResourceBMP : aciScreenResource
{
	int SizeX;
	int SizeY;

	unsigned char* data;

	virtual void load(void);
	virtual void free(void);

	virtual void redraw(int x,int y,int frame,int mode);
	virtual unsigned getColor(int x,int y,int frame = 0);

	aciScreenResourceBMP(void);
	~aciScreenResourceBMP(void);
};

struct aciScreenResourceHBM : aciScreenResource, hbm_Image
{
	virtual void load(void);
	virtual void free(void);

	virtual void redraw(int x,int y,int frame,int mode);
	virtual unsigned getColor(int x,int y,int frame = 0);

	virtual void change_coords(int dx,int dy){ X += dx; Y += dy; };

	aciScreenResourceHBM(void);
	~aciScreenResourceHBM(void);
};

struct aciScreenResourceBML : aciScreenResource
{
	int SizeX;
	int SizeY;
	int NumFrames;

	unsigned char* data;
	unsigned char** frameTable;

	virtual void load(void);
	virtual void free(void);

	virtual void redraw(int x,int y,int frame,int mode);
	virtual void buildData(void);
	virtual unsigned getColor(int x,int y,int frame = 0);

	aciScreenResourceBML(void);
	~aciScreenResourceBML(void);
};

struct aciScreenResourceBMO : aciScreenResource
{
	int SizeX;
	int SizeY;
	int NumFrames;

	int OffsX;
	int OffsY;

	unsigned char* data;
	unsigned char** frameTable;

	virtual void load(void);
	virtual void free(void);

	virtual void change_coords(int dx,int dy){ OffsX += dx; OffsY += dy; }
	virtual void redraw(int x,int y,int frame,int mode);
	virtual void buildData(void);
	virtual unsigned getColor(int x,int y,int frame = 0);

	aciScreenResourceBMO(void);
	~aciScreenResourceBMO(void);
};

struct aciScreenResourceXBM : aciScreenResource
{
	int PosX;
	int PosY;

	int CenterX;
	int CenterY;

	int bSizeX;
	int bSizeY;

	int SizeX;
	int SizeY;

	int SideX;
	int SideY;

	int ImageSize;
	unsigned char* data;

	virtual void load(void);
	virtual void free(void);

	virtual void change_coords(int dx,int dy){ PosX += dx; PosY += dy; }
	virtual void redraw(int x,int y,int frame,int mode);

	aciScreenResourceXBM(void);
	~aciScreenResourceXBM(void);
};

struct aciScreenKey : XListElement
{
	int code_type;
	int code;
};

struct aciScreenKeyObject
{
	XList* codes;

	int KeyTrap(int code, int opt_code = 0);
	void AddKey(int code, int opt_flag = 0);

	aciScreenKeyObject(void);
	~aciScreenKeyObject(void);
};

// aciScreenEventCommand::flags...
#define ACS_COMMAND_STARTED	0x01

struct aciScreenEventCommand : XListElement
{
	int flags;
	int code;
	int StartTimer;
	int data[4];

	void init(void);
	aciScreenEventCommand(void);
};

// aciScreenEvent::flags...
#define ACS_EVENT_ACTIVE	0x01

struct aciScreenEvent : XListElement
{
	int flags;

	int CurTimer;
	int MaxTimer;

	XList* commSeq;
	aciScreenKeyObject* keyObj;

	void AddCommand(aciScreenEventCommand* p);
	void AddKey(int code, int opt_code = 0){ keyObj -> AddKey(code, opt_code); }

	int KeyTrap(int code, int opt_code = 0){ return keyObj -> KeyTrap(code, opt_code); }

	void Start(void);
	void Stop(void);

	void Quant(void);

	aciScreenEvent(void);
	~aciScreenEvent(void);
};

struct aciScreenFrameSequence : XListElement
{
	int ID;
	int ResourceID;

	int StartFrame;
	int EndFrame;
	int CurFrame;

	int FrameDelta;

	void reset(void){ CurFrame = StartFrame; }

	aciScreenFrameSequence(void);
};

#define ACS_EVENT_LINE_SIZE	100

struct aciScreenEventLine
{
	int size;
	int first_index;
	int last_index;
	int flags;

	aciScreenEventCommand** table;
	char* mem_heap;

	void clear(void);
	void put(int cd,int dt0 = 0,int dt1 = 0,int dt2 = 0,int dt3 = 0);
	aciScreenEventCommand* get(void);

	aciScreenEventLine(void);
	~aciScreenEventLine(void);
};

// aciScreenObject:type values...
#define ACS_BASE_OBJ		0x01
#define ACS_INPUT_FIELD_OBJ	0x02
#define ACS_SCROLLER_OBJ	0x03

// aciScreenObject::flags...
#define ACS_REDRAW_OBJECT	0x01
#define ACS_ISCREEN_FONT	0x02
#define ACS_ALIGN_CENTER	0x04
#define ACS_ACTIVE_STRING	0x08
#define ACS_BACK_RES		0x10
#define ACS_BLACK_FON		0x20		
#define ACS_MOVED_OBJECT	0x40
#define ACS_ALPHA_MASK		0x80
#define ACS_TEXT			0x100
#define ACS_BLOCKED			0x200
#define ACS_HIDDEN			0x400
#define ACS_MOUSE_SELECT	0x800

struct aciScreenObject : XListElement
{
	//@caiiiycuk
	int objIndex[4];
	int align_x, align_y;
	int PosX0, PosY0, SizeX0, SizeY0;

	int ID;
	int type;

	int PosX;
	int PosY;

	int SizeX;
	int SizeY;


	int flags;

	int curFrame;

	int fonResID;
	int defResID;
	int ResID;
	aciScreenResource* resPtr;

	int PalID;
	aciScreenResource* palPtr;

	aciScreenFrameSequence* activeSeq;

	XList* frameSeq;
	XList* events;

	virtual void init(void);
	virtual void redraw(int flag = 0);
	virtual void Quant(int mx = 0, int my = 0);

	virtual void change_coords(int dx,int dy){ PosX += dx; PosY += dy; }
	virtual int CheckXY(int x,int y);

	int isActive(void);

	void prepare(void);
	void StopEvents(void);
	void EventQuant(void);

	void AddSeq(aciScreenFrameSequence* p);
	void AddEvent(aciScreenEvent* p);

	void set_redraw(void){ flags |= ACS_REDRAW_OBJECT; }

	aciScreenFrameSequence* GetSequence(int id);

#ifdef _ACS_MOVE_OBJECTS_
	void show_bound(void);
#endif

	virtual void AdjustSize(void) {
		//@caiiiycuk?
	};
	virtual void scale_coords(float x,float y);
	virtual void Scale(float dx,float dy) {
		if (PosX0 == -1) {
			PosX0 = PosX;
			PosY0 = PosY;
			SizeX0 = SizeX;
			SizeY0 = SizeY;
		}
		PosX = PosX0 * dx;
		PosY = PosY0 * dy;
		SizeX = SizeX0 * dx;
		SizeY = SizeY0 * dy;
	}

	aciScreenObject(void);
	~aciScreenObject(void);
};

struct aciScreenInputField : aciScreenObject
{
	//@caiiiycuk
	//int SizeX, SizeY, PosX, PosY;

	char* string;
	int MaxStrLen;

	int font;
	int Color;
	int Space;

	int Color0;
	int Color1;
	int Color2;

	int Alpha0;
	int Alpha1;
	int Alpha2;

	int AlphaCur;
	int AlphaDest;
	int dAlpha;

	int MaxState;
	int CurState;
	char** statePtr;

	virtual void redraw(int flag2d);
	virtual void init(void);
	void alloc_str(void);

	void SetAlpha(int v);

	void change_state(void);
	int get_state(void);
	void set_state(int v);

	void inc_state(void);
	void dec_state(void);

	virtual void Quant(int mx, int my);
	virtual void AdjustSize(void);

	void set_string(char* p){ strcpy(string,p); }

	aciScreenInputField(void);
	~aciScreenInputField(void);
};

struct aciScreenScroller : aciScreenObject
{
	int CurValue;
	int MaxValue;

	int Color;
	int BackColor;

	int AlphaCur;
	int AlphaDest;
	int dAlpha;

	int Alpha0;
	int Alpha1;

	int ScrResID;
	aciScreenResource* ScrRes;

	int sResID;
	aciScreenResource* sRes;

	int ScrResSX;
	int ScrResSY;

	int sResSX;
	int sResSY;

	int ActiveSX;
	int ActiveSY;

	virtual void init(void);
	virtual void redraw(int flag2d);
	virtual int CheckXY(int x,int y);
	virtual void Quant(int mx, int my);

	void SetAlpha(int v);

	void Change(int x);

	aciScreenScroller(void);
	~aciScreenScroller(void);
};

struct aciScreen : XListElement
{
	int ID;
	int flags;

	int nextID;
	int prevID;

	XList* events;
	XList* objList;

	XList* resources;

	int backResID;
	aciScreenResource* backRes;

	void AddEvent(aciScreenEvent* p);
	void AddObject(aciScreenObject* p);
	void AddResource(aciScreenResource* p);

	void ChangeCoords(int dx,int dy);

	aciScreenObject* GetObject(int id);
	aciScreenResource* GetResource(int id);

	void alloc_mem(void);
	void free_mem(void);

	int redraw(int force = 0, int flag2d = 0);

	void prepare(void);
	void init_objects(void);

	void KeyTrap(int code);
	void EventQuant(void);
	virtual void CenterMouse(void);
	virtual aciScreenObject* GetObjectXY(int x,int y,int dir,int coords_mode = 0);
	virtual void ScaleCoords(float dx,float dy);
	virtual void Scale(float dx,float dy);

	aciScreen(void);
	~aciScreen(void);
};

// aciScreenDispatcher::flags...
#define ACS_FORCED_REDRAW	0x01
#define ACS_FIRST_REDRAW	0x02
#define ACS_NEED_EXIT		0x04
#define ACS_FULLSCR_FLUSH	0x08
#define ACS_FULL_REDRAW		0x10

struct aciScreenDispatcher
{
	//@caiiiycuk
	int nextScrID;
	aciScreen* nextScr;

	int flags;
	int QuantCode;

	XList* scrList;

	int curScrID;
	aciScreen* curScr;

	XList* resources;

	aciScreenInputField* activeInput;

	aciScreenEventLine* eventLine;
	aciScreenEventLine* flushLine;

	void AddScreen(aciScreen* p);
	void AddResource(aciScreenResource* p);

	aciScreen* GetScreen(int id);
	aciScreenObject* GetObject(int id);
	aciScreenResource* GetResource(int id);

	void SetNextScrID(int scr_id,int id);
	void SetPrevScrID(int scr_id,int id);

	void KeyTrap(int code);

	void alloc_mem(void);
	void free_mem(void);

	int Quant();
	void EventQuant(void);
	int ObjectQuant(int mx = 0, int my = 0);
	void FlushEvents(void);

	int redraw(void);
	void flush(void);

	void PrepareInput(int obj_id);
	void InputQuant(int code,int chr);
	void CancelInput(void);
	void DoneInput(void);

	void ChangeCoords(int dx,int dy);

	void SetResource(int obj_id,int res_id,int fr);
	void SetSequence(int obj_id,int seq_id);
	void ChangeScroller(int obj_id,int x);
	void SetScroller(int obj_id,int v,int max_v);

	int GetObjectValue(int obj_id,int flag = 0);

	void SetFlush(void){ flags |= ACS_FULLSCR_FLUSH; }

	void ChangeScreen(int id);
	void PrepareChangeScreen(int id);
	virtual void DecScroller(int obj_id);
	virtual void IncScroller(int obj_id);
	virtual void RedrawQuant(int flush_log = 0);
	virtual void ActiveInputQuant(void);
	virtual void Scale(float dx,float dy);
	virtual void ScaleCoords(float dx,float dy);
	virtual void MouseQuant(void);
#ifdef _ACS_MOVE_OBJECTS_
	void DumpMovedObjects(void);
	aciScreenObject* GetMouseObject(void);
#endif

	aciScreenDispatcher(void);
	~aciScreenDispatcher(void);
};

extern aciScreenDispatcher* acsScrD;
extern int acsLowMemFlag;

aciScreenResource* acsAllocResource(int type);
void acsFreeResource(aciScreenResource* p);

aciScreenObject* acsAllocObject(int type);
void acsFreeObject(aciScreenObject* p);

void acsOutText16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space);
int acsTextStrLenMax(int fnt,unsigned char* str,int space);
int acsTextHeight(int fnt,unsigned char* str,int space);

//@caiiiycuk
struct acsTimer {
float ValueCur, ValueStart;
float dValue, dValueCur;
float period;

acsTimer(float st,float pr,float d);
void Quant(float dt);
float Value(void);
float Phase(void);
void acsShowSlotInfo(void);
};


#endif	// __ACI_SCR_H__

