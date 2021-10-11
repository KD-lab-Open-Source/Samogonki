////////////////////////////////////////////////////////////////////
//	Метеорит
////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Mechos.h"
#include "Meteorite.h"
#include "Params.h"
#include "SimpleClip.h"
#include "m3d_effects.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

Meteorite::Meteorite(int m3d, const Vect3f& R_, const Vect3f& V_, float damage_, int owner_ID)
: Body(m3d),
OwnerProtection(owner_ID)
{
	type_ = METEORITE;
	damage_ability_ = damage_;
	setTrans(R_);
	setGlobalVelocity(V_*(Gravity*0.1f));
	Vect3f w;
	w.x = frnd(meteorite_W);
	w.y = frnd(meteorite_W);
	w.z = frnd(meteorite_W);
	w.Normalize(meteorite_W);
	setGlobalAngularVelocity(w);
	max_velocity_threshould = max_velocity_threshould_meteorite;
	disable_collision_mask = 2;
}	

void Meteorite::scaleRadius(float factor)
{
	spheres.front().radius *= factor;
}

void Meteorite::calc_forces_and_drags()
{
	Body::calc_forces_and_drags();
	if(R().z < 0 || colliding()){ //  столкновение с поверхностью
		setTrans(R_prev);
		kill();
		return;
		}
}

void Meteorite::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner())
		kill();
}

void StatisticalMeteorite::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		putArcaneStatistics(body.ID);
		kill();
		}
}

BonusMeteorite::BonusMeteorite(int m3d_model, const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID)
: StatisticalMeteorite( m3d_model, R_, V_, damage__, owner_ID) 
{ 
	type_ = BONUS_METEORITE; 
	set_archimedean_force(bonus_meteorite_buoyancy); 
}

void BonusMeteorite::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner())
		kill();
}

void RealBonusMeteorite::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		if(dynamic_cast<Mechos&>(body).CheckBonusSlot(bonus_ID))
			dynamic_cast<Mechos&>(body).AddBonus(bonus_ID);
		kill();
		}
}

