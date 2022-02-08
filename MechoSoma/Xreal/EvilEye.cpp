#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "EvilEye.h"
#include "Mesh3ds.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

void fxlabTitaniumExplosion(Body* p);

EvilEye::EvilEye()
: Body(myM3DType(10), CenteredScaledBy2Bound)
{
	type_ = EVIL_EYE;
	//attack_latency.start(evil_eye_attack_latency_time); // ждет до первой атаки
	setAnimation(evil_eye_period);
	time_to_live.start(evil_eye_time_to_live);
	make_non_dynamic = 0;
	set_archimedean_force(evil_eye_buoyancy);
}

EvilEye::~EvilEye()
{
	stopSound(EFF_EVIL_EYE_ADD);
}

void EvilEye::release_body_handles()
{
//	victim = 0;
}

void EvilEye::setPositionInit()
{
	Body::setPositionInit();
	startSound(EFF_EVIL_EYE);
}

void EvilEye::body_overlap_reaction(Body& body)
{
	if(!attack_latency && is_dynamic() && body.type() == MECHOS && owner() != body.ID && body.visible()){
		make_non_dynamic = 1;
		Xem.invert(body.pose());
		Xem.postmult(pose());
		Vect3f& r = Xem.trans();
		float z = r.norm()*evil_eye_joining_z_factor; 
		if(r.z < z)
			r.z = z;
		float d = (radius() + body.radius())*evil_eye_joining_distance_factor;
		if(r.norm() > d)
			r.normalize(d);
		victim = &body;
		time_to_explode.start(min(time_to_live(), evil_eye_time_to_explode)); // Время до взрыва
		startSound(EFF_EVIL_EYE_ATTACK);
		}
} 

void EvilEye::non_dynamic_evolve(float dt)
{
	// Прилипший
	setPose(victim -> pose()*Xem);
	if(victim -> completed()){
		if(time_to_explode()){ // пришло время
			victim -> damage(1000);
			fxlabTitaniumExplosion(this);
			kill();  // взрывает мехос
			}
		else{
			if(dynamic_cast<Mechos*>(victim()) -> isFlyingByArcane() || !victim -> visible()){ // отлипает
				is_dynamic_ = 1;
				make_non_colliding();
				attack_latency.start(evil_eye_attack_latency_time); // ждет до следующей атаки
				stopSound(EFF_EVIL_EYE_ADD);
				}
			}
		}
	else
		kill(); // мехос развалился сам
}

void EvilEye::calc_forces_and_drags()
{ 
	Body::calc_forces_and_drags();

	// Ждущий жертву
	if(!attack_latency && !jump_latency && (colliding() || submersion() > 0.1)){ 
		Body* body = Mdisp -> searchBody(R(), MECHOS);
		if(body && (!body -> completed() || body -> ID == owner())){
			body = Mdisp -> searchBody(R(), MECHOS, body);
			if(body && !body -> completed())
				body = 0;
			}
		if(body){
			Vect3f dr = getDist(body -> R(), R());
			if(dr.norm() < evil_eye_attack_distance){ // попал в зону атаки
				dr.z += evil_eye_delta_z;
				dr *= evil_eye_velocity_factor;
				setGlobalVelocity(dr);
				jump_latency.start(evil_eye_jump_latency_time);
				startSound(EFF_EVIL_EYE_JUMP);
				}
			if(dr.norm() < evil_eye_catch_distance) 
				body_overlap_reaction(*body);
			}
		}
}		 

void EvilEye::quant()
{
	startSound(EFF_EVIL_EYE_ADD);
	float t = (sin(2*M_PI*getFrame()) + 1.)/2.;
	setColor(1, t, t);
	setScale(1.f + (1.f - time_to_live()/evil_eye_time_to_live)*(evil_eye_finish_size - 1.f));
	if(make_non_dynamic){
		make_non_dynamic = 0;
		is_dynamic_ = 0;
		}
	if(!time_to_live){
		fxlabTitaniumExplosion(this);
		kill();
		}
}
