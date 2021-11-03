#include "StdAfx.h"
#include "Body.h"
#include "PointControlledObject.h"
#include "SmartStone.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "sound.h"
#include "SimpleClip.h"

void fxlabStoneDust(Vect3f& p,float r);

SmartStoneRacer::SmartStoneRacer() 
: Body(myM3DType(2)), 
track("StoneBossPoints.sst")
{
	type_ = SMART_STONE;
	disable_traction_modulation = 1;
	damage_ability_ = smart_stone_damage_ability;
	psi_ = 0;

	friction = impact_friction_factor;

	catch_width = catch_length = radius();

	state = Rolling;

	effect_geometry = xrealM3D -> CreateObject(myM3DType(43));
	xrealM3D -> Detach(effect_geometry); 
}

SmartStoneRacer::~SmartStoneRacer() 
{
	xrealIVG -> ReleaseObject(effect_geometry);
}

void SmartStoneRacer::setPositionInit()
{
	PointControlledObject::setPositionInit();
	psi_ = psi();
}

void SmartStoneRacer::calc_forces_and_drags()
{
	Mat3f A(Alg());
	setRot(-psi_, Z_AXIS);
	control(); 
	setRot(A);

	if(state == Rolling){
		Vect3f axis(-cos(psi_), sin(psi_), 0);
		Vect3f Vn, Vt;
		decomposition(axis, Vglobal(), Vn, Vt);
		setGlobalVelocity(Vt + smart_stone_restitution*Vn);
		Vect3f Wn, Wt;
		decomposition(axis, Wglobal(), Wn, Wt);
		Wn = axis*traction*smart_stone_traction;
		setGlobalAngularVelocity(smart_stone_restitution*Wt + Wn); 
		psi_ -= rudder*SIGN(traction)*smart_stone_rudder;
		psi_ = SIGN(psi_)*fmod(fabs(psi_), M_PI);
		float theta = atan2(Alg()[2][1], Alg()[2][2]);
		setRot(Mat3f(-psi_, Z_AXIS)*Mat3f(theta, X_AXIS));
		}
	else{
		Vect3f dr = getDist(track(), (Vect3f)R());
		dr.Normalize(smart_stone_fly_velocity);
		setGlobalVelocity(dr);
		applyGlobalForce(Gravity*smart_stone_antigravity_factor, Z_AXIS);
		}
}

void SmartStoneRacer::quant()
{
	Vect3f dr = getDist((Vect3f)R(), track());
	dr.z = 0;
	if(dr.norm() < smart_stone_catch_point_distance){
		switch(state){
			case Rolling: 
				state = Flying;
				{
					Vect3f v1(R());
					fxlabStoneDust(v1, radius());
				}
				break;
			case Flying:
				state = Rolling;
				break;
			}
		swap(geometry, effect_geometry);
		xrealM3D -> Attach(geometry); 
		xrealM3D -> Detach(effect_geometry); 
		++track;
		}

	if(speed_avr > 5)
		startSound(EFF_ROCK_MASTER);
	else
		stopSound(EFF_ROCK_MASTER);
}

void SmartStoneRacer::ground_hit_reaction(const Contact& c)
{
	if(-c.u_n > stone_sound_velocity_threshould)
		startSound(EFF_ROCK_HIT);
}

void SmartStoneRacer::show() const 
{
	SimpleTrack::const_iterator ti;
	FOR_EACH(track, ti)
		show_vector(*ti, 3, XCOL(CYAN, 150));
	show_vector(track(), 5, XCOL(CYAN, 200));
}
