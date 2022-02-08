#ifndef __MATH3D_H__
#define __MATH3D_H__

#include "Maths.h"
#include "UMATH.H"
#include "Base.h"

extern void GetNormal(float *v1,float *v2,float *v3,float *Normal);
extern void NormalizeVector(int norma,int *x,int *y,int *z);
extern void VectMult(float xv1,float yv1,float zv1,float xv2,float yv2,float zv2,float *x,float *y,float *z);
extern float VectMult(float xv1,float yv1,float xv2,float yv2);
extern float AngleVector(float xv1,float yv1,float xv2,float yv2);
extern float NormaVector(float xv1,float yv1);

#endif //__MATH3D_H__

#ifndef __MATRIX_H__
#define __MATRIX_H__

class cMatrix : public MatXf
{
public:
	cMatrix() {}
	cMatrix(const Mat3f& R_, const Vect3f& d_) : MatXf(R_,d_) {}
	cMatrix(float r00,float r01,float r02,float tx,
			float r10,float r11,float r12,float ty,
			float r20,float r21,float r22,float tz);
	void NewMatrix()									{ MatXf::set(Mat3f::ID,Vect3f::ZERO); }
	void SetTranslate(float x=0,float y=0,float z=0);
	void Translate(float x=0,float y=0,float z=0);
	void Rotate(float Ax,float Ay,float Az);
	void Rotate(float w,float x,float y,float z);
	void Scale(float sx,float sy,float sz);
	inline float& operator () (int i,int j)				{ if(j<=2) return rot()[i][j]; else return trans()[i]; }
	inline cMatrix& operator = (MatXf Matrix)			{ rot()=Matrix.rot(); trans()=Matrix.trans(); return (*this); }
	inline void set(float* matrix)
	{
		rot()[0][0]=matrix[0]; rot()[1][0]=matrix[1]; rot()[2][0]=matrix[2]; 
		rot()[0][1]=matrix[3]; rot()[1][1]=matrix[4]; rot()[2][1]=matrix[5]; 
		rot()[0][2]=matrix[6]; rot()[1][2]=matrix[7]; rot()[2][2]=matrix[8]; 
		trans().set(matrix[9],matrix[10],matrix[11]);
	}
//	inline operator MatXf&()							{ return *(MatXf*)this; }
};

#define PREC_EPSILON			0.00001f

class cPlane
{
	float x1,y1,z1, x2,y2,z2, x3,y3,z3;
	float A,B,C,D;
public:
	cPlane(float x1,float y1,float z1,
		float x2,float y2,float z2,
		float x3,float y3,float z3);
	~cPlane()															{}
	char CrossCut(float x1,float y1,float z1,float x2,float y2,float z2,float *x,float *y,float *z);
	char CrossPolygon(float xv1,float yv1,float zv1,float xv2,float yv2,float zv2,float *x,float *y,float *z);
	// угол между двумя плоскостями (в градусах)
	float AnglePlane(cPlane *Plane);
	inline float Norma()								{ float f=A*A+B*B+C*C; if(f>PREC_EPSILON) return sqrtf(f); return 0; }
};

#endif // __MATRIX_H__

#ifndef __CONVERTOR3D_H__
#define __CONVERTOR3D_H__

extern float m0,m1,m2,m4,m5,m6,m8,m9,m10,dMx,dMy,dMz,xeCenterCamera,yeCenterCamera,xeFocusCamera,yeFocusCamera;
extern int xShareOfs,yShareOfs;

#define CONVERTOR_POINT_R(xw,yw,zw,xv,yv,zv)		{ xv=m0*(xw)+m1*(yw)+m2*(zw); yv=m4*(xw)+m5*(yw)+m6*(zw); zv=m8*(xw)+m9*(yw)+m10*(zw); }
#define CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv)		{ xv=m0*(xw)+m1*(yw)+m2*(zw)+dMx; yv=m4*(xw)+m5*(yw)+m6*(zw)+dMy; zv=m8*(xw)+m9*(yw)+m10*(zw)+dMz; }
#define CONVERTOR_POINT_VE(xv,yv,zv,xe,ye)			{ if(zv==0) zv=1; float div_zv=1/(zv); xe=round(xeCenterCamera+xeFocusCamera*(xv)*div_zv); ye=round(yeCenterCamera+yeFocusCamera*(yv)*div_zv); return round(zv); }
#define CONVERTOR_POINT_VE_FLOAT(xv,yv,zv,xe,ye)	{ if(zv==0) zv=1; float div_zv=1/(zv); xe=xeCenterCamera+xeFocusCamera*(xv)*div_zv; ye=yeCenterCamera+yeFocusCamera*(yv)*div_zv; return zv; }
#define CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye)	{ xe=round(xeCenterCamera+xeFocusCamera*(xv)); ye=round(yeCenterCamera+yeFocusCamera*(yv)); return round(zv); }
#define CONVERTOR_POINT_ORTHO_VE_FLOAT(xv,yv,zv,xe,ye)	{ xe=xeCenterCamera+xeFocusCamera*(xv); ye=yeCenterCamera+yeFocusCamera*(yv); return zv; }
#define SET_SHARE_OFS(x,y)							{ xShareOfs=GetShortDistX(x,round(Camera->x)); yShareOfs=GetShortDistY(y,round(Camera->y)); }

inline void ConvertorPointWV(float xw,float yw,float zw,float &xv,float &yv,float &zv)			{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); }
// Perspective
inline int  ConvertorPointWE(int xw,int yw,int zw,int &xe,int &ye)								{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointWE(int xw,int yw,int zw,short &xe,short &ye)							{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointWE(float xw,float yw,float zw,int &xe,int &ye)						{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,int &xe,int &ye)			{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,short &xe,short &ye)		{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline float ConvertorPointWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,float &xe,float &ye)		{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE_FLOAT(xv,yv,zv,xe,ye); }
// Ortho
inline int  ConvertorPointOrthoWE(int xw,int yw,int zw,int &xe,int &ye)							{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointOrthoWE(int xw,int yw,int zw,short &xe,short &ye)						{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointOrthoWE(float xw,float yw,float zw,int &xe,int &ye)				{ float xv,yv,zv; CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointOrthoWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,int &xe,int &ye)		{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointOrthoWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,short &xe,short &ye)	{ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE(xv,yv,zv,xe,ye); }
inline float ConvertorPointOrthoWE(float xw,float yw,float zw,float &xv,float &yv,float &zv,float &xe,float &ye){ CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_ORTHO_VE_FLOAT(xv,yv,zv,xe,ye); }
// Share
inline int  ConvertorPointShareWE(int xo,int yo,int zo,int &xe,int &ye)							{ float xv,yv,zv,xw=xo,yw=yo,zw=zo; int xr=xShareOfs+xw, yr=yShareOfs+yw; zw-=SHARE_FLOAT(xr*xr+yr*yr); CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }
inline int  ConvertorPointShareWE(float xw,float yw,float zw,int &xe,int &ye)					{ float xv,yv,zv; int xr=xShareOfs+xw, yr=yShareOfs+yw;	zw-=SHARE_FLOAT(xr*xr+yr*yr); CONVERTOR_POINT_RT(xw,yw,zw,xv,yv,zv); CONVERTOR_POINT_VE(xv,yv,zv,xe,ye); }

inline int  InverseConvertorVector(float xw,float yw,float zw,int *xv,int *yv)
{
	(*xv)= -round(m0*xw+m4*yw+m8*zw);
	(*yv)=  round(m1*xw+m5*yw+m9*zw);
	return -round(m2*xw+m6*yw+m10*zw);
}
inline int  ConvertorVector(int xw,int yw,int zw,int *xv,int *yv)
{
	(*xv)=round(m0*xw+m1*yw+m2*zw);
	(*yv)=round(m4*xw+m5*yw+m6*zw);
	return round(m8*xw+m9*yw+m10*zw);
}
inline void LoadMatrix(cMatrix *m,float sx,float sy,float sz)
{ // осуществляет переход из правой системы координат в левую
	m0 =MATRIX_SIGN_X(sx*m->rot()[0][0]), m1 =MATRIX_SIGN_X(sy*m->rot()[0][1]), m2 =MATRIX_SIGN_X(sz*m->rot()[0][2]), dMx=MATRIX_SIGN_X(m->trans()[0]);	
	m4 =MATRIX_SIGN_Y(sx*m->rot()[1][0]), m5 =MATRIX_SIGN_Y(sy*m->rot()[1][1]), m6 =MATRIX_SIGN_Y(sz*m->rot()[1][2]), dMy=MATRIX_SIGN_Y(m->trans()[1]);
	m8 =MATRIX_SIGN_Z(sx*m->rot()[2][0]), m9 =MATRIX_SIGN_Z(sy*m->rot()[2][1]), m10=MATRIX_SIGN_Z(sz*m->rot()[2][2]), dMz=MATRIX_SIGN_Z(m->trans()[2]);
}

#endif // __CONVERTOR3D_H__
