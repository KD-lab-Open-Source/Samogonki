////////////////////////////////////////////////////////////////////
//	Кристалл
////////////////////////////////////////////////////////////////////
#ifndef __TOMATO_H__
#define __TOMATO_H__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"

#pragma warning( disable : 4250 )  

class Tomato : virtual public Body, GrowingBody, public OwnerProtection, ArcaneStatisticsService 
{
	Vect3f R0;
public:
	Tomato();
	void setPositionInit();
	void calc_forces_and_drags();
	int is_colliding_with(const Body& body){ return owner() != body.ID; }
	void body_overlap_reaction(Body& body);
	void kill();
};

#pragma warning( default : 4250 )  
#endif // __TOMATO_H__
