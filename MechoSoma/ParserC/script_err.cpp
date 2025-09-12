/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include <stdlib.h>
#include <stdio.h>

#include "script_err.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

void (*script_ErrFnc)(const char*) = 0;

int scriptErrMode = 1;

void scriptSetErrorOutHandler(void (*hfnc)(const char*))
{
	script_ErrFnc = hfnc;
}

void scriptSetErrorMode(int mode)
{
	scriptErrMode = mode;
}

void scriptError(const char* msg)
{
	if(script_ErrFnc)
		(*script_ErrFnc)(msg);
	else
		printf(msg);

	if(scriptErrMode)
		exit(1);
}

void scriptError(const char* msg,int code,const char* text)
{
	static char msg_buf[1024];
	if(text)
		sprintf(msg_buf,msg,code,text);
	else
		sprintf(msg_buf,msg,code);

	scriptError(msg_buf);
}
