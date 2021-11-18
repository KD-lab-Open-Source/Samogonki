#include "xgraph.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

void xtRegisterSysMsgFnc(void (*fPtr)(void*),int id);

void XGR_MouseFnc(void* p);

XGR_Screen XGR_Obj;
XGR_Mouse XGR_MouseObj;

int xgrScreenSizeX = 0;
int xgrScreenSizeY = 0;

int XGR_InitFlag = 0;

int XGR_MouseOffsX = 0;
int XGR_MouseOffsY = 0;

int XGR_MASK_R = 0;
int XGR_MASK_G = 0;
int XGR_MASK_B = 0;

int XGR_SHIFT_R = 0;
int XGR_SHIFT_G = 0;
int XGR_SHIFT_B = 0;

int XGR_COLOR_MASK_R = 0;
int XGR_COLOR_MASK_G = 0;
int XGR_COLOR_MASK_B = 0;

int XGR_HighColorMode = 0;
int XGR_SysMsgFlag = 0;

#define XGR_MOUSE_DEFSIZE_X		12
#define XGR_MOUSE_DEFSIZE_Y		20
static unsigned char XGR_MouseDefFrame[240] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned short XGR_MouseDefFrameHC[240] =
{
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0xFF0, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0xFF0, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0xFF0, 0xFF0, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000
};

XGR_Mouse::XGR_Mouse(void)
{
	flags = XGM_AUTOCLIP;

	BackBuf = FrameBuf = NULL;
	CurFrame = 0;
	CurAlpha = 0;

	BegSeqFrame = EndSeqFrame = SeqDelta = 0;
	SeqMode = XGM_PLAY_ONCE;

	BegSeqAlpha = EndSeqAlpha = AlphaSeqDelta = 0;
	AlphaSeqMode = XGM_NONE;

	MovementX = MovementY = 0;

	SizeX = SizeY = 0;
	PosX = PosY = 0;
	PosZ = LastPosZ = MovementZ = 0;

	PromptColor = 255;
	PromptX = PromptY = 0;
	PromptDeltaX = PromptDeltaY = 0;
	PromptSizeX = PromptSizeY = 0;
	PromptFon = NULL;
	promptData = NULL;
	AlphaData = NULL;
}

void XGR_Mouse::Init(int x, int y, int sx, int sy, int num, void* p)
{
	PosX = x;
	PosY = y;
	SizeX = sx;
	SizeY = sy;
	PosZ = LastPosZ = MovementZ = 0;

	OffsX = XGR_MouseOffsX;
	OffsY = XGR_MouseOffsY;

	if (XGR_Obj.flags & XGR_HICOLOR)
	{
		flags |= XGM_HICOLOR;
	}
	else
	{
		flags &= ~XGM_HICOLOR;
	}

	if (!p)
	{
		SizeX = sx = XGR_MOUSE_DEFSIZE_X;
		SizeY = sy = XGR_MOUSE_DEFSIZE_Y;
		num = 1;
		p = XGR_MouseDefFrame;
		if (flags & XGM_HICOLOR)
		{
			p = XGR_MouseDefFrameHC;
		}
	}

	if (flags & XGM_AUTOCLIP)
	{
		SetClipAuto();
	}

	SetPos(x, y);
	SetCursor(sx, sy, num, p);

	if (!XGR_SysMsgFlag)
	{
		// TODO xtRegisterSysMsgFnc(XGR_MouseFnc, 0);
		XGR_SysMsgFlag = 1;
	}

	flags |= (XGM_VISIBLE | XGM_INIT);
}

void XGR_Mouse::InitPos(int x, int y)
{
	LastPosX = PosX;
	LastPosY = PosY;

	LastSizeX = SizeX;
	LastSizeY = SizeY;

	PosX = x;
	PosY = y;

	AdjustPos();
}

void XGR_Mouse::Hide(void)
{
	if (flags & XGM_VISIBLE)
	{
		flags &= ~XGM_VISIBLE;
		Redraw();
	}

	if (flags & XGM_PROMPT_ACTIVE)
	{
		flags &= ~XGM_PROMPT_ACTIVE;
		XGR_Flush(PromptX, PromptY, PromptSizeX, PromptSizeY);
	}
}

void XGR_Mouse::Show(void)
{
	if (!(flags & XGM_VISIBLE))
	{
		flags |= XGM_VISIBLE;
		Redraw();
	}
}

void XGR_Mouse::Redraw(void)
{
}

void XGR_Mouse::SetCursor(int sx, int sy, int num, void* p)
{
}

void XGR_Mouse::SetPos(int x, int y)
{
	// TODO
}

void XGR_Mouse::SetClipAuto(void)
{
	ClipCoords[XGR_LEFT] = ClipCoords[XGR_TOP] = 0;
	ClipCoords[XGR_RIGHT] = XGR_MAXX - SizeX + 1;
	ClipCoords[XGR_BOTTOM] = XGR_MAXY - SizeY + 1;
}

void XGR_Mouse::AdjustPos(void)
{
	if (PosX < ClipCoords[XGR_LEFT])
	{
		PosX = ClipCoords[XGR_LEFT];
	}

	if (PosX >= ClipCoords[XGR_RIGHT])
	{
		PosX = ClipCoords[XGR_RIGHT] - 1;
	}

	if (PosY < ClipCoords[XGR_TOP])
	{
		PosY = ClipCoords[XGR_TOP];
	}

	if (PosY >= ClipCoords[XGR_BOTTOM])
	{
		PosY = ClipCoords[XGR_BOTTOM] - 1;	
	}
}

void XGR_Mouse::SetPressHandler(int bt, XGR_MOUSE_HANDLER p)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			lBt.Press = p;
			break;

		case XGM_RIGHT_BUTTON:
			rBt.Press = p;
			break;

		case XGM_MIDDLE_BUTTON:
			mBt.Press = p;
			break;
	}
}

void XGR_Mouse::SetUnPressHandler(int bt, XGR_MOUSE_HANDLER p)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			lBt.UnPress = p;
			break;

		case XGM_RIGHT_BUTTON:
			rBt.UnPress = p;
			break;

		case XGM_MIDDLE_BUTTON:
			mBt.UnPress = p;
			break;
	}
}

void XGR_Mouse::SetDblHandler(int bt, XGR_MOUSE_HANDLER p)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			lBt.DblClick = p;
			break;

		case XGM_RIGHT_BUTTON:
			rBt.DblClick = p;
			break;

		case XGM_MIDDLE_BUTTON:
			mBt.DblClick = p;
			break;
	}
}

void XGR_Mouse::Move(int fl, int x, int y)
{
	if (flags & XGM_PROMPT_ACTIVE)
	{
		flags &= ~XGM_PROMPT_ACTIVE;
		XGR_Flush(PromptX, PromptY, PromptSizeX, PromptSizeY);
	}

	if (promptData)
	{
		promptData->Timer = 0;
	}

	if (MoveH)
	{
		(*MoveH)(fl, x + SpotX, y + SpotY);
	}
}

void XGR_Mouse::Press(int bt, int fl, int x, int y)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			if (lBt.Press)
			{
				(*lBt.Press)(fl, x + SpotX, y + SpotY);
			}
			lBt.Pressed = 1;
			break;

		case XGM_RIGHT_BUTTON:
			if (rBt.Press)
			{
				(*rBt.Press)(fl, x + SpotX, y + SpotY);
			}
			rBt.Pressed = 1;
			break;

		case XGM_MIDDLE_BUTTON:
			if (mBt.Press)
			{
				(*mBt.Press)(fl, x + SpotX, y + SpotY);
			}
			mBt.Pressed = 1;
			break;
	}
}

void XGR_Mouse::UnPress(int bt, int fl, int x, int y)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			if (lBt.UnPress)
			{
				(*lBt.UnPress)(fl, x + SpotX, y + SpotY);
			}
			lBt.Pressed = 0;
			break;

		case XGM_RIGHT_BUTTON:
			if (rBt.UnPress)
			{
				(*rBt.UnPress)(fl, x + SpotX, y + SpotY);
			}
			rBt.Pressed = 0;
			break;

		case XGM_MIDDLE_BUTTON:
			if (mBt.UnPress)
			{
				(*mBt.UnPress)(fl, x + SpotX, y + SpotY);
			}
			mBt.Pressed = 0;
			break;
	}
}

void XGR_Mouse::DblClick(int bt, int fl, int x, int y)
{
	switch (bt)
	{
		case XGM_LEFT_BUTTON:
			if (lBt.DblClick)
			{
				(*lBt.DblClick)(fl, x + SpotX, y + SpotY);
			}
			break;

		case XGM_RIGHT_BUTTON:
			if (rBt.DblClick)
			{
				(*rBt.DblClick)(fl, x + SpotX, y + SpotY);
			}
			break;

		case XGM_MIDDLE_BUTTON:
			if (mBt.DblClick)
			{
				(*mBt.DblClick)(fl, x + SpotX, y + SpotY);
			}
			break;
	}
}

void XGR_Pal64K::prepare(void* p)
{
	int i,R,G,B;
	unsigned char* ptr = (unsigned char*)p;

	for(i = 0; i < 256; i ++){
		if(XGR_MASK_R == XGR_MASK_R0){
			R = ptr[i * 3] >> 1;
			G = ptr[i * 3 + 1];
			B = ptr[i * 3 + 2] >> 1;
		}
		else {
			R = ptr[i * 3] >> 1;
			G = ptr[i * 3 + 1] >> 1;
			B = ptr[i * 3 + 2] >> 1;
		}
		data[i] = XGR_RGB64K(R,G,B);
	}
}

XGR_Screen::XGR_Screen(void)
{
}

void XGR_Screen::set_pitch(int p)
{
	int i;
	for (i = 0; i <= ScreenY; i++)
	{
		yOffsTable[i] = i * p;
	}

	yStrOffs = p;
}

void XGR_Screen::set_clip(int left,int top,int right,int bottom)
{
}

void XGR_Screen::get_clip(int& left,int& top,int& right,int& bottom)
{
}

void XGR_Screen::setpixel(int x,int y,int col)
{
}

int XGR_Screen::getpixel(int x,int y)
{
}

void XGR_Screen::flush(int x,int y,int sx,int sy)
{
}

void XGR_Screen::flush625(int x,int y,int sx,int sy)
{
}

void XGR_Screen::fill(int col)
{
}

void XGR_Screen::erase(int x,int y,int sx,int sy,int col)
{
}

void XGR_Screen::rectangle(int x,int y,int sx,int sy,int outcol,int incol,int mode)
{
}

void XGR_Screen::line(int x1,int y1,int x2,int y2,int col)
{
}

void XGR_Screen::lineto(int x,int y,int len,int dir,int col)
{
}

int XGR_Screen::init(int x,int y,int flags,void *hWnd)
{
}

void XGR_Screen::close(void)
{
}

void XGR_Screen::finit(void)
{
}

void XGR_Screen::putspr(int x,int y,int sx,int sy,void* p,int mode)
{
}

void XGR_Screen::getspr(int x,int y,int sx,int sy,void* p)
{
}

void XGR_Screen::getpal(void* p)
{
}

void XGR_Screen::setpal(void* pal,int start,int count)
{
}

void XGR_Screen::set_entries(int start,int count)
{
}

void XGR_Screen::capture_screen(char* bmp_name,char* pal_name)
{
}

// HiColor functions...
void XGR_Screen::putspr16(int x,int y,int sx,int sy,void* p,int mode)
{
}

void XGR_Screen::putspr16a(int x,int y,int sx,int sy,void* p,int mode,int alpha)
{
}

void XGR_Screen::putspr16ap(int x,int y,int sx,int sy,void* p,int mode,void* alpha_ptr)
{
}

void XGR_Screen::getspr16(int x,int y,int sx,int sy,void* p)
{
}

void XGR_Screen::erase16(int x,int y,int sx,int sy,int col)
{
}

void XGR_Screen::fill16(int col)
{
	int i, j;
	unsigned short* ptr = (unsigned short*)ScreenBuf;
	for (i = 0; i < ScreenY; i++)
	{
		ptr = (unsigned short*)(ScreenBuf + yOffsTable[i]);
		for (j = 0; j < ScreenX; j++)
		{
			ptr[j] = col;
		}
	}
}

void XGR_Screen::setpixel16(int x,int y,int col)
{
}

void XGR_Screen::line16(int x1,int y1,int x2,int y2,int col)
{
}

void XGR_Screen::lineto16(int x,int y,int len,int dir,int col)
{
}

void XGR_Screen::rectangle16(int x,int y,int sx,int sy,int outcol,int incol,int mode)
{
}

void XGR_OutText(int x,int y,int col,void* text,int font,int hspace,int vspace,int pr_flag)
{
}

void XGR_MouseFnc(void* p)
{
	// TODO
}
