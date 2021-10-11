#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "ControlledObject.h"
#include "CrawlingObject.h"
#include "Worm.h"
#include "Mesh3ds.h"
#include "Sound.h"

void fxlabGreenSlime(const Vect3f& v, const MatXf& mat);
void fxlabYellowSlime1(const Vect3f& v, const MatXf& mat);

Worm::Worm()
: Body(myM3DType(3))
{
	type_ = WORM;
	traction = 1;
	k_traction = 1;
	k_rudder = 0.1f;
	mesh_offset.z = 3;
	search_closest_triangle_delay = 4;
}

void Worm::quant()
{
	if(!rudder_latency){
		setInverseTarget(Mdisp -> searchBody(R(), WORM, this));
		rudder_latency.start(2000 + random(3000));
		//rudder = frnd(1.);
		}
	if(findFrame().IsPhasePassed(0))
		startSound(EFF_WORM);
}

void Worm::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS){
		fxlabGreenSlime(R(), pose());
		startSound(EFF_WORM_ADD);
		kill();
		}
}

///////////////////////////////////////////////////////////////////////
//		Caterpillar
///////////////////////////////////////////////////////////////////////
Caterpillar::Caterpillar()
: Body(myM3DType(14))
{
	type_ = CATERPILLAR;
	traction = 1;
	k_traction = 1;
	k_rudder = 0.1f;
	mesh_offset.z = 3;
	time_to_live.start(20000);
	search_closest_triangle_delay = 4;
}

void Caterpillar::quant()
{
	if(!rudder_latency)
		rudder = frnd(1.);
	if(findFrame().IsPhasePassed(0))
		startSound(EFF_WORM);
	if(!time_to_live)
		kill();
	HydrophobicObject::quant();
}

void Caterpillar::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && owner() != body.ID){
		putArcaneStatistics(body.ID);
		fxlabYellowSlime1(R(), pose());
		startSound(EFF_WORM_ADD);
		kill();
		}
}
