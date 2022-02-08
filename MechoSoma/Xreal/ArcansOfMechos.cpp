#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "FieldSource.h"
#include "Params.h"
#include "Mesh3ds.h"
#include "IVisGeneric.h"
#include "sound.h"
#include "MechosPrm.h"
#include "ArcansPrm.h"
#include "TERRA.H"

const int morph_time = 500;

/////////////////////////////////////////////////////////////////////////////////////////
//		Wheels tool
/////////////////////////////////////////////////////////////////////////////////////////
Mechos::WheelMode Mechos::forward_wheels_mode()
{
	static WheelMode forward_wheels_modes[] = {
		WHEEL, //		
		WHEEL, // 1
		WHEEL, // 2
		FOOT, // 3 - мотылек
		WHEEL, // 4
		WHEEL, // 5
		FOOT, // 6 - жабасома
		WHEEL, // 7 - вертолет
		FOOT, // 8 - сила дракона
		WHEEL // 9
	};
	return forward_wheels_modes[forward_wheels_kind()];
}
Mechos::WheelMode Mechos::backward_wheels_mode()
{
	static WheelMode backward_wheels_modes[] = {
		WHEEL, //		
		WHEEL, // 1
		WHEEL, // 2
		WHEEL, // 3
		WHEEL, // 4
		WHEEL, // 5
		FOOT, // 6 - ярость слизи
		WHEEL, // 7
		WHEEL, // 8 - бешеная плесень
		WHEEL, // 9
		WHEEL, // 10
		WHEEL, // 11
		WHEEL, // 12
		WHEEL, // 13
		WHEEL // 14
	};
	return backward_wheels_modes[backward_wheels_kind()];
}

int Mechos::forward_wheels_kind()
{
	return M3D_KIND(part_by_type(M3D_RF_WHEEL) -> Type);
}
int Mechos::backward_wheels_kind()
{
	return M3D_KIND(part_by_type(M3D_RB_WHEEL) -> Type);
}
int Mechos::change_forward_wheels_kind(int new_type)
{
	int old_type = part_by_type(M3D_RF_WHEEL) -> Type;
	xrealM3D -> RebuildObject(geometry, old_type, M3D_TOTAL_TYPE(new_type, M3D_RF_WHEEL) );
	xrealM3D -> RebuildObject(geometry, (old_type & ~M3D_RF_WHEEL) | M3D_LF_WHEEL, M3D_TOTAL_TYPE(new_type, M3D_LF_WHEEL));
	geometry -> ReCalcTotalBound();
	part_offsets[RFWheel] = part_by_index(RFWheel) -> LocalMatrix;
	part_offsets[LFWheel] = part_by_index(LFWheel) -> LocalMatrix;
	return M3D_KIND(old_type);
}

int Mechos::change_backward_wheels_kind(int new_type)
{
	int old_type = part_by_type(M3D_RB_WHEEL) -> Type;
	xrealM3D -> RebuildObject(geometry, old_type, M3D_TOTAL_TYPE(new_type, M3D_RB_WHEEL) );
	xrealM3D -> RebuildObject(geometry, (old_type & ~M3D_RB_WHEEL) | M3D_LB_WHEEL, M3D_TOTAL_TYPE(new_type, M3D_LB_WHEEL));
	geometry -> ReCalcTotalBound();
	part_offsets[RBWheel] = part_by_index(RBWheel) -> LocalMatrix;
	part_offsets[LBWheel] = part_by_index(LBWheel) -> LocalMatrix;
	return M3D_KIND(old_type);
}

void Mechos::set_forward_wheels_animation(float animation_period, float start_phase, float finish_phase)
{
	part_by_type(M3D_LF_WHEEL) -> SetAnimation(animation_period, start_phase,  finish_phase);
	part_by_type(M3D_RF_WHEEL) -> SetAnimation(animation_period, start_phase,  finish_phase);
}
void Mechos::set_backward_wheels_animation(float animation_period, float start_phase, float finish_phase)
{
	part_by_type(M3D_LB_WHEEL) -> SetAnimation(animation_period, start_phase,  finish_phase);
	part_by_type(M3D_RB_WHEEL) -> SetAnimation(animation_period, start_phase,  finish_phase);
}


///////////////////////////////////////////////////////////////////////////////
//		Wheels quants
///////////////////////////////////////////////////////////////////////////////
void Mechos::forward_rotated_quant()
{
	//if(!isFlyingByArcane()){
	//	Vect3f dr = getDist(R(), R_prev);
	//	Alg().invXform(dr);
	//	forward_wheels_phase += dr.y/part_by_type(M3D_RF_WHEEL) -> zmaxReal();
	//	}
	//Mat3f M(-forward_wheels_phase, X_AXIS);
	part_offsets[LFWheel].rot() = Mat3f(left_rudder_angle, Z_AXIS)*Mat3f(wheels[WheelLF].phi, X_AXIS);
	part_offsets[RFWheel].rot() = Mat3f(right_rudder_angle, Z_AXIS)*Mat3f(wheels[WheelRF].phi, X_AXIS);
}
void Mechos::forward_non_rotated_quant()
{
	forward_wheels_phase = 0;
	part_offsets[LFWheel].rot() = Mat3f::ID;
	part_offsets[RFWheel].rot() = Mat3f::ID;
}
void Mechos::forward_foot_quant()
{
	if(!isFlyingByArcane()){
		Vect3f dr = getDist(R(), R_prev);
		Alg().invXform(dr);
		forward_wheels_phase += mechos_foot_speed_factor*dr.y/part_by_type(M3D_RF_WHEEL) -> rmaxReal();
		forward_wheels_phase = fmod(forward_wheels_phase + 100, 1);
		}
	else
		forward_wheels_phase = 0;
	set_forward_wheels_animation(0, forward_wheels_phase);
}	
void Mechos::backward_rotated_quant()
{
//	if(!isFlyingByArcane()){
//		Vect3f dr = getDist(R(), R_prev);
//		Alg().invXform(dr);
//		backward_wheels_phase += dr.y/part_by_type(M3D_RB_WHEEL) -> zmaxReal();
//		}
//	Mat3f M(-backward_wheels_phase, X_AXIS);
	part_offsets[LBWheel].rot() = Mat3f(wheels[WheelLB].phi, X_AXIS);
	part_offsets[RBWheel].rot() = Mat3f(wheels[WheelRB].phi, X_AXIS);
}
void Mechos::backward_foot_quant()
{
	if(!isFlyingByArcane()){
		Vect3f dr = getDist(R(), R_prev);
		Alg().invXform(dr);
		backward_wheels_phase += mechos_foot_speed_factor*dr.y/part_by_type(M3D_RB_WHEEL) -> rmaxReal();
		backward_wheels_phase = fmod(backward_wheels_phase + 100, 1);
		}
	else
		backward_wheels_phase = 0;
	set_backward_wheels_animation(0, backward_wheels_phase);
}	


///////////////////////////////////////////////////////////////////////////////
//		Iron Hip
///////////////////////////////////////////////////////////////////////////////
int Mechos::iron_hip_quant()
{
	post_quant();
	cMesh* front = part_by_type(M3D_FRONT);
	front -> SetFirstAnimation(0, iron_hip_timer()/iron_hip_period);
	vector<Vect3f> fires;
	front -> GetGlobalDummy("fireb", fires);
	vector<Vect3f>::iterator fi;
	FOR_EACH(fires, fi){
		Vect3f v = *fi;
		Body* victim = Mdisp -> searchBody(v, MECHOS, this);
		if(victim && getDist(v, victim -> R()).norm() < iron_hip_radius)
			victim -> damage(iron_hip_damage);
		}
	if(front -> FindFrame() -> IsPhasePassed(0))
		startSound(EFF_IRON_SIDE);
	else
		updateSound(EFF_IRON_SIDE);

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	Cannon Ball 
///////////////////////////////////////////////////////////////////////////////
void Mechos::cannon_ball_flight()
{
	if(cannon_ball_timer())
		return;
	cannon_ball_timer.start(cannon_ball_time);
	forces.push_back((QuantFunction)&Mechos::cannon_ball_quant);
}

int Mechos::cannon_ball_quant()
{
	if(cannon_ball_timer()){
		Vect3f dr = Yglobal();
		dr.z = 0;
		dr.normalize(cannon_ball_Vxy);
		setGlobalVelocity(dr);
		if(relativeHeight() < cannon_ball_height)
			setGlobalVelocity(cannon_ball_Vz, Z_AXIS);
		else
			setGlobalVelocity(0, Z_AXIS);
		//startSound(EFF_BULLET_FLIGHT_ADD);
		}

	setLocalAngularVelocity(cannon_ball_rudder*rudder, Z_AXIS);
	applyLocalTurnTorque(Zlocal(), Vect3f::K);

	dragV += cannon_ball_dragV;
	dragW += cannon_ball_dragW;

	if(!cannon_ball_timer() && (colliding() || submersion() > 0.1)){
		//stopSound(EFF_BULLET_FLIGHT_ADD);
		return 0;
		}
	else
		return 1;
}


///////////////////////////////////////////////////////////////////////////////
//	Jump Pad
///////////////////////////////////////////////////////////////////////////////
void Mechos::jump_pad_start(const Vect3f& target)
{
	if(features() & FLYING_BY_JUMP_PAD)
		return;
	features_ |= FLYING_BY_JUMP_PAD;
	forces.push_back((QuantFunction)&Mechos::jump_pad_quant);

	jump_pad_target = target;
	jump_pad_target.z = tri_map -> H(jump_pad_target.x, jump_pad_target.y) + 20;
	Vect3f n = getDist(jump_pad_target, R());
	n.z = 0;
	jump_pad_dist0 = n.norm();
	float h0 = R().z;
	float ht = jump_pad_target.z;
	float hmax = max(h0, ht) + jump_pad_heigth;

	float t4 = hmax*hmax;
	float t6 = sqrt(ht*h0-h0*hmax-ht*hmax+t4);
	float t8 = jump_pad_dist0*jump_pad_dist0;
	float a1 = (h0+ht-2.0*hmax-2.0*t6)/t8;
	float b1 = (-2.0*ht+2.0*hmax+2.0*t6)/jump_pad_dist0;
	float a2 = (h0+ht-2.0*hmax+2.0*t6)/t8;
	float b2 = (-2.0*ht+2.0*hmax-2.0*t6)/jump_pad_dist0;
	if(a1 < a2){
		jump_pad_a = a1;
		jump_pad_b = b1;
		}
	else{
		jump_pad_a = a2;
		jump_pad_b = b2;
		}
	jump_pad_c = ht;
	if(active())
		startSound(EFF_BONUS_JUMP);
}

int Mechos::jump_pad_quant()
{
	if(active())
		updateSound(EFF_BONUS_JUMP);

	Vect3f n = getDist(jump_pad_target, R());
	n.z = 0;
	float dist = n.norm();
	features_ &= ~FLYING_BY_JUMP_PAD; // ты сам аркан
	if(dist < jump_pad_finish_radius || (isFlyingByArcane() && !(features() & JABASOMA)) || features() & STATIC_NULIFICATOR){
		setGlobalVelocity(0, X_AXIS);
		setGlobalVelocity(0, Y_AXIS);
		return 0;
		}
	features_ |= FLYING_BY_JUMP_PAD; 
	n /= dist;
	applyGlobalForce(n*(jump_pad_force_xy*Gravity));
	if((dist -= jump_pad_distance_prefetch) < 0)
		dist = 0;
	float z = (jump_pad_a*dist + jump_pad_b)*dist + jump_pad_c;
	if(R().z < z + 10)
		applyGlobalForce((jump_pad_force_z + 1)*Gravity, Z_AXIS);
	else
		applyGlobalForce(-jump_pad_force_z*Gravity, Z_AXIS);

	//applyGlobalTurnTorque(Vglobal(), Yglobal(), jump_pad_rudder);
	applyGlobalTorque(rudder*jump_pad_rudder, Z_AXIS);
	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	dragV += jump_pad_drag;
	dragW += Vect3f(5);
									
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	Jump
///////////////////////////////////////////////////////////////////////////////
void Mechos::jump()
{
	if(jump_timer())
		return;
	jump_timer.start(400);
	setLocalVelocity(jump_velocity_y, Y_AXIS); // скорость вперед, игнорируем предыдущую скорость
	setGlobalVelocity(jump_velocity_z, Z_AXIS); // скорость вверх
	forces.push_back((QuantFunction)&Mechos::jump_quant);
}
int Mechos::jump_quant()
{
	setLocalVelocity(jump_velocity_y, Y_AXIS); // скорость вперед, игнорируем предыдущую скорость
	applyLocalTurnTorque(Zlocal(), Vect3f::K);
	dragW += Vect3f(3, 3, 3);
	return jump_timer();
}

///////////////////////////////////////////////////////////////////////////////
//		Flying force general
///////////////////////////////////////////////////////////////////////////////
void Mechos::apply_flying_force(FlyingArcaneData& fly)
{
	Vect3f y_axis = Yglobal();
	y_axis.z = 0;
	applyGlobalForce(fly.traction_force*traction*y_axis);
	
	applyLocalTorque(rudder*(Vlocal().y + (traction >= 0 ? fly.zero_speed_rudder : -fly.zero_speed_rudder))
		*(control_points.empty() ? fly.rudder_torque_factor_manual : fly.rudder_torque_factor_point_controlled), Z_AXIS);
	if(brake)
		applyGlobalForce(SIGN(fly.brake_force, -Vlocal().y)*y_axis);

	float lift = relativeHeight() < fly.height ? fly.up_lift : fly.down_lift;
	applyGlobalForce(Gravity*lift, Z_AXIS, Vect3f(0, fly.lift_offset_y, fly.lift_offset_z));
	dragV += Vect3f(fly.dragVx, fly.dragVy, fly.dragVz);
	dragW += Vect3f(fly.dragWxy, fly.dragWxy, fly.dragWz);
}

///////////////////////////////////////////////////////////////////////////////
//		Dirigible
///////////////////////////////////////////////////////////////////////////////
int Mechos::dirigible_quant()
{
	apply_flying_force(dirigible_flying_data);
	startSound(EFF_DIRIGIBLE_ADD);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//		Helicopter
///////////////////////////////////////////////////////////////////////////////
void Mechos::helicopter_start()
{
	change_forward_wheels_kind(HELICOPTER_KIND | MORPH_KIND);
	set_forward_wheels_animation(morph_time, 0, 1.f);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::helicopter_morph_quant;
	part_offsets[LFWheel].rot() = Mat3f::ID;
	part_offsets[RFWheel].rot() = Mat3f::ID;
	startSound(EFF_HELI);
}

void Mechos::helicopter_morph_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(HELICOPTER_KIND | ACTIVATED_KIND);
		morph_finished.stop();
		forces.push_back((QuantFunction)&Mechos::helicopter_force);
		forward_wheels_quant = &Mechos::helicopter_quant;
		set_forward_wheels_animation(1000);
		}
}

void Mechos::helicopter_quant()
{
	startSound(EFF_HELI_ADD);
}

void Mechos::helicopter_stop()
{
	change_forward_wheels_kind(HELICOPTER_KIND | MORPH_KIND);
	set_forward_wheels_animation(-morph_time, 1.f, 0);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::helicopter_morph_inv_quant;
	forces.remove((QuantFunction)&Mechos::helicopter_force);
	stopSound(EFF_HELI_ADD);
	startSound(EFF_HELI);
}

void Mechos::helicopter_morph_inv_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(HELICOPTER_KIND);
		morph_finished.stop();
		forward_wheels_quant = &Mechos::forward_rotated_quant;
		startEngineSound();
		}
}

int Mechos::helicopter_force()
{
	apply_flying_force(helicopter_flying_data);
	return 1;
}


///////////////////////////////////////////////////////////////////////////////
//		Butterfly
///////////////////////////////////////////////////////////////////////////////
void Mechos::butterfly_start()
{
	change_forward_wheels_kind(BUTTERFLY_KIND | MORPH_KIND);
	set_forward_wheels_animation(morph_time, 0, 1.f);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::butterfly_morph_quant;
}

void Mechos::butterfly_morph_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(BUTTERFLY_KIND | ACTIVATED_KIND);
		morph_finished.stop();
		forces.push_back((QuantFunction)&Mechos::butterfly_force);
		forward_wheels_quant = &Mechos::butterfly_wing_quant;
		set_forward_wheels_animation(butterfly_sweep_period);
		stopEngineSound();
		}
}

void Mechos::butterfly_wing_quant()
{
	if(part_by_type(M3D_RF_WHEEL) -> FindFrame() -> IsPhasePassed(0))
		startSound(EFF_WING_BUTTERFLY_ADD);
}

void Mechos::butterfly_stop()
{
	change_forward_wheels_kind(BUTTERFLY_KIND | MORPH_KIND);
	set_forward_wheels_animation(-morph_time, 1.f, 0);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::butterfly_morph_inv_quant;
	forces.remove((QuantFunction)&Mechos::butterfly_force);
}

void Mechos::butterfly_morph_inv_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(BUTTERFLY_KIND);
		morph_finished.stop();
		forward_wheels_quant = &Mechos::forward_foot_quant;
		startEngineSound();
		}
}

int Mechos::butterfly_force()
{
	apply_flying_force(butterfly_flying_data);
	return 1;
}



///////////////////////////////////////////////////////////////////////////////
//		Jabasoma
///////////////////////////////////////////////////////////////////////////////
void Mechos::jabasoma_start()
{
	change_forward_wheels_kind(JABASOMA_KIND | MORPH_KIND);
	set_forward_wheels_animation(morph_time, 0, 1.f);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::jabasoma_morph_quant;
}

void Mechos::jabasoma_morph_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(JABASOMA_KIND | ACTIVATED_KIND);
		morph_finished.stop();
		forces.push_back((QuantFunction)&Mechos::jabasoma_force);
		forward_wheels_quant = &Mechos::jabasoma_jumping_foot_quant;
		set_forward_wheels_animation(0,0);
		stopEngineSound();
		enabled_fields &= ~FieldSource::Stream;
		}
}

void Mechos::jabasoma_jumping_foot_quant()
{
}

void Mechos::jabasoma_stop()
{
	change_forward_wheels_kind(JABASOMA_KIND | MORPH_KIND);
	set_forward_wheels_animation(-morph_time, 1.f, 0);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::jabasoma_morph_inv_quant;
	forces.remove((QuantFunction)&Mechos::jabasoma_force);
}

void Mechos::jabasoma_morph_inv_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(JABASOMA_KIND);
		morph_finished.stop();
		forward_wheels_quant = &Mechos::forward_foot_quant;
		startEngineSound();
		enabled_fields |= FieldSource::Stream;
		}
}

int Mechos::jabasoma_force()
{
	if(features() & (FLYING_BY_JUMP_PAD | ACCELERATED_BY_SPOT))
		return 1;

	if(jabasoma_jumping){ 
		Vect3f dr = Yglobal();
		dr.z = 0;
		if(jabasoma_jumping == 1){
			if(Mdisp -> waterDamage() != Forest5_water_damage){
				if(traction && fabs(velocity_y_avr) < jabasoma_jump_velocity_xy)
					applyGlobalForce(dr.Normalize(traction*jabasoma_jump_traction_force));
				else
					dragV.y += jabasoma_jump_stop_damping;
				if(relativeHeight() < jabasoma_jump_height)
					applyGlobalForce(jabasoma_jump_force_factor*Gravity, Z_AXIS);
				}
			else{
				setGlobalVelocity(dr.Normalize(jabasoma_jumping_log*traction*(control_points.empty() ? jabasoma_jump_velocity_xy_Forest5 : jabasoma_jump_velocity_xy_Forest5_point_controlled)));
				if(relativeHeight() < jabasoma_jump_height)
					setGlobalVelocity(jabasoma_jump_velocity_z, Z_AXIS);
				}
			}
		else{
			if(Mdisp -> waterDamage() != Forest5_water_damage){
				if(traction){
					applyGlobalForce(dr.Normalize(traction*jabasoma_dive_traction_force));
					applyLocalTorque(jabasoma_dive_torque, X_AXIS);
					}
				else
					dragV.y += jabasoma_dive_stop_damping;
				if(!colliding() && GetWH(R().xi(), R().yi()) - R().z < jabasoma_dive_depth)
					applyGlobalForce(-jabasoma_dive_force_factor*Gravity, Z_AXIS);
				}
			else{
				if(traction)
					applyGlobalForce(dr.Normalize(traction*jabasoma_forest5_traction_force));
				//damage(Forest5_water_damage*(float)xreal_evolve_period/(float)xreal_quant_period);
				}
			}
		if(!jabasoma_timer){
			set_forward_wheels_animation(jabasoma_animation_time, jabasoma_stop_phase, 0);
			jabasoma_jumping = 0;
			jabasoma_timer.start(jabasoma_sitting_time);
			}
		}	
	else{
		dragV.y += jabasoma_stop_damping;
		if(traction && !jabasoma_timer){
			Vect3f dr = Yglobal();
			dr.z = 0;
			if(colliding()){
				set_forward_wheels_animation(jabasoma_animation_time, 0, jabasoma_stop_phase);
				startSound(EFF_FROG_ADD);
				jabasoma_timer.start(jabasoma_flying_time);
				jabasoma_jumping = 1;
				jabasoma_jumping_log = 1;
				if(traction)
					setGlobalVelocity(dr.Normalize(traction*(Mdisp -> waterDamage() != Forest5_water_damage ? jabasoma_jump_velocity_xy : jabasoma_jump_velocity_xy_Forest5)));
				setGlobalVelocity(jabasoma_jump_velocity_z, Z_AXIS);
				}
			else
				if(submersion() > 0.5){
					set_forward_wheels_animation(jabasoma_animation_time, 0, jabasoma_stop_phase);
					startSound(EFF_FROG_ADD);
					jabasoma_timer.start(jabasoma_flying_time);
					jabasoma_jumping = 2;
					if(Mdisp -> waterDamage() != Forest5_water_damage){
						if(traction)
							setGlobalVelocity(dr.Normalize(traction*jabasoma_dive_velocity_xy));
						setGlobalVelocity(-jabasoma_dive_velocity_z, Z_AXIS);
						}
					}
			}
		}
	
	if(jabasoma_jumping || !colliding())
		setLocalAngularVelocity((num_control_points() ? jabasoma_rudder_point_controlled : jabasoma_rudder_manual)*rudder, Z_AXIS);
	applyLocalTurnTorque(Zlocal(), Vect3f::K);

	dragV += jabasoma_dragV;
	dragW += jabasoma_dragW;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//		Dragon Power
///////////////////////////////////////////////////////////////////////////////
void Mechos::dragon_power_start()
{
	change_forward_wheels_kind(DRAGON_POWER_KIND | MORPH_KIND);
	set_forward_wheels_animation(morph_time, 0, 1.f);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::dragon_power_morph_quant;
}

void Mechos::dragon_power_morph_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(DRAGON_POWER_KIND | ACTIVATED_KIND);
		morph_finished.stop();
		forces.push_back((QuantFunction)&Mechos::dragon_power_force);
		forward_wheels_quant = &Mechos::dragon_power_wing_quant;
		set_forward_wheels_animation(dragon_power_sweep_period);
		stopEngineSound();
		}
}

void Mechos::dragon_power_wing_quant()
{
	if(part_by_type(M3D_RF_WHEEL) -> FindFrame() -> IsPhasePassed(dragon_power_sound_phase))
		startSound(EFF_DRAGON_POWER);
}

void Mechos::dragon_power_stop()
{
	change_forward_wheels_kind(DRAGON_POWER_KIND | MORPH_KIND);
	set_forward_wheels_animation(-morph_time, 1.f, 0);
	morph_finished.start(morph_time);
	forward_wheels_quant = &Mechos::dragon_power_morph_inv_quant;
	forces.remove((QuantFunction)&Mechos::dragon_power_force);
}

void Mechos::dragon_power_morph_inv_quant()
{
	if(morph_finished()){
		change_forward_wheels_kind(DRAGON_POWER_KIND);
		morph_finished.stop();
		forward_wheels_quant = &Mechos::forward_foot_quant;
		startEngineSound();
		}
}

int Mechos::dragon_power_force()
{
	apply_flying_force(dragon_power_flying_data);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//		  Magnetic Cushion
///////////////////////////////////////////////////////////////////////////////
int Mechos::magnetic_cushion_force()
{
	applyLocalForce(magnetic_cushion_traction*traction, Y_AXIS);
//	applyLocalTorque(rudder*Vlocal().y*rudder_modulation(magnetic_cushion_rudder_modulation), Z_AXIS);

	float lift = colliding() ? 1.2 : 0;
	applyGlobalForce(Gravity*lift, Z_AXIS);
	applyLocalTurnTorque(Zlocal(), Vect3f::K);

	dragV += magnetic_cushion_dragV;
	dragW += magnetic_cushion_dragW;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	Fury of Slime
///////////////////////////////////////////////////////////////////////////////
void Mechos::fury_of_slime_foot_quant()
{
	Vect3f dr = getDist(R(), R_prev);
	Alg().invXform(dr);
	backward_wheels_phase += 0.05*dr.y/part_by_type(M3D_RB_WHEEL) -> zmaxReal();
	backward_wheels_phase = fmod(backward_wheels_phase + 100, 1);
	set_backward_wheels_animation(0, backward_wheels_phase);
}	

