//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: XRealPrm
//	Number of variables: 59
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
int syncro_by_clock = 1;
int STANDART_FRAME_PERIOD = 40;
int MAX_TIME_INTERVAL = 300;
int racer_quant_period = 91;
int xreal_quant_period = 91;
int xreal_evolve_period = 13;
float fxlabServerQuantPeriod = 91.f;
float fxlabGlobalTimeRateMs = 20.f;
float fxlabParticleSystemScale = 1.f;
float evolve_time_step_ms = 13.f;
float evolve_time_step = 0.07f;
float real_time_to_evolve_time = 5.4f;
float Gravity = 25.f;
float Density = 0.0001f;
float general_k_restitution_plus_one = 1.1f;
float max_height_threshould = 1000.f;
float max_velocity_threshould_general = 90.f;
float max_velocity_threshould_meteorite = 140.f;
float max_angular_velocity_threshould = 4.f;
float free_linear_damping = 0.05f;
float free_angular_damping = 0.1f;
float contact_linear_damping = 2.f;
float contact_angular_damping = 2.f;
float hit_reaction_velocity_threshould = 5.f;
float velocity_to_damage_factor = 0.01f;
float press_reaction_force_threshould = 0.5f;
float minimal_volume_level = 0.1f;
float archimedean_force_wave_time_factor = 0.005f;
float archimedean_force_wave_distance_factor = 0.08f;
float archimedean_force_wave_phase = 1.8f;
float archimedean_force_damping = 0.1f;
float triangle_clip_epsilon = 0.01f;
float get_placement_pose_water_dz = -5.f;
int rest_system_enabled = 0;
float rest_force2_threshould = 0.0000001f;
float rest_torque2_threshould = 0.0000001f;
float rest_velocity2_threshould = 0.05f;
float rest_angular_velocity2_threshould = 0.05f;
int constraint_impulse_calculation = 2;
float impulse_resolve_velocity_threshould = 0.01f;
float velocity_relaxation_value = 0.01f;
int constraint_force_calculation = 0;
float force_resolve_acceleration_threshould = 0.01f;
float acceleration_relaxation_value = 0.01f;
float penetration2acceleration_relaxation_factor = 0.01f;
float impact_friction_factor = 1.f;
float dynamic_friction_factor = 0.2f;
float static_friction_factor = 0.4f;
float VelocityZeroThreshould = 0.001f;
float MinDynamicFrictionVelocity = 0.5f;
float AccelerationZeroThreshould = 0.0001f;
float FrictionZeroThreshould = 0.01f;
float linear_projection_tolerance = 0.1f;
int UseCollisionHandler = 0;
int CollisionHandlerRelaxationType = 2;
float CollisionHandlerRelaxationTime = 10.f;
float CollisionHandlerTimeStep = 0.f;
float CollisionHandlerVelocityTolerance = 0.01f;
float CollisionHandlerPenetrationTolerance = 0.01f;

#ifdef _PRM_EDIT_
struct XRealPrm_ParameterSection : ParameterSection
{
	XRealPrm_ParameterSection() : ParameterSection("XRealPrm")
{
	add_dependency("Scripts/Xreal/Xreal.prm");
	sourceCRC = 3312608748;
	description = 2789218692;
	reserve(59);
	add(&syncro_by_clock, "syncro_by_clock");
	add(&STANDART_FRAME_PERIOD, "STANDART_FRAME_PERIOD");
	add(&MAX_TIME_INTERVAL, "MAX_TIME_INTERVAL");
	add(&racer_quant_period, "racer_quant_period");
	add(&xreal_quant_period, "xreal_quant_period");
	add(&xreal_evolve_period, "xreal_evolve_period");
	add(&fxlabServerQuantPeriod, "fxlabServerQuantPeriod");
	add(&fxlabGlobalTimeRateMs, "fxlabGlobalTimeRateMs");
	add(&fxlabParticleSystemScale, "fxlabParticleSystemScale");
	add(&evolve_time_step_ms, "evolve_time_step_ms");
	add(&evolve_time_step, "evolve_time_step");
	add(&real_time_to_evolve_time, "real_time_to_evolve_time");
	add(&Gravity, "Gravity");
	add(&Density, "Density");
	add(&general_k_restitution_plus_one, "general_k_restitution_plus_one");
	add(&max_height_threshould, "max_height_threshould");
	add(&max_velocity_threshould_general, "max_velocity_threshould_general");
	add(&max_velocity_threshould_meteorite, "max_velocity_threshould_meteorite");
	add(&max_angular_velocity_threshould, "max_angular_velocity_threshould");
	add(&free_linear_damping, "free_linear_damping");
	add(&free_angular_damping, "free_angular_damping");
	add(&contact_linear_damping, "contact_linear_damping");
	add(&contact_angular_damping, "contact_angular_damping");
	add(&hit_reaction_velocity_threshould, "hit_reaction_velocity_threshould");
	add(&velocity_to_damage_factor, "velocity_to_damage_factor");
	add(&press_reaction_force_threshould, "press_reaction_force_threshould");
	add(&minimal_volume_level, "minimal_volume_level");
	add(&archimedean_force_wave_time_factor, "archimedean_force_wave_time_factor");
	add(&archimedean_force_wave_distance_factor, "archimedean_force_wave_distance_factor");
	add(&archimedean_force_wave_phase, "archimedean_force_wave_phase");
	add(&archimedean_force_damping, "archimedean_force_damping");
	add(&triangle_clip_epsilon, "triangle_clip_epsilon");
	add(&get_placement_pose_water_dz, "get_placement_pose_water_dz");
	add(&rest_system_enabled, "rest_system_enabled");
	add(&rest_force2_threshould, "rest_force2_threshould");
	add(&rest_torque2_threshould, "rest_torque2_threshould");
	add(&rest_velocity2_threshould, "rest_velocity2_threshould");
	add(&rest_angular_velocity2_threshould, "rest_angular_velocity2_threshould");
	add(&constraint_impulse_calculation, "constraint_impulse_calculation");
	add(&impulse_resolve_velocity_threshould, "impulse_resolve_velocity_threshould");
	add(&velocity_relaxation_value, "velocity_relaxation_value");
	add(&constraint_force_calculation, "constraint_force_calculation");
	add(&force_resolve_acceleration_threshould, "force_resolve_acceleration_threshould");
	add(&acceleration_relaxation_value, "acceleration_relaxation_value");
	add(&penetration2acceleration_relaxation_factor, "penetration2acceleration_relaxation_factor");
	add(&impact_friction_factor, "impact_friction_factor");
	add(&dynamic_friction_factor, "dynamic_friction_factor");
	add(&static_friction_factor, "static_friction_factor");
	add(&VelocityZeroThreshould, "VelocityZeroThreshould");
	add(&MinDynamicFrictionVelocity, "MinDynamicFrictionVelocity");
	add(&AccelerationZeroThreshould, "AccelerationZeroThreshould");
	add(&FrictionZeroThreshould, "FrictionZeroThreshould");
	add(&linear_projection_tolerance, "linear_projection_tolerance");
	add(&UseCollisionHandler, "UseCollisionHandler");
	add(&CollisionHandlerRelaxationType, "CollisionHandlerRelaxationType");
	add(&CollisionHandlerRelaxationTime, "CollisionHandlerRelaxationTime");
	add(&CollisionHandlerTimeStep, "CollisionHandlerTimeStep");
	add(&CollisionHandlerVelocityTolerance, "CollisionHandlerVelocityTolerance");
	add(&CollisionHandlerPenetrationTolerance, "CollisionHandlerPenetrationTolerance");
	add_parameter_section(this);
}
};
static XRealPrm_ParameterSection XRealPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: XRealPrm
//////////////////////////////////////////////////////////////////////////////////////////////
