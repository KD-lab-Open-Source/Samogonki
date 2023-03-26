#include "Frame.h"
#include "Object3d.h"

#ifdef _MECHOSOMA_
#include "mechosoma.h" // For far target
#endif

sKeyTri::~sKeyTri()													
{ 
	if(Key) 
	{
		cTile *Tile=((cTile*)Key);
		delete Tile;
	}
}
sKeyMaterial::~sKeyMaterial()													
{ 
	if(Key) 
	{
		cTile *Tile=((cTile*)Key);
		delete Tile;
	}
}

cAnimChain::cAnimChain()													
{ 
	TimeChain=0; 
	Attribute=ANIMCHAIN_NULL; 
}
cAnimChain::cAnimChain(cAnimChain *AnimChain)
{
	KeyTri.Base=AnimChain->KeyTri.Base;
	KeyTri.length()=AnimChain->KeyTri.length();
	KeyMaterial.Base=AnimChain->KeyMaterial.Base;
	KeyMaterial.length()=AnimChain->KeyMaterial.length();
	SetTimeChain(AnimChain->GetTimeChain());
	GetName()=AnimChain->GetName();
	Attribute=AnimChain->GetAttribute()|ANIMCHAIN_COPY;
}
cAnimChain::~cAnimChain()
{ 
	if(GetAttribute(ANIMCHAIN_COPY))
	{
		KeyTri.Base=0; 
		KeyTri.length()=0; 
	}
	else 
		KeyTri.Delete();
	if(GetAttribute(ANIMCHAIN_COPY))
	{
		KeyMaterial.Base=0; 
		KeyMaterial.length()=0; 
	}
	else
		KeyMaterial.Delete();
}
void cAnimChain::Translate(const Vect3f &dPos)
{
	for(int i=0;i<GetNumberTri();i++)
	{
		if(GetTri(i)->Key==0) continue;
		((cTile*)GetTri(i)->Key)->TranslateTri(dPos);
	}
}
void cAnimChain::Scale(const Vect3f &scale)
{
	for(int i=0;i<GetNumberTri();i++)
	{
		if(GetTri(i)->Key==0) continue;
		((cTile*)GetTri(i)->Key)->ScaleTri(scale);
	}
}
void cAnimChain::GetTile(cTile *Tile,float phase)
{
	int number=GetTri(phase);
	float CurrentTime=GetTimeChain()*phase;
	float t;
	for(int i=0;i<Tile->GetNumberTile();i++)
	{
		sTile *a=0,*b=0,*tmp=Tile->GetTile(i);
		int ia;
		for(ia=number;ia>=0;ia--)
			if((GetTri(ia)->Time<=CurrentTime)&&(a=((cTile*)GetTri(ia)->Key)->GetTileByID(tmp->GetID())))
				break;
		int ib;
		for(ib=number;ib<GetNumberTri();ib++)
			if((GetTri(ib)->Time>=CurrentTime)&&(b=((cTile*)GetTri(ib)->Key)->GetTileByID(tmp->GetID())))
				break;
		assert(a);
		if(a->GetNumberPoint()!=tmp->GetNumberPoint())
			ErrAbort("Error:cAnimChain::GetTri()");
		if((b)&&(ia!=ib))
		{ // морфинг
			float aTime=GetTri(ia)->Time,bTime=GetTri(ib)->Time;
			assert(aTime!=bTime);
			if(a->GetNumberPoint()!=b->GetNumberPoint())
			{
				XBuffer buf;
				buf<"Error:cAnimChain::GetTri()\r\nNumberPoint object <"<a->GetName()<"> in AnimChain <"<this->name<"> change";
				ErrAbort(buf.address());
			}
			t=(CurrentTime-aTime)/(bTime-aTime);
			if(t>1) t=1; else if(t<0) t=0;
			sPoint *Point=tmp->GetPoint(),*PointA=a->GetPoint(),*PointB=b->GetPoint();
			tmp->MulCol=a->MulCol*(1-t)+b->MulCol*t;
			tmp->AddCol=a->AddCol*(1-t)+b->AddCol*t;
			for(int i=0;i<tmp->GetNumberPoint();i++)
			{
				Point[i].pos.x=PointA[i].pos.x+t*(PointB[i].pos.x-PointA[i].pos.x);
				Point[i].pos.y=PointA[i].pos.y+t*(PointB[i].pos.y-PointA[i].pos.y);
				Point[i].pos.z=PointA[i].pos.z+t*(PointB[i].pos.z-PointA[i].pos.z);
//				Point[i].normal=PointA[i].normal+(PointB[i].normal-PointA[i].normal)*t;
			}
			if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX_TEXEL|ATTRTILE_TEXEL))
			{
				sTexel *Texel=tmp->GetTexel(),*TexelA=a->GetTexel(),*TexelB=b->GetTexel();
				assert(tmp->GetNumberTexel()==a->GetNumberTexel()&&a->GetNumberTexel()==b->GetNumberTexel());
				for(int i=0;i<tmp->GetNumberTexel();i++)
				{
					float ua=TexelA[i].u(),va=TexelA[i].v(),ub=TexelB[i].u(),vb=TexelB[i].v();
					Texel[i].SetTexel(ua+(ub-ua)*t,va+(vb-va)*t);
				}
			}
		}
		else // нет морфирования
		{
			tmp->MulCol=a->MulCol;
			tmp->AddCol=a->AddCol;
			sPoint *Point=tmp->GetPoint(),*PointA=a->GetPoint();
			for(int i=0;i<tmp->GetNumberPoint();i++)
				Point[i].pos=PointA[i].pos;
//			memcpy(tmp->GetPoint(),a->GetPoint(),tmp->GetNumberPoint()*sizeof(sPoint));
			if(tmp->Attribute.GetAttribute(ATTRTILE_VERTEX_TEXEL|ATTRTILE_TEXEL))
				memcpy(tmp->GetTexel(),a->GetTexel(),tmp->GetNumberTexel()*sizeof(sTexel));
		}
	}
}

#define NEW_MEM_COPY(src,dest,size,type)			{ src=new type[size]; memcpy(src,dest,sizeof(type)*size); }
#define INTERPOLATION_QUADRATIC_INIT(x,Time,xk,nKey){ cSpline Spline; Spline.CalcKoofQuadSpline(nKey,Time,x); memcpy(xk,Spline.KoofQuadSpline,nKey*sizeof(float)); }

inline float QuadraticInterpolate(float x1,float x2,float y1,float y2,float ky1,float ky2,float t)
{
	float dx=x2-x1;
	float dt=t-x1;
	float ts=(t-(x1+x2)*0.5f);
	if(ts>0) ts*=ts; else ts=0; 
	return y1+((y2-y1)/dx-(3*ky1+ky2)*0.125*dx)*dt+(ky1*dt*dt+(ky2-ky1)*ts)*0.5f;
}
inline void SlerpInterpolate(float w1,float x1,float y1,float z1,float w2,float x2,float y2,float z2,float *w,float *x,float *y,float *z,float t)
{ // Slerp(q1,q2,t) = (sin((1-t)*ї)/sin(ї))*q1+(sin(t*ї)/sin(ї))*q2 
	float scale0,scale1,cosom=x1*x2+y1*y2+z1*z2+w1*w2; 
	if((1+cosom)>0.0001)
	{
		if((1.0-cosom)>0.0001) 
		{
			float omega=acosf(cosom);
			float div_sinom=1/sinf(omega);
			scale0=sinf((1-t)*omega)*div_sinom;
			scale1=sinf(t*omega)*div_sinom;
		} 
		else { scale0=1-t; scale1=t; }
		*x=scale0*x1+scale1*x2;
		*y=scale0*y1+scale1*y2;
		*z=scale0*z1+scale1*z2;
		*w=scale0*w1+scale1*w2;
		return;
	} 
	scale0=sinf((1-t)*M_PI*0.5f);
	scale1=sinf(t*M_PI*0.5f);
	*x=scale0*x1-scale1*y2;
	*y=scale0*y1+scale1*x2;
	*z=scale0*z1-scale1*w2;
	*w=scale0*w1+scale1*z2;
}

sKFrame::sKFrame(sKFrame *kf)
{ 
	if((kf)&&(kf->IsActive()))
	{
		NumberKey=kf->NumberKey; 
		NEW_MEM_COPY(Time,kf->Time,NumberKey,float);
		attribute=kf->attribute;
		count=kf->count;
	}
	else { attribute=1; count=0; NumberKey=0; Time=0; }
}
int sKFrame::GetNumber()
{
/*
	int i0=0,in=NumberKey-1,i,CurrentTime=round(count*GetMaxTime());
	if(in<=0) return 0;
	while(i=(i0+in)/2)
		if(in<=i0+1) return i=in;
		else if(CurrentTime<Time[i]) in=i;
		else if(CurrentTime>Time[i]) i0=i;
		else return i;
	return 1;
*/	
	if(NumberKey <= 1) 
		return 0;
	float CurrentTime = count*GetMaxTime();
	for(int i = 1; i < NumberKey;i++)
		if(CurrentTime < (float)Time[i]) 
			return i;
	return NumberKey-1;
}
void sKFrame::Set(void *time,int ofs,int nKey)
{
	Time=new float[NumberKey=nKey];
	float *ftime=(float*)time;
	for(int i=0;i<nKey;i++)
		Time[i]=ftime[i*ofs];
}
sPKFrame::sPKFrame(sPKFrame *kf) : sKFrame((sKFrame*)kf)
{
	if((kf)&&(kf->IsActive()))
	{
		NEW_MEM_COPY(x,kf->x,NumberKey,float);
		NEW_MEM_COPY(y,kf->y,NumberKey,float);
		NEW_MEM_COPY(z,kf->z,NumberKey,float);
		NEW_MEM_COPY(xk,kf->xk,NumberKey,float);
		NEW_MEM_COPY(yk,kf->yk,NumberKey,float);
		NEW_MEM_COPY(zk,kf->zk,NumberKey,float);
	}
	else { x=y=z=xk=yk=zk=0; attribute=KFRAME_QUADRATIC; }
}
void sPKFrame::Set(void *PosKXYZ,int nPos)
{
	sKFrame::Set(PosKXYZ,4,nPos);
	float *fPosKXYZ=(float*)PosKXYZ;
	x=new float [nPos]; y=new float [nPos]; z=new float [nPos]; 
	xk=new float [nPos]; yk=new float [nPos]; zk=new float [nPos]; 
	for(int i=0;i<nPos;i++)
	{	
		x[i]=fPosKXYZ[4*i+1]-fPosKXYZ[4*0+1];
		y[i]=fPosKXYZ[4*i+2]-fPosKXYZ[4*0+2];
		z[i]=fPosKXYZ[4*i+3]-fPosKXYZ[4*0+3];
	}
	INTERPOLATION_QUADRATIC_INIT(x,Time,xk,nPos);
	INTERPOLATION_QUADRATIC_INIT(y,Time,yk,nPos);
	INTERPOLATION_QUADRATIC_INIT(z,Time,zk,nPos);
}
void sPKFrame::Scale(float sx,float sy,float sz)
{
	for(int i=0;i<NumberKey;i++)
	{	
		x[i]*=sx; y[i]*=sy; z[i]*=sz;
	}
	INTERPOLATION_QUADRATIC_INIT(x,Time,xk,NumberKey);
	INTERPOLATION_QUADRATIC_INIT(y,Time,yk,NumberKey);
	INTERPOLATION_QUADRATIC_INIT(z,Time,zk,NumberKey);
}
void sPKFrame::Get(float *xPos,float *yPos,float *zPos)
{
	float t;
	int n=GetNumber();
	if((n-1)>=0) 
		switch(attribute)
		{
			case KFRAME_QUADRATIC:
				t=count*GetMaxTime();
				*xPos=QuadraticInterpolate(Time[n-1],Time[n],x[n-1],x[n],xk[n-1],xk[n],t);
				*yPos=QuadraticInterpolate(Time[n-1],Time[n],y[n-1],y[n],yk[n-1],yk[n],t);
				*zPos=QuadraticInterpolate(Time[n-1],Time[n],z[n-1],z[n],zk[n-1],zk[n],t);
				break;
			case KFRAME_COS:
				t=GetT(n);
				*xPos=CosInterpolate(x[n-1],x[n],t);
				*yPos=CosInterpolate(y[n-1],y[n],t);
				*zPos=CosInterpolate(z[n-1],z[n],t);
				break;
			case KFRAME_LINEAR:
				t=GetT(n);
				*xPos=LinearInterpolate(x[n-1],x[n],t);
				*yPos=LinearInterpolate(y[n-1],y[n],t);
				*zPos=LinearInterpolate(z[n-1],z[n],t);
				break;
			case KFRAME_ARCLENGTH:
				t=GetT(n);
				*xPos=ArcLengthInterpolate(x[n-1],x[n],t);
				*yPos=ArcLengthInterpolate(y[n-1],y[n],t);
				*zPos=ArcLengthInterpolate(z[n-1],z[n],t);
				break;
			default: assert(0);
		}
	else { *xPos=x[0]; *yPos=y[0]; *zPos=z[0]; }
}
sRKFrame::sRKFrame(sRKFrame *kf) : sKFrame((sKFrame*)kf)
{
	if((kf)&&(kf->IsActive()))
	{
		NEW_MEM_COPY(x,kf->x,NumberKey,float);
		NEW_MEM_COPY(y,kf->y,NumberKey,float);
		NEW_MEM_COPY(z,kf->z,NumberKey,float);
		NEW_MEM_COPY(w,kf->w,NumberKey,float);
	}
	else { x=y=z=w=0; attribute=KFRAME_LINEAR;  }
}
void sRKFrame::Set(void *RotKWXYZ,int nRot)
{
	sKFrame::Set(RotKWXYZ,5,nRot);
	float *fRotKWXYZ=(float*)RotKWXYZ;
	w=new float [nRot]; x=new float [nRot]; y=new float [nRot]; z=new float [nRot]; 
	for(int i=0;i<nRot;i++)
	{	
		w[i]=fRotKWXYZ[5*i+1];
		x[i]=fRotKWXYZ[5*i+2];
		y[i]=fRotKWXYZ[5*i+3];
		z[i]=fRotKWXYZ[5*i+4];
	}
	x[0]=y[0]=z[0]=0; w[0]=1;
	if(fabs(w[nRot-1]-w[0])<0.02) 
	{ x[nRot-1]=x[0]; y[nRot-1]=y[0]; z[nRot-1]=z[0]; w[nRot-1]=w[0]; }
}
void sRKFrame::Get(float *wRot,float *xRot,float *yRot,float *zRot)
{
	int n=GetNumber();
	if((n-1)>=0) 
	{
		float t=GetT(n);
		switch(attribute)
		{
			case KFRAME_LINEAR:
				*wRot=LinearInterpolate(w[n-1],w[n],t);
				*xRot=LinearInterpolate(x[n-1],x[n],t);
				*yRot=LinearInterpolate(y[n-1],y[n],t);
				*zRot=LinearInterpolate(z[n-1],z[n],t);
				break;
			case KFRAME_COS:
				*wRot=CosInterpolate(w[n-1],w[n],t);
				*xRot=CosInterpolate(x[n-1],x[n],t);
				*yRot=CosInterpolate(y[n-1],y[n],t);
				*zRot=CosInterpolate(z[n-1],z[n],t);
				break;
			case KFRAME_SLERP:
				SlerpInterpolate(w[n-1],x[n-1],y[n-1],z[n-1],
					w[n],x[n],y[n],z[n],wRot,xRot,yRot,zRot,t);
				break;
			case KFRAME_ARCLENGTH:
				*wRot=ArcLengthInterpolate(w[n-1],w[n],t);
				*xRot=ArcLengthInterpolate(x[n-1],x[n],t);
				*yRot=ArcLengthInterpolate(y[n-1],y[n],t);
				*zRot=ArcLengthInterpolate(z[n-1],z[n],t);
				break;
			default: assert(0);
		}
		float tmp=1.f/sqrtf((*wRot)*(*wRot)+(*xRot)*(*xRot)+(*yRot)*(*yRot)+(*zRot)*(*zRot));
		(*wRot)*=tmp; (*xRot)*=tmp; (*yRot)*=tmp; (*zRot)*=tmp;
	}
	else { *wRot=w[0]; *xRot=x[0]; *yRot=y[0]; *zRot=z[0]; }
}
sSKFrame::sSKFrame(sSKFrame *kf) : sKFrame((sKFrame*)kf)
{
	if((kf)&&(kf->IsActive()))
	{
		NEW_MEM_COPY(sx,kf->sx,NumberKey,float);
		NEW_MEM_COPY(sy,kf->sy,NumberKey,float);
		NEW_MEM_COPY(sz,kf->sz,NumberKey,float);
	}
	else { sx=sy=sz=0; attribute=KFRAME_LINEAR; }
}
void sSKFrame::Set(void *ScaleKXYZ,int nScale)
{
	sKFrame::Set(ScaleKXYZ,4,nScale);
	float *fScaleKXYZ=(float*)ScaleKXYZ;
	sx=new float [nScale]; sy=new float [nScale]; sz=new float [nScale]; 
	for(int i=0;i<nScale;i++)
	{	
		sx[i]=fScaleKXYZ[4*i+1];
		sy[i]=fScaleKXYZ[4*i+2];
		sz[i]=fScaleKXYZ[4*i+3];
	}
}
void sSKFrame::Get(float *xScale,float *yScale,float *zScale)
{
	int n=GetNumber();
	if((n-1)>=0) 
	{
		float t=GetT(n);
		switch(attribute)
		{
			case KFRAME_COS:
				*xScale=CosInterpolate(sx[n-1],sx[n],t);
				*yScale=CosInterpolate(sy[n-1],sy[n],t);
				*zScale=CosInterpolate(sz[n-1],sz[n],t);
				break;
			case KFRAME_LINEAR:
				*xScale=LinearInterpolate(sx[n-1],sx[n],t);
				*yScale=LinearInterpolate(sy[n-1],sy[n],t);
				*zScale=LinearInterpolate(sz[n-1],sz[n],t);
				break;
			case KFRAME_ARCLENGTH:
				*xScale=ArcLengthInterpolate(sx[n-1],sx[n],t);
				*yScale=ArcLengthInterpolate(sy[n-1],sy[n],t);
				*zScale=ArcLengthInterpolate(sz[n-1],sz[n],t);
				break;
			default: assert(0);
		}
	}
	else {*xScale=sx[0]; *yScale=sy[0]; *zScale=sz[0]; }
}
sMKFrame::sMKFrame(sMKFrame *kf) : sKFrame((sKFrame*)kf)
{
	if((kf)&&(kf->IsActive()))
		NEW_MEM_COPY(key,kf->key,NumberKey,cMesh*)
	else { key=0; attribute=KFRAME_LINEAR; }
}
void sMKFrame::Set(cMesh **Morph,float *time,int nMorph)
{
	key=new cMesh*[nMorph];
	sKFrame::Set(time,1,nMorph);
	for(int i=0;i<nMorph;i++)
		key[i]=Morph[i];
}
void sMKFrame::Get(void *Morph)
{
	extern void FunctionMorphLinear(void *key,void *key1,void *key2,float t);
	int n=GetNumber();
	if((n-1)>=0) 
	{
		float t=GetT(n);
		switch(attribute)
		{
			case KFRAME_LINEAR:
				FunctionMorphLinear(Morph,key[n-1],key[n],t);
				break;
			default: assert(0);
		}
	}
	else FunctionMorphLinear(Morph,key[0],key[0],0);
}

////////////////////////////////////////////////////////////////////////////////////////////
//		cFrame definitions
////////////////////////////////////////////////////////////////////////////////////////////
cFrame::cFrame(cFrame *Frame)
{
	if(Frame==0)
	{ 
		pkf=0; rkf=0; skf=0; mkf=0;
		ChainNumber=-1;
		Name[0]=Parent[0]=0;
		xCenter=yCenter=zCenter=xPivot=yPivot=zPivot=xOfs=yOfs=zOfs=0; 
		count=0;prev_count=-1; 
		finish=1;
		dcount=1.f/M3D_DEFAULT_ANIMATION_PERIOD;
		looped=1;
#ifdef _SURMAP_
		StartCount=count;
#endif //_SURMAP_
		return; 
	}
	xCenter=Frame->xCenter,yCenter=Frame->yCenter,zCenter=Frame->zCenter;
	xPivot=Frame->xPivot,yPivot=Frame->yPivot,zPivot=Frame->zPivot;
	xOfs=Frame->xOfs,yOfs=Frame->yOfs,zOfs=Frame->zOfs; 
	strcpy(Name,Frame->Name);
	strcpy(Parent,Frame->Parent);
	if(Frame->pkf) pkf=new sPKFrame(Frame->pkf); else pkf=0;
	if(Frame->rkf) rkf=new sRKFrame(Frame->rkf); else rkf=0;
	if(Frame->skf) skf=new sSKFrame(Frame->skf); else skf=0;
	if(Frame->mkf) mkf=new sMKFrame(Frame->mkf); else mkf=0;
	ChainNumber=Frame->GetCurrentChain();
	AnimChain.New(Frame->GetNumberAnimChain());
	for(int i=0;i<Frame->GetNumberAnimChain();i++)
		GetAnimChain(i)=new cAnimChain(Frame->GetAnimChain(i));
	SetCurrentChain(Frame->GetCurrentChain());
	prev_count=Frame->prev_count;
	looped=Frame->looped;
	finish=Frame->finish;
	dcount=Frame->dcount;
	count=Frame->count;
#ifdef _SURMAP_
	StartCount=Frame->StartCount;
#endif //_SURMAP_
}
cFrame::cFrame(const char *name,const char *parent,float xpivot,float ypivot,float zpivot,float xofs,float yofs,float zofs)
{
	ChainNumber=-1; count = 0; prev_count=-1; 
	finish=1; dcount=1.f/M3D_DEFAULT_ANIMATION_PERIOD;
	looped=1;
	strcpy(Name,name);
	strcpy(Parent,parent);
	xCenter=yCenter=zCenter=0;
	pkf=0; rkf=0; skf=0; mkf=0;
	xOfs=xofs; yOfs=yofs; zOfs=zofs;
	xPivot=xpivot; yPivot=ypivot; zPivot=zpivot; 
	AnimChain=0;
#ifdef _SURMAP_
	StartCount=count;
#endif //_SURMAP_
}
cFrame::~cFrame()
{
	if(pkf) delete pkf; 
	if(rkf) delete rkf; 
	if(skf) delete skf; 
	if(mkf) delete mkf; 
	for(int i=0;i<AnimChain.length();i++)
	{
		delete AnimChain[i];
		AnimChain[i]=0;
	}
	AnimChain.Delete();
}

void cFrame::Scale(const Vect3f &scale)
{
	xCenter*=scale.x; yCenter*=scale.y; zCenter*=scale.z;
	xPivot*=scale.x; yPivot*=scale.y; zPivot*=scale.z; 
	xOfs*=scale.x; yOfs*=scale.y; zOfs*=scale.z;
	if(pkf) pkf->Scale(scale.x,scale.y,scale.z);
	for(int i=0;i<GetNumberAnimChain();i++) 
		GetAnimChain(i)->Scale(scale);
}
void cFrame::Translate(const Vect3f &dPos)
{
	xCenter=-dPos.x-xOfs; yCenter=-dPos.y-yOfs; zCenter=-dPos.z-zOfs;
	for(int i=0;i<GetNumberAnimChain();i++) 
		GetAnimChain(i)->Translate(dPos);
}
void cFrame::SetPhase(float phase)
{ 
	prev_count = count;
	float new_count = cycle(phase);
	count = new_count;
	if(pkf) pkf->count=count; 
	if(rkf) rkf->count=count; 
	if(skf) skf->count=count;
	if(mkf) mkf->count=count;
}
int cFrame::IsPhasePassed(float phase)
{
	return fabs(dist(count, phase)) + fabs(dist(prev_count, phase)) <= fabs(dist(count, prev_count));
}
void cFrame::Set(float animation_period, float start_phase, float finish_phase)
{
	if(animation_period)
		dcount = SIGN(animation_period)*cycle(1.f/fabs(animation_period));
	else
		dcount = 0;

	if(start_phase != -1)
	{
		SetPhase(start_phase);
#ifdef _SURMAP_
		StartCount=count;
#endif//_SURMAP_
	}

	if(finish_phase != -1){
		finish = finish_phase;
		looped = 0;
		}
	else 
		looped = 1;
}	
void cFrame::AddPhase(float dt)
{ 
	float dPhase=cycle(dt*dcount);
	SetPhase(count + dPhase);
	if(!looped && IsPhasePassed(finish))
	{
		float tmp = prev_count;
		SetPhase(finish);
		prev_count = tmp;
		dcount = 0;
	}
}
float cFrame::dist(float v0,float v1)
{
	float d = v0 - v1;
	float ad = (float)fabs(d);
	float dd = frame_phase_range - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + frame_phase_range : d - frame_phase_range;
}
int cFrame::SetCurrentChain(const char *NameChain)
{
	assert(NameChain);
	for(int i=0;i<GetNumberAnimChain();i++)
		if(GetAnimChain(i)->GetName()==static_cast<const char*>(NameChain))
			return SetCurrentChain(i);
	return -1;
}
int cFrame::SetCurrentChain(const char *NameChainMask,int number)
{ // возвращает число цепочек имеющих в имени строку NameChainMask
	if(GetNumberAnimChain()<=0) return -1;
	if(NameChainMask==0) return GetNumberAnimChain();
	int count=0;
	for(int i=0;i<GetNumberAnimChain();i++)
		if(strstr(GetAnimChain(i)->GetName(),NameChainMask))
			if((count++)==number) return SetCurrentChain(i);
	return -1;
}
int cFrame::GetChain(const char *NameChainMask)
{ // возвращает число цепочек имеющих в имени строку NameChainMask
	if(NameChainMask==0) return GetNumberAnimChain();
	int count=0;
	for(int i=0;i<GetNumberAnimChain();i++)
		if(strstr(GetAnimChain(i)->GetName(),NameChainMask))
			count++;
	return count;
}
