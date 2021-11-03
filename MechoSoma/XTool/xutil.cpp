#include "xtool.h"

#include <string>

void* XGR_hWnd;

XList::XList(void)
{
	ListSize = 0;
	fPtr = NULL;
	lPtr = NULL;
}

XList::~XList(void)
{
}

void XList::AddElement(XListElement* p)
{
}

void XList::RemoveElement(XListElement* p)
{
}

char* XFindFirst(char* mask)
{
	return nullptr;
}

char* XFindNext(void)
{
	return nullptr;
}

void xtDeleteFile(char* fname)
{
}

void xtCreateRuntimeObjectTable(int len)
{
}

void xtRegisterRuntimeObject(XRuntimeObject* p)
{
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

unsigned int XRnd(unsigned int m)
{
	return 0;
}

void initclock(void)
{
}

int clocki(void)
{
	return 0;
}

double clockf(void)
{
	return 0.0;
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
