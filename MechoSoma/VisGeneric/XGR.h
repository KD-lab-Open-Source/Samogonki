#ifndef __XGR_H__
#define __XGR_H__

#define GET_RENDERMODE(a)	((a)&0xFFFF0000)
#define XGRAPH_HICOLOR		(0)
#define DIRECT3D_HICOLOR	(1<<16)

#include "xgraph.h"

#ifdef XGR_Init

#undef XGR_Init
#undef XGR_ReInit
#undef XGR_Finit
#undef XGR_SetClip
#undef XGR_GetClip
#undef XGR_SetClipMode
#undef XGR_Flush
#undef XGR_Flush625
#undef XGR_Fill
#undef XGR_Erase
#undef XGR_Rectangle
#undef XGR_SetPixel
#undef XGR_GetPixel
#undef XGR_Line
#undef XGR_LineTo
#undef XGR_SetPal
#undef XGR_GetPal
#undef XGR_PutSpr
#undef XGR_GetSpr
#undef XGR_WriteScreen
#undef XGR_PutSpr16
#undef XGR_GetSpr16
#undef XGR_PutSpr16a
#undef XGR_PutSpr16ap
#undef XGR_Erase16
#undef XGR_Fill16
#undef XGR_SetPixel16
#undef XGR_Line16
#undef XGR_LineTo16
#undef XGR_Rectangle16
// reset function

extern int XGR_Init(int x,int y,int fl);
extern void (*XGR_ReInit)(int x,int y,int fl);
extern void (*XGR_Finit)();
extern void (*XGR_SetClip)(int left,int top,int right,int bottom);
extern void (*XGR_GetClip)(int &left,int &top,int &right,int &bottom);
extern void (*XGR_SetClipMode)(int mode);

extern void (*XGR_Flush)(int x,int y,int sx,int sy);
extern void (*XGR_Flush625)(int x,int y,int sx,int sy);
extern void (*XGR_Fill)(int col);
extern void (*XGR_Erase)(int x,int y,int sx,int sy,int col);
extern void (*XGR_Rectangle)(int x,int y,int sx,int sy,int outcol,int incol,int mode);

extern void (*XGR_SetPixel)(int x,int y,int col);
extern int  (*XGR_GetPixel)(int x,int y);

extern void (*XGR_Line)(int x1,int y1,int x2,int y2,int col);
extern void (*XGR_LineTo)(int x,int y,int len,int dir,int col);

extern void (*XGR_SetPal)(void *ptr,int st,int cnt);
extern void (*XGR_GetPal)(void *ptr);

extern void (*XGR_PutSpr)(int x,int y,int sx,int sy,void *p,int mode);
extern void (*XGR_GetSpr)(int x,int y,int sx,int sy,void *p);

extern void (*XGR_WriteScreen)(char *bmp_name,char *pal_name);

extern void (*XGR_PutSpr16)(int x,int y,int sx,int sy,void *p,int mode);
extern void (*XGR_GetSpr16)(int x,int y,int sx,int sy,void *p);
extern void (*XGR_PutSpr16a)(int x,int y,int sx,int sy,void *p,int mode,int alpha);
extern void (*XGR_PutSpr16ap)(int x,int y,int sx,int sy,void *p,int mode,void *alpha);
extern void (*XGR_Erase16)(int x,int y,int sx,int sy,int col);
extern void (*XGR_Fill16)(int col);
extern void (*XGR_SetPixel16)(int x,int y,int col);
extern void (*XGR_Line16)(int x1,int y1,int x2,int y2,int col);
extern void (*XGR_LineTo16)(int x,int y,int len,int dir,int col);
extern void (*XGR_Rectangle16)(int x,int y,int sx,int sy,int outcol,int incol,int mode);

#endif

extern int RenderMode;

#endif //__XGR_H__

