#ifndef __EVIL_EYE__
#define __EVIL_EYE__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"

class EvilEye : public Body, public OwnerProtection, ArcaneStatisticsService 
{
public:
	EvilEye();
	~EvilEye();
	void quant();
	void calc_forces_and_drags();
	void non_dynamic_evolve(float dt);
	int is_colliding_with(const Body& body){ return owner() != body.ID && is_dynamic();  }
	void body_overlap_reaction(Body& body);
	void setPositionInit();
	void release_body_handles();

private:
	MatXf Xem;
	ShareHandle<Body> victim;
	DurationTimer time_to_live;
	DurationTimer attack_latency;
	DurationTimer jump_latency;
	DelayTimer time_to_explode;
	MeasurementTimer color_time;
	AverageConditionTimer not_colliding_avr;
	int make_non_dynamic;
};


#endif  // __EVIL_EYE__

