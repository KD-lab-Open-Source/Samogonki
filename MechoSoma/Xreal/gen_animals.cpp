//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: AnimalsPrm
//	Number of variables: 102
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
float frog_damage_ability = 0.5f;
float frog_xy_impulse = 40.f;
float frog_z_impulse = 15.f;
float frog_rudder = 0.1f;
float frog_contact_damping = 8.f;
float frog_fly_damping_W = 2.f;
float frog_start_jump_phase = 0.f;
float frog_stop_jump_phase = 0.5f;
float frog_jump_animation_period = 1000.f;
float frog_stop_animation_period = 1000.f;
float frog_phase_incr = 0.05f;
int frog_rest_time = 10000;
int frog_walk_distance = 10000;
float frog_Vy = 76.f;
float frog_az = 10.f;
float frog_radius = 60.f;
int frog_racer_recover_time = 2000;
float dragon_traction = 25.f;
float dragon_racer_traction = 52.f;
float dragon_rudder = 0.07f;
float dragon_width = 100.f;
float dragon_length = 150.f;
float dragon_height = 150.f;
float dragon_lift_bias = 0.5f;
float spider_max_distance = 200.f;
float spider_traction = 20.f;
float spider_boss_traction = 81.f;
float spider_rudder = 0.5f;
float spider_boss_rudder = 4.f;
float spider_antigravity_factor = 1.f;
float spider_stick_factor = 4.f;
float spider_levelling_factor = 3.f;
int spider_not_colliding_avr_time = 50;
float spider_catch_width = 60.f;
float spider_catch_length = 50.f;
float spider_foot_speed_factor = 1.f;
float stone_torque = 0.3f;
float stone_TOI_scale = 0.5f;
float stone_initial_velocity_xy = 30.f;
float stone_initial_velocity_z = 60.f;
int stone_disable_collision_time = 500;
float stone_restitution_plus_one = 1.5f;
float stone_sound_velocity_threshould = 3.f;
float debris_Vxy = 40.4055f;
float debris_Vz = 15.5034f;
float debris_W = 1.f;
float debris_damage_ability = 0.5f;
float debris_time_to_dwindle = 2000.f;
float smart_stone_damage_ability = 0.05f;
float smart_stone_traction = 2.f;
float smart_stone_rudder = 0.1f;
float smart_stone_restitution = 0.95f;
float smart_stone_catch_point_distance = 100.f;
int smart_stone_assembling_time = 800;
float smart_stone_antigravity_factor = 1.f;
float smart_stone_fly_velocity = 60.f;
float shark_spheres_radius = 10.f;
float shark_distance = 500.f;
float shark_damage = 1.f;
float shark_buoyancy = 0.07f;
float shark_traction = 4.f;
float shark_rudder = 0.04f;
float shark_animation_period = 1500.f;
float shark_rudder_duration = 3000.f;
float fish_spheres_radius = 10.f;
float fish_distance = 500.f;
float fish_damage = 0.3f;
float fish_buoyancy = 0.02f;
float fish_traction = 2.f;
float whale_TOI_scale = 3.f;
float whale_buoyancy = 0.015f;
float whale_sphere_offset_z = 20.f;
float whale_traction = 49.f;
float whale_rudder = 0.05f;
int whale_jump_time = 2000;
int whale_jump_reverse_time = 1000;
float whale_jump_axis_z = 0.8f;
float whale_jump_torque = 0.4f;
float whale_jump_point_distance = 120.f;
float dog_sensor_radius = 0.7f;
float dog_hit_velocity2sound_factor = 1.f;
float dog_activation_distance = 150.f;
float dog_animation_period = 8000.f;
float wicked_butterfly_traction = 100.f;
float wicked_butterfly_distance_to_traction_factor = 0.025f;
float brevno_spheres_radius = 25.f;
float brevno_mass = 1.f;
float brevno_return_force = 2.f;
float brevno_linear_damping = 0.2f;
float brevno_angular_damping = 0.2f;
float brevno_buoyancy = 0.05f;
float brevno_torque = 0.05f;
float raft_spheres_radius = 10.f;
float raft_mass = 1.25f;
float raft_buoyancy = 0.044f;
float ship_spheres_radius = 8.f;
float ship_buoyancy = 0.1f;
float ship_guard_distance = 500.f;
float ship_turn_delay = 5000.f;
float ship_damage = 50.f;
float ship_traction = 2.f;
float ship_rudder = 1.f;

#ifdef _PRM_EDIT_
struct AnimalsPrm_ParameterSection : ParameterSection
{
	AnimalsPrm_ParameterSection() : ParameterSection("AnimalsPrm")
{
	add_dependency("Scripts/Xreal/Animals.prm");
	sourceCRC = 3013138640;
	description = 2183502996;
	reserve(102);
	add(&frog_damage_ability, "frog_damage_ability");
	add(&frog_xy_impulse, "frog_xy_impulse");
	add(&frog_z_impulse, "frog_z_impulse");
	add(&frog_rudder, "frog_rudder");
	add(&frog_contact_damping, "frog_contact_damping");
	add(&frog_fly_damping_W, "frog_fly_damping_W");
	add(&frog_start_jump_phase, "frog_start_jump_phase");
	add(&frog_stop_jump_phase, "frog_stop_jump_phase");
	add(&frog_jump_animation_period, "frog_jump_animation_period");
	add(&frog_stop_animation_period, "frog_stop_animation_period");
	add(&frog_phase_incr, "frog_phase_incr");
	add(&frog_rest_time, "frog_rest_time");
	add(&frog_walk_distance, "frog_walk_distance");
	add(&frog_Vy, "frog_Vy");
	add(&frog_az, "frog_az");
	add(&frog_radius, "frog_radius");
	add(&frog_racer_recover_time, "frog_racer_recover_time");
	add(&dragon_traction, "dragon_traction");
	add(&dragon_racer_traction, "dragon_racer_traction");
	add(&dragon_rudder, "dragon_rudder");
	add(&dragon_width, "dragon_width");
	add(&dragon_length, "dragon_length");
	add(&dragon_height, "dragon_height");
	add(&dragon_lift_bias, "dragon_lift_bias");
	add(&spider_max_distance, "spider_max_distance");
	add(&spider_traction, "spider_traction");
	add(&spider_boss_traction, "spider_boss_traction");
	add(&spider_rudder, "spider_rudder");
	add(&spider_boss_rudder, "spider_boss_rudder");
	add(&spider_antigravity_factor, "spider_antigravity_factor");
	add(&spider_stick_factor, "spider_stick_factor");
	add(&spider_levelling_factor, "spider_levelling_factor");
	add(&spider_not_colliding_avr_time, "spider_not_colliding_avr_time");
	add(&spider_catch_width, "spider_catch_width");
	add(&spider_catch_length, "spider_catch_length");
	add(&spider_foot_speed_factor, "spider_foot_speed_factor");
	add(&stone_torque, "stone_torque");
	add(&stone_TOI_scale, "stone_TOI_scale");
	add(&stone_initial_velocity_xy, "stone_initial_velocity_xy");
	add(&stone_initial_velocity_z, "stone_initial_velocity_z");
	add(&stone_disable_collision_time, "stone_disable_collision_time");
	add(&stone_restitution_plus_one, "stone_restitution_plus_one");
	add(&stone_sound_velocity_threshould, "stone_sound_velocity_threshould");
	add(&debris_Vxy, "debris_Vxy");
	add(&debris_Vz, "debris_Vz");
	add(&debris_W, "debris_W");
	add(&debris_damage_ability, "debris_damage_ability");
	add(&debris_time_to_dwindle, "debris_time_to_dwindle");
	add(&smart_stone_damage_ability, "smart_stone_damage_ability");
	add(&smart_stone_traction, "smart_stone_traction");
	add(&smart_stone_rudder, "smart_stone_rudder");
	add(&smart_stone_restitution, "smart_stone_restitution");
	add(&smart_stone_catch_point_distance, "smart_stone_catch_point_distance");
	add(&smart_stone_assembling_time, "smart_stone_assembling_time");
	add(&smart_stone_antigravity_factor, "smart_stone_antigravity_factor");
	add(&smart_stone_fly_velocity, "smart_stone_fly_velocity");
	add(&shark_spheres_radius, "shark_spheres_radius");
	add(&shark_distance, "shark_distance");
	add(&shark_damage, "shark_damage");
	add(&shark_buoyancy, "shark_buoyancy");
	add(&shark_traction, "shark_traction");
	add(&shark_rudder, "shark_rudder");
	add(&shark_animation_period, "shark_animation_period");
	add(&shark_rudder_duration, "shark_rudder_duration");
	add(&fish_spheres_radius, "fish_spheres_radius");
	add(&fish_distance, "fish_distance");
	add(&fish_damage, "fish_damage");
	add(&fish_buoyancy, "fish_buoyancy");
	add(&fish_traction, "fish_traction");
	add(&whale_TOI_scale, "whale_TOI_scale");
	add(&whale_buoyancy, "whale_buoyancy");
	add(&whale_sphere_offset_z, "whale_sphere_offset_z");
	add(&whale_traction, "whale_traction");
	add(&whale_rudder, "whale_rudder");
	add(&whale_jump_time, "whale_jump_time");
	add(&whale_jump_reverse_time, "whale_jump_reverse_time");
	add(&whale_jump_axis_z, "whale_jump_axis_z");
	add(&whale_jump_torque, "whale_jump_torque");
	add(&whale_jump_point_distance, "whale_jump_point_distance");
	add(&dog_sensor_radius, "dog_sensor_radius");
	add(&dog_hit_velocity2sound_factor, "dog_hit_velocity2sound_factor");
	add(&dog_activation_distance, "dog_activation_distance");
	add(&dog_animation_period, "dog_animation_period");
	add(&wicked_butterfly_traction, "wicked_butterfly_traction");
	add(&wicked_butterfly_distance_to_traction_factor, "wicked_butterfly_distance_to_traction_factor");
	add(&brevno_spheres_radius, "brevno_spheres_radius");
	add(&brevno_mass, "brevno_mass");
	add(&brevno_return_force, "brevno_return_force");
	add(&brevno_linear_damping, "brevno_linear_damping");
	add(&brevno_angular_damping, "brevno_angular_damping");
	add(&brevno_buoyancy, "brevno_buoyancy");
	add(&brevno_torque, "brevno_torque");
	add(&raft_spheres_radius, "raft_spheres_radius");
	add(&raft_mass, "raft_mass");
	add(&raft_buoyancy, "raft_buoyancy");
	add(&ship_spheres_radius, "ship_spheres_radius");
	add(&ship_buoyancy, "ship_buoyancy");
	add(&ship_guard_distance, "ship_guard_distance");
	add(&ship_turn_delay, "ship_turn_delay");
	add(&ship_damage, "ship_damage");
	add(&ship_traction, "ship_traction");
	add(&ship_rudder, "ship_rudder");
	add_parameter_section(this);
}
};
static AnimalsPrm_ParameterSection AnimalsPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: AnimalsPrm
//////////////////////////////////////////////////////////////////////////////////////////////
