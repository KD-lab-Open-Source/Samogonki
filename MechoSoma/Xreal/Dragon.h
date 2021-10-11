
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include "FlyingObject.h"
#include "FirePoint.h"
#include "race.h"
#pragma warning( disable : 4250 )  

class Dragon : public FlyingObject, public FirePointBody 
{
protected:	
	DurationTimer latency;
	DurationTimer skip_latency;

public: 
	Dragon();
	void control();
	void target_achieved();
};

class DragonRacer : public Dragon, public mchBossRacer
{
	int during_start;

	void setPositionInit() { PointControlledObject::setPositionInit(); }
	void target_achieved();
	void control();
	void set_brake(int on);
public:	
	DragonRacer();
};

#endif // _DRAGON_H_
