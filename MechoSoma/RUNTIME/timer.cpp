
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void xsSetReadHandler(void (*fp)(unsigned),unsigned res);

void mchInitLoadTimer(void);
void mchFinitLoadTimer(void);
void mchSetLoadTimeMax(unsigned v);

void mchA_InitLoadingScreen(void);
void mchA_DrawLoadingScreen(int x,int y,int sx,int sy,int val,int max_val);
void mchDrawScrollbar(int x,int y,int sx,int sy,int v,int max_v);

void mchLoadTimerFunc(unsigned sz);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define TM_X		20
#define TM_Y		20

#define TM_SX		600
#define TM_SY		3

int timerCol0[3] = { 224, 100, 0 };
int timerCol1[3] = { 252, 180, 0 };

unsigned timerValueMax = 2300000U;

void mchInitLoadTimer(void)
{
	int x,y;

	mchA_d3dCreateBackBuffer();

	mchA_InitLoadingScreen();

	mchSetLoadTimeMax(10000U);
//	xsSetReadHandler(mchLoadTimerFunc,500000U);

	x = TM_X;
	y = 480 - TM_SY - TM_Y;
	mchA_DrawLoadingScreen(x,y,TM_SX,TM_SY,0,100);
	mchA_d3dFlip();
}

void mchLoadTimerFunc(unsigned sz)
{
	int x,y;
	unsigned max = timerValueMax;
	if(xtIsActive()){
		x = TM_X;
		y = 480 - TM_SY - TM_Y;

		if(sz >= max)
			sz = max - 1;

		mchA_DrawLoadingScreen(x,y,TM_SX,TM_SY,sz,max);
		mchA_d3dFlip();
	}

	xtClearMessageQueue();
}

void mchFinitLoadTimer(void)
{
	int x,y;

	xsSetReadHandler(NULL,0);

	if(RenderMode == XGRAPH_HICOLOR){
		x = TM_X;
		y = XGR_MAXY - TM_SY - TM_Y;

		XGR_Rectangle16(x,y,TM_SX,TM_SY,0,0,XGR_FILLED);
		XGR_Flush(x,y,TM_SX,TM_SY);
	}
	else {
		x = TM_X;
		y = 480 - TM_SY - TM_Y;
		mchA_DrawLoadingScreen(x,y,TM_SX,TM_SY,100,100);
		mchA_d3dFlip();
//		mchA_d3dReleaseBackBuffer();
	}

	xtClearMessageQueue();
}

void mchDrawScrollbar(int x,int y,int sx,int sy,int v,int max_v)
{
	int sx1,cl0,cl1;

	if(XGR_HighColorMode){
		cl0 = XGR_RGB64K(timerCol0[0] >> 3,timerCol0[1] >> 3,timerCol0[2] >> 3);
		cl1 = XGR_RGB64K(timerCol1[0] >> 3,timerCol1[1] >> 3,timerCol1[2] >> 3);
	}
	else {
		cl0 = XGR_RGB64K(timerCol0[0] >> 3,timerCol0[1] >> 2,timerCol0[2] >> 3);
		cl1 = XGR_RGB64K(timerCol1[0] >> 3,timerCol1[1] >> 2,timerCol1[2] >> 3);
	}

	sx1 = round((float)sx * (float)v / (float)max_v);

	XGR_Rectangle16(x,y,sx,sy,cl0,cl0,XGR_FILLED);
	XGR_Rectangle16(x,y,sx1,sy,cl1,cl1,XGR_FILLED);
	XGR_Flush(x,y,TM_SX,TM_SY);
}

void mchSetLoadTimeMax(unsigned v)
{
	timerValueMax = v;
}
