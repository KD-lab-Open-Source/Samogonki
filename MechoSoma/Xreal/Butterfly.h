
#include "FlyingObject.h"
#include "FirePoint.h"

#pragma warning (disable:4250)

class Butterfly : public FlyingObject, public FirePointBody
{
	DurationTimer rudder_latency;
public: 
	Butterfly();
	void quant();
};

class WickedButterfly : public Butterfly, public AttackAndHomeObject
{
public: 
	WickedButterfly();
	void setPositionInit(){ AttackAndHomeObject::setPositionInit(); FlyingObject::setPositionInit(); }
	void control();
	int is_colliding_with(const Body& body) { return 0; }
};
