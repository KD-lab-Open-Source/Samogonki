#include "StdAfx.h"
#include "Body.h"
#include "Spider.h"
#include "m3d_effects.h"
#include "sound.h"
#include "params.h"
#include "AnimalsPrm.h"
#include "SimpleClip.h"


Spider::Spider()
: Body(myM3DType(38), CenteredBottomAlignmentedBound),
AttackAndHomeObject(spider_max_distance, 1)
{
	type_ = SPIDER;
	traction = 0;
	k_traction = spider_traction;
	k_rudder = spider_rudder;
	damage_ability_ = 1000;
}

void Spider::setPositionInit()
{
	AttackAndHomeObject::setPositionInit();
	normal = Vect3f::K;
}

void Spider::post_quant()
{
	Body::post_quant();
	Vect3f dr = getDist(R(), R_prev);
	Alg().invXform(dr);
	phase += spider_foot_speed_factor*dr.y/radius();
	phase = fmod(phase + 100, 1);
	setAnimation(0, phase);
	updateSound(EFF_SPIDER);
}

void Spider::calc_forces_and_drags()
{
	control();
	float zmin = DBL_INF;
	int not_colliding = not_colliding_timer(!colliding(), spider_not_colliding_avr_time);
	if(not_colliding)
		normal = Vect3f::K;
	ContactList::iterator ci;
	FOR_EACH(contacts, ci)
		if(zmin > ci -> cp1.z){
			zmin = ci -> cp1.z;
			normal = ci -> normal;
			}

	Vect3f n_local;
	Alg().invXform(normal, n_local);
	//applyLocalTurnTorque(n_local, Vect3f::K);
	applyLocalTurnTorque(n_local, spider_normal_target, spider_levelling_factor);
	if(control_mode != NO_TARGET){
		//Vect3f dr = getDist(target, R());
		//dr.z = 0;
		//Alg().invXform(dr);
		//applyLocalTurnTorque(dr, Vect3f::J, k_rudder);
		applyGlobalTorque(k_rudder*rudder, Z_AXIS);
		}

	if(!not_colliding){
		setLocalVelocity(k_traction*traction, Y_AXIS);
		}
	applyGlobalForce(Gravity*spider_antigravity_factor, Z_AXIS);
	applyLocalForce(-Gravity*spider_stick_factor, Z_AXIS);
	    
	dragV = spider_dragV;
	dragW = spider_dragW;
}


SpiderRacer::SpiderRacer()
: Body(myM3DType(38), CenteredBottomAlignmentedBound),
AttackAndHomeObject(10000, 0)
{
	type_ = SPIDER_RACER;
	//check_points_only_controlled_ = 1;
	disable_traction_modulation = 1;
	k_traction = spider_boss_traction;
	k_rudder = spider_boss_rudder;
	catch_width = spider_catch_width;
	catch_length = spider_catch_length;
	damage_ability_ = 0;
}

void SpiderRacer::quant()
{
	Spider::quant();
	k_traction = spider_boss_traction;
	k_rudder = spider_boss_rudder;
}
