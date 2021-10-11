
#ifndef __VITAMINA_HPP
#define __VITAMINA_HPP


#ifdef __ICL
 typedef __declspec(align(16)) double real;
#else//of #ifdef __ICL
 typedef double real;
#endif//of #ifdef __ICL

#define align

#include <math.h>


#ifdef FC_DLL
# define FC_DLL_EXPORT __declspec(dllexport)
#else
# ifdef FC_REGISTRATION
#   error "Registration only possible in dll build"
# endif
# define FC_DLL_EXPORT
#endif

#ifndef PI
#define PI (((real)3.14159265358979323846))
#endif

#ifndef ASSERT
#ifdef _DEBUG
#ifdef WIN32
#include <crtdbg.h>
#include <windows.h>
 #define ASS_ERT(x) do{_ASSERTE(x);}while(0)
 
 //#define ASSERT(x)

 #define ASSERT(x)\
 {\
  static int ignore=0;\
  static int ordinary=0;\
  if(ordinary)\
   ASS_ERT(x);\
  else\
  {\
   if((!(x))&&ignore==0)\
   {\
    int result=MessageBox(0,#x\
 "\n\n"\
 "Permanently ignore this assert?\n\n"\
 "\"Yes\" to ignore permanently\n"\
 "\"No\" to continue using advanced assert\n"\
 "\"Cancel\" to convert to standard assert"\
 ,"Assertion failed",MB_YESNOCANCEL|MB_DEFBUTTON2);\
    if(result==IDYES)\
     ignore=1;\
    else if(result==IDCANCEL)\
    {\
     ordinary=1;\
     ASS_ERT(x);\
    }\
    else\
    ASS_ERT(x);\
   }\
  }\
 }\

#else
//#include <assert.h>
#define ASSERT(x) do{assert(x);}while(0)
#endif//of #ifdef WIN32
#else
#define ASSERT(x)
#endif //of #ifdef _DEBUG
#endif//of #ifndef ASSERT

namespace Vitamina
{
 #include "VectorOperationsAligned.inl"//platform specific vector routines
 #include "VectorOperations.inl"

 #ifndef memset
 static inline
 void* memset(void *dest, char c, size_t s)
 {
  unsigned int i;
  for(i=0;i<s;i++)
   ((char*)dest)[i]=c;
  return dest;
 }
 #endif

 #ifndef Min
 template<class T> 
 static inline
 const T& Min(const T& a, const T& b)
 {
  return a < b ? a : b;
 }
 #endif

 #ifndef Max
 template<class T>
 static inline
 const T& Max(const T& a, const T& b)
 {
  return a > b ? a : b;
 }
 #endif


 FC_DLL_EXPORT void MakeOrthonormalBasis(real x[3], real y[3], const real z[3]);
 // Given a unit vector, z, creates 2 arbitrary vectors x, y
 // such that (x,y,z) form a right handed coordinate system.

 static inline
 void NormalizeQuaternion(real Q[4])
 {
  if(Q[0]==((real)0.0) && Q[1]==((real)0.0) && Q[2]==((real)0.0) && Q[3]==((real)0.0))
  {
   Q[0]=((real)1.0);
   return;
  }
  real lengthi=1/sqrt(Q[0]*Q[0]+Q[1]*Q[1]+Q[2]*Q[2]+Q[3]*Q[3]);
  Q[0]*=lengthi;
  Q[1]*=lengthi;
  Q[2]*=lengthi;
  Q[3]*=lengthi;
 }

 static inline
 void QuaternionMultiply(real Q[4],const real Q1[4],const real Q2[4])
 {//Quaternion multiplication: Q=Q1*Q2
  Q[0]=Q1[0]*Q2[0]-Q1[1]*Q2[1]-Q1[2]*Q2[2]-Q1[3]*Q2[3];
  Q[1]=Q1[2]*Q2[3]-Q1[3]*Q2[2]+Q1[0]*Q2[1]+Q1[1]*Q2[0];
  Q[2]=Q1[3]*Q2[1]-Q1[1]*Q2[3]+Q1[0]*Q2[2]+Q1[2]*Q2[0];
  Q[3]=Q1[1]*Q2[2]-Q1[2]*Q2[1]+Q1[0]*Q2[3]+Q1[3]*Q2[0];
 }

 FC_DLL_EXPORT void MatrixToQuaternion(real Q[4],const real m[3][3]);

 static inline
 void QuaternionToMatrix(real m[3][3],const real Q[4])
 {//m:  x_body = m * x_world
  m[0][0]=((real)2.0)*(Q[1]*Q[1]+Q[0]*Q[0])-((real)1.0);
  m[0][1]=((real)2.0)*(Q[1]*Q[2]+Q[3]*Q[0]);
  m[0][2]=((real)2.0)*(Q[3]*Q[1]-Q[2]*Q[0]);
  m[1][0]=((real)2.0)*(Q[1]*Q[2]-Q[3]*Q[0]);
  m[1][1]=((real)2.0)*(Q[2]*Q[2]+Q[0]*Q[0])-((real)1.0);
  m[1][2]=((real)2.0)*(Q[2]*Q[3]+Q[1]*Q[0]);
  m[2][0]=((real)2.0)*(Q[3]*Q[1]+Q[2]*Q[0]);
  m[2][1]=((real)2.0)*(Q[2]*Q[3]-Q[1]*Q[0]);
  m[2][2]=((real)2.0)*(Q[3]*Q[3]+Q[0]*Q[0])-((real)1.0);
 }

 static inline
 void QuaternionToMatrix(real m[4][4],const real Q[4])
 {//m:  x_body = m * x_world
  m[0][0]=((real)2.0)*(Q[1]*Q[1]+Q[0]*Q[0])-((real)1.0);
  m[0][1]=((real)2.0)*(Q[1]*Q[2]+Q[3]*Q[0]);
  m[0][2]=((real)2.0)*(Q[3]*Q[1]-Q[2]*Q[0]);
  m[1][0]=((real)2.0)*(Q[1]*Q[2]-Q[3]*Q[0]);
  m[1][1]=((real)2.0)*(Q[2]*Q[2]+Q[0]*Q[0])-((real)1.0);
  m[1][2]=((real)2.0)*(Q[2]*Q[3]+Q[1]*Q[0]);
  m[2][0]=((real)2.0)*(Q[3]*Q[1]+Q[2]*Q[0]);
  m[2][1]=((real)2.0)*(Q[2]*Q[3]-Q[1]*Q[0]);
  m[2][2]=((real)2.0)*(Q[3]*Q[3]+Q[0]*Q[0])-((real)1.0);
 }

 static inline
 void QuaternionToMatrix(real t[16],const real Q[4])
 {
  t[0]=((real)2.0)*(Q[1]*Q[1]+Q[0]*Q[0])-((real)1.0);
  t[1]=((real)2.0)*(Q[1]*Q[2]+Q[3]*Q[0]);
  t[2]=((real)2.0)*(Q[3]*Q[1]-Q[2]*Q[0]);
  t[4]=((real)2.0)*(Q[1]*Q[2]-Q[3]*Q[0]);
  t[5]=((real)2.0)*(Q[2]*Q[2]+Q[0]*Q[0])-((real)1.0);
  t[6]=((real)2.0)*(Q[2]*Q[3]+Q[1]*Q[0]);
  t[8]=((real)2.0)*(Q[3]*Q[1]+Q[2]*Q[0]);
  t[9]=((real)2.0)*(Q[2]*Q[3]-Q[1]*Q[0]);
  t[10]=((real)2.0)*(Q[3]*Q[3]+Q[0]*Q[0])-((real)1.0);
 }

 static inline
 void CuboidMomentsOfInertia(real TOI[3],real mass,real Dx,real Dy,real Dz)
 {//D is length from edge to edge
  TOI[0]=1.0/((real)12.0)*mass*(Dy*Dy+Dz*Dz);
  TOI[1]=1.0/((real)12.0)*mass*(Dx*Dx+Dz*Dz);
  TOI[2]=1.0/((real)12.0)*mass*(Dx*Dx+Dy*Dy);
 }

 static inline
 void CuboidMomentsOfInertia(real TOI[3],real *pmass,real density,real Dx,real Dy,real Dz)
 {//D is length from edge to edge
  *pmass=Dx*Dy*Dz*density;
  TOI[0]=1.0/((real)12.0)**pmass*(Dy*Dy+Dz*Dz);
  TOI[1]=1.0/((real)12.0)**pmass*(Dx*Dx+Dz*Dz);
  TOI[2]=1.0/((real)12.0)**pmass*(Dx*Dx+Dy*Dy);
 }

 static inline
 void CylinderMomentsOfInertia(real TOI[3],real mass,real R,real H)
 {//Cylinder axle is alond z.
  TOI[0]=TOI[1]=mass*(((real)1.0)/((real)2.0)*R*R+1/((real)12.0)*H*H);
  TOI[2]=mass*((real)1.0)/((real)2.0)*R*R;
 }

 static inline
 void CylinderMomentsOfInertia(real TOI[3],real *pmass,real density,real R,real H)
 {//Cylinder axle is alond z.
  *pmass=PI*R*R*H*density;
  TOI[0]=TOI[1]=*pmass*(((real)1.0)/((real)2.0)*R*R+1/((real)12.0)*H*H);
  TOI[2]=*pmass*((real)1.0)/((real)2.0)*R*R;
 }

 static inline
 void ToWorld(real vector_world[3],const real vector_body[3],const real m[16])
 {
  vector_world[0]=m[0]*vector_body[0]+m[4]*vector_body[1]+m[8] *vector_body[2];
  vector_world[1]=m[1]*vector_body[0]+m[5]*vector_body[1]+m[9] *vector_body[2];
  vector_world[2]=m[2]*vector_body[0]+m[6]*vector_body[1]+m[10]*vector_body[2];
 }

 static inline
 void ToWorld(real vector_world[3],const real vector_body[3],const float m[16])
 {
  vector_world[0]=m[0]*vector_body[0]+m[4]*vector_body[1]+m[8] *vector_body[2];
  vector_world[1]=m[1]*vector_body[0]+m[5]*vector_body[1]+m[9] *vector_body[2];
  vector_world[2]=m[2]*vector_body[0]+m[6]*vector_body[1]+m[10]*vector_body[2];
 }

 static inline
 void ToWorld(real vector_world[3],const real vector_body[3],const real m[3][3])
 {
  vector_world[0]=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
  vector_world[1]=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
  vector_world[2]=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
 }

 /*
 static inline
 void ToWorld(real vector_world[3],const real vector_body[3],const real m[4][4])
 {
  vector_world[0]=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
  vector_world[1]=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
  vector_world[2]=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
 }
 */
 static inline
 void ToWorldAdd(real vector_world[3],const real vector_body[3],const real m[16])
 {
  vector_world[0]+=m[0]*vector_body[0]+m[4]*vector_body[1]+m[8] *vector_body[2];
  vector_world[1]+=m[1]*vector_body[0]+m[5]*vector_body[1]+m[9] *vector_body[2];
  vector_world[2]+=m[2]*vector_body[0]+m[6]*vector_body[1]+m[10]*vector_body[2];
 }

 static inline
 void ToWorldAdd(real vector_world[3],const real vector_body[3],const real m[3][3])
 {
  vector_world[0]+=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
  vector_world[1]+=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
  vector_world[2]+=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
 }

 /*
 static inline
 void ToWorldAdd(real vector_world[3],const real vector_body[3],const real m[4][4])
 {
  vector_world[0]+=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
  vector_world[1]+=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
  vector_world[2]+=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
 }
 */
 static inline
 void ToBody(real vector_body[3],const real vector_world[3],const real m[16])
 {
  ASSERT(vector_body!=vector_world);
  vector_body[0]=m[0]*vector_world[0]+m[1]*vector_world[1]+m[2] *vector_world[2];
  vector_body[1]=m[4]*vector_world[0]+m[5]*vector_world[1]+m[6] *vector_world[2];
  vector_body[2]=m[8]*vector_world[0]+m[9]*vector_world[1]+m[10]*vector_world[2];
 }

 static inline
 void ToBody(real vector_body[3],const real vector_world[3],const real m[3][3])
 {
  vector_body[0]=m[0][0]*vector_world[0]+m[0][1]*vector_world[1]+m[0][2]*vector_world[2];
  vector_body[1]=m[1][0]*vector_world[0]+m[1][1]*vector_world[1]+m[1][2]*vector_world[2];
  vector_body[2]=m[2][0]*vector_world[0]+m[2][1]*vector_world[1]+m[2][2]*vector_world[2];
 }

 /*
 static inline
 void ToBody(real vector_body[3],const real vector_world[3],const real m[4][4])
 {
  vector_body[0]=m[0][0]*vector_world[0]+m[0][1]*vector_world[1]+m[0][2]*vector_world[2];
  vector_body[1]=m[1][0]*vector_world[0]+m[1][1]*vector_world[1]+m[1][2]*vector_world[2];
  vector_body[2]=m[2][0]*vector_world[0]+m[2][1]*vector_world[1]+m[2][2]*vector_world[2];
 }
 */
 static inline
 void apply_omega(real Q[4], const real w[4], real time_step)
 {
  real angle,length;
  length=Magnitude(w);
  if(length==((real)0.0)) return;//No angular velocity
  angle=length*time_step;
  real dQ[4];
  dQ[0]=cos(angle/((real)2.0));
  real sinbylength=sin(angle/((real)2.0))/length;
  dQ[1]=sinbylength*w[0];
  dQ[2]=sinbylength*w[1];
  dQ[3]=sinbylength*w[2];
  const real Qp[4]={Q[0],Q[1],Q[2],Q[3]};
  //m_Q = dQ * Qp
  QuaternionMultiply(Q,dQ,Qp);
 }

 static inline
 void invert3x3_sym(real ai[3][3],real a[3][3])
 {
  real a01s=a[0][1]*a[0][1];
  real a02s=a[0][2]*a[0][2];
  real a0212=a[0][2]*a[1][2];
  real subt1=a[1][1]*a[2][2]-a[1][2]*a[1][2];
  real F=1.0/(-a02s*a[1][1]-a01s*a[2][2]+2.0*a[0][1]*a0212+subt1*a[0][0]);
  real C=(a[0][1]*a[2][2]-a0212)*F;
  real D=(a[0][0]*a[1][2]-a[0][1]*a[0][2])*F;
  real E=(a[0][1]*a[1][2]-a[0][2]*a[1][1])*F;
  ai[0][0]=subt1*F;
  ai[1][1]=(a[0][0]*a[2][2]-a02s)*F;
  ai[2][2]=(a[0][0]*a[1][1]-a01s)*F;
  ai[0][1]=ai[1][0]=-C;
  ai[0][2]=ai[2][0]=E;
  ai[1][2]=ai[2][1]=-D;
 }

 static inline
 void solve3x3_cholesky(real s[3],const real A[3][3],const real b[3])
 {//for PD matices only. 21 multiplications
  real m[3][3];
  Copy(m[0],A[0]);
  Copy(m[1],A[1]);
  Copy(m[2],A[2]);

  //the sqrt-free cholesky decomposition
  int i,j,k;
  real tem;
  for(k=0;k<2;k++)
  for(i=2;i>k;i--)
  {
   tem=m[i][k]/m[k][k];
   for(j=i;j>k;j--)m[i][j]-=m[j][k]*tem;
   m[i][k]=tem;
  }
  //back substitution
  Copy(s,b);
  for(k=0;k<3;k++)
  for(i=k+1;i<3;i++)
   s[i]-=m[i][k]*s[k];
  for(k=0;k<3;k++)
   s[k]=s[k]/m[k][k];
  for(k=2;k>-1;k--)
  for(i=k-1;i>-1;i--)
   s[i]-=m[k][i]*s[k];

#ifdef _DEBUG
  //check
  real res[3];
  Multiply(res,A,s);
  SubtractFrom(res,b);
  ASSERT(res[0]<1e-6&&res[0]>-1e-6);
  ASSERT(res[1]<1e-6&&res[1]>-1e-6);
  ASSERT(res[2]<1e-6&&res[2]>-1e-6);
#endif//of #ifdef _DEBUG
 }

 static inline
 void invert3x3_sym(real ai[4][4],real a[4][4])
 {//24 multiplications
  real a01s=a[0][1]*a[0][1];
  real a02s=a[0][2]*a[0][2];
  real a0212=a[0][2]*a[1][2];
  real subt1=a[1][1]*a[2][2]-a[1][2]*a[1][2];
  real F=1.0/(-a02s*a[1][1]-a01s*a[2][2]+2.0*a[0][1]*a0212+subt1*a[0][0]);
  real C=(a[0][1]*a[2][2]-a0212)*F;
  real D=(a[0][0]*a[1][2]-a[0][1]*a[0][2])*F;
  real E=(a[0][1]*a[1][2]-a[0][2]*a[1][1])*F;
  ai[0][0]=subt1*F;
  ai[1][1]=(a[0][0]*a[2][2]-a02s)*F;
  ai[2][2]=(a[0][0]*a[1][1]-a01s)*F;
  ai[0][1]=ai[1][0]=-C;
  ai[0][2]=ai[2][0]=E;
  ai[1][2]=ai[2][1]=-D;
 }

 static inline
 int invert3x3(real ai[3][3],const real a[3][3],real tolerance=1e-6)
 {//28 multiplications
  int i,j;

  ai[0][0]=a[1][1]*a[2][2]-a[1][2]*a[2][1];
  ai[0][1]=a[0][2]*a[2][1]-a[0][1]*a[2][2];
  ai[0][2]=a[0][1]*a[1][2]-a[0][2]*a[1][1];
  ai[1][0]=a[1][2]*a[2][0]-a[1][0]*a[2][2];
  ai[1][1]=a[0][0]*a[2][2]-a[0][2]*a[2][0];
  ai[1][2]=a[0][2]*a[1][0]-a[0][0]*a[1][2];
  ai[2][0]=a[1][0]*a[2][1]-a[1][1]*a[2][0];
  ai[2][1]=a[0][1]*a[2][0]-a[0][0]*a[2][1];
  ai[2][2]=a[0][0]*a[1][1]-a[0][1]*a[1][0];
 
  real determinant=a[0][0]*ai[0][0]+a[0][1]*ai[1][0]+ a[0][2]*ai[2][0];
 
  if(fabs(determinant)<=tolerance) return 0;
  real determinanti=1.0/determinant;
  for (i=0;i<3;i++)
  for (j=0;j<3;j++)
   ai[i][j]*=determinanti;
  return 1;
 }

 static inline
 int IS_EQ(real a,real b,real absprec)
 {
  ASSERT(absprec>=0);
  ASSERT(fabs((a)-(b))<=absprec);
  return (fabs((a)-(b))<=absprec);
 }

 static inline
 int IS_EQ(real a,real b,real absprec,real relprec)
 {
  ASSERT(absprec>=0);
  ASSERT(relprec>=0);
  real prec= fabs(a)*relprec+absprec;
  //ASSERT(fabs(a-b)<=prec);
  return (fabs(a-b)<=prec);
 }

 static inline
 int IS_LT(real a,real b,real absprec)
 {
  ASSERT(absprec>=0);
  return (b-a)>=absprec;
 }

 static inline
 int IS_GT(real a,real b,real absprec)
 {
  ASSERT(absprec>=0);
  return (a-b)>=absprec;
 }

 #ifndef FC_REGISTRATION //these matrices do not work in the dll
 extern real front_to_x_top_to_y[3][3];
 extern real front_to_x_top_to_z[3][3];
 extern real front_to_x_top_to_minus_y[3][3];
 extern real front_to_x_top_to_minus_z[3][3];
 extern real front_to_y_top_to_x[3][3];
 extern real front_to_y_top_to_z[3][3];
 extern real front_to_y_top_to_minus_x[3][3];
 extern real front_to_y_top_to_minus_z[3][3];
 extern real front_to_z_top_to_x[3][3];
 extern real front_to_z_top_to_y[3][3];
 extern real front_to_z_top_to_minus_x[3][3];
 extern real front_to_z_top_to_minus_y[3][3];
 extern real front_to_minus_x_top_to_y[3][3];
 extern real front_to_minus_x_top_to_z[3][3];
 extern real front_to_minus_x_top_to_minus_y[3][3];
 extern real front_to_minus_x_top_to_minus_z[3][3];
 extern real front_to_minus_y_top_to_x[3][3];
 extern real front_to_minus_y_top_to_z[3][3];
 extern real front_to_minus_y_top_to_minus_x[3][3];
 extern real front_to_minus_y_top_to_minus_z[3][3];
 extern real front_to_minus_z_top_to_x[3][3];
 extern real front_to_minus_z_top_to_y[3][3];
 extern real front_to_minus_z_top_to_minus_x[3][3];
 extern real front_to_minus_z_top_to_minus_y[3][3];
 #endif//of #ifndef FC_REGISTRATION 
}//of namespace Vitamina

#endif//of #ifndef __VITAMINA_HPP