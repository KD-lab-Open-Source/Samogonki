/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "xINI_File.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void xINI_Init(void);
int xINI_PutKey(const char* fname,const char* section,const char* key,const char* val);
char* xINI_GetKey(const char* fname,const char* section,const char* key);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define _xINI_ENABLE_

#ifdef _xINI_ENABLE_
int xINI_Enable = 1;
#else
int xINI_Enable = 0;
#endif

int xINI_InitFlag = 0;

xtList<xINI_File>* xINI_List = NULL;

void xINI_Init(void)
{
#ifdef _xINI_ENABLE_
	xINI_File* p;

	xINI_List = new xtList<xINI_File>;

	p = new xINI_File("game.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\options.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\ISCREEN\\camera.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\ISCREEN\\iscreen.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\ISCREEN\\local.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\hotseat.ini");
	xINI_List -> append(p);

	p = new xINI_File("RESOURCE\\bots.ini");
	xINI_List -> append(p);

	p = xINI_List -> first();
	while(p){
		p -> load();
		p = p -> next;
	}

	xINI_InitFlag = 1;
#endif
}

int xINI_PutKey(const char* fname,const char* section,const char* key,const char* val)
{
	xINI_File* p;
	p = xINI_List -> first();
	while(p){
		if(!stricmp(p -> getName(),fname)){
			p -> put_key(section,key,val);
			p -> save();
			return 1;
		}
		p = p -> next;
	}
	return 0;
}

char* xINI_GetKey(const char* fname,const char* section,const char* key)
{
	xINI_File* p;
	p = xINI_List -> first();
	while(p){
		if(!stricmp(p -> getName(),fname)){
			return p -> get_key(section,key);
		}
		p = p -> next;
	}
	return NULL;
}
