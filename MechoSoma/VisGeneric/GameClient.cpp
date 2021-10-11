#include "BaseReflection.h"
#include "GameClient.h"
#include "VisGeneric.h"
#include "PolyMgr.h"
#include "stdlib.h"
#include "Levin.h"
#include "time.h"

#define WARP_WAVE_PREC						12

extern cInterfaceVisGeneric *gb_IVisGeneric;
int gb_InitKeyMode=0;

float GlobalGetCPUSpeed()
{
	int count=0;
	clock_t start, finish;
	start=clock();
	__asm
	{
		mov ebx,0
		mov ecx,1
		mov edx,2
		mov eax,15000000
		BEGIN_CYCL:
	
			add ebx,ecx
			add ebx,edx
			add ecx,1
			add edx,2

			sub eax,1
			jnz BEGIN_CYCL
		mov [count],ebx
	}
	finish=clock();
	if(count==0) exit(1);
	float CyclPentiumIII_450=0.11f;
	float duration=(float)(finish-start)/CLOCKS_PER_SEC;
	return CyclPentiumIII_450/duration;
}
cSurfaceReflectionMultiMaterial* BaseObject_SurfaceReflection_Create(float xPos,float yPos,float zPos,float xSizeReflection,float ySizeReflection,int xTextureSize,int yTextureSize,float du,float dv,char **TextureName,char **OpacityName,char *TexturePath,int KoofReflection,float dPeriod)
{
	float kCPU=GlobalGetCPUSpeed();
	if(kCPU>0.75f) kCPU=1.f;
	else if(kCPU>0.3f) kCPU=0.75f;
	else kCPU=0.5f;
	int xsize=round(80*kCPU),ysize=round(80*kCPU);
	float xstep=xSizeReflection/(xsize-1), ystep=ySizeReflection/(ysize-1);
	cVisGeneric *VisGeneric=(cVisGeneric*)gb_IVisGeneric;
	if((VisGeneric->GetActiveScene()==0)||(VisGeneric->GetActiveScene()->GetBaseObjectMgr()==0)) return 0;
	cManagerBaseObject *BaseObjectMgr=VisGeneric->GetActiveScene()->GetBaseObjectMgr();
	BaseObjectMgr->Release(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	cSurfaceReflectionMultiMaterial *SurfaceReflection=0;
	for(cBaseObjectList *start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			SurfaceReflection=(cSurfaceReflectionMultiMaterial*)start->Base;
	if(SurfaceReflection==0) SurfaceReflection=(cSurfaceReflectionMultiMaterial*)BaseObjectMgr->New(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL,BASEOBJECT_KIND_DRAW_REFLECTION);
	SurfaceReflection->SetPosition(xPos,yPos,zPos);
	SurfaceReflection->New(xsize,ysize,xstep,ystep);
	SurfaceReflection->xMaterial=xTextureSize; 
	SurfaceReflection->yMaterial=yTextureSize;
#ifdef _MECHOSOMA_
	extern char* mch_mainINI;
	extern char* getIniKey(char* fname,char* section,char* key);
	SurfaceReflection->AmplitudeColor=atoi(getIniKey(mch_mainINI,"m3d","MenuAmplitudeColor"));
	SurfaceReflection->Attribute=0;
	if(atoi(getIniKey(mch_mainINI,"m3d","AddColorReflectionSurface")))
		SurfaceReflection->Attribute=ATTRIBUTE_POLYGRID_COLOR_ADD;
	if(atoi(getIniKey(mch_mainINI,"m3d","InitKeyMode")))
		gb_InitKeyMode=1;
#endif
	for(int j=0;j<xTextureSize*yTextureSize;j++)
		SurfaceReflection->SetTexture(TextureName[j],TexturePath);
	sBaseColor4c *pColor=(sBaseColor4c*)SurfaceReflection->pColor;
	sBaseWarpWave *Warp=(sBaseWarpWave*)SurfaceReflection->pStructWarp;
	for(int i=0;i<xsize*ysize;i++)
	{
		pColor[i].Set(255,255,255,round(255*KoofReflection));
		Warp[i].w[0]=Warp[i].w[1]=Warp[i].v[0]=Warp[i].v[1]=0;
	}
	SurfaceReflection->u=xTextureSize; SurfaceReflection->v=yTextureSize;
	SurfaceReflection->uofs=SurfaceReflection->vofs=0.f;
	SurfaceReflection->du=du; SurfaceReflection->dv=dv;
	SurfaceReflection->KoofReflection=KoofReflection;
	SurfaceReflection->dPeriodWave=dPeriod;
	return SurfaceReflection;
}
void BaseObject_SurfaceReflection_WaveWarp(float x,float y,float z)
{
	cVisGeneric *VisGeneric=(cVisGeneric*)gb_IVisGeneric;
	if((VisGeneric->GetActiveScene()==0)||(VisGeneric->GetActiveScene()->GetBaseObjectMgr()==0)) return;
	cManagerBaseObject *BaseObjectMgr=VisGeneric->GetActiveScene()->GetBaseObjectMgr();
	BaseObjectMgr->Release(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	cBaseObjectList *start;
	for(start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			break;
	if(start==0||start->Base==0) return;
	cSurfaceReflectionMultiMaterial *SurfaceReflection=(cSurfaceReflectionMultiMaterial*)start->Base;
	float xpos=x-SurfaceReflection->x(), ypos=y-SurfaceReflection->y();
	int ipos=round(xpos/SurfaceReflection->xstep),jpos=round(ypos/SurfaceReflection->ystep);
	if((ipos<0)||(SurfaceReflection->xsize<=ipos)||(jpos<0)||(SurfaceReflection->ysize<=jpos)) return;
	sBaseWarpWave *Warp=(sBaseWarpWave*)SurfaceReflection->pStructWarp;
	Warp[jpos*SurfaceReflection->xsize+ipos].w[SurfaceReflection->WarpWavePlane]=round((1<<WARP_WAVE_PREC)*sin(SurfaceReflection->PeriodWave+=SurfaceReflection->dPeriodWave));
}
void BaseObject_SurfaceReflection_Draw(cUnknownClass *UCamera)
{
	cVisGeneric *VisGeneric=(cVisGeneric*)gb_IVisGeneric;
	if((VisGeneric->GetActiveScene()==0)||(VisGeneric->GetActiveScene()->GetBaseObjectMgr()==0)) return;
	cManagerBaseObject *BaseObjectMgr=VisGeneric->GetActiveScene()->GetBaseObjectMgr();
	BaseObjectMgr->Release(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	cBaseObjectList *start;
	for(start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			break;
	if(start==0||start->Base==0) return;
	cUnkClassDynArrayPointer ArrayCamera(KIND_ARRAYCAMERA);
	ArrayCamera(0)=UCamera;
	cSurfaceReflectionMultiMaterial *SurfaceReflection=(cSurfaceReflectionMultiMaterial*)start->Base;
//	Graph3d->BeginScene();
	// обновление волнового процесса
	SurfaceReflection->uofs+=SurfaceReflection->du; SurfaceReflection->vofs+=SurfaceReflection->dv;
	sBaseWarpWave *Warp=(sBaseWarpWave*)SurfaceReflection->pStructWarp;
	int p1=-1-SurfaceReflection->xsize,p2=-SurfaceReflection->xsize,p3=1-SurfaceReflection->xsize,
		p4=-1,	p5=0,	p6=1,
		p7=-1+SurfaceReflection->xsize,p8=+SurfaceReflection->xsize,p9=1+SurfaceReflection->xsize;
	int OldPlane=SurfaceReflection->WarpWavePlane,NewPlane=SurfaceReflection->WarpWavePlane=!SurfaceReflection->WarpWavePlane;
	for(int j=1;j<SurfaceReflection->ysize-1;j++)
		for(int i=1;i<SurfaceReflection->xsize-1;i++)
		{
			int l=i+SurfaceReflection->xsize*j;
			sBaseWarpWave *bWarp=&Warp[l];
			bWarp[0].v[NewPlane]=(int)((((bWarp[p1].w[OldPlane]+bWarp[p2].w[OldPlane]+bWarp[p3].w[OldPlane]+	
				bWarp[p4].w[OldPlane]+bWarp[p6].w[OldPlane]+
				bWarp[p7].w[OldPlane]+bWarp[p8].w[OldPlane]+bWarp[p9].w[OldPlane])>>3)-bWarp[0].w[OldPlane]+bWarp[0].v[OldPlane])*((1<<WARP_WAVE_PREC)-122))>>WARP_WAVE_PREC;
	        int ofs=bWarp[0].v[NewPlane]+bWarp[0].w[OldPlane];
			if(ofs>SHRT_MAX) ofs=SHRT_MAX; else if(ofs<SHRT_MIN) ofs=SHRT_MIN;
	        bWarp[0].w[NewPlane]=ofs;
			ofs=ofs>>(WARP_WAVE_PREC-7+1);
			SurfaceReflection->pXYZWarp[l].Set(0,0,ofs>>3);
			if((SurfaceReflection->Attribute&ATTRIBUTE_POLYGRID_COLOR_ADD)==0)
				ofs=255-SurfaceReflection->AmplitudeColor+((ofs*SurfaceReflection->AmplitudeColor)>>6);
			else
				ofs=128+((ofs*SurfaceReflection->AmplitudeColor)>>6);
			if(ofs>255) ofs=255; else if(ofs<0) ofs=0;
			SurfaceReflection->pColor[l].Set(ofs,ofs,ofs);
		}
	// собственно отрисовка
	P3D->Draw(&ArrayCamera,(cSurfaceReflectionMultiMaterial*)start->Base);
	ArrayCamera(0)=0;
	ArrayCamera.Delete();
}
void BaseObject_SurfaceReflection_Pos(Vect3f &Pos,Vect3f &ReflectionPos)
{
	ReflectionPos.set(0,0,10000);
	cVisGeneric *VisGeneric=(cVisGeneric*)gb_IVisGeneric;
	if((VisGeneric->GetActiveScene()==0)||(VisGeneric->GetActiveScene()->GetBaseObjectMgr()==0)) return;
	cManagerBaseObject *BaseObjectMgr=VisGeneric->GetActiveScene()->GetBaseObjectMgr();
	BaseObjectMgr->Release(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	cBaseObjectList *start;
	for(start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			break;
	if(start==0||start->Base==0) return;
	cSurfaceReflection *SurfaceReflection=(cSurfaceReflection*)start->Base;
	if(SurfaceReflection->inVolume(Pos))
	{
		ReflectionPos.z=SurfaceReflection->Matrix.trans().z;
		if(SurfaceReflection->pStructWarp) 
		{
			int i=round((Pos.x-SurfaceReflection->Matrix.trans().x)/SurfaceReflection->xstep);
			int j=round((Pos.y-SurfaceReflection->Matrix.trans().y)/SurfaceReflection->ystep);
			if((0<=i)&&(i<SurfaceReflection->xsize)&&(0<=j)&&(j<SurfaceReflection->ysize))
			{
				sBaseWarpWave *Warp=(sBaseWarpWave*)SurfaceReflection->pStructWarp;
				int warp=127+(Warp[SurfaceReflection->xsize*j+i].w[0]>>(WARP_WAVE_PREC-7));
				if(warp>255) warp=255; else if(warp<0) warp=0;
				int angle_warp=(warp*3600)>>8;
				assert(angle_warp>=0 && angle_warp<3600);
				ReflectionPos.x=round(3*COS_FLOAT_DGRAD(angle_warp)); 
				ReflectionPos.y=round(3*SIN_FLOAT_DGRAD(angle_warp)); 
			}
		}
	}
}
void BaseObject_SurfaceReflection_Pos(Vect3f &Pos,float &fWarp)
{
	fWarp=0;
	cVisGeneric *VisGeneric=(cVisGeneric*)gb_IVisGeneric;
	if((VisGeneric->GetActiveScene()==0)||(VisGeneric->GetActiveScene()->GetBaseObjectMgr()==0)) return;
	cManagerBaseObject *BaseObjectMgr=VisGeneric->GetActiveScene()->GetBaseObjectMgr();
	BaseObjectMgr->Release(BASEOBJECT_TYPE_BASEWAVEPROCESS,BASEOBJECT_KIND_PHYSICPROCESS);
	cBaseObjectList *start;
	for(start=BaseObjectMgr->BaseList;start;start=start->next)
		if(start->Base->GetType(BASEOBJECT_TYPE_BASEREFLECTION_SURFACE_MULTIMATERIAL))
			break;
	if(start==0||start->Base==0) return;
	cSurfaceReflection *SurfaceReflection=(cSurfaceReflection*)start->Base;
	if((SurfaceReflection->inVolume(Pos))&&(SurfaceReflection->pStructWarp))
	{
		int i=round((Pos.x-SurfaceReflection->Matrix.trans().x)/SurfaceReflection->xstep);
		int j=round((Pos.y-SurfaceReflection->Matrix.trans().y)/SurfaceReflection->ystep);
		if((0<=i)&&(i<SurfaceReflection->xsize)&&(0<=j)&&(j<SurfaceReflection->ysize))
		{
			sBaseWarpWave *Warp=(sBaseWarpWave*)SurfaceReflection->pStructWarp;
			fWarp+=Warp[SurfaceReflection->xsize*j+i].w[0];
		}
	}
	fWarp/=1<<WARP_WAVE_PREC;
}
float GetCoord(float x1,float x2)
{
/*
#ifdef _MECHOSOMA_
	float d=x1-x2;
	float ad=fabs(d);
	float dd=(1<<H_SIZE_POWER)-ad;
	if(ad<=dd) return d;
	return d<0 ? d+(1<<H_SIZE_POWER) : d-(1<<H_SIZE_POWER);
#else
*/
	return x1-x2;
//#endif
}
void GenerationLevin(float x,float y,float z,float x1,float y1,float z1,float x2,float y2,float z2,
					 float r,float g,float b,float p_Stop,float p_Nascency,float p_X,float p_Y,float pX_Level,float pY_Level)
{
/*
	float Vx1=GetCoord(x,x1), Vy1=GetCoord(y,y1);
	float l=sqrtf(Vx1*Vx1+Vy1*Vy1);
	if(l==0) return;	Vx1/=l;	Vy1/=l;
#ifdef _MECHOSOMA_
	if(l>=(1<<H_SIZE_POWER)) l-=(1<<H_SIZE_POWER);
	else if(l>=(1<<(H_SIZE_POWER-1))) l=(1<<H_SIZE_POWER)-l;
#endif
	cLevin *Levin1=new cLevin(p_Stop,p_Nascency,p_X,p_Y,pX_Level,pY_Level);
	Levin1->SetColor(r,g,b);
	Levin1->SetPosition(x1,y1,z1,Vx1,Vy1);
	Levin1->size=round(l);
	if(L3D) L3D->Push(Levin1);

	float Vx2=GetCoord(x,x2), Vy2=GetCoord(y,y2);
	l=sqrtf(Vx2*Vx2+Vy2*Vy2);
	if(l==0) return;	Vx2/=l;	Vy2/=l;
#ifdef _MECHOSOMA_
	if(l>=(1<<H_SIZE_POWER)) l-=(1<<H_SIZE_POWER);
	else if(l>=(1<<(H_SIZE_POWER-1))) l=(1<<H_SIZE_POWER)-l;
#endif
	cLevin *Levin2=new cLevin(p_Stop,p_Nascency,p_X,p_Y,pX_Level,pY_Level);
	Levin2->SetColor(r,g,b);
	Levin2->SetPosition(x2,y2,z2,Vx2,Vy2);
	Levin2->size=round(l);
	if(L3D) L3D->Push(Levin2);
*/
}
