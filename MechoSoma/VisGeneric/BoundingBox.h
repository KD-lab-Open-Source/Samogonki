#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include <assert.h>
#include "umath.h"

class cBoundingBox
{
public:
	Vect3f	Min,Max,MinOriginal,MaxOriginal,scale;
	float	RadiusOriginal,Radius,ScaleMax;

	inline cBoundingBox();

	inline void AddPoint(const Vect3f &v);
	inline void Translate(const Vect3f &dv);
	inline void SetScale(const Vect3f &s);
	inline void Scale(const Vect3f &s)									{ Vect3f tmp=scale*s; SetScale(tmp); }
	inline void SetScaleOriginal(const Vect3f &s);
	inline void Interpolate(cBoundingBox& a,cBoundingBox& b,float t);
	inline void GetCenter(Vect3f &v)									{ v=(Min+Max)*0.5f; }
	inline void GetOriginal(Vect3f &vMin,Vect3f &vMax)				{ vMin=MinOriginal; vMax=MaxOriginal; }
	inline void SetOriginal(const Vect3f &vMin,const Vect3f &vMax);
	inline cBoundingBox operator + (cBoundingBox &a)					{ cBoundingBox b=*this; b+=a; return b; }
	inline cBoundingBox& operator +=(cBoundingBox &a);
	inline int Intersection(cBoundingBox &a);
	inline int TestPoint(const Vect3f &v);
	inline int AssertValid();
	inline void CalcBounding();
	inline void CalcRadius();
};

#define MAX_BOUNDING_BOX				1e30f
#define TEST_MAX(a,v)					{ if(a.x<v.x) { a.x=v.x; fl=0; } if(a.y<v.y) { a.y=v.y; fl=0; } if(a.z<v.z) { a.z=v.z; fl=0; } }
#define TEST_MIN(a,v)					{ if(a.x>v.x) { a.x=v.x; fl=0; } if(a.y<v.y) { a.y=v.y; fl=0; } if(a.z>v.z) { a.z=v.z; fl=0; } }
#define INTERPOLATE2(a,b,t)				((a)+((b)-(a))*(t))

inline cBoundingBox::cBoundingBox()
{ 
	MaxOriginal.set(-MAX_BOUNDING_BOX,-MAX_BOUNDING_BOX,-MAX_BOUNDING_BOX); 
	MinOriginal.set(MAX_BOUNDING_BOX,MAX_BOUNDING_BOX,MAX_BOUNDING_BOX); 
	Max=MaxOriginal; Min=MinOriginal; 
	Radius=RadiusOriginal=0; ScaleMax=1;
	scale.set(1,1,1);
}
inline void cBoundingBox::SetScaleOriginal(const Vect3f &s)
{
	assert(s.x>=0); assert(s.y>=0); assert(s.z>=0);
	MaxOriginal*=s; MinOriginal*=s; 
	CalcRadius(); CalcBounding();
}
inline void cBoundingBox::SetScale(const Vect3f &s)
{ 
	assert(s.x>=0); assert(s.y>=0); assert(s.z>=0);
	scale=s; ScaleMax=s.x; 
	if(ScaleMax<s.y) ScaleMax=s.y;
	if(ScaleMax<s.z) ScaleMax=s.z;
	CalcBounding();
}	
inline void cBoundingBox::Translate(const Vect3f &dv)
{ 
	MinOriginal+=dv;
	MaxOriginal+=dv; 
	CalcBounding();
}	
inline void cBoundingBox::AddPoint(const Vect3f &v)
{
	int fl=1;
	TEST_MIN(MinOriginal,v);
	TEST_MAX(MaxOriginal,v);
	if(!fl) { CalcRadius(); CalcBounding(); }
}
inline void cBoundingBox::CalcRadius()
{
	RadiusOriginal=MaxOriginal.distance(MinOriginal)*0.5f;
}
inline void cBoundingBox::CalcBounding()
{
	Min=MinOriginal*scale; Max=MaxOriginal*scale; 
	Radius=RadiusOriginal*ScaleMax;
	assert(AssertValid());
}
inline int cBoundingBox::AssertValid()
{
#ifdef _DEBUG
		if((Min.x>Max.x)||(MinOriginal.x>MaxOriginal.x)||
			(Min.y>Max.y)||(MinOriginal.y>MaxOriginal.y)||
			(Min.z>Max.z)||(MinOriginal.z>MaxOriginal.z))
			return 0;
#endif
	return 1;
}
inline cBoundingBox& cBoundingBox::operator +=(cBoundingBox &a)
{ 
	int fl=1;
	TEST_MIN(MinOriginal,a.MinOriginal)
	TEST_MAX(MaxOriginal,a.MinOriginal)
	TEST_MIN(MinOriginal,a.MaxOriginal)
	TEST_MAX(MaxOriginal,a.MaxOriginal)
	if(!fl) { CalcRadius(); CalcBounding(); }
	return *this;	
}
inline void cBoundingBox::Interpolate(cBoundingBox& a,cBoundingBox& b,float t)
{
	RadiusOriginal=INTERPOLATE2(a.RadiusOriginal,b.RadiusOriginal,t);
	MinOriginal=INTERPOLATE2(a.MinOriginal,b.MinOriginal,t);
	MaxOriginal=INTERPOLATE2(a.MaxOriginal,b.MaxOriginal,t);
	CalcBounding();	
}
inline void cBoundingBox::SetOriginal(const Vect3f &vMin,const Vect3f &vMax)
{
	MinOriginal=vMin; MaxOriginal=vMax; 
	CalcRadius(); CalcBounding();
}
inline int cBoundingBox::Intersection(cBoundingBox &a)
{
	assert(AssertValid()); assert(a.AssertValid());
	if ((Min.x>a.Max.x)||(Max.x<a.Max.x)) return 0;
	if ((Min.y>a.Max.y)||(Max.y<a.Max.y)) return 0;
	if ((Min.z>a.Max.z)||(Max.z<a.Max.z)) return 0;
	return 1;
}
inline int cBoundingBox::TestPoint(const Vect3f &v)
{
	assert(AssertValid()); 
	return ((v.x>=Min.x)&&(v.x<=Max.x)&&(v.y>=Min.y)&&(v.y<=Max.y)&&(v.z>=Min.z)&&(v.z<=Max.z));
}

#endif // __BOUNDING_BOX_H__