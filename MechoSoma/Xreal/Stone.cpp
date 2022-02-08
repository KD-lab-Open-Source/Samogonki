#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "Contact.h"
#include "BaseObject.h"
#include "Stone.h"
#include "Params.h"
#include "sound.h"
#include "AnimalsPrm.h"

void fxlabStoneDust(Vect3f& p,float r);

Stone::Stone(const Vect3f& R_, const Vect3f& finish)
: Body(Mdisp -> isSnow() ? myM3DType(22) : myM3DType(1), VolumetricEquivalentBound)
{
	type_ = STONE;
	hit_damage_ability_ = 50.f;
	energy = 200;

	I *= stone_TOI_scale;
	I_inv = Vect3f::ID/I;

	setTrans(R_);
	Vect3f v = getDist(finish, R());
	v.z = 0;
	v.Normalize(stone_initial_velocity_xy);
	v.z = stone_initial_velocity_z;
	setGlobalVelocity(v);

	restitution_plus_one = stone_restitution_plus_one; 
	friction = impact_friction_factor;
	disable_collision_test = 1;
	disable_collision_timer.start(stone_disable_collision_time);
	startSound(EFF_STONE);
}						  

void Stone::quant()
{
	if(disable_collision_timer()){
		disable_collision_test = 0;
		disable_collision_timer.stop();
		}
	damage(0.5f);
}
	
void Stone::calc_forces_and_drags()
{
	Vect3f torque = Wglobal();
	torque.z = 0;
	torque.Normalize(stone_torque);
	applyGlobalTorque(torque); 
}

void Stone::body_hit_reaction(const Body& body, const Contact& c)
{
	damage(body.hit_damage_ability(*this, -c.u_n));
}

void Stone::ground_hit_reaction(const Contact& c)
{
	if(-c.u_n > stone_sound_velocity_threshould)
		startSound(EFF_ROCK_HIT);
	if(R().z < 96)
		damage(1000);
}

void Stone::damage(float decr,int damage_style)
{ 
	if(alive()){
		Body::damage(decr);
		if(!alive())
			explode(Mdisp -> isSnow() ? myM3DType(23) : myM3DType(8));
		}
}

void Stone::kill()
{
	Body::kill();
	Vect3f v1(R());
	fxlabStoneDust(v1, radius());
	startSound(EFF_ROCK_BREAK);
};

StoneGun::StoneGun(const Vect3f& A, const Vect3f& B) 
: Vect3f(A), finish(B)
{ }

void StoneGun::quant()
{
	if(!stone){
		if(!fire_latency)
			Mdisp -> attachBody( stone = new Stone(*this, finish) );
		}
	else
		if(!stone -> alive()){
			stone = 0; // delete
			fire_latency.start(3000);
			}
}


