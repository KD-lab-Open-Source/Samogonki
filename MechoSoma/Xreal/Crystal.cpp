////////////////////////////////////////////////////////////////////
//	Кристалл
////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Body.h"
#include "Crystal.h"
#include "Contact.h"
#include "Params.h"

#include "Mesh3ds.h"
#include "M3d_effects.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

void fxlabSpikeBurst(class Body* p);

ArcaneStatisticsInfo ArcaneStatisticsService::current_arcane;

Crystal::Crystal()
: Body (myM3DType(40)),
GrowingBody(0.1f, 3000)
{
	type_ = CRYSTAL;
	is_static_ = 1;
	is_dynamic_ = 0;
	energy = crystal_energy;
}

void Crystal::setPositionInit()
{
	setRot(Mat3f(fabsRnd(2*M_PI), Z_AXIS)*Mat3f(0.15 + fabsRnd(crystal_max_slope), X_AXIS));
	Body::setPositionInit();
}

int Crystal::is_colliding_with(const Body& body)
{
	return !body.is_static() && owner() != body.ID;
}

void Crystal::post_quant()
{
	if(growing_mode() != Growing)
		return;

	setTrans(R() - Zglobal()*(geometry -> zmaxTotal()*0.9));
	GrowingBody::post_quant();
	setTrans(R() + Zglobal()*(geometry -> zmaxTotal()*0.9));
	Body::post_quant();
}

void Crystal::body_hit_reaction(const Body& body, const Contact& c)
{
	if(!alive() || owner() == body.ID)
		return;
	if(alive())
		startSound(EFF_CRYSTAL_HIT);
}		    

void Crystal::body_overlap_reaction(Body& body)
{
	if(body.type() != MECHOS || owner() == body.ID)
		return;
	damage(crystal_damage_by_mechos);
	if(!alive())
		startSound(EFF_CRYSTAL_BREAK);
	putArcaneStatistics(body.ID);
}		    

void Crystal::kill(void)
{
	fxlabSpikeBurst(this);
	Body::kill();
};


