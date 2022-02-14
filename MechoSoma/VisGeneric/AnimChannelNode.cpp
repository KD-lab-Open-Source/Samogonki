#include "UMATH.H"
#include "BaseClass.h"
#include "cString.h"
#define VISASSERT	assert

#include "AnimChannelNode.h"
#include "float.h"

//////////////////////////////////////////////////////////////////////////////////////////
// реализация cAnimChainNode
//////////////////////////////////////////////////////////////////////////////////////////
void QuatSlerp(QuatF &from,QuatF &to,float t,QuatF &res)
{
	QuatF	tmp;
	float	omega,cosom,sinom,scale0,scale1;

	// calc cosine
	cosom=from.x()*to.x()+from.y()*to.y()+from.z()*to.z()+from.s()*to.s();
	if(cosom<0.0)
	{ 
		cosom = -cosom; 
		tmp.set(-to.s(),-to.x(),-to.y(),-to.z());
	} 
	else  
		tmp=to;
	// calculate coefficients
   if((1.f - cosom) > 0.001f ) 
   {
		// standard case (slerp)
		omega = acosf(cosom);
		sinom = sinf(omega);
		scale0 = sinf((1.f - t) * omega) / sinom;
		scale1 = sinf(t * omega) / sinom;
	} 
	else 
	{        
		scale0 = 1.f - t;
		scale1 = t;
	}
	// calculate final values
	res=scale0*from+scale1*tmp;
}

cAnimChainNode::cAnimChainNode()
{
	Time=0;
}
cAnimChainNode::~cAnimChainNode()
{
}
void cAnimChainNode::Release()
{
	KeyVisible.Delete();
	KeyPosition.Delete();
	KeyRotation.Delete();
	KeyScale.Delete();
}
void cAnimChainNode::GetMatrix(float phase,int &visible,MatXf &Matrix)
{
	Identity(Matrix);
	float CurrentTime=phase*Time;
	{ // анимация позиции
		sKey3f *a=0,*b=0;
		int i;
		for(i=0;i<GetNumberPos();i++)
			if(GetPos(i).time>=CurrentTime)
			{ b=&GetPos(i); break; }
		for(i=GetNumberPos()-1;i>=0;i--)
			if(GetPos(i).time<=CurrentTime)
			{ a=&GetPos(i); break; }
		VISASSERT(a);
		if(b==0||a==b)
			Matrix.trans()=a->v;
		else
			Matrix.trans()=a->v+(b->v-a->v)*((CurrentTime-a->time)/(b->time-a->time));
	}
	{ // анимация вращения
		sKey4f *a=0,*b=0;
		int i;
		for(i=0;i<GetNumberRot();i++)
			if(GetRot(i).time>=CurrentTime)
			{ b=&GetRot(i); break; }
		for(i=GetNumberRot()-1;i>=0;i--)
			if(GetRot(i).time<=CurrentTime)
			{ a=&GetRot(i); break; }
		VISASSERT(a);
		if(b==0||a==b||a->q==b->q)
			Matrix.rot().set(a->q);
		else
		{
			QuatF q;
			QuatSlerp(a->q,b->q,(CurrentTime-a->time)/(b->time-a->time),q);
//			q.interpolate(a->q,b->q,(CurrentTime-a->time)/(b->time-a->time));
			q.normalize();
			Matrix.rot().set(q);
		}
	}
	{ // анимация масштаба
		sKey3f *a=0,*b=0;
		int i;
		for(i=0;i<GetNumberScale();i++)
			if(GetScale(i).time>=CurrentTime)
			{ b=&GetScale(i); break; }
		for(i=GetNumberScale()-1;i>=0;i--)
			if(GetScale(i).time<=CurrentTime)
			{ a=&GetScale(i); break; }
		VISASSERT(a);
		if(b==0||a==b)
			Matrix.rot().scale(a->v);
		else
			Matrix.rot().scale(a->v+(b->v-a->v)*((CurrentTime-a->time)/(b->time-a->time)));
	}
	{ // анимация видимости
		sKeyVisible *a=0;
		for(int i=GetNumberVisible()-1;i>=0;i--)
			if(GetVisible(i).time<=CurrentTime)
			{ a=&GetVisible(i); break; }
		VISASSERT(a);
		visible=a->visible;
	}
// TODO: @caiiiycuk investigate this
#ifdef WTF
	VISASSERT(_finite(Matrix.rot()(1,1))&&_finite(Matrix.trans().x));
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////
// реализация cAnimChannelNode
//////////////////////////////////////////////////////////////////////////////////////////
cAnimChannelNode::cAnimChannelNode(int number)
{
	if(number>0) NewChannel(number);
}
cAnimChannelNode::~cAnimChannelNode()
{
	for(int i=0;i<GetNumberChannel();i++)
		GetChannel(i)->Release();
	cBaseDynArray<cAnimChainNode>::Delete();
}
