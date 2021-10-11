#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "SimpleClip.h"
#include "Pig.h"
#include "Ball.h"
#include "xreal_utl.h"

Pig::Pig()
: Body(myM3DType(24), CenteredBottomAlignmentedBound),
AttackAndHomeObject(500, 1)
{
	type_ = PIG;
	k_traction = 8;
	k_rudder = 0.8f;
	traction = 1;
	setAnimation(800);
}

void Pig::setPositionInit()
{
	AttackAndHomeObject::setPositionInit();
	place_on_the_ground();
}

void Pig::calc_forces_and_drags()
{
	control();
	int good_normal = 0;
	Vect3f n = Zlocal();
	if(colliding()){
		ContactList::iterator ci;
		FOR_EACH(contacts, ci){
			if(!ci -> body2 && ci -> normal.z > 0.5){
				good_normal = 1;
				Alg().invXform(ci -> normal, n);
				break;
				}
			}
		}
	applyLocalTurnTorque(n, Vect3f::K);
	applyLocalTorque(k_rudder*rudder, Z_AXIS);

	if(good_normal){
		setLocalVelocity(k_traction*traction, Y_AXIS);
		dragV.set(4, 0.4f, 0.4f);
		}
	else
		dragV.set(4, 0.1f, 0.1f);
	dragW.set(1);
}

void Pig::quant()
{
	if(Zglobal().z < 0)
		setRot(-psi(), Z_AXIS);
}
