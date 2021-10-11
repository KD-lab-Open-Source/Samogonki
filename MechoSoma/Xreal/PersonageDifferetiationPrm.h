//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript declaration
//	Section: PersonageDifferetiationPrm
//	Number of variables: 6
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
extern float mechos_energy_max;
#ifndef __XScript_struct_PersonageDifferetiationData__
#define __XScript_struct_PersonageDifferetiationData__
#pragma pack( push, __XScript_struct_pack__, 4 )
struct PersonageDifferetiationData {
	char const* name;
	char const* russian_name;
	float energy_max;
	float mana_charge_factor;
	float mana_using_speed;
	float arcan_using_effectiveness;
	float rudder_incr;
	float max_rudder_angle_original;
	float mechos_rudder_speed_decrease_factor;
	float mechos_max_velocity_threshould;
	float mechos_traction_speed_decrease_factor;
	float mechos_forward_wheel_drive_torque;
	float mechos_backward_wheel_drive_torque;
	float mechos_auto_brake_torque;
	float mechos_auto_brake_damping;
	float mechos_brake_torque;
	float mechos_density;
	float mechos_CM_z_offset;
	float mechos_wheel_mass;
	float mechos_wheel_penetration;
	float mechos_wheel_z_offset;
	float mechos_suspension_damping_scale;
	float mechos_wheel_friction_factor;
	float mechos_wheel_radius;
	float mechos_xsize_scale;
	float mechos_zsize_scale;
	float mechos_front_sphere_radius;
	float mechos_front_sphere_dy;
	float mechos_front_sphere_dz;
	float mechos_border_sphere_radius;
	float mechos_border_sphere_dx;
	float mechos_border_sphere_dy;
	float mechos_border_sphere_dz;
	float mechos_engine_sphere_radius;
	float mechos_engine_sphere_dz;
};
#pragma pack( pop, __XScript_struct_pack__)
#endif  // __XScript_struct_PersonageDifferetiationData__
extern PersonageDifferetiationData personage_diff_data[10];
#ifndef __XScript_struct_WheelContactProperty__
#define __XScript_struct_WheelContactProperty__
#pragma pack( push, __XScript_struct_pack__, 4 )
struct WheelContactProperty {
	float static_friction;
	float dynamic_friction;
	float brake_torque;
	float brake_damping;
};
#pragma pack( pop, __XScript_struct_pack__)
#endif  // __XScript_struct_WheelContactProperty__
extern WheelContactProperty terrain_contact_property_array[4];
extern WheelContactProperty gear_contact_property;
extern WheelContactProperty slime_contact_property;
extern WheelContactProperty stream_contact_property;
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: PersonageDifferetiationPrm
//////////////////////////////////////////////////////////////////////////////////////////////
