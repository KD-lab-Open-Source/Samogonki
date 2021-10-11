//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: PersonageDifferetiationPrm
//	Number of variables: 6
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
float mechos_energy_max = 100.f;
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
PersonageDifferetiationData personage_diff_data[10] = { { "PADDER" 
 , "–¿«¡”’", 130.f, 1.f, 1.f, 1.f, 1.2f, 0.8f, 5.e-002f, 75.f, 0.1f, 90.f, 90.f, 4.f 
 , 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f 
 , 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f }, { "NAILER", "’¬Œ«ƒ‹", 120.f, 1.f, 1.f 
 , 1.f, 1.2f, 0.72f, 5.e-002f, 75.f, 0.1f, 100.f, 100.f, 4.f, 0.8f, 100.f, 1.e-004f 
 , 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f 
 , 12.f, 12.f, 12.f }, { "GOGGLEM", "“ﬁ“≈À‹", 100.f, 1.f, 1.f, 1.f, 1.2f, 0.88f, 5.e-002f 
 , 75.f, 0.1f, 110.f, 110.f, 4.f, 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f 
 , 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f }, { "ARK-A-N" 
 , "¿– ¿Õ", 120.f, 1.f, 1.f, 1.f, 1.2f, 0.8f, 5.e-002f, 75.f, 0.1f, 100.f, 100.f, 4.f 
 , 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f 
 , 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f }, { "BELLA", "√»–ﬂ", 140.f, 1.f, 1.f, 1.f 
 , 1.2f, 0.64f, 5.e-002f, 75.f, 0.1f, 110.f, 110.f, 4.f, 0.8f, 100.f, 1.e-004f, 0.f 
 , 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f, 12.f 
 , 12.f, 12.f }, { "4eyes", "œ»—“ŒÕ", 200.f, 1.f, 1.f, 1.f, 1.2f, 0.72f, 5.e-002f, 75.f 
 , 0.1f, 100.f, 100.f, 4.f, 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f 
 , 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f }, { "Lipman", "À¿Ãœ¿" 
 , 180.f, 1.f, 1.f, 1.f, 1.2f, 0.8f, 5.e-002f, 75.f, 0.1f, 90.f, 90.f, 4.f, 0.8f, 100.f 
 , 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f 
 , 0.f, 0.f, 12.f, 12.f, 12.f }, { "Swab", "—¿Ã”–¿…", 200.f, 1.f, 1.f, 1.f, 1.2f, 0.88f 
 , 5.e-002f, 75.f, 0.1f, 90.f, 90.f, 4.f, 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f 
 , 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f } 
 , { "Leechy", "Ãﬂ—Œ–”¡ ¿", 200.f, 1.f, 1.f, 1.f, 1.2f, 0.96f, 5.e-002f, 75.f, 0.1f 
 , 90.f, 90.f, 4.f, 0.8f, 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f 
 , 0.6f, 12.f, 10.f, 12.f, 12.f, 0.f, 0.f, 12.f, 12.f, 12.f }, { "Vampo", " ¿—œ≈–" 
 , 160.f, 1.f, 1.f, 1.f, 1.2f, 0.72f, 5.e-002f, 75.f, 0.1f, 100.f, 100.f, 4.f, 0.8f 
 , 100.f, 1.e-004f, 0.f, 0.1f, 6.f, 3.f, 0.1f, 1.f, 7.f, 1.f, 0.6f, 12.f, 10.f, 12.f 
 , 12.f, 0.f, 0.f, 12.f, 12.f, 12.f } };
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
WheelContactProperty terrain_contact_property_array[4] = { { 5.f 
 , 5.f, 0.f, 0.f }, { 5.f, 5.f, 0.f, 0.f }, { 1.f, 1.f, 0.f, 0.f }, { 0.9f, 0.9f, 0.f 
 , 0.f } };
WheelContactProperty gear_contact_property = { 2.5f, 2.5f, 0.f, 0.f };
WheelContactProperty slime_contact_property = { 0.8f 
 , 0.8f, 0.f, 0.f };
WheelContactProperty stream_contact_property = { 0.8f, 0.8f, 0.f 
 , 0.f };

#ifdef _PRM_EDIT_
struct PersonageDifferetiationPrm_ParameterSection : ParameterSection
{
	PersonageDifferetiationPrm_ParameterSection() : ParameterSection("PersonageDifferetiationPrm")
{
	add_dependency("Scripts/Xreal/PersonageDifferentiation.prm");
	sourceCRC = 2796117795;
	description = 2794591613;
	reserve(6);
	add(&mechos_energy_max, "mechos_energy_max");
	add(&personage_diff_data, "personage_diff_data");
	add(&terrain_contact_property_array, "terrain_contact_property_array");
	add(&gear_contact_property, "gear_contact_property");
	add(&slime_contact_property, "slime_contact_property");
	add(&stream_contact_property, "stream_contact_property");
	add_parameter_section(this);
}
};
static PersonageDifferetiationPrm_ParameterSection PersonageDifferetiationPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: PersonageDifferetiationPrm
//////////////////////////////////////////////////////////////////////////////////////////////
