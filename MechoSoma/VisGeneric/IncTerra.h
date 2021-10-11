#ifndef __INCTERRA_H__
#define __INCTERRA_H__

#ifdef _USE_TERRA_

#include "BaseDefine.h"
#include "terra.h"

#define TERRA_XCYCL(a)								XCYCL(a)
#define TERRA_YCYCL(a)								YCYCL(a)
#define TERRA_H_SIZE_POWER							H_SIZE_POWER
#define TERRA_kmVxBuf								kmBuf
#define TERRA_At_SHADOW								At_SHADOW
#define TERRA_At_SHADOWS							At_SHADOWS
#define TERRA_At_SHADOWV							At_SHADOWV
#define TERRA_At_WATER								At_WATER
#define TERRA_Tr_MASK								Tr_MASK
#define TERRA_TrW_MASK								TrW_MASK
#define TERRA_At_ICE								15
#define TERRA_SHIFT_VX								(8-SHIFT_FRACTION_VX)

inline int vMap_IsActive()							{ return vMap!=0; }
inline void vMap_Prepare(char *WorldPrm,int world)	{ vMapPrepare(WorldPrm,world); }
inline void	vMap_load3Buf(int x,int y,int track)	{ vMap->load3Buf(0,0,track); }
inline void vMap_loadTrack(int track)				{ vMap->loadTrack(track); }
inline int vMap_LevelH2O()							{ return vMap->LevelH2O; }
inline void vMap_Delete()							{ delete vMap; }
inline void vMap_releaseClBuf()						{ vMap->releaseClBuf(); }
inline unsigned short* vMap_ClBuf()					{ return &vMap->ClBuf[0][0][0]; }
inline unsigned char vMap_GetAt(int x,int y)		{ return GetAt(x,y); }
inline unsigned char vMap_GetAlt(int x,int y)		{ return GetAlt(x,y); }
inline unsigned char* vMap_VxBuf()					{ return (unsigned char*)&vMap->VxBuf[0][0][0]; }
inline unsigned char* vMap_AtBuf()					{ return &vMap->AtBuf[0][0][0]; }
inline char* vMap_GetTargetName(char *fname)		{ return GetTargetName(fname); }
inline int vMap_H2OCR()								{ return vMap->H2OCR; }
inline int vMap_H2OCG()								{ return vMap->H2OCG; }
inline int vMap_H2OCB()								{ return vMap->H2OCB; }
inline int vMap_H2OA()								{ return vMap->H2OA; }

#else

#define TERRA_XCYCL(a)								((a)&2047)
#define TERRA_YCYCL(a)								((a)&2047)
#define TERRA_At_SHADOW								0
#define TERRA_At_SHADOWS							0
#define TERRA_At_SHADOWV							0
#define TERRA_At_WATER								0
#define TERRA_Tr_MASK								0
#define TERRA_TrW_MASK								0
#define TERRA_At_ICE								0
#define TERRA_H_SIZE_POWER							11
#define TERRA_kmVxBuf								0
#define TERRA_SHIFT_VX								0

inline int vMap_IsActive()							{ return 0; }
inline void vMap_Prepare(char *WorldPrm,int world)	{ }
inline void	vMap_load3Buf(int x,int y,int track)	{ }
inline void vMap_loadTrack(int track)				{ }
inline int vMap_LevelH2O()							{ return 0; }
inline void vMap_Delete()							{ }
inline void vMap_releaseClBuf()						{ }
inline unsigned short* vMap_ClBuf()					{ return 0; }
inline unsigned char vMap_GetAt(int x,int y)		{ return 0; }
inline unsigned char vMap_GetAlt(int x,int y)		{ return 0; }
inline unsigned char* vMap_VxBuf()					{ return 0; }
inline unsigned char* vMap_AtBuf()					{ return 0; }
inline char* vMap_GetTargetName(char *fname)		{ return fname; }
inline int vMap_H2OCR()								{ return 0; }
inline int vMap_H2OCG()								{ return 0; }
inline int vMap_H2OCB()								{ return 0; }
inline int vMap_H2OA()								{ return 0; }

#endif //_USE_TERRA_

#endif //__INCTERRA_H__