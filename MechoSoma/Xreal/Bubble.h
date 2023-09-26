#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"
#include "CustomFieldSources.h"

#pragma warning( disable : 4250 )  

class Bubble :  public GrowingBody, public OwnerProtection, ArcaneStatisticsService 
{
	ShareHandle<BubbleFieldSource> attractor;
	Vect3f F_wind;
	int catch_mechos;
public:
	Bubble();
	~Bubble();
	void quant(); 
	void calc_forces_and_drags();
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body) { return 0; }
	void kill();
};

#pragma warning( default : 4250 )  

#endif  // __BUBBLE_H__
