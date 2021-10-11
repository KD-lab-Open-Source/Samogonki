
/*Unaligned and unpadded vector routines to be hardware optimized.

V=0                 Ident                  v={0,0,0,1}
                    Zero                   v={0,0,0,0}
V=n                 SetTo                  v={n,n,n,n}
V=V                 Copy                   v=v0
V+=V                AddTo                  v+=v0
V=V+V               Add                    v=v0+v1
V=V+V+V             TrippleAdd             v=v0+v1+v2
V-=V                SubtractFrom           v-=v0
V=V-V               Subtract               v=v0-v1
V*=n                SelfScale              v*=n
V=V*n               Scale                  v=v0*n
V+=V*n              AddScaledTo            v+=v0*n
V=V+V*n             AddScale               v=v0+(v1*n)
V=V*n+V*n+V*n       TripleScaledSum        v=v0*n0+v1*n1+v2*n2
V=V*V*n+V*V*n+V*V*n TripleSquaredScaledSum v=v0*v0*n0+v1*v1*n1+v2*v2*n2
V+=(V-V)*n          AddDiffScaledTo        v+=(v0-v1)*n
V=n*(V-((V*n+V)*n)) Special1               v=n0*(v0-((v1*n1+v2)*n2))
V=M*V               Multiply 0,1,2,3       v=M*v0

                    CrossProduct           v=v0xv1
                    DotProduct             v0.v1
                    Magnitude              length
                    Normalize              so that magnitude(v)==1
                    Normalize              v=normalize(v0)
                    ToBody                 vector_body=m*vector_world
                    ToWorld                vector_world=m^T*vector_body
                    ToWorldAdd             vector_world+=m^T*vector_body
*/

#define STATIC_INLINE static __forceinline

STATIC_INLINE
void Ident4(real v[4])
{//v={0,0,0,1}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
 v[3]=1.0;
}

STATIC_INLINE
void Zero(real v[4])
{//v={0,0,0}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
}

STATIC_INLINE
void Zero4(real v[4])
{//v={0,0,0,0}
 v[0]=0.0;
 v[1]=0.0;
 v[2]=0.0;
 v[3]=0.0;
}

STATIC_INLINE
void SetTo(real v[4],real n)
{//v={n,n,n}
 v[0]=n;
 v[1]=n;
 v[2]=n;
}

STATIC_INLINE
void SetTo4(real v[4],real n)
{//v={n,n,n,n}
 v[0]=n;
 v[1]=n;
 v[2]=n;
 v[3]=n;
}

STATIC_INLINE
void Copy(real v[4],const real v0[4])
{//v=v0
 v[0]=v0[0];
 v[1]=v0[1];
 v[2]=v0[2];
}

STATIC_INLINE
void Copy4(real v[4],const real v0[4])
{//v=v0
 v[0]=v0[0];
 v[1]=v0[1];
 v[2]=v0[2];
 v[3]=v0[3];
}

STATIC_INLINE
void AddTo(real v[4],const real v0[4])
{//v+=v0
 v[0]+=v0[0];
 v[1]+=v0[1];
 v[2]+=v0[2];
}

STATIC_INLINE
void AddTo4(real v[4],const real v0[4])
{//v+=v0
 v[0]+=v0[0];
 v[1]+=v0[1];
 v[2]+=v0[2];
 v[3]+=v0[3];
}

STATIC_INLINE
void Add(real v[4],const real v0[4],const real v1[4])
{//v=v0+v1
 v[0]=v0[0]+v1[0];
 v[1]=v0[1]+v1[1];
 v[2]=v0[2]+v1[2];
}

STATIC_INLINE
void Add4(real v[4],const real v0[4],const real v1[4])
{//v=v0+v1
 v[0]=v0[0]+v1[0];
 v[1]=v0[1]+v1[1];
 v[2]=v0[2]+v1[2];
 v[3]=v0[3]+v1[3];
}

STATIC_INLINE
void TrippleAdd(real v[4],const real v0[4],const real v1[4],const real v2[4])
{//v=v0+v1+v2
 v[0]=v0[0]+v1[0]+v2[0];
 v[1]=v0[1]+v1[1]+v2[1];
 v[2]=v0[2]+v1[2]+v2[2];
}

STATIC_INLINE
void TrippleAdd4(real v[4],const real v0[4],const real v1[4],const real v2[4])
{//v=v0+v1+v2
 v[0]=v0[0]+v1[0]+v2[0];
 v[1]=v0[1]+v1[1]+v2[1];
 v[2]=v0[2]+v1[2]+v2[2];
 v[3]=v0[3]+v1[3]+v2[3];
}

STATIC_INLINE
void SubtractFrom(real v[4],const real v0[4])
{//v-=v0
 v[0]-=v0[0];
 v[1]-=v0[1];
 v[2]-=v0[2];
}

STATIC_INLINE
void SubtractFrom4(real v[4],const real v0[4])
{//v-=v0
 v[0]-=v0[0];
 v[1]-=v0[1];
 v[2]-=v0[2];
 v[3]-=v0[3];
}

STATIC_INLINE
void Subtract(real v[4],const real v0[4],const real v1[4])
{//v=v0-v1
 v[0]=v0[0]-v1[0];
 v[1]=v0[1]-v1[1];
 v[2]=v0[2]-v1[2];
}

STATIC_INLINE
void Subtract4(real v[4],const real v0[4],const real v1[4])
{//v=v0-v1
 v[0]=v0[0]-v1[0];
 v[1]=v0[1]-v1[1];
 v[2]=v0[2]-v1[2];
 v[3]=v0[3]-v1[3];
}

STATIC_INLINE
void SelfScale(real v[4],real n)
{//v*=n
 v[0]*=n;
 v[1]*=n;
 v[2]*=n;
}

STATIC_INLINE
void SelfScale4(real v[4],real n)
{//v*=n
 v[0]*=n;
 v[1]*=n;
 v[2]*=n;
 v[3]*=n;
}

STATIC_INLINE
void Scale(real v[4],const real v0[4],real n)
{//v=v0*n
 v[0]=v0[0]*n;
 v[1]=v0[1]*n;
 v[2]=v0[2]*n;
}

STATIC_INLINE
void Scale4(real v[4],const real v0[4],real n)
{//v=v0*n
 v[0]=v0[0]*n;
 v[1]=v0[1]*n;
 v[2]=v0[2]*n;
 v[3]=v0[3]*n;
}

STATIC_INLINE
void AddScaledTo(real v[4],const real v0[4],real n)
{//v+=v0*n
 v[0]+=v0[0]*n;
 v[1]+=v0[1]*n;
 v[2]+=v0[2]*n;
}

STATIC_INLINE
void AddScaledTo4(real v[4],const real v0[4],real n)
{//v+=v0*n
 v[0]+=v0[0]*n;
 v[1]+=v0[1]*n;
 v[2]+=v0[2]*n;
 v[3]+=v0[3]*n;
}

STATIC_INLINE
void Combo(real v[4],const real v0[4],const real v1[4],real n)
{//v=v0+v1*n
 v[0]=v0[0]+v1[0]*n;
 v[1]=v0[1]+v1[1]*n;
 v[2]=v0[2]+v1[2]*n;
}

STATIC_INLINE
void Combo4(real v[4],const real v0[4],const real v1[4],real n)
{//v=v0+v1*n
 v[0]=v0[0]+v1[0]*n;
 v[1]=v0[1]+v1[1]*n;
 v[2]=v0[2]+v1[2]*n;
 v[3]=v0[3]+v1[3]*n;
}

STATIC_INLINE
void TripleScaledSum(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*n0+v1*n1+v2*n2
 v[0]=v0[0]*n0+v1[0]*n1+v2[0]*n2;
 v[1]=v0[1]*n0+v1[1]*n1+v2[1]*n2;
 v[2]=v0[2]*n0+v1[2]*n1+v2[2]*n2;
}

STATIC_INLINE
void TripleScaledSum4(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*n0+v1*n1+v2*n2
 v[0]=v0[0]*n0+v1[0]*n1+v2[0]*n2;
 v[1]=v0[1]*n0+v1[1]*n1+v2[1]*n2;
 v[2]=v0[2]*n0+v1[2]*n1+v2[2]*n2;
 v[3]=v0[3]*n0+v1[3]*n1+v2[3]*n2;
}

STATIC_INLINE
void TripleSquaredScaledSum(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*v0*n0+v1*v1*n1+v2*v2*n2
 v[0]=v0[0]*v0[0]*n0+v1[0]*v1[0]*n1+v2[0]*v2[0]*n2;
 v[1]=v0[1]*v0[1]*n0+v1[1]*v1[1]*n1+v2[1]*v2[1]*n2;
 v[2]=v0[2]*v0[2]*n0+v1[2]*v1[2]*n1+v2[2]*v2[2]*n2;
}

STATIC_INLINE
void TripleSquaredScaledSum4(real v[4],const real v0[4],real n0,const real v1[4],real n1,const real v2[4],real n2)
{//v=v0*v0*n0+v1*v1*n1+v2*v2*n2
 v[0]=v0[0]*v0[0]*n0+v1[0]*v1[0]*n1+v2[0]*v2[0]*n2;
 v[1]=v0[1]*v0[1]*n0+v1[1]*v1[1]*n1+v2[1]*v2[1]*n2;
 v[2]=v0[2]*v0[2]*n0+v1[2]*v1[2]*n1+v2[2]*v2[2]*n2;
 v[3]=v0[3]*v0[3]*n0+v1[3]*v1[3]*n1+v2[3]*v2[3]*n2;
}

STATIC_INLINE
void AddDiffScaledTo(real v[4],const real v0[4],const real v1[4],real n)
{//v+=(v0-v1)*n
 v[0]+=(v0[0]-v1[0])*n;
 v[1]+=(v0[1]-v1[1])*n;
 v[2]+=(v0[2]-v1[2])*n;
}

STATIC_INLINE
void AddDiffScaledTo4(real v[4],const real v0[4],const real v1[4],real n)
{//v+=(v0-v1)*n
 v[0]+=(v0[0]-v1[0])*n;
 v[1]+=(v0[1]-v1[1])*n;
 v[2]+=(v0[2]-v1[2])*n;
 v[3]+=(v0[3]-v1[3])*n;
}

STATIC_INLINE
void Special(real v[4],real n0,const real v0[4],const real v1[4],real n1,const real v2[4],real n2)
{//v=n0*(v0-((v1*n1+v2)*n2))
 v[0]=n0*(v0[0]-(v1[0]*n1+v2[0])*n2);
 v[1]=n0*(v0[1]-(v1[1]*n1+v2[1])*n2);
 v[2]=n0*(v0[2]-(v1[2]*n1+v2[2])*n2);
}

STATIC_INLINE
void CrossProduct(real v[4],const real v0[4],const real v1[4])
{//v=v0xv1
 v[0]=v0[1]*v1[2]-v0[2]*v1[1];
 v[1]=v0[2]*v1[0]-v0[0]*v1[2];
 v[2]=v0[0]*v1[1]-v0[1]*v1[0];
}

STATIC_INLINE
real DotProduct(const real v0[4],const real v1[4])
{//=v0.v1
 return v0[0]*v1[0]+v0[1]*v1[1]+v0[2]*v1[2];
}

STATIC_INLINE
real Magnitude(const real v0[4])
{//Length
 return sqrt(DotProduct(v0,v0));
}

STATIC_INLINE
void Normalize(real v[4])
{//so that Magnitude(v)==1
 const real l=Magnitude(v);
 if(l!=0.0)
 {
  const real li=1.0/l;
  v[0]*=li;
  v[1]*=li;
  v[2]*=li;
 }
}

STATIC_INLINE
void Normalize(real v[4],const real v0[4])
{//v=Normalize(v0)
 const real l=Magnitude(v0);
 if(l!=0.0)
 {
  const real li=1.0/l;
  v[0]=v0[0]*li;
  v[1]=v0[1]*li;
  v[2]=v0[2]*li;
 }
}

/*
static inline
void CrossProduct(real a[3],const real b[3],const real c[3])
{
 //fcmCrossProduct(a,b,c);
 a[0]=b[1]*c[2]-b[2]*c[1];
 a[1]=b[2]*c[0]-b[0]*c[2];
 a[2]=b[0]*c[1]-b[1]*c[0];
}

static inline
real DotProduct(const real a[3],const real b[3])
{
 //return fcmDotProduct(a,b);
 return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

static inline
real Magnitude(const real a[3])
{
 //return fcmMagnitude(a);
 return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}

static inline
void Normalize(real normed[3],const real unnormed[3])
{
 ASSERT(sqrt(unnormed[0]*unnormed[0]+unnormed[1]*unnormed[1]+unnormed[2]*unnormed[2])!=0.0);
 //fcmNormalize(normed,unnormed);
 
 real lengthi=1/sqrt(unnormed[0]*unnormed[0]+unnormed[1]*unnormed[1]+unnormed[2]*unnormed[2]);
 normed[0]=unnormed[0]*lengthi;
 normed[1]=unnormed[1]*lengthi;
 normed[2]=unnormed[2]*lengthi;
}
*/

STATIC_INLINE
void ToBody(real vector_body[4],const real vector_world[4],const real m[4][4])
{//vector_body=m*vector_world
 vector_body[0]=DotProduct(m[0],vector_world);
 vector_body[1]=DotProduct(m[1],vector_world);
 vector_body[2]=DotProduct(m[2],vector_world);
}

STATIC_INLINE
void ToWorld(real vector_world[4],const real vector_body[4],const real m[4][4])
{//vector_world=m^T*vector_body
 vector_world[0]=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
 vector_world[1]=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
 vector_world[2]=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
}

STATIC_INLINE
void ToWorldAdd(real vector_world[4],const real vector_body[4],const real m[4][4])
{//vector_world+=m^T*vector_body
 vector_world[0]+=m[0][0]*vector_body[0]+m[1][0]*vector_body[1]+m[2][0]*vector_body[2];
 vector_world[1]+=m[0][1]*vector_body[0]+m[1][1]*vector_body[1]+m[2][1]*vector_body[2];
 vector_world[2]+=m[0][2]*vector_body[0]+m[1][2]*vector_body[1]+m[2][2]*vector_body[2];
}

STATIC_INLINE
void Multiply(real v[3],const real m[3][3],const real v1[3])
{
 v[0]=DotProduct(m[0],v1);
 v[1]=DotProduct(m[1],v1);
 v[2]=DotProduct(m[2],v1);
}

STATIC_INLINE
void MultiplyTranspose(real v[3],const real m[3][3],const real v1[3])
{
 v[0]=m[0][0]*v1[0]+m[1][0]*v1[1]+m[2][0]*v1[2];
 v[1]=m[0][1]*v1[0]+m[1][1]*v1[1]+m[2][1]*v1[2];
 v[2]=m[0][2]*v1[0]+m[1][2]*v1[1]+m[2][2]*v1[2];
}

#undef STATIC_INLINE
