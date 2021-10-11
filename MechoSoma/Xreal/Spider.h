
#ifndef __SPIDER__
#define __SPIDER__

#include "race.h"
#pragma warning( disable : 4250 )  

class Spider : virtual public AttackAndHomeObject 
{
	float phase;
	Vect3f normal;
	AverageConditionTimer not_colliding_timer;

protected:
	void post_quant();
	void calc_forces_and_drags();
	void setPositionInit();

public: 
	Spider();
};

class SpiderRacer : public Spider, public mchBossRacer
{
	void setPositionInit() { PointControlledObject::setPositionInit(); Spider::setPositionInit(); }
	void control() { PointControlledObject::control(); }
	void show() const { PointControlledObject::show(); }
	void quant();
public:	
	SpiderRacer();
};

#endif  // __SPIDER__
