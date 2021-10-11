
#include "StdAfx.h"
#ifndef __VITAMINA_CPP
#define __VITAMINA_CPP

#include "Vitamina.hpp"

namespace Vitamina
{
 FC_DLL_EXPORT
 void MakeOrthonormalBasis(real x[3], real y[3], const real z[3])
 {
  ASSERT(fabs(DotProduct(z,z)-1) < 1E-6);
  if(fabs(z[0]) > fabs(z[1]))
  { // |z.e1| > |z.e2|
   // x=e2 cross z
   const real s=1/sqrt(z[2]*z[2]+z[0]*z[0]);
   x[0]=z[2]*s;
   x[1]=0;
   x[2]=-z[0]*s;
   // y=z cross x
   y[0]=z[1]*x[2];
   y[1]=z[2]*x[0] - z[0]*x[2];
   y[2]=-z[1]*x[0];
  }
  else
  {
   // |z.e1| <= |z.e2|
   // x=e1 cross z
   const real s=1/sqrt(z[2]*z[2]+z[1]*z[1]);
   x[0]=0;
   x[1]=-z[2]*s;
   x[2]=z[1]*s;
   // y=z cross x
   y[0]=z[1]*x[2] - z[2]*x[1];
   y[1]=-z[0]*x[2];
   y[2]=z[0]*x[1];
  }
  ASSERT(fabs(DotProduct(x,x)-1) < 1E-6);
  ASSERT(fabs(DotProduct(y,y)-1) < 1E-6);
  ASSERT(fabs(DotProduct(x,y)) < 1E-6);
  ASSERT(fabs(DotProduct(y,z)) < 1E-6);
  ASSERT(fabs(DotProduct(z,x)) < 1E-6);
 /*
#ifdef _DEBUG
  real cp[3];
  //right-handedness check
  CrossProduct(cp,m[0],m[1]);
  ASSERT(IS_EQ(cp[0],m[2][0],1e-6));
  ASSERT(IS_EQ(cp[1],m[2][1],1e-6));
  ASSERT(IS_EQ(cp[2],m[2][2],1e-6));

  //orthonormality check
  ASSERT(IS_EQ(Magnitude(m[0]),1.0,1e-6));
  ASSERT(IS_EQ(Magnitude(m[1]),1.0,1e-6));
  ASSERT(IS_EQ(Magnitude(m[2]),1.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[0],m[1]),0.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[1],m[2]),0.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[2],m[0]),0.0,1e-6));
#endif//of #ifdef _DEBUG
 */
 }

 FC_DLL_EXPORT
 void MatrixToQuaternion(real Q[4],const real m[3][3])
 {//m: x_body=m*x_world
  real trace,sr;

#ifdef _DEBUG
  real cp[3];
  //right-handedness check
  CrossProduct(cp,m[0],m[1]);
  ASSERT(IS_EQ(cp[0],m[2][0],1e-6));
  ASSERT(IS_EQ(cp[1],m[2][1],1e-6));
  ASSERT(IS_EQ(cp[2],m[2][2],1e-6));

  //orthonormality check
  ASSERT(IS_EQ(Magnitude(m[0]),1.0,1e-6));
  ASSERT(IS_EQ(Magnitude(m[1]),1.0,1e-6));
  ASSERT(IS_EQ(Magnitude(m[2]),1.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[0],m[1]),0.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[1],m[2]),0.0,1e-6));
  ASSERT(IS_EQ(DotProduct(m[2],m[0]),0.0,1e-6));
#endif//of #ifdef _DEBUG
 
  trace=m[0][0]+m[1][1]+m[2][2];
  if(trace>=0)
  {
   sr=sqrt(trace+1);
   Q[0]=-((real)0.5)*sr;
   sr=((real)0.5)/sr;
   Q[1]=(m[2][1]-m[1][2])*sr;
   Q[2]=(m[0][2]-m[2][0])*sr;
   Q[3]=(m[1][0]-m[0][1])*sr;
  }
  else
  {
   int i=0;
   if(m[1][1]>m[0][0])
    i=1;
   if(m[2][2]>m[i][i])
    i=2;
   switch(i)
   {
   case 0:
    {
     sr=sqrt((m[0][0]-(m[1][1]+m[2][2]))+1);
     Q[1]=((real)0.5)*sr;
     sr=((real)0.5)/sr;
     Q[2]=(m[0][1]+m[1][0])*sr;
     Q[3]=(m[2][0]+m[0][2])*sr;
     Q[0]=-(m[2][1]-m[1][2])*sr;
     break;
    }
   case 1:
    {
     sr=sqrt((m[1][1]-(m[2][2]+m[0][0]))+1);
     Q[2]=((real)0.5)*sr;
     sr=((real)0.5)/sr;
     Q[3]=(m[1][2]+m[2][1])*sr;
     Q[1]=(m[0][1]+m[1][0])*sr;
     Q[0]=-(m[0][2]-m[2][0])*sr;
     break;
    }
   case 2:
    {
     sr=sqrt((m[2][2]-(m[0][0]+m[1][1]))+1);
     Q[3]=((real)0.5)*sr;
     sr=((real)0.5)/sr;
     Q[1]=(m[2][0]+m[0][2])*sr;
     Q[2]=(m[1][2]+m[2][1])*sr;
     Q[0]=-(m[1][0]-m[0][1])*sr;
     break;
    }
   }
  }
 }

#ifndef FC_REGISTRATION //these matrices do not work in the dll
//{initial rotations
 real front_to_x_top_to_y[3][3]=
 {{ 1, 0, 0},
  { 0, 1, 0},
  { 0, 0, 1}};
 real front_to_x_top_to_z[3][3]=
 {{ 1, 0, 0},
  { 0, 0, 1},
  { 0,-1, 0}};
 real front_to_x_top_to_minus_y[3][3]=
 {{ 1, 0, 0},
  { 0,-1, 0},
  { 0, 0,-1}};
 real front_to_x_top_to_minus_z[3][3]=
 {{ 1, 0, 0},
  { 0, 0,-1},
  { 0, 1, 0}};

 real front_to_y_top_to_x[3][3]=
 {{ 0, 1, 0},
  { 1, 0, 0},
  { 0, 0,-1}};
 real front_to_y_top_to_z[3][3]=
 {{ 0, 1, 0},
  { 0, 0, 1},
  { 1, 0, 0}};
 real front_to_y_top_to_minus_x[3][3]=
 {{ 0, 1, 0},
  {-1, 0, 0},
  { 0, 0, 1}};
 real front_to_y_top_to_minus_z[3][3]=
 {{ 0, 1, 0},
  { 0, 0,-1},
  {-1, 0, 0}};

 real front_to_z_top_to_x[3][3]=
 {{ 0, 0, 1},
  { 1, 0, 0},
  { 0, 1, 0}};
 real front_to_z_top_to_y[3][3]=
 {{ 0, 0, 1},
  { 0, 1, 0},
  {-1, 0, 0}};
 real front_to_z_top_to_minus_x[3][3]=
 {{ 0, 0, 1},
  {-1, 0, 0},
  { 0,-1, 0}};
 real front_to_z_top_to_minus_y[3][3]=
 {{ 0, 0, 1},
  { 0,-1, 0},
  { 1, 0, 0}};


 real front_to_minus_x_top_to_y[3][3]=
 {{-1, 0, 0},
  { 0, 1, 0},
  { 0, 0,-1}};
 real front_to_minus_x_top_to_z[3][3]=
 {{-1, 0, 0},
  { 0, 0, 1},
  { 0, 1, 0}};
 real front_to_minus_x_top_to_minus_y[3][3]=
 {{-1, 0, 0},
  { 0,-1, 0},
  { 0, 0, 1}};
 real front_to_minus_x_top_to_minus_z[3][3]=
 {{-1, 0, 0},
  { 0, 0,-1},
  { 0,-1, 0}};

 real front_to_minus_y_top_to_x[3][3]=
 {{ 0,-1, 0},
  { 1, 0, 0},
  { 0, 0, 1}};
 real front_to_minus_y_top_to_z[3][3]=
 {{ 0,-1, 0},
  { 0, 0, 1},
  {-1, 0, 0}};
 real front_to_minus_y_top_to_minus_x[3][3]=
 {{ 0,-1, 0},
  {-1, 0, 0},
  { 0, 0,-1}};
 real front_to_minus_y_top_to_minus_z[3][3]=
 {{ 0,-1, 0},
  { 0, 0,-1},
  { 1, 0, 0}};

 real front_to_minus_z_top_to_x[3][3]=
 {{ 0, 0,-1},
  { 1, 0, 0},
  { 0,-1, 0}};
 real front_to_minus_z_top_to_y[3][3]=
 {{ 0, 0,-1},
  { 0, 1, 0},
  { 1, 0, 0}};
 real front_to_minus_z_top_to_minus_x[3][3]=
 {{ 0, 0,-1},
  {-1, 0, 0},
  { 0, 1, 0}};
 real front_to_minus_z_top_to_minus_y[3][3]=
 {{ 0, 0,-1},
  { 0,-1, 0},
  {-1, 0, 0}};
//}initial rotations
#endif//of #ifndef FC_REGISTRATION 
}//namespace Vitamina
#endif//of #ifndef __VITAMINA_CPP
