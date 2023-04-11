
#include <windows.h>
#include <process.h>
#include <crtdbg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <_xtool.h>
#include <xmath.h>
#include <KEYS.H>


/* @caiiiycuk

This file conatins functions with dummy implementations, because we didn't find any implementation.
Also some function in source base are mocked to work. Most of them are located in Crystal.cpp, aci_scr.cpp,
and some other places.

*/

namespace {
const char *sizeMSG	= "FILE SIZE CALCULATION ERROR";
}

long XStream::size() {
	long tmp = extSize;
	
	if(tmp == -1){
		tmp=GetFileSize(handler,0);
		if (tmp == -1L)
			if (ErrHUsed) ErrH.Abort(sizeMSG,XERR_USER,GetLastError(),fname);
				 else return -1;
	}
	return tmp;
}

void fxCannonFire(Vect3f pos,Vect3f vel,float pow) {
}

void KeyBuffer::copy(KeyBuffer* p) {
	p->size = size;
	p->table = table;
	p->state_table = state_table;
	p->flag = flag;
	p->first_index = first_index;
	p->last_index = last_index;
}

void* win32_load_icon(void) {
	return NULL;
}

void win32_shell_execute(char* command) {
}

std::string win32_get_path_from_regkey(int key_id,const char* subkey_name,const char* value_name) {
	DWORD dwType = REG_SZ;
	HKEY hKey = 0;
	char value[1024];
	DWORD value_length = 1024;
	const char* subkey = "Software\\SamoGonki";
	RegOpenKey(HKEY_CURRENT_USER,subkey,&hKey);
	RegQueryValueEx(hKey, "SourcePath", NULL, &dwType, (LPBYTE)&value, &value_length);
	return std::string(value);
}