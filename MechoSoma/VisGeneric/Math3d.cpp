#include "Math3d.h"
#include "Maths.h"
#include "math.h"

inline void GetNormal(float *v1,float *v2,float *v3,float *Normal)
{
	float	mx=v3[0]-v1[0],	my=v3[1]-v1[1],	mz=v3[2]-v1[2],
			nx=v2[0]-v1[0],	ny=v2[1]-v1[1],	nz=v2[2]-v1[2],
			x=my*nz-mz*ny, y=nx*mz-nz*mx, z=mx*ny-my*nx;
	float	l=sqrtf(x*x+y*y+z*z);
	if(l==0) { Normal[0]=Normal[1]=Normal[2]=0; return; }
	l=1/l;
	Normal[0]=x*l; Normal[1]=y*l; Normal[2]=z*l;
}
inline void NormalizeVector(int norma,int *x,int *y,int *z)
{
	float l=sqrtf((float)((*x)*(*x)+(*y)*(*y)+(*z)*(*z)));
	if(l==0) l=1;
	l=norma/l;
	(*x)=round((*x)*l); (*y)=round((*y)*l); (*z)=round((*z)*l);
}
inline void VectMult(float xv1,float yv1,float zv1,float xv2,float yv2,float zv2,float *x,float *y,float *z)
{
	*x=yv1*zv2-zv1*yv2;
	*y=zv1*xv2-xv1*zv2;
	*z=xv1*yv2-yv1*xv2;
}
inline float VectMult(float xv1,float yv1,float xv2,float yv2)
{
	return xv1*yv2-yv1*xv2;
}
inline float NormaVector(float xv1,float yv1)
{
	float n=xv1*xv1+yv1*yv1;
	if(n<PREC_EPSILON) return 0;
	return sqrtf(n);
}
inline float AngleVector(float xv1,float yv1,float xv2,float yv2)
{
	return acosf((xv1*xv2+yv1*yv2)/sqrtf(NormaVector(xv1,yv1)*NormaVector(xv2,yv2)));
}

#include "Maths.h"
#include "Math3d.h"
#include "math.h"

cMatrix::cMatrix(	float r00,float r01,float r02,float tx,
					float r10,float r11,float r12,float ty,
					float r20,float r21,float r22,float tz)
{ 
	rot()[0][0]=r00; rot()[0][1]=r01; rot()[0][2]=r02;
	rot()[1][0]=r10; rot()[1][1]=r11; rot()[1][2]=r12;
	rot()[2][0]=r20; rot()[2][1]=r21; rot()[2][2]=r22;
	trans().set(tx,ty,tz); 
}
void cMatrix::SetTranslate(float x,float y,float z)
{ 
	MatXf::trans().set(x,y,z); 
}
void cMatrix::Translate(float x,float y,float z)
{
	cMatrix t(1,0,0,x, 0,1,0,y, 0,0,1,z );
	(*this)*=t;	
}
void cMatrix::Rotate(float Ax,float Ay,float Az)
{
    float sinAx = sinf(Ax*GRAD_RAD), cosAx = cosf(Ax*GRAD_RAD);
    float sinAy = sinf(Ay*GRAD_RAD), cosAy = cosf(Ay*GRAD_RAD);
    float sinAz = sinf(Az*GRAD_RAD), cosAz = cosf(Az*GRAD_RAD);

	float sAx_sAz=sinAx*sinAz,	sAx_cAz=sinAx*cosAz,
		  cAx_sAz=cosAx*sinAz,  cAx_cAz=cosAx*cosAz;
/*	cMatrix r(	cosAy*cosAz,	-cAx_sAz-sAx_cAz*sinAy,	sAx_sAz-cAx_cAz*sinAy,	0,
				cosAy*sinAz,	cAx_cAz-sAx_sAz*sinAy,  -sAx_cAz-cAx_sAz*sinAy,	0,
				sinAy,			cosAy*sinAx,			cosAy*cosAx,			0 );	*/
	cMatrix rx(	1,	0,	0,	0,
			0,	cosAx,	-sinAx,	0,
			0,	sinAx,	cosAx,	0 );
	cMatrix ry(	cosAy,	0,	-sinAy,	0,
			0,	1,	0,	0,
			sinAy,	0,	cosAy,	0 );
	cMatrix rz(	cosAz,	-sinAz,	0,	0,
			sinAz,	cosAz,	0,	0,
			0,	0,	1,	0 );
	(*this)*=rx*ry*rz;
}
void cMatrix::Rotate(float w,float x,float y,float z)
{
	float xx2=x*x*2.f, yy2=y*y*2.f, zz2=z*z*2.f;
	float wx2=w*x*2.f, wy2=w*y*2.f, wz2=w*z*2.f;
	float xy2=x*y*2.f, xz2=x*z*2.f, yz2=y*z*2.f;
/*	cMatrix r(	1.f-yy2-zz2,	xy2+wz2,		xz2-wy2,	0,
				xy2-wz2,		1.f-xx2-zz2,	yz2+wx2,	0,
				xz2+wy2,		yz2-wx2,		1.f-xx2-yy2,0 );*/
	cMatrix r(	1.0f-yy2-zz2,	xy2-wz2,		xz2+wy2,		0,
				xy2+wz2,		1.0f-xx2-zz2,	yz2-wx2,		0,
				xz2-wy2,		yz2+wx2,		1.0f-xx2-yy2,	0); 
	(*this)*=r;
}
void cMatrix::Scale(float sx,float sy,float sz)
{
	cMatrix t (	sx,	0,	0,	0,
				0,	sy,	0,	0,
				0,	0,	sz,	0);
	(*this)*=t;	
}

cPlane::cPlane(float x1,float y1,float z1, float x2,float y2,float z2, float x3,float y3,float z3)
{
	cPlane::x1=x1,cPlane::y1=y1,cPlane::z1=z1,
	cPlane::x2=x2,cPlane::y2=y2,cPlane::z2=z2,
	cPlane::x3=x3,cPlane::y3=y3,cPlane::z3=z3;
	A=(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
	B=(x2-x1)*(z3-z1)-(x3-x1)*(z2-z1);
	C=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
	D=-A*x1-B*y1-C*z1;
}
char cPlane::CrossCut(float x1,float y1,float z1,float x2,float y2,float z2,float *x,float *y,float *z)
{
	float m=x2-x1, n=y2-y1, p=z2-z1, t;
	(*x)=x1; (*y)=y1; (*z)=z1;
	if((t=A*m+B*n+C*p)==0) 
		if(A*x1+B*y1+C*z1+D==0) return 1; else return 0;
	t=-(A*x1+B*y1+C*z1+D)/t;
	if((t<0)||(t>=1)) return 0;
	(*x)+=t*m; (*y)+=t*n; (*z)+=t*p;
	return 1;
}
char cPlane::CrossPolygon(float xv1,float yv1,float zv1,float xv2,float yv2,float zv2,float *x,float *y,float *z)
{
	float m=xv2-xv1, n=yv2-yv1, p=zv2-zv1, t;
	if((t=A*m+B*n+C*p)==0) return 0;
	t=-(A*xv1+B*yv1+C*zv1+D)/t;
	xv1+=t*m; yv1+=t*n; zv1+=t*p;
	float vx1,vy1,vz1, vx2,vy2,vz2;
	VectMult(x1-x2,y1-y2,z1-z2,xv1-x2,yv1-y2,zv1-z2,&vx1,&vy1,&vz1);
	VectMult(x3-x2,y3-y2,z3-z2,xv1-x2,yv1-y2,zv1-z2,&vx2,&vy2,&vz2);
	if(vx1*vx2+vy1*vy2+vz1*vz2>0) return 0;
	VectMult(x2-x3,y2-y3,z2-z3,xv1-x3,yv1-y3,zv1-z3,&vx1,&vy1,&vz1);
	VectMult(x1-x3,y1-y3,z1-z3,xv1-x3,yv1-y3,zv1-z3,&vx2,&vy2,&vz2);
	if(vx1*vx2+vy1*vy2+vz1*vz2>0) return 0;
	(*x)=xv1; (*y)=yv1; (*z)=zv1;
	return 1;
}
float cPlane::AnglePlane(cPlane *Plane)
{
	float n1=Norma(),n2=Plane->Norma();
	if((n1<PREC_EPSILON)||(n2<PREC_EPSILON)) return 0;
	float cosAngle=(A*Plane->A+B*Plane->B+C*Plane->C)/(n1*n2);
	return acosf(cosAngle)*(1/GRAD_RAD);
}

float m0,m1,m2,m4,m5,m6,m8,m9,m10,dMx,dMy,dMz,xeCenterCamera,yeCenterCamera,xeFocusCamera,yeFocusCamera;
int xShareOfs,yShareOfs;

