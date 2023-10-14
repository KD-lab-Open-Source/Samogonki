/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "Md3d.h"
#include "HFONT.H"
#include "hbm.h"

#include "Base.h"

#include "mch_rto.H"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "Mesh3ds.h"
#include "IGraph3d.h"
#include "CameraDispatcher.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern HFont** acsFntTable;

extern mchArcaneBMP* mchA_MouseCrossBMP;
extern mchArcaneBMP* mchA_MouseHandBMP;

extern mchArcaneBMP* mchA_ZzzBMP;

extern mchArcaneBMP* mchA_PointerBMP;
extern mchArcaneBMP** mchA_ShapeBMP;
extern mchArcaneBMP** mchA_iShapeBMP;
extern mchArcaneBMP** mchA_MapPointer;

extern const char* mchIscreenINI;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

MD3DERROR d3dSetClipRect(const MD3DRECT &lprcClipRect);

void mchA_d3dLoadBmp(int x,int y,int idx,int ch_idx,mchArcaneBMP* p);

void mchA_d3dInitPal(void);
void mchA_d3dMemPutSpr(int x,int y,int sx,int sy,char* p,int bsx,char* buf);
void mchA_d3dSprPutpixel(int x,int y,int idx,unsigned r,unsigned g,unsigned b);
void mchA_d3dSaveSprite(int idx);

char* getIniKey(const char* fname,const char* section,const char* key);
int acsOpenResource(const char* fname,XStream& fh,int res_flag = -1);

int mchA_GetEssenceID(void);

MD3DERROR mchA_d3dCreateChildSprite(int parent_id,int x,int y,int sx,int sy,int child_id);

/* --------------------------- DEFINITION SECTION --------------------------- */

#ifndef _FINAL_VERSION_
//#define AE_D3D_DEBUG
#endif

int mchA_d3dMaxSpr = 0;
uint32_t mchA_d3dSprParent[4];
uint32_t mchA_d3dSpr[AE_D3DSPR_MAX];

#ifdef AE_D3D_DEBUG
int mchA_d3dSprX[AE_D3DSPR_MAX];
int mchA_d3dSprY[AE_D3DSPR_MAX];
int mchA_d3dSprSX[AE_D3DSPR_MAX];
int mchA_d3dSprSY[AE_D3DSPR_MAX];
#endif

unsigned mchA_d3dPalStart = 0;
unsigned mchA_d3dPal[32];

uint32_t mchA_d3dTexMode = D3DTEXFMT_ARGB4444;
M3DTEXTUREFORMAT mchA_d3dTexFmt;

int mchA_d3dAlphaRef = 0;
int mchA_d3dBackBufferFlag = 0;

int mchA_d3dSpriteParent = 0;

float mchA_d3dResX = 1.0f;
float mchA_d3dResY = 1.0f;

int mchA_d3dEssenceID = -1;

unsigned mchA_d3dColorKey = 0x00;

int mchA_d3dNumModes = 0;
MD3DMODE* mchA_d3dModes = NULL;

void mchA_d3dInit(void)
{
	int i,idx = 0,x,y,xx,yy;
	HFont* p = acsFntTable[AE_D3D_FONT];
	XStream fh;

	uint32_t pitch;
	void* spr_buf;
	char* buf;

	d3dEnumVideoMode(&mchA_d3dNumModes,&mchA_d3dModes);

#ifdef AE_D3D_DEBUG
	for(i = 0; i < 256; i ++){
		mchA_d3dSprX[i] = mchA_d3dSprY[i] = 0;
		mchA_d3dSprSX[i] = mchA_d3dSprSY[i] = 0;
	}
#endif

	mchA_d3dResX = (float)XGR_MAXX / 640.0f;
	mchA_d3dResY = (float)XGR_MAXY / 480.0f;

	d3dGetTextureFormatData(mchA_d3dTexMode,&mchA_d3dTexFmt);
	mchA_d3dInitPal();

	x = y = 1;
	buf = new char[256 * 256 * 2];

	for(i = 0; i < 256 * 256; i ++){
		((unsigned short*)buf)[i] = mchA_d3dPal[0];
	}

	for(i = 0; i < 256; i ++){
		if(!(p -> data[i] -> Flags & NULL_HCHAR)){
			mchA_d3dMemPutSpr(x,y,p -> data[i] -> SizeX,p -> data[i] -> SizeY,(char*)p -> data[i] -> HeightMap,256*2,buf);
			x += p -> data[i] -> SizeX + 1;
			if(x >= 256 - p -> SizeX - 1){
				x = 1;
				y += p -> SizeY;
				if(y >= 256 - p -> SizeY){
					y = 1;
					d3dCreateSprite(256,256,mchA_d3dTexMode,MD3DSP_USEALPHABLEND | MD3DSP_USEALPHATEST,&mchA_d3dSprParent[idx]);
					d3dLockSprite(mchA_d3dSprParent[idx],&spr_buf,&pitch);
					for(yy = 0; yy < 256; yy ++){
						memcpy((char*)spr_buf + pitch * yy,buf + yy * 512,512);
					}
					d3dUnlockSprite(mchA_d3dSprParent[idx]);
					memset(buf,0,256 * 256 * 2);
					idx ++;
				}
			}
		}
	}

	d3dCreateSprite(256,256,mchA_d3dTexMode,MD3DSP_USEALPHABLEND | MD3DSP_USEALPHATEST,&mchA_d3dSprParent[idx]);
	d3dLockSprite(mchA_d3dSprParent[idx],&spr_buf,&pitch);
	for(yy = 0; yy < 256; yy ++){
		memcpy((char*)spr_buf + pitch * yy,buf + yy * 512,512);
	}
	d3dUnlockSprite(mchA_d3dSprParent[idx]);

	idx ++;

	mchA_d3dMaxSpr = idx;

	x = y = 1;
	idx = 0;
	for(i = 0; i < 256; i ++){
		if(!(p -> data[i] -> Flags & NULL_HCHAR)){
			mchA_d3dCreateChildSprite(idx,x,y,p -> data[i] -> SizeX,p -> data[i] -> SizeY,i);
			x += p -> data[i] -> SizeX + 1;
			if(x >= 256 - p -> SizeX - 1){
				x = 1;
				y += p -> SizeY;
				if(y >= 256 - p -> SizeY){
					y = 1;
					idx ++;
				}
			}
		}
	}
	for(i = 0; i < 256; i ++){
		if(p -> data[i] -> Flags & NULL_HCHAR){
			mchA_d3dSpr[i] = mchA_d3dSpr[0];
		}
	}

	idx = mchA_d3dMaxSpr - 1;
	x = 1;
	y = 128;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_CIRCLE,mchA_ShapeBMP[0]);
	x += mchA_ShapeBMP[0] -> SizeX + 1;
/*
	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_TRIANGLE,mchA_ShapeBMP[1]);
	x += mchA_ShapeBMP[1] -> SizeX + 1;
*/

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_POINTER,mchA_PointerBMP);
	x += mchA_PointerBMP -> SizeX + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_MOUSE_CROSS,mchA_MouseCrossBMP);
	x += mchA_MouseCrossBMP -> SizeX + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_MOUSE_HAND,mchA_MouseHandBMP);
	x += mchA_MouseHandBMP -> SizeX + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_ZZZ,mchA_ZzzBMP);
	x += mchA_ZzzBMP -> SizeX + 1;

	x = 1;
	y += mchA_ShapeBMP[0] -> SizeY + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_SQUARE,mchA_ShapeBMP[2]);

	const int sq = 10;
	const int sq1 = 10;
	xx = mchA_ShapeBMP[2] -> SizeX;
	yy = mchA_ShapeBMP[2] -> SizeY;

	mchA_d3dCreateChildSprite(0,x,y + sq1,sq,sq1,AE_D3DSPR_SQ_SIDE0);
	mchA_d3dCreateChildSprite(0,x + sq,y,sq1,sq,AE_D3DSPR_SQ_SIDE1);
	mchA_d3dCreateChildSprite(0,x + xx - sq,y + sq,sq,sq1,AE_D3DSPR_SQ_SIDE2);
	mchA_d3dCreateChildSprite(0,x + sq,y + yy - sq,sq1,sq,AE_D3DSPR_SQ_SIDE3);

	mchA_d3dCreateChildSprite(0,x,y,sq,sq,AE_D3DSPR_SQ_CORNER0);
	mchA_d3dCreateChildSprite(0,x + xx - sq,y,sq,sq,AE_D3DSPR_SQ_CORNER1);
	mchA_d3dCreateChildSprite(0,x + xx - sq,y + yy - sq,sq,sq,AE_D3DSPR_SQ_CORNER2);
	mchA_d3dCreateChildSprite(0,x,y + yy - sq,sq,sq,AE_D3DSPR_SQ_CORNER3);

	x += mchA_ShapeBMP[2] -> SizeX + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_TRIANGLE,mchA_ShapeBMP[1]);
	x += mchA_ShapeBMP[1] -> SizeX + 1;

	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_MAP_POINTER,mchA_MapPointer[0]);
	x += mchA_MapPointer[0] -> SizeX + 1;
	mchA_d3dLoadBmp(x,y,idx,AE_D3DSPR_MAP_BAR,mchA_MapPointer[4]);
	x += mchA_MapPointer[4] -> SizeX + 1;

	x = 1;
	y += mchA_ShapeBMP[2] -> SizeY + 1;

	for(yy = 0; yy < 16; yy ++){
		for(xx = 0; xx < 16; xx ++){
			mchA_d3dSprPutpixel(x + xx,y + yy,idx,255,255,255);
		}
	}
	mchA_d3dCreateChildSprite(idx,x,y,16,16,AE_D3DSPR_DUMMY);
	x += 17;

	mchA_d3dSaveSprite(0);

#ifdef AE_D3D_DEBUG
	ErrH.Exit();
#endif
	mchA_d3dSpriteParent = mchA_d3dSprParent[idx];

	delete[] buf;
}

void mchA_d3dOutSprite(int x,int y,float sx,float sy,int idx,int col,int alpha,float rotate,int center_flag)
{
	int mode = (center_flag) ? MD3DORG_CENTER : MD3DORG_TOPLEFT;

	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_COLORFACTOR,col);
	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_ALPHAFACTOR,alpha);
	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_ALPHAREF,mchA_d3dAlphaRef);
	d3dDrawSprite(mchA_d3dSpr[idx],(float)x * mchA_d3dResX,(float)y * mchA_d3dResY,mode,sx * mchA_d3dResX,sy * mchA_d3dResY,rotate);
}

void mchA_d3dOutSpriteZ(int x,int y,int z,float sx,float sy,int idx,int col,int alpha,float rotate,int center_flag)
{
	float zHard,wHard,z_;
	int mode = (center_flag) ? MD3DORG_CENTER : MD3DORG_TOPLEFT;

	z_ = (float)z;
	sRectangle4f Clipping;
	Vect2f Center,Focus,zPlane,zBufferScale;
	wHard=z_ ? 1/z_ : 0;
	gb_IVisGeneric->GetCameraFrustum(camera_dispatcher -> camera(),&Center,&Clipping,&Focus,&zPlane,&zBufferScale);
	zHard=zBufferScale.y-(zBufferScale.y-zBufferScale.x)*zPlane.x*wHard;

	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_COLORFACTOR,col);
	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_ALPHAFACTOR,alpha);
	d3dSetSpriteMode(mchA_d3dSpr[idx],MD3DSP_ALPHAREF,mchA_d3dAlphaRef);
	d3dDrawSpriteZ(mchA_d3dSpr[idx],(float)x * mchA_d3dResX,(float)y * mchA_d3dResY,zHard,mode,sx * mchA_d3dResX,sy * mchA_d3dResY,rotate);
}

void mchA_d3dDrawSprite(int x,int y,float sx,float sy,int handle,int col,int alpha,float rotate,int center_flag)
{
	int ret,cnt = 0,mode = (center_flag) ? MD3DORG_CENTER : MD3DORG_TOPLEFT;
	
	do {
/*
		d3dEndScene();
		ret = d3dCacheSprite(handle);
		d3dBeginScene();
*/
		ret = MD3D_OK;

		if(++cnt > 100) 
			break;

		if(ret == MD3D_OK){
			d3dSetSpriteMode(handle,MD3DSP_COLORFACTOR,col);
			d3dSetSpriteMode(handle,MD3DSP_ALPHAFACTOR,alpha);
			d3dSetSpriteMode(handle,MD3DSP_ALPHAREF,mchA_d3dAlphaRef);
			d3dDrawSprite(handle,(float)x * mchA_d3dResX,(float)y * mchA_d3dResY,mode,sx * mchA_d3dResX,sy * mchA_d3dResY,rotate);
		}
	} while(ret != MD3D_OK);
}

void mchA_d3dDrawSpriteZ(int x,int y,int z,float sx,float sy,int handle,int col,int alpha,float rotate,int center_flag)
{
	float zHard,wHard,z_;
	int ret,cnt = 0,mode = (center_flag) ? MD3DORG_CENTER : MD3DORG_TOPLEFT;
	
	do {
		z_ = (float)z;
		sRectangle4f Clipping;
		Vect2f Center,Focus,zPlane,zBufferScale;
		wHard=z_ ? 1/z_ : 0;
		gb_IVisGeneric->GetCameraFrustum(camera_dispatcher -> camera(),&Center,&Clipping,&Focus,&zPlane,&zBufferScale);
		zHard=zBufferScale.y-(zBufferScale.y-zBufferScale.x)*zPlane.x*wHard;

		ret = MD3D_OK;

		if(++cnt > 100) 
			break;

		if(ret == MD3D_OK){
			d3dSetSpriteMode(handle,MD3DSP_COLORFACTOR,col);
			d3dSetSpriteMode(handle,MD3DSP_ALPHAFACTOR,alpha);
			d3dSetSpriteMode(handle,MD3DSP_ALPHAREF,mchA_d3dAlphaRef);
			d3dDrawSpriteZ(handle,(float)x * mchA_d3dResX,(float)y * mchA_d3dResY,zHard,mode,sx * mchA_d3dResX,sy * mchA_d3dResY,rotate);
		}
	} while(ret != MD3D_OK);
}

void mchA_d3dOutString(int x,int y,float sx,float sy,const void* str,int col,int alpha,int fnt,int space,float mul,int pr_flag,int vspace)
{
	int i,sz;
	float xx,yy;

	HFont* p = acsFntTable[fnt];
	HChar* ch;
	const unsigned char* str_buf = (const unsigned char*)str;

	sz = strlen((char*)str);

	xx = (float)x;
	yy = (float)y;

	for(i = 0; i < sz; i ++){
		if(str_buf[i] != '\r' && str_buf[i] != '\n'){
			ch = p -> data[str_buf[i]];
			if(pr_flag)
				xx -= (float)ch -> LeftOffs * mul;
			mchA_d3dOutSprite(round(xx),round(yy),sx * mul,sy * mul,str_buf[i],col,alpha);

			if(pr_flag)
				xx += (float)(ch -> SizeX - ch -> RightOffs + space) * mul;
			else
				xx += (float)(p -> SizeX + space) * mul;
		}
		else {
			if(str_buf[i] == '\n'){
				xx = (float)x;
				yy += (float)(p -> SizeY + vspace) * mul;
			}
		}
	}
}

void mchA_d3dOutStringZ(int x,int y,int z,float sx,float sy,const void* str,int col,int alpha,int fnt,int space,float mul,int pr_flag)
{
	int i,sz;
	float xx,yy;

	HFont* p = acsFntTable[fnt];
	HChar* ch;
	const unsigned char* str_buf = (const unsigned char*)str;

	sz = strlen((char*)str);

	xx = (float)x;
	yy = (float)y;

	for(i = 0; i < sz; i ++){
		if(str_buf[i] != '\r' && str_buf[i] != '\n'){
			ch = p -> data[str_buf[i]];
			if(pr_flag)
				xx -= (float)ch -> LeftOffs * mul;
			mchA_d3dOutSpriteZ(round(xx),round(yy),z,sx * mul,sy * mul,str_buf[i],col,alpha);

			if(pr_flag)
				xx += (float)(ch -> SizeX - ch -> RightOffs + space) * mul;
			else
				xx += (float)(p -> SizeX + space) * mul;
		}
		else {
			if(str_buf[i] == '\n'){
				xx = (float)x;
				yy += (float)p -> SizeY * mul;
			}
		}
	}
}

void mchA_d3dPrepare(void)
{
}

void mchA_d3dLoadBmp(int x,int y,int idx,int ch_idx,mchArcaneBMP* p)
{
	uint32_t pitch;
	void* spr_buf;

	d3dLockSprite(mchA_d3dSprParent[idx],&spr_buf,&pitch);

	mchA_d3dMemPutSpr(x,y,p -> SizeX,p -> SizeY,p -> data,pitch,(char*)spr_buf);

	d3dUnlockSprite(mchA_d3dSprParent[idx]);
	mchA_d3dCreateChildSprite(idx,x,y,p -> SizeX,p -> SizeY,ch_idx);

	mchA_d3dSaveSprite(idx);
}

void mchA_d3dMemPutSpr(int x,int y,int sx,int sy,char* p,int bsx,char* buf)
{
	int xx,yy;
	unsigned cl;
	char* src_buf,*dest_buf;

	src_buf = p;
	dest_buf = buf + x*2 + bsx * y;
	for(yy = 0; yy < sy; yy ++){
		for(xx = 0; xx < sx; xx ++){
			cl = src_buf[xx];
			((unsigned short*)dest_buf)[xx] = mchA_d3dPal[cl];
		}
		dest_buf += bsx;
		src_buf += sx;
	}
}

void mchA_d3dInitPal(void)
{
	unsigned i,cl,cl0,cl1,cl2,al,fon_cl;

	cl0 = atoi(getIniKey(mchIscreenINI,"d3d_palette","cl0"));
	cl1 = atoi(getIniKey(mchIscreenINI,"d3d_palette","cl1")) >> (8 - mchA_d3dTexFmt.dwRBitCount);
	cl2 = atoi(getIniKey(mchIscreenINI,"d3d_palette","cl2")) >> (8 - mchA_d3dTexFmt.dwRBitCount);
	fon_cl = atoi(getIniKey(mchIscreenINI,"d3d_palette","fon_color")) >> (8 - mchA_d3dTexFmt.dwRBitCount);

	mchA_d3dAlphaRef = atoi(getIniKey(mchIscreenINI,"d3d_alpha","alpha_ref"));

	mchA_d3dPalStart = cl0;

	for(i = 0; i < 32; i ++){
		cl = (i <= cl0) ? fon_cl : (cl2 - cl1) * (i - cl0) / (32 - cl0) + cl1;

//		al = (i <= cl0) ? 0 : (cl >> (mchA_d3dTexFmt.dwRBitCount - mchA_d3dTexFmt.dwAlphaBitCount));
		if(mchA_d3dTexFmt.dwAlphaBitCount > 1)
			al = (i <= cl0) ? i : (i >> (5 - mchA_d3dTexFmt.dwAlphaBitCount));
		else
			al = (i <= cl0) ? 0 : 1;

		mchA_d3dPal[i] = ((cl << mchA_d3dTexFmt.dwRBitShift) & mchA_d3dTexFmt.dwRBitMask);
		mchA_d3dPal[i] |= ((cl << mchA_d3dTexFmt.dwGBitShift) & mchA_d3dTexFmt.dwGBitMask);
		mchA_d3dPal[i] |= ((cl << mchA_d3dTexFmt.dwBBitShift) & mchA_d3dTexFmt.dwBBitMask);
		mchA_d3dPal[i] |= ((al << mchA_d3dTexFmt.dwAlphaBitShift) & mchA_d3dTexFmt.dwAlphaBitMask);
	}
}

void mchA_d3dSaveSprite(int idx)
{
#ifdef AE_D3D_DEBUG
	int sz;

	DWORD pitch;
	void* spr_buf;
	XBuffer XBuf;
	XStream fh;
	int x,y,_idx = 0,idx0 = 0;
	unsigned cl;
	char buf[256 * 256];
	char str[20];

	d3dLockSprite(mchA_d3dSprParent[idx],&spr_buf,&pitch);

	for(y = 0; y < 256; y ++){
		for(x = 0; x < 256; x ++){
			cl = *((unsigned short*)((char*)spr_buf + idx0 + x * 2));
			buf[_idx + x] = ((cl & mchA_d3dTexFmt.dwAlphaBitMask) >> mchA_d3dTexFmt.dwAlphaBitShift) << 3;
		}
		_idx += 256;
		idx0 += pitch;
	}

	XBuf < "\\a\\spr00" <= idx < ".bmp";
	fh.open(XBuf,XS_OUT);
	fh < (short)256 < (short)256;
	fh.write(buf,256 * 256);
	fh.close();

	d3dUnlockSprite(mchA_d3dSprParent[idx]);

	sz = 0;

	XBuf.init();
	XBuf < "\\a\\font0" <= idx < ".cpp";
	fh.open(XBuf,XS_OUT);

	fh < "\r\nint fntSpriteIdx[256][4] = {\r\n";
	for(x = 0; x < 256; x ++){
		fh < "\t{ " <= mchA_d3dSprX[x] < ", " <= mchA_d3dSprY[x] < ", " <= mchA_d3dSprSX[x] < ", " <= mchA_d3dSprSY[x] < " }";
		if(x < 255)
			fh < ",";

		fh < "\r\n";
	}
	fh < "};\r\n\r\n";

	_idx = 0;
	fh < "unsigned char fntSpriteData[256 * 128] = {\r\n";
	for(y = 0; y < 128; y ++){
		for(x = 0; x < 256; x ++){
			sprintf(str,"0x%02X",buf[_idx]);
			fh < str;
			if(x < 255 || y < 128) fh < ", ";
			_idx ++;
		}
		fh < "\r\n";
	}
	fh < "};\r\n\r\n";

	fh.close();
#endif
}

void mchA_d3dSprPutpixel(int x,int y,int idx,unsigned r,unsigned g,unsigned b)
{
	unsigned cl = 0,alpha,br;
	void* spr_buf;
	uint32_t pitch;

	br = (r + g + b) / 3;

	cl |= (((r >> (8 - mchA_d3dTexFmt.dwRBitCount)) << mchA_d3dTexFmt.dwRBitShift) & mchA_d3dTexFmt.dwRBitMask);
	cl |= (((g >> (8 - mchA_d3dTexFmt.dwGBitCount)) << mchA_d3dTexFmt.dwGBitShift) & mchA_d3dTexFmt.dwGBitMask);
	cl |= (((b >> (8 - mchA_d3dTexFmt.dwBBitCount)) << mchA_d3dTexFmt.dwBBitShift) & mchA_d3dTexFmt.dwBBitMask);

	if(mchA_d3dTexFmt.dwAlphaBitCount > 1)
		alpha = (!br) ? 0 : (br >> (8 - mchA_d3dTexFmt.dwAlphaBitCount));
	else
		alpha = (!br) ? 0 : 1;

	cl |= (alpha << mchA_d3dTexFmt.dwAlphaBitShift);

	d3dLockSprite(mchA_d3dSprParent[idx],&spr_buf,&pitch);
	*(unsigned short*)((char*)spr_buf + x*2 + y * pitch) = cl;
	d3dUnlockSprite(mchA_d3dSprParent[idx]);
}

void mchA_d3dSetClip(int x,int y,int sx,int sy)
{
//	gb_IGraph3d->EndScene();

	MD3DRECT r = {
		int32_t(round((float)x * mchA_d3dResX)),
		int32_t(round((float)y * mchA_d3dResY)),
		int32_t(round(float(x + sx) * mchA_d3dResX)),
		int32_t(round(float(y + sy) * mchA_d3dResY))
	};
	if(!sx || !sy) return;

	d3dSetClipRect(r);

//	gb_IGraph3d->BeginScene();
}

void mchA_d3dClearClip(void)
{
	gb_IGraph3d->EndScene();

	MD3DRECT r = { 0, 0, XGR_MAXX,XGR_MAXY };

//	d3dSetClipRect(&r);

	gb_IGraph3d->BeginScene();
}

void mchA_d3dScreenShot(void* buf,int sz)
{
	d3dScreenShot(buf,sz);
}

void mchA_d3dCreateBackBuffer(void)
{
	uint32_t fmt;

	if(mchA_d3dBackBufferFlag)
		return;

	mchA_d3dBackBufferFlag = 1;

	d3dCreateBackBuffer();
	d3dGetBackBufferFormat(&fmt);

	if(fmt == MD3DBBFORMAT_RGB565)
		XGR_HighColorMode = 0;
	else
		XGR_HighColorMode = 1;

	XGR_Obj.flags |= XGR_HICOLOR;
	XGR_Obj.ScreenX = XGR_MAXX;
	XGR_Obj.ScreenY = XGR_MAXY;
}

void mchA_d3dLockBackBuffer(void)
{
	void* p;
	uint32_t pitch;

	d3dLockBackBuffer(&p,&pitch);

	if(!XGR_Obj.yOffsTable)
		XGR_Obj.yOffsTable = new int[XGR_MAXX + 1];

	XGR_Obj.ScreenBuf = (unsigned char*)p;
	XGR_Obj.set_pitch(pitch);
}

void mchA_d3dUnlockBackBuffer(void)
{
	d3dUnlockBackBuffer();
}

void mchA_d3dReleaseBackBuffer(void)
{
	if(mchA_d3dBackBufferFlag){
		d3dReleaseBackBuffer();
		mchA_d3dBackBufferFlag = 0;
	}
}

void mchA_d3dFlushBackBuffer(int x,int y,int sx,int sy)
{
	MD3DRECT r = { x,y,x + sx,y + sy };
	if(!mchA_d3dBackBufferFlag || !sx || !sy) return;

	int err=gb_IGraph3d->FlushBackBuffer(&r);

	if(err)
		ErrH.Abort("Backbuffer error",XERR_USER,err);
}

void mchA_d3dFlip(void)
{
	d3dFlip();
}

void mchA_d3dClear(unsigned cl)
{
	d3dClear(cl);
}

void mchA_d3dToggleColorKey(int value)
{
	if(value){
		d3dSetBackBufferColorKey(mchA_d3dColorKey);
		d3dEnableBackBufferColorKey(true);
	}
	else {
		d3dEnableBackBufferColorKey(false);
	}
}

void mchA_d3dDrawScrollbar(int x,int y,int sx,int sy,int val,int max_val)
{
	float fsx0,fsx,fsy;

	if(val > max_val) val = max_val;

	fsx = ((float)sx * (float)val / (float)max_val) / 16.0f;
	fsx0 = (float)sx / 16.0f;
	fsy = (float)sy / 16.0f;

	mchA_d3dOutSprite(x,y,fsx0,fsy,AE_D3DSPR_DUMMY,mchA_ColorF[0],255);
	if(fsx > 0.0f)
		mchA_d3dOutSprite(x,y,fsx,fsy,AE_D3DSPR_DUMMY,mchA_ColorF[2],255);
}

void mchA_d3dRectangleSq(int x,int y,int sx,int sy,int col,int al)
{
	const int sq = 10;
	const int sq1 = 10;
	mchA_d3dOutSprite(x,y,1.0f,1.0f,AE_D3DSPR_SQ_CORNER0,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x + sx - sq,y,1.0f,1.0f,AE_D3DSPR_SQ_CORNER1,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x + sx - sq,y + sy - sq,1.0f,1.0f,AE_D3DSPR_SQ_CORNER2,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x,y + sy - sq,1.0f,1.0f,AE_D3DSPR_SQ_CORNER3,mchA_ColorF[col],al);

	mchA_d3dOutSprite(x,y + sq,1.0f,float(sy - sq*2)/float(sq1),AE_D3DSPR_SQ_SIDE0,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x + sq,y,float(sx - sq*2)/float(sq1),1.0f,AE_D3DSPR_SQ_SIDE1,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x + sx - sq,y + sq,1.0f,float(sy - sq*2)/float(sq1),AE_D3DSPR_SQ_SIDE2,mchA_ColorF[col],al);
	mchA_d3dOutSprite(x + sq,y + sy - sq,float(sx - sq*2)/float(sq1),1.0f,AE_D3DSPR_SQ_SIDE3,mchA_ColorF[col],al);
}

void mchA_d3dRectangle(int x,int y,int sx,int sy,int col,int outline)
{
	if(outline){
		mchA_d3dOutSprite(x,y,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],255);
		mchA_d3dOutSprite(x,y,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],255);
		mchA_d3dOutSprite(x + sx,y,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],255);
		mchA_d3dOutSprite(x,y + sy,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],255);
	}
	else
		mchA_d3dOutSprite(x,y,(float)sx / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],255);
}

void mchA_d3dRectangleZ(int x,int y,int z,int sx,int sy,int col,int outline,int alpha)
{
	if(outline){
		mchA_d3dOutSpriteZ(x,y,z,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],alpha);
		mchA_d3dOutSpriteZ(x,y,z,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],alpha);
		mchA_d3dOutSpriteZ(x + sx,y,z,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],alpha);
		mchA_d3dOutSpriteZ(x,y + sy,z,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],alpha);
	}
	else
		mchA_d3dOutSpriteZ(x,y,z,(float)sx / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],alpha);
}

void mchA_d3dRectangle_cl(int x,int y,int sx,int sy,int col,int outline)
{
	if(outline){
		mchA_d3dOutSprite(x,y,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,col,255);
		mchA_d3dOutSprite(x,y,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,col,255);
		mchA_d3dOutSprite(x + sx,y,1.0f / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,col,255);
		mchA_d3dOutSprite(x,y + sy,(float)sx / 16.0f,1.0f / 16.0f,AE_D3DSPR_DUMMY,col,255);
	}
	else
		mchA_d3dOutSprite(x,y,(float)sx / 16.0f,(float)sy / 16.0f,AE_D3DSPR_DUMMY,col,255);
}

unsigned mchA_d3dMakeColor(int r,int g,int b)
{
	return RGB_MAKE(r,g,b);
}

void mchA_d3dBeginScene(void)
{
	if(RenderMode == DIRECT3D_HICOLOR)
		gb_IGraph3d->BeginScene();
}

void mchA_d3dEndScene(void)
{
	if(RenderMode == DIRECT3D_HICOLOR)
		gb_IGraph3d->EndScene();
}

void mchA_d3dSetGamma(float v)
{
	if(v < 0.2f) v = 0.2f;
	if(v > 5.0f) v = 5.0f;

	if(RenderMode == DIRECT3D_HICOLOR)
		d3dSetAdjustedGamma(v,v,v);
}

float mchA_d3dGetGamma(void)
{
	float gr,gg = 0.0f,gb;

	if(RenderMode == DIRECT3D_HICOLOR)
		d3dGetAdjustedGamma(&gr,&gg,&gb);

	return gg;
}

void mchA_d3dLockSprite(int handle,void** p,unsigned& pitch)
{
	uint32_t pt;
	d3dLockSprite(handle,p,&pt);

	pitch = pt;
}

void mchA_d3dUnlockSprite(int handle)
{
	d3dUnlockSprite(handle);
}

int mchA_d3dCreateSlot(int sx,int sy)
{
	uint32_t handle = 0;

	if(RenderMode == DIRECT3D_HICOLOR)
		d3dCreateSprite(sx,sy,mchA_d3dTexMode,MD3DSP_USEALPHABLEND | MD3DSP_USEALPHATEST,&handle);

	return handle;
}

void mchA_d3dConvertSprite(int sx,int sy,void* src,void* dest,int src_colors)
{
	int x,y,src_idx,dest_idx;
	unsigned cl,r,g,b,a;
	unsigned char* src_ptr = (unsigned char*)src;
	unsigned short* dest_ptr = (unsigned short*)dest;

	unsigned r_bits,r_shift,r_mask;
	unsigned g_bits,g_shift,g_mask;
	unsigned b_bits,b_shift,b_mask;
	unsigned a_bits,a_shift,a_mask;

	if(RenderMode == DIRECT3D_HICOLOR){
		r_bits = mchA_d3dTexFmt.dwRBitCount;
		r_shift = mchA_d3dTexFmt.dwRBitShift;
		r_mask = mchA_d3dTexFmt.dwRBitMask;

		g_bits = mchA_d3dTexFmt.dwGBitCount;
		g_shift = mchA_d3dTexFmt.dwGBitShift;
		g_mask = mchA_d3dTexFmt.dwGBitMask;

		b_bits = mchA_d3dTexFmt.dwBBitCount;
		b_shift = mchA_d3dTexFmt.dwBBitShift;
		b_mask = mchA_d3dTexFmt.dwBBitMask;

		a_bits = mchA_d3dTexFmt.dwAlphaBitCount;
		a_shift = mchA_d3dTexFmt.dwAlphaBitShift;
		a_mask = mchA_d3dTexFmt.dwAlphaBitMask;
	}
	else {
		r_bits = 5;
		r_shift = XGR_SHIFT_R;
		r_mask = XGR_MASK_R;

		g_bits = (XGR_SHIFT_R == XGR_SHIFT_R0) ? 6  : 5;
		g_shift = XGR_SHIFT_G;
		g_mask = XGR_MASK_G;
	
		b_bits = 5;
		b_shift = XGR_SHIFT_B;
		b_mask = XGR_MASK_B;

		a_bits = 0;
		a_shift = 0;
		a_mask = 0;
	}

	src_idx = dest_idx = 0;
	switch(src_colors){
		case 32:
			for(y = 0; y < sy; y ++){
				for(x = 0; x < sx; x ++){
					r = src_ptr[src_idx + 2];
					g = src_ptr[src_idx + 1];
					b = src_ptr[src_idx + 0];
					a = src_ptr[src_idx + 3];

					cl = 0;
					cl |= (((r >> (8 - r_bits)) << r_shift) & r_mask);
					cl |= (((g >> (8 - g_bits)) << g_shift) & g_mask);
					cl |= (((b >> (8 - b_bits)) << b_shift) & b_mask);
					cl |= (((a >> (8 - a_bits)) << a_shift) & a_mask);

					dest_ptr[dest_idx] = cl;

					src_idx += 4;
					dest_idx ++;
				}
			}
			break;
		case 24:
			for(y = 0; y < sy; y ++){
				for(x = 0; x < sx; x ++){
					r = src_ptr[src_idx + 2];
					g = src_ptr[src_idx + 1];
					b = src_ptr[src_idx + 0];
					a = 255;

					cl = 0;
					cl |= (((r >> (8 - r_bits)) << r_shift) & r_mask);
					cl |= (((g >> (8 - g_bits)) << g_shift) & g_mask);
					cl |= (((b >> (8 - b_bits)) << b_shift) & b_mask);
					cl |= (((a >> (8 - a_bits)) << a_shift) & a_mask);

					dest_ptr[dest_idx] = cl;

					src_idx += 3;
					dest_idx ++;
				}
			}
			break;
	}
}

void mchA_d3dClipSprite(int handle,float l,float t,float r,float b)
{
	d3dSetSpriteRect(handle,l,t,r,b);
}


struct VERT_DIFFUSE {
	float	x,y,z,rhw;
	uint32_t		rgba;
	uint32_t		srgba;
	float	u,v;
};

void mchA_DarkenRect(int dwLeft,int dwTop,int dwRight,int dwBottom,int dwDarkness)
{
	int i;

	uint32_t dwAlphaBlendEnable;
	uint32_t dwSrcFactor;
	uint32_t dwDestFactor;
	uint32_t dwZEnable;
	uint32_t dwZWriteEnable;

	VERT_DIFFUSE Quad[4];

	Quad[0].x = float( dwLeft );
	Quad[0].y = float( dwTop );

	Quad[1].x = float( dwRight );
	Quad[1].y = float( dwTop );

	Quad[2].x = float( dwRight );
	Quad[2].y = float( dwBottom );

	Quad[3].x = float( dwLeft );
	Quad[3].y = float( dwBottom );

	for(i = 0; i < 4; i++ ){
		Quad[i].z = 0.0001f;
		Quad[i].rhw = 0.9999f;
		Quad[i].rgba = RGBA_MAKE( 0, 0, 0, dwDarkness );
		Quad[i].srgba = 0;
		Quad[i].u = Quad[i].v = 0;
	}

	// Save current render states

	d3dGetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlendEnable );
	d3dGetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcFactor );
	d3dGetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestFactor );

	d3dGetRenderState( D3DRENDERSTATE_ZENABLE, &dwZEnable );
	d3dGetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable );

	// Set render states

	d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, true );
	d3dSetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	d3dSetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	d3dSetRenderState( D3DRENDERSTATE_SPECULARENABLE, false );
	d3dSetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_FALSE );
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE, false );

	d3dSetTextureBlendMode( MD3DTB_DIFFUSE, MD3DTB_DIFFUSE );

	d3dTriangleFan( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, &Quad, 4 );

	// Restore render states

	d3dSetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlendEnable );
	d3dSetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcFactor );
	d3dSetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestFactor );

	d3dSetRenderState( D3DRENDERSTATE_ZENABLE, dwZEnable );
	d3dSetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable );
}

int mchA_d3dCheckMode(int mode,int color_depth)
{
	int i,sx,sy;

	if(mchA_d3dModes){
		switch(mode){
			case MCH_320x200:
				sx = 320;
				sy = 200;
				break;
			case MCH_640x480:
				sx = 640;
				sy = 480;
				break;
			case MCH_800x600:
				sx = 800;
				sy = 600;
				break;
			case MCH_1024x768:
				sx = 1024;
				sy = 768;
				break;
			case MCH_1152x864:
				sx = 1152;
				sy = 864;
				break;
			case MCH_1280x1024:
				sx = 1280;
				sy = 1024;
				break;
			case MCH_1600x1200:
				sx = 1600;
				sy = 1200;
				break;
		}
		for(i = 0; i < mchA_d3dNumModes; i ++){
			if(mchA_d3dModes[i].dx == sx && mchA_d3dModes[i].dy == sy && mchA_d3dModes[i].bitPerPixel == color_depth)
				return 1;
		}
		return 0;
	}

	return 1;
}

MD3DERROR mchA_d3dCreateChildSprite(int parent_id,int x,int y,int sx,int sy,int child_id)
{
#ifdef AE_D3D_DEBUG
	mchA_d3dSprX[child_id] = x;
	mchA_d3dSprY[child_id] = y;
	mchA_d3dSprSX[child_id] = sx;
	mchA_d3dSprSY[child_id] = sy;
#endif
	return d3dCreateChildSprite(mchA_d3dSprParent[parent_id],x,y,sx,sy,&mchA_d3dSpr[child_id]);
}
