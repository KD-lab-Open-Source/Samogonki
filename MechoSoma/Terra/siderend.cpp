#include "StdAfx.h"

//#include "common.h"
#include "terra.h"
//#include "vmap.h"
//#include "world.h"
//#include "render.h"


//**************** Extern section **********************
extern int delta_s;
//extern unsigned char light_G[2][512*3];
//******************************************************

//#define MEMSET(a,b,c) memset(a,b,c)

//typedef unsigned char BYTE;

//const int POSPOWER = 8;
//const int POSVAL = 1 << POSPOWER;
//const int SHADOWHEIGHT = 32;

//extern BYTE* shadowParent;
//extern BYTE FloodLEVEL;
//extern int RenderingLayer;

/*void regDownRender(int LowX,int LowY,int HiX,int HiY,int changed);

inline void PreStage(int& LastStep,int HiX,BYTE* pa0,int& hC,BYTE* pmask)
{
	int MaskShift = LastStep >> POSPOWER;
	int x = XCYCL(HiX + 1);
	BYTE* pa = pa0 + x;
	BYTE* pf = pa + H_SIZE;

	while(hC - MaskShift < MAX_ALT){
		if(!x){
			pa = pa0;
			pf = pa + H_SIZE;
			}
		if(*pf & DOUBLE_LEVEL){
			const int dh = *pa + MaskShift;

			LastStep -= SHADOWDEEP;
			MaskShift = LastStep >> POSPOWER;

			pa++;
			const int h = *pa + MaskShift;
			if(hC < dh){
				MEMSET(pmask + hC,1,dh - hC);
				hC = dh;
				}
			MEMSET(pmask + h - SS_WIDTH,1,SS_WIDTH + 1);
			pa++;
			}
		else {
			int h = *pa + MaskShift;
			if(hC < h){
				MEMSET(pmask + hC,1,h - hC);
				hC = h;
				}
			LastStep -= SHADOWDEEP;
			MaskShift = LastStep >> POSPOWER;

			pa++;
			h = *pa + MaskShift;
			if(hC < h){
				MEMSET(pmask + hC,1,h - hC);
				hC = h;
				}
			pa++;
			}
          	LastStep -= SHADOWDEEP;
		MaskShift = LastStep >> POSPOWER;

		x = XCYCL(x + 2);
		pf += 2;
		}
	hC -= MAX_ALT;
}

inline void MainStage(BYTE* pa0,int& hC,BYTE* pc0,int SizeX,int& x,BYTE*& grid,int& MaxAlt)
{
	BYTE* pa = pa0 + x;
	BYTE* pf = pa + H_SIZE;
	BYTE* pc = pc0 + x;

	BYTE* palCLRlast;
	BYTE* lightCLRlast;
	BYTE typeC = 0xFF;
	register int i;
	for(i = 0;i < SizeX;i += 2){
		if(*pf & DOUBLE_LEVEL){
			const BYTE lxVal = *pa;
			const BYTE rxVal = *(pa0 + XCYCL(x + 2));
			const BYTE h = *pa;
			const BYTE dh = *(pa - 1);
			const BYTE type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
			const BYTE level = type ? h : FloodLEVEL;
#else
			const BYTE level = h;
#endif
			if(type != typeC){
				typeC = type;
				palCLRlast = palCLR[type];
				lightCLRlast = lightCLR[type];
				}

			grid += 3;
			hC -= 3;
			MaxAlt -= 3;

			if(*(grid + level)){
				*pf |= SHADOW_MASK;
				*(pf - 1) |= SHADOW_MASK;
				*(pc - 1) = *pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
				}
			else {
				*pf &= ~SHADOW_MASK;
				*(pf - 1) &= ~SHADOW_MASK;
				*(pc - 1) = *pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
				}
#ifndef TERRAIN16
			if(*(grid + SHADOWHEIGHT + level)){
				*pf |= OBJSHADOW;
				*(pf - 1) |= OBJSHADOW;
				}
			else {
				*pf &= ~OBJSHADOW;
				*(pf - 1) &= ~OBJSHADOW;
				}
#endif
			pa--; pf--; pc--;
			if(dh > hC){
				MEMSET(grid + hC,1,dh - hC);
				hC = dh;
				}
			MEMSET(grid + h - SS_WIDTH,1,SS_WIDTH + 1);
			if(h > MaxAlt) MaxAlt = h;
			}
		else {
			BYTE lxVal = *(pa - 1);
			BYTE rxVal = *(pa0 + XCYCL(x + 1));
			BYTE h = *pa;
			BYTE type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
			BYTE level = type ? h : FloodLEVEL;
#else
			BYTE level = h;
#endif
			grid += 1;
			hC -= 1;
			MaxAlt -= 1;
			if(type != typeC){
				typeC = type;
				palCLRlast = palCLR[type];
				lightCLRlast = lightCLR[type];
				}

			if(*(grid + level)){
				*pf |= SHADOW_MASK;
				*pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
				}
			else {
				*pf &= ~SHADOW_MASK;
				*pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
				}
#ifndef TERRAIN16
			if(*(grid + SHADOWHEIGHT + level))
				*pf |= OBJSHADOW;
			else
				*pf &= ~OBJSHADOW;
#endif
			if(h > hC){
				MEMSET(grid + hC,1,h - hC);
				hC = h;
				}
			if(h > MaxAlt) MaxAlt = h;
			
			pa--; pf--; pc--;
			rxVal = h;
			h = lxVal;
			lxVal = *(pa0 + XCYCL(x - 2));
			grid += 2;
			hC -= 2;
			MaxAlt -= 2;
			type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
			level = type ? h : FloodLEVEL;
#else
			level = h;
#endif			
			if(type != typeC){
				typeC = type;
				palCLRlast = palCLR[type];
				lightCLRlast = lightCLR[type];
				}

			if(*(grid + level)){
				*pf |= SHADOW_MASK;
				*pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
				}
			else {
				*pf &= ~SHADOW_MASK;
				*pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
				}
#ifndef TERRAIN16
			if(*(grid + SHADOWHEIGHT + level))
				*pf |= OBJSHADOW;
			else
				*pf &= ~OBJSHADOW;
#endif
			if(h > hC){
				MEMSET(grid + hC,1,h - hC);
				hC = h;
				}
			if(h > MaxAlt)
				MaxAlt = h;
			}
		if(x == 1){
			x = H_SIZE - 1;
			pa = pa0 + x;
			pf = pa + H_SIZE;
			pc = pc0 + x;
			}
		else {
			x -= 2;
			pa--; pf--; pc--;
			}
		}
}
*/
void regRender(int LowX,int LowY,int HiX,int HiY,int changed)
{
/*#ifdef _SURMAP_
	if(RenderingLayer == DOWN_LAYER){
		regDownRender(LowX,LowY,HiX,HiY,changed);
		return;
		}
#endif
*/

	LowX = XCYCL(LowX);
	HiX = XCYCL(HiX);
	LowY = YCYCL(LowY);
	HiY = YCYCL(HiY);

	LowX &= ~1;
	HiX |= 1;

//	int SizeY = (LowY == HiY) ? V_SIZE : YCYCL(HiY - LowY);
	int SizeY = (LowY == HiY) ? V_SIZE : YCYCL(HiY - LowY);
	int SizeX = (0 == XCYCL(HiX - LowX)) ? H_SIZE : XCYCL(HiX - LowX);

//	int BackScanLen = 0;
	register int j;
	for(j = 0;j <= SizeY;j++){
		const int y = YCYCL(j + LowY);
		if(changed) vMap -> changedT[y] = 1;
		RenderStr(y);
	}
}
/*
		BYTE* pa0 = vMap -> lineT[y];
		BYTE* pc0 = vMap -> lineTcolor[y];
		if(!pa0) return;
		if(changed) vMap -> changedT[y] = 1;
		MEMSET(shadowParent,0,(((SizeX*SHADOWDEEP) >> POSPOWER) + 4*MAX_ALT));
//		MEMSET(shadowParent,0,H_SIZE);
		int hC = MAX_ALT;
		int LastStep = (H_SIZE - 1 - HiX) * SHADOWDEEP;
		LastStep -= ((LastStep >> POSPOWER) - MAX_ALT) << POSPOWER;

		//препроход
		PreStage(LastStep,HiX,pa0,hC,shadowParent + MAX_ALT);
		//основной проход
		int x = HiX;
		BYTE* grid = shadowParent + MAX_ALT + MAX_ALT;
		int MaxAlt = 0;
		MainStage(pa0,hC,pc0,SizeX,x,grid,MaxAlt);
		
		//пост проход
		{
		x |= 1;
		BYTE* pa = pa0 + x;
		BYTE* pf = pa + H_SIZE;
		BYTE* pc = pc0 + x;
		BYTE* palCLRlast;
		BYTE* lightCLRlast;
		BYTE typeC = 0xFF;
		int MaxPossibleAlt = MAX_ALT;
		unsigned bNeedScan = 1;
		while(bNeedScan && MaxPossibleAlt >= 0){
			bNeedScan = 0;
			if(*pf & DOUBLE_LEVEL){
				const BYTE lxVal = *pa;
				const BYTE rxVal = *(pa0 + XCYCL(x + 2));
				const BYTE h = *pa;
				const BYTE dh = *(pa - 1);
				const BYTE type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
				const BYTE level = type ? h : FloodLEVEL;
#else
				const BYTE level = h;
#endif
				if(type != typeC){
					typeC = type;
					palCLRlast = palCLR[type];
					lightCLRlast = lightCLR[type];
					}

				grid += 3;
				hC -= 3;
				MaxAlt -= 3;
				MaxPossibleAlt -= 3;
				if(dh < MaxAlt || (*pf & SHADOW_MASK)) bNeedScan = 1;
				if(*(grid + level)){
					*pf |= SHADOW_MASK;
					*(pf - 1) |= SHADOW_MASK;
					*(pc - 1) = *pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
					}
				else {
					*pf &= ~SHADOW_MASK;
					*(pf - 1) &= ~SHADOW_MASK;
					*(pc - 1) = *pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
					}
#ifndef TERRAIN16
				if(*(grid + SHADOWHEIGHT + level)){
					*pf |= OBJSHADOW;
					*(pf - 1) |= OBJSHADOW;
					}
				else {
					*pf &= ~OBJSHADOW;
					*(pf - 1) &= ~OBJSHADOW;
					}
#endif
				pa--; pf--; pc--;
				if(*pf & SHADOW_MASK) bNeedScan = 1;
				if(dh > hC){
					MEMSET(grid + hC,1,dh - hC);
					hC = dh;
					}
				MEMSET(grid + h - SS_WIDTH,1,SS_WIDTH + 1);
				}
			else {
				BYTE lxVal = *(pa - 1);
				BYTE rxVal = *(pa0 + XCYCL(x + 1));
				BYTE h = *pa;
				BYTE type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
				BYTE level = type ? h : FloodLEVEL;
#else
				BYTE level = h;
#endif
				grid += 1;
				hC -= 1;
				MaxAlt -= 1;
				MaxPossibleAlt -= 1;
				if(type != typeC){
					typeC = type;
					palCLRlast = palCLR[type];
					lightCLRlast = lightCLR[type];
					}

				if(h <= MaxAlt || (*pf & SHADOW_MASK)) bNeedScan = 1;
				if(*(grid + level)){
					*pf |= SHADOW_MASK;
					*pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
					}
				else {
					*pf &= ~SHADOW_MASK;
					*pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
					}
#ifndef TERRAIN16
				if(*(grid + SHADOWHEIGHT + level))
					*pf |= OBJSHADOW;
				else
					*pf &= ~OBJSHADOW;
#endif
				if(h > hC){
					MEMSET(grid + hC,1,h - hC);
					hC = h;
					}
				
				pa--; pf--; pc--;
				rxVal = h;
				h = lxVal;
				lxVal = *(pa0 + XCYCL(x - 2));
				grid += 2;
				hC -= 2;
				MaxAlt -= 2;
				MaxPossibleAlt -= 2;
				type = (*pf & TERRAIN_MASK) >> TERRAIN_OFFSET;
#ifndef TERRAIN16
				level = type ? h : FloodLEVEL;
#else
				level = h;
#endif
				if(type != typeC){
					typeC = type;
					palCLRlast = palCLR[type];
					lightCLRlast = lightCLR[type];
					}

				if(h <= MaxAlt || (*pf & SHADOW_MASK)) bNeedScan = 1;
				if(*(grid + level)){
					*pf |= SHADOW_MASK;
					*pc = palCLRlast[256 + (lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION) >> 1)];
					}
				else {
					*pf &= ~SHADOW_MASK;
					*pc = palCLRlast[256 + lightCLRlast[255 - (lxVal - rxVal)] - (255 - h >> H_CORRECTION)];
					}
#ifndef TERRAIN16
				if(*(grid + SHADOWHEIGHT + level))
					*pf |= OBJSHADOW;
				else
					*pf &= ~OBJSHADOW;
#endif
				if(h > hC){
					MEMSET(grid + hC,1,h - hC);
					hC = h;
					}
				}
			if(x == 1){
				x = H_SIZE - 1;
				pa = pa0 + x;
				pf = pa + H_SIZE;
				pc = pc0 + x;
				}
			else {
				x = XCYCL(x - 2);
				pa--; pf--; pc--;
				}
			}
		const int CurrScanLenght = XCYCL(LowX - x);
		if(CurrScanLenght > BackScanLen) BackScanLen = CurrScanLenght;
		}
		}
}
*/
/*
void regRenderSNGC(int LowX,int LowY,int HiX,int HiY)//regRenderSNGC
{

	LowX = XCYCL(LowX-2);
	HiX = XCYCL(HiX+2);
	LowY = YCYCL(LowY-2);
	HiY = YCYCL(HiY+2);

//	LowX &= ~1;
//	HiX |= 1;

//	int SizeY = (LowY == HiY) ? V_SIZE : YCYCL(HiY - LowY);
	int SizeY = (LowY == HiY) ? V_SIZE : YCYCL(HiY - LowY);
	int SizeX = (0 == XCYCL(HiX - LowX)) ? H_SIZE : XCYCL(HiX - LowX);

//	int BackScanLen = 0;
	unsigned char *pv0,*pv1,*pv2,*pv3;
	unsigned char *pa0,*pa1,*pa2,*pa3;
	unsigned char *pc1,*pc2;
	unsigned char *ps0,*ps1,*ps2,*ps3;
	unsigned char lxV0,lxV1,lxV2,lxV3,rxV0,rxV1,rxV2,rxV3,V0,V1,V2,V3;
	for(int j = 0;j <= SizeY;j+=2){
		int y = YCYCL(j + LowY);
		pv0 = &(vMap->VxBuf[0][y][0]);
		pv1 = &(vMap->VxBuf[0][YCYCL(y+1)][0]);
		pv2 = &(vMap->VxBuf[0][YCYCL(y+2)][0]);
		pv3 = &(vMap->VxBuf[0][YCYCL(y+3)][0]);
		pa0 = &(vMap->AtBuf[0][y][0]);
		pa1 = &(vMap->AtBuf[0][YCYCL(y+1)][0]);
		pa2 = &(vMap->AtBuf[0][YCYCL(y+2)][0]);
		pa3 = &(vMap->AtBuf[0][YCYCL(y+3)][0]);
		pc1 = &(vMap->ClTrBuf[0][YCYCL(y+1)][0]);
		pc2 = &(vMap->ClTrBuf[0][YCYCL(y+2)][0]);
		ps0 = &(vMap->SpecBuf[0][y][0]);
		ps1 = &(vMap->SpecBuf[0][YCYCL(y+1)][0]);
		ps2 = &(vMap->SpecBuf[0][YCYCL(y+2)][0]);
		ps3 = &(vMap->SpecBuf[0][YCYCL(y+3)][0]);
		int x=HiX;
		if(*(pa0+x)&At_WATER) rxV0=*(ps0+x);
		else rxV0=*(pv0+x);
		if(*(pa1+x)&At_WATER) rxV1=*(ps1+x);
		else rxV1=*(pv1+x);
		if(*(pa2+x)&At_WATER) rxV2=*(ps2+x);
		else rxV2=*(pv2+x);
		if(*(pa3+x)&At_WATER) rxV3=*(ps3+x);
		else rxV3=*(pv3+x);
		x=XCYCL(HiX-1);
		if(*(pa0+x)&At_WATER) V0=*(ps0+x);
		else V0=*(pv0+x);
		if(*(pa1+x)&At_WATER) V1=*(ps1+x);
		else V1=*(pv1+x);
		if(*(pa2+x)&At_WATER) V2=*(ps2+x);
		else V2=*(pv2+x);
		if(*(pa3+x)&At_WATER) V3=*(ps3+x);
		else V3=*(pv3+x);
		int h_s0=0,h_s1=0,h_s2=0,h_s3=0;
		do {
			x=XCYCL(x-1);
			if(h_s0 > delta_s) h_s0-=delta_s;
			else h_s0=0;
			if(h_s1 > delta_s) h_s1-=delta_s;
			else h_s1=0;
			if(h_s0 > delta_s) h_s2-=delta_s;
			else h_s2=0;
			if(h_s3 > delta_s) h_s3-=delta_s;
			else h_s3=0;
			//Выборка вокселей
			if(*(pa0+x)&At_WATER) lxV0=*(ps0+x);
			else lxV0=*(pv0+x);
			if(*(pa1+x)&At_WATER) lxV1=*(ps1+x);
			else lxV1=*(pv1+x);
			if(*(pa2+x)&At_WATER) lxV2=*(ps2+x);
			else lxV2=*(pv2+x);
			if(*(pa3+x)&At_WATER) lxV3=*(ps3+x);
			else lxV3=*(pv3+x);
			x=XCYCL(x+1);
			int temp1=256*3+lxV0+lxV1+lxV2-rxV0-rxV1-rxV2;
			int temp1y=256*3+lxV0+V0+rxV0-lxV2-V2-rxV2;
			int temp2=256*3+lxV1+lxV2+lxV3-rxV1-rxV2-rxV3;
			int temp2y=256*3+lxV1+V1+rxV1-lxV3-V3-rxV3;
			unsigned char type1=*(pa1+x) & Tr_MASK;
			//H2O 1tchk
			if(*(pa1+x)&At_WATER){
				//*(pc1+x)=light_G[1][temp1]-(127-(V1>>1));
				*(pc1+x)=((terra.light_front[1][type1][temp1] * terra.light_sideways[1][type1][temp1y] >>8) +terra.ambient_light[1][type1]);
				if((h_s1 >(*(pv1+x)<<16)) || (*(pa1+x)&At_SHADOW )){
					if( ((h_s2 >(*(pv2+x)<<16))&&(h_s0 >(*(pv0+x)<<16))&&((h_s1-delta_s)>(*(pv1+x)+1<<16))) || (*(pa1+x)&At_SHADOW) ){
						*(pc1+x)>>=1;
						if(*(pc1+x) < (*(pv1+x)-V1>>1)) *(pc1+x)=0;
						else *(pc1+x)-=(*(pv1+x)-V1>>1);
						if((*(pa1+x)&At_SHADOWS) ==0) *(pa1+x)|=At_SHADOWV;
					}
					else {
						*(pc1+x)-=*(pc1+x)>>2;//Ослабленная тень
						if(*(pc1+x) < (*(pv1+x)-V1>>1)) *(pc1+x)=0;
						else *(pc1+x)-=(*(pv1+x)-V1>>1);
					}
				}
				else {
					if(*(pc1+x) < (*(pv1+x)-V1>>1)) *(pc1+x)=0;
					else *(pc1+x)-=(*(pv1+x)-V1>>1);
//					if(h_s0 <= (V0<<16)) h_s0=V0<<16;
				}
			}
			//СУША 1tchk
			else{
				//*(pc1+x)=light_G[0][temp1]-(127-(V1>>1));
				*(pc1+x)=((terra.light_front[0][type1][temp1] * terra.light_sideways[0][type1][temp1y] >>8) +terra.ambient_light[0][type1]);
				if((h_s1 >(V1<<16)) || (*(pa1+x)&At_SHADOW)){
					if( ((h_s2 >(V2<<16))&&(h_s0 >(V0<<16))&&((h_s1-delta_s)>(V1+1<<16))) || (*(pa1+x)&At_SHADOW) ){
						*(pc1+x)>>=1;
						if((*(pa1+x)&At_SHADOWS) ==0) *(pa1+x)|=At_SHADOWV;
					}
					else {
						*(pc1+x)-=*(pc1+x)>>2;//Ослабленная тень
					}
				}
				else{
//					if(h_s0 <= (V0<<16)) h_s0=V0<<16;
				}
			}
//			
			//H2O 2tchk
			unsigned char type2=*(pa2+x) & Tr_MASK;
			if(*(pa2+x)&At_WATER){
				//*(pc2+x)=light_G[1][temp2]-(127-(V2>>1));
				*(pc2+x)=((terra.light_front[1][type2][temp2] * terra.light_sideways[1][type2][temp2y] >>8) +terra.ambient_light[1][type2]);
				if((h_s2 >(*(pv2+x)<<16)) || (*(pa2+x)&At_SHADOW)){
					if( ((h_s3 >(*(pv3+x)<<16))&&(h_s1 >(*(pv1+x)<<16))&&((h_s2-delta_s)>(*(pv2+x)+1<<16))) || (*(pa2+x)&At_SHADOW) ){
						*(pc2+x)>>=1;
						if(*(pc2+x) < (*(pv2+x)-V2>>1)) *(pc2+x)=0;
						else *(pc2+x)-=(*(pv2+x)-V2>>1);
						if((*(pa2+x)&At_SHADOWS) ==0) *(pa2+x)|=At_SHADOWV;
					}
					else {
						*(pc2+x)-=*(pc2+x)>>2;//Ослабленная тень
						if(*(pc2+x) < (*(pv2+x)-V2>>1)) *(pc2+x)=0;
						else *(pc2+x)-=(*(pv2+x)-V2>>1);
					}
				}
				else {
					if(*(pc2+x) < (*(pv2+x)-V2>>1)) *(pc2+x)=0;
					else *(pc2+x)-=(*(pv2+x)-V2>>1);
//					if(h_s1 <= (V1<<16)) h_s1=V1<<16;
//					if(h_s2 <= (V2<<16)) h_s2=V2<<16;
//					if(h_s3 <= (V3<<16)) h_s3=V3<<16;
				}
			}
			//СУША 2tchk
			else{
				//*(pc2+x)=light_G[0][temp2]-(127-(V2>>1));
				*(pc2+x)=((terra.light_front[0][type2][temp2] * terra.light_sideways[0][type2][temp2y] >>8) +terra.ambient_light[0][type2]);
				if((h_s2 >(V2<<16)) || (*(pa2+x)&At_SHADOW)){
					if( ((h_s3 >(V3<<16))&&(h_s1 >(V1<<16))&&((h_s2-delta_s)>(V2+1<<16))) || (*(pa2+x)&At_SHADOW) ){
						*(pc2+x)>>=1;
						if((*(pa2+x)&At_SHADOWS) ==0) *(pa2+x)|=At_SHADOWV;
					}
					else {
						*(pc2+x)-=*(pc2+x)>>2;//Ослабленная тень
					}
				}
				else{
//					if(h_s1 <= (V1<<16)) h_s1=V1<<16;
//					if(h_s2 <= (V2<<16)) h_s2=V2<<16;
//					if(h_s3 <= (V3<<16)) h_s3=V3<<16;
				}
			}
					if(h_s0 <= (V0<<16)) h_s0=V0<<16;
					if(h_s1 <= (V1<<16)) h_s1=V1<<16;
					if(h_s2 <= (V2<<16)) h_s2=V2<<16;
					if(h_s3 <= (V3<<16)) h_s3=V3<<16;

		x=XCYCL(x-1);
		rxV0=V0;rxV1=V1;rxV2=V2;rxV3=V3;
		V0=lxV0;V1=lxV1;V2=lxV2;V3=lxV3;
		}while(x!=LowX);

	}
}*/
//		const int y = YCYCL(j + LowY);
//		vMap -> changedT[y] = 1;
