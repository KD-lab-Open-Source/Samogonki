#ifndef __SHARK__
#define __SHARK__

#include "AnimalsPrm.h"

class Shark : virtual public AttackAndHomeObject
{
	DurationTimer rudder_timer;
public: 
	Shark() : Body(myM3DType(28), FloatsBound, shark_spheres_radius), AttackAndHomeObject(shark_distance, 1) 
	{ 
		type_ = SHARK; 
		damage_ability_ = shark_damage;
		set_archimedean_force(shark_buoyancy); 
		dragV.set(4, 0.2f, 0.5f);
		dragW.set(2);
		k_traction = shark_traction;
		k_rudder = shark_rudder;
		setAnimation(shark_animation_period);
	}
	void calc_forces_and_drags()
	{
		control();
		if(control_mode == NO_TARGET && !rudder_timer){
			rudder = frnd(1);
			rudder_timer.start(shark_rudder_duration);
			}
		applyLocalTurnTorque(Zlocal(), Vect3f::K);
		applyLocalTorque((speed_avr > 4 ? rudder : 1)*(Vlocal().y + 5)*k_rudder, Z_AXIS);
		applyLocalForce(k_traction, Y_AXIS);
	}
};

class Fish : public Shark
{
public: 
	Fish() : Body(myM3DType(29), FloatsBound, fish_spheres_radius), AttackAndHomeObject(fish_distance, 1) 
	{ 
		type_ = FISH; 
		damage_ability_ = fish_damage;
		set_archimedean_force(fish_buoyancy); 
		dragV.set(4, 0.3f, 0.5f);
		dragW.set(2);
		k_traction = fish_traction*mass;
	}
};

#endif  // __SHARK__
