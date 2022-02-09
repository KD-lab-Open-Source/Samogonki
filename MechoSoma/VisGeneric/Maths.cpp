#include "Maths.h"
#include "UMATH.H"
#include <stdlib.h>

#define	SIZE_SHARE_FLOAT			((74*74*2)<<3)

unsigned char	*CosNChar=0;
float			SinFloat[GRAD_TO_DGRAD(360)],CosFloat[GRAD_TO_DGRAD(360)];
int				DivSqrt[SIZE_DIV_SQRT_INT];
float			*ShareFloat=0;

unsigned char	*OmniLightMap=0;

void InitArray()
{
	if(CosNChar==0) // таблица cos в степени COS_DEGREE типа char для источника света cLight
	{	
		CosNChar=new unsigned char [128+3]; // 3 - это из-за неточности в расчетах
		int j;
		for(j=0;j<(128+3);j++)
		{
			int i=j;
			for(int k=0;k<7;k++)
				i=(i*j)>>7;
			if(i>127) i=127;
			COSN_CHAR(j)=i<<1;
		}
		for(j=0;j<GRAD_TO_DGRAD(360);j++)
		{
			SinFloat[j]=sinf(j*GRAD_RAD/GRAD_TO_DGRAD(1));
			CosFloat[j]=cosf(j*GRAD_RAD/GRAD_TO_DGRAD(1));
		}
	}
	if(OmniLightMap==0) // ¤ўг¬Ґа­ п дг­ЄжЁп гЎлў ­Ёп Ё­вҐ­бЁў­®бвЁ в®зҐз­®Ј® Ёбв®з­ЁЄ  cOmni
	{
		OmniLightMap=new unsigned char [1<<OMNI_MAP_SIZE<<OMNI_MAP_SIZE];
		for(int j=0;j<1<<OMNI_MAP_SIZE;j++)
			for(int i=0;i<1<<OMNI_MAP_SIZE;i++)
			{
				int x=i-(1<<OMNI_MAP_SIZE)/2,y=j-(1<<OMNI_MAP_SIZE)/2;
				double l=sqrt((double)x*x+y*y)/((1<<OMNI_MAP_SIZE)/2);
				if(l>=1.0f)	OmniLightMap[i+(j<<OMNI_MAP_SIZE)]=0;
				else
				{
					int k=round(DEPTH_INTENSITY_OMNI*(OMNI_QUADRATIC*l*l+OMNI_LINEAR*l+OMNI_CONSTANT));
					if(k<=0) k=0; else if(k>DEPTH_INTENSITY_OMNI) k=DEPTH_INTENSITY_OMNI;
					OmniLightMap[i+(j<<OMNI_MAP_SIZE)]=k;
				}
			}
	}
	DivSqrt[0]=(127<<16)/1;
	for(int i=1;i<SIZE_DIV_SQRT_INT;i++)
		DivSqrt[i]=(127<<16)/sqrt((float)i);
}
void InitArrayShare(int RadiusShare)
{
	if(ShareFloat) delete ShareFloat;
	ShareFloat=new float[SIZE_SHARE_FLOAT];
	for(int i=0;i<SIZE_SHARE_FLOAT;i++)
	{
		int d=RadiusShare*RadiusShare-i*(1<<5);
		if(d<0) d=0;
		ShareFloat[i]=RadiusShare-sqrt((float)d);
	}
}
void DoneArray()
{
	if(CosNChar) delete CosNChar; CosNChar=0;
	if(OmniLightMap) delete OmniLightMap; OmniLightMap=0; 
	if(ShareFloat) delete ShareFloat; ShareFloat=0;
}

void cSpline::CalcKoofQuadSpline(int number,float *x,float *y)
{
	if(KoofQuadSpline) delete KoofQuadSpline;
	KoofQuadSpline=new float[number];
	float *f=new float[number],*p=new float[number],*z=new float[number];
	number--;
	int i;
	for(i=1;i<number;i++)
	{
		float hi=x[i]-x[i-1], hi1=x[i+1]-x[i];
		KoofQuadSpline[i]=hi/(hi+hi1);
		f[i]=8*((y[i+1]-y[i])*hi-(y[i]-y[i-1])*hi1)/(hi*hi1*(hi+hi1));
	}
	KoofQuadSpline[0]=KoofQuadSpline[number]=0.5f;
	p[0]=3;
	z[0]=f[0]=f[number]=0;
	for(i=1;i<=number;i++)
	{
		float tmp=KoofQuadSpline[i]/p[i-1];
		p[i]=3-(1-KoofQuadSpline[i-1])*tmp;	
		z[i]=f[i]-z[i-1]*tmp;
	}
	KoofQuadSpline[number]=z[number]/p[number];
	for(i=number-1;i>=0;i--)
		KoofQuadSpline[i]=(z[i]-(1-KoofQuadSpline[i])*KoofQuadSpline[i+1])/p[i];	
	delete[] f;
    delete[] p;
    delete[] z;
}
float cSpline::CalcFunction(int number,float *x,float *y,float xu)
{
	int j1=0, jn=number-1, j;
	if(jn)
	{
		while(1)
		{
			j=(j1+jn)/2;
			if(jn<=j1+1) { j=jn; break; }
			if(xu<x[j]) jn=j;
			else if(xu>x[j]) j1=j;
			else break;
		}
		float hj=x[j]-x[j-1];
		float t=(xu-(x[j-1]+x[j])/2);
		float ts=0;
		if(t>0) ts=t*t;
		float yu=y[j-1]+((y[j]-y[j-1])/hj-(3*KoofQuadSpline[j-1]+KoofQuadSpline[j])/8*hj)*(xu-x[j-1]);
		return yu+KoofQuadSpline[j-1]/2*(xu-x[j-1])*(xu-x[j-1])+(KoofQuadSpline[j]-KoofQuadSpline[j-1])*ts/2;
	}
	else return y[0];
}
