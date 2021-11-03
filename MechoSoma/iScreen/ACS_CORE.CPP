/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "HFONT.H"
#include "sound.h"

#include "ACI_EVNT.H"
#include "ACI_SCR.H"

#include "KEYS.H"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "I-World.h"

#include "XJoystick.h"

#include "XGR.h"
#include "IGraph3d.h"

#include "LocalVersion.h"

#include "mch_common.h" // For far target
#include "hbm.h"

#pragma warning( disable : 4305 )
#pragma warning( disable : 4244 )

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern float acsMouseMaxZ;
extern int acsMouseLPFlag;
extern int acsMouseRPFlag;
extern int mchResourcesFlag;

extern int acsActiveFlag;

extern cInterfaceGraph3d	*gb_IGraph3d;

extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];
extern unsigned mchA_ColorF[];

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int mchGetKeyID(int key,int config = -1);
int mchGetKeyID_First(int key,int config = -1);
int mchGetKeyID_Next(void);

char* mchGetKeyNameText(int code);

void mchA_d3dPrepare(void);
unsigned mch_toupper(unsigned ch);

Vect3f acsCalcR(Vect3f r0,float phase);
void acsPrepareChangeScreen3D(int scr_id);
float acsChangeScreenQuant3D(void);

void acsStrLen3D(void* str,int fnt,int space,int& sx,int& sy);
void acsOutRect3D(float x,float y,float z,int sx,int sy,int col,int alpha,int mat_fl = 0,int ch = 2,Mat3f mat = Mat3f::ZERO,float gamma = 1.0f);
void acsOutString3D(int x,int y,void* str,int fnt,int col,int space,int alpha,float z = 0.0f,float sc = 1.0f);

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);

int acsGetKeyName(int vkey,int shift,int lng_flag);
void ibsout(int x,int y,void* ptr);

int acsEventActive(aciScreenEventCommand* p);
int acsExtEventActive(aciScreenEventCommand* p);
int iwEventActive(aciScreenEventCommand* p);

void acsSendEvent(aciScreenEventCommand* p);
void acsSendEvent(int code,int data0,int data1,int data2,int data3);
void acsSetFlush(int x,int y,int sx,int sy);

aciScreenResource* acsGetResource(int id);
void acsGetResourceSize(aciScreenResource* p,int& x,int& y,int& sx,int& sy);
int acsGetMaxFrame(int res_id);

void acsOutStr(int x,int y,int fnt,int col,unsigned char* str,int space);
void acsOutStr16(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space);
void acsOutStr16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutStr16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
int acsStrLen(int fnt,unsigned char* str,int space);
void acsOutText16_a(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);
void acsOutText16_a2(int x,int y,int fnt,int col,unsigned char* str,unsigned* pal,int space,int alpha);

void acsRectangle(int x,int y,int sx,int sy,int col,unsigned* pal,int alpha);

void acsChangeStrState(int id);

void acsLoadFonts(void);
void acsFreeFonts(void);

void acsHandleExtEvent(int code,int data0 = 0,int data1 = 0,int data2 = 0);

void acsPrepareSprite(int sx,int sy,void* dst_buf,void* src_buf,void* pal);
void acsPutSpr16(int x,int y,int sx,int sy,void* p,void* pal,int mode);

void acsExtInit(void);
void acsExtQuant(void);
void acsExtRedraw(void);
void acsExtPostRedraw(void);
void acsExtRedrawD3D(void);
void acsExtPostRedrawD3D(void);

void acsSetAlphaSeq(int dest,int delta);
void acsAlphaQuant(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define _ACI_3D_VIEW_


#define aci_FREE_LIST(lst,p1,p2,tp)	p1 = (tp*)lst -> fPtr;		\
					while(p1){			\
						p2 = (tp*)p1 -> next;	\
						delete p1;		\
						p1 = p2;		\
					}				\
					lst -> ClearList();

// Frame ID...
#define ACS_MAX_FRAME		-1
#define ACS_MID_FRAME		-2

#define ACS_MAX_FONT		2
#define ACS_INPUT_TIMER 	3

const int ACS_DELTA_X	= (800 - 640) / 2;
const int ACS_DELTA_Y	= (600 - 480) / 2;

int acsInputTimer = 0;
int acsInputFlag = 0;

int acsFirstRedraw = 0;

int acsInputMode = 0;
char* acsBackupStr = NULL;
aciScreenDispatcher* acsScrD;

int acsNumFonts = 3;
char* acsFontDir = "FONTS";
char* acsFontPrefix = "font";
HFont** acsFntTable = NULL;

char* acsSTR_ON = "ON";
char* acsSTR_OFF = "OFF";

int acsHighColor = 1;

int acsLastKey = 0;
int acsCancelInputFlag = 0;

#ifdef _ACS_MOVE_OBJECTS_
aciScreenObject* acsMoveObj = NULL;
int aciShowBounds = 0;
#endif

int acsAlpha = 0;
int acsAlphaDest = 0;
int acsAlphaDelta = 0;
int acsAlphaData = -1;
int acsAlphaEvent = 0;

int acsDisableEvents = 0;
int acsLowMemFlag = 0;

float acsObjPhase = 1.0f;

float acsObjScalePhase = 0;
float acsObjScalePeriod = 20.0f;
float acsObjScaleDelta = 1.0f;

aciScreenObject* acsLastObj = NULL;
aciScreenObject* acsMouseObj = NULL;

float acsMouseSpeed = 20.0f;
int acsMouseTargetX = -1;
int acsMouseTargetY = -1;

int acsY0 = 0;

int acsEventActive(aciScreenEventCommand* p)
{
	aciScreenObject* obj;

	if(acsAlphaEvent) return 0;

	switch(p -> code){
		case ACS_EXEC_SEQUENCE:
			obj = acsScrD -> GetObject(p -> data[0]);
			if(obj -> activeSeq) return 0;
			break;
		case ACS_INPUT_STRING:
		case ACS_INPUT_KEYNAME:
			if(acsScrD -> activeInput) return 0;
			break;
		case ACS_EXT_EVENT:
			return acsExtEventActive(p);
		case ACS_IWORLD_EVENT:
			return iwEventActive(p);
		case ACS_ALPHA_SEQ:
			if(acsAlphaDelta) return 0;
			break;
		case ACS_CHANGE_SCREEN:
			if(acsAlphaDelta > 0) return 0;
			break;
	}
	return 1;
}

aciScreenResource* acsGetResource(int id)
{
	return acsScrD -> GetResource(id);
}

void acsSendEvent(aciScreenEventCommand* p)
{
	acsScrD -> eventLine -> put(p -> code,p -> data[0],p -> data[1],p -> data[2], p -> data[3]);
}

void acsSendEvent(int code,int data0,int data1,int data2,int data3)
{
	acsScrD -> eventLine -> put(code,data0,data1,data2,data3);
}

void acsSetFlush(int x,int y,int sx,int sy)
{
	acsScrD -> flushLine -> put(0,x,y,sx,sy);
}

aciScreenScroller::aciScreenScroller(void)
{
	type = ACS_SCROLLER_OBJ;

	CurValue = 0;
	MaxValue = 1;
	Color = BackColor = 0;

	AlphaCur = AlphaDest = dAlpha = Alpha0 = Alpha1 = -1;

	ScrResID = 0;
	ScrRes = NULL;

	sResID = 0;
	sRes = NULL;

	ActiveSX = ActiveSY = 0;

	ScrResSX = ScrResSY = NULL;
	sResSX = sResSY = NULL;
}

aciScreenScroller::~aciScreenScroller(void)
{
}

aciScreenInputField::aciScreenInputField(void)
{
	type = ACS_INPUT_FIELD_OBJ;

	MaxStrLen = 0;
	string = NULL;
	Color = 140;

#ifndef _ACI_3D_VIEW_
	Space = 1;
#else
	Space = 2;
#endif

	Color0 = 140;
	Color1 = -1;
	Color2 = -1;

	AlphaCur = -1;
	AlphaDest = -1;

	Alpha0 = Alpha1 = Alpha2 = -1;

	font = 0;

	MaxState = CurState = 0;
	statePtr = NULL;
}

aciScreenInputField::~aciScreenInputField(void)
{
	if(string) delete string;
}

aciScreen::aciScreen(void)
{
	ID = 0;
	flags = 0;

	nextID = prevID = -1;

	backResID = 0;
	backRes = NULL;

	events = new XList;
	objList = new XList;
	resources = new XList;
}

aciScreen::~aciScreen(void)
{
	aciScreenEvent* ev,*ev1;
	aciScreenObject* obj,*obj1;
	aciScreenResource* res,*res1;

	res = (aciScreenResource*)resources -> fPtr;
	while(res){
		res1 = (aciScreenResource*)res -> next;
		res -> free();
		acsFreeResource(res);
		res = res1;
	}
	delete resources;

	aci_FREE_LIST(events,ev,ev1,aciScreenEvent);
	delete events;

	obj = (aciScreenObject*)objList -> fPtr;
	while(obj){
		obj1 = (aciScreenObject*)obj -> next;
		acsFreeObject(obj);
		obj = obj1;
	}
	delete objList;
}

aciScreenDispatcher::aciScreenDispatcher(void)
{
	flags = 0;
	curScr = NULL;
	curScrID = 1;

	QuantCode = 0;

	activeInput = NULL;

	scrList = new XList;
	resources = new XList;

	eventLine = new aciScreenEventLine;
	flushLine = new aciScreenEventLine;
}

aciScreenDispatcher::~aciScreenDispatcher(void)
{
	aciScreen* p,*p1;
	aciScreenResource* res,*res1;

	res = (aciScreenResource*)resources -> fPtr;
	while(res){
		res1 = (aciScreenResource*)res -> next;
		res -> free();
		acsFreeResource(res);
		res = res1;
	}
	delete resources;

	aci_FREE_LIST(scrList,p,p1,aciScreen);
	delete scrList;

	delete eventLine;
	delete flushLine;
}

aciScreenEventLine::aciScreenEventLine(void)
{
	int i;
	table = new aciScreenEventCommand*[ACS_EVENT_LINE_SIZE];
	mem_heap = new char[ACS_EVENT_LINE_SIZE * sizeof(aciScreenEventCommand)];
	for(i = 0; i < ACS_EVENT_LINE_SIZE; i ++){
		table[i] = (aciScreenEventCommand*)(mem_heap + i * sizeof(aciScreenEventCommand));
		table[i] -> init();
	}
	clear();
}

aciScreenEventLine::~aciScreenEventLine(void)
{
	delete mem_heap;
	delete table;
	clear();
}

aciScreenEventCommand::aciScreenEventCommand(void)
{
	init();
}

aciScreenObject::aciScreenObject(void)
{
	int i;

	ID = 0;
	type = ACS_BASE_OBJ;

	for(i = 0; i < 4; i ++)
		objIndex[i] = -1;

	objIndex[0] = objIndex[2] = 0;

	PosX = PosY = -1;
	PosX0 = PosY0 = -1;
	SizeX = SizeY = -1;
	SizeX0 = SizeY0 = -1;

	align_x = align_y = -1;

	flags = 0;

	curFrame = 0;
	defResID = fonResID = ResID = -1;
	resPtr = NULL;

	PalID = -1;
	palPtr = NULL;

	frameSeq = new XList;
	events = new XList;
}

aciScreenObject::~aciScreenObject(void)
{
	aciScreenFrameSequence* s,*s1;
	aciScreenEvent* ev,*ev1;

	aci_FREE_LIST(frameSeq,s,s1,aciScreenFrameSequence);
	delete frameSeq;

	aci_FREE_LIST(events,ev,ev1,aciScreenEvent);
	delete events;
}

void aciScreenObject::init(void)
{
	palPtr = acsGetResource(PalID);
}

aciScreenFrameSequence::aciScreenFrameSequence(void)
{
	ID = 0;
	ResourceID = -1;
	StartFrame = EndFrame = CurFrame = FrameDelta = 0;
}

aciScreenEvent::aciScreenEvent(void)
{
	flags = 0;
	CurTimer = 0;
	MaxTimer = 1;

	commSeq = new XList;
	keyObj = new aciScreenKeyObject;
}

aciScreenEvent::~aciScreenEvent(void)
{
	aciScreenEventCommand* p,*p1;
	aci_FREE_LIST(commSeq,p,p1,aciScreenEventCommand);
	delete commSeq;
	delete keyObj;
}

aciScreenKeyObject::aciScreenKeyObject(void)
{
	codes = new XList;
}

aciScreenKeyObject::~aciScreenKeyObject(void)
{
	aciScreenKey* p,*p1;
	aci_FREE_LIST(codes,p,p1,aciScreenKey);
	delete codes;
}

aciScreenResource::aciScreenResource(void)
{
	ID = 0;
	type = ACS_NONE;
	flags = 0;

	fname = NULL;
	dataPtr = NULL;

	palID = -1;
	palPtr = NULL;
}

aciScreenResource::~aciScreenResource(void)
{
	if(fname)
		delete fname;
}

aciScreenResourcePAL::aciScreenResourcePAL(void)
{
	type = ACS_PAL;
	data = NULL;
	data16 = NULL;
}

aciScreenResourcePAL::~aciScreenResourcePAL(void)
{
	free();
}

aciScreenResourceBMP::aciScreenResourceBMP(void)
{
	type = ACS_BMP;
	SizeX = SizeY = 0;
	data = NULL;
}

unsigned aciScreenResourceBMP::getColor(int x,int y,int frame)
{
	if(data)
		return data[x + y * SizeX];
	return 1;
}

unsigned aciScreenResourceBML::getColor(int x,int y,int frame)
{
	if(data){
		if(flags & ACS_TRUECOLOR)
			return ((unsigned short*)frameTable[frame])[x + y * SizeX];
		else
			return frameTable[frame][x + y * SizeX];
	}
	return 1;
}


unsigned aciScreenResourceBMO::getColor(int x,int y,int frame)
{
	if(data){
		if(flags & ACS_TRUECOLOR)
			return ((unsigned short*)frameTable[frame])[x + y * SizeX];
		else
			return frameTable[frame][x + y * SizeX];
	}
	return 1;
}

unsigned aciScreenResourceHBM::getColor(int x,int y,int frame)
{
	if(scr_frameTable){
		return ((unsigned short*)scr_frameTable[frame] -> dataPtr)[x + y * scr_frameTable[frame] -> SizeX];
	}
	return 1;
}

aciScreenResourceBMP::~aciScreenResourceBMP(void)
{
	if(data)
		delete data;
}

aciScreenResourceBML::aciScreenResourceBML(void)
{
	type = ACS_BML;
	SizeX = SizeY = NumFrames = 0;
	data = NULL;
}

aciScreenResourceBML::~aciScreenResourceBML(void)
{
	if(data)
		delete data;
}

aciScreenResourceHBM::aciScreenResourceHBM(void)
{
	type = ACS_HBM;
}

aciScreenResourceHBM::~aciScreenResourceHBM(void)
{
}

aciScreenResourceBMO::aciScreenResourceBMO(void)
{
	type = ACS_BMO;
	SizeX = SizeY = NumFrames = 0;
	OffsX = OffsY = 0;
	data = NULL;
}

aciScreenResourceBMO::~aciScreenResourceBMO(void)
{
	if(data)
		delete data;
}

aciScreenResourceXBM::aciScreenResourceXBM(void)
{
	type = ACS_XBM;
	PosX = PosY = 0;
	CenterX = CenterY = 0;
	bSizeX = bSizeY = 0;
	SizeX = SizeY = 0;
	SideX = SideY = 0;

	ImageSize = 0;
	data = NULL;
}

aciScreenResourceXBM::~aciScreenResourceXBM(void)
{
	if(data)
		delete data;
}

void aciScreenResourceHBM::load(void)
{
	int res_flag = -1;
	if(aciScreenResource::flags & ACS_MEMORY_FLAG)
		res_flag = acsLowMemFlag;

	XStream fh;

	if(!fname) return;
	acsOpenResource(fname,fh,res_flag);

	hbm_Image::load(fh);
	hbm_Image::build_frames();
	hbm_Image::free_frames();
}

void aciScreenResourceHBM::free(void)
{
	hbm_Image::free();
}

void aciScreenResourceHBM::redraw(int x,int y,int frame,int mode)
{
	hbm_Frame* p;
	if(scr_frameTable){
		p = scr_frameTable[frame];
		if(x == -1 && y == -1){
			if(acsHighColor){
				p -> redraw(mode);
			}
			else
				XGR_PutSpr(X + p -> X,Y + p -> Y,p -> SizeX,p -> SizeY,p -> dataPtr,mode);
			if(!(aciScreenResource::flags & ACS_NO_FLUSH))
				acsSetFlush(X + p -> X,Y + p -> Y,p -> SizeX,p -> SizeY);
		}
		else {
			if(acsHighColor){
				p -> redraw(mode);
			}
			else
				XGR_PutSpr(x + p -> X,y + p -> Y,p -> SizeX,p -> SizeY,p -> dataPtr,mode);
			if(!(aciScreenResource::flags & ACS_NO_FLUSH))
				acsSetFlush(x + p -> X,y + p -> Y,p -> SizeX,p -> SizeY);
		}
	}
}

void aciScreenResourceBMP::load(void)
{
	XStream fh;
	short sx,sy;
	int sz,res_flag = -1;

	if(!fname) return;

	if(flags & ACS_MEMORY_FLAG)
		res_flag = acsLowMemFlag;
	acsOpenResource(fname,fh,res_flag);

	fh > sx > sy;

	SizeX = sx;
	SizeY = sy;
	sz = SizeX * SizeY;

	data = new unsigned char[sz];
	fh.read(data,sz);

	fh.close();

	flags |= ACS_LOADED;
}

void aciScreenResourceBMP::free(void)
{
	aciScreenResource::free();
	if(flags & ACS_LOADED && data){
		delete data;
	}
	flags &= ~ACS_LOADED;
	data = NULL;
}

void aciScreenResourcePAL::prepare(void)
{
	int i,R,G,B;
	if(!data16) data16 = new unsigned[256];
	for(i = 0; i < 256; i ++){
		if(XGR_MASK_R == XGR_MASK_R0){
			R = data[i * 3] >> 1;
			G = data[i * 3 + 1];
			B = data[i * 3 + 2] >> 1;
		}
		else {
			R = data[i * 3] >> 1;
			G = data[i * 3 + 1] >> 1;
			B = data[i * 3 + 2] >> 1;
		}
		data16[i] = XGR_RGB64K(R,G,B);
	}
}

void aciScreenResourcePAL::load(void)
{
	XStream fh;
	if(!fname) return;

	acsOpenResource(fname,fh);

	data = new unsigned char[768];
	fh.read(data,768);

	fh.close();

	if(acsHighColor) prepare();

	flags |= ACS_LOADED;
}

void aciScreenResourcePAL::free(void)
{
	aciScreenResource::free();

	if(data) delete data;
	if(data16) delete data16;

	data = NULL;
	data16 = NULL;

	flags &= ~ACS_LOADED;
}

void aciScreenResourceBML::load(void)
{
	XStream fh;
	short sx,sy,num;
	int i,sz,res_flag = -1;

	if(!fname) return;

	if(flags & ACS_MEMORY_FLAG)
		res_flag = acsLowMemFlag;

	acsOpenResource(fname,fh);

	fh > sx > sy > num;

	SizeX = sx;
	SizeY = sy;
	NumFrames = num;
	sz = SizeX * SizeY * NumFrames;

	data = new unsigned char[sz];
	fh.read(data,sz);

	fh.close();

	frameTable = new unsigned char*[NumFrames];
	for(i = 0; i < NumFrames; i ++) frameTable[i] = data + i * SizeX * SizeY;
	if(flags & ACS_TRUECOLOR) buildData();

	flags |= ACS_LOADED;
}

void aciScreenResourceBML::buildData(void)
{
	int x,y,index,xr,xg,xb;
	unsigned short* p;
	if(!acsHighColor || NumFrames < 3) return;

	dataPtr = new char[SizeX * SizeY * 2];
	p = (unsigned short*)dataPtr;

	xr = xb = 3;
	xg = (XGR_HighColorMode) ? 3 : 2;

	index = 0;
	for(y = 0; y < SizeY; y ++){
		for(x = 0; x < SizeX; x ++){
			p[index] = XGR_RGB64K(frameTable[2][index] >> xr,frameTable[1][index] >> xg,frameTable[0][index] >> xb);
			index ++;			
		}
	}
}

void aciScreenResourceBML::free(void)
{
	aciScreenResource::free();

	if(flags & ACS_LOADED){
		if(data) delete data;
		if(frameTable) delete frameTable;
	}
	flags &= ~ACS_LOADED;
	data = NULL;
	frameTable = NULL;
}

void aciScreenResourceBMO::buildData(void)
{
	int x,y,index,xr,xg,xb;
	unsigned short* p;
	if(!acsHighColor || NumFrames < 3) return;

	dataPtr = new char[SizeX * SizeY * 2];
	p = (unsigned short*)dataPtr;

	xr = xb = 3;
	xg = (XGR_HighColorMode) ? 3 : 2;

	index = 0;
	for(y = 0; y < SizeY; y ++){
		for(x = 0; x < SizeX; x ++){
			p[index] = XGR_RGB64K(frameTable[2][index] >> xr,frameTable[1][index] >> xg,frameTable[0][index] >> xb);
			index ++;			
		}
	}
}

void aciScreenResourceBMO::load(void)
{
	XStream fh;
	short sx,sy,num,ox,oy;
	int i,sz,res_flag = -1;

	if(flags & ACS_MEMORY_FLAG)
		res_flag = acsLowMemFlag;


	if(!(flags & ACS_BUILD_RESOURCE)){
		if(!fname) return;
		acsOpenResource(fname,fh,res_flag);

		if(!(flags & ACS_BML_FILE)){
			fh > sx > sy > num > ox > oy;
			OffsX = ox;
			OffsY = oy;
		}
		else 
			fh > sx > sy > num;

		SizeX = sx;
		SizeY = sy;
		NumFrames = num;

		sz = SizeX * SizeY * NumFrames;

		data = new unsigned char[sz];
		fh.read(data,sz);

		fh.close();
	}
	else {
		NumFrames = 1;
		if(acsHighColor){
			sz = SizeX * SizeY * NumFrames * 2;
			data = new unsigned char[sz];
			XGR_GetSpr16(OffsX,OffsY,SizeX,SizeY,data);
		}
		else {
			sz = SizeX * SizeY * NumFrames;
			data = new unsigned char[sz];
			XGR_GetSpr(OffsX,OffsY,SizeX,SizeY,data);
		}
	}

	frameTable = new unsigned char*[NumFrames];
	for(i = 0; i < NumFrames; i ++) frameTable[i] = data + i * SizeX * SizeY;
	if(flags & ACS_TRUECOLOR) buildData();


	flags |= ACS_LOADED;
}

void aciScreenResourceBMO::free(void)
{
	aciScreenResource::free();

	if(flags & ACS_LOADED){
		if(data) delete data;
		if(frameTable) delete frameTable;
	}
	flags &= ~ACS_LOADED;
	data = NULL;
	frameTable = NULL;
}

void aciScreenResourceXBM::load(void)
{
	int res_flag = -1;

	XStream fh;
	if(!fname) return;

	if(flags & ACS_MEMORY_FLAG)
		res_flag = acsLowMemFlag;

	acsOpenResource(fname,fh,res_flag);

	fh > PosX > PosY > bSizeX > bSizeY > SizeX > SizeY > ImageSize;
	data = new unsigned char[ImageSize];
	fh.read(data,ImageSize);
	fh.close();

	SideX = SizeX/2;
	SideY = SizeY/2;

	CenterX = PosX + SideX;
	CenterY = PosY + SideY;

	flags |= ACS_LOADED;
}

void aciScreenResourceXBM::free(void)
{
	aciScreenResource::free();

	if(flags & ACS_LOADED && data){
		delete data;
	}
	flags &= ~ACS_LOADED;
	data = NULL;
}

void aciScreenResourceBMP::redraw(int x,int y,int frame,int mode)
{
	if(x < 0) x = (XGR_MAXX - SizeX)/2;
	if(y < 0) y = (XGR_MAXY - SizeY)/2;

	if(data){
		if(acsHighColor)
			acsPutSpr16(x,y,SizeX,SizeY,data,((aciScreenResourcePAL*)palPtr) -> data16,mode);
		else
			XGR_PutSpr(x,y,SizeX,SizeY,data,mode);

		if(!(flags & ACS_NO_FLUSH))
			acsSetFlush(x,y,SizeX,SizeY);
	}
}

void aciScreenResourceBML::redraw(int x,int y,int frame,int mode)
{
	if(data && frameTable){
		if(acsHighColor){
			if(flags & ACS_TRUECOLOR)
				XGR_PutSpr16(x,y,SizeX,SizeY,dataPtr,mode);
			else
				acsPutSpr16(x,y,SizeX,SizeY,frameTable[frame],((aciScreenResourcePAL*)palPtr) -> data16,mode);
		}
		else
			XGR_PutSpr(x,y,SizeX,SizeY,frameTable[frame],mode);
		if(!(flags & ACS_NO_FLUSH))
			acsSetFlush(x,y,SizeX,SizeY);
	}
}

void aciScreenResourceBMO::redraw(int x,int y,int frame,int mode)
{
	if(data && frameTable){
		if(x == -1 && y == -1){
			if(acsHighColor){
				if(flags & ACS_TRUECOLOR){
					XGR_PutSpr16(OffsX,OffsY,SizeX,SizeY,dataPtr,mode);
				}
				else {
					if(flags & ACS_BUILD_RESOURCE)
						XGR_PutSpr16(OffsX,OffsY,SizeX,SizeY,frameTable[frame],XGR_BLACK_FON);
					else
						acsPutSpr16(OffsX,OffsY,SizeX,SizeY,frameTable[frame],((aciScreenResourcePAL*)palPtr) -> data16,mode);
				}
			}
			else
				XGR_PutSpr(OffsX,OffsY,SizeX,SizeY,frameTable[frame],mode);
			if(!(flags & ACS_NO_FLUSH))
				acsSetFlush(OffsX,OffsY,SizeX,SizeY);
		}
		else {
			if(acsHighColor){
				if(flags & ACS_TRUECOLOR){
					XGR_PutSpr16(x,y,SizeX,SizeY,dataPtr,mode);
				}
				else {
					if(flags & ACS_BUILD_RESOURCE)
						XGR_PutSpr16(x,y,SizeX,SizeY,frameTable[frame],XGR_BLACK_FON);
					else
						acsPutSpr16(x,y,SizeX,SizeY,frameTable[frame],((aciScreenResourcePAL*)palPtr) -> data16,mode);
				}
			}
			else
				XGR_PutSpr(x,y,SizeX,SizeY,frameTable[frame],mode);
			if(!(flags & ACS_NO_FLUSH))
				acsSetFlush(x,y,SizeX,SizeY);
		}
	}
}

void aciScreenResourceXBM::redraw(int x,int y,int frame,int mode)
{
	if(data){
		if(x == -1 && y == -1){
			ibsout(PosX,PosY,data);
			if(!(flags & ACS_NO_FLUSH))
				acsSetFlush(PosX,PosY,SizeX,SizeY);
		}
		else {
			ibsout(x,y,data);
			if(!(flags & ACS_NO_FLUSH))
				acsSetFlush(x,y,SizeX,SizeY);
		}
	}
}

int aciScreenKeyObject::KeyTrap(int code,int opt_code)
{
	aciScreenKey* p = (aciScreenKey*)codes -> fPtr;

	while(p){
		if(!p -> code_type){
			if(p -> code == code)
				return 1;
		}
		else {
			if(p -> code == opt_code)
				return 1;
		}
		p = (aciScreenKey*)p -> next;
	}
	return 0;
}

void aciScreenKeyObject::AddKey(int code,int opt_flag)
{
	aciScreenKey* p = new aciScreenKey;
	p -> code = code;
	p -> code_type = opt_flag;

	codes -> AddElement((XListElement*)p);
}

void aciScreenEventCommand::init(void)
{
	int i;
	for(i = 0; i < 4; i ++) data[i] = 0;
	code = StartTimer = 0;
	flags = 0;
}

void aciScreenEventLine::put(int cd,int dt0,int dt1,int dt2,int dt3)
{
	if(size < ACS_EVENT_LINE_SIZE){
		table[last_index] -> code = cd;
		table[last_index] -> data[0] = dt0;
		table[last_index] -> data[1] = dt1;
		table[last_index] -> data[2] = dt2;
		table[last_index] -> data[3] = dt3;

		last_index ++;
		if(last_index >= ACS_EVENT_LINE_SIZE)
			last_index = 0;
		size ++;
	}
//	else
//		ErrH.Abort("Event buffer overflow...");
}

aciScreenEventCommand* aciScreenEventLine::get(void)
{
	aciScreenEventCommand* p;
	if(size){
		p = table[first_index];

		first_index ++;
		if(first_index >= ACS_EVENT_LINE_SIZE)
			first_index = 0;

		size --;
		return p;
	}
	return NULL;
}

void aciScreenEventLine::clear(void)
{
	size = 0;
	first_index = 0;
	last_index = 0;
}

void aciScreenEvent::AddCommand(aciScreenEventCommand* p)
{
	commSeq -> AddElement((XListElement*)p);
}

void aciScreenObject::AddSeq(aciScreenFrameSequence* p)
{
	frameSeq -> AddElement((XListElement*)p);
}

void aciScreenObject::AddEvent(aciScreenEvent* p)
{
	events -> AddElement((XListElement*)p);
}

void aciScreen::AddEvent(aciScreenEvent* p)
{
	events -> AddElement((XListElement*)p);
}

void aciScreen::AddObject(aciScreenObject* p)
{
	objList -> AddElement((XListElement*)p);
}

void aciScreen::AddResource(aciScreenResource* p)
{
	resources -> AddElement((XListElement*)p);
}

void aciScreenDispatcher::AddScreen(aciScreen* p)
{
	scrList -> AddElement((XListElement*)p);
}

void aciScreenDispatcher::AddResource(aciScreenResource* p)
{
	resources -> AddElement((XListElement*)p);
}

aciScreenResource* aciScreen::GetResource(int id)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		if(p -> ID == id) return p;
		p = (aciScreenResource*)p -> next;
	}
	return NULL;
}

aciScreenResource* aciScreenDispatcher::GetResource(int id)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		if(p -> ID == id) return p;
		p = (aciScreenResource*)p -> next;
	}
	if(curScr)
		return curScr -> GetResource(id);
	else
		return NULL;
}

aciScreen* aciScreenDispatcher::GetScreen(int id)
{
	aciScreen* p = (aciScreen*)scrList -> fPtr;
	while(p){
		if(p -> ID == id) return p;
		p = (aciScreen*)p -> next;
	}
	return NULL;
}

aciScreenObject* aciScreenDispatcher::GetObject(int id)
{
	return curScr -> GetObject(id);
}

aciScreenObject* aciScreen::GetObject(int id)
{
	aciScreenObject* p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(p -> ID == id) return p;
		p = (aciScreenObject*)p -> next;
	}
	return NULL;
}

void aciScreen::alloc_mem(void)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		p -> load();
		p -> palPtr = acsGetResource(p -> palID);
		if(!(p -> flags & ACS_BUILD_RESOURCE) && XGR_MAXX != 640){
			p -> change_coords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);
		}
		p = (aciScreenResource*)p -> next;
	}
}

void aciScreen::free_mem(void)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		p -> free();
		p = (aciScreenResource*)p -> next;
	}
}

void aciScreenDispatcher::alloc_mem(void)
{
	if(XGR_Obj.flags & XGR_HICOLOR)  acsHighColor = 1;

	eventLine -> clear();
	flushLine -> clear();

	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		p -> load();
		p -> palPtr = GetResource(p -> palID);
		if(!(p -> flags & ACS_BUILD_RESOURCE) && XGR_MAXX != 640){
			p -> change_coords((XGR_MAXX - 640)/2,(XGR_MAXY - 480)/2);
		}
		p = (aciScreenResource*)p -> next;
	}
	curScr = GetScreen(curScrID);
	curScr -> prepare();
	curScr -> CenterMouse();
	curScr -> alloc_mem();
	curScr -> init_objects();

	curScr -> backRes = GetResource(curScr -> backResID);

	flags &= ~ACS_NEED_EXIT;
	flags |= ACS_FORCED_REDRAW;
	flags |= ACS_FIRST_REDRAW;

	KeyTrap(ACS_STARTUP_KEY);
}

void aciScreenDispatcher::free_mem(void)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	if(activeInput) CancelInput();
	while(p){
		p -> free();
		p = (aciScreenResource*)p -> next;
	}
	if(curScr) 
		curScr -> free_mem();
}

void aciScreen::init_objects(void)
{
	aciScreenObject* p = (aciScreenObject*)objList -> fPtr;
	while(p){
		p -> init();
		p -> prepare();
		p = (aciScreenObject*)p -> next;
	}
}

void aciScreenDispatcher::KeyTrap(int code)
{
//	int chr,shift = 0,r_flag = 0;
//	  if(iRussian) r_flag = xtGetKeyState(VK_CAPITAL);

#ifdef _ACS_MOVE_OBJECTS_
	int delta = (XKey.Pressed(VK_CONTROL)) ? 10 : 1;
	switch(code){
		case VK_TAB:
			aciShowBounds ^= 1;
			flags |= ACS_FORCED_REDRAW;
			break;
		case VK_SPACE:
			DumpMovedObjects();
			break;
		case VK_LEFT:
			if(acsMoveObj){ 
				acsMoveObj -> PosX -= delta;
				acsMoveObj -> flags |= ACS_MOVED_OBJECT;
				flags |= ACS_FORCED_REDRAW;
			}
			break;
		case VK_UP:
			if(acsMoveObj){ 
				acsMoveObj -> PosY -= delta;
				acsMoveObj -> flags |= ACS_MOVED_OBJECT;
				flags |= ACS_FORCED_REDRAW;
			}
			break;
		case VK_RIGHT:
			if(acsMoveObj){ 
				acsMoveObj -> PosX += delta;
				acsMoveObj -> flags |= ACS_MOVED_OBJECT;
				flags |= ACS_FORCED_REDRAW;
			}
			break;
		case VK_DOWN:
			if(acsMoveObj){ 
				acsMoveObj -> PosY += delta;
				acsMoveObj -> flags |= ACS_MOVED_OBJECT;
				flags |= ACS_FORCED_REDRAW;
			}
			break;
	}
	if(acsMoveObj){
		if(acsMoveObj -> PosX < 0) acsMoveObj -> PosX = 0;
		if(acsMoveObj -> PosY < 0) acsMoveObj -> PosY = 0;

		if(acsMoveObj -> PosX >= XGR_MAXX - acsMoveObj -> SizeX) acsMoveObj -> PosX = XGR_MAXX - acsMoveObj -> SizeX;
		if(acsMoveObj -> PosY >= XGR_MAXY - acsMoveObj -> SizeY) acsMoveObj -> PosY = XGR_MAXY - acsMoveObj -> SizeY;
	}
#else
	int x,y,dir = -1;
	aciScreenObject* p = NULL;

	x = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
	y = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

	if(!acsDisableEvents && !activeInput){
		switch(code){
			case VK_LEFT:
				dir = 0;
				break;
			case VK_UP:
				dir = 1;
				break;
			case VK_RIGHT:
				dir = 2;
				break;
			case VK_DOWN:
			case VK_TAB:
				dir = 3;
				break;
			case VK_SPACE:
			case VK_RETURN:
				if(!activeInput)
					KeyBuf -> put(VK_MOUSE_LBUTTON,KBD_CUR_KEY_PRESSED);
				break;
		}
	}

	if(dir != -1){
		p = curScr -> GetObjectXY(x,y,dir);
		if(!p)
			p = curScr -> GetObjectXY(x,y,dir,0);

		if(p){
			XGR_MouseObj.SetPos(p -> PosX + p -> SizeX/2 - XGR_MouseObj.SizeX/2,p -> PosY + p -> SizeY/2 - XGR_MouseObj.SizeY/2);
//			acsMouseTargetX = p -> PosX + p -> SizeX/2 - XGR_MouseObj.SizeX/2;
//			acsMouseTargetY = p -> PosY + p -> SizeY/2 - XGR_MouseObj.SizeY/2;
		}
	}
#endif
	mchA_CHQuant(code);
	if(!acsDisableEvents){
		if(activeInput){
			if((code >= 0 && code <= 256) || (code >= VK_BUTTON_1 && code <= VK_BUTTON_32))
				InputQuant(code,0);
			else // mouse, etc
				curScr -> KeyTrap(code);
		}
		else
			curScr -> KeyTrap(code);
	}
/*
	if(!acsDisableEvents){
		if(activeInput){
			if(code >= 0 && code <= 256){
				if(XKey.Pressed(VK_SHIFT)) shift = 1;
				chr = acsGetKeyName(code,shift,r_flag);
				InputQuant(code,mch_toupper(chr));
			}
			else // mouse, etc
				curScr -> KeyTrap(code);
		}
		else
			curScr -> KeyTrap(code);
	}
*/
}

void aciScreen::KeyTrap(int code)
{
	int x,y,mouse = 0,opt_code;
	aciScreenEvent* ev;
	aciScreenObject* p;

	if(code < 0){
		mouse = 1;
		x = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
		y = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

		if(iWorldMode && !acsActiveFlag){
			x = round(float(x) / mchA_d3dResX);
			y = round(float(y) / mchA_d3dResY);
		}
	}

	opt_code = mchGetKeyID_First(code);

	ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(code,opt_code))
			ev -> Start();

		ev = (aciScreenEvent*)ev -> next;
	}

	p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(!(p -> flags & ACS_BLOCKED)){
			if(!mouse || p -> CheckXY(x,y)){
				ev = (aciScreenEvent*)p -> events -> fPtr;
				while(ev){
					if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(code,opt_code)){
						ev -> Start();

						if(code < 0)
							acsLastObj = p;
					}
					ev = (aciScreenEvent*)ev -> next;
				}
			}
		}
		p = (aciScreenObject*)p -> next;
	}

	opt_code = mchGetKeyID_Next();

	while(opt_code != -1){
		ev = (aciScreenEvent*)events -> fPtr;
		while(ev){
			if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(code,opt_code))
				ev -> Start();

			ev = (aciScreenEvent*)ev -> next;
		}

		p = (aciScreenObject*)objList -> fPtr;
		while(p){
			if(!(p -> flags & ACS_BLOCKED)){
				if(!mouse || p -> CheckXY(x,y)){
					ev = (aciScreenEvent*)p -> events -> fPtr;
					while(ev){
						if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(code,opt_code)){
							ev -> Start();

							if(code < 0)
								acsLastObj = p;
						}
						ev = (aciScreenEvent*)ev -> next;
					}
				}
			}
			p = (aciScreenObject*)p -> next;
		}

		opt_code = mchGetKeyID_Next();
	}
}

void aciScreenObject::prepare(void)
{
	aciScreenFrameSequence* seq;
	activeSeq = NULL;

	StopEvents();
	seq = (aciScreenFrameSequence*)frameSeq -> fPtr;
	while(seq){
		if(seq -> StartFrame == ACS_MAX_FRAME)
			seq -> StartFrame = acsGetMaxFrame(seq -> ResourceID);
		if(seq -> StartFrame == ACS_MID_FRAME)
			seq -> StartFrame = (acsGetMaxFrame(seq -> ResourceID) + 1) / 2;

		if(seq -> EndFrame == ACS_MAX_FRAME)
			seq -> EndFrame = acsGetMaxFrame(seq -> ResourceID);
		if(seq -> EndFrame == ACS_MID_FRAME)
			seq -> EndFrame = (acsGetMaxFrame(seq -> ResourceID) + 1) / 2;

		seq = (aciScreenFrameSequence*)seq -> next;
	}
}

void aciScreenObject::StopEvents(void)
{
	aciScreenEvent* ev;
	ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		ev -> Stop();
		ev = (aciScreenEvent*)ev -> next;
	}
}

int aciScreenObject::isActive(void)
{
	aciScreenEvent* ev;
	ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		if(ev -> flags & ACS_EVENT_ACTIVE) return 1;
		ev = (aciScreenEvent*)ev -> next;
	}
	return 0;
}

void aciScreen::prepare(void)
{
	aciScreenEvent* ev;

	ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		ev -> Stop();
		ev = (aciScreenEvent*)ev -> next;
	}

//	CenterMouse();
}

void aciScreenEvent::Start(void)
{
	Stop();
	CurTimer = 0;
	flags |= ACS_EVENT_ACTIVE;
}

void aciScreenEvent::Stop(void)
{
	flags &= ~ACS_EVENT_ACTIVE;
	aciScreenEventCommand* p = (aciScreenEventCommand*)commSeq -> fPtr;
	while(p){
		p -> flags &= ~ACS_COMMAND_STARTED;
		p = (aciScreenEventCommand*)p -> next;
	}
}

void aciScreenEvent::Quant(void)
{
	aciScreenEventCommand* p = (aciScreenEventCommand*)commSeq -> fPtr;
	int nextQuant = 1;

	while(p){
		if(p -> StartTimer == CurTimer){
			if(p -> flags & ACS_COMMAND_STARTED){
				if(!acsEventActive(p))
					nextQuant = 0;
			}
			else {
				p -> flags |= ACS_COMMAND_STARTED;
				acsSendEvent(p);
				nextQuant = 0;
			}
		}
		p = (aciScreenEventCommand*)p -> next;
	}
	if(nextQuant){
		CurTimer ++;
		if(CurTimer > MaxTimer) Stop();
	}
}

void aciScreenObject::EventQuant(void)
{
	aciScreenEvent* ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		if(ev -> flags & ACS_EVENT_ACTIVE)
			ev -> Quant();
		ev = (aciScreenEvent*)ev -> next;
	}
}

void aciScreen::EventQuant(void)
{
	aciScreenObject* p = (aciScreenObject*)objList -> fPtr;
	aciScreenEvent* ev = (aciScreenEvent*)events -> fPtr;
	while(ev){
		if(ev -> flags & ACS_EVENT_ACTIVE)
			ev -> Quant();
		ev = (aciScreenEvent*)ev -> next;
	}
	while(p){
		p -> EventQuant();
		p = (aciScreenObject*)p -> next;
	}
}

void aciScreenDispatcher::EventQuant(void)
{
	curScr -> EventQuant();
}

void aciScreenDispatcher::FlushEvents(void)
{
	aciScreenObject* p;
	aciScreenEventCommand* ev;

	while(eventLine -> size){
		ev = eventLine -> get();
		switch(ev -> code){
			case ACS_SET_FULL_REDRAW:
				if(ev -> data[0])
					flags |= ACS_FULL_REDRAW;
//				else
//					flags &= ~ACS_FULL_REDRAW;
				break;
			case ACS_DISABLE_EVENTS:
				acsDisableEvents = 1;
				break;
			case ACS_ENABLE_EVENTS:
				acsDisableEvents = 0;
				break;
			case ACS_EXIT:
				QuantCode = ev -> data[0];
				flags |= ACS_NEED_EXIT;
				break;
			case ACS_SET_RESOURCE:
				SetResource(ev -> data[0],ev -> data[1],ev -> data[2]);
				break;
			case ACS_EXEC_SEQUENCE:
				SetSequence(ev -> data[0],ev -> data[1]);
				break;
			case ACS_CHANGE_SCROLLER:
				ChangeScroller(ev -> data[0],XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2);
				break;
			case ACS_DEC_SCROLLER:
				DecScroller(ev -> data[0]);
				break;
			case ACS_INC_SCROLLER:
				IncScroller(ev -> data[0]);
				break;
			case ACS_INPUT_STRING:
				acsInputMode = 0;
				PrepareInput(ev -> data[0]);
				break;
			case ACS_INPUT_KEYNAME:
				acsInputMode = ACS_KEY_INPUT_MODE;
				PrepareInput(ev -> data[0]);
				break;
			case ACS_EXT_EVENT:
				acsHandleExtEvent(ev -> data[0],ev -> data[1],ev -> data[2],ev -> data[3]);
				break;
			case ACS_IWORLD_EVENT:
				iwHandleEvent(ev -> data[0],ev -> data[1],ev -> data[2]);
				break;
			case ACS_CHANGE_STR_STATE:
				acsChangeStrState(ev -> data[0]);
				break;
			case ACS_CHANGE_SCREEN:
				PrepareChangeScreen(ev -> data[0]);
				break;	
			case ACS_CHANGE_SCREEN_NEXT:
				if(curScr -> nextID == -1)
					acsHandleExtEvent(ACS_EXT_NEW_GAME);
				else
					PrepareChangeScreen(curScr -> nextID);
				break;	
			case ACS_CHANGE_SCREEN_PREV:
				PrepareChangeScreen(curScr -> prevID);
				break;	
			case ACS_INC_STATE:
				p = GetObject(ev -> data[0]);
				if(p && p -> type == ACS_INPUT_FIELD_OBJ)
					((aciScreenInputField*)p) -> inc_state();
				break;
			case ACS_DEC_STATE:
				p = GetObject(ev -> data[0]);
				if(p && p -> type == ACS_INPUT_FIELD_OBJ)
					((aciScreenInputField*)p) -> dec_state();
				break;
			case ACS_HIDE_OBJ:
				p = GetObject(ev -> data[0]);
				if(p)
					p -> flags |= ACS_HIDDEN | ACS_BLOCKED;
				break;
			case ACS_SHOW_OBJ:
				p = GetObject(ev -> data[0]);
				if(p)
					p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
				break;
			case ACS_LOCK_OBJ:
				p = GetObject(ev -> data[0]);
				if(p)
					p -> flags |= ACS_BLOCKED;
				break;
			case ACS_UNLOCK_OBJ:
				p = GetObject(ev -> data[0]);
				if(p)
					p -> flags &= ~ACS_BLOCKED;
				break;
			case ACS_ALPHA_SEQ:
				acsSetAlphaSeq(ev -> data[0],ev -> data[1]);
				break;
		}
	}
}

void aciScreenDispatcher::flush(void)
{
	aciScreenEventCommand* ev;
	if(flags & ACS_FULLSCR_FLUSH){
//		if(RenderMode == DIRECT3D_HICOLOR)
//			mchA_d3dFlushBackBuffer(0,0,XGR_MAXX,XGR_MAXY);
//		else
//			XGR_Flush(0,0,XGR_MAXX,XGR_MAXY);
		gb_IGraph3d->Flush();
		flushLine -> clear();
		flags ^= ACS_FULLSCR_FLUSH;
	}
	else {
		while(flushLine -> size){
			ev = flushLine -> get();
//			if(RenderMode == DIRECT3D_HICOLOR)
				mchA_d3dFlushBackBuffer(ev -> data[0],ev -> data[1],ev -> data[2],ev -> data[3]);
//			else
//				XGR_Flush(ev -> data[0],ev -> data[1],ev -> data[2],ev -> data[3]);
		}
	}
}

void aciScreenObject::redraw(int flag2d)
{
	int mode = XGR_HIDDEN_FON;

	if(flags & ACS_BLACK_FON) 
		mode = XGR_BLACK_FON;

	if(!palPtr && PalID != -1)
		palPtr = acsGetResource(PalID);

	if(resPtr)
		resPtr -> redraw(PosX,PosY,curFrame,mode);

	acsSetFlush(PosX,PosY,SizeX,SizeY);
	flags &= ~ACS_REDRAW_OBJECT;
}

void aciScreen::ChangeCoords(int dx,int dy)
{
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		p -> change_coords(dx,dy);
		p = (aciScreenResource*)p -> next;
	}
	aciScreenObject* obj = (aciScreenObject*)objList -> fPtr;
	while(obj){
		if(obj -> PosX != -1)
			obj -> change_coords(dx,dy);
		obj = (aciScreenObject*)obj -> next;
	}
}

void aciScreen::ScaleCoords(float dx,float dy)
{
	aciScreenObject* obj = (aciScreenObject*)objList -> fPtr;
	while(obj){
		if(obj -> PosX != -1){
			obj -> scale_coords(dx,dy);
			obj -> AdjustSize();
		}
		obj = (aciScreenObject*)obj -> next;
	}
}

void aciScreen::Scale(float dx,float dy)
{
	aciScreenObject* obj = (aciScreenObject*)objList -> fPtr;
	while(obj){
		if(obj -> PosX != -1){
			//@caiiiycuk was scale
			obj -> Scale(dx,dy);
			obj -> AdjustSize();
		}
		obj = (aciScreenObject*)obj -> next;
	}
}

void aciScreenDispatcher::ScaleCoords(float dx,float dy)
{
	aciScreen* scr = (aciScreen*)scrList -> fPtr;
	while(scr){
		scr -> ScaleCoords(dx,dy);
		scr = (aciScreen*)scr -> next;
	}
}

void aciScreenDispatcher::Scale(float dx,float dy)
{
	aciScreen* scr = (aciScreen*)scrList -> fPtr;
	while(scr){
		scr -> Scale(dx,dy);
		scr = (aciScreen*)scr -> next;
	}
}

void aciScreenDispatcher::ChangeCoords(int dx,int dy)
{
#ifndef _ACI_3D_VIEW_
	aciScreen* scr = (aciScreen*)scrList -> fPtr;
	aciScreenResource* p = (aciScreenResource*)resources -> fPtr;
	while(p){
		p -> change_coords(dx,dy);
		p = (aciScreenResource*)p -> next;
	}
	while(scr){
		scr -> ChangeCoords(dx,dy);
		scr = (aciScreen*)scr -> next;
	}
#else
	float sx,sy;

	sx = (640.0f + (float)dx * 2.0f) / 640.0f;
	sy = (480.0f + (float)dy * 2.0f) / 480.0f;

	Scale(sx,sy);
#endif
}

int aciScreen::redraw(int force,int flag2d)
{
	int ret = 0;
	aciScreenResource* res = (aciScreenResource*)resources -> fPtr;
	aciScreenObject* p = (aciScreenObject*)objList -> fPtr;
	if(!mchResourcesFlag && force && !acsAlphaDelta){
		if(backRes)
			backRes -> redraw(-1,-1,0,XGR_BLACK_FON);
		else
			gb_IGraph3d->Fill(0,0,0);
	}
	if(acsFirstRedraw){
		while(res){
			if(res -> flags & ACS_BUILD_RESOURCE){
				res -> free();
				res -> load();
			}
			res = (aciScreenResource*)res -> next;
		}
		acsFirstRedraw = 0;
	}
	while(p){
		if(force || p -> flags & ACS_REDRAW_OBJECT){
			if(!(p -> flags & ACS_HIDDEN)) p -> redraw(flag2d);
			else p -> flags &= ~ACS_REDRAW_OBJECT;
			ret = 1;
		}
		p = (aciScreenObject*)p -> next;
	}
	return ret;
}

#ifdef _ACS_MOVE_OBJECTS_
void aciScreen::show_bound(void)
{
	aciScreenObject* p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(!(p -> flags & ACS_HIDDEN)) p -> show_bound();
		p = (aciScreenObject*)p -> next;
	}
}
#endif

int aciScreenDispatcher::redraw(void)
{
	int force,ret = 0;

	if(flags & ACS_FORCED_REDRAW || flags & ACS_FULL_REDRAW){
		force = 1;
		flags |= ACS_FULLSCR_FLUSH;
		flags &= ~ACS_FORCED_REDRAW;
	}
	else
		force = 0;

	if(flags & ACS_FIRST_REDRAW){
		flags ^= ACS_FIRST_REDRAW;
		acsFirstRedraw = 1;
	}
	else
		acsFirstRedraw = 0;

#ifdef _ACI_3D_VIEW_
	if(mchResourcesFlag){
		if(acsAlphaDelta){
			acsAlpha = acsAlphaDest;
			nextScr -> redraw(force);
		}
	}
	else {
		if(acsAlphaDelta){
			acsObjPhase = 1.0f - acsChangeScreenQuant3D();
			nextScr -> redraw(force);
			acsObjPhase = 1.0f - acsObjPhase;
		}
	}
#endif
	ret = curScr -> redraw(force);
	return ret;
}

int aciScreenDispatcher::Quant(void)
{
	acsObjScalePhase += acsObjScaleDelta;
	if(acsObjScalePhase < 0.0f){
		acsObjScalePhase = 0.0f;
		acsObjScaleDelta = fabs(acsObjScaleDelta);
	}
	if(acsObjScalePhase > acsObjScalePeriod){
		acsObjScalePhase = acsObjScalePeriod;
		acsObjScaleDelta = -fabs(acsObjScaleDelta);
	}
	
	acsMouseObj = NULL;
	MouseQuant();

	acsExtQuant();
	acsAlphaQuant();
	EventQuant();
	FlushEvents();
	acsExtQuant();
	ObjectQuant();
	ActiveInputQuant();

	return QuantCode;
}

void aciScreenDispatcher::ActiveInputQuant(void)
{
	int sz,chr;
	if(activeInput){
		if(XKey.LastChar != -1){
			if(isprint(XKey.LastChar)){
				chr = toupper(XKey.LastChar);
				if(chr != -1)
					InputQuant(0,chr);
			}

			XKey.LastChar = -1;
		}

		acsInputTimer ++;
		if(acsInputTimer >= ACS_INPUT_TIMER){
			acsInputTimer = 0;
			acsInputFlag = 1;
		}
		if(acsInputFlag){
			sz = strlen(activeInput -> string);
			if(activeInput -> string[sz - 1] == '_')
				activeInput -> string[sz - 1] = ' ';
			else
				activeInput -> string[sz - 1] = '_';
			activeInput -> flags |= ACS_REDRAW_OBJECT;
			acsInputFlag = 0;
		}
	}
}

void aciScreenDispatcher::RedrawQuant(int flush_log)
{
	redraw();
	acsExtRedraw();

	if(flush_log)
		flush();
	else
		flushLine -> clear();
}

int aciScreenDispatcher::ObjectQuant(int mx,int my)
{
	int ret = 0;

//	if(mx == -1 || my == -1){
		mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
		my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;
//	}

	aciScreenObject* p = (aciScreenObject*)curScr -> objList -> fPtr;
	while(p){
		if(p -> activeSeq){
			p -> ResID = p -> activeSeq -> ResourceID;
			p -> resPtr = GetResource(p -> ResID);
			p -> curFrame = p -> activeSeq -> CurFrame;
			if(p -> activeSeq -> CurFrame == p -> activeSeq -> EndFrame)
				p -> activeSeq = NULL;
			else
				p -> activeSeq -> CurFrame += p -> activeSeq -> FrameDelta;
			p -> flags |= ACS_REDRAW_OBJECT;
			ret = 1;
		}
		if(p -> flags & ACS_MOUSE_SELECT){
			if(!acsDisableEvents){
				if(p -> CheckXY(mx,my)){
					p -> flags &= ~ACS_BLOCKED;
					p -> flags |= ACS_REDRAW_OBJECT;
				}
				else {
					p -> flags |= ACS_BLOCKED;
					p -> flags |= ACS_REDRAW_OBJECT;
				}
			}
		}
		p -> Quant();
		p = (aciScreenObject*)p -> next;
	}
	return ret;
}

void aciScreenResource::set_name(char* p)
{
	int sz = strlen(p) + 1;
	fname = new char[sz];
	strcpy(fname,p);
}

void aciScreenDispatcher::SetResource(int obj_id,int res_id,int fr)
{
	int x,y;
	aciScreenResource* res = GetResource(res_id);
	aciScreenObject* obj = GetObject(obj_id);

	if(obj && res){
		obj -> ResID = res -> ID;
		obj -> resPtr = res;
		if(fr == ACS_MAX_FRAME)
			fr = acsGetMaxFrame(res_id);
		obj -> curFrame = fr;
		res -> flags |= ACS_NO_FLUSH;

		if(obj -> PosX == -1 && obj -> PosY == -1)
			acsGetResourceSize(res,obj -> PosX,obj -> PosY,obj -> SizeX,obj -> SizeY);
		else
			acsGetResourceSize(res,x,y,obj -> SizeX,obj -> SizeY);
	}
	else
		ErrH.Abort("SetResource() failed...");
}

void aciScreenDispatcher::SetSequence(int obj_id,int seq_id)
{
	aciScreenFrameSequence* seq;
	aciScreenObject* obj = GetObject(obj_id);

	if(obj){
		seq = obj -> GetSequence(seq_id);
		obj -> activeSeq = seq;
		seq -> CurFrame = seq -> StartFrame;
	}
	else
		ErrH.Abort("SetSequence() failed...");
}

aciScreenFrameSequence* aciScreenObject::GetSequence(int id)
{
	aciScreenFrameSequence* seq = (aciScreenFrameSequence*)frameSeq -> fPtr;
	while(seq){
		if(seq -> ID == id) return seq;
		seq = (aciScreenFrameSequence*)seq -> next;
	}
	return NULL;
}

int acsGetMaxFrame(int res_id)
{
	aciScreenResource* p;
	aciScreenResourceBML* bml;
	aciScreenResourceBMO* bmo;
	aciScreenResourceHBM* hbm;

	p = acsScrD -> GetResource(res_id);
	if(!p) return 0;

	switch(p -> type){
		case ACS_BML:
			bml = (aciScreenResourceBML*)p;
			return (bml -> NumFrames - 1);
		case ACS_BMO:
			bmo = (aciScreenResourceBMO*)p;
			return (bmo -> NumFrames - 1);
		case ACS_HBM:
			hbm = (aciScreenResourceHBM*)p;
			return (hbm -> numFrames - 1);
	}
	return 0;
}

void acsGetResourceSize(aciScreenResource* p,int& x,int& y,int& sx,int& sy)
{
	aciScreenResourceBMP* bmp;
	aciScreenResourceBML* bml;
	aciScreenResourceBMO* bmo;
	aciScreenResourceXBM* xbm;
	aciScreenResourceHBM* hbm;

	switch(p -> type){
		case ACS_BMP:
			bmp = (aciScreenResourceBMP*)p;
			sx = bmp -> SizeX;
			sy = bmp -> SizeY;
			break;
		case ACS_BML:
			bml = (aciScreenResourceBML*)p;
			sx = bml -> SizeX;
			sy = bml -> SizeY;
			break;
		case ACS_BMO:
			bmo = (aciScreenResourceBMO*)p;
			x = bmo -> OffsX;
			y = bmo -> OffsY;
			sx = bmo -> SizeX;
			sy = bmo -> SizeY;
			break;
		case ACS_XBM:
			xbm = (aciScreenResourceXBM*)p;
			x = xbm -> PosX;
			y = xbm -> PosY;
			sx = xbm -> SizeX;
			sy = xbm -> SizeY;
			break;
		case ACS_HBM:
			hbm = (aciScreenResourceHBM*)p;
			x = hbm -> X;
			y = hbm -> Y;
			sx = hbm -> SizeX;
			sy = hbm -> SizeY;
			break;
	}
}

int aciScreenObject::CheckXY(int x,int y)
{
	if(x >= PosX && x < PosX + SizeX && y >= PosY && y < PosY + SizeY){
		return 1;
	}
	return 0;
}

int aciScreenScroller::CheckXY(int x,int y)
{
	int dx = 0;
	if(ScrRes) dx = ScrResSX * 3;
	if(x >= PosX - dx && x < PosX + SizeX + dx && y >= PosY && y < PosY + SizeY)
		return 1;
	return 0;
}

aciScreenResource* acsAllocResource(int type)
{
	aciScreenResource* p = NULL;
	aciScreenResourceBMP* bmp;
	aciScreenResourceBML* bml;
	aciScreenResourceBMO* bmo;
	aciScreenResourceXBM* xbm;
	aciScreenResourcePAL* pal;
	aciScreenResourceHBM* hbm;

	switch(type){
		case ACS_BMP:
			bmp = new aciScreenResourceBMP;
			p = bmp;
			break;
		case ACS_BML:
			bml = new aciScreenResourceBML;
			p = bml;
			break;
		case ACS_BMO:
			bmo = new aciScreenResourceBMO;
			p = bmo;
			break;
		case ACS_XBM:
			xbm = new aciScreenResourceXBM;
			p = xbm;
			break;
		case ACS_PAL:
			pal = new aciScreenResourcePAL;
			p = pal;
			break;
		case ACS_HBM:
			hbm = new aciScreenResourceHBM;
			p = hbm;
			break;
	}
	return p;
}

void acsFreeResource(aciScreenResource* p)
{
	aciScreenResourceBMP* bmp;
	aciScreenResourceBML* bml;
	aciScreenResourceBMO* bmo;
	aciScreenResourceXBM* xbm;
	aciScreenResourcePAL* pal;
	aciScreenResourceHBM* hbm;

	switch(p -> type){
		case ACS_BMP:
			bmp = (aciScreenResourceBMP*)p;
			delete bmp;
			break;
		case ACS_BML:
			bml = (aciScreenResourceBML*)p;
			delete bml;
			break;
		case ACS_BMO:
			bmo = (aciScreenResourceBMO*)p;
			delete bmo;
			break;
		case ACS_XBM:
			xbm = (aciScreenResourceXBM*)p;
			delete xbm;
			break;
		case ACS_HBM:
			hbm = (aciScreenResourceHBM*)p;
			delete hbm;
			break;
		case ACS_PAL:
			pal = (aciScreenResourcePAL*)p;
			delete pal;
			break;
	}
}

aciScreenObject* acsAllocObject(int type)
{
	aciScreenObject* p = NULL;
	aciScreenScroller* scr;
	aciScreenInputField* inp;
	switch(type){
		case ACS_BASE_OBJ:
			p = new aciScreenObject;
			break;
		case ACS_SCROLLER_OBJ:
			scr = new aciScreenScroller;
			p = scr;
			break;
		case ACS_INPUT_FIELD_OBJ:
			inp = new aciScreenInputField;
			p = inp;
			break;
	}
	return p;
}

void acsFreeObject(aciScreenObject* p)
{
	aciScreenScroller* scr;
	aciScreenInputField* inp;
	switch(p -> type){
		case ACS_BASE_OBJ:
			delete p;
			break;
		case ACS_SCROLLER_OBJ:
			scr = (aciScreenScroller*)p;
			delete scr;
			break;
		case ACS_INPUT_FIELD_OBJ:
			inp = (aciScreenInputField*)p;
			delete inp;
			break;
	}
}

void aciScreenScroller::redraw(int flag2d)
{
	int sx,_sx,_sy,sdx,sdy,al;

	if(ActiveSX) _sx = ActiveSX;
	else _sx = SizeX;

	if(ActiveSY) _sy = ActiveSY;
	else _sy = SizeY;

	sdx = (SizeX - _sx) / 2;
	sdy = (SizeY - _sy) / 2;

	if(!ScrRes)
		sx = (CurValue * _sx) / MaxValue;
	else
		sx = (CurValue * (_sx - ScrResSX)) / MaxValue + ScrResSX/2;

	int x,y,dx,dy;

	int cl;
	float z,phase;
	Vect3f r,r0,r1;
	Mat3f mat;

	if(flag2d){
		if(flags & ACS_BACK_RES)
			aciScreenObject::redraw();

		if(Color){
			if(acsHighColor){
				if(AlphaCur == -1){
					XGR_Rectangle16(PosX + sdx,PosY + sdy,_sx,_sy,((aciScreenResourcePAL*)palPtr) -> data16[BackColor],((aciScreenResourcePAL*)palPtr) -> data16[BackColor],XGR_FILLED);
					XGR_Rectangle16(PosX + sdx,PosY + sdy,sx,_sy,((aciScreenResourcePAL*)palPtr) -> data16[Color],((aciScreenResourcePAL*)palPtr) -> data16[Color],XGR_FILLED);
				}
				else {
					al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
					acsRectangle(PosX + sdx,PosY + sdy,_sx,_sy,BackColor,((aciScreenResourcePAL*)palPtr) -> data16,al);
					acsRectangle(PosX + sdx,PosY + sdy,sx,_sy,Color,((aciScreenResourcePAL*)palPtr) -> data16,al);
				}
			}
			else {
				XGR_Rectangle(PosX + sdx,PosY + sdy,_sx,_sy,BackColor,BackColor,XGR_FILLED);
				XGR_Rectangle(PosX + sdx,PosY + sdy,sx,_sy,Color,Color,XGR_FILLED);
			}
		}
		if(!(flags & ACS_BACK_RES))
			aciScreenObject::redraw();
		if(sRes){
			dx = (SizeX - sResSX) / 2;
			dy = (SizeY - sResSY) / 2;
			sRes -> redraw(PosX + dx,PosY + dy,0,XGR_HIDDEN_FON);
		}
		if(ScrRes){
			sx = (CurValue * (_sx - ScrResSX)) / MaxValue;
			x = PosX + sx;
			y = PosY + ((SizeY - ScrResSY) >> 1);
			ScrRes -> redraw(x + sdx,y,0,XGR_HIDDEN_FON);
		}
	}
	else {
		al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
		z = (float)al / 4.0f;

		if(CheckXY(XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2,XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2))
			cl = BackColor;
		else 
			cl = Color;

		if(!acsAlphaDelta){
			if(_sx - sx > 0)
				acsOutRect3D(PosX + sdx + sx + (_sx - sx)/2,PosY + sdy + _sy/2,z,_sx - sx,_sy,Color,0,0,1);

			if(sx)
				acsOutRect3D(PosX + sdx + sx/2,PosY + sdy + _sy/2,z,sx,_sy,cl,0);
		}
		else {
			phase = acsObjPhase;

			if(_sx - sx > 0){
				r0 = Vect3f(PosX + sdx + sx + (_sx - sx)/2,PosY + sdy + _sy/2,0);
				r = acsCalcR(r0,phase);

				r1 = (r0 - r) % r.K;
				mat.set(r1,M_PI * 2.0f * phase);

				acsOutRect3D(r.x,r.y,r.z,_sx - sx,_sy,Color,0,1,1,mat);
			}

			if(sx){
				r0 = Vect3f(PosX + sdx + sx/2,PosY + sdy + _sy/2,0);
				r = acsCalcR(r0,phase);

				r1 = (r0 - r) % r.K;
				mat.set(r1,M_PI * 2.0f * phase);

				acsOutRect3D(r.x,r.y,r.z,sx,_sy,cl,0,1,2,mat);
			}
		}
	}
}

void aciScreenScroller::Change(int x)
{
	x -= PosX;
	CurValue = (x * MaxValue) / SizeX;

	if(CurValue < 0) CurValue = 0;
	if(CurValue > MaxValue) CurValue = MaxValue;

	flags |= ACS_REDRAW_OBJECT;
}

void aciScreenInputField::SetAlpha(int v)
{
	if(dAlpha){
		AlphaDest = v;
		dAlpha = (v >= AlphaCur) ? abs(dAlpha) : -abs(dAlpha);
	}
	else
		AlphaCur = v;
}

void aciScreenScroller::SetAlpha(int v)
{
	if(dAlpha){
		AlphaDest = v;
		dAlpha = (v >= AlphaCur) ? abs(dAlpha) : -abs(dAlpha);
	}
	else
		AlphaCur = v;
}

void aciScreenInputField::redraw(int flag2d)
{
	int x,y,sx,sy,al;
	aciScreenResourcePAL* pl;

	float z,sc;

	aciScreenObject::redraw(flag2d);
	pl = (aciScreenResourcePAL*)palPtr;

	if(RenderMode == XGRAPH_HICOLOR && !pl -> flags & ACS_LOADED) 
		pl -> load();

	if(flag2d){
		if(!(flags & ACS_TEXT)){
			if(!(flags & ACS_ALIGN_CENTER)){
				if(acsHighColor){
					if(AlphaCur != -1){
						al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
						if(RenderMode == DIRECT3D_HICOLOR)
							mchA_d3dOutString(PosX,PosY + acsY0,mchA_FontScaleX[font],mchA_FontScaleY[font],string,mchA_ColorF[Color],255 - al,font,Space);
						else
							acsOutStr16_a2(PosX,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space,al);
					}
					else {
						if(RenderMode == DIRECT3D_HICOLOR)
							mchA_d3dOutString(PosX,PosY + acsY0,mchA_FontScaleX[font],mchA_FontScaleY[font],string,mchA_ColorF[Color],255,font,Space);
						else
							acsOutStr16(PosX,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space);
					}
				}
				else
					acsOutStr(PosX,PosY + acsY0,font,Color,(unsigned char*)string,Space);
			}
			else {
				sx = acsStrLen(font,(unsigned char*)string,Space);
				if(acsHighColor){
					if(AlphaCur != -1){
						al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
						if(RenderMode == DIRECT3D_HICOLOR)
							mchA_d3dOutString(PosX + (SizeX - sx)/2,PosY + acsY0,mchA_FontScaleX[font],mchA_FontScaleY[font],string,mchA_ColorF[Color],255 - al,font,Space);
						else
							acsOutStr16_a2(PosX + (SizeX - sx)/2,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space,al);
					}
					else {
						if(RenderMode == DIRECT3D_HICOLOR)
							mchA_d3dOutString(PosX + (SizeX - sx)/2,PosY + acsY0,mchA_FontScaleX[font],mchA_FontScaleY[font],string,mchA_ColorF[Color],255,font,Space);
						else
							acsOutStr16(PosX + (SizeX - sx)/2,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space);
					}
				}
				else
					acsOutStr(PosX + (SizeX - sx)/2,PosY + acsY0,font,Color,(unsigned char*)string,Space);
			}
		}
		else {
			if(!acsHighColor) return;

			if(!(flags & ACS_ALIGN_CENTER)){
				if(AlphaCur != -1){
					al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
					acsOutText16_a2(PosX,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space,al);
				}
				else
					acsOutText16(PosX,PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space);
			}
			else {
				sx = acsTextStrLenMax(font,(unsigned char*)string,Space);
				sy = acsTextHeight(font,(unsigned char*)string,Space);

				x = (SizeX - sx)/2;
				y = (SizeY - sy)/2;

				if(AlphaCur != -1){
					al = AlphaCur + (((256 - AlphaCur) * acsAlpha) >> 8);
					acsOutText16_a2(x + PosX,y + PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space,al);
				}
				else
					acsOutText16(x + PosX,y + PosY + acsY0,font,Color,(unsigned char*)string,pl -> data16,Space);
			}
		}
	}
	else {
		if(acsAlphaDelta) return;

		if(AlphaCur != -1)
			al = AlphaCur;	// + (((256 - AlphaCur) * acsAlpha) >> 8);
		else
			al = 256;

		z = (float)al / 8.0f;

		sc = 1.0f + (float)al * 0.2f / 256.0f;

		acsStrLen3D(string,font,Space,sx,sy);

		x = round(((float)SizeX - (float)sx * sc)/2);
		y = (SizeY - sy)/2;

		if(flags & ACS_ALIGN_CENTER)
			acsOutString3D(x + PosX,y + PosY,string,font,Color,Space,0,z,sc);
		else
			acsOutString3D(PosX,y + PosY,string,font,Color,Space,0,z,sc);
	}
}

void aciScreenDispatcher::ChangeScroller(int obj_id,int x)
{
	aciScreenScroller* p = (aciScreenScroller*)GetObject(obj_id);
	p -> Change(x);
}

void aciScreenDispatcher::DecScroller(int obj_id)
{
	int delta;
	aciScreenScroller* p = (aciScreenScroller*)GetObject(obj_id);

	delta = p -> MaxValue / 20;
	if(!delta) delta = 1;

	p -> CurValue -= delta;

	if(p -> CurValue < 0) p -> CurValue = 0;
	if(p -> CurValue > p -> MaxValue) p -> CurValue = p -> MaxValue;
}

void aciScreenDispatcher::IncScroller(int obj_id)
{
	int delta;
	aciScreenScroller* p = (aciScreenScroller*)GetObject(obj_id);

	delta = p -> MaxValue / 20;
	if(!delta) delta = 1;

	p -> CurValue += delta;

	if(p -> CurValue < 0) p -> CurValue = 0;
	if(p -> CurValue > p -> MaxValue) p -> CurValue = p -> MaxValue;
}

void aciScreenDispatcher::SetScroller(int obj_id,int v,int max_v)
{
	aciScreenScroller* p = (aciScreenScroller*)GetObject(obj_id);
	if(!max_v) max_v = 1;
	p -> CurValue = (p -> MaxValue * v) / max_v;
	p -> flags |= ACS_REDRAW_OBJECT;
}

void aciScreenScroller::init(void)
{
	int x,y;
	aciScreenObject::init();
	ScrRes = acsGetResource(ScrResID);
	if(ScrRes)
		acsGetResourceSize(ScrRes,x,y,ScrResSX,ScrResSY);

	sRes = acsGetResource(sResID);
	if(sRes)
		acsGetResourceSize(sRes,x,y,sResSX,sResSY);
}

void aciScreenDispatcher::InputQuant(int code,int chr)
{
	int sz;
	char* p;
	unsigned char* ptr = NULL;
	HFont* hfnt = NULL;

	if(!activeInput) return;

	ptr = (unsigned char*)activeInput -> string;
	if(activeInput -> flags & ACS_ISCREEN_FONT)
		hfnt = acsFntTable[activeInput -> font];

	if(acsInputMode != ACS_KEY_INPUT_MODE){
		switch(code){
			case VK_RETURN:
				DoneInput();
				break;
			case VK_ESCAPE:
				CancelInput();
				break;
			case VK_LEFT:
			case VK_BACK:
				sz = strlen((char*)ptr);
				if(sz > 1){
					ptr[sz - 1] = 0;
					ptr[sz - 2] = '_';
					activeInput -> flags |= ACS_REDRAW_OBJECT;
				}
				break;
			default:
				if(chr){
					if(hfnt && (hfnt -> data[chr] -> Flags & NULL_HCHAR) && chr != ' ')
						break;
					sz = strlen((char*)ptr);
					if(sz <= activeInput -> MaxStrLen){
						ptr[sz - 1] = chr;
						ptr[sz] = '_';
						ptr[sz + 1] = 0;
						activeInput -> flags |= ACS_REDRAW_OBJECT;
					}
				}
				break;
		}
	}
	else {
		if(code == VK_ESCAPE){
			CancelInput();
			return;
		}
		if(code != VK_F7 && code != VK_F8 && code != VK_F9 && code != VK_F10 && code != VK_F11 && code != VK_F12){
			p = mchGetKeyNameText(code);
			if(p){
				strcpy(activeInput -> string,p);
				acsLastKey = code;
				DoneInput();
			}
		}
	}
}

void aciScreenDispatcher::PrepareInput(int obj_id)
{
	int sz;
	aciScreenInputField* p = (aciScreenInputField*)GetObject(obj_id);
	if(p){
		if(activeInput){
			activeInput -> StopEvents();
			CancelInput();
		}
		sz = strlen(p -> string) + 1;
		if(!acsBackupStr){
			acsBackupStr = new char[sz];
			strcpy(acsBackupStr,p -> string);
		}

		if(acsInputMode == ACS_KEY_INPUT_MODE || (p -> MaxState && !strcmp(p -> string,p -> statePtr[1]))){
			*p -> string = 0;
			sz = 1;
		}

		p -> string[sz - 1] = '_';
		p -> string[sz] = 0;
		p -> flags |= ACS_ACTIVE_STRING;
		activeInput = p;

		XKey.LastChar = -1;
	}
}

void aciScreenInputField::inc_state(void)
{
	if(++CurState > MaxState) CurState = 0;
	set_state(CurState);
	flags |= ACS_REDRAW_OBJECT;
}

void aciScreenInputField::dec_state(void)
{
	if(--CurState < 0) CurState = MaxState;
	set_state(CurState);
	flags |= ACS_REDRAW_OBJECT;
}

void aciScreenInputField::alloc_str(void)
{
	int i;
	string = new char[MaxStrLen + 2];
	memset(string,0,MaxStrLen + 2);
	
	if(MaxState){
		statePtr = new char*[MaxState + 1];
		for(i = 0; i < MaxState + 1; i ++){
			statePtr[i] = new char[MaxStrLen + 2];
			memset(statePtr[i],0,MaxStrLen + 2);
		}
	}
}

int aciScreenDispatcher::GetObjectValue(int obj_id,int flag)
{
	int ret = 0;
	aciScreenObject* p = GetObject(obj_id);
	if(p){
		switch(p -> type){
			case ACS_BASE_OBJ:
				if(p -> curFrame) ret = 1;
				break;
			case ACS_SCROLLER_OBJ:
				if(!flag)
					ret = ((aciScreenScroller*)p) -> CurValue;
				else
					ret = ((aciScreenScroller*)p) -> MaxValue;
				break;
		}
	}
	return ret;
}

void aciScreenDispatcher::CancelInput(void)
{
	strcpy(activeInput -> string,acsBackupStr);
	activeInput -> flags |= ACS_REDRAW_OBJECT;
	activeInput -> flags &= ~ACS_ACTIVE_STRING;
	activeInput = NULL;
	delete acsBackupStr;
	acsBackupStr = NULL;
	acsCancelInputFlag = 1;
}

void aciScreenDispatcher::DoneInput(void)
{
	int sz;
	sz = strlen(activeInput -> string);

	if(acsInputMode != ACS_KEY_INPUT_MODE)
		activeInput -> string[sz - 1] = 0;

	activeInput -> flags |= ACS_REDRAW_OBJECT;
	activeInput -> flags &= ~ACS_ACTIVE_STRING;
	activeInput = NULL;
	delete acsBackupStr;
	acsBackupStr = NULL;
	acsCancelInputFlag = 0;
}


void aciScreenScroller::Quant(int mx,int my)
{
	int al = Alpha0;
	aciScreenEvent* ev;

	aciScreenObject::Quant(mx,my);

	if(CheckXY(XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2,XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2)){
		if(!(flags & (ACS_HIDDEN | ACS_BLOCKED)) && Alpha1 != Alpha0 && AlphaCur == Alpha0)
			mchStartSoundEFF(EFF_TOGGLE);

		if(Alpha1)
			al = Alpha1;

		if(acsMouseLPFlag){
			ev = (aciScreenEvent*)events -> fPtr;
			while(ev){
				if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(VK_MOUSE_LBUTTON))
					ev -> Start();
				ev = (aciScreenEvent*)ev -> next;
			}
		}
		if(acsMouseRPFlag){
			ev = (aciScreenEvent*)events -> fPtr;
			while(ev){
				if(!(ev -> flags & ACS_EVENT_ACTIVE) && ev -> KeyTrap(VK_MOUSE_RBUTTON))
					ev -> Start();
				ev = (aciScreenEvent*)ev -> next;
			}
		}
	}

	if(al != -1) SetAlpha(al);
	if(AlphaDest != -1){
		if(abs(AlphaDest - AlphaCur) < abs(dAlpha)){
			AlphaCur = AlphaDest;
			AlphaDest = -1;
		}
		else {
			AlphaCur += dAlpha;
		}
		flags |= ACS_REDRAW_OBJECT;
	}

	if(AlphaCur < 0) AlphaCur = 0;
	if(AlphaCur > 256) AlphaCur = 256;
}

void aciScreenInputField::Quant(int mx,int my)
{
	int prev = Color,al;
	Color = Color0;
	al = Alpha0;

	aciScreenObject::Quant(mx,my);

	//if(mx == -1 || my == -1){
		mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
		my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;
	//}

	if(!(flags & ACS_ACTIVE_STRING)){
		if(Color1 != -1 && CheckXY(mx,my)){
			Color = Color1;
			al = Alpha1;

			if(!(flags & (ACS_HIDDEN | ACS_BLOCKED)) && Alpha1 != Alpha0 && AlphaCur == Alpha0)
				mchStartSoundEFF(EFF_TOGGLE);
			}
	}
	else {
		if(Color2 != -1){
			Color = Color2;
			al = Alpha2;
		}
	}

	if(flags & ACS_BLOCKED){
		Color = Color0;
		al = Alpha0;
	}

	if(prev != Color) flags |= ACS_REDRAW_OBJECT;

	if(al != -1) SetAlpha(al);
	if(AlphaDest != -1){
		if(abs(AlphaDest - AlphaCur) < abs(dAlpha)){
			AlphaCur = AlphaDest;
			AlphaDest = -1;
		}
		else {
			AlphaCur += dAlpha;
		}
		flags |= ACS_REDRAW_OBJECT;
	}

	if(AlphaCur < 0) AlphaCur = 0;
	if(AlphaCur > 256) AlphaCur = 256;
}

void aciScreenInputField::init(void)
{
	aciScreenObject::init();
	Color = Color0;
	flags &= ~ACS_ACTIVE_STRING;
}

void aciScreenInputField::change_state(void)
{
	if(!strcmp(string,acsSTR_OFF)){
		strcpy(string,acsSTR_ON);
	}
	else {
		strcpy(string,acsSTR_OFF);
	}
	flags |= ACS_REDRAW_OBJECT;
}

void aciScreenInputField::set_state(int v)
{
	if(v <= MaxState){
		strcpy(string,statePtr[v]);
		CurState = v;
	}
	flags |= ACS_REDRAW_OBJECT;
}

int aciScreenInputField::get_state(void)
{
	return CurState;
}

void acsChangeStrState(int id)
{
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(id);
	if(p) p -> change_state();
}

void acsSetStrState(int id,int state)
{
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(id);
	if(p) p -> set_state(state);
}

int acsGetStrState(int id)
{
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(id);
	if(p) return p -> get_state();
	return 0;
}

void aciScreenDispatcher::ChangeScreen(int id)
{
	if(activeInput){ 
		if(acsInputMode == ACS_KEY_INPUT_MODE)
			CancelInput();
		else
			DoneInput();
	}
	acsDisableEvents = 0;

	curScr -> free_mem();
#ifndef _ACI_3D_VIEW_
	curScr = GetScreen(id);

	curScr -> prepare();
	curScr -> CenterMouse();
	curScr -> alloc_mem();
	curScr -> init_objects();

	curScr -> backRes = GetResource(curScr -> backResID);

	flags &= ~ACS_NEED_EXIT;
	flags |= ACS_FORCED_REDRAW;
	flags |= ACS_FIRST_REDRAW;

	KeyTrap(ACS_STARTUP_KEY);

	acsExtInit();
	EventQuant();
#else
	curScr = nextScr;
#endif
	acsSetAlphaSeq(0,-ACS_ALPHA_SPEED);
}

#ifdef _ACS_MOVE_OBJECTS_
void aciScreenObject::show_bound(void)
{
	int x,y,sx,sy;
	int col = 0xFFFF;

	if(RenderMode == XGRAPH_HICOLOR){
		if(acsMoveObj && acsMoveObj -> ID == ID)
			col =  XGR_RGB64K(31,0,0);

		x = PosX + 1;
		y = PosY + 1;

		sx = SizeX - 2;
		sy = SizeY - 2;

		XGR_SetClipMode(1);

		if(acsHighColor)
			XGR_Rectangle16(x,y,sx,sy,col,col,XGR_OUTLINED);
		else
			XGR_Rectangle(x,y,sx,sy,0xFF,0xFF,XGR_OUTLINED);

		XGR_SetClipMode(0);
	}
	else {
		x = round(float(PosX) / mchA_d3dResX);
		y = round(float(PosY) / mchA_d3dResY);
		sx = round(float(SizeX) / mchA_d3dResX);
		sy = round(float(SizeY) / mchA_d3dResY);
		
		mchA_d3dRectangle(x,y,sx,sy,7,1);
	}
}

void aciScreenDispatcher::DumpMovedObjects(void)
{
	aciScreenObject* p;
	aciScreen* scr = (aciScreen*)scrList -> fPtr;

	XStream fh("iScreen\\SCRIPTS\\TEMP\\acs_dump.txt",XS_OUT);
	while(scr){
		p = (aciScreenObject*)scr -> objList -> fPtr;
		while(p){
			if(p -> flags & ACS_MOVED_OBJECT)
				fh <= scr -> ID < "\t" <= p -> ID < "\t" <= p -> PosX < "\t" <= p -> PosY < "\r\n";
			p = (aciScreenObject*)p -> next;
		}
		scr = (aciScreen*)scr -> next;
	}
	fh.close();
}

aciScreenObject* aciScreenDispatcher::GetMouseObject(void)
{
	aciScreenObject* p = (aciScreenObject*)curScr -> objList -> fPtr;
	while(p){
		if(p -> CheckXY(XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2,XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2)) return p;
		p = (aciScreenObject*)p -> next;
	}
	return NULL;
}
#endif

void acsSetAlphaSeq(int dest,int delta)
{
	acsAlphaDest = dest;
	acsAlphaDelta = delta;
}

void acsAlphaQuant(void)
{
	if(acsAlphaDelta){
		if(abs(acsAlpha - acsAlphaDest) <= abs(acsAlphaDelta)){
			acsAlpha = acsAlphaDest;
			acsAlphaDelta = 0;
			acsAlphaEvent = 0;
			if(acsAlphaData != -1){
				acsScrD -> ChangeScreen(acsAlphaData);
				acsAlphaData = -1;
			}
		}
		else
			acsAlpha += acsAlphaDelta;

		acsScrD -> flags |= ACS_FORCED_REDRAW;
	}
}

void aciScreenDispatcher::PrepareChangeScreen(int id)
{
	acsAlphaData = id;
	acsSetAlphaSeq(256,ACS_ALPHA_SPEED);
	acsAlphaEvent = 1;

#ifdef _ACI_3D_VIEW_
	aciScreen* p;

	nextScrID = id;
	nextScr = GetScreen(id);

	nextScr -> prepare();
	nextScr -> CenterMouse();
	nextScr -> alloc_mem();
	nextScr -> init_objects();

	nextScr -> backRes = GetResource(nextScr -> backResID);

	flags &= ~ACS_NEED_EXIT;
	flags |= ACS_FORCED_REDRAW;
	flags |= ACS_FIRST_REDRAW;

	p = curScr;
	curScr = nextScr;
	KeyTrap(ACS_STARTUP_KEY);

	acsExtInit();
	EventQuant();
	FlushEvents();

	curScr = p;
	acsPrepareChangeScreen3D(id);
#endif
}

void aciScreenObject::Quant(int mx,int my)
{
//	if(mx == -1 || my == -1){
		mx = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
		my = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;
//	}

	if(!(flags & ACS_HIDDEN) && CheckXY(mx,my)){
		acsMouseMaxZ = -60.0f;
		acsMouseObj = this;
	}
}

void aciScreenInputField::AdjustSize(void)
{
#ifdef _ACI_3D_VIEW_
	int sx,sy,dx,dy;
	if(flags & ACS_NO_RESIZE) return;

	acsStrLen3D(string,font,Space,sx,sy);

	dx = sx - SizeX;
	dy = sy - SizeY;

	if(flags & ACS_ALIGN_CENTER){
		PosX += (SizeX - sx) / 2;
		SizeX = sx + 4;
	}
	if(!SizeY) SizeY = sy;
/*
	if(abs(dx) > 5){
		PosX += dx / 2;
		SizeX = sx;
	}
	if(abs(dy) > 5){
		PosY += dy / 2;
		SizeY = sy;
	}
*/
//	flags |= ACS_NO_RESIZE;
#endif
}

void aciScreen::CenterMouse(void)
{
	int x,y;

	x = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
	y = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

	int dx,dy,cx,cy,dist,dist0;
	aciScreenObject* p,*p0,*p1;

	p0 = p1 = NULL;

	p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(p -> flags & ACS_KEYB_SELECT && (!(p -> flags & ACS_BLOCKED) || p -> flags & ACS_MOUSE_SELECT) && p -> CheckXY(x,y)){
			p1 = p;
			break;
		}
		p = (aciScreenObject*)p -> next;
	}

	if(!p1){
		p = (aciScreenObject*)objList -> fPtr;
		while(p){
			if(p -> flags & ACS_KEYB_SELECT && (!(p -> flags & ACS_BLOCKED) || p -> flags & ACS_MOUSE_SELECT) && !p -> CheckXY(x,y)){
				cx = p -> PosX + p -> SizeX/2;
				cy = p -> PosY + p -> SizeY/2;

				dx = x - cx;
				dy = y - cy;
				dist = dx * dx + dy * dy;

				if(!p1 || dist < dist0){
					dist0 = dist;
					p1 = p;
				}
			}
			p = (aciScreenObject*)p -> next;
		}
	}

	p = p1;

	if(p){
		x = p -> PosX + p -> SizeX/2 - XGR_MouseObj.SizeX/2;
		y = p -> PosY + p -> SizeY/2 - XGR_MouseObj.SizeY/2;

		XGR_MouseObj.SetPos(x,y);
	}
}

aciScreenObject* aciScreen::GetObjectXY(int x,int y,int dir,int coords_mode)
{
	int dx,dy,cx,cy,dist,dist0,dist1;
	aciScreenObject* p,*p0,*p1;

	p0 = p1 = NULL;

	p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(p -> flags & ACS_KEYB_SELECT && !(p -> flags & ACS_HIDDEN) && p -> CheckXY(x,y)){
			p1 = p;
			break;
		}
		p = (aciScreenObject*)p -> next;
	}

	if(p1 && dir != -1 && !p1 -> objIndex[dir])
		return NULL;

	if(p1 && dir != -1 && p1 -> objIndex[dir]){
		p0 = GetObject(p1 -> objIndex[dir]);
		while(p0 && (p0 -> flags & ACS_HIDDEN || (p0 -> flags & ACS_BLOCKED && !(p0 -> flags & ACS_MOUSE_SELECT)))){
			p0 = GetObject(p0 -> objIndex[dir]);
		}
		if(p0)
			return p0;
	}

	p = (aciScreenObject*)objList -> fPtr;
	while(p){
		if(p -> flags & ACS_KEYB_SELECT && !(p -> flags & ACS_HIDDEN) && (!(p -> flags & ACS_BLOCKED) || p -> flags & ACS_MOUSE_SELECT) && !p -> CheckXY(x,y)){
			cx = p -> PosX + p -> SizeX/2;
			cy = p -> PosY + p -> SizeY/2;

			dist1 = 1;

			switch(dir){
				case 0: // left
					dist = x - cx;
					if(p1 && abs(p1 -> PosY - p -> PosY) >= (p -> SizeY + p1 -> SizeY)/2 - 3)
						dist1 = 0;
					break;
				case 1: // up
					dist = y - cy;
					if(p1 && abs(p1 -> PosX - p -> PosX) >= (p -> SizeX + p1 -> SizeX)/2 - 3)
						dist1 = 0;
					break;
				case 2: // right
					dist = cx - x;
					if(p1 && abs(p1 -> PosY - p -> PosY) >= (p -> SizeY + p1 -> SizeY)/2 - 3)
						dist1 = 0;
					break;
				case 3: // down
					dist = cy - y;
					if(p1 && abs(p1 -> PosX - p -> PosX) >= (p -> SizeX + p1 -> SizeX)/2 - 3)
						dist1 = 0;
					break;
				default:
					dx = x - cx;
					dy = y - cy;
					dist = dx * dx + dy * dy;
					dist1 = 1;
					break;
			}

			if(!coords_mode) dist1 = 1;
			if(dist1 && dist >= 0 && (!p0 || dist < dist0)){
				dist0 = dist;
				p0 = p;
			}
		}
		p = (aciScreenObject*)p -> next;
	}
	return p0;
}

void aciScreenDispatcher::MouseQuant(void)
{
	int x,y;
	float dx,dy,dist;
	if(acsMouseTargetX == -1 && acsMouseTargetY == -1)
		return;

	x = XGR_MouseObj.PosX;
	y = XGR_MouseObj.PosY;

	dx = acsMouseTargetX - x;
	dy = acsMouseTargetY - y;
	dist = sqrt(dx * dx + dy * dy);

	if(dist > acsMouseSpeed){
		dx *= acsMouseSpeed / dist;
		dy *= acsMouseSpeed / dist;
		
		XGR_MouseObj.SetPos(x + round(dx),y + round(dy));
	}
	else {
		XGR_MouseObj.SetPos(acsMouseTargetX,acsMouseTargetY);
		acsMouseTargetX = acsMouseTargetY = -1;
	}
}

void aciScreenObject::scale_coords(float x,float y)
{
	int sx,sy,dx,dy;

	sx = 640.0f * x;
	sy = 480.0f * y;

	if(x != 1){
		switch(align_x){
			case 0:
				PosX = PosX0;
				break;
			case -1:
				PosX = round((float)PosX0 * x);
				break;
			case 1:
				dx = 640 - PosX0 - SizeX0;
				PosX = sx - SizeX0 - round((float)dx * x);
				break;
		}
		PosY = PosY0;
	}
	else
		PosX = PosX0;

	if(y != 1){
		switch(align_y){
			case 0:
				PosY = PosY0;
				break;
			case -1:
				PosY = round((float)PosY0 * y);
				break;
			case 1:
				dy = 640 - PosY0 - SizeY0;
				PosY = sy - SizeY0 - round((float)dy * y);
				break;
		}
		PosX = PosX0;
	}
	else
		PosY = PosY0;
}

void aciScreenDispatcher::SetNextScrID(int scr_id,int id)
{
	int id1;
	aciScreen* p;

	p = (scr_id == -1) ? curScr : GetScreen(scr_id);
	id1 = (id == -1) ? curScr -> ID : id;
	if(p) p -> nextID = id1;
}

void aciScreenDispatcher::SetPrevScrID(int scr_id,int id)
{
	int id1;
	aciScreen* p;

	p = (scr_id == -1) ? curScr : GetScreen(scr_id);
	id1 = (id == -1) ? curScr -> ID : id;
	if(p) p -> prevID = id1;
}

