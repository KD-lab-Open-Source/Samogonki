#ifndef __UMATH_H__
#define __UMATH_H__

#if _MSC_VER == 1100 /* if MSVisual C++ 5.0 */
#ifndef __forceinline 
#define __forceinline inline
#endif //__forceinline 
#endif //_MSC_VER

#include "xmath.h"

#define EQUAL_FLT(a,b)								(fabs((a)-(b))<.001f)
#define SET_BIT(a,b)								(a)|=(b)
#define CLEAR_BIT(a,b)								(a)&=~(b)

#define GRAD_RAD  									0.01745329251994f
//#define PI_RAD										3.14159265359f  // use M_PI instead

#define MEM_SET_NULL(a)								memset(&(a),0,sizeof(a))

inline int ReturnBit(int a)
{
	int count=0;
	while((a>>=1)>=1) count++;
	return count;
}
inline float LinearInterpolate(float a,float b,float x)
{
	return a+(b-a)*x;
}
inline float CosInterpolate(float a,float b,float x)
{
	float f=(1-cosf(x*3.14159f))*0.5f;
	return a*(1-f)+b*f;
}
inline float ArcLengthInterpolate(float a,float b,float x)
{
	return a+((1-x)*x+x)*(b-a);
}
inline float CubicInterpolate(float v0,float v1,float v2,float v3,float x) // v0 <= v1 <= x <= v2 <= v3
{
	float p=(v3-v2)-(v0-v1);
	float q=(v0-v1)-p;
	float r=v2-v0;
	float s=v1;
	return ((p*x+q)*x+r)*x+s;
}
inline float TriangleArea(float x1,float y1,float x2,float y2,float x3,float y3)
{ // формула Герона
	float a=(x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
	if(a<0) return 0;
	a=sqrtf(a);
	float b=(x3-x1)*(x3-x1)+(y3-y1)*(y3-y1);
	if(b<0) return 0;
	b=sqrtf(b);
	float c=(x2-x3)*(x2-x3)+(y2-y3)*(y2-y3);
	if(c<0) return 0;
	c=sqrtf(c);
	float p=(a+b+c)*0.5f;
	p=p*(p-a)*(p-b)*(p-c);
	if(p<0) return 0;
	return sqrtf(p);
}


struct sVect3c
{
	char x,y,z;

	sVect3c()										{ }
	sVect3c(int x,int y,int z)						{ sVect3c::x=x; sVect3c::y=y; sVect3c::z=z; }
	sVect3c(sVect3c &v)								{ sVect3c::x=v.x; sVect3c::y=v.y; sVect3c::z=v.z; }
	sVect3c(Vect3f &v)								{ sVect3c::x=round(v.x); sVect3c::y=round(v.y); sVect3c::z=round(v.z); }

	inline int norm()								{ return round(sqrtf((float)(x*x+y*y+z*z))); }
	inline int norm2()								{ return x*x+y*y+z*z; }
	inline int distance(const sVect3c &v)			{ int dx=v.x-x,dy=v.y-y,dz=v.z-z; return round(sqrtf((float)(dx*dx+dy*dy+dz*dz))); }
	inline void normalize(int norma)				{ float f=(float)norma/(float)sqrtf((float)(x*x+y*y+z*z)); x=round(x*f); y=round(y*f); z=round(z*f); }
	inline void set(int x,int y,int z)				{ sVect3c::x=x; sVect3c::y=y; sVect3c::z=z; }
	inline sVect3c& operator += (const sVect3c& v)	{ x+=v.x; y+=v.y; z+=v.z; return *this; }
	inline sVect3c& operator -= (const sVect3c& v)	{ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline sVect3c& operator *= (const sVect3c& v)	{ x*=v.x; y*=v.y; z*=v.z; return *this; }
	inline sVect3c& operator *= (float f)			{ x=round(x*f); y=round(y*f); z=round(z*f); return *this; }
	inline sVect3c& operator /= (float f)			{ if(f!=0.f) f=1/f; else f=0.0001f; x=round(x*f); y=round(y*f); z=round(z*f); return *this; }
	inline sVect3c& operator = (const sVect3c& v)	{ x=v.x; y=v.y; z=v.z; return *this; }
	inline sVect3c operator - (const sVect3c &v)	{ sVect3c tmp(x-v.x,y-v.y,z-v.z); return tmp; }
	inline sVect3c operator + (const sVect3c &v)	{ sVect3c tmp(x+v.x,y+v.y,z+v.z); return tmp; }
	inline sVect3c operator * (const sVect3c &v)	{ sVect3c tmp(x*v.x,y*v.y,z*v.z); return tmp; }
	inline sVect3c operator * (float f)				{ sVect3c tmp(round(x*f),round(y*f),round(z*f)); return tmp; }
	inline sVect3c operator / (float f)				{ if(f!=0.f) f=1/f; else f=0.0001f; sVect3c tmp(round(x*f),round(y*f),round(z*f)); return tmp; }
	inline int operator == (const sVect3c& v)		{ return ((x==v.x)&&(y==v.y)&&(z==v.z)); }
	inline operator float* () const					{ return (float*)this; }
};

struct sColor4f
{
	float r,g,b,a;
	
	sColor4f() 										{}
	sColor4f(const float f[4])	{ r = f[0]; g = f[1]; b = f[2]; a = f[3];}
	sColor4f(float r,float g,float b,float a=1.f)	{ sColor4f::r=r; sColor4f::g=g; sColor4f::b=b; sColor4f::a=a; }

	void set(float r,float g,float b,float a)		{ sColor4f::r=r; sColor4f::g=g; sColor4f::b=b; sColor4f::a=a; }
	sColor4f&	operator += (const sColor4f &color)	{ r+=color.r; g+=color.g; b+=color.b; a+=color.a; return *this; }
	sColor4f&	operator *= (const sColor4f &color)	{ r*=color.r; g*=color.g; b*=color.b; a*=color.a; return *this; }
	sColor4f&	operator *= (float f)				{ r*=f; g*=f; b*=f; a*=f; return *this; }
	sColor4f&	operator /= (float f)				{ if(f!=0) f=1/f; else f=0.001f; r*=f; g*=f; b*=f; a*=f; return *this; }
	sColor4f	operator + (const sColor4f &color) const	{ sColor4f tmp(r+color.r,g+color.g,b+color.b,a+color.a); return tmp; }
	sColor4f	operator * (const sColor4f &color) const	{ sColor4f tmp(r*color.r,g*color.g,b*color.b,a*color.a); return tmp; }
	sColor4f	operator * (float f) const 			{ sColor4f tmp(r*f,g*f,b*f,a*f); return tmp; }
	sColor4f	operator / (float f) const 			{ if(f!=0.f) f=1/f; else f=0.001f; sColor4f tmp(r*f,g*f,b*f,a*f); return tmp; }
	int GetR() const 								{ return round(255*r); }
	int GetG() const 								{ return round(255*g); }
	int GetB() const 								{ return round(255*b); }
	int GetA() const 								{ return round(255*a); }
	inline void interpolate(sColor4f &u,sColor4f &v,float f) { r=u.r+(v.r-u.r)*f; g=u.g+(v.g-u.g)*f; b=u.b+(v.b-u.b)*f; a=u.a+(v.a-u.a)*f; }
};

struct sColor4c
{
	unsigned char r,g,b,a;
	
	sColor4c()										{}
	sColor4c(int rc,int gc,int bc)					{ r=rc; g=gc; b=bc; }
	sColor4c(int rc,int gc,int bc,int ac)			{ r=rc; g=gc; b=bc; a=ac; }
	inline void set(int rc,int gc,int bc)			{ r=rc; g=gc; b=bc; }
	inline void set(int rc,int gc,int bc,int ac)	{ r=rc; g=gc; b=bc; a=ac; }
	inline sColor4c& operator += (sColor4c &p)		{ r+=p.r; g+=p.g; b+=p.b; a+=p.a; return *this; }
	inline sColor4c operator + (sColor4c &p)		{ sColor4c tmp(r+p.r,g+p.g,b+p.b,a+p.a); return tmp; }
};

struct sColor4s
{
	short r,g,b,a;
	
	sColor4s()										{}
	sColor4s(int rc,int gc,int bc)					{ r=rc; g=gc; b=bc; }
	sColor4s(int rc,int gc,int bc,int ac)			{ r=rc; g=gc; b=bc; a=ac; }
	inline void set(int rc,int gc,int bc)			{ r=rc; g=gc; b=bc; }
	inline void set(int rc,int gc,int bc,int ac)	{ r=rc; g=gc; b=bc; a=ac; }
	inline sColor4s& operator += (sColor4s &p)		{ r+=p.r; g+=p.g; b+=p.b; a+=p.a; return *this; }
	inline sColor4s operator + (sColor4s &p)		{ sColor4s tmp(r+p.r,g+p.g,b+p.b,a+p.a); return tmp; }
};

struct sRectangle4f
{
	Vect2f	min;
	Vect2f	max;
	
	sRectangle4f()									{ }
	sRectangle4f(float xmin,float ymin,float xmax,float ymax)	{ min.x=xmin; min.y=ymin; max.x=xmax; max.y=ymax; }
	sRectangle4f(sRectangle4f &rect)				{ min=rect.min; max=rect.max; }
	inline void set(float xmin,float ymin,float xmax,float ymax){ min.x=xmin; min.y=ymin; max.x=xmax; max.y=ymax; }
	float xmin() const								{ return min.x; }
	float ymin() const								{ return min.y; }
	float xmax() const								{ return max.x; }
	float ymax() const								{ return max.y; }
	float& xmin()									{ return min.x; }
	float& ymin()									{ return min.y; }
	float& xmax()									{ return max.x; }
	float& ymax()									{ return max.y; }
};

struct sBox6f
{
	Vect3f	min;
	Vect3f	max;

	sBox6f()										{ }
	sBox6f(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax)	{ min.x=xmin; min.y=ymin; min.z=zmin; max.x=xmax; max.y=ymax; max.z=zmax; }
	sBox6f(sBox6f &rect)							{ min=rect.min; max=rect.max; }
	inline void SetBound(const Vect3f &v)			{ if(xmin()>v.x) xmin()=v.x; if(xmax()<v.x) xmax()=v.x; if(ymin()>v.y) ymin()=v.y; if(ymax()<v.y) ymax()=v.y; if(zmin()>v.z) zmin()=v.z; if(zmax()<v.z) zmax()=v.z; }
	inline sBox6f& operator += (const Vect3f &v)	{ min+=v; max+=v; return *this; }
	inline sBox6f& operator -= (const Vect3f &v)	{ min-=v; max-=v; return *this; }
	float xmin() const								{ return min.x; }
	float xmax() const								{ return max.x; }
	float ymin() const								{ return min.y; }
	float ymax() const								{ return max.y; }
	float zmin() const								{ return min.z; }
	float zmax() const								{ return max.z; }
	float& xmin()									{ return min.x; }
	float& xmax()									{ return max.x; }
	float& ymin()									{ return min.y; }
	float& ymax()									{ return max.y; }
	float& zmin()									{ return min.z; }
	float& zmax()									{ return max.z; }
};

struct sPlane4f
{
	float A,B,C,D;

	sPlane4f()											{ }
	sPlane4f(const Vect3f &a,const Vect3f &b,const Vect3f &c)
	{ // инициализация плоскости по трем точкам
		A=(b.y-a.y)*(c.z-a.z)-(c.y-a.y)*(b.z-a.z);
		B=(b.x-a.x)*(c.z-a.z)-(c.x-a.x)*(b.z-a.z);
		C=(b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
		GetNormal().normalize();
		D=-A*a.x-B*a.y-C*a.z;
	}
	void Set(const Vect3f &a,const Vect3f &b,const Vect3f &c)
	{ // инициализация плоскости по трем точкам
		A=(b.y-a.y)*(c.z-a.z)-(c.y-a.y)*(b.z-a.z);
		B=(b.x-a.x)*(c.z-a.z)-(c.x-a.x)*(b.z-a.z);
		C=(b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
		GetNormal().normalize();
		D=-A*a.x-B*a.y-C*a.z;
	}
	float GetDistance(const Vect3f &a)
	{ // расстояние от точки до плоскости
		float t=A*a.x+B*a.y+C*a.y+D;
		return t;
	}
	float GetCross(const Vect3f &a,const Vect3f &b)
	{ // поиск пересечения данной плоскости с прямой заданной двумя точками a и b
		Vect3f v=a-b;
		float t=A*v.x+B*v.y+C*v.z;
		if(t==0) return 0;			// прямая и плоскость параллельны
		t=(A*a.x+B*a.y+C*a.z+D)/t;
		return t;
	}
	Vect3f& GetNormal()								{ return *(Vect3f*)&A; }
};

inline MatXf GetMatrix(const Vect3f &Pos,const Vect3f &Angle)
{
	return MatXf(
		Mat3f((float)G2R(Angle.x), X_AXIS)*
		Mat3f((float)G2R(Angle.y), Y_AXIS)*
		Mat3f((float)G2R(Angle.z), Z_AXIS), 
		Pos);
}
inline void SetPosition(MatXf &Matrix,const Vect3f &Pos,const Vect3f &Angle)
{
	Matrix = MatXf(
		Mat3f((float)G2R(Angle.x), X_AXIS)*
		Mat3f((float)G2R(Angle.y), Y_AXIS)*
		Mat3f((float)G2R(Angle.z), Z_AXIS), 
		Pos);
}
inline void dSetPosition(MatXf &Matrix,const Vect3f &dPos,const Vect3f &dAngle)
{
	MatXf m = MatXf(
		Mat3f((float)G2R(dAngle.x), X_AXIS)*
		Mat3f((float)G2R(dAngle.y), Y_AXIS)*
		Mat3f((float)G2R(dAngle.z), Z_AXIS), 
		dPos);
	Matrix*=m;
}
inline void SetPosition(MatXf &Matrix,float *matrix)
{
	Matrix.rot()[0][0]=matrix[0]; Matrix.rot()[1][0]=matrix[1]; Matrix.rot()[2][0]=matrix[2]; 
	Matrix.rot()[0][1]=matrix[3]; Matrix.rot()[1][1]=matrix[4]; Matrix.rot()[2][1]=matrix[5]; 
	Matrix.rot()[0][2]=matrix[6]; Matrix.rot()[1][2]=matrix[7]; Matrix.rot()[2][2]=matrix[8]; 
	Matrix.trans().set(matrix[9],matrix[10],matrix[11]);
}
inline void Identity(MatXf &Matrix)
{
	Matrix.set(Mat3f(Vect3f(1,0,0),Vect3f(0,1,0),Vect3f(0,0,1)),Vect3f(0,0,0));
}
inline void Translate(MatXf &Matrix,const Vect3f &v)
{
	MatXf t(Mat3f::ID,v);
	Matrix*=t;	
}
inline void Rotate(MatXf &Matrix,const QuatF &q)
{
/*
	float xx2=q.x()*q.x()*2.f, yy2=q.y()*q.y()*2.f, zz2=q.z()*q.z()*2.f;
	float wx2=q.s()*q.x()*2.f, wy2=q.s()*q.y()*2.f, wz2=q.s()*q.z()*2.f;
	float xy2=q.x()*q.y()*2.f, xz2=q.x()*q.z()*2.f, yz2=q.y()*q.z()*2.f;
	MatXf r(Mat3f(
		Vect3f(	1.0f-yy2-zz2,	xy2-wz2,		xz2+wy2),
		Vect3f(	xy2+wz2,		1.0f-xx2-zz2,	yz2-wx2),
		Vect3f(	xz2-wy2,		yz2+wx2,		1.0f-xx2-yy2)),
		Vect3f(	0,				0,				0)); 
*/
	MatXf r(Mat3f(q),Vect3f(0,0,0)); 
	Matrix*=r;
}
inline void Rotate(MatXf &Matrix,const Vect3f &Angle)
{
	MatXf r(Mat3f((float)G2R(Angle.x), X_AXIS)*
			Mat3f((float)G2R(Angle.y), Y_AXIS)*
			Mat3f((float)G2R(Angle.z), Z_AXIS),
			Vect3f(0,0,0));
	Matrix*=r;
}
inline void Scale(MatXf &Matrix,const Vect3f &v)
{
	MatXf s(Mat3f(
		Vect3f(	v.x,	0,		0),
		Vect3f(	0,		v.y,	0),
		Vect3f(	0,		0,		v.z)),
		Vect3f(	0,		0,		0)); 
	Matrix*=s;
}
#define XR2L(a)						(+(a))
#define YR2L(a)						(-(a))
#define ZR2L(a)						(+(a))
inline void RightToLeft(MatXf &Matrix)
{
	Matrix.rot()(1,1)= XR2L(XR2L(Matrix.rot()(1,1)));
	Matrix.rot()(1,2)= XR2L(YR2L(Matrix.rot()(1,2)));
	Matrix.rot()(1,3)= XR2L(ZR2L(Matrix.rot()(1,3)));

	Matrix.rot()(2,1)= YR2L(XR2L(Matrix.rot()(2,1)));
	Matrix.rot()(2,2)= YR2L(YR2L(Matrix.rot()(2,2)));
	Matrix.rot()(2,3)= YR2L(ZR2L(Matrix.rot()(2,3)));

	Matrix.rot()(3,1)= ZR2L(XR2L(Matrix.rot()(3,1)));
	Matrix.rot()(3,2)= ZR2L(YR2L(Matrix.rot()(3,2)));
	Matrix.rot()(3,3)= ZR2L(ZR2L(Matrix.rot()(3,3)));

	Matrix.trans().x= XR2L(Matrix.trans().x);
	Matrix.trans().y= YR2L(Matrix.trans().y);
	Matrix.trans().z= ZR2L(Matrix.trans().z);
}
inline void RightToLeft(Vect3f &v)
{
	v.set(XR2L(v.x),YR2L(v.y),ZR2L(v.z));
}
inline void RightToLeft(QuatF &q)
{
	q.set(XR2L(YR2L(ZR2L(q.s()))),XR2L(q.x()),YR2L(q.y()),ZR2L(q.z()));
}

#endif // __UMATH_H__