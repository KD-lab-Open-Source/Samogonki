// StaticBonus.cpp: implementation of the StaticBonus class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StaticBonus.h"
#include "Mechos.h"
#include "AnimalArcansPrm.h"

StaticBonus::StaticBonus(int bonusID_)
{
	type_ = STATIC_BONUS; 
	bonusID = bonusID_; 
	is_dynamic_ = 0;
}

void StaticBonus::setPosition(const Vect3f& pose, float psi)
{
	setPose(MatXf(Mat3f(-psi, Z_AXIS), pose));
	Body::setPositionInit();
}

void StaticBonus::quant()
{
	if(birth_timer()){
		birth_timer.stop();
		xrealM3D -> Attach(geometry); 
		completed_ = 1;
		}
}

void StaticBonus::body_overlap_reaction(Body& body)
{
	if(completed() && body.type() == MECHOS && dynamic_cast<Mechos&>(body).CheckBonusSlot(bonusID)){
		dynamic_cast<Mechos&>(body).AddBonus(bonusID);
		fxlabTakeBonus(bonusID, R());
		birth_timer.start(static_bonus_birth_delay);
		xrealM3D -> Detach(geometry); 
		completed_ = 0;
		}
}

