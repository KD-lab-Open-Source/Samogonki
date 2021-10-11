#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "FlyingObject.h"
#include "SimpleClip.h"
#include "Params.h"

FlyingObject::FlyingObject() 
{ 
	is_dynamic_ = 0; 
	lift_bias = 0.1f;
	relative_height = 100; 
	dragV.set(1.5);
	dragW.set(1.5);
}

void FlyingObject::setPositionInit()
{
	setTrans(R().z + relative_height, Z_AXIS);
	ControlledObject::setPositionInit();
}

void FlyingObject::setHeight(float relative_height_)
{
	//average(relative_height, relative_height_, 0.22f);
	relative_height = relative_height_;
}

float FlyingObject::currentHeight()
{
	vector<Vect3f> points(5);
	points[0] = R();
	Vect3f dx(width()/2, 0, 0);
	Vect3f dy(0, length()/2, 0);
	pose().xformPoint(dx + dy, points[1]);
	pose().xformPoint(dx - dy, points[2]);
	pose().xformPoint(-dx + dy, points[3]);
	pose().xformPoint(-dx - dy, points[4]);

	float hmax = 0;
	vector<Vect3f>::const_iterator i;
	FOR_EACH(points, i){
		float x = i -> x;
		float y = i -> y;
		float z = tri_map -> H(round(x), round(y));
		if(z > hmax)	
			hmax = z;
		}
	return hmax;
}

void FlyingObject::non_dynamic_evolve(float dt)
{
	control();
	clearForces();

	applyLocalForce(traction*k_traction, Y_AXIS);
	applyLocalTorque(k_rudder*rudder*Vlocal().y, Z_AXIS);

	float z = currentHeight() + relative_height;
	float dz = z - R().z;
	applyGlobalForce(dz*lift_bias, Z_AXIS);
	applyGlobalForce(Gravity, Z_AXIS);

	 integrateEuler(dt);
}

