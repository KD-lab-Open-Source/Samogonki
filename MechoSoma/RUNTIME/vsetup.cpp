
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "aci_ids.h"
#include "aci_scr.h"

#include "..\Resource.h"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern char* mch_optionsINI;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void mchGraphicsSetup(void);
BOOL APIENTRY vsDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);
void vsInitDlg(HWND hdlg);
void vsFinitDlg(HWND hdlg);

void vsLowerStr(char* p);

void acsLoadScript(char* fname);

char* getIniKey(char* fname,char* section,char* key);
void putIniKey(char* fname,char* section,char* key,char* val);
void putIniKey(char* fname,char* section,char* key,int val);

/* --------------------------- DEFINITION SECTION --------------------------- */

BOOL APIENTRY vsDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int wmID;

	switch(msg){
		case WM_INITDIALOG:
			vsInitDlg(hdlg);
			return 1;
		case WM_COMMAND:
			wmID = LOWORD(wParam);
			switch(wmID){
				case IDOK:
					vsFinitDlg(hdlg);
					EndDialog(hdlg,1);
					return 1;
				case IDCANCEL:
					EndDialog(hdlg,0);
					return 1;
			}
			break;
	}
	return 0;
}

void mchGraphicsSetup(void)
{
	DialogBox((HINSTANCE)xtGet_hInstance(),MAKEINTRESOURCE(IDD_GRAPHICS),NULL,vsDlgProc);
	ErrH.Exit();
}

void vsInitDlg(HWND hdlg)
{
	int i,id;
	static char str[256];

	aciScreen* sp;
	aciScreenInputField* p,*p1;

	acsLoadScript("iscreen.scb");

	sp = (aciScreen*)acsScrD -> GetScreen(ACS_MAIN_MENU_SCR0);
	if(!sp) return;
	p = (aciScreenInputField*)sp -> GetObject(ACS_OPTIONS_OPTION);

	sp = (aciScreen*)acsScrD -> GetScreen(ACS_OPTIONS_SCR);
	if(!sp) return;
	p1 = (aciScreenInputField*)sp -> GetObject(ACS_GRAPHICS_OPTION);

	if(p && p1){
		sprintf(str,"%s / %s",p -> string,p1 -> string);
		vsLowerStr(str);
		SendMessage(hdlg,WM_SETTEXT,(WPARAM)0,(LPARAM)str);
	}

	sp = (aciScreen*)acsScrD -> GetScreen(ACS_GRAPHICS_SCR);
	if(!sp) return;

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_SCREEN_RES1);
	if(p){
		for(i = 0; i <= p -> MaxState; i ++)
			SendDlgItemMessage(hdlg,IDC_RESOLUTION,CB_INSERTSTRING,(WPARAM)-1,(LPARAM)p -> statePtr[i]);
	}

	id = atoi(getIniKey(mch_optionsINI,"current","resolution"));
	SendDlgItemMessage(hdlg,IDC_RESOLUTION,CB_SETCURSEL,(WPARAM)id,(LPARAM)0);

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_COLOR_DEPTH);
	if(p){
		strcpy(str,p -> string);
//		vsLowerStr(str);
		SendDlgItemMessage(hdlg,IDC_COLOR_DEPTH_TEXT,WM_SETTEXT,(WPARAM)0,(LPARAM)str);
	}

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_SCREEN_RES);
	if(p)
		SendDlgItemMessage(hdlg,IDC_RESOLUTION_TEXT,WM_SETTEXT,(WPARAM)0,(LPARAM)p -> string);

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_COLOR_DEPTH1);
	if(p){
		for(i = 0; i <= p -> MaxState; i ++)
			SendDlgItemMessage(hdlg,IDC_COLOR_DEPTH,CB_INSERTSTRING,(WPARAM)-1,(LPARAM)p -> statePtr[i]);
	}

	id = atoi(getIniKey(mch_optionsINI,"current","color_depth"));
	SendDlgItemMessage(hdlg,IDC_COLOR_DEPTH,CB_SETCURSEL,(WPARAM)id,(LPARAM)0);

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_FULLSCR);
	if(p){
		strcpy(str,p -> string);
//		vsLowerStr(str);
		SendDlgItemMessage(hdlg,IDC_MODE_TEXT,WM_SETTEXT,(WPARAM)0,(LPARAM)str);
	}

	p = (aciScreenInputField*)sp -> GetObject(ACS_GR_FULLSCR1);
	if(p){
		for(i = 0; i <= p -> MaxState; i ++)
			SendDlgItemMessage(hdlg,IDC_MODE,CB_INSERTSTRING,(WPARAM)-1,(LPARAM)p -> statePtr[i]);
	}

	id = atoi(getIniKey(mch_optionsINI,"current","fullscreen"));
	SendDlgItemMessage(hdlg,IDC_MODE,CB_SETCURSEL,(WPARAM)id,(LPARAM)0);

	p = (aciScreenInputField*)sp -> GetObject(ACS_BACK_OPTION);
	if(p)
		SendDlgItemMessage(hdlg,IDOK,WM_SETTEXT,(WPARAM)0,(LPARAM)p -> string);

	sp = (aciScreen*)acsScrD -> GetScreen(ACS_LOAD_GAME_SCR);
	if(!sp) return;

	p = (aciScreenInputField*)sp -> GetObject(ACS_BACK_OPTION);
	if(p){
		strcpy(str,p -> string);
		vsLowerStr(str);
		SendDlgItemMessage(hdlg,IDCANCEL,WM_SETTEXT,(WPARAM)0,(LPARAM)str);
	}
}

void vsFinitDlg(HWND hdlg)
{
	int id;

	id = SendDlgItemMessage(hdlg,IDC_RESOLUTION,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	putIniKey(mch_optionsINI,"current","resolution",id);

	id = SendDlgItemMessage(hdlg,IDC_COLOR_DEPTH,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	putIniKey(mch_optionsINI,"current","color_depth",id);

	id = SendDlgItemMessage(hdlg,IDC_MODE,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	putIniKey(mch_optionsINI,"current","fullscreen",id);
}

void vsLowerStr(char* p)
{
	int i,sz,chr;

	sz = strlen(p);

	unsigned char* up = (unsigned char*)p;

	for(i = 1; i < sz; i ++){
		chr = tolower(up[i]);
		p[i] = chr;
	}
}
