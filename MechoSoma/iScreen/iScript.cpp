/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "HFONT.H"

#include "ACI_EVNT.H"
#include "ACI_SCR.H"

#include "SCR_DEFS.H"
#include "aci_parser.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int acsNumFonts;
extern char* acsFontDir;
extern char* acsFontPrefix;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(const char* fname,XStream& fh,int res_flag = -1);

void acsLoadScript(const char* fname);

aciScreenResource* acsCreateResource(scrDataBlock* p);
aciScreenKeyObject* acsCreateKeyObj(scrDataBlock* p);
aciScreenEventCommand* acsCreateCommand(scrDataBlock* p);
aciScreenEvent* acsCreateEvent(scrDataBlock* p);
aciScreenFrameSequence* acsCreateFrameSeq(scrDataBlock* p);

aciScreenObject* acsCreateObject(scrDataBlock* p);
void acsInitInputFieldObj(scrDataBlock* p,aciScreenInputField* obj);
void acsInitScrollerObj(scrDataBlock* p,aciScreenScroller* obj);

aciScreen* acsCreateScreen(scrDataBlock* p);

/* --------------------------- DEFINITION SECTION --------------------------- */

scrDataBlock* acsScrRoot = NULL;

void acsLoadScript(const char* fname)
{
	XStream fh;

	scrDataBlock* p;

	acsOpenResource(fname,fh);
	acsScrRoot = loadScript(fh);

	acsScrD = new aciScreenDispatcher;
	p = acsScrRoot -> nextLevel -> first();
	while(p){
		switch(p -> ID){
			case ASCR_SCREEN:
				acsScrD -> AddScreen(acsCreateScreen(p));
				break;
			case ASCR_RESOURCE:
				acsScrD -> AddResource(acsCreateResource(p));
				break;
			case ASCR_FONTS_DIR:
				acsFontDir = new char[strlen(p -> c_dataPtr) + 1];
				strcpy(acsFontDir,p -> c_dataPtr);
				break;
			case ASCR_FONTS_PREFIX:
				acsFontPrefix = new char[strlen(p -> c_dataPtr) + 1];
				strcpy(acsFontPrefix,p -> c_dataPtr);
				break;
			case ASCR_FONTS_NUM:
				acsNumFonts = *p -> i_dataPtr;
				break;
		}
		p = p -> next;
	}
}

aciScreenResource* acsCreateResource(scrDataBlock* p)
{
	int type = ACS_BMP;
	scrDataBlock* b;

	aciScreenResource* res = NULL;
	aciScreenResourceBMP* resBMP = NULL;
	aciScreenResourceBML* resBML = NULL;
	aciScreenResourceBMO* resBMO = NULL;
	aciScreenResourceXBM* resXBM = NULL;
	aciScreenResourceHBM* resHBM = NULL;
	aciScreenResourcePAL* resPAL = NULL;

	b = p -> find(ASCR_TYPE);
	if(b) type = *b -> i_dataPtr;

	switch(type){
		case ACS_BMP:
			resBMP = new aciScreenResourceBMP;
			res = resBMP;
			break;
		case ACS_BML:
			resBML = new aciScreenResourceBML;
			res = resBML;
			break;
		case ACS_BMO:
			resBMO = new aciScreenResourceBMO;
			res = resBMO;
			break;
		case ACS_XBM:
			resXBM = new aciScreenResourceXBM;
			res = resXBM;
			break;
		case ACS_PAL:
			resPAL = new aciScreenResourcePAL;
			res = resPAL;
			break;	
		case ACS_HBM:
			resHBM = new aciScreenResourceHBM;
			res = resHBM;
			break;

	}
	if(!res) ErrH.Abort("Undefined resource type...");

	b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_ID:
				res -> ID = *b -> i_dataPtr;
				break;
			case ASCR_FILE_NAME:
				res -> set_name(b -> c_dataPtr);
				break;
			case ASCR_PALETTE_ID:
				res -> palID = *b -> i_dataPtr;
				break;
			case ASCR_POS_X:
				if(resBMO) resBMO -> OffsX = *b -> i_dataPtr;
				break;	
			case ASCR_POS_Y:
				if(resBMO) resBMO -> OffsY = *b -> i_dataPtr;
				break;
			case ASCR_SIZE_X:
				if(resBMO) resBMO -> SizeX = *b -> i_dataPtr;
				break;	
			case ASCR_SIZE_Y:
				if(resBMO) resBMO -> SizeY = *b -> i_dataPtr;
				break;
			case ASCR_FLAG:
				res -> flags |= *b -> i_dataPtr;
				break;	
		}
		b = b -> next;
	}


	return res;
}

aciScreenKeyObject* acsCreateKeyObj(scrDataBlock* p)
{
	scrDataBlock* b;
	aciScreenKeyObject* key = new aciScreenKeyObject;
	b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_CODE:
				key -> AddKey(*b -> i_dataPtr,0);
				break;
			case ASCR_OPT_CODE:
				key -> AddKey(*b -> i_dataPtr,1);
				break;
		}
		b = b -> next;
	}
	return key;
}

aciScreenEventCommand* acsCreateCommand(scrDataBlock* p)
{
	int i;
	scrDataBlock* b;
	aciScreenEventCommand* comm = new aciScreenEventCommand;
	b = p -> find(ASCR_CODE);
	if(b) comm -> code = *b -> i_dataPtr;
	b = p -> find(ASCR_EV_COMM_DATA);
	if(b){
		for(i = 0; i < 4; i ++) comm -> data[i] = b -> i_dataPtr[i];
	}
	b = p -> find(ASCR_START_TIME);
	if(b) comm -> StartTimer = *b -> i_dataPtr;
	return comm;
}

aciScreenEvent* acsCreateEvent(scrDataBlock* p)
{
	aciScreenEvent* ev = new aciScreenEvent;
	aciScreenEventCommand* cm;

	scrDataBlock* b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_EV_COMMAND:
				cm = acsCreateCommand(b);
				ev -> AddCommand(cm);
				break;
			case ASCR_CODE:
				ev -> AddKey(*b -> i_dataPtr,0);
				break;
			case ASCR_OPT_CODE:
				ev -> AddKey(*b -> i_dataPtr,1);
				break;
			case ASCR_TIME:
				ev -> MaxTimer = *b -> i_dataPtr;
				break;
		}
		b = b -> next;
	}
	return ev;
}

aciScreenFrameSequence* acsCreateFrameSeq(scrDataBlock* p)
{
	scrDataBlock* b;
	aciScreenFrameSequence* seq = new aciScreenFrameSequence;

	b = p -> find(ASCR_ID);
	if(b) seq -> ID = *b -> i_dataPtr;

	b = p -> find(ASCR_RES_ID);
	if(b) seq -> ResourceID = *b -> i_dataPtr;

	b = p -> find(ASCR_START_TIME);
	if(b) seq -> StartFrame = *b -> i_dataPtr;

	b = p -> find(ASCR_END_TIME);
	if(b) seq -> EndFrame = *b -> i_dataPtr;

	b = p -> find(ASCR_DELTA);
	if(b) seq -> FrameDelta = *b -> i_dataPtr;

	return seq;
}

aciScreenObject* acsCreateObject(scrDataBlock* p)
{
	int type = ACS_BASE_OBJ;
	scrDataBlock* b;

	aciScreenObject* obj = NULL;
	aciScreenInputField* f_obj;
	aciScreenScroller* s_obj;

	b = p -> find(ASCR_TYPE);
	if(b) type = *b -> i_dataPtr;
	switch(type){
		case ACS_BASE_OBJ:
			obj = new aciScreenObject;
			break;
		case ACS_INPUT_FIELD_OBJ:
			f_obj = new aciScreenInputField;
			acsInitInputFieldObj(p,f_obj);
			obj = f_obj;
			break;
		case ACS_SCROLLER_OBJ:
			s_obj = new aciScreenScroller;
			acsInitScrollerObj(p,s_obj);
			obj = s_obj;
			break;
	}

	obj -> type = type;
	b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_ID:
				obj -> ID = *b -> i_dataPtr;
				break;
			case ASCR_RES_ID:
				obj -> defResID = *b -> i_dataPtr;
				break;
			case ASCR_FON_RES_ID:
				obj -> fonResID = *b -> i_dataPtr;
				break;
			case ASCR_POS_X:
				obj -> PosX = *b -> i_dataPtr;
				break;
			case ASCR_POS_Y:
				obj -> PosY = *b -> i_dataPtr;
				break;
			case ASCR_SIZE_X:
				obj -> SizeX = *b -> i_dataPtr;
				break;
			case ASCR_SIZE_Y:
				obj -> SizeY = *b -> i_dataPtr;
				break;
			case ASCR_PALETTE_ID:
				obj -> PalID = *b -> i_dataPtr;
				break;
			case ASCR_FLAG:
				obj -> flags |= *b -> i_dataPtr;
				break;
			case ASCR_EVENT:
				obj -> AddEvent(acsCreateEvent(b));
				break;
			case ASCR_FRAME_SEQ:
				obj -> AddSeq(acsCreateFrameSeq(b));
				break;
			case ASCR_OBJ_INDEX:
				obj -> objIndex[b -> i_dataPtr[0]] = b -> i_dataPtr[1];
				break;
			case ASCR_ALIGN_X:
				obj -> align_x = b -> i_dataPtr[0];
				break;
			case ASCR_ALIGN_Y:
				obj -> align_y = b -> i_dataPtr[0];
				break;
		}
		b = b -> next;
	}
	//@caiiiycuk: no impl
	//obj -> backup_coords();
	return obj;
}

void acsInitInputFieldObj(scrDataBlock* p,aciScreenInputField* obj)
{
	int idx = 0;
	scrDataBlock* b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_FONT:
				obj -> font = *b -> i_dataPtr;
				break;
			case ASCR_ALPHA0:
				obj -> AlphaCur = obj -> Alpha0 = *b -> i_dataPtr;
				break;
			case ASCR_ALPHA1:
				obj -> Alpha1 = *b -> i_dataPtr;
				break;
			case ASCR_ALPHA2:
				obj -> Alpha2 = *b -> i_dataPtr;
				break;
			case ASCR_D_ALPHA:
				obj -> dAlpha = *b -> i_dataPtr;
				break;
			case ASCR_SPACE:
				obj -> Space = *b -> i_dataPtr;
				break;
			case ASCR_COLOR0:
				obj -> Color0 = *b -> i_dataPtr;
				break;
			case ASCR_COLOR1:
				obj -> Color1 = *b -> i_dataPtr;
				break;
			case ASCR_COLOR2:
				obj -> Color2 = *b -> i_dataPtr;
				break;
			case ASCR_MAX_STATE:
				obj -> MaxState = *b -> i_dataPtr;
				break;
			case ASCR_CUR_STATE:
				obj -> CurState = *b -> i_dataPtr;
				break;
			case ASCR_STR_LEN:
				obj -> MaxStrLen = *b -> i_dataPtr;
				break;	
		}
		b = b -> next;
	}
	obj -> alloc_str();
	b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_STATE_STR:
				strcpy(obj -> statePtr[idx++],b -> c_dataPtr);
				break;
			case ASCR_STRING:
				obj -> set_string(b -> c_dataPtr);
				break;
		}
		b = b -> next;
	}

	if(obj -> MaxState)
		obj -> set_state(obj -> CurState);

	if(obj -> Color1 != obj -> Color0)
		obj -> flags |= ACS_KEYB_SELECT;
}

void acsInitScrollerObj(scrDataBlock* p,aciScreenScroller* obj)
{
	scrDataBlock* b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_ALPHA0:
				obj -> AlphaCur = obj -> Alpha0 = *b -> i_dataPtr;
				break;
			case ASCR_ALPHA1:
				obj -> Alpha1 = *b -> i_dataPtr;
				break;
			case ASCR_D_ALPHA:
				obj -> dAlpha = *b -> i_dataPtr;
				break;
			case ASCR_CUR_VALUE:
				obj -> CurValue = *b -> i_dataPtr;
				break;
			case ASCR_MAX_VALUE:
				obj -> MaxValue = *b -> i_dataPtr;
				break;
			case ASCR_COLOR0:
				obj -> Color = *b -> i_dataPtr;
				break;
			case ASCR_COLOR1:
				obj -> BackColor = *b -> i_dataPtr;
				break;
		}
		b = b -> next;
	}
	obj -> flags |= ACS_KEYB_SELECT;
}

aciScreen* acsCreateScreen(scrDataBlock* p)
{
	aciScreen* scr = new aciScreen;
	scrDataBlock* b = p -> nextLevel -> first();
	while(b){
		switch(b -> ID){
			case ASCR_ID:
				scr -> ID = *b -> i_dataPtr;
				break;
			case ASCR_NEXT_SCREEN:
				scr -> nextID = *b -> i_dataPtr;
				break;
			case ASCR_PREV_SCREEN:
				scr -> prevID = *b -> i_dataPtr;
				break;
			case ASCR_RES_ID:
				scr -> backResID = *b -> i_dataPtr;
				break;
			case ASCR_FLAG:
				scr -> flags |= *b -> i_dataPtr;
				break;
			case ASCR_OBJECT:
				scr -> AddObject(acsCreateObject(b));
				break;
			case ASCR_RESOURCE:
				scr -> AddResource(acsCreateResource(b));
				break;
			case ASCR_EVENT:
				scr -> AddEvent(acsCreateEvent(b));
				break;
		}
		b = b -> next;
	}
	return scr;
}


