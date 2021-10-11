#ifndef __WIN32_H__
#define __WIN32_H__

int win32_SingleCheck(void);
void win32_debugSet(void);
void win32_getMemStat(int& max,int& cur);
void win32_check(void);
int win32_getMTotal(void);
void win32_dump_mem(void);

#ifdef _DEBUG
//#define DBGCHECK	win32_check();
#define DBGCHECK	
#else
#define DBGCHECK
#endif

int win32_GetProcessPriority(void);
void win32_SetPriorityProcess(int mode);
void win32_SetWorkingSet(int& rmin,int& rmax);
int win32_VLock(void* p,int size);
void win32_VUnLock(void* p,int size);

void* win32_FileMirror(int h,int size,int close = 0);
void* win32_VMPMirrorON(int h);
void win32_VMPMirrorOFF(void* p);

char* win32_findfirst(char* mask);
char* win32_findnext(void);

char* win32_errmsg(int id);

char* GetINIstring(const char* iniFile,const char* section,const char* key);
char* getOS(int& val);

void allocation_tracking(char*);

void win32_shell_execute(char* command);

void* win32_load_icon(void);

char* win32_get_path_from_regkey(int key_id,char* subkey_name,char* value_name);

#endif // __WIN32_H__
