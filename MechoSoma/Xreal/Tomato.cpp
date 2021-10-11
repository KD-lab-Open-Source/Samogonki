////////////////////////////////////////////////////////////////////
//	Кристалл
////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Body.h"
#include "Tomato.h"
#include "Contact.h"

#include "mesh3ds.h"
#include "m3d_effects.h"
#include "sound.h"
#include "AnimalArcansPrm.h"
#include "params.h"

static const float overlap_decrement = 0.005f;

void fxlabTomatoBurst(Vect3f& v);


Tomato::Tomato()
: Body(myM3DType(42)),
GrowingBody(0.2f, 2000, 15000, 37000)
{
	type_ = TOMATO;
//	is_static_ = 1;
//	is_dynamic_ = 0;
	setMass(tomato_mass);
	I *= tomato_TOI_scale;
	I_inv = Vect3f::ID/I;
	spheres.front().radius *= tomato_radius_scale;
	set_archimedean_force(tomato_buoyancy);
}

void Tomato::setPositionInit()
{
	R0 = R();
	Body::setPositionInit();
}

void Tomato::calc_forces_and_drags()
{
	dragV.set(tomato_linear_damping);
	dragW.set(tomato_angular_damping);
	if(submersion() > 0.1 && !ground_colliding){
		float phase = archimedean_force_wave_time_factor*global_time() - archimedean_force_wave_distance_factor*sqrt(sqr(R().x) + sqr(R().y));
		applyGlobalForce(tomato_swing_lift_force*sin(phase), Z_AXIS);
		applyGlobalTorque(tomato_swing_torque*sin(phase + archimedean_force_wave_phase), X_AXIS);

		Vect3f dR = getDist(R0, R());
		dR.Normalize(tomato_return_force);
		dR.z = 0;
		applyGlobalForce(dR);
		}
}	

void Tomato::body_overlap_reaction(Body& body)
{
	if(body.type() != MECHOS || owner() == body.ID)
		return;
	decrSize(overlap_decrement);
	putArcaneStatistics(body.ID);
}		    

void Tomato::kill()
{
	fxlabTomatoBurst(Vect3f(R()));	
	startSound(EFF_TOMATO_ADD);
	Body::kill();
}
