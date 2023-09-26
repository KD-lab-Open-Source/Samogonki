
#ifndef __WORM__
#define __WORM__

#include "CrawlingObject.h"
#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "HydrophobicObject.h"

class Worm : public CrawlingObject
{
	DurationTimer rudder_latency;
public: 
	Worm();
	void quant();
	void body_overlap_reaction(Body& body);
};

class Caterpillar : public CrawlingObject, public OwnerProtection, ArcaneStatisticsService, public HydrophobicObject
{
	DurationTimer rudder_latency;
	DurationTimer time_to_live;
public: 
	Caterpillar();
	void quant();
	void body_overlap_reaction(Body& body);
};



#endif  // __WORM__
