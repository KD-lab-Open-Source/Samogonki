
////////////////////////////////////////////////////////////////////
//	Титановый шарик
////////////////////////////////////////////////////////////////////
#ifndef __TITANIUM_BALL__
#define __TITANIUM_BALL__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "GrowingBody.h"

#pragma warning( disable : 4250 )  

class TitaniumBall : virtual public Body, GrowingBody, public OwnerProtection, ArcaneStatisticsService 
{
public:
	TitaniumBall(const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID);
	void calc_forces_and_drags() {}
	int is_colliding_with(const Body& body){ return owner() != body.ID; }
	void body_overlap_reaction(Body& body)
	{
		if(body.type() == MECHOS && body.ID != owner())
			putArcaneStatistics(body.ID);
	}
	void kill();
};

#pragma warning( default : 4250 )  

#endif // __TITANIUM_BALL__
