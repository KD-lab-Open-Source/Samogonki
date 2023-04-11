/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "hbm.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

#define HBM_SIGN	"HBM10"

hbm_Frame::hbm_Frame(void)
{
	flags = 0;

	X = Y = 0;
	SizeX = SizeY = 0;

	dataSize = 0;
	dataPtr = NULL;

	ownerPtr = NULL;
}

hbm_Frame::~hbm_Frame(void)
{
	if(dataPtr) 
		delete dataPtr;
}

hbm_Image::hbm_Image(void)
{
	X = Y = 0;
	SizeX = SizeY = 0;

	numFrames = 0;
	flags = 0;

	colors = 24;

	paletteSize = 0;
	palettePtr = NULL;

	frameTable = NULL;
	scr_frameTable = NULL;
}

hbm_Image::~hbm_Image(void)
{
	free();
}

void hbm_Image::load(const char* fname,int conv)
{
	XStream fh(fname,XS_IN);
	load(fh,conv);
}

void hbm_Image::load(XStream& fh,int conv)
{
	int i,sz,ver;
	char sign[20];

	sz = strlen(HBM_SIGN);

	fh.read(sign,sz);
	sign[sz] = 0;
	if(strncmp(sign,HBM_SIGN,3)) ErrH.Abort("Bad HBM file...");

	ver = atoi(sign + 3);
	switch(ver){
		case 10:
			fh > SizeX > SizeY > X > Y > numFrames > flags > colors;
			if(flags & HBM_PALETTE){
				fh > paletteSize;
				palettePtr = new char[paletteSize];
				fh.read(palettePtr,paletteSize);
			}
			frameTable = new hbm_Frame*[numFrames];
			for(i = 0; i < numFrames; i ++){
				frameTable[i] = new hbm_Frame;
				frameTable[i] -> load(fh);
			}
			break;
		default:
			ErrH.Abort("Unrecognized HBM version...");
			break;
	}
	fh.close();

	for(i = 0; i < numFrames; i ++)
		frameTable[i] -> ownerPtr = this;

	if(conv) build_frames();
}

void hbm_Image::save(const char* fname)
{
	int i,sz;

	sz = strlen(HBM_SIGN);

	XStream fh(fname,XS_OUT);

	fh.write(HBM_SIGN,sz);

	fh < SizeX < SizeY < X < Y < numFrames < flags < colors;
	if(flags & HBM_PALETTE){
		fh < paletteSize;
		fh.write(palettePtr,paletteSize);
	}

	for(i = 0; i < numFrames; i ++)
		frameTable[i] -> save(fh);

	fh.close();
}

void hbm_Image::free(void)
{
	int i;
	if(palettePtr) delete palettePtr;
	palettePtr = NULL;

	if(frameTable){
		for(i = 0; i < numFrames; i ++){
			frameTable[i] -> free();
		}
		delete[] frameTable;
		frameTable = NULL;
	}

	if(scr_frameTable){
		for(i = 0; i < numFrames; i ++){
			scr_frameTable[i] -> free();
		}
		delete scr_frameTable;
		scr_frameTable = NULL;
	}
}

void hbm_Image::free_frames(void)
{
	int i;
	if(frameTable){
		for(i = 0; i < numFrames; i ++){
			frameTable[i] -> free();
		}
		delete frameTable;
		frameTable = NULL;
	}
}

void hbm_Frame::load(XStream& fh)
{
	fh > SizeX > SizeY > X > Y > flags > dataSize;
	dataPtr = new char[dataSize];

	fh.read(dataPtr,dataSize);
}

void hbm_Frame::save(XStream& fh)
{
	fh < SizeX < SizeY < X < Y < flags < dataSize;
	fh.write(dataPtr,dataSize);
}

void hbm_Frame::free(void)
{
	if(dataPtr) delete[] dataPtr;
	dataPtr = NULL;
	dataSize = 0;
}

#define XGR_64KR0(c)		(((c) >> XGR_SHIFT_R0) & XGR_COLOR_MASK_R0)
#define XGR_64KG0(c)		(((c) >> XGR_SHIFT_G0) & XGR_COLOR_MASK_G0)
#define XGR_64KB0(c)		(((c) >> XGR_SHIFT_B0) & XGR_COLOR_MASK_B0)

#define XGR_64KR1(c)		(((c) >> XGR_SHIFT_R1) & XGR_COLOR_MASK_R1)
#define XGR_64KG1(c)		(((c) >> XGR_SHIFT_G1) & XGR_COLOR_MASK_G1)
#define XGR_64KB1(c)		(((c) >> XGR_SHIFT_B1) & XGR_COLOR_MASK_B1)

void hbm_Image::build_frames(void)
{
	int i,x,y;
	unsigned r,g,b;

	if(colors == 8 && flags & HBM_PALETTE){
		scr_frameTable = frameTable;
		frameTable = NULL;

		for(i = 0; i < 256; i ++){
			r = ((unsigned char*)palettePtr)[i * 3] >> 1;
			g = ((unsigned char*)palettePtr)[i * 3 + 1];
			b = ((unsigned char*)palettePtr)[i * 3 + 2] >> 1;
			if(XGR_HighColorMode) g >>= 1;

			palette16[i] = XGR_RGB64K(r,g,b);
		}

		return;
	}

	scr_frameTable = new hbm_Frame*[numFrames];
	for(i = 0; i < numFrames; i ++){
		scr_frameTable[i] = new hbm_Frame;
		scr_frameTable[i] -> ownerPtr = this;

		scr_frameTable[i] -> SizeX = frameTable[i] -> SizeX;
		scr_frameTable[i] -> SizeY = frameTable[i] -> SizeY;

		scr_frameTable[i] -> X = frameTable[i] -> X;
		scr_frameTable[i] -> Y = frameTable[i] -> Y;

		if(XGR_Obj.flags & XGR_HICOLOR){
			scr_frameTable[i] -> dataSize = scr_frameTable[i] -> SizeX * scr_frameTable[i] -> SizeY * 2;
			scr_frameTable[i] -> dataPtr = new char[scr_frameTable[i] -> dataSize];

			switch(colors){
				case 15:
					for(y = 0; y < frameTable[i] -> SizeY; y ++){
						for(x = 0; x < frameTable[i] -> SizeX; x ++){
							r = XGR_64KR1(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);
							g = XGR_64KG1(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);
							b = XGR_64KB1(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);

							if(!XGR_HighColorMode) g <<= 1;

							((unsigned short*)scr_frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX] = XGR_RGB64K(r,g,b);
						}
					}
					break;
				case 16:
					for(y = 0; y < frameTable[i] -> SizeY; y ++){
						for(x = 0; x < frameTable[i] -> SizeX; x ++){
							r = XGR_64KR0(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);
							g = XGR_64KG0(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);
							b = XGR_64KB0(((unsigned short*)frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX]);

							if(XGR_HighColorMode) g >>= 1;

							((unsigned short*)scr_frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX] = XGR_RGB64K(r,g,b);
						}
					}
					break;
				case 24:
					for(y = 0; y < frameTable[i] -> SizeY; y ++){
						for(x = 0; x < frameTable[i] -> SizeX; x ++){
							r = ((unsigned char*)frameTable[i] -> dataPtr)[(x + y * frameTable[i] -> SizeX) * 3];
							g = ((unsigned char*)frameTable[i] -> dataPtr)[(x + y * frameTable[i] -> SizeX) * 3 + 1];
							b = ((unsigned char*)frameTable[i] -> dataPtr)[(x + y * frameTable[i] -> SizeX) * 3 + 2];

							if(XGR_HighColorMode) g >>= 3;
							else g >>= 2;

							r >>= 3;
							b >>= 3;

							((unsigned short*)scr_frameTable[i] -> dataPtr)[x + y * frameTable[i] -> SizeX] = XGR_RGB64K(r,g,b);
						}
					}
					break;
			}
		}
	}
}

void hbm_Image::convert_frameRGBA(int x,int y,int frame,void* buf,int pitch,int shift_r,int shift_g,int shift_b,int alpha_shift)
{
	int _x,_y;
	unsigned r,g,b,alpha;
	switch(colors){
		case 24:
			for(_y = 0; _y < frameTable[frame] -> SizeY; _y ++){
				for(_x = 0; _x < frameTable[frame] -> SizeX; _x ++){
					r = ((unsigned char*)frameTable[frame] -> dataPtr)[(_x + _y * frameTable[frame] -> SizeX) * 3];
					g = ((unsigned char*)frameTable[frame] -> dataPtr)[(_x + _y * frameTable[frame] -> SizeX) * 3 + 1];
					b = ((unsigned char*)frameTable[frame] -> dataPtr)[(_x + _y * frameTable[frame] -> SizeX) * 3 + 2];

					r >>= 4;
					g >>= 4;
					b >>= 4;

					alpha = (r + g + b);
					if(alpha > 15) alpha = 15;

					*((unsigned short*)((char*)buf + (x + _x)*2 + (y + _y) * pitch)) = (r << shift_r) | (g << shift_g) | (b << shift_b) | (alpha << alpha_shift);
				}
			}
			break;
	}
}

void hbm_Frame::redraw(int mode)
{
	int x,y,_x,_y,idx,colors;
	unsigned sz,sz1,cl;
	char* buf0,*buf1;

	unsigned short* scrBuf;
	unsigned char* dataBuf;

	x = ownerPtr -> X + X;
	y = ownerPtr -> Y + Y;

	if(mode == -1) mode = XGR_HIDDEN_FON;

	colors = ownerPtr -> colors;
	if(flags & HBM_FRAME_ZIP_COMPRESSED){
		_y = y;
		idx = SizeX * colors/8;
		buf0 = (char*)dataPtr;
		buf1 = (char*)dataPtr + idx;
		for(y = 0; y < SizeY; y ++){
			sz = *((unsigned*)buf1);
			buf1 += sizeof(unsigned);
			if(sz){
				sz1 = ZIP_GetExpandedSize(buf1);
				ZIP_expand(buf0,sz1,buf1,sz);
				XGR_PutSpr16(x,y + _y,SizeX,1,buf0,XGR_HIDDEN_FON);
				buf1 += sz;
			}
		}
	}
	else {
		if(colors == 8){
			scrBuf = (unsigned short*)(XGR_Obj.ScreenBuf + XGR_Obj.yOffsTable[y]) + x;
			dataBuf = (unsigned char*)dataPtr;
			if(mode == XGR_BLACK_FON){
				for(_y = 0; _y < SizeY; _y ++){
					for(_x = 0; _x < SizeX; _x ++){
						scrBuf[_x] = ownerPtr -> palette16[dataBuf[_x]];
					}

					scrBuf += XGR_MAXX;
					dataBuf += SizeX;
				}
			}
			else {
				for(_y = 0; _y < SizeY; _y ++){
					for(_x = 0; _x < SizeX; _x ++){
						cl = dataBuf[_x];
						if(cl) scrBuf[_x] = ownerPtr -> palette16[cl];
					}

					scrBuf += XGR_MAXX;
					dataBuf += SizeX;
				}
			}
		}
		else 
			XGR_Obj.putspr16(x,y,SizeX,SizeY,dataPtr,mode);
	}
}

void hbm_Image::compress(int mode,int scr_mode)
{
	int i;
	if(scr_mode){
		for(i = 0; i < numFrames; i ++)
			scr_frameTable[i] -> compress(mode);
	}
	else {
		for(i = 0; i < numFrames; i ++)
			frameTable[i] -> compress(mode);
	}
}

void hbm_Frame::compress(int mode)
{
	int i,j,idx0 = 0,fl,colors = ownerPtr -> colors;
	unsigned char* cptr0,*cptr;
	unsigned short* sptr;
	void* tmp_ptr;

	if(flags & HBM_FRAME_ZIP_COMPRESSED) return;

	if(mode == HBM_FRAME_ZIP_COMPRESSED){
		tmp_ptr = new char[dataSize + SizeY * sizeof(int) * 2 + SizeX * colors/8];
		idx0 += SizeX * colors/8;
		cptr0 = (unsigned char*)tmp_ptr;

		switch(colors){
			case 15:
			case 16:
				sptr = (unsigned short*)dataPtr;
				for(i = 0; i < SizeY; i ++){
					fl = 0;
					for(j = 0; j < SizeX; j ++){
						if(sptr[j]){
							fl = 1;
							break;
						}
					}
					if(fl){
						j = ZIP_compress((char*)cptr0,SizeX*2,(char*)sptr,SizeX*2);
						*((unsigned*)(cptr0 + idx0)) = (unsigned)j;
						idx0 += sizeof(unsigned);
						memcpy(cptr0 + idx0,cptr0,j);
						idx0 += j;
					}
					else {
						*((unsigned*)(cptr0 + idx0)) = 0;
						idx0 += sizeof(unsigned);
					}
					sptr += SizeX;
				}
				break;
			case 24:
				cptr = (unsigned char*)dataPtr;
				for(i = 0; i < SizeY; i ++){
					fl = 0;
					for(j = 0; j < SizeX * 3; j ++){
						if(cptr[j]){
							fl = 1;
							break;
						}
					}
					if(fl){
						j = ZIP_compress((char*)cptr0,SizeX*3,(char*)cptr,SizeX*3);
						memcpy(cptr0 + idx0,cptr0,j);
						idx0 += j;
					}
					else {
						*((unsigned*)(cptr0 + idx0)) = 0;
						idx0 += sizeof(unsigned);
					}
					cptr += SizeX * 3;
				}
				break;
		}
		flags |= mode;

		delete dataPtr;
		dataPtr = new char[idx0];

		dataSize = idx0;
		memcpy(dataPtr,tmp_ptr,idx0);
		delete tmp_ptr;
	}
}

void hbm_Frame::crop(void)
{
	int x,y,x0,x1,y0,y1,sx,sy;

	unsigned char* cp,*cp0,*cp1;
	unsigned short* sp,*sp0,*sp1;

	x0 = x1 = SizeX;
	y0 = y1 = SizeY * SizeX;
	switch(ownerPtr -> colors){
		case 8:
			cp = (unsigned char*)dataPtr;
			for(y = 0; y < SizeY; y ++){
				x = 0;
				while(x < SizeX && !cp[x]) x ++;
				if(x < x0) x0 = x;

				x = 0;
				while(x < SizeX && !cp[SizeX - x - 1]) x ++;
				if(x < x1) x1 = x;

				cp += SizeX;
			}

			cp = (unsigned char*)dataPtr;
			for(x = 0; x < SizeX; x ++){
				y = 0;
				while(y < SizeY * SizeX && !cp[y]) y += SizeX;
				if(y < y0) y0 = y;

				y = 0;
				while(y < SizeY * SizeX && !cp[(SizeY - 1) * SizeX - y]) y += SizeX;
				if(y < y1) 
					y1 = y;

				cp ++;
			}

			sx = SizeX - x0 - x1;
			sy = SizeY - (y0 + y1)/SizeX;
			if(sx < 1){ sx = 1; x0 = x1 = 0; }
			if(sy < 1){ sy = 1; y0 = y1 = 0; }

			cp0 = (unsigned char*)dataPtr + x0 + y0;
			cp = new unsigned char[sx * sy];
			cp1 = cp;
			for(y = 0; y < sy; y ++){
				memcpy(cp1,cp0,sx);
				cp1 += sx;
				cp0 += SizeX;
			}
			delete dataPtr;

			dataSize = sx * sy;
			dataPtr = cp;
			X += x0;
			Y += y0 / SizeX;
			SizeX = sx;
			SizeY = sy;
			break;
		case 15:
		case 16:
			sp = (unsigned short*)dataPtr;
			for(y = 0; y < SizeY; y ++){
				x = 0;
				while(x < SizeX && !sp[x]) x ++;
				if(x < x0) x0 = x;

				x = 0;
				while(x < SizeX && !sp[SizeX - x - 1]) x ++;
				if(x < x1) x1 = x;

				sp += SizeX;
			}

			sp = (unsigned short*)dataPtr;
			for(x = 0; x < SizeX; x ++){
				y = 0;
				while(y < SizeY * SizeX && !sp[y]) y += SizeX;
				if(y < y0) y0 = y;

				y = 0;
				while(y < SizeY * SizeX && !sp[(SizeY - 1) * SizeX - y]) y += SizeX;
				if(y < y1) 
					y1 = y;

				sp ++;
			}

			sx = SizeX - x0 - x1;
			sy = SizeY - (y0 + y1)/SizeX;
			if(sx < 1) sx = 1;
			if(sy < 1) sy = 1;

			sp0 = (unsigned short*)dataPtr + x0 + y0;
			sp = new unsigned short[sx * sy * 2];
			sp1 = sp;
			for(y = 0; y < sy; y ++){
				memcpy(sp1,sp0,sx * 2);
				sp1 += sx;
				sp0 += SizeX;
			}
			delete dataPtr;

			dataSize = sx * sy * 2;
			dataPtr = sp;
			X += x0;
			Y += y0 / SizeX;
			SizeX = sx;
			SizeY = sy;
			break;
		case 24:
			cp = (unsigned char*)dataPtr;
			for(y = 0; y < SizeY; y ++){
				x = 0;
				while(x < SizeX*3 && !cp[x]) x ++;
				if(x < x0) x0 = x;

				x = 0;
				while(x < SizeX*3 && !cp[SizeX*3 - x - 1]) x ++;
				if(x < x1) x1 = x;

				cp += SizeX*3;
			}

			cp = (unsigned char*)dataPtr;
			for(x = 0; x < SizeX*3; x ++){
				y = 0;
				while(y < SizeY * SizeX*3 && !cp[y]) y += SizeX*3;
				if(y < y0) y0 = y;

				y = 0;
				while(y < SizeY * SizeX*3 && !cp[(SizeY - 1) * SizeX*3 - y]) y += SizeX*3;
				if(y < y1) 
					y1 = y;

				cp ++;
			}

			x0 -= x0 % 3;
			x1 -= x1 % 3;

			sx = SizeX - (x0 + x1)/3;
			sy = SizeY - (y0 + y1)/SizeX/3;
			if(sx < 1) sx = 1;
			if(sy < 1) sy = 1;

			cp0 = (unsigned char*)dataPtr + x0 + y0;
			cp = new unsigned char[sx * sy * 3];
			cp1 = cp;
			for(y = 0; y < sy; y ++){
				memcpy(cp1,cp0,sx*3);
				cp1 += sx*3;
				cp0 += SizeX*3;
			}
			delete dataPtr;

			dataSize = sx * sy * 3;
			dataPtr = cp;
			X += x0/3;
			Y += y0 / SizeX / 3;
			SizeX = sx;
			SizeY = sy;
			break;
	}
}