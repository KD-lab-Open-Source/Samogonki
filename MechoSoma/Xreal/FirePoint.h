
#ifndef __FIRE_POINT__
#define __FIRE_POINT__

#include "fxlabInterface.h"
#include "Body.h"

struct FirePoint : Vect3f {
	Vect3f direction;

	FirePoint(){}
	FirePoint(const Vect3f& dummyA, const Vect3f& dummyB) 
	: Vect3f(dummyB) 
	{ 
		direction.sub(dummyB, dummyA); 
		direction.normalize();
	}

	virtual ~FirePoint(void)
	{
	}

	virtual void Fire(const MatXf& bodyPose)
	{
	};

	virtual void FireWithRotate(const MatXf& bodyPose)
	{
	};
};

void fxlabCannonFire(const Vect3f& pos,Vect3f& vel,float pow);

struct CannonPoint : FirePoint
{
	DurationTimer latency;

	CannonPoint(const Vect3f& dummyA, const Vect3f& dummyB) : FirePoint(dummyA,dummyB)
	{ 
		latency.start(5000);
	};

	void Fire(const MatXf& bodyPose)
	{
		Vect3f v, d;
		if(!latency()){
			latency.start(5000);
			bodyPose.xformPoint(*this, v);
			bodyPose.xformVect(direction, d);
			CYCLE(v);

			Vect3f v1(d*(4 + fabsRnd(4)));
			fxlabCannonFire(v,v1,2000);
		};
	};
};

struct DragonPoint : FirePoint
{	
	fxlabFirePointInterface fxlabFirePoint;

	DragonPoint(const Vect3f& dummyA, const Vect3f& dummyB,const int fxID);

	~DragonPoint(void)
	{
		fxlabFirePoint.Close();
	}

	void Fire(const MatXf& bodyPose)
	{
		Vect3f v, d;
		bodyPose.xformPoint(*this, v);
		bodyPose.xformVect(direction, d);
		CYCLE(v);
		fxlabFirePoint.Update(v,d);
	};

	void FireWithRotate(const MatXf& bodyPose)
	{
		Vect3f v/*, d*/;
		bodyPose.xformPoint(*this, v);
//		bodyPose.xformVect(direction, d);
		CYCLE(v);
		fxlabFirePoint.Update(v,(Mat3f)(bodyPose.rot()));
	};
};


class FirePointList : public std::vector<PtrHandle<FirePoint> >
{
public:
	FirePointList(){}
	FirePointList(class cMesh* mesh){ parseDummy(mesh); }

	int parseDummy(class DummyList& dummies);
	int parseDummy(class cMesh* mesh);

	virtual void fire(const MatXf& bodyPose);
	virtual void fire_with_rotate(const MatXf& bodyPose);
};

class FirePointBody : public FirePointList, virtual public Body
{
public:
	FirePointBody(){}
	FirePointBody(cMesh* mesh) :  FirePointList(mesh) {}

	void post_quant(){ Body::post_quant(); fire(pose()); }
};


#endif // __FIRE_POINT__
