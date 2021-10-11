//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript declaration
//	Section: ArcansPrm
//	Number of variables: 68
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
extern float mechos_foot_speed_factor;
#ifndef __XScript_struct_FlyingArcaneData__
#define __XScript_struct_FlyingArcaneData__
#pragma pack( push, __XScript_struct_pack__, 4 )
struct FlyingArcaneData {
	float height;
	float traction_force;
	float brake_force;
	float zero_speed_rudder;
	float rudder_torque_factor_manual;
	float rudder_torque_factor_point_controlled;
	float dragVx;
	float dragVy;
	float dragVz;
	float dragWxy;
	float dragWz;
	float up_lift;
	float down_lift;
	float lift_offset_y;
	float lift_offset_z;
};
#pragma pack( pop, __XScript_struct_pack__)
#endif  // __XScript_struct_FlyingArcaneData__
extern FlyingArcaneData butterfly_flying_data;
extern float butterfly_sweep_period;
extern float butterfly_sound_phase;
extern FlyingArcaneData dirigible_flying_data;
extern FlyingArcaneData helicopter_flying_data;
extern FlyingArcaneData dragon_power_flying_data;
extern float dragon_power_sweep_period;
extern float dragon_power_sound_phase;
extern float cannon_ball_height;
extern int cannon_ball_time;
extern float cannon_ball_Vxy;
extern float cannon_ball_Vz;
extern float cannon_ball_rudder;
extern float formicary_force;
extern float jabasoma_jump_height;
extern float jabasoma_jump_velocity_xy;
extern float jabasoma_jump_velocity_xy_Forest5;
extern float jabasoma_jump_velocity_xy_Forest5_point_controlled;
extern float jabasoma_jump_traction_force;
extern float jabasoma_jump_velocity_z;
extern float jabasoma_jump_force_factor;
extern float jabasoma_jump_stop_damping;
extern float jabasoma_dive_depth;
extern float jabasoma_dive_velocity_xy;
extern float jabasoma_dive_traction_force;
extern float jabasoma_dive_velocity_z;
extern float jabasoma_dive_force_factor;
extern float jabasoma_dive_torque;
extern float jabasoma_dive_stop_damping;
extern float jabasoma_forest5_traction_force;
extern float jabasoma_stop_damping;
extern float jabasoma_rudder_manual;
extern float jabasoma_rudder_point_controlled;
extern int jabasoma_animation_time;
extern float jabasoma_stop_phase;
extern int jabasoma_sitting_time;
extern int jabasoma_flying_time;
extern int mechos_acceleration_spot_time;
extern float mechos_acceleration_spot_traction;
extern float mechos_acceleration_spot_max_speed;
extern float jump_pad_distance_prefetch;
extern float jump_pad_heigth;
extern float jump_pad_force_xy;
extern float jump_pad_force_z;
extern float jump_pad_rudder;
extern float jump_pad_finish_radius;
extern float wild_thicket_drag;
extern float red_track_damage;
extern float mechos_red_track_time;
extern float mechos_red_track_animation_period;
extern float mechos_red_track_thickness_add;
extern int slime_duration;
extern float iron_hip_damage;
extern float iron_hip_radius;
extern float iron_hip_period;
extern float humble_scale;
extern float humble_mass_scale;
extern float humble_drag;
extern int humble_duration;
extern float magnetic_field_repulsion;
extern float k_attraction_arcan;
extern float magnetic_cushion_traction;
extern float magnetic_cushion_rudder_modulation;
extern float k_vortex_force;
extern float k_vortex_torque;
extern float k_vortex_attraction;
extern float k_vortex_lift;
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: ArcansPrm
//////////////////////////////////////////////////////////////////////////////////////////////
