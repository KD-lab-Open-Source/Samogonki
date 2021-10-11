
#ifndef __CUICIDE__
#define __CUICIDE__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"
#include "TrackIterator.h"

#pragma warning( disable : 4250 )  

class Suicide : public PointControlledObject, public GrowingBody, public OwnerProtection, ArcaneStatisticsService 
{
	AverageConditionTimer average_stop;
	TrackIterator initial_position, current_position;
	int laps;
public: 
	Suicide();
	~Suicide();
	void quant();
	void calc_forces_and_drags();
	int is_colliding_with(const Body& body){ return owner() != body.ID; }
	void ground_hit_reaction(const Contact& c);
	void body_overlap_reaction(Body& body);
	void setPositionInit();
	void add_points();
	void kill();
};

#pragma warning( default : 4250 )  

#endif  // __CUICIDE__
