#ifndef __TERRA_H__
#define __TERRA_H__

//Global Define
#define XCYCL(x)	(int(x) & clip_mask_x)
#define YCYCL(y)	(int(y) & clip_mask_y)

const unsigned int H_SIZE_POWER= 11;
const unsigned int H_SIZE = 1 << H_SIZE_POWER;
const unsigned int clip_mask_x = H_SIZE - 1;
const unsigned int H2_SIZE = 2*H_SIZE;

extern unsigned int V_SIZE_POWER;
extern unsigned int V_SIZE;
extern unsigned int clip_mask_y;

/*#ifdef _SURMAP_
#define ZV(i,j) (vMap->VxBuf[0][j][i])
#define Z(i) (vMap->VxBuf[0][0][i])
#else
#define ZV(i,j) (vMap->VxBuf[0][0][((j>>kmVxBuf)<<(H_SIZE_POWER-kmVxBuf))+(i>>kmVxBuf)])
#define Z(i) (vMap->VxBuf[0][0][(i>>kmVxBuf)>>kmVxBuf ])
#endif*/


#ifdef _SURMAP_
const int MAX_RADIUS = 175;
#else
const int MAX_RADIUS = 48;
#endif

//------Render define------//
#define H_CORRECTION 2
#define At_SHADOW 0xC0
#define At_SHADOWS 0x80
#define At_SHADOWV 0x40
#define At_WATER 0x10
#define Tr_MASK 0x0F
#define TrW_MASK 0x1F
#define SHADOW_CORRECTION 1

//#define WaterLevel 100
//_________________________//
const int TERRAIN_MAX = 16;

//for RenderPrepare
const int SHADOWDEEP	       =  384;		//362;
//for PixSet
extern int CurrentTerrain, ActiveTerrain[TERRAIN_MAX];
extern int MiniLevel,MaxiLevel;
//for Ysetup
extern unsigned char POWER;
extern unsigned char QUANT;
extern unsigned int WPART_POWER;
extern unsigned int part_map_size_y;
extern unsigned int part_map_size;
extern unsigned int net_size;
extern unsigned int PART_MAX;
extern int MINSQUARE;

void buildWorld(void);
void WORLD_colcalc(int y0,int y1,int isColor = 1);
//void DOWNWORLD_colcalc(int y0,int y1);
void worldPrepare(void);
void renderHiBmp(void);

//for vrtMap::init
void LoadVPR(int ind = 0);
void SaveVPR(int ind = 0);
void LoadPP(void);

extern int MESH;

void landPrepare(void);
//for world.cpp
const unsigned char WATER_TERRAIN_INDEX = 0;
const unsigned char MAIN_TERRAIN_INDEX	= 1;
const unsigned char WATER_TERRAIN = WATER_TERRAIN_INDEX;// << TERRAIN_OFFSET;
const unsigned char MAIN_TERRAIN = MAIN_TERRAIN_INDEX;// << TERRAIN_OFFSET;
const unsigned char MIN_ALT = 0;
const unsigned char MAX_ALT = 255;
const int WORLD_TYPE	       =  3;
#define COLOR_MAP(y,x)	  (*(color_map + ((y) << H_SIZE_POWER) + (x)))
#define ALT_MAP(y,x)	  (*(alt_map   + ((y) << H_SIZE_POWER) + (x)))
#define PTR_ALT_MAP(y,x)  (alt_map   + ((y) << H_SIZE_POWER) + (x))
#define TEMP_MAP(y,x)	  (*(temp_map  + ((y) << H_SIZE_POWER) + (x)))
#define R_NET_MAP(y,x)	  r_net_map[((y) << (H_SIZE_POWER - POWER)) + (x)]
#define M_NET_MAP(y,x)	  m_net_map[((y) << (H_SIZE_POWER - POWER)) + (x)]
const int BIZARRE_ROUGHNESS_MAP = 0;
const int BIZARRE_ALT_MAP	= 0;
#define SET_DELTA(a,b)	{ a &= ~DELTA_MASK, a += b; }
const int NOISELEVEL	       = 256;
const unsigned char DELTA_MASK = 1 | (1 << 1);

#include "VMAP.H"
#include "RENDER.H"
#include "ch_points.h"
#include "sur_scr.h"
#include "tools.h"

#ifdef _SURMAP_
inline unsigned char& ZV(int i,int j) { return vMap->VxBuf[0][j][i]; }
inline unsigned char& Z(int i) { return vMap->VxBuf[0][0][i]; }
#else
inline unsigned short ZV(int x,int y)
{
//	if(!(vMap->fl_kmVxBuf)) return (vMap -> VxBuf[0][y][x]);
//	else 
	return (vMap -> VxBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)])<<(8-SHIFT_FRACTION_VX);
}
inline unsigned char& Z(int i) { return vMap->VxBuf[0][0][(i>>kmVxBuf)>>kmVxBuf ]; }
#endif

#endif // __TERRA_H__
