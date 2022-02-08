#include "xtool.h"
#include "XGR.h"
#include <cassert>

#include "Md3d.h"

int RenderMode=XGRAPH_HICOLOR;

void (*XGR_ReInit)(int x,int y,int fl);
void (*XGR_Finit)();
void (*XGR_SetClip)(int left,int top,int right,int bottom);
void (*XGR_GetClip)(int &left,int &top,int &right,int &bottom);
void (*XGR_SetClipMode)(int mode);
void (*XGR_Flush)(int x,int y,int sx,int sy);
void (*XGR_Flush625)(int x,int y,int sx,int sy);
void (*XGR_Fill)(int col);
void (*XGR_Erase)(int x,int y,int sx,int sy,int col);
void (*XGR_Rectangle)(int x,int y,int sx,int sy,int outcol,int incol,int mode);
void (*XGR_SetPixel)(int x,int y,int col);
int  (*XGR_GetPixel)(int x,int y);
void (*XGR_Line)(int x1,int y1,int x2,int y2,int col);
void (*XGR_LineTo)(int x,int y,int len,int dir,int col);
void (*XGR_SetPal)(void *ptr,int st,int cnt);
void (*XGR_GetPal)(void *ptr);
void (*XGR_PutSpr)(int x,int y,int sx,int sy,void *p,int mode);
void (*XGR_GetSpr)(int x,int y,int sx,int sy,void *p);
void (*XGR_WriteScreen)(char *bmp_name,char *pal_name);
void (*XGR_PutSpr16)(int x,int y,int sx,int sy,void *p,int mode);
void (*XGR_GetSpr16)(int x,int y,int sx,int sy,void *p);
void (*XGR_PutSpr16a)(int x,int y,int sx,int sy,void *p,int mode,int alpha);
void (*XGR_PutSpr16ap)(int x,int y,int sx,int sy,void *p,int mode,void *alpha);
void (*XGR_Erase16)(int x,int y,int sx,int sy,int col);
void (*XGR_Fill16)(int col);
void (*XGR_SetPixel16)(int x,int y,int col);
void (*XGR_Line16)(int x1,int y1,int x2,int y2,int col);
void (*XGR_LineTo16)(int x,int y,int len,int dir,int col);
void (*XGR_Rectangle16)(int x,int y,int sx,int sy,int outcol,int incol,int mode);

void Null_ReInit(int x,int y,int fl)
{
	XGR_Finit();
	XGR_Init(x,y,fl);
}
void Null_Finit()
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.close();
			break;
		case DIRECT3D_HICOLOR:
			d3dClose();
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
	RenderMode=0;
}
void Null_SetClip(int left,int top,int right,int bottom)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.set_clip(left,top,right,bottom);
			break;
		case DIRECT3D_HICOLOR:
			{
				RECT rect={left,top,right,bottom};
//				d3dSetClipRect(&rect);
			}
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_GetClip(int &left,int &top,int &right,int &bottom)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.get_clip(left,top,right,bottom);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_SetClipMode(int mode)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.set_clip_mode(mode);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Flush(int x,int y,int sx,int sy)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.flush(x,y,sx,sy);
			break;
		case DIRECT3D_HICOLOR:
			d3dFlip();
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Flush625(int x,int y,int sx,int sy)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.flush625(x,y,sx,sy);
			break;
		case DIRECT3D_HICOLOR:
			d3dFlip();
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Fill(int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.fill(col);
			break;
		case DIRECT3D_HICOLOR:
			d3dClear(col);
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Erase(int x,int y,int sx,int sy,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.erase(x,y,sx,sy,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Rectangle(int x,int y,int sx,int sy,int outcol,int incol,int mode)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.rectangle(x,y,sx,sy,outcol,incol,mode);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_SetPixel(int x,int y,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.setpixel(x,y,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
int  Null_GetPixel(int x,int y)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			return XGR_Obj.getpixel(x,y);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
	return 0;
}
void Null_Line(int x1,int y1,int x2,int y2,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.line(x1,y1,x2,y2,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_LineTo(int x,int y,int len,int dir,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.lineto(x,y,len,dir,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_SetPal(void *ptr,int st,int cnt)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.setpal(ptr,st,cnt);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_GetPal(void *ptr)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.getpal(ptr);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_PutSpr(int x,int y,int sx,int sy,void *p,int mode)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.putspr(x,y,sx,sy,p,mode);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_GetSpr(int x,int y,int sx,int sy,void *p)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.getspr(x,y,sx,sy,p);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_WriteScreen(char *bmp_name,char *pal_name)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.capture_screen(bmp_name,pal_name);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_PutSpr16(int x,int y,int sx,int sy,void *p,int mode)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.putspr16(x,y,sx,sy,p,mode);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_GetSpr16(int x,int y,int sx,int sy,void *p)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.getspr16(x,y,sx,sy,p);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_PutSpr16a(int x,int y,int sx,int sy,void *p,int mode,int alpha)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.putspr16a(x,y,sx,sy,p,mode,alpha);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_PutSpr16ap(int x,int y,int sx,int sy,void *p,int mode,void *alpha)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.putspr16ap(x,y,sx,sy,p,mode,alpha);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Erase16(int x,int y,int sx,int sy,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.erase16(x,y,sx,sy,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Fill16(int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.fill16(col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_SetPixel16(int x,int y,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.setpixel16(x,y,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Line16(int x1,int y1,int x2,int y2,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.line16(x1,y1,x2,y2,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_LineTo16(int x,int y,int len,int dir,int col)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.lineto16(x,y,len,dir,col);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
void Null_Rectangle16(int x,int y,int sx,int sy,int outcol,int incol,int mode)
{
	switch(RenderMode)
	{
		case XGRAPH_HICOLOR:
			XGR_Obj.rectangle16(x,y,sx,sy,outcol,incol,mode);
			break;
		case DIRECT3D_HICOLOR:
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
}
inline int XGR_Init(int x,int y,int fl)
{
	switch(RenderMode=GET_RENDERMODE(fl))
	{
		case XGRAPH_HICOLOR:
//			XGR_Obj.init(x,y,fl);
			XGR_ReInit=Null_ReInit;
			XGR_Finit=Null_Finit;
			XGR_SetClip=Null_SetClip;
			XGR_GetClip=Null_GetClip;
			XGR_SetClipMode=Null_SetClipMode;
			XGR_Flush=Null_Flush;
			XGR_Flush625=Null_Flush625;
			XGR_Fill=Null_Fill;
			XGR_Erase=Null_Erase;
			XGR_Rectangle=Null_Rectangle;
			XGR_SetPixel=Null_SetPixel;
			XGR_GetPixel=Null_GetPixel;
			XGR_Line=Null_Line;
			XGR_LineTo=Null_LineTo;
			XGR_SetPal=Null_SetPal;
			XGR_GetPal=Null_GetPal;
			XGR_PutSpr=Null_PutSpr;
			XGR_GetSpr=Null_GetSpr;
			XGR_WriteScreen=Null_WriteScreen;
			XGR_PutSpr16=Null_PutSpr16;
			XGR_GetSpr16=Null_GetSpr16;
			XGR_PutSpr16a=Null_PutSpr16a;
			XGR_PutSpr16ap=Null_PutSpr16ap;
			XGR_Erase16=Null_Erase16;
			XGR_Fill16=Null_Fill16;
			XGR_SetPixel16=Null_SetPixel16;
			XGR_Line16=Null_Line16;
			XGR_LineTo16=Null_LineTo16;
			XGR_Rectangle16=Null_Rectangle16;
			break;
		case DIRECT3D_HICOLOR:
			XGR_HighColorMode=0;
			XGR_MASK_R=XGR_MASK_R0;
			XGR_MASK_G=XGR_MASK_G0;
			XGR_MASK_B=XGR_MASK_B0;
			XGR_SHIFT_R=XGR_SHIFT_R0;
			XGR_SHIFT_G=XGR_SHIFT_G0;
			XGR_SHIFT_B=XGR_SHIFT_B0;
			XGR_COLOR_MASK_R=XGR_COLOR_MASK_R0;
			XGR_COLOR_MASK_G=XGR_COLOR_MASK_G0;
			XGR_COLOR_MASK_B=XGR_COLOR_MASK_G0;
			XGR_MAXX=x,XGR_MAXY=y;

//			d3dInit(XGR_MAXX=x,XGR_MAXY=y,fl);
			XGR_ReInit=Null_ReInit;
			XGR_Finit=Null_Finit;
			XGR_SetClip=Null_SetClip;
			XGR_GetClip=Null_GetClip;
			XGR_SetClipMode=Null_SetClipMode;
			XGR_Flush=Null_Flush;
			XGR_Flush625=Null_Flush625;
			XGR_Fill=Null_Fill;
			XGR_Erase=Null_Erase;
			XGR_Rectangle=Null_Rectangle;
			XGR_SetPixel=Null_SetPixel;
			XGR_GetPixel=Null_GetPixel;
			XGR_Line=Null_Line;
			XGR_LineTo=Null_LineTo;
			XGR_SetPal=Null_SetPal;
			XGR_GetPal=Null_GetPal;
			XGR_PutSpr=Null_PutSpr;
			XGR_GetSpr=Null_GetSpr;
			XGR_WriteScreen=Null_WriteScreen;
			XGR_PutSpr16=Null_PutSpr16;
			XGR_GetSpr16=Null_GetSpr16;
			XGR_PutSpr16a=Null_PutSpr16a;
			XGR_PutSpr16ap=Null_PutSpr16ap;
			XGR_Erase16=Null_Erase16;
			XGR_Fill16=Null_Fill16;
			XGR_SetPixel16=Null_SetPixel16;
			XGR_Line16=Null_Line16;
			XGR_LineTo16=Null_LineTo16;
			XGR_Rectangle16=Null_Rectangle16;
			break;
		default: { XBuffer buf; buf<"Error: unknown render mode = "<=RenderMode; ErrH.Abort(buf.address()); }
	}
	return 0;
}
