#include "StdAfx.h"
#include "Body.h"
#include "ControlledObject.h"
#include "Ship.h"
#include "M3d_effects.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "Xreal_utl.h"

Ship::Ship()
: Body(myM3DType(37), FloatsBound, ship_spheres_radius),
  AttackAndHomeObject(ship_guard_distance, 1, ship_turn_delay),
  FirePointBody(geometry)
{
	type_ = SHIP;
	traction = 1;
	rudder = .8f;

	set_archimedean_force(ship_buoyancy);

	hit_damage_ability_ = ship_damage;
}

void Ship::setPositionInit()
{
	AttackAndHomeObject::setPositionInit();
	place_on_the_ground();
}

void Ship::calc_forces_and_drags()
{
	control();
	traction = 1;

	float drag = -1.5*submersion() + 2;
	dragV.set(drag);
	dragW.set(drag);

	dragV.x += 4;
	dragW.z += 4;

	applyLocalTorque(ship_rudder*rudder, Z_AXIS);
	applyLocalForce(ship_traction*traction, Y_AXIS);
}	


