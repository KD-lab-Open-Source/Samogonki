////////////////////////////////////////////////////////////////////
//	Кристалл
////////////////////////////////////////////////////////////////////
#ifndef __CRYSTAL_H__
#define __CRYSTAL_H__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"

class Crystal : public GrowingBody,  public OwnerProtection, ArcaneStatisticsService 
{
	void setPositionInit();
public:
	Crystal();
	void post_quant();
	void calc_forces_and_drags(){}
	int is_colliding_with(const Body& body);
	void body_hit_reaction(const Body& body, const Contact& c);
	void body_overlap_reaction(Body& body);
	void kill();
};

#endif // __CRYSTAL_H__
