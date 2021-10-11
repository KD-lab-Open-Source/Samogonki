#ifndef __Dog__
#define __Dog__

#include "CustomFieldSources.h"
#include "I-World.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "SimpleClip.h"
#include "snd_ids.h"

class Dog : public Pig
{
	ShareHandle<SensorFieldSource> sensor;
public: 
	Dog() : Body(myM3DType(27), CenteredBottomAlignmentedBound), AttackAndHomeObject(400)
	{ 
		type_ = DOG; 
		traction = 0; 
		k_rudder = 0; 
		sensor = 0;
		setAnimation(dog_animation_period);
	}
	~Dog()
	{
		if(sensor)
			Mdisp -> deleteFieldSource(sensor);
	}
	void control()
	{
		traction = 0;
	}
	void quant()
	{
		if(!sensor){
			sensor = new SensorFieldSource(Vect3f::ZERO, radius()*dog_sensor_radius, IW_DOGGY_SENSOR);
			Mdisp -> attachFieldSource(sensor);
			}
		Mdisp -> changeFieldSource(sensor, R() + Alg()*dog_sensor_offset*radius(), radius()*dog_sensor_radius);
		Body* b = Mdisp -> searchBody(R(), MECHOS);
		if(b && getDist(b -> R(), R() + Alg()*dog_sensor_offset*radius()).norm() < dog_activation_distance){
			setChannel("talk");
			startSound(EFF_ANDROID);
			}
		else{
			setChannel("main");
			stopSound(EFF_ANDROID);
			}
	}
	void body_hit_reaction(const Body& body, const Contact& c)
	{
		startSound(EFF_DOG_ROAR, -c.u_n*dog_hit_velocity2sound_factor);
	}		    
};

#endif  // __Dog__
