#include "xtool.h"

#include <chrono>
#include <string>

void* XGR_hWnd;

#define XT_DEFAULT_TABLE_SIZE 32

XRuntimeObject** XRObjTable = NULL;
unsigned int XRObjTableSize = 0;

XRuntimeObject* XRObjFirst = NULL;
XRuntimeObject* XRObjLast = NULL;

XList::XList(void)
{
	ClearList();
}

XList::~XList(void)
{
}

void XList::AddElement(XListElement* p)
{
	if (!fPtr)
	{
		fPtr = lPtr = p;
		p->prev = p;
		p->next = NULL;
	}
	else
	{
		lPtr->next = p;
		p->prev = lPtr;
		p->next = NULL;
		lPtr = p;
		fPtr -> prev = p;
	}
	ListSize ++;
}

void XList::RemoveElement(XListElement* p)
{
	XListElement* pPtr;
	XListElement* nPtr;

	ListSize--;
	if (ListSize)
	{
		pPtr = p->prev;
		nPtr = p->next;

		pPtr->next = nPtr;
		if (nPtr)
		{
			nPtr->prev = pPtr;
		}

		if (p == fPtr)
		{
			fPtr = nPtr;
		}

		if (p == lPtr)
		{
			lPtr = pPtr;
		}

		lPtr->next = NULL;
		fPtr->prev = lPtr;
	}
	else
	{
		ClearList();	
	}
}

char* XFindFirst(const char* mask)
{
	return nullptr;
}

char* XFindNext(void)
{
	return nullptr;
}

void xtDeleteFile(const char* fname)
{
}

void xtCreateRuntimeObjectTable(int len)
{
	int i;
	if(!len) len = XT_DEFAULT_TABLE_SIZE;
	XRObjTableSize = len;
	XRObjTable = new XRuntimeObject*[len];

	for(i = 0; i < len; i ++){
		XRObjTable[i] = NULL;
	}
}

void xtRegisterRuntimeObject(XRuntimeObject* p)
{
	if(!XRObjFirst){
		XRObjFirst = XRObjLast = p;
	}
	else {
		XRObjLast -> next = p;
		XRObjLast = p;
	}
	XRObjTable[p -> ID - 1] = p;
}

XRuntimeObject* xtGetRuntimeObject(int id)
{
	if(id == XT_TERMINATE_ID)
		return NULL;
	if(!XRObjTable || !XRObjTableSize || id < 1 || id > XRObjTableSize)
		ErrH.Abort("XTool system error...");
	return XRObjTable[id - 1];
}

int xtIsActive(void)
{
	return 0;
}

void xtClearMessageQueue(void)
{
}

void xtSysQuantDisable(int v)
{
}

static unsigned int XRndValue = 83838383;

unsigned int XRnd(unsigned int m)
{
	XRndValue = XRndValue * 214013L + 2531011L;
	if(!m){
		return 0;
	}
	return ((XRndValue>> 16) & 0x7fff) % m;
}

using ClockType = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<ClockType>;
TimePoint start_time;

void initclock(void)
{
	start_time = std::chrono::high_resolution_clock::now();
}

int clocki(void)
{
	const auto d = std::chrono::high_resolution_clock::now() - start_time;
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
	return static_cast<int>(ms.count());
}

double clockf(void)
{
	const auto d = std::chrono::high_resolution_clock::now() - start_time;
	return std::chrono::duration<double>(d).count();
}

void* xtGet_hInstance(void)
{
	return nullptr;
}

void xtRegisterSysFinitFnc(void (*fPtr)(void),int id)
{
}

void xtDeactivateSysFinitFnc(int id)
{
}

void mchGraphicsSetup(void)
{
}

std::string win32_get_path_from_regkey(int key_id,char* subkey_name,char* value_name)
{
	return "";
}

int win32_GetKeybLayoutID(void)
{
    return 0;
}

void win32_InitLocale(void)
{
}

void* win32_load_icon(void)
{
	return nullptr;
}

void* GetDS_Ptr(void)
{
	return nullptr;
}
