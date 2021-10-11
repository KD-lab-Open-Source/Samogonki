#include "StdAfx.h"
#include "Body.h"
#include "SimpleClip.h"
#include "Mesh3ds.h"
#include "PointControlledObject.h"
#include "Frog.h"
#include "params.h"
#include "sound.h"
#include "AnimalsPrm.h"

Frog::Frog()
: Body(myM3DType(5), CenteredBottomAlignmentedBound)
{
	type_ = FROG;
	damage_ability_ = frog_damage_ability;
	setGeometry(geometry);
	target_achieved();
}

void Frog::setPositionInit()
{
	place_on_the_ground();
	home = R();
	ControlledObject::setPositionInit();
}

void Frog::calc_forces_and_drags()
{
	control();
	if(colliding()){
		dragV.set(frog_contact_damping);
		dragW.set(frog_contact_damping);
		}
	else{
		dragV.set(free_linear_damping);
		dragW.set(frog_fly_damping_W);
		}
	if(traction && getFrame() > frog_start_jump_phase && getFrame() < frog_stop_jump_phase){
		Vect3f dr = Yglobal();
		dr.z = 0;
		dr.normalize();
		Vect3f v = dr*frog_xy_impulse;
		if(R().z < 300)
			v.z += frog_z_impulse;
		setGlobalVelocity(v);
		}
		
	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	setGlobalAngularVelocity(frog_rudder*rudder*sqrt(sqr(Vlocal().x) + sqr(Vlocal().y)), Z_AXIS);
}

void Frog::quant()
{
	float frame = getFrame();
	if(traction || frame > frog_start_jump_phase)
		frame += frog_phase_incr;
	if(frame < 1.f)
		setFrame(frame);
	else
		if(colliding())
			setFrame(0);
	if(geometry -> FindFrame() -> IsPhasePassed(frog_start_jump_phase))
		startSound(EFF_FROG_JUMP);

	if(!leave_home)
		traction = 1;
}

void Frog::target_achieved()
{
	if(getDist(R(), home).norm() < radius())
		setTarget(home + Vect3f().setSpherical(frnd(M_PI), M_PI/2, frog_rest_time));
	else
		setTarget(home);
	leave_home.start(random(frog_rest_time));
	traction = 0;
}

////////////////////////////////////////////////////////////////////////////////
//		       FrogRacer
////////////////////////////////////////////////////////////////////////////////
FrogRacer::FrogRacer() 
: Body(myM3DType(5), CenteredBottomAlignmentedBound),
track("WaterLeaves.sst")
{
	jumping = 0;
	disable_traction_modulation = 1;
	ignore_point_control = 1; 
}

void FrogRacer::setPositionInit() 
{ 
	PointControlledObject::setPositionInit(); 
	Frog::setPositionInit(); 
	traction = 1;
}

void FrogRacer::calc_forces_and_drags()
{
	ControlledObject::control();
	if(jumping){
		if(!recover_timer)
			setTrans(target);
		Vect3f r = getDist(target, R());
		Alg().invXform(r);
		float y1 = sqrt(sqr(r.x) + sqr(r.y)) + 1;
		float z1 = r.z;
		float vz = (2.0*z1*frog_Vy*frog_Vy + frog_az*y1*y1)/y1/frog_Vy/2.;
		setLocalVelocity(Vect3f(0, frog_Vy, vz) );
		dragV.set(free_linear_damping);
		dragW.set(frog_fly_damping_W);
		if(r.norm() < frog_radius){
			jumping = 0;
			setAnimation(frog_stop_animation_period, frog_stop_jump_phase, frog_start_jump_phase);
			stop_timer.start(frog_stop_animation_period*fmod(frog_start_jump_phase + 1 - frog_stop_jump_phase, 1));
			}
		}
	else{
		dragV.set(frog_contact_damping);
		dragW.set(frog_contact_damping);

		if(colliding() && !stop_timer && traction){
			setAnimation(frog_jump_animation_period, frog_start_jump_phase, frog_stop_jump_phase);
			jumping = 1;
			startSound(EFF_FROG_JUMP);
			setTarget((++track)());
			recover_timer.start(frog_racer_recover_time);
			}
		}
		
	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	setGlobalAngularVelocity(frog_rudder*rudder*(fabs(Vlocal().y) + 5), Z_AXIS);
}

void FrogRacer::quant()
{
}

void FrogRacer::show() const
{
	ControlledObject::show();
//	show_vector(target, 2, XCOL(CYAN, 200));
//	show_map_pixel(R(), CYAN);
//	show_map_line(R(), target, CYAN);
//	SimpleTrack::const_iterator i;
//	FOR_EACH(track, i)
//		show_map_pixel(*i, CYAN);
}
