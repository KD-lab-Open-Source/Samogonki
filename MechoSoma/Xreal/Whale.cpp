#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Whale.h"
#include "SimpleClip.h"
#include "Params.h"
#include "AnimalsPrm.h"
#include "sound.h"

WhaleRacer::WhaleRacer()
: Body(myM3DType(17), CenteredBottomAlignmentedBound), 
FirePointBody(geometry)
{
	type_ = WHALE;
	traction = 1;
	set_archimedean_force(whale_buoyancy);
	spheres.front().center.z = -whale_sphere_offset_z;
	disable_traction_modulation = 1; 
	during_start = 0;
	I *= whale_TOI_scale;
	I_inv = Vect3f::ID/I;
}

void WhaleRacer::setPositionInit()
{
	PointControlledObject::setPositionInit();
	place_on_the_ground();
}

void WhaleRacer::calc_forces_and_drags()
{
	if(!during_start){
		control(); 
		traction = 1;
		}
	else{
		if(control_mode == NO_TARGET)
			setTarget(Mdisp -> searchBody(R(), MECHOS));
		ControlledObject::control();
		traction = 0.5;
		}

	dragV = whale_dragV;
	dragW = whale_dragW;

	applyLocalForce(traction*whale_traction, Y_AXIS);
	applyLocalTorque(rudder*whale_rudder*Vlocal().y, Z_AXIS);
	if(jump_timer()){
		Vect3f cross = Zglobal() % jump_axis;
		float len = cross.norm();
		if(len > FLT_EPS){
			cross *= Acos(dot(Zglobal(), jump_axis))*whale_jump_torque/len;
			applyGlobalTorque(cross);
			}
		}
	else
		applyLocalTurnTorque(Zlocal(), Vect3f::K);
}

void fxlabWhaleWaveSource(const Vect3f& v);

void WhaleRacer::post_quant()
{
	Body::post_quant();
	fire_with_rotate(pose());
	if(!wave_timer){
		fxlabWhaleWaveSource(pose().trans());
		wave_timer.start(250);
		}

	int jump = 0;
	for(int i = 0; i < whale_jump_points_size; i++){
		Vect3f v = getDist(whale_jump_points[i], R());
		v.z = 0;
		if(v.norm() < whale_jump_point_distance)
			jump = 1;
		}
	
	if(jump && !jump_timer){
		jump_timer.start(whale_jump_time);
		jump_reverse_timer.start(whale_jump_reverse_time);
		jump_axis = -Vglobal();
		jump_axis.z = 0;
		jump_axis.Normalize();
		jump_axis.z = whale_jump_axis_z;
		jump_axis.Normalize();
		}
	if(jump_reverse_timer()){
		jump_reverse_timer.stop();
		jump_axis.x = -jump_axis.x;
		jump_axis.y = -jump_axis.y;
		jump_axis.Normalize();
		}
	updateSound(EFF_WHALE);
}
