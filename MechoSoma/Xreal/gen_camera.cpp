//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: CameraPrm
//	Number of variables: 85
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
float fCamera = 512.f;
int camera_integration_steps = 1;
float camera_distance_min = 30.f;
int enable_zoom_by_wheel = 1;
float camera_mouse_wheel_sensitivity = 0.125f;
float camera_zoom_sensitivity = 0.01f;
float camera_slope_mass_inv = 0.002f;
float camera_water_slope_mass_inv = 0.001f;
float camera_turn_mass_inv = 0.025f;
float camera_distance_mass_inv = 0.001f; // orig 0.0001
float camera_centering_mass_inv = 0.001f;
float camera_const_input_mass_inv = 0.003f;
float camera_damping_factor = 1.9f;
float camera_position_mass_inv_cliped = 0.1f;
float camera_slope_mass_inv_cliped_up = 0.1f;
float camera_slope_mass_inv_cliped_down = 0.005f;
float camera_turn_mass_inv_cliped = 0.05f;
float camera_distance_mass_inv_cliped = 0.6f;
float camera_damping_factor_cliped = 3.f;
int camera_setMode_interpolation_time = 1000;
int camera_runStartTimeScenario_interpolation_time = 2000;
int camera_setObserver_interpolation_time = 1000;
int camera_enable_clipping = 1;
int camera_enable_static_objects_clipping = 0;
int camera_enable_gears_clipping = 1;
float camera_angle_step = 0.1f;
int from_desired_search_attempts = 50;
float camera_theta_dominance = 10.f;
float camera_transparensy_distance = 35.f;
float camera_transparensy_power = 3.5f;
float clip_ray_radius = 20.f;
float clip_ray_t_min = 0.025f;
float S2G_clip_ray_radius = 0.01f;
float camera_scroll_damping_factor = 1.9f;
float camera_scroll_force_of_distance = 0.2f;
float camera_scroll_mass_inv = 1.f;
float camera_fly_stop_time_scale_x = 0.3f;
float camera_fly_stop_time_scale_y = 0.3f;
float camera_scroll_move_of_distance = 0.002f;
float camera_scroll_move_max = 200.f;
float centering_delta_default = 0.2f;
float centering_delta_max = 0.4f;
float spot_acc_time = 2000.f;
float spot_acc_front = 500.f;
float fly_slope = 60.f;
float zoom_by_speed = 0.4f;
float zoom_by_speed_velocity_max = 50.f;
float start_scenario_slope0 = 90.f;
float start_scenario_distance0 = 700.f;
int start_scenario_time1 = 2000;
int start_scenario_time2 = 400;
float finish_fly_angle_increment = 0.7f;
float track_fly_z_offset = 90.f;
int track_fly_enable_binary_search = 1;
float track_fly_distance = 5.f;
float track_fly_increment_initial = 1.f;
float track_fly_increment_epsilon = 0.005f;
float track_fly_increment = 0.1f;
int rotation_when_stop_time_time = 250;
float rotation_when_stop_time_speed = 1.f;
float stop_time_z_offset = 50.f;
float stop_time_dxy = 0.001f; // orig. 0.0001f
float stop_time_max_step = 5.f;
float stop_time_precision = 1.f;
int stop_time_max_iterations = 100;
float stop_time_hCamera = 512.f;
float stop_time_share_radius_factor = 0.45f;
float free_fly_body_radius = 40.f;
float free_fly_max_climb_angle = 1.8f;
float free_fly_roll_angle_decrement_factor = 0.01f;
int camera_new_clip = 1;
int camera_new_clip_reuse_clipping_number = 4;
int camera_new_clip_x_size = 15;
int camera_new_clip_y_size = 40;
int camera_new_clip_average_distance = 100;
float camera_z_buffer_x_dominance = 10.f;
float camera_z_buffer_delta_z = 30.f;
float camera_z_buffer_z_min = 10.f;
float MAX_SLOPE_ANGLE = 90.f;
float SCAN_X_MAX = 100.f;
float camera_min_slope_angle_to_change_psi = 25.f;
float camera_clip_delta_origin = 60.f;
float camera_clip_delta_final = 60.f;
float camera_clip_distance_delta = 0.f;
float camera_clip_distance_factor = 1.f;
float static_observer_rotation_speed = 1.f;
float camera_turn_angle_delta = 0.f;

#ifdef _PRM_EDIT_
struct CameraPrm_ParameterSection : ParameterSection
{
	CameraPrm_ParameterSection() : ParameterSection("CameraPrm")
{
	add_dependency("Scripts/Xreal/Camera.prm");
	sourceCRC = 2613647483;
	description = 132572427;
	reserve(87);
	add(&fCamera, "fCamera");
	add(&camera_integration_steps, "camera_integration_steps");
	add(&camera_distance_min, "camera_distance_min");
	add(&enable_zoom_by_wheel, "enable_zoom_by_wheel");
	add(&camera_mouse_wheel_sensitivity, "camera_mouse_wheel_sensitivity");
	add(&camera_zoom_sensitivity, "camera_zoom_sensitivity");
	add(&camera_slope_mass_inv, "camera_slope_mass_inv");
	add(&camera_water_slope_mass_inv, "camera_water_slope_mass_inv");
	add(&camera_turn_mass_inv, "camera_turn_mass_inv");
	add(&camera_distance_mass_inv, "camera_distance_mass_inv");
	add(&camera_centering_mass_inv, "camera_centering_mass_inv");
	add(&camera_const_input_mass_inv, "camera_const_input_mass_inv");
	add(&camera_damping_factor, "camera_damping_factor");
	add(&camera_position_mass_inv_cliped, "camera_position_mass_inv_cliped");
	add(&camera_slope_mass_inv_cliped_up, "camera_slope_mass_inv_cliped_up");
	add(&camera_slope_mass_inv_cliped_down, "camera_slope_mass_inv_cliped_down");
	add(&camera_turn_mass_inv_cliped, "camera_turn_mass_inv_cliped");
	add(&camera_distance_mass_inv_cliped, "camera_distance_mass_inv_cliped");
	add(&camera_damping_factor_cliped, "camera_damping_factor_cliped");
	add(&camera_setMode_interpolation_time, "camera_setMode_interpolation_time");
	add(&camera_runStartTimeScenario_interpolation_time, "camera_runStartTimeScenario_interpolation_time");
	add(&camera_setObserver_interpolation_time, "camera_setObserver_interpolation_time");
	add(&camera_enable_clipping, "camera_enable_clipping");
	add(&camera_enable_static_objects_clipping, "camera_enable_static_objects_clipping");
	add(&camera_enable_gears_clipping, "camera_enable_gears_clipping");
	add(&camera_angle_step, "camera_angle_step");
	add(&from_desired_search_attempts, "from_desired_search_attempts");
	add(&camera_theta_dominance, "camera_theta_dominance");
	add(&camera_transparensy_distance, "camera_transparensy_distance");
	add(&camera_transparensy_power, "camera_transparensy_power");
	add(&clip_ray_radius, "clip_ray_radius");
	add(&clip_ray_t_min, "clip_ray_t_min");
	add(&S2G_clip_ray_radius, "S2G_clip_ray_radius");
	add(&camera_scroll_damping_factor, "camera_scroll_damping_factor");
	add(&camera_scroll_force_of_distance, "camera_scroll_force_of_distance");
	add(&camera_scroll_mass_inv, "camera_scroll_mass_inv");
	add(&camera_fly_stop_time_scale_x, "camera_fly_stop_time_scale_x");
	add(&camera_fly_stop_time_scale_y, "camera_fly_stop_time_scale_y");
	add(&camera_scroll_move_of_distance, "camera_scroll_move_of_distance");
	add(&camera_scroll_move_max, "camera_scroll_move_max");
	add(&centering_delta_default, "centering_delta_default");
	add(&centering_delta_max, "centering_delta_max");
	add(&spot_acc_time, "spot_acc_time");
	add(&spot_acc_front, "spot_acc_front");
	add(&fly_slope, "fly_slope");
	add(&zoom_by_speed, "zoom_by_speed");
	add(&zoom_by_speed_velocity_max, "zoom_by_speed_velocity_max");
	add(&start_scenario_slope0, "start_scenario_slope0");
	add(&start_scenario_distance0, "start_scenario_distance0");
	add(&start_scenario_time1, "start_scenario_time1");
	add(&start_scenario_time2, "start_scenario_time2");
	add(&finish_fly_angle_increment, "finish_fly_angle_increment");
	add(&track_fly_z_offset, "track_fly_z_offset");
	add(&track_fly_enable_binary_search, "track_fly_enable_binary_search");
	add(&track_fly_distance, "track_fly_distance");
	add(&track_fly_increment_initial, "track_fly_increment_initial");
	add(&track_fly_increment_epsilon, "track_fly_increment_epsilon");
	add(&track_fly_increment, "track_fly_increment");
	add(&rotation_when_stop_time_time, "rotation_when_stop_time_time");
	add(&rotation_when_stop_time_speed, "rotation_when_stop_time_speed");
	add(&stop_time_z_offset, "stop_time_z_offset");
	add(&stop_time_dxy, "stop_time_dxy");
	add(&stop_time_max_step, "stop_time_max_step");
	add(&stop_time_precision, "stop_time_precision");
	add(&stop_time_max_iterations, "stop_time_max_iterations");
	add(&stop_time_hCamera, "stop_time_hCamera");
	add(&stop_time_share_radius_factor, "stop_time_share_radius_factor");
	add(&free_fly_body_radius, "free_fly_body_radius");
	add(&free_fly_max_climb_angle, "free_fly_max_climb_angle");
	add(&free_fly_roll_angle_decrement_factor, "free_fly_roll_angle_decrement_factor");
	add(&camera_new_clip, "camera_new_clip");
	add(&camera_new_clip_reuse_clipping_number, "camera_new_clip_reuse_clipping_number");
	add(&camera_new_clip_x_size, "camera_new_clip_x_size");
	add(&camera_new_clip_y_size, "camera_new_clip_y_size");
	add(&camera_new_clip_average_distance, "camera_new_clip_average_distance");
	add(&camera_z_buffer_x_dominance, "camera_z_buffer_x_dominance");
	add(&camera_z_buffer_delta_z, "camera_z_buffer_delta_z");
	add(&camera_z_buffer_z_min, "camera_z_buffer_z_min");
	add(&MAX_SLOPE_ANGLE, "MAX_SLOPE_ANGLE");
	add(&SCAN_X_MAX, "SCAN_X_MAX");
	add(&camera_min_slope_angle_to_change_psi, "camera_min_slope_angle_to_change_psi");
	add(&camera_clip_delta_origin, "camera_clip_delta_origin");
	add(&camera_clip_delta_final, "camera_clip_delta_final");
	add(&camera_clip_distance_delta, "camera_clip_distance_delta");
	add(&camera_clip_distance_factor, "camera_clip_distance_factor");
	add(&static_observer_rotation_speed, "static_observer_rotation_speed");
	add(&camera_turn_angle_delta, "camera_turn_angle_delta");
	add_parameter_section(this);
}
};
static CameraPrm_ParameterSection CameraPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: CameraPrm
//////////////////////////////////////////////////////////////////////////////////////////////
