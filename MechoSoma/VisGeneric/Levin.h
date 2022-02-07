#ifndef __LEVIN_H__
#define __LEVIN_H__

#include "UMATH.H"
#include "BaseClass.h"

#define BIAS_LEVEL_COLOR								0.2
#define MAX_SIZE_LEVIN									1000

class cUnknownClass;

class cLevin
{
	float		pStop;										// вероятность обрыва ствола
	float		pNascency;									// вероятность порождения
	Vect3f		pAberration;								// вероятность отклонения
	Vect3f		pLevel;										// степень отклонения поржденного ствола
	Vect3f		Pos;										// точка рождения
	Vect3f		Direction;									// направление
	sColor4f	Color;
public:
	int	  size;											// минимальная длина ствола для var=4

	cLevin( float p_Stop=1.0,float p_Nascency=0,
			float pX=0,float pY=0,float pZ=0,float pXLevel=0,float pYLevel=0,float pZLevel=0)		{ SetLevin(p_Stop,p_Nascency,pX,pY,pZ,pXLevel,pYLevel,pZLevel); size=0; }
	~cLevin()																{ }
	void Draw(cUnknownClass *UCameraList,int var=1);
	void SetLevin(float p_Stop,float p_Nascency,
			float pX,float pY,float pXLevel,float pYLevel)					{ pStop=p_Stop; pNascency=p_Nascency; pAberration.set(pX,pY,0); pLevel.set(pXLevel,pYLevel,0); }
	void SetLevin(float p_Stop,float p_Nascency,float pX,float pY,float pZ,
		float pXLevel,float pYLevel,float pZLevel)							{ pStop=p_Stop; pNascency=p_Nascency; pAberration.set(pX,pY,pZ); pLevel.set(pXLevel,pYLevel,pZLevel); }
	void SetPosition(float xPos,float yPos,float zPos,float Vx,float Vy,float Vz=0)	{ Pos.set(xPos,yPos,zPos); Direction.set(Vx,Vy,Vz); }
	void SetColor(float rLevin,float gLevin,float bLevin,float aLevin=1.f)	{ Color.set(rLevin,gLevin,bLevin,aLevin); }
private:
	void GenerationLevin1(const Vect3f &pos,const Vect3f &dpos,const Vect2f &width,int level,int count=0);
	void GenerationLevin4(const Vect3f &pos,const Vect3f &dpos,const Vect2f &width,int length,int level,int count=0);
};

extern void GenerationLevin(float x,float y,float z,float x1,float y1,float z1,float x2,float y2,float z2,
			float r=1.0f,float g=1.0f,float b=1.0f,float p_Stop=0.05f,float p_Nascency=0.05f,float p_X=3.0f,float p_Y=3.0f,float pX_Level=0.2f,float pY_Level=0.2f);

typedef cBaseList <cLevin> cLevinList;
typedef cBaseStack <cLevin,cLevinList> cLevinDispatcher;

class cL3D : public cLevinDispatcher
{
public:
	cL3D()																	{}
	~cL3D()																	{}
	void Draw(unsigned short *scr,unsigned short *zBuffer);
private:
};
extern cL3D *L3D;

#endif // __LEVIN_H__

