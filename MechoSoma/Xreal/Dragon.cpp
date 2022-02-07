#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "Dragon.h"
#include "M3d_effects.h"
#include "Mesh3ds.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "M3d_effects.h"

Dragon::Dragon()
: Body(myM3DType(36), CenteredBottomAlignmentedBound),
  FirePointBody(geometry)
{
	type_ = DRAGON;
	traction = 1;
	rudder = 0;
	k_traction = dragon_traction;
	k_rudder = dragon_rudder;
	dragV = dragon_dragV;
	dragW = dragon_dragW;
	setHeight(dragon_height);
	lift_bias = dragon_lift_bias;
}

void Dragon::control()
{
	if(control_mode == NO_TARGET){
		if(!skip_latency){
			rudder = frnd(0.2);
			skip_latency.start(3000);
			}
		}
	else
		if(!skip_latency)
			ControlledObject::control();

	if(!latency){
		if(control_mode == NO_TARGET){
			setTarget(Mdisp -> searchBody(R(), MECHOS, dynamic_target));
			}
		else{
			control_mode = NO_TARGET;
			rudder = 0;
			}
		latency.start(control_mode == NO_TARGET ? 10000 : 30000);
		}
}

void Dragon::target_achieved()
{
	skip_latency.start(3000);
}

//////////////////////////////////////////////////////////////////////////////////////
//		DragonRacer
//////////////////////////////////////////////////////////////////////////////////////
DragonRacer::DragonRacer() 
: Body(myM3DType(36), CenteredBottomAlignmentedBound) 
{
	during_start = 0;
	disable_traction_modulation = 1;
	k_traction = dragon_racer_traction;
	allowed_width = catch_width = dragon_width;
	catch_length = dragon_length;
}

void DragonRacer::set_brake(int on) 
{ 
	during_start = on; 
}

void DragonRacer::control() 
{ 
	if(!during_start)
		PointControlledObject::control(); 
	else{
		if(!latency && control_mode == NO_TARGET)
			setTarget(Mdisp -> searchBody(R(), MECHOS));
		ControlledObject::control();
		traction = 0.5;
		}
}

void DragonRacer::target_achieved()
{
	PointControlledObject::target_achieved();
	skip_latency.start(500);
}
