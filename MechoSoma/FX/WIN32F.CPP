#pragma hdrstop

#include <windows.h>

//#define BETA_TESTING

//#undef NDEBUG
//#define _DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <crtdbg.h>
#include <malloc.h>
#include <assert.h>

#include "_xtool.h"
#include "win32f.h"
#include "AllocationTracking.h"

#include "mch_common.h" // For far target

void PrintError(char*,char*);

static HANDLE hSingularEvent = 0;
static char psSingularEventName[] = "SingleMechosoma";

extern int MainRqCounter,AuxRqCounter;
extern void* XGR_hWnd;
extern int MAX_LINE;


#ifdef _DEBUG
static int allocation_size = 0;
static int allocation_counter = 0;
static int allocation_size_stamp = 0;
static int allocation_counter_stamp = 0;

/*
void allocation_tracking_clear()
{
	allocation_size_stamp = allocation_size; 
	allocation_counter_stamp = allocation_counter;
}

void allocation_tracking(char* title)
{
	allocation_tracking(title, 0);
}

void allocation_tracking(char* title, AllocationAccumulator* state)
{
	static XStream out;
	static int opened;
	if(!opened){
		opened = 1;
		out.open("memory.lst", XS_OUT);
		out < "\n\tMemory allocation listing.\n\n";
		out < "-------------------------------------------------------------------------------\n";
		out < "  Allocated before Title  |        Total   Allocated  | \n";
		out < "                          |                           |    Title\n";
		out < "    Size   Blocks  BSize  |     Size   Blocks  BSize  |\n";
		out < "-------------------------------------------------------------------------------\n";
		}
	int size = allocation_size - allocation_size_stamp;
	int cnt = allocation_counter - allocation_counter_stamp;
	if(state){
		size = state -> size += size;
		cnt = state -> blocks += cnt;
		}
	char buf[2048];
	sprintf(buf, "%10i  %5i  %6i | %10i  %5i  %6i | %s \n", size, cnt, cnt ? size/cnt : 0, allocation_size, allocation_counter, allocation_counter ? allocation_size/allocation_counter : 0, title);
	out < buf;
	allocation_size_stamp = allocation_size; 
	allocation_counter_stamp = allocation_counter;
}
*/
#else
void allocation_tracking_clear(){}
void allocation_tracking(char* title){}
void allocation_tracking(char* title, AllocationAccumulator* state){}
#endif

	
#ifdef _DEBUG

#define nNoMansLandSize 4
typedef struct _CrtMemBlockHeader
{
        struct _CrtMemBlockHeader * pBlockHeaderNext;
        struct _CrtMemBlockHeader * pBlockHeaderPrev;
        char *                      szFileName;
        int                         nLine;
        size_t                      nDataSize;
        int                         nBlockUse;
        long                        lRequest;
        unsigned char               gap[nNoMansLandSize];
        /* followed by:
         *  unsigned char           data[nDataSize];
         *  unsigned char           anotherGap[nNoMansLandSize];
         */
} _CrtMemBlockHeader;

#define pHdr(pbData) (((_CrtMemBlockHeader *)pbData)-1)

int __cdecl MyAllocHook(  int  nAllocType,  void   * pvData,  unsigned int   nSize,  int      nBlockUse,  long     lRequest,  const unsigned char * szFileName,  int      nLine  )
{
/*   char *operation[] = { "", "allocating", "re-allocating", "freeing" };
   char *blockType[] = { "Free", "Normal", "CRT", "Ignore", "Client" };*/

   /*if ( nBlockUse == _CRT_BLOCK )   // Ignore internal C runtime library allocations
      return( 1 );*/

	switch(nAllocType){
		case _HOOK_REALLOC:
			{
				_CrtMemBlockHeader *pHead=pHdr(pvData);
				int dSize=(int)nSize-pHead->nDataSize;
				allocation_size +=dSize;
				break;
			}
		case _HOOK_ALLOC:   
			{
				allocation_size += nSize;
				allocation_counter++;
				break;
			}
		case _HOOK_FREE:   
			{
				_CrtMemBlockHeader *pHead=pHdr(pvData);
				nSize=pHead->nDataSize;
				allocation_size -= nSize;
				allocation_counter--;
				break;
			}
		default:{
			_CrtMemBlockHeader *pHead=pHdr(pvData);
			break;
			}

   }

   return( 1 );         // Allow the memory operation to proceed
}
#endif

void win32_debugSet(void)
{
#ifdef _DEBUG
/*
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	int flags = 0;

	flags |= _CRTDBG_ALLOC_MEM_DF;	// включает проверку heap'а
//	flags |= _CRTDBG_CHECK_ALWAYS_DF;		// проверка heap'а при каждом выделении памяти
	flags |= _CRTDBG_CHECK_CRT_DF;			// проверка внутреннего runtime-heap'а
//	  flags |= _CRTDBG_DELAY_FREE_MEM_DF;	// не освобождать на самом деле память
//	  flags |= _CRTDBG_LEAK_CHECK_DF;			// ловить лики
	int log = _CrtSetDbgFlag(flags);
*/
	_CrtSetAllocHook( &MyAllocHook );
#endif
}

void win32_check(void)
{
	_ASSERTE(_CrtCheckMemory()) ;
}

void win32_getMemStat(int& max,int& cur)
{
#ifdef _DEBUG
	_CrtMemState state;
	_CrtMemCheckpoint(&state);
	max = state.lHighWaterCount;
	cur = state.lTotalCount;
#endif
}

#ifdef _DEBUG
int deltaMTotal,curMTotal;
int win32_getMTotal(void)
{
	static _CrtMemState state;

//#define VAL	state.lHighWaterCount
#define VAL	state.lTotalCount

	curMTotal = VAL;
	_CrtMemCheckpoint(&state);
	deltaMTotal = VAL - curMTotal;
	return VAL;
}
#endif

int win32_SingleCheck(void)
{
	SetConsoleTitle("Mechosoma Console");

	hSingularEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, psSingularEventName);
	if(0 == hSingularEvent)
	{
		hSingularEvent = CreateEvent(0, TRUE, TRUE, psSingularEventName);
		return 1;
	};
	return 0;
};

int win32_GetProcessPriority(void)
{
	return GetPriorityClass(GetCurrentProcess());
}

void win32_SetPriorityProcess(int mode)
{
//	_amblksiz = 1024;
	SetPriorityClass(GetCurrentProcess(),mode);
}

/*
void win32_SetWorkingSet(int& rmin,int& rmax)
{
	unsigned long min,max;
	SetProcessWorkingSetSize(GetCurrentProcess(),12*1024*1024,26*1024*1024);
	GetProcessWorkingSetSize(GetCurrentProcess(),&min,&max);
	rmin = min;
	rmax = max;
}
*/

int win32_VLock(void* p,int size)
{
	int ret = 0;
	if(VirtualLock(p,size) == FALSE) ret = GetLastError();
	return ret;
}

void win32_VUnLock(void* p,int size)
{
	VirtualUnlock(p,size);
}

void* win32_FileMirror(int h,int size,int close)
{
	static HANDLE fmh;
	static void* p;

	if(!close){
		fmh = CreateFileMapping((HANDLE)h,NULL,PAGE_READONLY,0,0,NULL);
		if(fmh == NULL) return NULL;
		p = MapViewOfFile(fmh,FILE_MAP_READ,0,0,0);
		}
	else {
		UnmapViewOfFile(p);
		CloseHandle(fmh);
		}
	return p;

//	DWORD oldP;
//	if(!VirtualProtect(p,size,PAGE_READWRITE,&oldP)) return NULL;
}

static HANDLE fmhv;
void* win32_VMPMirrorON(int h)
{
	fmhv = CreateFileMapping((HANDLE)h,NULL,PAGE_READWRITE,0,0,NULL);
	if(fmhv == NULL) return NULL;
	return MapViewOfFile(fmhv,FILE_MAP_ALL_ACCESS,0,0,0);
}

void win32_VMPMirrorOFF(void* p)
{
	UnmapViewOfFile(p);
	CloseHandle(fmhv);
}

void win32_MainWinMinimize(void)
{
//	ShowWindow(XGR_hWnd,SW_SHOWMINIMIZED);
}

int win32_MT_vmpread(int h,void* p,int size,int offset)
{
	unsigned long ret;
	if(SetFilePointer((HANDLE)h,offset,NULL,FILE_BEGIN) == 0xFFFFFFFF) return 1;
	if(ReadFile((HANDLE)h,p,size,&ret,NULL) == FALSE) return 2;
	return 0;
}

static WIN32_FIND_DATA FFdata;
static HANDLE FFh;

char* win32_findnext(void)
{
	if(FindNextFile(FFh,&FFdata) == TRUE){
		if(FFdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return win32_findnext();
		return FFdata.cFileName;
		}
	else {
		FindClose(FFh);
		return NULL;
		}
}

char* win32_findfirst(char* mask)
{
	FFh = FindFirstFile(mask,&FFdata);
	if(FFh == INVALID_HANDLE_VALUE) return NULL;
	if(FFdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return win32_findnext();
	return FFdata.cFileName;
}

char* GetINIstring(const char* iniFile,const char* section,const char* key)
{
//	static char buffer[64];
	static char buffer[256];
	GetPrivateProfileString(section,key,"",buffer,64,iniFile);
	return buffer;
}

char* getOS(int& val)
{
   DWORD dwVersion;
   static char szVersion[80];

   dwVersion = GetVersion();

   if (dwVersion < 0x80000000) {
   // Windows NT
       wsprintf (szVersion, "Microsoft Windows NT %u.%u (Build: %u)",
		(DWORD)(LOBYTE(LOWORD(dwVersion))),
		(DWORD)(HIBYTE(LOWORD(dwVersion))),
		(DWORD)(HIWORD(dwVersion)));
	val = 0;
   }
   else if (LOBYTE(LOWORD(dwVersion))<4) {
   // Win32s
       wsprintf (szVersion, "Microsoft Win32s %u.%u (Build: %u)",
		(DWORD)(LOBYTE(LOWORD(dwVersion))),
		(DWORD)(HIBYTE(LOWORD(dwVersion))),
		(DWORD)(HIWORD(dwVersion) & ~0x8000));
	val = -1;
   } else {
   // Windows 95
       wsprintf (szVersion, "Microsoft Windows 95 %u.%u (Build: %u)",
		(DWORD)(LOBYTE(LOWORD(dwVersion))),
		(DWORD)(HIBYTE(LOWORD(dwVersion))),
		(DWORD)(HIWORD(dwVersion) & ~0x8000));
	val = 1;
   }
	return szVersion;
}

char win32_msgbuf[256];
char* win32_errmsg(int id)
{
	char* msg = win32_msgbuf;
	memset(msg,0,256);
	int ret = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		id,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		msg,
		256,
		NULL
	);
	return msg;
}

static __int64 FileTimeToQW(PFILETIME pFileTime)
{
	__int64 qw;
	qw = pFileTime -> dwHighDateTime;
	qw <<= 32;
	qw |= pFileTime -> dwLowDateTime;
	return qw;
}

static FILETIME ftKernelStart,ftKernelEnd;
static FILETIME ftUserStart,ftUserEnd;

void win32StartTimer(void)
{
	FILETIME ftDummy;
	GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&ftKernelStart,&ftUserStart);
//	GetProcessTimes(GetCurrentProcess(),&ftDummy,&ftDummy,&ftKernelStart,&ftUserStart);
}

int win32GetTimer(void)
{
	FILETIME ftDummy;
	GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&ftKernelEnd,&ftUserEnd);
//	GetProcessTimes(GetCurrentProcess(),&ftDummy,&ftDummy,&ftKernelEnd,&ftUserEnd);
	__int64 tm = /*(FileTimeToQW(&ftKernelEnd) - FileTimeToQW(&ftKernelStart)) + */(FileTimeToQW(&ftUserEnd) - FileTimeToQW(&ftUserStart));
	return (int)tm;
}

void win32_dump_mem(void)
{
	HANDLE fh = CreateFile("memory.dmp",
				GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				0,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
				0);

	_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN,fh);
	_CrtSetReportMode(_CRT_ERROR,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR,fh);
	_CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT,fh);

	_CrtMemDumpAllObjectsSince(NULL);
	CloseHandle(fh);
}

/*
char* getVideoPath(void)
{
	DWORD len;
	HKEY key;
	LONG ret;
	
#if defined(RUSSIAN_VERSION) && !defined(GERMAN_VERSION)
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Buka\\Vangers\\Release",0,KEY_QUERY_VALUE,&key);
#else
	ret = RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE\\I-Magic\\Vangers",0,KEY_QUERY_VALUE,&key);
#endif
#ifdef BETA_TESTING
	if(ret != ERROR_SUCCESS) ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Interactive Magic\\Vangers Beta\\1.00",0,KEY_QUERY_VALUE,&key);
#endif
	if(ret != ERROR_SUCCESS) return NULL;

	ret = RegQueryValueEx(key,"AVIPath",NULL,NULL,NULL,&len);
	if(ret) return NULL;
	unsigned char* path = new unsigned char[len + 2];
	ret = RegQueryValueEx(key,"AVIPath",NULL,NULL,path,&len);
	return (char*)path;
}

#ifdef BETA_TESTING
char* getOwner(void)
{
	DWORD len;
	HKEY key;
	LONG ret;
	int release = 1;
		
	ret = RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE\\I-Magic\\Vangers",0,KEY_QUERY_VALUE,&key);
	if(ret != ERROR_SUCCESS){
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Interactive Magic\\Vangers Beta\\1.00",0,KEY_QUERY_VALUE,&key);
		release = 0;
		}
	if(ret != ERROR_SUCCESS) return NULL;

	int n = 0;
	ret = RegQueryValueEx(key,"UserName",NULL,NULL,NULL,&len);
	if(ret != ERROR_SUCCESS) return NULL;
	char* user = new char[len + 1];
	n += len;
	ret = RegQueryValueEx(key,"UserName",NULL,NULL,(unsigned char*)user,&len);

	ret = RegQueryValueEx(key,"UserCompany",NULL,NULL,NULL,&len);
	if(ret != ERROR_SUCCESS) return NULL;
	char* company = new char[len + 1];
	n += len;
	ret = RegQueryValueEx(key,"UserCompany",NULL,NULL,(unsigned char*)company,&len);

	char* serial = NULL;
	if(!release){
		ret = RegQueryValueEx(key,"Serial",NULL,NULL,NULL,&len);
		if(ret != ERROR_SUCCESS) return NULL;
		serial = new char[len + 1];
		n += len;
		ret = RegQueryValueEx(key,"Serial",NULL,NULL,(unsigned char*)serial,&len);
		}

	char* buf = new char[n + 64];
	if(release)
		strcpy(buf,"Gamer: ");
	else
		strcpy(buf,"Beta Tester: ");
	strcat(buf,user);
	strcat(buf," [");
	strcat(buf,company);
	if(!release){
		strcat(buf,"] Internal Code: ");
		strcat(buf,serial);
		}
	else
		strcat(buf,"]");
	if(strlen(buf) > 700/8) buf[700/8] = '\0';
	
	return (char*)buf;
}
#endif

#ifdef _ROAD_
int getCDdrives(void)
{
	DWORD d = GetLogicalDrives();
	DWORD mask = 1;
	char* str = "???";
	str[2] = '\\';
	str[1] = ':';
	for(int i = 0;i < 26;i++){
		if(d & mask){
			*str = 'A' + i;
			if(GetDriveType(str) != DRIVE_CDROM) d &= ~mask;
			}
		mask <<= 1;
		}
	return d;
}

extern char* roadFNTname;
extern char* XGraphWndID;
extern char* mapFName;
extern char* iVideoPathDefault;
extern int SkipCD;
int isCDok(int drive)
{
	char* str = new char[64];
	char* label = new char[64];
	char* system = new char[64];

	str[2] = '\\';
	str[1] = ':';
	*str = 'A' + drive;
	str[3] = '\0';
	int ret = 1;

	BOOL rt = GetVolumeInformation(	str,
									label,64,
									NULL,NULL,
									NULL,system,64);

	if(system[3] != 'S' || system[1] != 'D' || system[2] != 'F' || system[0] != 'C') ret = 0;
	if(ret) if(stricmp(label,XGraphWndID)) ret = 0;
	
	if(ret){
#if defined(RUSSIAN_VERSION) && !defined(GERMAN_VERSION)
		strcat(str,XGraphWndID);
		strcat(str,".");
		strcat(str,"ico");
		strlwr(str);
		DWORD a = GetFileAttributes(str);
		if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;
		if(ret){
			str[3] = '\0';
			strcat(str,iVideoPathDefault + 9);
			strcat(str,"INTRO");
			strcat(str,"\\logo2");
			strcat(str,".avi");
			DWORD a = GetFileAttributes(str);
			if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;
			}
#else
		strcat(str,roadFNTname);
		DWORD a = GetFileAttributes(str);
		if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;
		if(ret){
			str[3] = '\0';
			strcat(str,mapFName);
			DWORD a = GetFileAttributes(str);
			if(a == 0xFFFFFFFF || !(a & FILE_ATTRIBUTE_READONLY)) ret = 0;
			}
#endif
		}

	delete str;
	delete label;
	delete system;
	return SkipCD ? 1 : ret;
}
#endif
*/
