//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: ArcansPrm
//	Number of variables: 68
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
float mechos_foot_speed_factor = 0.12f;
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
FlyingArcaneData butterfly_flying_data = { 70.f 
 , 24.f, 40.f, 40.f, 3.e-002f, 0.1f, 4.f, 0.3f, 0.3f, 1.f, 2.f, 2.f, -0.2f, 2.f, 20.f 
  };
float butterfly_sweep_period = 300.f;
float butterfly_sound_phase = 0.25f;
FlyingArcaneData dirigible_flying_data = { 70.f 
 , 26.f, 40.f, 40.f, 3.e-002f, 0.1f, 4.f, 0.3f, 0.3f, 1.f, 2.f, 2.f, -0.2f, 2.f, 20.f 
  };
FlyingArcaneData helicopter_flying_data = { 70.f, 29.f, 40.f, 40.f, 3.e-002f 
 , 0.1f, 4.f, 0.3f, 0.3f, 1.f, 2.f, 2.f, -0.2f, 2.f, 20.f };
FlyingArcaneData dragon_power_flying_data = { 70.f 
 , 32.f, 40.f, 40.f, 3.e-002f, 0.1f, 4.f, 0.3f, 0.3f, 1.f, 2.f, 2.f, -0.2f, 2.f, 20.f 
  };
float dragon_power_sweep_period = 800.f;
float dragon_power_sound_phase = 0.f;
float cannon_ball_height = 70.f;
int cannon_ball_time = 3500;
float cannon_ball_Vxy = 85.f;
float cannon_ball_Vz = 30.f;
float cannon_ball_rudder = 0.5f;
float formicary_force = 2.f;
float jabasoma_jump_height = 70.f;
float jabasoma_jump_velocity_xy = 85.f;
float jabasoma_jump_velocity_xy_Forest5 = 75.f;
float jabasoma_jump_velocity_xy_Forest5_point_controlled = 69.f;
float jabasoma_jump_traction_force = 80.f;
float jabasoma_jump_velocity_z = 20.f;
float jabasoma_jump_force_factor = 2.5f;
float jabasoma_jump_stop_damping = 0.2f;
float jabasoma_dive_depth = 40.f;
float jabasoma_dive_velocity_xy = 65.f;
float jabasoma_dive_traction_force = 30.f;
float jabasoma_dive_velocity_z = 20.f;
float jabasoma_dive_force_factor = 4.f;
float jabasoma_dive_torque = -1.f;
float jabasoma_dive_stop_damping = 0.f;
float jabasoma_forest5_traction_force = 10.f;
float jabasoma_stop_damping = 1.f;
float jabasoma_rudder_manual = 0.6f;
float jabasoma_rudder_point_controlled = 1.f;
int jabasoma_animation_time = 800;
float jabasoma_stop_phase = 0.3f;
int jabasoma_sitting_time = 500;
int jabasoma_flying_time = 1000;
int mechos_acceleration_spot_time = 1500;
float mechos_acceleration_spot_traction = 20.f;
float mechos_acceleration_spot_max_speed = 90.f;
float jump_pad_distance_prefetch = 20.f;
float jump_pad_heigth = 120.f;
float jump_pad_force_xy = 3.5f;
float jump_pad_force_z = 3.5f;
float jump_pad_rudder = 5.f;
float jump_pad_finish_radius = 10.f;
float wild_thicket_drag = 8.f;
float red_track_damage = 3.f;
float mechos_red_track_time = 30000.f;
float mechos_red_track_animation_period = 1000.f;
float mechos_red_track_thickness_add = 1.f;
int slime_duration = 700;
float iron_hip_damage = 8.f;
float iron_hip_radius = 25.f;
float iron_hip_period = 2000.f;
float humble_scale = 0.4f;
float humble_mass_scale = 0.3f;
float humble_drag = 0.25f;
int humble_duration = 10000;
float magnetic_field_repulsion = 4.f;
float k_attraction_arcan = 4.f;
float magnetic_cushion_traction = 16.f;
float magnetic_cushion_rudder_modulation = 1.f;
float k_vortex_force = 5.e-002f;
float k_vortex_torque = 6.07753e-002f;
float k_vortex_attraction = 4.f;
float k_vortex_lift = 1.2f;

#ifdef _PRM_EDIT_
struct ArcansPrm_ParameterSection : ParameterSection
{
	ArcansPrm_ParameterSection() : ParameterSection("ArcansPrm")
{
	add_dependency("Scripts/Xreal/Arcans.prm");
	sourceCRC = 2811833398;
	description = 694915878;
	reserve(68);
	add(&mechos_foot_speed_factor, "mechos_foot_speed_factor");
	add(&butterfly_flying_data, "butterfly_flying_data");
	add(&butterfly_sweep_period, "butterfly_sweep_period");
	add(&butterfly_sound_phase, "butterfly_sound_phase");
	add(&dirigible_flying_data, "dirigible_flying_data");
	add(&helicopter_flying_data, "helicopter_flying_data");
	add(&dragon_power_flying_data, "dragon_power_flying_data");
	add(&dragon_power_sweep_period, "dragon_power_sweep_period");
	add(&dragon_power_sound_phase, "dragon_power_sound_phase");
	add(&cannon_ball_height, "cannon_ball_height");
	add(&cannon_ball_time, "cannon_ball_time");
	add(&cannon_ball_Vxy, "cannon_ball_Vxy");
	add(&cannon_ball_Vz, "cannon_ball_Vz");
	add(&cannon_ball_rudder, "cannon_ball_rudder");
	add(&formicary_force, "formicary_force");
	add(&jabasoma_jump_height, "jabasoma_jump_height");
	add(&jabasoma_jump_velocity_xy, "jabasoma_jump_velocity_xy");
	add(&jabasoma_jump_velocity_xy_Forest5, "jabasoma_jump_velocity_xy_Forest5");
	add(&jabasoma_jump_velocity_xy_Forest5_point_controlled, "jabasoma_jump_velocity_xy_Forest5_point_controlled");
	add(&jabasoma_jump_traction_force, "jabasoma_jump_traction_force");
	add(&jabasoma_jump_velocity_z, "jabasoma_jump_velocity_z");
	add(&jabasoma_jump_force_factor, "jabasoma_jump_force_factor");
	add(&jabasoma_jump_stop_damping, "jabasoma_jump_stop_damping");
	add(&jabasoma_dive_depth, "jabasoma_dive_depth");
	add(&jabasoma_dive_velocity_xy, "jabasoma_dive_velocity_xy");
	add(&jabasoma_dive_traction_force, "jabasoma_dive_traction_force");
	add(&jabasoma_dive_velocity_z, "jabasoma_dive_velocity_z");
	add(&jabasoma_dive_force_factor, "jabasoma_dive_force_factor");
	add(&jabasoma_dive_torque, "jabasoma_dive_torque");
	add(&jabasoma_dive_stop_damping, "jabasoma_dive_stop_damping");
	add(&jabasoma_forest5_traction_force, "jabasoma_forest5_traction_force");
	add(&jabasoma_stop_damping, "jabasoma_stop_damping");
	add(&jabasoma_rudder_manual, "jabasoma_rudder_manual");
	add(&jabasoma_rudder_point_controlled, "jabasoma_rudder_point_controlled");
	add(&jabasoma_animation_time, "jabasoma_animation_time");
	add(&jabasoma_stop_phase, "jabasoma_stop_phase");
	add(&jabasoma_sitting_time, "jabasoma_sitting_time");
	add(&jabasoma_flying_time, "jabasoma_flying_time");
	add(&mechos_acceleration_spot_time, "mechos_acceleration_spot_time");
	add(&mechos_acceleration_spot_traction, "mechos_acceleration_spot_traction");
	add(&mechos_acceleration_spot_max_speed, "mechos_acceleration_spot_max_speed");
	add(&jump_pad_distance_prefetch, "jump_pad_distance_prefetch");
	add(&jump_pad_heigth, "jump_pad_heigth");
	add(&jump_pad_force_xy, "jump_pad_force_xy");
	add(&jump_pad_force_z, "jump_pad_force_z");
	add(&jump_pad_rudder, "jump_pad_rudder");
	add(&jump_pad_finish_radius, "jump_pad_finish_radius");
	add(&wild_thicket_drag, "wild_thicket_drag");
	add(&red_track_damage, "red_track_damage");
	add(&mechos_red_track_time, "mechos_red_track_time");
	add(&mechos_red_track_animation_period, "mechos_red_track_animation_period");
	add(&mechos_red_track_thickness_add, "mechos_red_track_thickness_add");
	add(&slime_duration, "slime_duration");
	add(&iron_hip_damage, "iron_hip_damage");
	add(&iron_hip_radius, "iron_hip_radius");
	add(&iron_hip_period, "iron_hip_period");
	add(&humble_scale, "humble_scale");
	add(&humble_mass_scale, "humble_mass_scale");
	add(&humble_drag, "humble_drag");
	add(&humble_duration, "humble_duration");
	add(&magnetic_field_repulsion, "magnetic_field_repulsion");
	add(&k_attraction_arcan, "k_attraction_arcan");
	add(&magnetic_cushion_traction, "magnetic_cushion_traction");
	add(&magnetic_cushion_rudder_modulation, "magnetic_cushion_rudder_modulation");
	add(&k_vortex_force, "k_vortex_force");
	add(&k_vortex_torque, "k_vortex_torque");
	add(&k_vortex_attraction, "k_vortex_attraction");
	add(&k_vortex_lift, "k_vortex_lift");
	add_parameter_section(this);
}
};
static ArcansPrm_ParameterSection ArcansPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: ArcansPrm
//////////////////////////////////////////////////////////////////////////////////////////////
