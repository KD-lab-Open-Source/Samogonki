
#ifndef __WHALE__
#define __WHALE__

#include "race.h"
#include "FirePoint.h"

#pragma warning( disable : 4250 )  

class WhaleRacer : public mchBossRacer, public FirePointBody
{
	int during_start;
	DurationTimer wave_timer;
	DurationTimer jump_timer;
	DelayTimer jump_reverse_timer;
	Vect3f jump_axis;
public: 
	WhaleRacer();
	void post_quant();
	void calc_forces_and_drags();
	void setPositionInit();
	void set_brake(int on){ during_start = on; }
};

#endif  // __WHALE__
