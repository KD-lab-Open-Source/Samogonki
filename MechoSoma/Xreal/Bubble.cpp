#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "Bubble.h"
#include "CustomFieldSources.h"

#include "Mesh3ds.h"
#include "M3d_effects.h"
#include "sound.h"
#include "AnimalArcansPrm.h"
#include "Params.h"

void fxlabBubbleBurst(Vect3f& v);

Bubble::Bubble()
: Body(myM3DType(39)),
GrowingBody(0.1f, bubble_time_to_grow, 100, bubble_time_to_live)
{
	type_ = BUBBLE;
	restitution_plus_one = bubble_restitution_plus_one;

	attractor = 0;

	float angle = frnd(M_PI);
	F_wind.x = sin(angle);
	F_wind.y = cos(angle);
	F_wind.z = 0;

	catch_mechos = 0;

	set_archimedean_force(bubble_buoyancy);
}

Bubble::~Bubble()
{
	if(attractor)
		Mdisp -> deleteFieldSource(attractor);
	startSound(EFF_SOAP_BUBBLE_ADD);
}

void Bubble::calc_forces_and_drags()
{
	dragV = bubble_dragV;
	dragW = bubble_dragW;
	applyGlobalForce(Gravity, Z_AXIS);
	if(!attractor || !attractor -> catch_victim()){
		applyGlobalForce(F_wind*bubble_freeWind);
		applyGlobalForce((relativeHeight() - radius() < bubble_freeHeight ? 1 : -1)*bubble_freeGravity, Z_AXIS);
		}
	else{
		applyGlobalForce(F_wind*bubble_cargoWind);
		applyGlobalForce((relativeHeight() - radius() < bubble_cargoHeight || colliding() ? 1 : -1)*bubble_cargoGravity, Z_AXIS);
		}
}

void Bubble::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && attractor && body.ID != attractor -> owner() && attractor -> catch_victim() && !catch_mechos){
		setTimeToLive(bubble_time_to_hold_mechos);
		catch_mechos = 1;
		}
//	else
//		if(body.damage_ability(*this) > 0)
//			kill();
}		
	
void Bubble::quant()
{
	if(!attractor && size > bubble_active_size){
		attractor = new BubbleFieldSource(Vect3f::ZERO, radius());
		Mdisp -> attachFieldSource(attractor);
		attractor -> set_owner(owner(), bubble_owner_protection_time);
		}
		
	if(attractor)
		Mdisp -> changeFieldSource(attractor, R(), radius());
}


void Bubble::kill()
{
	Vect3f v1(R());
	fxlabBubbleBurst(v1);
	Body::kill();
}
