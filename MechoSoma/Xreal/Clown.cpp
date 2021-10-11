#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "ControlledObject.h"
#include "Clown.h"
#include "Ball.h"
#include "AnimalArcansPrm.h"
#include "SimpleClip.h"
#include "mesh3ds.h"

void fxlabClownProcess(Vect3f& v);
void fxlabBubbleSource(Vect3f& pos,DurationTimer& timer);

Clown::Clown()
: Body(myM3DType(9), CenteredScaledBy2Bound), 
GrowingBody(0.05f, 500, 1000, 20000),
AttackAndHomeObject(100, 1)
{
	type_ = CLOWN;
	k_traction = 15;
	k_rudder = 0.8f;
	traction = 1;
	dragV.set(0.1f);
	dragW.set(0.1f);
	set_archimedean_force(clown_buoyancy);
}

void Clown::setPositionInit()
{
	AttackAndHomeObject::setPositionInit();
	place_on_the_ground();
	setLocalVelocity(Vect3f(0, 60, 20));
}

void Clown::calc_forces_and_drags()
{
	control();
	if(growing_mode() == NormalSize)
		Body::calc_forces_and_drags();
	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	applyLocalForce(k_traction, Y_AXIS);
	applyLocalTorque(k_rudder*rudder, Z_AXIS);
}

void Clown::body_overlap_reaction(Body& body) 
{
	if(body.type() == MECHOS && body.completed() && owner() != body.ID && !job_latency && body.life_time() > 6000){
		Mdisp -> attachBody(new Ball(&body));
		job_latency.start(300);
		fxlabClownProcess(Vect3f(R()));
		}
} 

void Clown::quant()
{
	if(!wave_timer && submersion())
		fxlabBubbleSource(Vect3f(R()),wave_timer);
}
