#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#include "UMATH.H"

class cUnknownClass;
class cSurfaceReflectionMultiMaterial;

extern cSurfaceReflectionMultiMaterial* BaseObject_SurfaceReflection_Create(float xPos,float yPos,float zPos,float xSizeReflection,float ySizeReflection,int xTextureSize,int yTextureSize,float du,float dv,char **TextureName=0,char **OpacityName=0,char *TexturePath=0,int KoofReflection=100,float dPeriod=0.5f);
extern void BaseObject_SurfaceReflection_Pos(Vect3f &Pos,Vect3f &ReflectionPos);
extern void BaseObject_SurfaceReflection_Pos(Vect3f &Pos,float &fWarp);
extern void BaseObject_SurfaceReflection_WaveWarp(float x,float y,float z);
extern void BaseObject_SurfaceReflection_Draw(cUnknownClass *UCamera);

#endif //__GAMECLIENT_H__