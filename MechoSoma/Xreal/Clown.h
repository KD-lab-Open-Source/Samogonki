
#ifndef __CLOWN__
#define __CLOWN__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"

#pragma warning( disable : 4250 )  

class Clown : public AttackAndHomeObject, public GrowingBody, public OwnerProtection, ArcaneStatisticsService 
{
public: 
	Clown();
	void quant();
	void calc_forces_and_drags();
	int is_colliding_with(const Body& body){ return owner() != body.ID; }
	void body_overlap_reaction(Body& body);
	void setPositionInit();

private:
	DurationTimer job_latency;
	DurationTimer wave_timer;
};

#pragma warning( default : 4250 )  

#endif  // __CLOWN__
