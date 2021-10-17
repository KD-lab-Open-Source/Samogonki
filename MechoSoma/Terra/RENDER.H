#if _MSC_VER == 1100 /* if MSVisual C++ 5.0 */
#ifndef __forceinline 
#define __forceinline inline
#endif //__forceinline 
#endif //_MSC_VER

//@caiiiycuk can't find defined of SHIFT_FRACTION_VX
#define SHIFT_FRACTION_VX 6
#define VX_FRACTION 6 // from LAND.CPP
#define MAX_VX_HEIGHT 255 // from LAND>CPP

const int CLR_MAX_SIDE = 255;
const int CLR_MAX = 2*CLR_MAX_SIDE + 1;
//extern unsigned char* lightCLR[TERRAIN_MAX];
//extern unsigned char palCLR[TERRAIN_MAX][2*256];

struct s_terra{
/*	unsigned char ambient_light[2][16];
	float d_x [2][16];
	float d_y [2][16];
	unsigned char light_front[2][16][512*3];
	unsigned int light_sideways[2][16][512*3];*/
	unsigned char ambient_light[2][16];
	float d_x [2][16];
	float d_y [2][16];
	unsigned char light_front[2][16][(1<<16)];
	unsigned char light_sideways[2][16][(1<<16)];
};
extern s_terra terra;


#define DX(x,d) 	(XCYCL((x) + (d)))
#define DY(y,d) 	(YCYCL((y) + (d)))


inline unsigned char GetTer(unsigned char attribute)
{
#ifdef _SURMAP_
	return (attribute & Tr_MASK);
#else
	return (attribute & Tr_MASK);
#endif
}

inline unsigned char GetTer(int x,int y)
{
#ifdef _SURMAP_
	return ((vMap -> AtBuf[0][y][x]) & Tr_MASK);
#else
	return (vMap -> AtBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)])& Tr_MASK;
#endif
}

inline void SetTer(int x,int y, unsigned char newtype)
{
#ifdef _SURMAP_
	vMap -> AtBuf[0][y][x] = ((vMap -> AtBuf[0][y][x]) & (~Tr_MASK))|newtype;
#else
#endif
}
inline unsigned char GetAt(int x,int y)
{
#ifdef _SURMAP_
	return vMap -> AtBuf[0][y][x];
#else
	return (vMap -> AtBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)]);
#endif
}

inline void SetAt(int x,int y, unsigned char newat)
{
#ifdef _SURMAP_
	vMap -> AtBuf[0][y][x] = (vMap -> AtBuf[0][y][x]) | newat;
#else
#endif
}

inline void MovAt(int x,int y, unsigned char newat)
{
#ifdef _SURMAP_
	vMap -> AtBuf[0][y][x] = newat;
#else
#endif
}

inline int GetW(int x,int y)
{
#ifdef _SURMAP_
	return (int)(vMap -> AtBuf[0][y][x] & At_WATER);
#else
	return (vMap -> AtBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)])&At_WATER;
#endif
}

inline int GetWH(int x,int y)
{
#ifdef _SURMAP_
	if(vMap -> AtBuf[0][y][x] & At_WATER) return vMap->LevelH2O;
	else return 0;
#else
	if((vMap -> AtBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)])&At_WATER)return vMap->LevelH2O;
	return 0;
#endif
}

inline void SetW(int x,int y)
{
#ifdef _SURMAP_
	vMap -> AtBuf[0][y][x] |= At_WATER;
#else
#endif
}

inline void SetNW(int x,int y)
{
#ifdef _SURMAP_
	vMap -> AtBuf[0][y][x] &= ~At_WATER;
#else
#endif
}


#ifdef _SURMAP_
inline unsigned short GetAlt(int x,int y)
{
	return (vMap -> VxBuf[0][y][x]);
}
#else
__forceinline unsigned short GetAlt(int x,int y)
{
//	if(!(vMap->fl_kmVxBuf)) return (vMap -> VxBuf[0][y][x]);
//	else 
	return (vMap -> VxBuf[0][0][((y>>kmBuf)<<(H_SIZE_POWER-kmBuf))+ (x>>kmBuf)])<<(8-SHIFT_FRACTION_VX);
}
#endif



inline void SetAlt(int x,int y,int v)
{
#ifdef _SURMAP_
	vMap -> VxBuf[0][y][x]=v;
#else
#endif
}

inline unsigned short GetAltC(int x,int y){ return GetAlt(XCYCL(x), YCYCL(y)); }

void regRender(int x0,int y0,int x1,int y1,int changed = 1);
void regDownRender(int x0,int y0,int x1,int y1,int changed = 1);
void pixSet(int x,int y,int delta,int terrain=-1);
//void pixSetFDZ(int x,int y,int delta,int surf = 1);
void pixSetR(int x,int y,int delta,int surf = 1);
void pixDownSet(int x,int y,int delta,int surf = 1);
void regSet(int x0,int y0,int x1,int y1,int dry,int surf = 1);
void RenderStr(int Y);
void RenderRegStr(int Yh,int Yd);
void RenderShadovM3D(int number, float y);
//void regRenderSNGC(int LowX,int LowY,int HiX,int HiY);
void WorldRender(void);
void renderHiBmp(void);

