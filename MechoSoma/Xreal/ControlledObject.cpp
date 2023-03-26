#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "ControlledObject.h"
#include "Params.h"
#include "DebugPrm.h"

ControlledObject::ControlledObject() 
{	
	control_mode = NO_TARGET; 
	traction = rudder = 0; 
	brake = 0;
	k_rudder = 0.1f;
	k_traction = 50;
}

void ControlledObject::release_body_handles()
{
	control_mode = NO_TARGET; 
	dynamic_target = 0;
}

void ControlledObject::setTraction(float traction_)
{
	average(traction, traction_, 0.22f);
}

void ControlledObject::setRudder(float rudder_)
{
//	average(rudder, rudder_, 0.22);
	rudder =  rudder_;
	if(fabs(rudder_) < 0.05)
		rudder = 0;
}

void ControlledObject::setPositionInit()
{
	Body::setPositionInit();
	target = R();
}
	
void ControlledObject::setTarget(const Vect3f& r)
{
	control_mode = STATIC_TARGET;
	target = r;
}
	
void ControlledObject::setTarget(const Body* b)
{
	control_mode = b ? DYNAMIC_TARGET : NO_TARGET;
	dynamic_target = const_cast<Body*>(b);
}

void ControlledObject::setInverseTarget(const Vect3f& r)
{
	control_mode = INVERSE_STATIC_TARGET;
	target = r;
}
	
void ControlledObject::setInverseTarget(const Body* b)
{
	control_mode = b ? INVERSE_DYNAMIC_TARGET : NO_TARGET;
	dynamic_target = const_cast<Body*>(b);
}

void ControlledObject::control()
{
#ifndef _FINAL_VERSION_
	if(exclusive_keyboard_control && active()){
		keyboard_control();
		return;
		}
#endif

	switch(control_mode){
		case NO_TARGET:
			return;
		case DYNAMIC_TARGET:
		case INVERSE_DYNAMIC_TARGET:
			target = dynamic_target -> R();
			break;
		}

	Vect3f t;
	switch(control_mode){
		case STATIC_TARGET:
		case DYNAMIC_TARGET:
			t = getDist(target, R());
			break;
		case INVERSE_STATIC_TARGET:
		case INVERSE_DYNAMIC_TARGET:
			t = getDist(R(), target);
			break;
		default:
			assert(0);
		}

	t.z = 0;
	if(t.norm() < radius()){
		target_achieved();
		return;
		}

	Mat3f A_l2g_plane(-psi(), Z_AXIS);
	A_l2g_plane.invXform(t);
	if(traction < 0)
		t.negate();
	rudder = atan2(-t.x, t.y)/M_PI;
}

void ControlledObject::target_achieved()
{
	control_mode = NO_TARGET;
}	

int ControlledObject::obstacle()
{
	return 0;
}

void ControlledObject::write_state(XBuffer& buf)
{
	Body::write_state(buf);
	buf < "traction: " <= traction < "\n";
	buf < "rudder: " <= rudder < "\n";
	buf < "brake: " <= brake < "\n";
	buf < "target: " <= target;
	const char* modes[] = {
			"NO_TARGET",
			"STATIC_TARGET",
			"DYNAMIC_TARGET", 
			"INVERSE_STATIC_TARGET",	
			"INVERSE_DYNAMIC_TARGET", 
			"TARGET_ACHIEVED"
			};
	buf < "ControlMode: " < modes[control_mode] < "\n";
	buf < "-------------------------------------------\n";
}

void ControlledObject::keyboard_control()
{
	const float drudder = 0.1f;
	const float dtraction = 0.1f;

	const float traction_decr = 0.05f;
	const float rudder_decr = 0.05f;

	if(fabs(rudder) > rudder_decr)
		rudder -= rudder_decr*SIGN(rudder);
	else
		rudder = 0;
	if(fabs(traction) > traction_decr)
		traction -= traction_decr*SIGN(traction);
	else
		traction = 0;

	if(XKey.Pressed(VK_UP))
		if((traction += dtraction) > 1)
			traction = 1;

	if(XKey.Pressed(VK_DOWN))
		if((traction -= dtraction) < -1)
			traction = -1;

	if(XKey.Pressed(VK_LEFT))
		if((rudder -= drudder) < -1)
			rudder = -1;

	if(XKey.Pressed(VK_RIGHT))
		if((rudder += drudder) > 1)
			rudder = 1;

//	if(XKey.Pressed(VK_SHIFT))
//		brake = 1;
}

////////////////////////////////////////////////////////////////////////////////////////
//		AttackAndHomeObject
////////////////////////////////////////////////////////////////////////////////////////
AttackAndHomeObject::AttackAndHomeObject(float max_distance_, int do_attack_, int waiting_time_)
{
	do_attack = do_attack_;
	max_distance = max_distance_;
	turn_latency.start(waiting_time = waiting_time_);
}
void AttackAndHomeObject::setPositionInit()
{
	ControlledObject::setPositionInit();
	home = R();
}
void AttackAndHomeObject::control()
{
	if(!turn_latency || getDist((Vect3f)R(), home).norm() > max_distance){
		traction = 0.5;
		setTarget(home + Vect3f(frnd(max_distance), frnd(max_distance), 0));
		turn_latency.start(waiting_time);
		}
	//if(!turn_latency && do_attack){
	if(do_attack){
		Body* b = Mdisp -> searchBody(R(), MECHOS);
		if(b && b -> completed() && getDist((Vect3f)b -> R(), home).norm() < max_distance){
			traction = 1;
			setTarget(b);
			}
		}

	ControlledObject::control();
}
		
void ControlledObject::show() const
{
	Body::show();

	if(show_point_controlled_points){
		show_map_circle(R(), 5, CYAN);
		if(control_mode != NO_TARGET){
			show_map_circle(target, 4,control_mode == STATIC_TARGET ? BLUE : RED);
			show_map_line(R(), target, CYAN);
			show_map_line(R(), R() + Vglobal()*10, MAGENTA);
			}
		}
}

void AttackAndHomeObject::show() const
{
	ControlledObject::show();
	if(show_point_controlled_points)
		show_map_circle(home, 5, MAGENTA);
}