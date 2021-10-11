#include <windows.h>

extern HWND g_hWnd;

#define _NOSTD_

void dprintf(char *format, ...);

//õ¸ûø b=false òvòþôøª ¸þþñ•õýøõ ø òv¿þôøª ø÷ ÿ¨þó¨ðüüv
void ERRORM(BOOL b, char *format=NULL, ...);

int __cdecl AssertFail(char szErr[], char szFileName[], int nLine, char szMessage[]);
#define MDebugBreak() __asm { int 3 }
#define ASSERT(f) \
		do {\
			if(!(f))\
			{\
				if(AssertFail(#f, __FILE__, __LINE__,NULL))\
				{\
					MDebugBreak();\
				}\
			}\
		} while(0)
#define VERIFY(x) ASSERT(x)

#include "..\TextConverter\parse\paramblock.h"

#ifndef _DEBUG
#undef ASSERT
#undef VERIFY
#define ASSERT( expr ) ( (void)0 )
#define VERIFY(x) (x)
#endif _DEBUG


#include <LIMITS.H>

#include "array.h"

#define SIZE(ar) (sizeof(ar)/sizeof(ar[0]))

#define PI    3.14159265358979323846
inline int round(double x){return (int)(x+0.5);}
inline float frand(){return rand()/(float)RAND_MAX;}
