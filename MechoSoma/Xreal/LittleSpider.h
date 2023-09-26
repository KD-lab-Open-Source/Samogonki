
#ifndef __LITTLE_SPIDER__
#define __LITTLE_SPIDER__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "HydrophobicObject.h"
#pragma warning( disable : 4250 )  

class LittleSpider : public CrawlingObject, public OwnerProtection, ArcaneStatisticsService, public HydrophobicObject
{
	DurationTimer target_timer;
	DurationTimer time_to_live;
public: 
	LittleSpider();
	void quant();
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body){ return owner() != body.ID; }
};

#endif  // __LITTLE_SPIDER__
