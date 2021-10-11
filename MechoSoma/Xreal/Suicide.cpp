#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "PointControlledObject.h"
#include "Suicide.h"
#include "SimpleClip.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

void fxlabSuicideExplosion(class Body* p);

Suicide::Suicide()
: Body(myM3DType(11), CenteredScaledBy2Bound),
GrowingBody(0.1f, suicide_time_to_grow, 0, !interface_world_now ? suicide_time_to_live : suicide_time_to_live_interface)
{
	type_= SUICIDE;
	bbox_.x = suicide_bbox_x;
	bbox_.y = suicide_bbox_y;
}

Suicide::~Suicide()
{
	stopSound(EFF_SUICIDE);
}


void Suicide::setPositionInit()
{
	PointControlledObject::setPositionInit();

	laps = suicide_laps;
	current_position = initial_position.set(R().xi(), R().yi());
	add_points();
	startSound(EFF_SUICIDE_ADD);
	setLocalVelocity(suicide_initial_Vy, Y_AXIS);
}

void Suicide::add_points()
{
	if(!laps)
		return;
	int n = num_control_points();
	while(n < 8){
		if(++current_position == initial_position)
			laps--;
		if(++current_position == initial_position)
			laps--;
		add_control_point(current_position);
		n++;
		}
}

void Suicide::quant()
{
	startSound(EFF_SUICIDE);

	if(average_stop(Vlocal().y < suicide_average_stop_velocity, suicide_average_stop_delay))
		kill();
}

void Suicide::ground_hit_reaction(const Contact& c)
{
	if(-c.u_n > suicide_impact_velocity && fabs(dot(Alg().col(Y_AXIS), c.normal)) > suicide_impact_normal_y) // local_normal.y
		kill();
}

void Suicide::calc_forces_and_drags()
{
	if(control_mode == NO_TARGET){
		//if(!num_control_points())  // все семена закончились
		//	kill();
		add_points();
		PointControlledObject::control();
		Body* b = Mdisp -> searchBody(R(), MECHOS);
		if(b && b -> ID == owner())	
			b = Mdisp -> searchBody(R(), MECHOS, b);
		if(b && getDist(b -> R(), R()).norm() < suicide_max_distance_to_attack_mechos)
			setTarget(b);
		}
	else
		ControlledObject::control();

	dragV.set(suicide_dragVx, suicide_dragVy, suicide_dragVz);
	dragW.set(suicide_dragWxy, suicide_dragWxy, suicide_dragWz);

	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	applyLocalForce(suicide_traction, Y_AXIS);
	applyLocalTorque(suicide_rudder*rudder*Vlocal().y, Z_AXIS);
}

void Suicide::body_overlap_reaction(Body& body) 
{
	if(body.type() == MECHOS && owner() != body.ID){
		body.damage(1000);
		kill();
		}
} 

void Suicide::kill()
{
	fxlabSuicideExplosion(this);
	Body::kill();
}
