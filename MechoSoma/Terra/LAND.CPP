#include "StdAfx.h"

#include "terra.h"
#include "tgai.h"
//#include "vmap.h"
//#include "world.h"
//#include "render.h"
//#include "color.h"
#include "mesh3ds.h"
#ifdef _SURMAP_
#include "common.h"
#include "sqint.h"
#include "sqexp.h"
#include "tools.h"
#define DEBUG_SHOW
#endif

#pragma warning( disable : 4554 )  


const int _COL1 = 224 + 15;
const int _COL2 = 224 + 10;

/*-------------------------------RENDER SECTION______________________________*/
//unsigned char* lightCLR[TERRAIN_MAX];
//unsigned char palCLR[TERRAIN_MAX][2*256];
unsigned char light_G[2][512*3];
unsigned char light_GM[2][16][512*3];
unsigned int sin_GM[2][512*3];

s_terra terra;

int h_l=160;
//const  h_l=120; //интенсивность 255 // 1/2 - 180
int d_x=4;
int d_xw=8;
int delta_s;
int l_ras=32;
/* ----------------------------- EXTERN SECTION ---------------------------- */
//extern int RenderingLayer;
//extern int ExclusiveLayerMode;
#ifdef _SURMAP_
extern int MosaicToggle;
#endif
/* --------------------------- PROTOTYPE SECTION --------------------------- */
void GeoPoint(int x,int y,int level,int delta,int mode);
void _regRender(int icLowX,int icLowY,int icHiX,int icHiY,int changed);
/* --------------------------- DEFINITION SECTION -------------------------- */
int GeonetMESH		 = 200;
int TunnelProof = 16;

static int* xRad[MAX_RADIUS + 1];
static int* yRad[MAX_RADIUS + 1];
static int maxRad[MAX_RADIUS + 1];

unsigned char BEGCOLOR[TERRAIN_MAX];
unsigned char ENDCOLOR[TERRAIN_MAX];

unsigned char POWER;
unsigned char QUANT;
unsigned int WPART_POWER;
unsigned int part_map_size_y;
unsigned int part_map_size;
int MINSQUARE;

//const int MATERIAL_MAX = 1;
//const int TERRAIN_MATERIAL[TERRAIN_MAX] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
//const double TERRAIN_DXKOEF[MATERIAL_MAX] = { 1.0 };
//const double TERRAIN_SDKOEF[MATERIAL_MAX] = { 1.0 };
//const double TERRAIN_JJKOEF[MATERIAL_MAX] = { 1.0 };

//unsigned char lightCLRmaterial[MATERIAL_MAX][CLR_MAX];

unsigned char FloodLEVEL;

unsigned char* shadowParent;

void renderHiBmp(void);

//#ifndef TERRAIN16
//static unsigned char* waterBuf[3];
//#endif

int LandBounded = 1;
int VisiRegR = 0;

/*void RenderPrepare(void)
{
	register int j,ind,v;
	if(!shadowParent){
		memset(shadowParent = new unsigned char[4*H_SIZE],0,4*H_SIZE);
#ifndef TERRAIN16
		unsigned char* p = new unsigned char[3*H_SIZE];
		memset(p,0,3*H_SIZE);
		for(int i = 0;i < 3;i++,p += H_SIZE) waterBuf[i] = p;
#endif
		}

//	FloodLEVEL = FloodLvl[0];

	const double dx = 8.0;
	const double dx2 = dx*dx;
	const double sd = 256.0/(double)SHADOWDEEP;
	double jj,DX,SD;
	int colnum;
	for(ind = 0;ind < MATERIAL_MAX;ind++){
		DX = TERRAIN_DXKOEF[ind]*dx;
		SD = TERRAIN_SDKOEF[ind]*sd;
		for(j = -CLR_MAX_SIDE;j <= CLR_MAX_SIDE;j++){
			jj = TERRAIN_JJKOEF[ind]*(double)j;
			v = round(255.0*(DX*SD - jj)/sqrt((1.0 + SD*SD)*(DX*DX + jj*jj)));
			if(v < 0) v = 0;
			lightCLRmaterial[ind][CLR_MAX_SIDE + j] = v;
			}
		}
	for(ind = 0;ind < TERRAIN_MAX;ind++){
		lightCLR[ind] = lightCLRmaterial[TERRAIN_MATERIAL[ind]];
		memset(palCLR[ind],BEGCOLOR[ind],256);
		colnum = ENDCOLOR[ind] - BEGCOLOR[ind];
#ifndef TERRAIN16
		if(!ind){
			int d = (255 - FloodLEVEL) >> H_CORRECTION;
			memset(palCLR[ind] + 2*256 - d,ENDCOLOR[ind],d);
			for(j = 0;j < 256 - d;j++){
				v = round((double)j*1.25*(double)colnum/(255.0 - (double)d) - 0.25*(double)colnum);
				if(v < 0) v = 0;
				palCLR[ind][256 + j] = BEGCOLOR[ind] + v;
				}
			}
		else 
#endif
			for(j = 0;j < 256;j++)
				palCLR[ind][256 + j] = BEGCOLOR[ind] + round((double)j*(double)colnum/255.0);
		}
}*/
///////////////// For SurMAP II //////////////////////////
#ifdef _SURMAP_
void landPrepare(void)
{
	const int SIDE = 2*MAX_RADIUS + 1;

	short* rad = new short[SIDE*SIDE];
	int max = 0;

	int calc = 1;
	register int i,j,r,ind;
	short* p = rad;
	if(calc){
		for(j = -MAX_RADIUS;j <= MAX_RADIUS;j++)
			for(i = -MAX_RADIUS;i <= MAX_RADIUS;i++,p++){
				r = (int)sqrt(i*(double)i + j*(double)j);
				if(r > MAX_RADIUS) *p = -1;
				else {
					*p = r;
					maxRad[r]++;
					max++;
					}
				}
		}

	int* xheap = new int[max];
	int* yheap = new int[max];
	for(ind = 0;ind <= MAX_RADIUS;ind++){
		xRad[ind] = xheap;
		yRad[ind] = yheap;
		for(p = rad,r = 0,j = -MAX_RADIUS;j <= MAX_RADIUS;j++)
			for(i = -MAX_RADIUS;i <= MAX_RADIUS;i++,p++)
				if(*p == ind){
					xheap[r] = i;
					yheap[r] = j;
					r++;
					}
		xheap += maxRad[ind];
		yheap += maxRad[ind];
		}
	delete rad;
}

void deltaZone(int x,int y,int rad,int smth,int dh,int smode,int eql)
{
	static int locp;

	register int i,j;
	int max;
	int* xx,*yy;

	int r = rad - rad*smth/10;
	double d = 1.0/(rad - r + 1),dd,ds,s;
	int v,h,k,mean;

	if(dh){
		for(i = 0;i <= r;i++){
			max = maxRad[i];
			xx = xRad[i];
			yy = yRad[i];
			for(j = 0;j < max;j++)
				pixSet((x + xx[j]) & clip_mask_x,(y + yy[j]) & clip_mask_y,dh);
			}

		for(i = r + 1,dd = 1.0 - d;i <= rad;i++,dd -= d){
			max = maxRad[i];
			xx = xRad[i];
			yy = yRad[i];
			h = (int)(dd*dh);
			if(!h) h = dh > 0 ? 1 : -1;

			switch(smode){
				case 0:
					v = (int)(dd*max);
					ds = (double)v/(double)max;
					for(s = ds,k = 0,j = locp%max;k < max;j = (j + 1 == max) ? 0 : j + 1,k++,s += ds)
						if(s >= 1.0){
							pixSet((x + xx[j]) & clip_mask_x,(y + yy[j]) & clip_mask_y,h);
							s -= 1.0;
							}
					break;
				case 1:
					v = (int)(dd*1000000.0);
					for(j = 0;j < max;j++)
						if((int)XRnd(1000000) < v) pixSet((x + xx[j]) & clip_mask_x,(y + yy[j]) & clip_mask_y,h);
					break;
				case 2:
					v = (int)(dd*max);
					for(k = 0,j = locp%max;k < v;j = (j + 1 == max) ? 0 : j + 1,k++)
						pixSet((x + xx[j]) & clip_mask_x,(y + yy[j]) & clip_mask_y,h);
					locp += max;
					break;
				}
			}
		locp++;
		}
	else {
//		if(RenderingLayer == UP_LAYER){
			//unsigned char** lt = vMap -> lineT;
			//unsigned char* pa,*pa0,*pa_,*pa0_;
			//unsigned char *pv_,*pv0_,* pv,*pv0;
			int cx,h,cy,cx_;
			if(eql){
				mean = k = 0;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						//pv0 = pv = &(vMap -> VxBuf[0][YCYCL(y+yy[j])][0]);
						{ //if(pv)
							cx = XCYCL(x + xx[j]);//pv += ()
							mean += GetAlt(cx,YCYCL(y+yy[j]));//*pv;
							}
						}
					k += max;
					}
				mean /= k;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						//pv0 = pv = &(vMap -> VxBuf[0][cy=YCYCL(y+yy[j])][0]);
						cy=YCYCL(y+yy[j]);
						{//if(pv)
							cx = XCYCL(x + xx[j]);//pv += ()
							h = GetAlt(cx,cy);//*pv;
							if(abs(h - mean) < eql)
								if(h > mean) pixSet(cx,cy,-1);
								else if(h < mean) pixSet(cx,cy,1);
							}
						}
					}
				for(i = r + 1,dd = 1.0 - d;i <= rad;i++,dd -= d){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					h = (int)(dd*dh);
					if(!h) h = dh > 0 ? 1 : -1;

					v = (int)(dd*max);
					ds = (double)v/(double)max;
					for(s = ds,k = 0,j = locp%max;k < max;j = (j + 1 == max) ? 0 : j + 1,k++,s += ds)
						if(s >= 1.0){
							//pv0 = pv = &(vMap -> VxBuf[0][cy=YCYCL(y+yy[j])][0]);
							cy=YCYCL(y+yy[j]);
							{//if(pv)
								cx = XCYCL(x + xx[j]);//pv += ()
								h = GetAlt(cx,cy);//*pv;
								if(abs(h - mean) < eql)
									if(h > mean) pixSet(cx,cy,-1);
									else if(h < mean) pixSet(cx,cy,1);
								}
							s -= 1.0;
							}
					}
				}
			else {
				int dx,dy;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						//pv0 = pv = &(vMap -> VxBuf[0][cy=YCYCL(y+yy[j])][0]);
						cy=YCYCL(y+yy[j]);
						{ //if(pv)
							cx = XCYCL(x + xx[j]);//pv += ()
							h = GetAlt(cx,cy);//*pv;
							v = 0;
							switch(smode){
								case 0:
									for(dy = -1;dy <= 1;dy++)
										for(dx = -1;dx <= 1;dx++){
											//pv0_ = pv_ = &(vMap -> VxBuf[0][YCYCL(cy +dy)][0]);
											{//if(pv_)
												cx_ = XCYCL(cx+dx);//pv_ += ()
												v += GetAlt(cx_,YCYCL(cy +dy));//*pv_;
												}
											}
									v -= h;
									v >>= 3;
									break;
								case 1:
								case 2:
									for(dy = -1;dy <= 1;dy++)
										for(dx = -1;dx <= 1;dx++){
											//pv0_ = pv_ = &(vMap -> VxBuf[0][YCYCL(cy +dy)][0]);
											{//if(pv_)
												cx_ = XCYCL(cx+dx);//pv_ += ()
												if(abs(dx) + abs(dy) == 2)
													v += GetAlt(cx_,YCYCL(cy +dy));//*pv_;
												}
											}
									v >>= 2;
									break;
								}
							pixSet(cx,cy,v - h);
							}
						}
					}
				for(i = r + 1,dd = 1.0 - d;i <= rad;i++,dd -= d){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					h = (int)(dd*dh);
					if(!h) h = dh > 0 ? 1 : -1;

					v = (int)(dd*max);
					ds = (double)v/(double)max;
					for(s = ds,k = 0,j = locp%max;k < max;j = (j + 1 == max) ? 0 : j + 1,k++,s += ds)
						if(s >= 1.0){
							//pv0 = pv = &(vMap -> VxBuf[0][cy=YCYCL(y+yy[j])][0]);
							cy=YCYCL(y+yy[j]);
							{//if(pv)
								cx = XCYCL(x + xx[j]);//pv += ()
								h = GetAlt(cx,cy);//*pv;
								v = 0;
								switch(smode){
									case 0:
										for(dy = -1;dy <= 1;dy++)
											for(dx = -1;dx <= 1;dx++){
												//pv0_ = pv_ = &(vMap -> VxBuf[0][YCYCL(cy +dy)][0]);
												{//if(pv_)
													cx_ = XCYCL(cx+dx);//pv_ += ()
													v += GetAlt(cx_,YCYCL(cy +dy));//*pv_;
													}
												}
										v -= h;
										v >>= 3;
										break;
									case 1:
									case 2:
										for(dy = -1;dy <= 1;dy++)
											for(dx = -1;dx <= 1;dx++){
												//pv0_ = pv_ = &(vMap -> VxBuf[0][YCYCL(cy +dy)][0]);
												{//if(pv_)
													cx_ = XCYCL(cx+dx);//pv_ += ()
													if(abs(dx) + abs(dy) == 2)
														v += GetAlt(cx_,YCYCL(cy +dy));//*pv_;
													}
												}
										v >>= 2;
										break;
									}
								pixSet(cx,cy,v - h);
								}
							s -= 1.0;
							}
					}
				}
//			}
/*		else {
			unsigned char** lt = vMap -> lineT;
			unsigned char* pa,*pa0,*pa_,*pa0_;
			int cx,h,cy,cx_;
			if(eql){
				mean = k = 0;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						pa0 = pa = lt[YCYCL(y + yy[j])];
						if(pa){
							pa += (cx = XCYCL(x + xx[j]));
							mean += GET_DOWN_ALT(pa + H_SIZE,*pa,pa0,cx);
							}
						}
					k += max;
					}
				mean /= k;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						pa0 = pa = lt[cy = YCYCL(y + yy[j])];
						if(pa){
							pa += (cx = XCYCL(x + xx[j]));
							h = GET_DOWN_ALT(pa + H_SIZE,*pa,pa0,cx);
							if(abs(h - mean) < eql)
								if(h > mean) pixSet(cx,cy,-1);
								else if(h < mean) pixSet(cx,cy,1);
							}
						}
					}
				for(i = r + 1,dd = 1.0 - d;i <= rad;i++,dd -= d){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					h = (int)(dd*dh);
					if(!h) h = dh > 0 ? 1 : -1;

					v = (int)(dd*max);
					ds = (double)v/(double)max;
					for(s = ds,k = 0,j = locp%max;k < max;j = (j + 1 == max) ? 0 : j + 1,k++,s += ds)
						if(s >= 1.0){
							pa0 = pa = lt[cy = YCYCL(y + yy[j])];
							if(pa){
								pa += (cx = XCYCL(x + xx[j]));
								h = GET_DOWN_ALT(pa + H_SIZE,*pa,pa0,cx);
								if(abs(h - mean) < eql)
									if(h > mean) pixSet(cx,cy,-1);
									else if(h < mean) pixSet(cx,cy,1);
								}
							s -= 1.0;
							}
					}
				}
			else {
				int dx,dy;
				for(i = 0;i <= r;i++){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					for(j = 0;j < max;j++){
						pa0 = pa = lt[cy = YCYCL(y + yy[j])];
						if(pa){
							pa += (cx = XCYCL(x + xx[j]));
							h = GET_DOWN_ALT(pa + H_SIZE,*pa,pa0,cx);
							v = 0;
							switch(smode){
								case 0:
									for(dy = -1;dy <= 1;dy++)
										for(dx = -1;dx <= 1;dx++){
											pa0_ = pa_ = lt[YCYCL(cy + dy)];
											if(pa_){
												pa_ += (cx_ = XCYCL(cx + dx));
												v += GET_DOWN_ALT(pa_ + H_SIZE,*pa_,pa0_,cx_);
												}
											}
									v -= h;
									v >>= 3;
									break;
								case 1:
								case 2:
									for(dy = -1;dy <= 1;dy++)
										for(dx = -1;dx <= 1;dx++){
											pa0_ = pa_ = lt[YCYCL(cy + dy)];
											if(pa_){
												pa_ += (cx_ = XCYCL(cx + dx));
												if(abs(dx) + abs(dy) == 2)
													v += GET_DOWN_ALT(pa_ + H_SIZE,*pa_,pa0_,cx_);
												}
											}
									v >>= 2;
									break;
								}
							pixSet(cx,cy,v - h);
							}
						}
					}
				for(i = r + 1,dd = 1.0 - d;i <= rad;i++,dd -= d){
					max = maxRad[i];
					xx = xRad[i];
					yy = yRad[i];
					h = (int)(dd*dh);
					if(!h) h = dh > 0 ? 1 : -1;

					v = (int)(dd*max);
					ds = (double)v/(double)max;
					for(s = ds,k = 0,j = locp%max;k < max;j = (j + 1 == max) ? 0 : j + 1,k++,s += ds)
						if(s >= 1.0){
							pa0 = pa = lt[cy = YCYCL(y + yy[j])];
							if(pa){
								pa += (cx = XCYCL(x + xx[j]));
								h = GET_DOWN_ALT(pa + H_SIZE,*pa,pa0,cx);
								v = 0;
								switch(smode){
									case 0:
										for(dy = -1;dy <= 1;dy++)
											for(dx = -1;dx <= 1;dx++){
												pa0_ = pa_ = lt[YCYCL(cy + dy)];
												if(pa_){
													pa_ += (cx_ = XCYCL(cx + dx));
													v += GET_DOWN_ALT(pa_ + H_SIZE,*pa_,pa0_,cx_);
													}
												}
										v -= h;
										v >>= 3;
										break;
									case 1:
									case 2:
										for(dy = -1;dy <= 1;dy++)
											for(dx = -1;dx <= 1;dx++){
												pa0_ = pa_ = lt[YCYCL(cy + dy)];
												if(pa_){
													pa_ += (cx_ = XCYCL(cx + dx));
													if(abs(dx) + abs(dy) == 2)
														v += GET_DOWN_ALT(pa_ + H_SIZE,*pa_,pa0_,cx_);
													}
												}
										v >>= 2;
										break;
									}
								pixSet(cx,cy,v - h);
								}
							s -= 1.0;
							}
					}
				}
			}*/
		}
	regRender(x - rad,y - rad,x + rad,y + rad);
}

void GeoSetZone(int x,int y,int rad,int level,int delta)
{
	register int i,j;
	int max;
	int* xx,*yy;

	GeoPoint(0,0,0,0,1);

	for(i = 0;i <= rad;i++){
		max = maxRad[i];
		xx = xRad[i];
		yy = yRad[i];
		for(j = 0;j < max;j++)
			GeoPoint((x + xx[j]) & clip_mask_x,(y + yy[j]) & clip_mask_y,level,delta,0);
		}

	GeoPoint(0,0,level,delta,2);//здесь дельта и левел нобходимы для правильного масштабирования по измененной поверхности
}
#endif

void pixSet(int x,int y,int delta,int terrain)//В заголовочнике terrain по умолчанию -1
{
////	if(!delta) return;
	x=XCYCL(x);
	y=YCYCL(y);
	//unsigned char *pv = &(vMap -> VxBuf[0][y][x]);
	//unsigned char *pa = &(vMap -> AtBuf[0][y][x]);

	int h = GetAlt(x,y);//*(pv);

#ifdef _SURMAP_
	if(!ActiveTerrain[GetTer(x,y)]) return;
	if(h < MiniLevel || h > MaxiLevel) return;
	if(MosaicToggle && mosaicBMP.mode <= 1) delta = mosaicBMP.getDelta(x,y,delta);
#endif

#ifdef _SURMAP_
//		if(ExclusiveLayerMode) return; else
#endif
		h += delta;

#ifdef _SURMAP_
	if(LandBounded){
#endif
		if(h < 0) h = 0;
		else if(h > MAX_VX_HEIGHT) h = MAX_VX_HEIGHT;
#ifdef _SURMAP_
		}
	else {
		if(h < 0) h += 256;
		else if(h > 255) h -= 256;
		}
	if(h < MiniLevel) h = MiniLevel;
	if(h > MaxiLevel) h = MaxiLevel;
#endif

	SetAlt(x,y,h);//*pv=h;

#ifdef _SURMAP_
	if(MosaicToggle && mosaicBMP.mode == 2){
		int t = mosaicBMP.getType(x,y);
		if(t >= 0) SetTer(x,y,(t%TERRAIN_MAX));
		}
	else  if(CurrentTerrain != -1) SetTer(x,y,CurrentTerrain);
#endif
	if (terrain!=-1) SetTer(x,y,terrain%TERRAIN_MAX);

// ПРоверка на воду 
	if(h < vMap->LevelH2O ){
		if (GetW(XCYCL(x-1),y)>0 || GetW(XCYCL(x+1),y)>0 || GetW(x,y)>0 || GetW(x,YCYCL(y-1))>0 || GetW(x,YCYCL(y+1))>0 ) SetW(x,y);
	}
	else {
		SetNW(x,y);
	}

}


void WorldRender(void)
{
#ifdef _SURMAP_
	for(unsigned int i=0; i<V_SIZE; i++) RenderStr(i);
	RenderShadovM3DAll();
#else
	for(unsigned int i=0; i<V_SIZE; i++) 
		for(unsigned int j=0; j<H_SIZE; j++){
			int li=vMap->ClBuf[0][i][j];
			li= (((li&0x1f)<<1) + ((li&0x7e0)>>5) + (((li&0xf800)>>11)<<1) <<2)+64;
			if (li>255)li=255;
			vMap->ClTrBuf[0][i][j]=li;
		}
#endif
}
void RenderRegStr(int Yh,int Yd)
{
	int i=Yh;
	int j=YCYCL(Yd+1);
	while (i!=j)
	{
		RenderStr(i);
		i=YCYCL(++i); // РАЗНИЦА МЕЖДУ i++ ++i тут ОГРОМНАЯ (2048 не зацикливается)
	}
}

const int gamut=1<<11; // (1<<11)-1
const int fraction=1<<VX_FRACTION;//1<<6; //(1<<6)-1
// begin 16bit version
void RenderPrepare1(void)
{

	int j,i,h_l1;
	float d_x,d_y,dz;
	double k;

	//if (h_l>255) h_l1=511-h_l;
	//else h_l1=h_l; 
	h_l1=h_l; 
//	k=h_l1/(sqrt(257*257-h_l1*h_l1));
//	delta_s=round(k*256)*fraction;
	k=h_l1/(sqrt((double)257*257-h_l1*h_l1)); // k -это tg угла падения света
	delta_s=round(k*(1<<8)*(1<<VX_FRACTION)); 

	//Расчет суши
	for(i=0;i<TERRAIN_MAX;i++){
		h_l1=h_l; 
		d_x=terra.d_x[0][i];
		d_y=terra.d_y[0][i];
		for(j=0; j<gamut*2; j++){
			dz=(float)(j-gamut)/(1<<VX_FRACTION);
			k=( ((dz/sqrt(dz*dz+d_x*d_x)) * 
				sqrt((double)255*255-h_l1*h_l1)/255 + 
				d_x/sqrt(dz*dz+d_x*d_x) * h_l1/255) );
			if(k<0)k=0;
			terra.light_front[0][i][j]=round((double)(255-terra.ambient_light[0][i])*(sqrt(k*k)));//+l_ras
			terra.light_sideways[0][i][j]= round( (double) ((d_y)/sqrt(dz*dz+d_y*d_y)) *255 );
			terra.light_front[1][i][j]=round((double)(255-terra.ambient_light[0][i])*(sqrt(k*k)));//+l_ras
			terra.light_sideways[1][i][j]= round( (double) ((d_y)/sqrt(dz*dz+d_y*d_y)) *255 );
		}
/*		int begzl=terra.light_front[0][i][256*3];
		for(j=(256*3-1);j>0;j--){
			terra.light_front[0][i][j]=begzl;
			terra.light_sideways[0][i][j]=terra.light_sideways[0][i][512*3-j];
			if(begzl>0)begzl--;
		}*/

	}
	//расчет дна
//	h_l=200;
/*	for(i=0;i<TERRAIN_MAX;i++){
		h_l1=h_l;
		d_xw=terra.d_x[1][i];
		d_yw=terra.d_y[1][i];
		for(j=0;j<512*3;j++){
			dz=(j-255*3); 
			k=( ((dz/sqrt(dz*dz+d_xw*d_xw*9)) * sqrt(255*255-h_l1*h_l1)/255 + d_xw*3/sqrt(dz*dz+d_xw*d_xw*9) * h_l1/255) );
			if(k<0)k=0;
			terra.light_front[1][i][j]=round((double)(255-terra.ambient_light[1][i])*(sqrt(k*k)));//+l_ras
			terra.light_sideways[1][i][j]= round( (double) ((3*d_yw)/sqrt(dz*dz+3*3*d_yw*d_yw)) *256 );
		}
	}*/

}

void RenderStr(int Y)
{
#ifdef _SURMAP_
	Y=YCYCL(Y);
	unsigned char type,*pa,*pc; 
	unsigned short Vr=128,V,Vh, *pv, *pvh;
	int h_s=0;
		pv = &(vMap -> VxBuf[0][Y][0]);
		pvh = &(vMap -> VxBuf[0][YCYCL(Y-1)][0]);	
		pa = &(vMap -> AtBuf[0][Y][0]);
		pc = &(vMap -> ClTrBuf[0][Y][0]);
		int LevelH2O=vMap -> LevelH2O;

		int i=XS_Buf-1;
//		int j=XS_Buf+((delta_s>>8)*256)+1;
		int j=XS_Buf+( 1024*(1<<VX_FRACTION)*(1<<8)/delta_s )+1; //1024 -Максимальная высота для которой высчитывается запас тени
		Vr = vMap -> VxBuf[0][Y][0];

		for(j;j>=0;j--){ //,pvh--,pvd--
			i=XCYCL(j);
			if (h_s > delta_s) {h_s -=delta_s;}
			else {h_s = 0;}
			//h_s = 0;
			type=*(pa+i) & Tr_MASK;

			V=*(pv+XCYCL(i));
			Vh=*(pvh+XCYCL(i));

			*(pa+i)&=~At_SHADOW;
			int idex_light_front=gamut+(int)V-(int)Vr; if (idex_light_front >= gamut*2)idex_light_front=gamut*2-1; if(idex_light_front<0)idex_light_front=0;
			int idex_light_sideways=gamut+(int)Vh-(int)V; if(idex_light_sideways>=gamut*2)idex_light_sideways=gamut*2-1; if(idex_light_sideways<0)idex_light_sideways=0;
			if (*(pa+i)&At_WATER){
				if( h_s > ((int)LevelH2O<<8) ) {
					*(pc+i)=((terra.light_front[1][type][idex_light_front] * terra.light_sideways[1][type][idex_light_sideways] >>8) +terra.ambient_light[1][type]);
					*(pc+i)>>=1;
					if (*(pc+i) < (LevelH2O-V>>7)) *(pc+i)=0;
					else *(pc+i) -= LevelH2O-V >>7;
					*(pa+i)|=At_SHADOWV;
				}
				else {
					*(pc+i)=((terra.light_front[1][type][idex_light_front] * terra.light_sideways[1][type][idex_light_sideways] >>8) +terra.ambient_light[1][type]);
					if (*(pc+i) < (LevelH2O-V>>7)) *(pc+i)=0;
					else *(pc+i) -= LevelH2O-V >>7;
					if(h_s  <= (V<<8)) { h_s=V<<8; }
				}

			}
			else {
				if( h_s > ((int)V<<8) ) {
					*(pc+i)=((terra.light_front[0][type][idex_light_front] * terra.light_sideways[0][type][idex_light_sideways] >>8) +terra.ambient_light[0][type]);
					*(pc+i)=*(pc+i)>>1;
					*(pa+i)|=At_SHADOWV;
				}
				else {
					*(pc+i)=((terra.light_front[0][type][idex_light_front] * terra.light_sideways[0][type][idex_light_sideways] >>8) +terra.ambient_light[0][type]);
					if(h_s  <= (V<<8)) { h_s=V<<8; }
					//h_s=*pv<<16;
				}
			}
			Vr=V;
		}
#endif	
}




void RenderShadovM3D(int number, float y)
{
#ifdef _SURMAP_ 
	cShadow Sh;
	Sh.number=number;
	float f_dzx=(float)1/((float)delta_s/(256*fraction));
	DrawMeshShadeP(&Sh,10000,round(y),f_dzx);///10000*delta_s>>16);
	short *shadeBuf=Sh.shade;
		for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)  
		{
			int y=YCYCL(Sh.y+j), x=Sh.x,x_cycl=Sh.x;
			float v;
			unsigned short *pv0=&(vMap->VxBuf[0][0][y<<H_SIZE_POWER]);
			unsigned char *pc0=&(vMap->ClTrBuf[0][y][0]);
			unsigned char *pa0=&(vMap->AtBuf[0][y][0]);
			v=(*(pv0+(x_cycl)))/fraction;
			int xshade=round((Sh.x-x)-(Sh.z-v)*f_dzx);
			while(xshade<Sh.xShade)
			{
				if((xshade>0)&&(shadeBuf[xshade]>=(v=*(pv0+(x_cycl))/fraction ))&&((pa0[x_cycl]&At_SHADOW)==0))
					*(pc0+x_cycl)>>=1;
				x_cycl=XCYCL(--x);
				v=*(pv0+(x_cycl))/fraction;
				xshade=round((Sh.x-x)-(Sh.z-v)*f_dzx);
			}
		}

	editM3Dyh=YCYCL(Sh.y);
	editM3Dyd=YCYCL(Sh.y+Sh.yShade);
#endif
}



///End 16bit version




/*
void RenderPrepare1(void)
{
	int j,i,dz,h_l1;
	float d_x,d_y,d_xw,d_yw;
	double k;

	unsigned short *tp = &(vMap -> TrPal[0][0]);
	//if (h_l>255) h_l1=511-h_l;
	//else h_l1=h_l; 
	h_l1=h_l; 
	k=h_l1/(sqrt(257*257-h_l1*h_l1));
	delta_s=round(k*65536*2);
	//Расчет суши
	for(i=0;i<TERRAIN_MAX;i++){
		h_l1=h_l; 
		d_x=terra.d_x[0][i];
		d_y=terra.d_y[0][i];
		for(j=0;j<512*3;j++){
			dz=(j-255*3);
			k=( ((dz/sqrt(dz*dz+d_x*d_x*3*3)) * sqrt(255*255-h_l1*h_l1)/255 + d_x*3/sqrt(dz*dz+d_x*d_x*3*3) * h_l1/255) );
			if(k<0)k=0;
//			light_G[0][j]=round((double)(255-128)*(sqrt(k*k)))+128;
//			light_GM[0][i][j]=round((double)(255-l_ras)*(sqrt(k*k)));//+l_ras
//			sin_GM[0][j]=round( (double) ((3*d_x)/sqrt(dz*dz+3*3*d_x*d_x))  *256 );
			terra.light_front[0][i][j]=round((double)(255-terra.ambient_light[0][i])*(sqrt(k*k)));//+l_ras
			terra.light_sideways[0][i][j]= round( (double) ((3*d_y)/sqrt(dz*dz+3*3*d_y*d_y)) *256 );
		}
		int begzl=terra.light_front[0][i][256*3];
		for(j=(256*3-1);j>0;j--){
			terra.light_front[0][i][j]=begzl;
			terra.light_sideways[0][i][j]=terra.light_sideways[0][i][512*3-j];
			if(begzl>0)begzl--;
		}

	}
	//расчет дна
//	h_l=200;
	for(i=0;i<TERRAIN_MAX;i++){
		h_l1=h_l;
		d_xw=terra.d_x[1][i];
		d_yw=terra.d_y[1][i];
		for(j=0;j<512*3;j++){
			dz=(j-255*3); 
			k=( ((dz/sqrt(dz*dz+d_xw*d_xw*9)) * sqrt(255*255-h_l1*h_l1)/255 + d_xw*3/sqrt(dz*dz+d_xw*d_xw*9) * h_l1/255) );
			if(k<0)k=0;
//			light_GM[1][i][j]=round((double)(255)*(sqrt(k*k)));
			terra.light_front[1][i][j]=round((double)(255-terra.ambient_light[1][i])*(sqrt(k*k)));//+l_ras
			terra.light_sideways[1][i][j]= round( (double) ((3*d_yw)/sqrt(dz*dz+3*3*d_yw*d_yw)) *256 );
		}
	}

}

//Для мехосомы с фиксированной высотой воды
void RenderStr(int Y)
{
	Y=YCYCL(Y);
	unsigned char *pv,*pa,*pc,*pah,*pad; //*ps,*psh,*psd,
	unsigned char type,lxV=128,rxV=128,lxVh,rxVh,lxVd,rxVd,V,Vh,Vd;
	int h_s=0,h_sh=0,h_sd=0;
		pv = &(vMap -> VxBuf[0][Y][0]);
		pa = &(vMap -> AtBuf[0][Y][0]);
		pc = &(vMap -> ClTrBuf[0][Y][0]);
		unsigned char *pvh = &(vMap -> VxBuf[0][YCYCL(Y-1)][0]);	
		unsigned char *pvd = &(vMap -> VxBuf[0][YCYCL(Y+1)][0]);	
		//ps = &(vMap -> SpecBuf[0][Y][0]);
		//psh = &(vMap -> SpecBuf[0][YCYCL(Y-1)][0]);
		//psd = &(vMap -> SpecBuf[0][YCYCL(Y+1)][0]);
		pah = &(vMap -> AtBuf[0][YCYCL(Y-1)][0]);
		pad = &(vMap -> AtBuf[0][YCYCL(Y+1)][0]);
		int LevelH2O=vMap -> LevelH2O;

		int i=XS_Buf-1;
		int j=XS_Buf+((delta_s>>16)*256)+1;
		rxV = vMap -> VxBuf[0][Y][0];
		rxVh = vMap -> VxBuf[0][YCYCL(Y-1)][0];
		rxVd = vMap -> VxBuf[0][YCYCL(Y+1)][0];

		V=*(pv+i);
		Vh=*(pvh+i);
		Vd=*(pvd+i);
		for(j;j>=0;j--){ //,pvh--,pvd--
			i=XCYCL(j);
			if (h_s > delta_s) {h_s -=delta_s;}
			else {h_s = 0;}
			if (h_sh > delta_s) {h_sh -=delta_s;}
			else {h_sh = 0;}
			if (h_sd > delta_s) {h_sd -=delta_s;}
			else {h_sd = 0;}
			type=*(pa+i) & Tr_MASK;

			lxV=*(pv+XCYCL(i-1));
			lxVh=*(pvh+XCYCL(i-1));
			lxVd=*(pvd+XCYCL(i-1));

			*(pa+i)&=~At_SHADOW;
			if (*(pa+i)&At_WATER){
					if( h_s > (LevelH2O<<16) ) {
						if( h_sd > (LevelH2O<<16)) {
							if( (h_sh > (LevelH2O<<16)) && ((h_s-delta_s) > (LevelH2O+1<<16)) ){
								*(pc+i)=((terra.light_front[1][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[1][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[1][type]);
								*(pc+i)>>=1;
								if (*(pc+i) < (LevelH2O-V>>1)) *(pc+i)=0;
								else *(pc+i) -= LevelH2O-V >>1;
								*(pa+i)|=At_SHADOWV;
							}
							else {
								*(pc+i)=((terra.light_front[1][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[1][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[1][type]);
								*(pc+i)-=*(pc+i)>>2;
								if (*(pc+i) < (LevelH2O-V>>1)) *(pc+i)=0;
								else *(pc+i) -= LevelH2O-V >>1;
								
							}
						}
						else {
							*(pc+i)=((terra.light_front[1][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[1][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[1][type]);
							*(pc+i)-=*(pc+i)>>2;
							if (*(pc+i) < (LevelH2O-V>>1)) *(pc+i)=0;
							else *(pc+i) -= LevelH2O-V >>1;
						}
					}
					else {
						*(pc+i)=((terra.light_front[1][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[1][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[1][type]);
								if (*(pc+i) < (LevelH2O-V>>1)) *(pc+i)=0;
								else *(pc+i) -= LevelH2O-V >>1;
						if(h_sd  <= (Vd<<16)) { h_sd=Vd<<16; }
						if(h_sh  <= (Vh<<16)) { h_sh=Vh<<16; }
						if(h_s  <= (V<<16)) { h_s=V<<16; }
						//h_s=*pv<<16;
					}

			}
			else {
				if( h_s > (V<<16) ) {
					if( h_sd > (Vd<<16)) {
						if( (h_sh > (Vh<<16)) && ((h_s-delta_s) > (V+1<<16)) ){
							*(pc+i)=((terra.light_front[0][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[0][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[0][type]) >>1;
							*(pa+i)|=At_SHADOWV;
						}
						else {
							*(pc+i)=((terra.light_front[0][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[0][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[0][type]);
							*(pc+i)-=*(pc+i)>>2;
							
						}
					}
					else {
						*(pc+i)=((terra.light_front[0][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[0][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[0][type]);
						*(pc+i)-=*(pc+i)>>2;
					}
				}
				else {
					*(pc+i)=((terra.light_front[0][type][256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd] * terra.light_sideways[0][type][256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd] >>8) +terra.ambient_light[0][type]);
					if(h_sd  <= (Vd<<16)) { h_sd=Vd<<16; }
					if(h_sh  <= (Vh<<16)) { h_sh=Vh<<16; }
					if(h_s  <= (V<<16)) { h_s=V<<16; }
					//h_s=*pv<<16;
				}
			}
			rxV=V;
			rxVh=Vh;
			rxVd=Vd;
			V=lxV;
			Vh=lxVh;
			Vd=lxVd;

		}
	
}
///End Mechosoma version
*/
/*
void RenderShadovM3D(int number, float y)
{

	cShadow Sh;
	//Sh.dzx=(float)delta_s/65536;
	Sh.number=number;
	float f_dzx=(float)1/(delta_s/65536);
	DrawMeshShadeP(&Sh,10000,round(y),f_dzx);///10000*delta_s>>16);
	short *shadeBuf=Sh.shade;
		for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)  
		{
			int y=YCYCL(Sh.y+j), x=Sh.x,x_cycl=Sh.x,v;
			unsigned char *pv0=&(vMap->VxBuf[0][0][y<<H_SIZE_POWER]);
			unsigned char *pc0=&(vMap->ClTrBuf[0][y][0]);
			unsigned char *pa0=&(vMap->AtBuf[0][y][0]);
//			while((Sh.z<(v=*(pv0+(x_cycl))))&&((Sh.x-x)<Sh.xShade))
//				x_cycl=XCYCL(--x);
			//if(v>Sh.z) continue;
			v=*(pv0+(x_cycl));
			int xshade=round((Sh.x-x)-(Sh.z-v)*f_dzx);
			while(xshade<Sh.xShade)
			{
				if((xshade>0)&&(shadeBuf[xshade]>=(v=*(pv0+(x_cycl))))&&((pa0[x_cycl]&At_SHADOW)==0))
					*(pc0+x_cycl)>>=1;
				x_cycl=XCYCL(--x);
				v=*(pv0+(x_cycl));
				xshade=round((Sh.x-x)-(Sh.z-v)*f_dzx);
			}
		}

	editM3Dyh=YCYCL(Sh.y);
	editM3Dyd=YCYCL(Sh.y+Sh.yShade);

}
*/
//short vClBuf[YS_Buf][XS_Buf];





#if defined(_OLDSURMAP_)
//Рендер от точечных источников

inline int radiusL(int j, int i){
	return 4-XRnd(8)+(int)sqrt(j*j+i*i);
//	return (int)sqrt(j*j+i*i);
}
inline int MODL(int l)
{
	if(l>(512*3-1)) l=(512*3-1);
	if (l<0) l=0;
	return l;
}
void render025HiBmp0(int,int);
void render025HiBmp1(int xl, int yl);
void render025HiBmp2(int xl, int yl);
void render025HiBmp3(int xl, int yl);

short (*tempG)[YS_Buf][XS_Buf];
short (*tempV)[YS_Buf][XS_Buf];
unsigned short (*vidBuf)[YS_Buf][XS_Buf];
unsigned char (*vidBufR)[YS_Buf][XS_Buf];
unsigned char (*vidBufG)[YS_Buf][XS_Buf];
unsigned char (*vidBufB)[YS_Buf][XS_Buf];

	int begOffLight=10<<16;	// Начальное смещение угла яркости ~~ 10-20 <<16
	int dOffLight=2<<10;	// delta уменьшения яркости 1-я       2<<10
	int minOffLight= -20<<16;	// Минимальное смещение угла яркости 
	double begDelta_s=5;		//Начальная величена уменьшения тени 5
	double dDelta_s=0.015;		// delta уменьшения тени             0.015
	double minDelta_s=0.5;		// Минимальная величена уменьшения тени 0.5
	int begLight=0;			// Начальная освещенность источника
	int dLight=1<<12;		// delta уменьшения яркости 2-я 1<<12

	int minLightForShadow=0; //Освещенность при которой уже не отбрасывается тень 

	double tgAnglLight[XS_Buf];//На самом деле надо sqrt(XS_Buf*XS_Buf/2/2 + YS_Buf*YS_Buf/2/2)=1450

	//short CLSlight[256];
	unsigned char CLSlightR[256];
	unsigned char CLSlightG[256];
	unsigned char CLSlightB[256];
	int CLSalpha,CLSalpha1;

	struct s_tk{
		unsigned char R;
		unsigned char G;
		unsigned char B;
	};
	s_tk TerraPal[32][256];

void RenderShadowM3DPLold(int xL, int yL, int zL)
{

	cMesh *Mesh=M3D->First();
	while(Mesh)
	{
		if(IS_STATIC(Mesh->Type)) {

			cShadow Sh;
			Sh.number=Mesh->ID;
			int meshX=XCYCL(round(Mesh->x())),meshY=YCYCL(round(Mesh->y()));
			int dxL=meshX-xL;
			int dyL=meshY-yL;

			int cmpx=H_SIZE; cmpx=(cmpx>>1);
			int cmpy=V_SIZE; cmpy=(cmpy>>1);
			if(dxL<-cmpx) dxL+=H_SIZE; if(dxL>cmpx)dxL-=H_SIZE; //Расцикливание по источнику
			if(dyL<-cmpy) dyL+=V_SIZE; if(dyL>cmpy)dyL-=V_SIZE;

			double LLight=sqrt((dxL)*(dxL)+(dyL)*(dyL) );
			float f_dzx=1/tgAnglLight[round(LLight)];
			DrawMeshShade(&Sh,xL,yL,f_dzx);
			//int dxS=Sh.x-xL,dyS=Sh.y-yL;
			//if(dxS<-cmpx) dxS+=H_SIZE; if(dxS>cmpx)dxS-=H_SIZE; //Расцикливание по источнику
			//if(dyS<-cmpy) dyS+=V_SIZE; if(dyS>cmpy)dyS-=V_SIZE;
			//int LSh=sqrt((dxS)*(dxS)+(dyS)*(dyS) );
			float dshx=(dxL)/LLight;
			float dshy=(dyL)/LLight;
			short *shadeBuf=Sh.shade;
			int stl,v;
			for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)  
			{
				//int y=YCYCL(Sh.y+j), x=Sh.x,x_cycl=Sh.x,v;
				//unsigned char *pv0=&(vMap->VxBuf[0][y][0]);
				//unsigned char *pa0=&(vMap->AtBuf[0][y][0]);
				//while( (Sh.z<(v=*(pv0+x_cycl))&&((Sh.x-x)<Sh.xShade)) )
				//	x_cycl=XCYCL(--x);
				//if(v>Sh.z) continue;
				stl=0;
				v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)));
				//f_dzx=1/tgAnglLight[LSh+stl];
				int xshade=round((stl)-(Sh.z-v)*f_dzx);
				while( xshade<Sh.xShade )//или cmpy т.е. 1/2 карты
				{
					if( (xshade>0)&&(shadeBuf[xshade]>= v)) {
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWS); //*(pc0+x_cycl)>>=1;
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)+1),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWS); //*(pc0+x_cycl)>>=1;
					}
					stl++;
					v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)));
					//f_dzx=1/tgAnglLight[LSh+stl];
					xshade=round((stl)-(Sh.z-v)*f_dzx);
				}
			}

		}
		Mesh=M3D->Next();
	}


}


void RenderShadowM3DPL(int xL, int yL, int zL) //Не исправленные баги тени:
{											// 1.тень шире из-за трассировки || лучами
											// 2.Расстояние до тени считается до левого угла битмапа,
											// а надо до центра
	cMesh *Mesh=M3D->First();
	while(Mesh)
	{
		if(IS_STATIC(Mesh->Type) && (Mesh->NumberTrack&STATIC_NUMBER) ) {

			cShadow Sh;
			Sh.number=Mesh->ID;
			int meshX=XCYCL(round(Mesh->x())),meshY=YCYCL(round(Mesh->y()));
			int dxL=meshX-xL;
			int dyL=meshY-yL;

			int cmpx=H_SIZE; cmpx=(cmpx>>1);
			int cmpy=V_SIZE; cmpy=(cmpy>>1);
			if(dxL<-cmpx) dxL+=H_SIZE; if(dxL>cmpx)dxL-=H_SIZE; //Расцикливание по источнику
			if(dyL<-cmpy) dyL+=V_SIZE; if(dyL>cmpy)dyL-=V_SIZE;

			double LLight=sqrt((dxL)*(dxL)+(dyL)*(dyL) );
			//float f_dzx=1/tgAnglLight[round(LLight)];
			double hLight=tgAnglLight[round(LLight)]*LLight;
			double f_dzx=1/((hLight -YCYCL(round(Mesh->z())) )/LLight);
			DrawMeshShade(&Sh,xL,yL,f_dzx);

			int dxS=Sh.x-xL,dyS=Sh.y-yL;
			if(dxS<-cmpx) dxS+=H_SIZE; if(dxS>cmpx)dxS-=H_SIZE; //Расцикливание по источнику
			if(dyS<-cmpy) dyS+=V_SIZE; if(dyS>cmpy)dyS-=V_SIZE;
			int LSh=sqrt((dxS)*(dxS)+(dyS)*(dyS) );
			double dshx=(dxL)/LLight;
			double dshy=(dyL)/LLight;
			short *shadeBuf=Sh.shade;
			int stl,v;
			for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)  
			{
				//int y=YCYCL(Sh.y+j), x=Sh.x,x_cycl=Sh.x,v;
				//unsigned char *pv0=&(vMap->VxBuf[0][y][0]);
				//unsigned char *pa0=&(vMap->AtBuf[0][y][0]);
				//while( (Sh.z<(v=*(pv0+x_cycl))&&((Sh.x-x)<Sh.xShade)) )
				//	x_cycl=XCYCL(--x);
				//if(v>Sh.z) continue;
				stl=Sh.xShade;
				v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)));
				//f_dzx=1/tgAnglLight[LSh+stl];
				f_dzx=(double)(LSh+stl)/(hLight-v);
				int xshade=round((stl)-(Sh.z-v)*f_dzx);
				while( xshade>0 )//или cmpy т.е. 1/2 карты
				{
					if( (xshade<Sh.xShade)&&(shadeBuf[xshade]> v)&&((LSh+stl)<cmpx) ) { // 
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWS); //*(pc0+x_cycl)>>=1;
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)+1),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWS); //*(pc0+x_cycl)>>=1;
					}
					stl--;
					v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)));
					//f_dzx=1/tgAnglLight[LSh+stl];
					f_dzx=(double)(LSh+stl)/(hLight-v);
					xshade=round((stl)-(Sh.z-v)*f_dzx);
				}
			}

		}
		Mesh=M3D->Next();
	}


}


void clearShBit()
{
	for (int j=0; j<YS_Buf; j++)
		for (int i=0; i<XS_Buf; i++) 
			MovAt(j,i,(GetAt(j,i)&(~At_SHADOWS)) );
}

void renderHiBmp(int num_track)
{
	//Открытие Скрипта скрипта
	XBuffer buft;
	char tmpstr[10];
	buft < "SLight" < itoa(num_track%10,tmpstr,10) < ".sst";
	if( sur_scr.load_scr(GetTargetName(buft))==0 ) return;
	sur_scr.load_data();

	minLightForShadow =8;
	int i,j;
	//Конвер палитры в Try Color
	double dr,dg,db;
	for(i=0; i<32; i++){
		dr=(double)( (((vMap ->TrPal[i][200]>>11)&0x1f)<<3) - (((vMap ->TrPal[i][0]>>11)&0x1f)<<3) )/200;
		dg=(double)( (((vMap ->TrPal[i][200]>>5)&0x3f) <<2) - (((vMap ->TrPal[i][0]>>5)&0x3f) <<2) )/200;
		db=(double)( (((vMap ->TrPal[i][200]>>0)&0x1f) <<3) - (((vMap ->TrPal[i][0]>>0)&0x1f) <<3) )/200;
		for(j=0; j<200; j++){
			TerraPal[i][j].R=round( (((vMap ->TrPal[i][0]>>11)&0x1f) <<3)+j*dr);
			TerraPal[i][j].G=round( (((vMap ->TrPal[i][0]>>5)&0x3f) <<2) +j*dg);
			TerraPal[i][j].B=round( (((vMap ->TrPal[i][0]>>0)&0x1f) <<3) +j*db);
		}
		dr=(double)( (((vMap ->TrPal[i][250]>>11)&0x1f)<<3) - (((vMap ->TrPal[i][200]>>11)&0x1f)<<3) )/50;
		dg=(double)( (((vMap ->TrPal[i][250]>>5)&0x3f) <<2) - (((vMap ->TrPal[i][200]>>5)&0x3f) <<2) )/50;
		db=(double)( (((vMap ->TrPal[i][250]>>0)&0x1f) <<3) - (((vMap ->TrPal[i][200]>>0)&0x1f) <<3) )/50;
		for(j=0; j<50; j++){
			TerraPal[i][j+200].R=round( (((vMap ->TrPal[i][200]>>11)&0x1f) <<3)+j*dr);
			TerraPal[i][j+200].G=round( (((vMap ->TrPal[i][200]>>5)&0x3f) <<2) +j*dg);
			TerraPal[i][j+200].B=round( (((vMap ->TrPal[i][200]>>0)&0x1f) <<3) +j*db);
		}
		dr=(double)( (((vMap ->TrPal[i][255]>>11)&0x1f)<<3) - (((vMap ->TrPal[i][250]>>11)&0x1f)<<3) )/6;
		dg=(double)( (((vMap ->TrPal[i][255]>>5)&0x3f) <<2) - (((vMap ->TrPal[i][250]>>5)&0x3f) <<2) )/6;
		db=(double)( (((vMap ->TrPal[i][255]>>0)&0x1f) <<3) - (((vMap ->TrPal[i][250]>>0)&0x1f) <<3) )/6;
		for(j=0; j<6; j++){
			TerraPal[i][j+250].R=round( (((vMap ->TrPal[i][250]>>11)&0x1f) <<3)+j*dr);
			TerraPal[i][j+250].G=round( (((vMap ->TrPal[i][250]>>5)&0x3f) <<2) +j*dg);
			TerraPal[i][j+250].B=round( (((vMap ->TrPal[i][250]>>0)&0x1f) <<3) +j*db);
		}

	}
	//vidBuf=new short[1][YS_Buf][XS_Buf];
	vidBuf=vMap-> ClBuf;
	vidBufR=new unsigned char [1][YS_Buf][XS_Buf];//(unsigned char (*)[YS_Buf][XS_Buf])&(vMap->ClBuf[0][0][0]);
	vidBufG=new unsigned char [1][YS_Buf][XS_Buf];//(unsigned char (*)[YS_Buf][XS_Buf])&(vMap->ClBuf[0][YS_Buf/2][0]);
	vidBufB= new unsigned char [1][YS_Buf][XS_Buf];
	tempG=new short[1][YS_Buf][XS_Buf];
	tempV=new short[1][YS_Buf][XS_Buf];
	//Перемещение поверхности под водой на воксельную карту
	for(i=0; i<V_SIZE; i++){
		for(j=0; j<H_SIZE; j++){
			if(vMap->AtBuf[0][i][j] & At_WATER){
				int a=vMap->VxBuf[0][i][j];
				vMap->VxBuf[0][i][j]=vMap->SpecBuf[0][i][j];
				vMap->SpecBuf[0][i][j]=a;
			}
		}
	}
	//Создание нормалей горизонтальных- вертикальных
	for(i=0; i<V_SIZE; i++){ //Может быть оптимизировано
		for(j=0; j<H_SIZE; j++){
			unsigned char lxV,lxVh,lxVd,rxV,rxVh,rxVd,Vh,Vd;
			unsigned char (*tV)[YS_Buf][XS_Buf]=vMap -> VxBuf;
			lxVh=tV[0][YCYCL(i-1)][XCYCL(j-1)];
			lxV =tV[0][i][XCYCL(j-1)];
			lxVd=tV[0][YCYCL(i+1)][XCYCL(j-1)];
			rxVh=tV[0][YCYCL(i-1)][XCYCL(j+1)];
			rxV =tV[0][i][XCYCL(j+1)];
			rxVd=tV[0][YCYCL(i+1)][XCYCL(j+1)];
			Vh  =tV[0][YCYCL(i-1)][j];
			Vd  =tV[0][YCYCL(i+1)][j];
			//256*3+lxV+lxVh+lxVd-rxV-rxVh-rxVd
			//(*tempG)[i][j]=vMap -> VxBuf[0][YCYCL(i-1)][XCYCL(j-1)]+vMap -> VxBuf[0][i][XCYCL(j-1)]+vMap -> VxBuf[0][YCYCL(i+1)][XCYCL(j-1)]-vMap -> VxBuf[0][YCYCL(i-1)][XCYCL(j+1)]-vMap -> VxBuf[0][i][XCYCL(j+1)]-vMap -> VxBuf[0][YCYCL(i+1)][XCYCL(j+1)] ;
			(*tempG)[i][j]=lxV+lxVh+lxVd-rxV-rxVh-rxVd;
			//256*3+lxVh+Vh+rxVh-lxVd-Vd-rxVd
			//(*tempV)[i][j]=vMap -> VxBuf[0][YCYCL(i-1)][XCYCL(j-1)]+vMap -> VxBuf[0][YCYCL(i-1)][j]+vMap -> VxBuf[0][YCYCL(i-1)][XCYCL(j+1)]- vMap -> VxBuf[0][YCYCL(i+1)][XCYCL(j-1)]- vMap -> VxBuf[0][YCYCL(i+1)][j]- vMap -> VxBuf[0][YCYCL(i+1)][XCYCL(j+1)];
			(*tempV)[i][j]=lxVh+Vh+rxVh-lxVd-Vd-rxVd;
			//очистка ClTrBuf
			vMap -> ClTrBuf[0][i][j]=0;
			(*vidBuf)[i][j]=0;
			(*vidBufR)[i][j]=0;
			(*vidBufG)[i][j]=0;
			(*vidBufB)[i][j]=0;
			vMap -> AtBuf[0][i][j]&=~At_SHADOW;
		}
	}
	//Перемещение поверхности обратно
	for(i=0; i<V_SIZE; i++){
		for(j=0; j<H_SIZE; j++){
			if(vMap->AtBuf[0][i][j] & At_WATER){
				int a=vMap->VxBuf[0][i][j];
				vMap->VxBuf[0][i][j]=vMap->SpecBuf[0][i][j];
				vMap->SpecBuf[0][i][j]=a;
			}
		}
	}

	int t_x,t_y, t_r,t_g,t_b,t_a,t_dlinaTeni;
	double t_begOffLight, t_dOffLight, t_minOffLight, t_begDelta_s, t_dDelta_s, t_minDelta_s, t_begLight, t_radius;//t_dLight,
//Рендер источников без тени
	for(i=0; i<sur_scr.numbers_cell; i++){
		sur_scr.row[0].get_el(i,t_x);
		sur_scr.row[1].get_el(i,t_y);
		sur_scr.row[2].get_el(i,t_begOffLight);
		sur_scr.row[3].get_el(i,t_dOffLight);
		sur_scr.row[4].get_el(i,t_minOffLight);
		sur_scr.row[5].get_el(i,t_begDelta_s);
		sur_scr.row[6].get_el(i,t_dDelta_s);
		sur_scr.row[7].get_el(i,t_minDelta_s);
		sur_scr.row[8].get_el(i,t_begLight);
		sur_scr.row[9].get_el(i,t_radius);//t_dLight);
		////
		sur_scr.row[10].get_el(i,t_r);
		sur_scr.row[11].get_el(i,t_g);
		sur_scr.row[12].get_el(i,t_b);
		sur_scr.row[13].get_el(i,t_a);
		sur_scr.row[14].get_el(i,t_dlinaTeni);
//Глобальные переменные рендера
	begOffLight=round(t_begOffLight*65536);	// Начальное смещение угла яркости ~~ 
	dOffLight=	round(t_dOffLight*65536);	// delta уменьшения яркости 1-я 
	minOffLight=round(t_minOffLight*65536);	// Минимальное смещение угла яркости 
	begDelta_s=	255;		//Начальная величена уменьшения тени 
	dDelta_s=	0; //t_dDelta_s;		// delta уменьшения тени
	minDelta_s=	t_minDelta_s;		// Минимальная величена уменьшения тени
	begLight=	round(t_begLight*65536);			// Начальная освещенность источника
	dLight=		(255+t_begLight)*65536/t_radius;//round(t_dLight*65536);		// delta уменьшения яркости 2-я 1<<12

	for(j=0; j<XS_Buf; j++) tgAnglLight[j]=255;


	double dcr,dcg,dcb;
	dcr = (double) (t_r<<3)/256;
	dcg = (double) (t_g<<2)/256;
	dcb = (double) (t_b<<3)/256; // Палитра 565 !!!
	for(j=0; j<256; j++) {
		CLSlightR[j]= round(dcr*j);
		CLSlightG[j]= round(dcg*j);
		CLSlightB[j]= round(dcb*j);
	}

	CLSalpha=t_a<<(16-6);//Вычисление целочисленной альфы
	CLSalpha1=(63-t_a)<<(16-6);

	render025HiBmp0(t_x,t_y);
	render025HiBmp1(t_x,t_y);
	render025HiBmp2(t_x,t_y);
	render025HiBmp3(t_x,t_y);
	}
	/////// Сложение итоговых источников с палитройповерхности карты
	for(i=0; i<YS_Buf; i++){ 
		for(j=0; j<XS_Buf; j++){
			int r,g,b;
			int terr=(vMap -> AtBuf[0][i][j])&TrW_MASK;
			int li=vMap -> ClTrBuf[0][i][j];
			r=TerraPal[terr][li].R+(*vidBufR)[i][j];
			g=TerraPal[terr][li].G+(*vidBufG)[i][j];
			b=TerraPal[terr][li].B+(*vidBufB)[i][j];
			if(r>255)r=255; if(g>255)g=255;	if(b>255) b=255;
			(*vidBufR)[i][j]=r;
			(*vidBufG)[i][j]=g;
			(*vidBufB)[i][j]=b;
			(*vidBuf)[i][j]=((((*vidBufR)[i][j]>>3)&0x1f)<<11)+((((*vidBufG)[i][j]>>2)&0x3f)<<5)+(((*vidBufB)[i][j]>>3)&0x1f);
		}
	}

//Запись HiColor битмапа, который без теней
	TGAHEAD thead;
	thead.save3layers(GetTargetName("outputnh.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufR),(unsigned char*)(*vidBufG),(unsigned char*)(*vidBufB) );

//Очистка перед рендером
	for(i=0; i<V_SIZE; i++){ //Может быть оптимизировано
		for(j=0; j<H_SIZE; j++){
			vMap -> ClTrBuf[0][i][j]=0;
			(*vidBufR)[i][j]=0;
			(*vidBufG)[i][j]=0;
			(*vidBufB)[i][j]=0;
			vMap -> AtBuf[0][i][j]&=~At_SHADOW;
		}
	}

//Рендер источников с тенью
	for(i=0; i<sur_scr.numbers_cell; i++){
		sur_scr.row[0].get_el(i,t_x);
		sur_scr.row[1].get_el(i,t_y);
		sur_scr.row[2].get_el(i,t_begOffLight);
		sur_scr.row[3].get_el(i,t_dOffLight);
		sur_scr.row[4].get_el(i,t_minOffLight);
		sur_scr.row[5].get_el(i,t_begDelta_s);
		sur_scr.row[6].get_el(i,t_dDelta_s);
		sur_scr.row[7].get_el(i,t_minDelta_s);
		sur_scr.row[8].get_el(i,t_begLight);
		sur_scr.row[9].get_el(i,t_radius);//t_dLight);
		////
		sur_scr.row[10].get_el(i,t_r);
		sur_scr.row[11].get_el(i,t_g);
		sur_scr.row[12].get_el(i,t_b);
		sur_scr.row[13].get_el(i,t_a);
		sur_scr.row[14].get_el(i,t_dlinaTeni);
//Глобальные переменные рендера
	begOffLight=round(t_begOffLight*65536);	// Начальное смещение угла яркости ~~ 
	dOffLight=	round(t_dOffLight*65536);	// delta уменьшения яркости 1-я 
	minOffLight=round(t_minOffLight*65536);	// Минимальное смещение угла яркости 
	begDelta_s=	t_begDelta_s;		//Начальная величена уменьшения тени 
	dDelta_s=	(double)t_dlinaTeni/1000; //t_dDelta_s;		// delta уменьшения тени
	minDelta_s=	t_minDelta_s;		// Минимальная величена уменьшения тени
	begLight=	round(t_begLight*65536);			// Начальная освещенность источника
	dLight=		(255+t_begLight)*65536/t_radius;//round(t_dLight*65536);		// delta уменьшения яркости 2-я 1<<12

	if(t_dlinaTeni==0)minDelta_s=255;
	for(j=0; j<XS_Buf; j++) { //На самом деле j< 1450
		double ttg=(double)t_dlinaTeni/(1+j);
		if(ttg<minDelta_s)ttg=minDelta_s;
		tgAnglLight[j]=ttg;//round(ttg*65536);
		//tgAnglLight[j]=t_begDelta_s-j*dDelta_s;
		//if(tgAnglLight[j]<minDelta_s)tgAnglLight[j]=minDelta_s;
	}
	clearShBit();
	if(minDelta_s<255 && t_dlinaTeni!=0 ) RenderShadowM3DPL(t_x,t_y,t_dlinaTeni);

//	double dcr,dcg,dcb;
//	dcr = (double) t_r/256;
//	dcg = (double) t_g/256;
//	dcb = (double) t_b/256; // Палитра 565 !!!
//	for(j=0; j<256; j++) CLSlight[j]= ((round(dcr*j)&0x1f)<<11) + ((round(dcg*j)&0x3f)<<5) + (round(dcb*j)&0x1f);
	double dcr,dcg,dcb;
	dcr = (double) (t_r<<3)/256;
	dcg = (double) (t_g<<2)/256;
	dcb = (double) (t_b<<3)/256; // Палитра 565 !!!
	for(j=0; j<256; j++) {
		CLSlightR[j]= round(dcr*j);
		CLSlightG[j]= round(dcg*j);
		CLSlightB[j]= round(dcb*j);
	}

	CLSalpha=t_a<<(16-6);//Вычисление целочисленной альфы
	CLSalpha1=(63-t_a)<<(16-6);

	render025HiBmp0(t_x,t_y);
	render025HiBmp1(t_x,t_y);
	render025HiBmp2(t_x,t_y);
	render025HiBmp3(t_x,t_y);

	}
	thead.save3layers(GetTargetName("colormap.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufR),(unsigned char*)(*vidBufG),(unsigned char*)(*vidBufB) );
	//thead.save3layers(GetTargetName("lightmap.tga"),H_SIZE,V_SIZE,(unsigned char*)(*(vMap -> ClTrBuf)),(unsigned char*)(*(vMap -> ClTrBuf)),(unsigned char*)(*(vMap -> ClTrBuf)) );
	/////// Сложение итоговых источников с палитройповерхности карты
	for(i=0; i<YS_Buf; i++){ 
		for(j=0; j<XS_Buf; j++){
			int r,g,b;
			int terr=(vMap -> AtBuf[0][i][j])&TrW_MASK;
			int li=vMap -> ClTrBuf[0][i][j];
			r=TerraPal[terr][li].R+(*vidBufR)[i][j];
			g=TerraPal[terr][li].G+(*vidBufG)[i][j];
			b=TerraPal[terr][li].B+(*vidBufB)[i][j];
			if(r>255)r=255; if(g>255)g=255;	if(b>255) b=255;
			(*vidBufR)[i][j]=r;
			(*vidBufG)[i][j]=g;
			(*vidBufB)[i][j]=b;
			(*vidBuf)[i][j]=((((*vidBufR)[i][j]>>3)&0x1f)<<11)+((((*vidBufG)[i][j]>>2)&0x3f)<<5)+(((*vidBufB)[i][j]>>3)&0x1f);
		}
	}

	///// Запись HiColor bitmap-а
	thead.save3layers(GetTargetName("output.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufR),(unsigned char*)(*vidBufG),(unsigned char*)(*vidBufB) );


/////////////////////////////////////////////////////////////////////////


/*	XStream ff(GetTargetName("output.hc"), XS_OUT);
	unsigned short* line = new unsigned short[H_SIZE],*p;
	for(j = 0; j<V_SIZE; j++){
		p = line;
		for(i = 0; i<H_SIZE; i++){
			*p++ = (*vidBuf)[j][i];
		}
		ff.write(line,H_SIZE*2);
	}
	ff.close();
	delete line;
*/

	sur_scr.delete_data();
	delete [] (tempG);
	delete [] (tempV);
	//delete [] (vidBuf);
//	Компайр двух поверхностей
	unsigned char (*vidBufR2)[YS_Buf][XS_Buf];
	unsigned char (*vidBufG2)[YS_Buf][XS_Buf];
	unsigned char (*vidBufB2)[YS_Buf][XS_Buf];
	unsigned char (*vidBufSh)[YS_Buf][XS_Buf];
	vidBufR2=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufG2=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufB2=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufSh=new unsigned char [1][YS_Buf][XS_Buf];
	thead.load3layers(GetTargetName("outputnh.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufR2),(unsigned char*)(*vidBufG2),(unsigned char*)(*vidBufB2) );
	for(i=0; i<V_SIZE; i++){
		for(j=0; j<H_SIZE; j++){
			unsigned char atr=GetAt(j,i); atr&=~At_SHADOW;
			if( ((*vidBufR)[i][j]!=(*vidBufR2)[i][j]) || ((*vidBufG)[i][j]!=(*vidBufG2)[i][j]) || ((*vidBufB)[i][j]!=(*vidBufB2)[i][j]) ) {
				int a= round( (float)0.5*(((*vidBufR2)[i][j]-(*vidBufR)[i][j])) + (float)0.3*(((*vidBufG2)[i][j]-(*vidBufG)[i][j])) + (float)0.2*(((*vidBufB2)[i][j]-(*vidBufB)[i][j])) );
				if(a>255) a=255; if(a<0) a=0;
				(*vidBufSh)[i][j]=255-a;
				if(a > minLightForShadow)atr|=At_SHADOW;
			}
			else (*vidBufSh)[i][j]=255;
			MovAt(j,i,atr);
		}
	}
	thead.save3layers(GetTargetName("outputsh.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufSh),(unsigned char*)(*vidBufSh),(unsigned char*)(*vidBufSh) );
	for(i=0;i<V_SIZE;i++) vMap->changedT[i]=1;
	vMap->save3Buf();
/*	for(j=0; j<V_SIZE; j++){
		for(i=0; i<H_SIZE; i++){
			int r= (*vidBufR2)[i][j]-(*vidBufR)[i][j];
			int g= (*vidBufG2)[i][j]-(*vidBufG)[i][j];
			int b= (*vidBufB2)[i][j]-(*vidBufB)[i][j];
			if(r>255) r=255; if(r<0) r=0;
			if(g>255) g=255; if(g<0) g=0;
			if(b>255) b=255; if(b<0) b=0;
			(*vidBufR2)[i][j]=255-r;
			(*vidBufG2)[i][j]=255-g;
			(*vidBufB2)[i][j]=255-b;
		}
	}
	thead.save3layers(GetTargetName("outputsh.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufR2),(unsigned char*)(*vidBufG2),(unsigned char*)(*vidBufB2) );
*/
	delete [] (vidBufR);
	delete [] (vidBufG);
	delete [] (vidBufB);

	delete [] (vidBufR2);
	delete [] (vidBufG2);
	delete [] (vidBufB2);

}
void render025HiBmp0(int xl, int yl)
{
	int LightDeep=H_SIZE/2;
	double buf_shadow1[H_SIZE];
	double buf_shadow2[H_SIZE];
	double (*b_s)[H_SIZE] = &buf_shadow1;
	double (*b_s_o)[H_SIZE] = &buf_shadow2;
	(*b_s)[H_SIZE/2]=0;(*b_s_o)[H_SIZE/2]=0;

	int i,j;

//Рендер от 1-го источника верхняя четверть
	unsigned char *pc;
	char type;
	double d_bs=0;//int d_bs=0;
	//int delta_s=15<<16; //Начальное значение убывания тени
	double delta_s=begDelta_s;
	double dd_s=dDelta_s;
	unsigned char V;
	unsigned char *pv,*pa;//,*ps;
	int off;
	short *tg,*tv;
	unsigned char *tvidR,*tvidG,*tvidB;
	double s;
	int dg=begOffLight; //15 <<16
	int ddg=dOffLight; //2<<10
	int Light=begLight;
	//int dLight= 2<<12;
//Очистка буверов тени 
	for(i=0; i<H_SIZE; i++){ buf_shadow2[i]=buf_shadow1[i]=0;}


	for(i=0; i<LightDeep; i++){
		pc = &(vMap -> ClTrBuf[0][YCYCL(yl-i)][0]);
		pv = &(vMap -> VxBuf[0][YCYCL(yl-i)][0]);
		//ps = &(vMap -> SpecBuf[0][YCYCL(yl-i)][0]);
		pa = &(vMap -> AtBuf[0][YCYCL(yl-i)][0]);
		tg=&((*tempG)[YCYCL(yl-i)][0]);
		tv=&((*tempV)[YCYCL(yl-i)][0]);
		tvidR=&((*vidBufR)[YCYCL(yl-i)][0]);
		tvidG=&((*vidBufG)[YCYCL(yl-i)][0]);
		tvidB=&((*vidBufB)[YCYCL(yl-i)][0]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_o=&buf_shadow1; }
		else { b_s=&buf_shadow1; b_s_o=&buf_shadow2; }
		int delta_a= round((double)(0.5/i)*65536);
		for(j=-i; j<=i; j++){
			off=XCYCL(j+xl);
			type=(*(pa+off))&TrW_MASK;
			//if(type&At_WATER) V=*(ps+off);
			V=*(pv+off);
//			s=(*b_s_o)[H_SIZE/2 + round(d_bs*j)]; //(d_bs*j>>16)];
			double t1;
			if(j>=0){
				t1= (double)d_bs*j - floor(d_bs*j);
				s= ( ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)])*(1-t1) + ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)+1])*(t1) ) ; 
			}
			else {
				t1= ((double)d_bs*(-j) - floor(d_bs*(-j)) );
				s= ( ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))])*(1-t1) + ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))-1])*(t1) ) ; 
			}

///			if(s >(delta_s) ) s-=(delta_s);
///			else s=0;
			int dlina=radiusL(j,i);
			if(s > tgAnglLight[dlina])s-=tgAnglLight[dlina];
			else s=0;

/*			int ilf = MODL(256*3+ *(tv+off) + (dg>>16)) ;
			int ils;
			if(j>=0) ils= MODL(256*3+ *(tg+off) + (ddv*j>>16));
			else ils= MODL(256*3+ *(tg+off) - (ddv*(-j)>>16));*/
			int ilf,ils;
			if(j>=0){
				ilf= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*j)>>16) - (*(tg+off)*(delta_a*j)>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*j>>16)
				ils= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*j)>>16) + (*(tv+off)*(delta_a*j)>>16) ) ;
			}
			else { 
				ilf= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*(-j))>>16) + (*(tg+off)*(delta_a*(-j))>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*(-j)>>16)
				ils= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*(-j))>>16) - (*(tv+off)*(delta_a*(-j))>>16) ) ;
			}
			//ils=256*3;
			//ilf=256*3;
			int l;

			l=((terra.light_front[0][type][ilf] * terra.light_sideways[0][type][ils] >>8) +terra.ambient_light[0][type]);
			if (l > minLightForShadow){
				if (s-tgAnglLight[dlina] > V+1) {
//					if(*(pa+off) & At_SHADOWV) l-=l>>2; //Что-бы тень на пересечении была посветлее
//					else l=l>>1;
					l=l>>1;
					*(pa+off)|=At_SHADOWV;
				}
				else {
					//Не тень(Воксельная)
					s=V;
					if (*(pa+off)&At_SHADOWS) {
//						if(*(pa+off) & At_SHADOWV) l-=l>>2;
//						else l=l>>1;
						l=l>>1;
						*(pa+off)|=At_SHADOWV;
					}
				}
			}
			l+=Light - dLight*dlina >>16;
			if (l<0) l=0; if(l>255)l=255;
			//*(tvid+off)+=CLSlight[l];
/*			unsigned short r,g,b;
			r=*(tvidR+off);r+=CLSlightR[l]; if (r>255)r=255; *(tvidR+off)=r;
			g=*(tvidG+off);g+=CLSlightG[l]; if (g>255)g=255; *(tvidG+off)=g;
			b=*(tvidB+off);b+=CLSlightB[l]; if (b>255)b=255; *(tvidB+off)=b;*/
			int r,g,b;
			r=CLSlightR[l]; g=CLSlightG[l]; b=CLSlightB[l];
			r=r*CLSalpha1>>16; g=g*CLSalpha1>>16; b=b*CLSalpha1>>16;
			l=l*CLSalpha>>16;
			r+=*(tvidR+off); if (r>255)r=255; *(tvidR+off)=r;
			g+=*(tvidG+off); if (g>255)g=255; *(tvidG+off)=g;
			b+=*(tvidB+off); if (b>255)b=255; *(tvidB+off)=b;

			l= l+ *(pc+off);
			if(l>255)l=255;
			*(pc+off)=(unsigned char)l;

			(*b_s)[H_SIZE/2 +j]=s;
		}
///		if(delta_s > dd_s +minDelta_s) delta_s-=dd_s; //2<<10
		d_bs = round((double)i/(i+1)*65536);//(i<<16)/(i+2);//+ (1<<15) //(((i*2+1)+2)<<16)/i;
		d_bs=(double)i/(i+1);
		dg-=ddg;
		if(dg < minOffLight) dg=minOffLight;
		//Light-=dLight;
	}

}

void render025HiBmp1(int xl, int yl)
{
	int LightDeep=H_SIZE/2;
	double buf_shadow1[H_SIZE];
	double buf_shadow2[H_SIZE];
	double (*b_s)[H_SIZE] = &buf_shadow1;
	double (*b_s_o)[H_SIZE] = &buf_shadow2;
	(*b_s)[H_SIZE/2]=0;(*b_s_o)[H_SIZE/2]=0;

	int i,j;

//Рендер от 1-го источника верхняя четверть
	unsigned char *pc;
	char type;
	double d_bs=0;//int d_bs=0;
	//int delta_s=15<<16; //Начальное значение убывания тени
	double delta_s=begDelta_s;
	double dd_s=dDelta_s;
	unsigned char V;
	unsigned char *pv,*pa;//,*ps;
	int off;
	short *tg,*tv;
	unsigned char *tvidR,*tvidG,*tvidB;
	double s;
	int dg=begOffLight; //15 <<16
	int ddg=dOffLight; //2<<10
	int Light=begLight;
	//int dLight= 2<<12;
//Очистка буверов тени 
	for(i=0; i<H_SIZE; i++){ buf_shadow2[i]=buf_shadow1[i]=0;}


	for(i=1; i<LightDeep; i++){ //!
		pc = &(vMap -> ClTrBuf[0][0][XCYCL(xl-i)]); //! YCYCL(yl-i)
		pv = &(vMap -> VxBuf[0][0][XCYCL(xl-i)]); //!
		//ps = &(vMap -> SpecBuf[0][0][XCYCL(xl-i)]); //!
		pa = &(vMap -> AtBuf[0][0][XCYCL(xl-i)]); //!
		tg=&((*tempG)[0][XCYCL(xl-i)]); //!
		tv=&((*tempV)[0][XCYCL(xl-i)]); //!
		tvidR=&((*vidBufR)[0][XCYCL(xl-i)]);
		tvidG=&((*vidBufG)[0][XCYCL(xl-i)]);
		tvidB=&((*vidBufB)[0][XCYCL(xl-i)]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_o=&buf_shadow1; }
		else { b_s=&buf_shadow1; b_s_o=&buf_shadow2; }
		int delta_a= round((double)(0.5/i)*65536);
		for(j=-i+1; j<=i-1; j++){
			off=YCYCL(j+yl)*XS_Buf;
			type=(*(pa+off))&TrW_MASK;
			//if(type&At_WATER) V=*(ps+off);
			V=*(pv+off);
//			s=(*b_s_o)[H_SIZE/2 + round(d_bs*j)]; //(d_bs*j>>16)];
			double t1;
			if(j>=0){
				t1= (double)d_bs*j - floor(d_bs*j);
				s= ( ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)])*(1-t1) + ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)+1])*(t1) ) ; 
			}
			else {
				t1= ((double)d_bs*(-j) - floor(d_bs*(-j)) );
				s= ( ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))])*(1-t1) + ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))-1])*(t1) ) ; 
			}

///			if(s >(delta_s) ) s-=(delta_s);
///			else s=0;
			int dlina=radiusL(j,i);
			if(s > tgAnglLight[dlina])s-=tgAnglLight[dlina];
			else s=0;

			int ilf,ils;
			if(j>=0) {
				ilf= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*j)>>16) - (*(tv+off)*(delta_a*j)>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*j>>16)
				ils= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*j)>>16) + (*(tg+off)*(delta_a*j)>>16) ) ;
			}
			else {
				ilf= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*(-j))>>16) + (*(tv+off)*(delta_a*(-j))>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*(-j)>>16)
				ils= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*(-j))>>16) - (*(tg+off)*(delta_a*(-j))>>16) ) ;
			}
			//ils=256*3;
			//ilf=256*3;
			int l;
//			if( s-tgAnglLight[dlina] > V+1 ){ //delta_s
			l=((terra.light_front[0][type][ilf] * terra.light_sideways[0][type][ils] >>8) +terra.ambient_light[0][type]);
			if (l > minLightForShadow){
				if (s-tgAnglLight[dlina] > V+1) {
//					if(*(pa+off) & At_SHADOWV) l-=l>>2; //Что-бы тень на пересечении была посветлее
//					else l=l>>1;
					l=l>>1;
					*(pa+off)|=At_SHADOWV;
				}
				else {
					//Не тень(Воксельная)
					s=V;
					if (*(pa+off)&At_SHADOWS) {
//						if(*(pa+off) & At_SHADOWV) l-=l>>2;
//						else l=l>>1;
						l=l>>1;
						*(pa+off)|=At_SHADOWV;
					}
				}
			}
			l+=Light - dLight*dlina >>16;
			if (l<0) l=0; if(l>255)l=255;
			//*(tvid+off)+=CLSlight[l];
/*			unsigned short r,g,b;
			r=*(tvidR+off);r+=CLSlightR[l]; if (r>255)r=255; *(tvidR+off)=r;
			g=*(tvidG+off);g+=CLSlightG[l]; if (g>255)g=255; *(tvidG+off)=g;
			b=*(tvidB+off);b+=CLSlightB[l]; if (b>255)b=255; *(tvidB+off)=b;*/
			int r,g,b;
			r=CLSlightR[l]; g=CLSlightG[l]; b=CLSlightB[l];
			r=r*CLSalpha1>>16; g=g*CLSalpha1>>16; b=b*CLSalpha1>>16;
			l=l*CLSalpha>>16;
			r+=*(tvidR+off); if (r>255)r=255; *(tvidR+off)=r;
			g+=*(tvidG+off); if (g>255)g=255; *(tvidG+off)=g;
			b+=*(tvidB+off); if (b>255)b=255; *(tvidB+off)=b;

			l= l+ *(pc+off);
			if(l>255)l=255;
			*(pc+off)=(unsigned char)l;

			(*b_s)[H_SIZE/2 +j]=s;
		}
///		if(delta_s > dd_s +minDelta_s) delta_s-=dd_s; //2<<10
		d_bs = round((double)i/(i+1)*65536);//(i<<16)/(i+2);//+ (1<<15) //(((i*2+1)+2)<<16)/i;
		d_bs=(double)i/(i+1);
		dg-=ddg;
		if(dg < minOffLight) dg=minOffLight;
		//Light-=dLight;
	}

}

void render025HiBmp2(int xl, int yl)
{
	int LightDeep=H_SIZE/2;
	double buf_shadow1[H_SIZE];
	double buf_shadow2[H_SIZE];
	double (*b_s)[H_SIZE] = &buf_shadow1;
	double (*b_s_o)[H_SIZE] = &buf_shadow2;
	(*b_s)[H_SIZE/2]=0;(*b_s_o)[H_SIZE/2]=0;

	int i,j;

//Рендер от 1-го источника верхняя четверть
	unsigned char *pc;
	char type;
	double d_bs=0;//int d_bs=0;
	//int delta_s=15<<16; //Начальное значение убывания тени
	double delta_s=begDelta_s;
	double dd_s=dDelta_s;
	unsigned char V;
	unsigned char *pv,*pa;//,*ps;
	int off;
	short *tg,*tv;
	unsigned char *tvidR,*tvidG,*tvidB;
	double s;
	int dg=begOffLight; //15 <<16
	int ddg=dOffLight; //2<<10
	int Light=begLight;
	//int dLight= 2<<12;
//Очистка буверов тени 
	for(i=0; i<H_SIZE; i++){ buf_shadow2[i]=buf_shadow1[i]=0;}


	for(i=1; i<LightDeep; i++){ //!
		pc = &(vMap -> ClTrBuf[0][0][XCYCL(xl+i)]); //! YCYCL(yl-i)
		pv = &(vMap -> VxBuf[0][0][XCYCL(xl+i)]); //!
		//ps = &(vMap -> SpecBuf[0][0][XCYCL(xl+i)]); //!
		pa = &(vMap -> AtBuf[0][0][XCYCL(xl+i)]); //!
		tg=&((*tempG)[0][XCYCL(xl+i)]); //!
		tv=&((*tempV)[0][XCYCL(xl+i)]); //!
		tvidR=&((*vidBufR)[0][XCYCL(xl+i)]);
		tvidG=&((*vidBufG)[0][XCYCL(xl+i)]);
		tvidB=&((*vidBufB)[0][XCYCL(xl+i)]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_o=&buf_shadow1; }
		else { b_s=&buf_shadow1; b_s_o=&buf_shadow2; }
		int delta_a= round((double)(0.5/i)*65536);
		for(j=-i+1; j<=i-1; j++){
			off=YCYCL(j+yl)*XS_Buf;
			type=(*(pa+off))&TrW_MASK;
			//if(type&At_WATER) V=*(ps+off);
			V=*(pv+off);
//			s=(*b_s_o)[H_SIZE/2 + round(d_bs*j)]; //(d_bs*j>>16)];
			double t1;
			if(j>=0){
				t1= (double)d_bs*j - floor(d_bs*j);
				s= ( ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)])*(1-t1) + ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)+1])*(t1) ) ; 
			}
			else {
				t1= ((double)d_bs*(-j) - floor(d_bs*(-j)) );
				s= ( ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))])*(1-t1) + ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))-1])*(t1) ) ; 
			}

///			if(s >(delta_s) ) s-=(delta_s);
///			else s=0;
			int dlina=radiusL(j,i);
			if(s > tgAnglLight[dlina])s-=tgAnglLight[dlina];
			else s=0;

			int ilf,ils;
			if(j>=0) {
				ilf= MODL(256*3 - (*(tg+off)*((1<<16)-delta_a*j)>>16) - (*(tv+off)*(delta_a*j)>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*j>>16)
				ils= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*j)>>16) - (*(tg+off)*(delta_a*j)>>16) ) ;
			}
			else {
				ilf= MODL(256*3 - (*(tg+off)*((1<<16)-delta_a*(-j))>>16) + (*(tv+off)*(delta_a*(-j))>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*(-j)>>16)
				ils= MODL(256*3 + (*(tv+off)*((1<<16)-delta_a*(-j))>>16) + (*(tg+off)*(delta_a*(-j))>>16) ) ;
			}
			//ils=256*3;
			//ilf=256*3;
			int l;
//			if( s-tgAnglLight[dlina] > V+1 ){//delta_s
			l=((terra.light_front[0][type][ilf] * terra.light_sideways[0][type][ils] >>8) +terra.ambient_light[0][type]);
			if (l > minLightForShadow){
				if (s-tgAnglLight[dlina] > V+1) {
//					if(*(pa+off) & At_SHADOWV) l-=l>>2; //Что-бы тень на пересечении была посветлее
//					else l=l>>1;
					l=l>>1;
					*(pa+off)|=At_SHADOWV;
				}
				else {
					//Не тень(Воксельная)
					s=V;
					if (*(pa+off)&At_SHADOWS) {
//						if(*(pa+off) & At_SHADOWV) l-=l>>2;
//						else l=l>>1;
						l=l>>1;
						*(pa+off)|=At_SHADOWV;
					}
				}
			}
			l+=Light - dLight*dlina >>16;
			if (l<0) l=0; if(l>255)l=255;
			//*(tvid+off)+=CLSlight[l];
/*			unsigned short r,g,b;
			r=*(tvidR+off);r+=CLSlightR[l]; if (r>255)r=255; *(tvidR+off)=r;
			g=*(tvidG+off);g+=CLSlightG[l]; if (g>255)g=255; *(tvidG+off)=g;
			b=*(tvidB+off);b+=CLSlightB[l]; if (b>255)b=255; *(tvidB+off)=b;*/
			int r,g,b;
			r=CLSlightR[l]; g=CLSlightG[l]; b=CLSlightB[l];
			r=r*CLSalpha1>>16; g=g*CLSalpha1>>16; b=b*CLSalpha1>>16;
			l=l*CLSalpha>>16;
			r+=*(tvidR+off); if (r>255)r=255; *(tvidR+off)=r;
			g+=*(tvidG+off); if (g>255)g=255; *(tvidG+off)=g;
			b+=*(tvidB+off); if (b>255)b=255; *(tvidB+off)=b;

			l= l+ *(pc+off);
			if(l>255)l=255;
			*(pc+off)=(unsigned char)l;

			(*b_s)[H_SIZE/2 +j]=s;
		}
///		if(delta_s > dd_s +minDelta_s) delta_s-=dd_s; //2<<10
		d_bs = round((double)i/(i+1)*65536);//(i<<16)/(i+2);//+ (1<<15) //(((i*2+1)+2)<<16)/i;
		d_bs=(double)i/(i+1);
		dg-=ddg;
		if(dg < minOffLight) dg=minOffLight;
		//Light-=dLight;
	}

}

void render025HiBmp3(int xl, int yl)
{
	int LightDeep=H_SIZE/2;
	double buf_shadow1[H_SIZE];
	double buf_shadow2[H_SIZE];
	double (*b_s)[H_SIZE] = &buf_shadow1;
	double (*b_s_o)[H_SIZE] = &buf_shadow2;
	(*b_s)[H_SIZE/2]=0;(*b_s_o)[H_SIZE/2]=0;

	int i,j;

//Рендер от 1-го источника верхняя четверть
	unsigned char *pc;
	char type;
	double d_bs=0;//int d_bs=0;
	//int delta_s=15<<16; //Начальное значение убывания тени
	double delta_s=begDelta_s;
	double dd_s=dDelta_s;
	unsigned char V;
	unsigned char *pv,*pa;//,*ps;
	int off;
	short *tg,*tv;
	unsigned char *tvidR,*tvidG,*tvidB;
	double s;
	int dg=begOffLight; //15 <<16
	int ddg=dOffLight; //2<<10
	int Light=begLight;
	//int dLight= 2<<12;
//Очистка буверов тени 
	for(i=0; i<H_SIZE; i++){ buf_shadow2[i]=buf_shadow1[i]=0;}


	for(i=0; i<LightDeep; i++){
		pc = &(vMap -> ClTrBuf[0][YCYCL(yl+i)][0]);
		pv = &(vMap -> VxBuf[0][YCYCL(yl+i)][0]);
		//ps = &(vMap -> SpecBuf[0][YCYCL(yl+i)][0]);
		pa = &(vMap -> AtBuf[0][YCYCL(yl+i)][0]);
		tg=&((*tempG)[YCYCL(yl+i)][0]);
		tv=&((*tempV)[YCYCL(yl+i)][0]);
		tvidR=&((*vidBufR)[YCYCL(yl+i)][0]);
		tvidG=&((*vidBufG)[YCYCL(yl+i)][0]);
		tvidB=&((*vidBufB)[YCYCL(yl+i)][0]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_o=&buf_shadow1; }
		else { b_s=&buf_shadow1; b_s_o=&buf_shadow2; }
		int delta_a= round((double)(0.5/i)*65536);
		for(j=-i; j<=i; j++){
			off=XCYCL(j+xl);
			type=(*(pa+off))&TrW_MASK;
			//if(type&At_WATER) V=*(ps+off);
			V=*(pv+off);
//			s=(*b_s_o)[H_SIZE/2 + round(d_bs*j)]; //(d_bs*j>>16)];
			double t1;
			if(j>=0){
				t1= (double)d_bs*j - floor(d_bs*j);
				s= ( ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)])*(1-t1) + ((*b_s_o)[H_SIZE/2 + (int)(d_bs*j)+1])*(t1) ) ; 
			}
			else {
				t1= ((double)d_bs*(-j) - floor(d_bs*(-j)) );
				s= ( ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))])*(1-t1) + ((*b_s_o)[H_SIZE/2 - (int)(d_bs*(-j))-1])*(t1) ) ; 
			}

///			if(s >(delta_s) ) s-=(delta_s);
///			else s=0;
			int dlina=radiusL(j,i);
			if(s > tgAnglLight[dlina])s-=tgAnglLight[dlina];
			else s=0;

/*			int ilf = MODL(256*3+ *(tv+off) + (dg>>16)) ;
			int ils;
			if(j>=0) ils= MODL(256*3+ *(tg+off) + (ddv*j>>16));
			else ils= MODL(256*3+ *(tg+off) - (ddv*(-j)>>16));*/
			int ilf,ils;
			if(j>=0) {
				ilf= MODL(256*3 - (*(tv+off)*((1<<16)-delta_a*j)>>16) - (*(tg+off)*(delta_a*j)>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*j>>16)
				ils= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*j)>>16) - (*(tv+off)*(delta_a*j)>>16) ) ;
			}
			else {
				ilf= MODL(256*3 - (*(tv+off)*((1<<16)-delta_a*(-j))>>16) + (*(tg+off)*(delta_a*(-j))>>16) + (dg-dlina*ddg >>16) );//+(dg>>16)  ) ; //-(dgg*(-j)>>16)
				ils= MODL(256*3+ (*(tg+off)*((1<<16)-delta_a*(-j))>>16) + (*(tv+off)*(delta_a*(-j))>>16) ) ;
			}
			//ils=256*3;
			//ilf=256*3;
			int l;
//			if( s-tgAnglLight[dlina] > V+1 ){//delta_s
			l=((terra.light_front[0][type][ilf] * terra.light_sideways[0][type][ils] >>8) +terra.ambient_light[0][type]);
			if (l > minLightForShadow){
				if (s-tgAnglLight[dlina] > V+1) {
//					if(*(pa+off) & At_SHADOWV) l-=l>>2; //Что-бы тень на пересечении была посветлее
//					else l=l>>1;
					l=l>>1;
					*(pa+off)|=At_SHADOWV;
				}
				else {
					//Не тень(Воксельная)
					s=V;
					if (*(pa+off)&At_SHADOWS) {
//						if(*(pa+off) & At_SHADOWV) l-=l>>2;
//						else l=l>>1;
						l=l>>1;
						*(pa+off)|=At_SHADOWV;
					}
				}
			}
			l+=Light - dLight*dlina >>16;
			if (l<0) l=0; if(l>255)l=255;
			//*(tvid+off)+=CLSlight[l];
/*			unsigned short r,g,b;
			r=*(tvidR+off);r+=CLSlightR[l]; if (r>255)r=255; *(tvidR+off)=r;
			g=*(tvidG+off);g+=CLSlightG[l]; if (g>255)g=255; *(tvidG+off)=g;
			b=*(tvidB+off);b+=CLSlightB[l]; if (b>255)b=255; *(tvidB+off)=b;*/
			int r,g,b;
			r=CLSlightR[l]; g=CLSlightG[l]; b=CLSlightB[l];
			r=r*CLSalpha1>>16; g=g*CLSalpha1>>16; b=b*CLSalpha1>>16;
			l=l*CLSalpha>>16;
			r+=*(tvidR+off); if (r>255)r=255; *(tvidR+off)=r;
			g+=*(tvidG+off); if (g>255)g=255; *(tvidG+off)=g;
			b+=*(tvidB+off); if (b>255)b=255; *(tvidB+off)=b;

			l= l+ *(pc+off);
			if(l>255)l=255;
			*(pc+off)=(unsigned char)l;

			(*b_s)[H_SIZE/2 +j]=s;
		}
///		if(delta_s > dd_s +minDelta_s) delta_s-=dd_s; //2<<10
		d_bs = round((double)i/(i+1)*65536);//(i<<16)/(i+2);//+ (1<<15) //(((i*2+1)+2)<<16)/i;
		d_bs=(double)i/(i+1);
		dg-=ddg;
		if(dg < minOffLight) dg=minOffLight;
		//Light-=dLight;
	}

}

#endif // if defined(_SURMAP_)

/////////////////////////////////////////////////////////////////////////////////


#if defined(_SURMAP_)
#include "land.h"
s_renderV renderV;

void s_renderV::RenderShadowM3DPL(int xL, int yL, int zL, int RadiusL) //Не исправленные баги тени:
{											// 1.тень шире из-за трассировки || лучами
											// 2.Расстояние до тени считается до левого угла битмапа,
											// а надо до центра
	extern cInterfaceVisGeneric *gb_IVisGeneric;
	cM3D *M3D=(cM3D*)gb_IVisGeneric->GetM3D();
	assert(M3D);
	cMesh *Mesh=M3D->First();
	while(Mesh)
	{
		//if(IS_STATIC(Mesh->Type) && (Mesh->NumberTrack&STATIC_NUMBER) ) {
		if( IS_STATIC(Mesh->Type) && (!(Mesh->Attribute&MESH_NOT_SHADE)) ) { //Первая проверка не обязательна т.к. Surmape нет не статических объектов

			cShadow Sh;
			Sh.number=Mesh->ID;
			int meshX=XCYCL(round(Mesh->x())),meshY=YCYCL(round(Mesh->y()));
			int dxL=meshX-xL;
			int dyL=meshY-yL;

			int cmpx=H_SIZE; cmpx=(cmpx>>1);
			int cmpy=V_SIZE; cmpy=(cmpy>>1);
			if(dxL<-cmpx) dxL+=H_SIZE; if(dxL>cmpx)dxL-=H_SIZE; //Расцикливание по источнику
			if(dyL<-cmpy) dyL+=V_SIZE; if(dyL>cmpy)dyL-=V_SIZE;

			double LLight=sqrt((dxL)*(dxL)+(dyL)*(dyL) );
			//float f_dzx=1/tgAnglLight[round(LLight)];
			//double hLight=tgAnglLight[round(LLight)]*LLight;
			double f_dzx=1/((zL -(round(Mesh->z())) )/LLight);//YCYCL
			DrawMeshShade(&Sh,xL,yL,f_dzx,RadiusL);

			int dxS=Sh.x-xL,dyS=Sh.y-yL;
			if(dxS<-cmpx) dxS+=H_SIZE; if(dxS>cmpx)dxS-=H_SIZE; //Расцикливание по источнику
			if(dyS<-cmpy) dyS+=V_SIZE; if(dyS>cmpy)dyS-=V_SIZE;
			int LSh=sqrt((dxS)*(dxS)+(dyS)*(dyS) );
			double dshx=(dxL)/LLight;
			double dshy=(dyL)/LLight;
			short *shadeBuf=Sh.shade;
/*			int stl,v;
			for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)  
			{
				//int y=YCYCL(Sh.y+j), x=Sh.x,x_cycl=Sh.x,v;
				//unsigned char *pv0=&(vMap->VxBuf[0][y][0]);
				//unsigned char *pa0=&(vMap->AtBuf[0][y][0]);
				//while( (Sh.z<(v=*(pv0+x_cycl))&&((Sh.x-x)<Sh.xShade)) )
				//	x_cycl=XCYCL(--x);
				//if(v>Sh.z) continue;
				stl=Sh.xShade;
				v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)))>>SHIFT_FRACTION_VX;
				//f_dzx=1/tgAnglLight[LSh+stl];
				f_dzx=(double)(LSh+stl)/(zL-v);
				int xshade=round((stl)-(Sh.z-v)*f_dzx);
				while( xshade>0 )//или cmpy т.е. 1/2 карты
				{
					if( (xshade<Sh.xShade)&&(shadeBuf[xshade]> v)&&((LSh+stl)<cmpx) ) { // 
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWV); //*(pc0+x_cycl)>>=1;
						SetAt(XCYCL(round(Sh.x+dshx*stl+dshy*j)+1),YCYCL(round(Sh.y+dshy*stl-dshx*j)),At_SHADOWV); //*(pc0+x_cycl)>>=1;
					}
					stl--;
					v=GetAlt(XCYCL(round(Sh.x+dshx*stl+dshy*j)),YCYCL(round(Sh.y+dshy*stl-dshx*j)))>>SHIFT_FRACTION_VX;
					//f_dzx=1/tgAnglLight[LSh+stl];
					f_dzx=(double)(LSh+stl)/(zL-v);
					xshade=round((stl)-(Sh.z-v)*f_dzx);
				}
			}*/


/*			{	//Показ границ и самого битмапа тени, для отладки
				for (int j=0;j<Sh.yShade;j++,shadeBuf+=Sh.xShade)   {
					for (int i=0;i<Sh.xShade;i++)  {
					
						if (shadeBuf[i]>0) SetAt(XCYCL(round(Sh.x+dshx*i+dshy*j)),YCYCL(round(Sh.y+dshy*i-dshx*j)),At_SHADOWV); 
						if ( (i==0) || (i==Sh.xShade-1) || (j==0) || (j==Sh.yShade-1) ) SetAt(XCYCL(round(Sh.x+dshx*i+dshy*j)),YCYCL(round(Sh.y+dshy*i-dshx*j)),At_SHADOWV); 
					}
				}
			}*/
			int xBitC=round(Sh.x+dshx*Sh.xShade/2+dshy*Sh.yShade/2);//Координаты центра битмапа
			int yBitC=round(Sh.y+dshy*Sh.xShade/2-dshx*Sh.yShade/2);

			int dxSC=xBitC-xL,dySC=yBitC-yL;
			if(dxSC<-cmpx) dxSC+=H_SIZE; if(dxSC>cmpx)dxSC-=H_SIZE; //Расцикливание по источнику расстояния 
			if(dySC<-cmpy) dySC+=V_SIZE; if(dySC>cmpy)dySC-=V_SIZE; //от источника до центра тени

			int Rlb=sqrt((double)(dxSC*dxSC)+(double)(dySC*dySC));
			float cosPovK,sinPovK;
			if (Rlb>0){
				cosPovK=(float)dxSC/(float)Rlb;
				sinPovK=(float)dySC/(float)Rlb;
			}
			else { cosPovK=1; sinPovK=0; }

			int boundXL=Rlb-Sh.xShade/2;
			int boundXR=Rlb+Sh.xShade/2;
			int boundYL=-Sh.yShade/2;
			int boundYR=+Sh.yShade/2;

			//Расчет наиболее близкой точки которая может быть при высоте поверхности 1024
			//int ddl=(Rlb-Sh.xShade/2);
			//float ctgT;
			//if(zL>0)ctgT=(float)ddl/(float)zL;
			//int dl=round(-ctgT*1024);
			int dl=-2*Mesh->rmaxTotal();

			int x[4],y[4];
			//x[0]=Sh.x,y[0]=Sh.y;
			//x[1]=(round(Sh.x+dshx*0+dshy*Sh.yShade)), y[1]=(round(Sh.y+dshy*0-dshx*Sh.yShade));
			x[0]=(round(Sh.x+dshx*dl+dshy*0)), y[0]=(round(Sh.y+dshy*dl-dshx*0));
			x[1]=(round(Sh.x+dshx*dl+dshy*Sh.yShade)), y[1]=(round(Sh.y+dshy*dl-dshx*Sh.yShade));
			//if(Sh.number==10){ 
			//	int a=0; //Для остановки при отладке на определенной модели
			//}
			x[2]=(round(Sh.x+dshx*Sh.xShade+dshy*0)), y[2]=(round(Sh.y+dshy*Sh.xShade-dshx*0));
			x[3]=(round(Sh.x+dshx*Sh.xShade+dshy*Sh.yShade)), y[3]=(round(Sh.y+dshy*Sh.xShade-dshx*Sh.yShade));
			int i,t;
			//В нулевую ячейку помещаем самую верхнюю вешину
			for(i=1; i<4; i++) if(y[0]>y[i]) { t=x[0]; x[0]=x[i]; x[i]=t; t=y[0]; y[0]=y[i]; y[i]=t;}
			//В первую ячейку помещаем самую нижнию вешину
			for(i=2; i<4; i++) if(y[1]<y[i]) { t=x[1]; x[1]=x[i]; x[i]=t; t=y[1]; y[1]=y[i]; y[i]=t;}
			//Во вторую ячейку помещаем самую верхнюю из оставшихся вешину 
			if(y[2]>y[3]) { t=x[2]; x[2]=x[3]; x[3]=t; t=y[2]; y[2]=y[3]; y[3]=t;}
			//В третьей остается самая низшая из оставшихся вершин
			int dx02,dx03,dx21,dx31;
			int curXl=(x[0]<<16)+(1<<15),curXr=(x[0]<<16)+(1<<15); //Левая и правая границы четырехугольника
			int X=x[0], Y=y[0];
			int dxl,dxr;
			//Теперь 3 этапа заполнения четырехугольника 
			//1-й от 0 до 2-й вершины
			if((y[2]-y[0]) >0){ // само собой (y[3]-y[0]) >0
				dx02=((x[0]-x[2]<<16) ) / (y[2]-y[0]); //+(1<<15)
				dx03=((x[0]-x[3]<<16) ) / (y[3]-y[0]); //+(1<<15)
				if(dx02>dx03){ dxl=dx02; dxr=dx03;}
				else { dxl=dx03; dxr=dx02;}
				for (Y; Y<=y[2]; Y++){
					for(X=(curXl>>16); X<=(curXr>>16); X++){

						int V=GetAlt(XCYCL(X),YCYCL(Y))>>SHIFT_FRACTION_VX;
						int dxSL=(X-xL), dySL=(Y-yL);
						if(dxSL<-cmpx) dxSL+=H_SIZE; if(dxSL>cmpx)dxSL-=H_SIZE; //Расцикливание по источнику расстояния 
						if(dySL<-cmpy) dySL+=V_SIZE; if(dySL>cmpy)dySL-=V_SIZE; //от источника до точки
						float L=sqrt(dxSL*dxSL+ dySL*dySL);
						float ddxSL,ddySL;
						if(L>0) { ddxSL=dxSL/L; ddySL=dySL/L; }
						else { ddxSL=0; ddySL=0;}
						float dL;
						if (zL-V>0) dL=((float)L/(zL-V))*(float)V;
						else	 dL=0;
						float xs=dxSL + dL*ddxSL;
						float ys=dySL + dL*ddySL;
						int xns=round(xs*cosPovK + ys*sinPovK);
						int yns=round(xs*sinPovK - ys*cosPovK);
						//извлечение из тени и проверка по Z буферу
						if( (xns > boundXL) && (xns< boundXR) && (yns>boundYL) && (yns<boundYR) ){
							if (Sh.shade[(xns-boundXL)+((yns+boundYR)*Sh.xShade)]>V )
								SetAt(XCYCL(X),YCYCL(Y),At_SHADOWV);
						}


					}
					curXl-=dxl; curXr-=dxr;
				}
			}
			//2-й от 2 до 3-й вершины
			if((y[3]-y[2]) >0){ // само собой (y[1]-y[2])>0
				dx21=((x[2]-x[1]<<16) ) / (y[1]-y[2]); //+(1<<15)
				dx03=((x[0]-x[3]<<16) ) / (y[3]-y[0]);//Повтор вместо условия //+(1<<15)
				if(dx02>dx03){ dxl=dx21; dxr=dx03; curXl=x[2]<<16+(1<<15);} //Определение с какой стороны 2и 3я вершины; и 
				else { dxl=dx03; dxr=dx21; curXr=x[2]<<16+(1<<15); }		// коррекция curXr(curXl) чтобыне накапливалась ошибка
				for (Y; Y<=y[3]; Y++){
					for(X=(curXl>>16); X<=(curXr>>16); X++){
						//SetAt(XCYCL(X),YCYCL(Y),At_SHADOWV);
						int V=GetAlt(XCYCL(X),YCYCL(Y))>>SHIFT_FRACTION_VX;
						int dxSL=(X-xL), dySL=(Y-yL);
						if(dxSL<-cmpx) dxSL+=H_SIZE; if(dxSL>cmpx)dxSL-=H_SIZE; //Расцикливание по источнику расстояния 
						if(dySL<-cmpy) dySL+=V_SIZE; if(dySL>cmpy)dySL-=V_SIZE; //от источника до точки
						float L=sqrt(dxSL*dxSL+ dySL*dySL);
						float ddxSL,ddySL;
						if(L>0) { ddxSL=dxSL/L; ddySL=dySL/L; }
						else { ddxSL=0; ddySL=0;}
						float dL;
						if (zL-V>0) dL=((float)L/(zL-V))*(float)V;
						else	 dL=0;
						float xs=dxSL + dL*ddxSL;
						float ys=dySL + dL*ddySL;
						int xns=round(xs*cosPovK + ys*sinPovK);
						int yns=round(xs*sinPovK - ys*cosPovK);
						//извлечение из тени и проверка по Z буферу
						if( (xns > boundXL) && (xns< boundXR) && (yns>boundYL) && (yns<boundYR) ){
							if (Sh.shade[(xns-boundXL)+((yns+boundYR)*Sh.xShade)]>V )
								SetAt(XCYCL(X),YCYCL(Y),At_SHADOWV);
						}
					}
					curXl-=dxl; curXr-=dxr;
				}
			}
			//3-й от 3 до 1-ой вершины
			if((y[1]-y[3]) >0){ // само собой (y[1]-y[2])>0
				dx21=((x[2]-x[1]<<16) ) / (y[1]-y[2]);//+(1<<15)
				dx31=((x[3]-x[1]<<16) ) / (y[1]-y[3]);//Повтор вместо условия //+(1<<15)
				if(dx02>dx03){ dxl=dx21; dxr=dx31; curXr=x[3]<<16+(1<<15);}
				else { dxl=dx31; dxr=dx21; curXl=x[3]<<16+(1<<15);}
				for (Y; Y<=y[1]; Y++){
					for(X=(curXl>>16); X<=(curXr>>16); X++){
						//SetAt(XCYCL(X),YCYCL(Y),At_SHADOWV);
						int V=GetAlt(XCYCL(X),YCYCL(Y))>>SHIFT_FRACTION_VX;
						int dxSL=(X-xL), dySL=(Y-yL);
						if(dxSL<-cmpx) dxSL+=H_SIZE; if(dxSL>cmpx)dxSL-=H_SIZE; //Расцикливание по источнику расстояния 
						if(dySL<-cmpy) dySL+=V_SIZE; if(dySL>cmpy)dySL-=V_SIZE; //от источника до точки
						float L=sqrt(dxSL*dxSL+ dySL*dySL);
						float ddxSL,ddySL;
						if(L>0) { ddxSL=dxSL/L; ddySL=dySL/L; }
						else { ddxSL=0; ddySL=0;}
						float dL;
						if (zL-V>0) dL=((float)L/(zL-V))*(float)V;
						else	 dL=0;
						float xs=dxSL + dL*ddxSL;
						float ys=dySL + dL*ddySL;
						int xns=round(xs*cosPovK + ys*sinPovK);
						int yns=round(xs*sinPovK - ys*cosPovK);
						//извлечение из тени и проверка по Z буферу
						if( (xns > boundXL) && (xns< boundXR) && (yns>boundYL) && (yns<boundYR) ){
							if (Sh.shade[(xns-boundXL)+((yns+boundYR)*Sh.xShade)]>V )
								SetAt(XCYCL(X),YCYCL(Y),At_SHADOWV);
						}
					}
					curXl-=dxl; curXr-=dxr;
				}
			}

		}
		Mesh=M3D->Next();
	}
}

void s_renderV::init(void)
{
	k_Ambient=30;
	k_SAmbient=30;
/*	for(int i=0; i<TERRAIN_MAX; i++){
		ref_n[i]=5; //0-20(степень)
		ref_k[i]=50; //0-100
		self_luminescence[i]=0; //очистка самосвечения
		dif_k[i]=1;
	}
	self_luminescence[2]=90;
	ref_n[2]=0;
	ref_k[2]=0;
	dif_k[2]=(float)0.2;*/
}

void s_renderV::main(int num_track)
{
	//Инициализация (потом нужно заменить на загрузку данных)

	//Открытие Скрипта скрипта
	XBuffer buft;
	char tmpstr[10];
	buft < "SLightR" < itoa(num_track%10,tmpstr,10) < ".sst";
	if( sur_scr.load_scr(GetTargetName(buft))==0 ) return;
	sur_scr.load_data();

	int i,j;

	vidBufRI=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufGI=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufBI= new unsigned char [1][YS_Buf][XS_Buf];
	vidBufRO=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufGO=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufBO= new unsigned char [1][YS_Buf][XS_Buf];
	N_X=new char [1][YS_Buf][XS_Buf];
	N_Y=new char [1][YS_Buf][XS_Buf];
	N_Z=new char [1][YS_Buf][XS_Buf];

	shadowBuf=vMap -> ClTrBuf;
	vBuf=vMap -> VxBuf;

	buft.init();
	buft < "i_track" < itoa(num_track%10,tmpstr,10) < ".tga";
	TGAHEAD thead;
	thead.load3layers(GetTargetName(buft),H_SIZE,V_SIZE,(unsigned char*)(*vidBufRI),(unsigned char*)(*vidBufGI),(unsigned char*)(*vidBufBI) );


	//Создание нормалей вокселей
	for(i=0; i<V_SIZE; i++){ //Может быть оптимизировано
		for(j=0; j<H_SIZE; j++){
			unsigned int Z0,Z1,Z2; //Z0-главный, Z1 - правый, Z2 -верхний
			unsigned short (*tV)[YS_Buf][XS_Buf]=vMap -> VxBuf;
			//unsigned char (*tVL)[YS_Buf][XS_Buf]=vMap -> SpecBuf;
			Z0 =(tV[0][i][j]<<2);
			Z1 =(tV[0][i][XCYCL(j+1)]<<2);
			Z2 =(tV[0][YCYCL(i-1)][j]<<2);
			int A=(Z1-Z0),B=(Z0-Z2), C=-1<<8; 
			int ModV=round(sqrt( A*A + B*B + C*C ));
			N_X[0][i][j]=(A<<8)/ModV>>2;
			N_Y[0][i][j]=(B<<8)/ModV>>2; 
			N_Z[0][i][j]=(C<<8)/ModV>>2;

			//очистка
			shadowBuf[0][i][j]=0;
			//Занесение амбиентного источника и самосветящихся поверхностей
			int rt=0;//(*vidBufRI)[i][j]*k_Ambient/100; 
			int gt=0;//(*vidBufGI)[i][j]*k_Ambient/100;
			int bt=0;//(*vidBufBI)[i][j]*k_Ambient/100;
			unsigned char s_l=self_luminescence[GetTer(j,i)];
			if(s_l!=0){
				rt+=(*vidBufRI)[i][j]*s_l/100;
				gt+=(*vidBufGI)[i][j]*s_l/100;
				bt+=(*vidBufBI)[i][j]*s_l/100;
			}
			if(rt>255)rt=255; if(gt>255)gt=255; if(bt>255)bt=255;
			(*vidBufRO)[i][j]=rt;
			(*vidBufGO)[i][j]=gt;
			(*vidBufBO)[i][j]=bt;
			vMap -> AtBuf[0][i][j]&=~At_SHADOW;
		}
	}

	calculateCOSnG();
	//float nxVis=1/sqrt(3), nyVis=1/sqrt(3), nzVis=1/sqrt(3);
	int xVis, yVis, zVis=1000; 
	float nxVis,nyVis,nzVis;

	for(i=0; i<sur_scr.numbers_cell; i++){
		int xL=1024,yL=1024,zL=1512;
		int rL=150,gL=150,bL=150;
		//int Lambient=30;
		int k_light,Radius,k_SmusShadow;


		sur_scr.row[0].get_el(i,xL);
		sur_scr.row[1].get_el(i,yL);
		xVis=xL, yVis=yL, zVis=GetAlt(xL,yL)+50;	// Для расчета угла зрения
		sur_scr.row[2].get_el(i,zL);
		sur_scr.row[3].get_el(i,rL);
		sur_scr.row[4].get_el(i,gL);
		sur_scr.row[5].get_el(i,bL);
		sur_scr.row[6].get_el(i,k_light);
		rL=rL*k_light/100; //if (rL>255) rL=255;
		gL=gL*k_light/100; //if (gL>255) gL=255;
		bL=bL*k_light/100; //if (bL>255) bL=255;
		float kR=(float)rL/MAX_LIGHT;
		float kG=(float)gL/MAX_LIGHT;
		float kB=(float)bL/MAX_LIGHT;
		sur_scr.row[7].get_el(i,Radius);
		sur_scr.row[8].get_el(i,k_SmusShadow);
/*		sur_scr.row[9].get_el(i,t_radius);//t_dLight);
		////
		sur_scr.row[10].get_el(i,t_r);
		sur_scr.row[11].get_el(i,t_g);
		sur_scr.row[12].get_el(i,t_b);
		sur_scr.row[13].get_el(i,t_a);
		sur_scr.row[14].get_el(i,t_dlinaTeni);*/


		calculateLightOnDistance(Radius); //Расчет яркости в зависимости от расстояния
		calculateShadowVx(xL,yL,zL,Radius);
//		if(Radius <=1024){
			RenderShadowM3DPL(xL,yL,zL,Radius);
//		}
//		else {
//			RenderShadowM3DPL(xL,yL,zL,1023);
//		}
		prepareGaussFilter(k_SmusShadow);// (радиус/10)
		//float kl=1,kml=0.001; //kml - коэффициент масштабирования по расстоянию затухание источника
		double k_zat=1;//0.999;//0.2
		//int begL=255;
		float Ld,Ls,Ldif,Lspec; //Ld - освещение с учетом коэффициента диффузного отражения
								//Ls - освещение с учетом коэффициента спекулярного отражения
		int l;


		//непосредственно рендер
		int x,y,xx,yy;
		int dx,dy,dz;
		int dxVis,dyVis,dzVis,lVis;
		unsigned char ShadowIs;
//		for( y=0; y<V_SIZE; y++)
		for( yy=yL-Radius; yy<yL+Radius; yy++){
			y=YCYCL(yy);
			//dy=calcDY(yL,y);//yL-y;
			//dyVis=calcDY(yVis,y);					//Для расчета угла зрения
			dy=yy-yL;
			dyVis=yy-yVis;
//			for( x=0; x<H_SIZE; x++)
			for( xx=xL-Radius; xx< xL+Radius; xx++){
				x=XCYCL(xx);
				//dx=calcDX(xL,x);//xL-x;
				dx=xx-xL;
				dz=(vBuf[0][y][x]>>SHIFT_FRACTION_VX)-zL;

				//Тень, которая выходит за границы мира была положена атрибутом At_SHADOWS
				if( abs(dy)>V_SIZE/2 || (abs(dx) > H_SIZE/2) ) ShadowIs=At_SHADOWS;
				else {
					//if( ((abs(dy)< V_SIZE/2+3) && (abs(dy)> V_SIZE/2-3)) || ((abs(dx)< H_SIZE/2+3) && (abs(dx)> H_SIZE/2-3)) ){
					//	ShadowIs=At_SHADOW;
					//}
					//else 
				//Тень, которая не выходит за границы мира была положена атрибутом At_SHADOWV
						ShadowIs=At_SHADOWV;
				}

				//dxVis=calcDX(xVis,x);					 //Для расчета угла зрения
				dxVis=xx-xVis;
				dzVis=zVis-(vBuf[0][y][x]>>SHIFT_FRACTION_VX);
				lVis=round(sqrt(dxVis*dxVis+dyVis*dyVis+dzVis*dzVis));
				nxVis=(float)-dxVis/lVis, nyVis=(float)-dyVis/lVis, nzVis=(float)dzVis/lVis;

				l=round(sqrt(dx*dx+dy*dy+dz*dz));
				unsigned char atr=GetAt(x,y);
				unsigned char terrain=atr&Tr_MASK;//GetTer(x,y);
				//L=(float)begL/((float)l*kml+kl); // Яркость источника в данной точке
				//L=begL*(pow(k_zat,l>>2));
				//L=begL-k_zat*(float)l; if(L<0)L=0;
				float lightDistance=tableLightOnDistance[round(sqrt(dx*dx+dy*dy))];//+XRnd(10);
				Ld=lightDistance*dif_k[terrain];
				Ls=lightDistance;
				float nxL=(float)dx/l,nyL=(float)dy/l,nzL=(float)dz/l;
				float cosA=((int)N_X[0][y][x]*nxL + (int)N_Y[0][y][x]*nyL +(int)N_Z[0][y][x]*nzL)/64;// на 64 умножена нормаль вокселей (<<8 и >>2)
				if (cosA<0) cosA=0;
				Ldif=Ld*cosA; if(Ldif>256)Ldif=256; //+Lambient
				float cosLV=nxL*nxVis + nyL*nyVis + nzL*nzVis;
				float cosB=( (int)N_X[0][y][x]*nxVis +(int)N_Y[0][y][x]*nyVis +(int)N_Z[0][y][x]*nzVis)/64;
				int cosG=(cosLV-2*cosA*cosB)*255;
				if (cosG > 255) cosG=255; if(cosG < 0)cosG=0; //if(cosG < -255)cosG=-255;
				Lspec=Ls*COSnG[terrain][256+cosG];
				int ro,go,bo;
/*				if((atr&At_SHADOWV)==0){//if(shadowBuf[0][y][x]==0){ //нет тени
					ro=vidBufRI[0][y][x]*Ldif*kR/256 + (rL*Lspec/(256*256));
					go=vidBufGI[0][y][x]*Ldif*kG/256 + (gL*Lspec/(256*256));
					bo=vidBufBI[0][y][x]*Ldif*kB/256 + (bL*Lspec/(256*256));
				}
				else { //есть тень
					ro=0; go=0; bo=0;
					//ro=(vidBufRI[0][y][x]*Ldif/256)*k_ambient/100;
					//go=(vidBufGI[0][y][x]*Ldif/256)*k_ambient/100;
					//bo=(vidBufBI[0][y][x]*Ldif/256)*k_ambient/100;
				}*/
					ro=vidBufRI[0][y][x]*Ldif*kR/256 + (rL*Lspec/(256*256));
					go=vidBufGI[0][y][x]*Ldif*kG/256 + (gL*Lspec/(256*256));
					bo=vidBufBI[0][y][x]*Ldif*kB/256 + (bL*Lspec/(256*256));
					int shad=0;
					for(int k=-radius_gb; k< radius_gb; k++){
						for(int m=-radius_gb; m< radius_gb; m++){
							if(GetAt(XCYCL(x+m),YCYCL(y+k))&ShadowIs) shad+=gaussBuf[radius_gb+m+(radius_gb+k)*diametr_gb];
						}
					}
					ro=ro*(MAX_VALUE_GB+1-shad)/(MAX_VALUE_GB+1);
					go=go*(MAX_VALUE_GB+1-shad)/(MAX_VALUE_GB+1);
					bo=bo*(MAX_VALUE_GB+1-shad)/(MAX_VALUE_GB+1);
					int sh=shadowBuf[0][y][x];
					sh+=lightDistance - lightDistance*(MAX_VALUE_GB+1-shad)/(MAX_VALUE_GB+1);
					if(sh>255)sh=255; if(sh<0)sh=0;
					shadowBuf[0][y][x]=sh;

				ro+=vidBufRO[0][y][x];
				go+=vidBufGO[0][y][x];
				bo+=vidBufBO[0][y][x];
				if(ro>255)ro=255;if(go>255)go=255;if(bo>255)bo=255;
				vidBufRO[0][y][x]=ro;
				vidBufGO[0][y][x]=go;
				vidBufBO[0][y][x]=bo;

			}
		}





	}
	for(i=0; i<V_SIZE; i++){
		for(j=0; j<H_SIZE; j++){
			int sh=shadowBuf[0][i][j];
			int rt,gt,bt;
			if(sh==0){
				rt=(*vidBufRI)[i][j]*k_Ambient/100; 
				gt=(*vidBufGI)[i][j]*k_Ambient/100;
				bt=(*vidBufBI)[i][j]*k_Ambient/100;
			}
			else{
				rt=((*vidBufRI)[i][j]*k_Ambient/100)*(256-sh)/256;
				gt=((*vidBufGI)[i][j]*k_Ambient/100)*(256-sh)/256;
				bt=((*vidBufBI)[i][j]*k_Ambient/100)*(256-sh)/256;
				rt+=((*vidBufRI)[i][j]*k_SAmbient/100)*(sh)/256;
				gt+=((*vidBufGI)[i][j]*k_SAmbient/100)*(sh)/256;
				bt+=((*vidBufBI)[i][j]*k_SAmbient/100)*(sh)/256;
			}
			rt+=(*vidBufRO)[i][j];
			gt+=(*vidBufGO)[i][j];
			bt+=(*vidBufBO)[i][j];
			if(rt>255)rt=255; if(gt>255)gt=255; if(bt>255)bt=255;
			(*vidBufRO)[i][j]=rt;
			(*vidBufGO)[i][j]=gt;
			(*vidBufBO)[i][j]=bt;

			vMap -> AtBuf[0][i][j]&=~At_SHADOW;
			if(sh > 20) vMap -> AtBuf[0][i][j]|=At_SHADOWV;

		}
	}




	XBuffer bt;
	char ts[10];
	bt < "track" < itoa(num_track%10,ts,10) < ".tga";
	thead.save3layers(GetTargetName(bt),H_SIZE,V_SIZE,(unsigned char*)(*vidBufRO),(unsigned char*)(*vidBufGO),(unsigned char*)(*vidBufBO) );

	vMap->saveVMM();

	//Конверт из Try в High Color для показа на экране
	// И изменение режима показа на экране
	for(i=0; i<YS_Buf; i++){ 
		for(j=0; j<XS_Buf; j++){
			vMap->ClBuf[0][i][j]=((((*vidBufRO)[i][j]>>3)&0x1f)<<11)+((((*vidBufGO)[i][j]>>2)&0x3f)<<5)+(((*vidBufBO)[i][j]>>3)&0x1f);
		}
	}
	GCM.setFV( 1 , FLAG_Show_PrimaryOrRender_World);

	//Показ где тень
	//for(i=0; i<V_SIZE; i++) for(j=0; j<H_SIZE; j++) {
	//	if(GetAt(j,i)&At_SHADOWV) vMap->ClTrBuf[0][i][j]=255;
	//}



	delete [] vidBufRI;
	delete [] vidBufGI;
	delete [] vidBufBI;
	delete [] vidBufRO;
	delete [] vidBufGO;
	delete [] vidBufBO;
	delete [] N_X;
	delete [] N_Y;
	delete [] N_Z;


}

void s_renderV::calculateCOSnG(void)
{
	int i,j;
	for(i=0; i<TERRAIN_MAX; i++){
		for(j=-256; j<256; j++){
			double m=pow((double)j/256,((double)ref_n[i]/10) )* ((double)ref_k[i]/100);
			COSnG[i][256+j]=round(m*256);
		}
	}
}

void s_renderV::calculateLightOnDistance(int distance)
{
		//L=begL*(pow(k_zat,l>>2));
/*	int i;
	double k=1.0/(double)distance;
	for(i=0; i<MAX_DISTANCE; i++){
		double t=sqrt(1.0*1.0 - k*i);
		tableLightOnDistance[i]=MAX_LIGHT*t;
		//tableLightOnDistance[i]=MAX_LIGHT-(k*i)*MAX_LIGHT;
		if(tableLightOnDistance[i]<0) tableLightOnDistance[i]=0;
	}*/
	int i;
	double k=MAX_LIGHT/((double)distance/4);
	double cur_light=MAX_LIGHT;
	for(i=0; i<MAX_DISTANCE; i++){
		if( i< (distance-distance/4)) {
			tableLightOnDistance[i]=cur_light;
		}
		else {
			cur_light-=k;
			tableLightOnDistance[i]=cur_light;
		}
		if(tableLightOnDistance[i]<0) tableLightOnDistance[i]=0;
	}

}

void s_renderV::calculateShadowVx(int xl, int yl,int zl, int Radius_Light)
{
	//int LightDeep=H_SIZE/2;
	const int BUF_CENTR=2*H_SIZE/2+1;
	const int BUF_SIZE=2*H_SIZE+2;
	float buf_shadow1[BUF_SIZE];
	float buf_delta_shadow1[BUF_SIZE];
	float buf_shadow2[BUF_SIZE];
	float buf_delta_shadow2[BUF_SIZE];
	float (*b_s)[BUF_SIZE] = &buf_shadow1;
	float (*b_s_i)[BUF_SIZE] = &buf_shadow2;
	(*b_s)[BUF_CENTR]=0;(*b_s_i)[BUF_CENTR]=0;
	float (*b_ds)[BUF_SIZE] = &buf_delta_shadow1;
	float (*b_ds_i)[BUF_SIZE] = &buf_delta_shadow2;
	(*b_ds)[BUF_CENTR]=0;(*b_ds_i)[BUF_CENTR]=0;

	int i,j;
	//Очистка атрибутов тени (At_SHADOWV и At_SHADOS)
	for(i=0;i<V_SIZE;i++) for(j=0;j<H_SIZE;j++) vMap->AtBuf[0][i][j]&=~At_SHADOW;

	char typeL,typeR;
	float d_bs=0;//int d_bs=0;
	float VL,VR;
	unsigned char *pa;//,*ps;
	unsigned short *pv;
	int offL,offR;
	float sL,dsL,sR,dsR;
	unsigned char ShadowIs;
	//Расчет тени Верхней четверти
	//Очистка буверов тени 
	for(i=0; i<BUF_SIZE; i++){ buf_delta_shadow2[i]=buf_delta_shadow1[i]=buf_shadow2[i]=buf_shadow1[i]=0;}

	for(i=0; i<Radius_Light; i++){
		pv = &(vMap -> VxBuf[0][YCYCL(yl-i)][0]);
		pa = &(vMap -> AtBuf[0][YCYCL(yl-i)][0]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_i=&buf_shadow1; b_ds=&buf_delta_shadow2; b_ds_i=&buf_delta_shadow1;}
		else { b_s=&buf_shadow1; b_s_i=&buf_shadow2; b_ds=&buf_delta_shadow1; b_ds_i=&buf_delta_shadow2;}
		if( i > V_SIZE/2) ShadowIs=At_SHADOWS;
		else ShadowIs=At_SHADOWV;
		for(j=i; j>=0; j--){
			offL=XCYCL(xl-j);
			offR=XCYCL(xl+j);
			typeL=(*(pa+offL))&TrW_MASK;
			typeR=(*(pa+offR))&TrW_MASK;
			VL=(float)*(pv+offL)/(float)(1<<VX_FRACTION);
			VR=(float)*(pv+offR)/(float)(1<<VX_FRACTION);
//			s=(*b_s_i)[BUF_CENTR + round(d_bs*j)]; //(d_bs*j>>16)];
			float t1;
			t1= d_bs*j - floor(d_bs*j);

			int ind1=i-1-ceil(d_bs*(i-j));
			int ind2=i-1-floor(d_bs*(i-j));
			//int ind1=floor(d_bs*-j);
			//int ind2=ceil(d_bs*-j);
			if(j!=0){
				sL= ( ((*b_s_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR - ind2])*(t1) ) ; 
				dsL= ( ((*b_ds_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR - ind2])*(t1) ) ; 

				sR= ( ((*b_s_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR + ind2])*(t1) ) ; 
				dsR= ( ((*b_ds_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR + ind2])*(t1) ) ; 
			}
			else {
				sL= (*b_s_i)[BUF_CENTR] ; 
				dsL=(*b_ds_i)[BUF_CENTR]; 

				sR= (*b_s_i)[BUF_CENTR] ; 
				dsR=(*b_ds_i)[BUF_CENTR]; 
			}


			
			sL-=dsL;
			sR-=dsR;

			if (sL > VL)  { *(pa+offL)|=ShadowIs; } //Тень
			else { 				//Не тень(Воксельная)
				//*(pa+offL)&=~At_SHADOWV;
				sL=VL;
				dsL=(float)(zl-VL)/(float)(i);
			}
			if (sR > VR)  { *(pa+offR)|=ShadowIs; } //Тень
			else {
				//Не тень(Воксельная)
				//*(pa+offR)&=~At_SHADOWV;
				sR=VR;
				dsR=(float)(zl-VR)/(float)(i);
			}

			(*b_s)[BUF_CENTR -j]=sL;
			(*b_ds)[BUF_CENTR -j]=dsL;
			(*b_s)[BUF_CENTR +j]=sR;
			(*b_ds)[BUF_CENTR +j]=dsR;
		}
		d_bs=(float)i/(i+1);
	}

	//Расчет тени нижней четверти
	//Очистка буверов тени 
	for(i=0; i<BUF_SIZE; i++){ buf_delta_shadow2[i]=buf_delta_shadow1[i]=buf_shadow2[i]=buf_shadow1[i]=0;}

	for(i=0; i<Radius_Light; i++){
		pv = &(vMap -> VxBuf[0][YCYCL(yl+i)][0]);
		pa = &(vMap -> AtBuf[0][YCYCL(yl+i)][0]);
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_i=&buf_shadow1; b_ds=&buf_delta_shadow2; b_ds_i=&buf_delta_shadow1;}
		else { b_s=&buf_shadow1; b_s_i=&buf_shadow2; b_ds=&buf_delta_shadow1; b_ds_i=&buf_delta_shadow2;}
		if( i > V_SIZE/2) ShadowIs=At_SHADOWS;
		else ShadowIs=At_SHADOWV;
		for(j=i; j>=0; j--){
			offL=XCYCL(xl-j);
			offR=XCYCL(xl+j);
			typeL=(*(pa+offL))&TrW_MASK;
			typeR=(*(pa+offR))&TrW_MASK;
			VL=(float)*(pv+offL)/(float)(1<<VX_FRACTION);
			VR=(float)*(pv+offR)/(float)(1<<VX_FRACTION);
//			s=(*b_s_i)[BUF_CENTR + round(d_bs*j)]; //(d_bs*j>>16)];
			float t1;
			t1= d_bs*j - floor(d_bs*j);

			int ind1=i-1-ceil(d_bs*(i-j));
			int ind2=i-1-floor(d_bs*(i-j));
			//int ind1=floor(d_bs*-j);
			//int ind2=ceil(d_bs*-j);
			if(j!=0){
				sL= ( ((*b_s_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR - ind2])*(t1) ) ; 
				dsL= ( ((*b_ds_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR - ind2])*(t1) ) ; 

				sR= ( ((*b_s_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR + ind2])*(t1) ) ; 
				dsR= ( ((*b_ds_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR + ind2])*(t1) ) ; 
			}
			else {
				sL= (*b_s_i)[BUF_CENTR] ; 
				dsL=(*b_ds_i)[BUF_CENTR]; 

				sR= (*b_s_i)[BUF_CENTR] ; 
				dsR=(*b_ds_i)[BUF_CENTR]; 
			}


			
			sL-=dsL;
			sR-=dsR;

			if (sL > VL)  { *(pa+offL)|=ShadowIs; } //Тень
			else { 				//Не тень(Воксельная)
				//*(pa+offL)&=~At_SHADOWV;
				sL=VL;
				dsL=(float)(zl-VL)/(float)(i);
			}
			if (sR > VR)  { *(pa+offR)|=ShadowIs; } //Тень
			else {
				//Не тень(Воксельная)
				//*(pa+offR)&=~At_SHADOWV;
				sR=VR;
				dsR=(float)(zl-VR)/(float)(i);
			}

			(*b_s)[BUF_CENTR -j]=sL;
			(*b_ds)[BUF_CENTR -j]=dsL;
			(*b_s)[BUF_CENTR +j]=sR;
			(*b_ds)[BUF_CENTR +j]=dsR;
		}
		d_bs=(float)i/(i+1);
	}


	//Расчет тени левой четверти
	//Очистка буверов тени 
	for(i=0; i<BUF_SIZE; i++){ buf_delta_shadow2[i]=buf_delta_shadow1[i]=buf_shadow2[i]=buf_shadow1[i]=0;}

	for(i=1; i<Radius_Light; i++){
		pv = &(vMap -> VxBuf[0][0][XCYCL(xl-i)]); 
		pa = &(vMap -> AtBuf[0][0][XCYCL(xl-i)]); 
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_i=&buf_shadow1; b_ds=&buf_delta_shadow2; b_ds_i=&buf_delta_shadow1;}
		else { b_s=&buf_shadow1; b_s_i=&buf_shadow2; b_ds=&buf_delta_shadow1; b_ds_i=&buf_delta_shadow2;}
		if( i > V_SIZE/2) ShadowIs=At_SHADOWS;
		else ShadowIs=At_SHADOWV;
		for(j=i-1; j>=0; j--){
			offL=YCYCL(yl-j)*XS_Buf;
			offR=YCYCL(yl+j)*XS_Buf;
			typeL=(*(pa+offL))&TrW_MASK;
			typeR=(*(pa+offR))&TrW_MASK;
			VL=(float)*(pv+offL)/(float)(1<<VX_FRACTION);
			VR=(float)*(pv+offR)/(float)(1<<VX_FRACTION);
//			s=(*b_s_i)[BUF_CENTR + round(d_bs*j)]; //(d_bs*j>>16)];
			float t1;
			t1= d_bs*j - floor(d_bs*j);

			int ind1=i-1-1-ceil(d_bs*(i-1-j));
			int ind2=i-1-1-floor(d_bs*(i-1-j));
			//int ind1=floor(d_bs*-j);
			//int ind2=ceil(d_bs*-j);
			if(j!=0){
				sL= ( ((*b_s_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR - ind2])*(t1) ) ; 
				dsL= ( ((*b_ds_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR - ind2])*(t1) ) ; 

				sR= ( ((*b_s_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR + ind2])*(t1) ) ; 
				dsR= ( ((*b_ds_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR + ind2])*(t1) ) ; 
			}
			else {
				sL= (*b_s_i)[BUF_CENTR] ; 
				dsL=(*b_ds_i)[BUF_CENTR]; 

				sR= (*b_s_i)[BUF_CENTR] ; 
				dsR=(*b_ds_i)[BUF_CENTR]; 
			}


			
			sL-=dsL;
			sR-=dsR;

			if (sL > VL)  { *(pa+offL)|=ShadowIs; } //Тень
			else { 				//Не тень(Воксельная)
				//*(pa+offL)&=~At_SHADOWV;
				sL=VL;
				dsL=(float)(zl-VL)/(float)(i);
			}
			if (sR > VR)  { *(pa+offR)|=ShadowIs; } //Тень
			else {
				//Не тень(Воксельная)
				//*(pa+offR)&=~At_SHADOWV;
				sR=VR;
				dsR=(float)(zl-VR)/(float)(i);
			}

			(*b_s)[BUF_CENTR -j]=sL;
			(*b_ds)[BUF_CENTR -j]=dsL;
			(*b_s)[BUF_CENTR +j]=sR;
			(*b_ds)[BUF_CENTR +j]=dsR;
		}
		d_bs=(float)i/(i+1);
	}

	//Расчет тени правой четверти
	//Очистка буверов тени 

	for(i=0; i<BUF_SIZE; i++){ buf_delta_shadow2[i]=buf_delta_shadow1[i]=buf_shadow2[i]=buf_shadow1[i]=0;}

	for(i=1; i<Radius_Light; i++){
		pv = &(vMap -> VxBuf[0][0][XCYCL(xl+i)]); 
		pa = &(vMap -> AtBuf[0][0][XCYCL(xl+i)]); 
		if(b_s == &buf_shadow1) { b_s=&buf_shadow2; b_s_i=&buf_shadow1; b_ds=&buf_delta_shadow2; b_ds_i=&buf_delta_shadow1;}
		else { b_s=&buf_shadow1; b_s_i=&buf_shadow2; b_ds=&buf_delta_shadow1; b_ds_i=&buf_delta_shadow2;}
		if( i > V_SIZE/2) ShadowIs=At_SHADOWS;
		else ShadowIs=At_SHADOWV;
		for(j=i-1; j>=0; j--){
			offL=YCYCL(yl-j)*XS_Buf;
			offR=YCYCL(yl+j)*XS_Buf;
			typeL=(*(pa+offL))&TrW_MASK;
			typeR=(*(pa+offR))&TrW_MASK;
			VL=(float)*(pv+offL)/(float)(1<<VX_FRACTION);
			VR=(float)*(pv+offR)/(float)(1<<VX_FRACTION);
//			s=(*b_s_i)[BUF_CENTR + round(d_bs*j)]; //(d_bs*j>>16)];
			float t1;
			t1= d_bs*j - floor(d_bs*j);

			int ind1=i-1-1-ceil(d_bs*(i-1-j));
			int ind2=i-1-1-floor(d_bs*(i-1-j));
			//int ind1=floor(d_bs*-j);
			//int ind2=ceil(d_bs*-j);
			if(j!=0){
				sL= ( ((*b_s_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR - ind2])*(t1) ) ; 
				dsL= ( ((*b_ds_i)[BUF_CENTR - ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR - ind2])*(t1) ) ; 

				sR= ( ((*b_s_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_s_i)[BUF_CENTR + ind2])*(t1) ) ; 
				dsR= ( ((*b_ds_i)[BUF_CENTR + ind1])*(1.0-t1) + ((*b_ds_i)[BUF_CENTR + ind2])*(t1) ) ; 
			}
			else {
				sL= (*b_s_i)[BUF_CENTR] ; 
				dsL=(*b_ds_i)[BUF_CENTR]; 

				sR= (*b_s_i)[BUF_CENTR] ; 
				dsR=(*b_ds_i)[BUF_CENTR]; 
			}

			
			sL-=dsL;
			sR-=dsR;

			if (sL > VL)  { *(pa+offL)|=ShadowIs; } //Тень
			else { 				//Не тень(Воксельная)
				//*(pa+offL)&=~At_SHADOWV;
				sL=VL;
				dsL=(float)(zl-VL)/(float)(i);
			}
			if (sR > VR)  { *(pa+offR)|=ShadowIs; } //Тень
			else {
				//Не тень(Воксельная)
				//*(pa+offR)&=~At_SHADOWV;
				sR=VR;
				dsR=(float)(zl-VR)/(float)(i);
			}

			(*b_s)[BUF_CENTR -j]=sL;
			(*b_ds)[BUF_CENTR -j]=dsL;
			(*b_s)[BUF_CENTR +j]=sR;
			(*b_ds)[BUF_CENTR +j]=dsR;
		}
		d_bs=(float)i/(i+1);
	}

}

//inline int void s_renderV::getIntensityShadow(int x, int y)
void s_renderV::prepareGaussFilter(int Radius)
{
	double rad=(double)Radius/10;
	if(rad > MAX_GAUSS_RADIUS) rad=MAX_GAUSS_RADIUS;
	radius_gb=round(rad);
	diametr_gb=radius_gb*2;
	int y,x;
	int norma=0,f;
	for(y = -radius_gb;y < radius_gb;y++){
		for(x = -radius_gb;x < radius_gb;x++){
			f = round(MAX_VALUE_GB*exp(-(sqr((double)x) + sqr((double)y))/rad));
			norma += f;
			gaussBuf[radius_gb+x+(radius_gb+y)*diametr_gb] = f;
			}
	}
	for(y = -radius_gb;y < radius_gb;y++){
		for(x = -radius_gb;x < radius_gb;x++){
			gaussBuf[radius_gb+x+(radius_gb+y)*diametr_gb] = (unsigned char)((int)gaussBuf[radius_gb+x+(radius_gb+y)*diametr_gb]*MAX_VALUE_GB/norma);
			}
	}

}

#endif // if defined(_SURMAP_)

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/*

#include "land.h"

s_renderV renderV;

void s_renderV::init(void)
{
	for(int i=0; i<TERRAIN_MAX; i++){
		ref_n[i]=8; ref_k[i]=50;
	}
}

void s_renderV::main(int num_track)
{
	//Инициализация (потом нужно заменить на загрузку данных)
	init();

	//Открытие Скрипта скрипта
	XBuffer buft;
	char tmpstr[10];
	buft < "SLightR" < itoa(num_track%10,tmpstr,10) < ".sst";
	if( sur_scr.load_scr(GetTargetName(buft))==0 ) return;
	sur_scr.load_data();

	int i,j;

	vidBufRI=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufGI=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufBI= new unsigned char [1][YS_Buf][XS_Buf];
	vidBufRO=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufGO=new unsigned char [1][YS_Buf][XS_Buf];
	vidBufBO= new unsigned char [1][YS_Buf][XS_Buf];
	N_X=new char [1][YS_Buf][XS_Buf];
	N_Y=new char [1][YS_Buf][XS_Buf];
	N_Z=new char [1][YS_Buf][XS_Buf];

	shadowBuf=vMap -> ClTrBuf;
	vBuf=vMap -> VxBuf;

	TGAHEAD thead;
	thead.load3layers(GetTargetName("input.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufRI),(unsigned char*)(*vidBufGI),(unsigned char*)(*vidBufBI) );


	//Создание нормалей вокселей
	for(i=0; i<V_SIZE; i++){ //Может быть оптимизировано
		for(j=0; j<H_SIZE; j++){
			unsigned short Z0,Z1,Z2; //Z0-главный, Z1 - правый, Z2 -верхний
			unsigned char (*tV)[YS_Buf][XS_Buf]=vMap -> VxBuf;
			unsigned char (*tVL)[YS_Buf][XS_Buf]=vMap -> SpecBuf;
			Z0 =(tV[0][i][j]<<8) | tVL[0][i][j];
			Z1 =(tV[0][i][XCYCL(j+1)]<<8) | tVL[0][i][XCYCL(j+1)];
			Z2 =(tV[0][YCYCL(i-1)][j]<<8) | tVL[0][YCYCL(i-1)][j];
			int A=(Z1-Z0),B=(Z0-Z2), C=-1<<8; 
			int ModV=round(sqrt( A*A + B*B + C*C ));
			N_X[0][i][j]=(A<<8)/ModV>>2;
			N_Y[0][i][j]=(B<<8)/ModV>>2; 
			N_Z[0][i][j]=(C<<8)/ModV>>2;

			//очистка
			shadowBuf[0][i][j]=0;
			(*vidBufRO)[i][j]=0;
			(*vidBufGO)[i][j]=0;
			(*vidBufBO)[i][j]=0;
			vMap -> AtBuf[0][i][j]&=~At_SHADOW;
		}
	}

	calculateCOSnG();
	float nxVis=1/sqrt(3), nyVis=1/sqrt(3), nzVis=1/sqrt(3);
	//float nxVis=1, nyVis=0, nzVis=0;

	for(i=0; i<sur_scr.numbers_cell; i++){
		int xL=1024,yL=1024,zL=1512;
		int rL=150,gL=150,bL=150;
		//int Lambient=30;
		int k_light,Radius,k_ambient;


		sur_scr.row[0].get_el(i,xL);
		sur_scr.row[1].get_el(i,yL);
		sur_scr.row[2].get_el(i,zL);
		sur_scr.row[3].get_el(i,rL);
		sur_scr.row[4].get_el(i,gL);
		sur_scr.row[5].get_el(i,bL);
		sur_scr.row[6].get_el(i,k_light);
		rL=rL*k_light/100; if (rL>255) rL=255;
		gL=gL*k_light/100; if (gL>255) gL=255;
		bL=bL*k_light/100; if (bL>255) bL=255;
		sur_scr.row[7].get_el(i,Radius);
		sur_scr.row[8].get_el(i,k_ambient);


		calculateLightOnDistance(Radius); //Расчет яркости в зависимости от расстояния

		//float kl=1,kml=0.001; //kml - коэффициент масштабирования по расстоянию затухание источника
		double k_zat=1;//0.999;//0.2
		//int begL=255;
		float L,Ldif,Lspec;
		int l;


		//непосредственно рендер
		int x,y;
		int dx,dy,dz;
		for( y=0; y<V_SIZE; y++){
			dy=calcDY(yL,y);//yL-y;
			for( x=0; x<H_SIZE; x++){
				dx=calcDX(xL,x);//xL-x;
				dz=vBuf[0][y][x]-zL;
				l=round(sqrt(dx*dx+dy*dy+dz*dz));
				//L=(float)begL/((float)l*kml+kl); // Яркость источника в данной точке
				//L=begL*(pow(k_zat,l>>2));
				//L=begL-k_zat*(float)l; if(L<0)L=0;
				L=tableLightOnDistance[l];
				float nxL=(float)dx/l,nyL=(float)dy/l,nzL=(float)dz/l;
				float cosA=((int)N_X[0][y][x]*nxL + (int)N_Y[0][y][x]*nyL +(int)N_Z[0][y][x]*nzL)/64;// на 64 умножена нормаль вокселей (<<8 и >>2)
				if (cosA<0) cosA=0;
				Ldif=L*cosA; if(Ldif>256)Ldif=256; //+Lambient
				float cosLV=nxL*nxVis + nyL*nyVis + nzL*nzVis;
				float cosB=( (int)N_X[0][y][x]*nxVis +(int)N_Y[0][y][x]*nyVis +(int)N_Z[0][y][x]*nzVis)/64;
				int cosG=(cosLV-2*cosA*cosB)*255;
				if (cosG > 255) cosG=255; if(cosG < 0)cosG=0;
				Lspec=L*COSnG[GetTer(x,y)][256+cosG];
				int ro,go,bo;
				if(shadowBuf[0][y][x]==0){ //нет тени
					ro=vidBufRI[0][y][x]*Ldif/256 + (rL*Lspec/(256*256));
					go=vidBufGI[0][y][x]*Ldif/256 + (gL*Lspec/(256*256));
					bo=vidBufBI[0][y][x]*Ldif/256 + (bL*Lspec/(256*256));
				}
				else { //есть тень
					ro=(vidBufRI[0][y][x]*Ldif/256)*k_ambient/100;
					go=(vidBufGI[0][y][x]*Ldif/256)*k_ambient/100;
					bo=(vidBufBI[0][y][x]*Ldif/256)*k_ambient/100;
				}
				if(ro>255)ro=255;if(go>255)go=255;if(bo>255)bo=255;
				vidBufRO[0][y][x]=ro;
				vidBufGO[0][y][x]=go;
				vidBufBO[0][y][x]=bo;

			}
		}





	}

	thead.save3layers(GetTargetName("track0.tga"),H_SIZE,V_SIZE,(unsigned char*)(*vidBufRO),(unsigned char*)(*vidBufGO),(unsigned char*)(*vidBufBO) );

	//Конверт из Try в High Color для показа на экране
	for(i=0; i<YS_Buf; i++){ 
		for(j=0; j<XS_Buf; j++){
			vMap->ClBuf[0][i][j]=((((*vidBufRO)[i][j]>>3)&0x1f)<<11)+((((*vidBufGO)[i][j]>>2)&0x3f)<<5)+(((*vidBufBO)[i][j]>>3)&0x1f);
		}
	}





	delete [] vidBufRI;
	delete [] vidBufGI;
	delete [] vidBufBI;
	delete [] vidBufRO;
	delete [] vidBufGO;
	delete [] vidBufBO;
	delete [] N_X;
	delete [] N_Y;
	delete [] N_Z;


}

void s_renderV::calculateCOSnG(void)
{
	int i,j;
	for(i=0; i<TERRAIN_MAX; i++){
		for(j=-256; j<256; j++){
			double m=pow((double)j/256,ref_n[i])* ((double)ref_k[i]/100);
			COSnG[i][256+j]=round(m*256);
		}
	}
}

void s_renderV::calculateLightOnDistance(int distance)
{
		//L=begL*(pow(k_zat,l>>2));
	int i;
	double k=1.0/(double)distance;
	for(i=0; i<MAX_DISTANCE; i++){
		double t=sqrt(1*1 - k*i);
		tableLightOnDistance[i]=MAX_LIGHT*t;
		//tableLightOnDistance[i]=MAX_LIGHT-(k*i)*MAX_LIGHT;
		if(tableLightOnDistance[i]<0) tableLightOnDistance[i]=0;
	}
}

*/