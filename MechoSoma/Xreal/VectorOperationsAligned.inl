
/*Vector routines to be hardware optimized.
1)Pointers passed to these routines are guaranteed to
  be alligned at 16 byte boundary.
2)If a function from this list operates on 3D vectors
  they are be padded at the end by an unused 4th element.

V=0                 fcmIdent                  v={0,0,0,1}
                    fcmZero                   v={0,0,0,0}
V=n                 fcmSetTo                  v={n,n,n,n}
V=V                 fcmCopy                   v=v0
V+=V                fcmAddTo                  v+=v0
V=V+V               fcmAdd                    v=v0+v1
V=V+V+V             fcmTrippleAdd             v=v0+v1+v2
V-=V                fcmSubtractFrom           v-=v0
V=V-V               fcmSubtract               v=v0-v1
V*=n                fcmSelfScale              v*=n
V=V*n               fcmScale                  v=v0*n
V+=V*n              fcmAddScaledTo            v+=v0*n
V=V+V*n             fcmAddScale               v=v0+(v1*n)
V=V*n+V*n+V*n       fcmTripleScaledSum        v=v0*n0+v1*n1+v2*n2
V=V*V*n+V*V*n+V*V*n fcmTripleSquaredScaledSum v=v0*v0*n0+v1*v1*n1+v2*v2*n2
V+=(V-V)*n          fcmAddDiffScaledTo        v+=(v0-v1)*n
V=n*(V-((V*n+V)*n)) fcmSpecial1               v=n0*(v0-((v1*n1+v2)*n2))

                    fcmCrossProduct           v=v0xv1
                    fcmDotProduct             v0.v1
                    fcmMagnitude              length
                    fcmNormalize              so that magnitude(v)==1
                    fcmNormalize              v=normalize(v0)
                    fcmToBody                 vector_body=m*vector_world
                    fcmToWorld                vector_world=m^T*vector_body
                    fcmToWorldAdd             vector_world+=m^T*vector_body
*/

#define FCM_STATIC_INLINE static __forceinline

FCM_STATIC_INLINE
void fcmIdent4(real v[4])
{//v={0,0,0,1}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
 v[3]=1.0;
}

FCM_STATIC_INLINE
void fcmZero(real v[4])
{//v={0,0,0}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
}

FCM_STATIC_INLINE
void fcmZero4(real v[4])
{//v={0,0,0,0}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
 v[3]=0.0;
}

FCM_STATIC_INLINE
void fcmSetTo(real v[4],real n)
{//v={n,n,n}
 v[0]=n;
 v[1]=n;
 v[2]=n;
}

FCM_STATIC_INLINE
void fcmSetTo4(real v[4],real n)
{//v={n,n,n,n}
 v[0]=n;
 v[1]=n;
 v[2]=n;
 v[3]=n;
}

FCM_STATIC_INLINE
void fcmCopy(real v[4],const real v0[4])
{//v=v0
 v[0]=v0[0];
 v[1]=v0[1];
 v[2]=v0[2];
}

FCM_STATIC_INLINE
void fcmCopy4(real v[4],const real v0[4])
{//v=v0
 v[0]=v0[0];
 v[1]=v0[1];
 v[2]=v0[2];
 v[3]=v0[3];
}

FCM_STATIC_INLINE
void fcmAddTo(real v[4],const real v0[4])
{//v+=v0
 v[0]+=v0[0];
 v[1]+=v0[1];
 v[2]+=v0[2];
}

FCM_STATIC_INLINE
void fcmAddTo4(real v[4],const real v0[4])
{//v+=v0
 v[0]+=v0[0];
 v[1]+=v0[1];
 v[2]+=v0[2];
 v[3]+=v0[3];
}

FCM_STATIC_INLINE
void fcmAdd(real v[4],const real v0[4],const real v1[4])
{//v=v0+v1
 v[0]=v0[0]+v1[0];
 v[1]=v0[1]+v1[1];
 v[2]=v0[2]+v1[2];
}

FCM_STATIC_INLINE
void fcmAdd4(real v[4],const real v0[4],const real v1[4])
{//v=v0+v1
 v[0]=v0[0]+v1[0];
 v[1]=v0[1]+v1[1];
 v[2]=v0[2]+v1[2];
 v[3]=v0[3]+v1[3];
}

FCM_STATIC_INLINE
void fcmTrippleAdd(real v[4],const real v0[4],const real v1[4],const real v2[4])
{//v=v0+v1+v2
 v[0]=v0[0]+v1[0]+v2[0];
 v[1]=v0[1]+v1[1]+v2[1];
 v[2]=v0[2]+v1[2]+v2[2];
}

FCM_STATIC_INLINE
void fcmTrippleAdd4(real v[4],const real v0[4],const real v1[4],const real v2[4])
{//v=v0+v1+v2
 v[0]=v0[0]+v1[0]+v2[0];
 v[1]=v0[1]+v1[1]+v2[1];
 v[2]=v0[2]+v1[2]+v2[2];
 v[3]=v0[3]+v1[3]+v2[3];
}

FCM_STATIC_INLINE
void fcmSubtractFrom(real v[4],const real v0[4])
{//v-=v0
 v[0]-=v0[0];
 v[1]-=v0[1];
 v[2]-=v0[2];
}

FCM_STATIC_INLINE
void fcmSubtractFrom4(real v[4],const real v0[4])
{//v-=v0
 v[0]-=v0[0];
 v[1]-=v0[1];
 v[2]-=v0[2];
 v[3]-=v0[3];
}

FCM_STATIC_INLINE
void fcmSubtract(real v[4],const real v0[4],const real v1[4])
{//v=v0-v1
 v[0]=v0[0]-v1[0];
 v[1]=v0[1]-v1[1];
 v[2]=v0[2]-v1[2];
}

FCM_STATIC_INLINE
void fcmSubtract4(real v[4],const real v0[4],const real v1[4])
{//v=v0-v1
 v[0]=v0[0]-v1[0];
 v[1]=v0[1]-v1[1];
 v[2]=v0[2]-v1[2];
 v[3]=v0[3]-v1[3];
}

FCM_STATIC_INLINE
void fcmSelfScale(real v[4],real n)
{//v*=n
 v[0]*=n;
 v[1]*=n;
 v[2]*=n;
}

FCM_STATIC_INLINE
void fcmSelfScale4(real v[4],real n)
{//v*=n
 v[0]*=n;
 v[1]*=n;
 v[2]*=n;
 v[3]*=n;
}

FCM_STATIC_INLINE
void fcmScale(real v[4],const real v0[4],real n)
{//v=v0*n
 v[0]=v0[0]*n;
 v[1]=v0[1]*n;
 v[2]=v0[2]*n;
}

FCM_STATIC_INLINE
void fcmScale4(real v[4],const real v0[4],real n)
{//v=v0*n
 v[0]=v0[0]*n;
 v[1]=v0[1]*n;
 v[2]=v0[2]*n;
 v[3]=v0[3]*n;
}

FCM_STATIC_INLINE
void fcmAddScaledTo(real v[4],const real v0[4],real n)
{//v+=v0*n
 v[0]+=v0[0]*n;
 v[1]+=v0[1]*n;
 v[2]+=v0[2]*n;
}

FCM_STATIC_INLINE
void fcmAddScaledTo4(real v[4],const real v0[4],real n)
{//v+=v0*n
 v[0]+=v0[0]*n;
 v[1]+=v0[1]*n;
 v[2]+=v0[2]*n;
 v[3]+=v0[3]*n;
}

FCM_STATIC_INLINE
void fcmCombo(real v[4],const real v0[4],const real v1[4],real n)
{//v=v0+v1*n
 v[0]=v0[0]+v1[0]*n;
 v[1]=v0[1]+v1[1]*n;
 v[2]=v0[2]+v1[2]*n;
}

FCM_STATIC_INLINE
void fcmCombo4(real v[4],const real v0[4],const real v1[4],real n)
{//v=v0+v1*n
 v[0]=v0[0]+v1[0]*n;
 v[1]=v0[1]+v1[1]*n;
 v[2]=v0[2]+v1[2]*n;
 v[3]=v0[3]+v1[3]*n;
}

FCM_STATIC_INLINE
void fcmTripleScaledSum(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*n0+v1*n1+v2*n2
 v[0]=v0[0]*n0+v1[0]*n1+v2[0]*n2;
 v[1]=v0[1]*n0+v1[1]*n1+v2[1]*n2;
 v[2]=v0[2]*n0+v1[2]*n1+v2[2]*n2;
}

FCM_STATIC_INLINE
void fcmTripleScaledSum4(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*n0+v1*n1+v2*n2
 v[0]=v0[0]*n0+v1[0]*n1+v2[0]*n2;
 v[1]=v0[1]*n0+v1[1]*n1+v2[1]*n2;
 v[2]=v0[2]*n0+v1[2]*n1+v2[2]*n2;
 v[3]=v0[3]*n0+v1[3]*n1+v2[3]*n2;
}

FCM_STATIC_INLINE
void fcmTripleSquaredScaledSum(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*v0*n0+v1*v1*n1+v2*v2*n2
 v[0]=v0[0]*v0[0]*n0+v1[0]*v1[0]*n1+v2[0]*v2[0]*n2;
 v[1]=v0[1]*v0[1]*n0+v1[1]*v1[1]*n1+v2[1]*v2[1]*n2;
 v[2]=v0[2]*v0[2]*n0+v1[2]*v1[2]*n1+v2[2]*v2[2]*n2;
}

FCM_STATIC_INLINE
void fcmTripleSquaredScaledSum4(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*v0*n0+v1*v1*n1+v2*v2*n2
 v[0]=v0[0]*v0[0]*n0+v1[0]*v1[0]*n1+v2[0]*v2[0]*n2;
 v[1]=v0[1]*v0[1]*n0+v1[1]*v1[1]*n1+v2[1]*v2[1]*n2;
 v[2]=v0[2]*v0[2]*n0+v1[2]*v1[2]*n1+v2[2]*v2[2]*n2;
 v[3]=v0[3]*v0[3]*n0+v1[3]*v1[3]*n1+v2[3]*v2[3]*n2;
}

FCM_STATIC_INLINE
void fcmAddDiffScaledTo(real v[4],const real v0[4],const real v1[4],real n)
{//v+=(v0-v1)*n
 v[0]+=(v0[0]-v1[0])*n;
 v[1]+=(v0[1]-v1[1])*n;
 v[2]+=(v0[2]-v1[2])*n;
}

FCM_STATIC_INLINE
void fcmAddDiffScaledTo4(real v[4],const real v0[4],const real v1[4],real n)
{//v+=(v0-v1)*n
 v[0]+=(v0[0]-v1[0])*n;
 v[1]+=(v0[1]-v1[1])*n;
 v[2]+=(v0[2]-v1[2])*n;
 v[3]+=(v0[3]-v1[3])*n;
}

FCM_STATIC_INLINE
void fcmSpecial(real v[4],real n0,const real v0[4],const real v1[4],real n1,const real v2[4],real n2)
{//v=n0*(v0-((v1*n1+v2)*n2))
 v[0]=n0*(v0[0]-(v1[0]*n1+v2[0])*n2);
 v[1]=n0*(v0[1]-(v1[1]*n1+v2[1])*n2);
 v[2]=n0*(v0[2]-(v1[2]*n1+v2[2])*n2);
}

FCM_STATIC_INLINE
void fcmCrossProduct(real v[4],const real v0[4],const real v1[4])
{//v=v0xv1
 v[0]=v0[1]*v1[2]-v0[2]*v1[1];
 v[1]=v0[2]*v1[0]-v0[0]*v1[2];
 v[2]=v0[0]*v1[1]-v0[1]*v1[0];
}

FCM_STATIC_INLINE
real fcmDotProduct(const real v0[4],const real v1[4])
{//=v0.v1
 return v0[0]*v1[0]+v0[1]*v1[1]+v0[2]*v1[2];
}

FCM_STATIC_INLINE
real fcmMagnitude(const real v0[4])
{//Length
 return sqrt(fcmDotProduct(v0,v0));
}

FCM_STATIC_INLINE
void fcmNormalize(real v[4])
{//so that Magnitude(v)==1
 const real l=fcmMagnitude(v);
 if(l!=0.0)
 {
  const real li=1.0/l;
  v[0]*=li;
  v[1]*=li;
  v[2]*=li;
 }
}

FCM_STATIC_INLINE
void fcmNormalize(real v[4],const real v0[4])
{//v=Normalize(v0)
 const real l=fcmMagnitude(v0);
 if(l!=0.0)
 {
  const real li=1.0/l;
  v[0]=v0[0]*li;
  v[1]=v0[1]*li;
  v[2]=v0[2]*li;
 }
}

FCM_STATIC_INLINE
void fcmToBody(real vector_body[4],const real vector_world[4],const real m[4][4])
{//vector_body=m*vector_world
 vector_body[0]=fcmDotProduct(m[0],vector_world);
 vector_body[1]=fcmDotProduct(m[1],vector_world);
 vector_body[2]=fcmDotProduct(m[2],vector_world);
}

FCM_STATIC_INLINE
void fcmToWorld(real vector_world[4],const real vector_body[4],const real m[4][4])
{//vector_world=m^T*vector_body
 vector_world[0]=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
 vector_world[1]=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
 vector_world[2]=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
}

FCM_STATIC_INLINE
void fcmToWorldAdd(real vector_world[4],const real vector_body[4],const real m[4][4])
{//vector_world+=m^T*vector_body
 vector_world[0]+=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
 vector_world[1]+=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
 vector_world[2]+=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
}

#undef FCM_STATIC_INLINE
