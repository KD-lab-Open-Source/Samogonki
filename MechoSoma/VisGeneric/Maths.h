#ifndef __MATHS_H__
#define __MATHS_H__

#include <cstdint>
#include <cmath>
#include <cassert>
#include <climits>

#include "Base.h"

#define XSIZE_CYCL_FLT				2048.f
#define YSIZE_CYCL_FLT				2048.f
#define XSIZE_CYCL_INT				2048
#define YSIZE_CYCL_INT				2048
#define ABS(a)						((a) >= 0 ? (a) :-(a))
#define GET_MIN(a,b)				((a) <=(b)? (a) : (b))
#define GET_MAX(a,b)				((a) >=(b)? (a) : (b))
#define SWAP_TYPE(type,a,b)			{ type swap=(a); (a)=(b); (b)=swap; }
#define FREE_MEM(a)					{ if(a) delete a; a=0; }
#define COPY_MEM(a,b,type,size)		{ if(b) { a=new type [(size)]; memcpy(a,b,(size)*sizeof(type)); } else a=b; }
#define SET_SHORT_DIST(ax,ay,bx,by)	{ ax=GetShortDistX(ax,bx)+bx; ay=GetShortDistY(ay,by)+by; }

#define SET_USHORT(a,b)				{ assert(b<=USHRT_MAX); a=b; }
#define SET_SHORT(a,b)				{ assert((b<=SHRT_MAX)&&(b>=SHRT_MIN)); a=b; }
#define SET_UCHAR(a,b)				{ assert(b<=UCHAR_MAX); a=b; }
#define SET_CHAR(a,b)				{ assert((b<=SCHAR_MAX)&&(b>=SCHAR_MIN)); a=b; }
#define SET_BYTE(a,b)				{ if((b)<0) a=0; else if((b)>255) a=255; else a=(b); }

#define GET_DIST_TYPE(v,v0,v1,type,abs_type,cycl_type)	\
	{												\
		type d = v0 - v1;							\
		type ad = (type)abs_type(d);				\
		type dd = cycl_type - ad;					\
		if(ad <= dd) return d;						\
		v = d < 0 ? d + cycl_type : d - cycl_type;	\
	}

inline int round(int i)	{ return i; }
inline int CosAngle(int x1,int y1,int z1,int x2,int y2,int z2)
{	return (x1*x2+y1*y2+z1*z2)>>MAX_SIZE_NORMAL_BIT;	}
inline void memfill(uint32_t *buf,int size,uint32_t fill)
{
	uint32_t *start=buf, *end=buf+size;
	while(start<end) (*(start++))=fill;
}
inline float Cycl(float a,float border)
{
//	assert(a<10000);
//	assert(a>-10000);
//	while(a>=border) a-=border; while(a<0) a+=border; return a;
	return a=fmod(fmod(a,border)+border,border);
}
inline int GetShortDistX(int a,int b,int cycl=XSIZE_CYCL_INT)
{
	a=Cycl(a,cycl);
	GET_DIST_TYPE(a,a,b,int,ABS,cycl);
	return a;
}
inline int GetShortDistY(int a,int b,int cycl=YSIZE_CYCL_INT)
{
	a=Cycl(a,cycl);
	GET_DIST_TYPE(a,a,b,int,ABS,cycl);
	return a;
}
inline float GetShortDistX(float a,float b,float cycl=XSIZE_CYCL_FLT)
{
	a=Cycl(a,cycl);
	GET_DIST_TYPE(a,a,b,float,fabs,cycl);
	return a;
}
inline float GetShortDistY(float a,float b,float cycl=YSIZE_CYCL_FLT)
{
	a=Cycl(a,cycl);
	GET_DIST_TYPE(a,a,b,float,fabs,cycl);
	return a;
}

#endif // __MATHS_H__

#ifndef __ARRAY_H__
#define __ARRAY_H__

#define PREC_ARRAY					12
#define	PERSPECT_MAX				10
#define	PERSPECT_PREC				8
#define	SIZE_DIV_SQRT_INT			64*64*3
#define SIN_COS_PREC				10
#define GRAD_TO_DGRAD(a)			((a)*SIN_COS_PREC)

#define OMNI_MAP_SIZE				6 /*7*/					/*for Omni*/
#define OMNI_CONSTANT				1.0
#define OMNI_QUADRATIC				0.01
#define OMNI_LINEAR					(-(OMNI_QUADRATIC+OMNI_CONSTANT))
#define DEPTH_INTENSITY_OMNI_BIT	7						/* recomended < 8 */
#define DEPTH_INTENSITY_OMNI		((1<<DEPTH_INTENSITY_OMNI_BIT)-1)

#define COSN_CHAR(a)			CosNChar[a]
#define SIN_FLOAT_DGRAD(a)		SinFloat[a]				/* dgrad = GRAD_TO_DGRAD(grad) */
#define COS_FLOAT_DGRAD(a)		CosFloat[a]
#define SHARE_FLOAT(a)			ShareFloat[(a)>>5]

#define DEL_MEM(p)				{ if(p) delete p; p=0; }

extern unsigned char	*CosNChar;
extern float			SinFloat[GRAD_TO_DGRAD(360)],CosFloat[GRAD_TO_DGRAD(360)];
extern int				DivSqrt[SIZE_DIV_SQRT_INT];
extern float			*ShareFloat;

//for Light - Omni
extern unsigned char	*OmniLightMap;

extern void InitArray();
extern void InitArrayShare(int RadiusShare=1000);
extern void DoneArray();

#endif //__ARRAY_H__

#ifndef __SPLINE_H__
#define __SPLINE_H__

struct cSpline
{
public:
	float *KoofQuadSpline;

	cSpline()								{ KoofQuadSpline=0; }
	cSpline(cSpline *Spline,int number)		{ KoofQuadSpline=new float[number]; for(int i=0;i<number;i++) KoofQuadSpline[i]=Spline->KoofQuadSpline[i]; }
	~cSpline()								{ if(KoofQuadSpline) delete[] KoofQuadSpline; }
	void CalcKoofQuadSpline(int number,float *x,float *y);
	float CalcFunction(int number,float *x,float *y,float xu);
};

#endif //__SPLINE_H__
