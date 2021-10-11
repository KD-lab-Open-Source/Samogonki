
#ifndef __FROG__
#define __FROG__

#include "race.h"
#include "SimpleTrack.h"

class Frog : virtual public ControlledObject
{
protected:
	DurationTimer jump_latency;
	DurationTimer jump_timer;
	DurationTimer leave_home;
	Vect3f home;
	
	void quant(); 
	void setPositionInit();
	void calc_forces_and_drags();
	void target_achieved();
public:
	Frog();
};

#pragma warning( disable : 4250 )  

class FrogRacer : public Frog, public mchBossRacer
{
	int jumping;
	DurationTimer stop_timer;
	SimpleTrack track;
	DurationTimer recover_timer;

	void quant();
	void calc_forces_and_drags();
	void setPositionInit();
	void target_achieved(){ ControlledObject::target_achieved(); }
	void show() const;

public:	
	FrogRacer();
};

#pragma warning( default : 4250 )  

#endif // __FROG__
