//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: DebugPrm
//	Number of variables: 40
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
int Xreal_write_mode = 0;
int Xreal_show_mode = 0;
int terrain_show_distance = 300;
int terrain_show_grid_color = 0;
int environment_disabled = 0;
int exclusive_keyboard_control = 0;
int disable_point_control = 0;
int mechos_default_config_enable = 0;
#ifndef __XScript_struct_MechosConfig__
#define __XScript_struct_MechosConfig__
#pragma pack( push, __XScript_struct_pack__, 4 )
struct MechosConfig {
	int array[4];
};
#pragma pack( pop, __XScript_struct_pack__)
#endif  // __XScript_struct_MechosConfig__
MechosConfig mechos_default_config[5] = { { { 6 
 , 11, 6, 10 } }, { { 7, 12, 7, 6 } }, { { 8, 13, 8, 12 } }, { { 9, 14, 1, 8 } }, { { 10 
 , 15, 2, 14 } } };
int enable_random_assembling = 0;
int show_body_active_only = 1;
int show_body_radius = 0;
int show_body_polyhedrons = 1;
int show_body_spheres = 1;
int show_body_contacts = 1;
int show_body_velocity = 0;
int show_point_controlled_points = 0;
int show_map_borders = 0;
float show_vector_scale = 10.f;
float show_map_x_scale = 0.27f;
float show_map_y_scale = 0.22f;
float show_map_x_offset = 20.f;
float show_map_y_offset = 10.f;
int camera_perspective = 1;
int camera_world_share = 1;
int triangle_show_m3d_objects = 1;
int triangle_show_terrain = 0;
int triangle_show_ForTestCollision = 1;
int triangle_show_ForRayCasting = 0;
int triangle_show_ForTestSpot = 0;
int triangle_show_brush = 0;
int triangle_show_grid_area = 0;
int triangle_show_normal_by_color = 1;
int triangle_show_by_normals = 0;
float show_vector_zmin = 200.f;
float show_vector_zmax = 4000.f;
int camera_show_track = 1;
int camera_show_clip_buffer = 0;
int camera_free_fly_by_key = 1;
int camera_disable_zoom_by_speed = 0;

#ifdef _PRM_EDIT_
struct DebugPrm_ParameterSection : ParameterSection
{
	DebugPrm_ParameterSection() : ParameterSection("DebugPrm")
{
	add_dependency("Scripts/Xreal/Debug.prm");
	sourceCRC = 2074016590;
	description = 2866283180;
	reserve(40);
	add(&Xreal_write_mode, "Xreal_write_mode");
	add(&Xreal_show_mode, "Xreal_show_mode");
	add(&terrain_show_distance, "terrain_show_distance");
	add(&terrain_show_grid_color, "terrain_show_grid_color");
	add(&environment_disabled, "environment_disabled");
	add(&exclusive_keyboard_control, "exclusive_keyboard_control");
	add(&disable_point_control, "disable_point_control");
	add(&mechos_default_config_enable, "mechos_default_config_enable");
	add(&mechos_default_config, "mechos_default_config");
	add(&enable_random_assembling, "enable_random_assembling");
	add(&show_body_active_only, "show_body_active_only");
	add(&show_body_radius, "show_body_radius");
	add(&show_body_polyhedrons, "show_body_polyhedrons");
	add(&show_body_spheres, "show_body_spheres");
	add(&show_body_contacts, "show_body_contacts");
	add(&show_body_velocity, "show_body_velocity");
	add(&show_point_controlled_points, "show_point_controlled_points");
	add(&show_map_borders, "show_map_borders");
	add(&show_vector_scale, "show_vector_scale");
	add(&show_map_x_scale, "show_map_x_scale");
	add(&show_map_y_scale, "show_map_y_scale");
	add(&show_map_x_offset, "show_map_x_offset");
	add(&show_map_y_offset, "show_map_y_offset");
	add(&camera_perspective, "camera_perspective");
	add(&camera_world_share, "camera_world_share");
	add(&triangle_show_m3d_objects, "triangle_show_m3d_objects");
	add(&triangle_show_terrain, "triangle_show_terrain");
	add(&triangle_show_ForTestCollision, "triangle_show_ForTestCollision");
	add(&triangle_show_ForRayCasting, "triangle_show_ForRayCasting");
	add(&triangle_show_ForTestSpot, "triangle_show_ForTestSpot");
	add(&triangle_show_brush, "triangle_show_brush");
	add(&triangle_show_grid_area, "triangle_show_grid_area");
	add(&triangle_show_normal_by_color, "triangle_show_normal_by_color");
	add(&triangle_show_by_normals, "triangle_show_by_normals");
	add(&show_vector_zmin, "show_vector_zmin");
	add(&show_vector_zmax, "show_vector_zmax");
	add(&camera_show_track, "camera_show_track");
	add(&camera_show_clip_buffer, "camera_show_clip_buffer");
	add(&camera_free_fly_by_key, "camera_free_fly_by_key");
	add(&camera_disable_zoom_by_speed, "camera_disable_zoom_by_speed");
	add_parameter_section(this);
}
};
static DebugPrm_ParameterSection DebugPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: DebugPrm
//////////////////////////////////////////////////////////////////////////////////////////////
