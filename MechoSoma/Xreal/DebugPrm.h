//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript declaration
//	Section: DebugPrm
//	Number of variables: 40
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
enum WriteModes {
WRITE_BODY_STATE = 1,
WRITE_CAMERA_STATE,
WRITE_BODY_DISPATCHER_STATISTICS = 4
};
enum ShowModes {
SHOW_BODIES = 1,
SHOW_TERRAIN,
SHOW_CAMERA = 4,
SHOW_FIELDS = 8,
SHOW_MAP_OBJECTS = 16
};
extern int Xreal_write_mode;
extern int Xreal_show_mode;
extern int terrain_show_distance;
extern int terrain_show_grid_color;
extern int environment_disabled;
extern int exclusive_keyboard_control;
extern int disable_point_control;
extern int mechos_default_config_enable;
#ifndef __XScript_struct_MechosConfig__
#define __XScript_struct_MechosConfig__
#pragma pack( push, __XScript_struct_pack__, 4 )
struct MechosConfig {
	int array[4];
};
#pragma pack( pop, __XScript_struct_pack__)
#endif  // __XScript_struct_MechosConfig__
extern MechosConfig mechos_default_config[5];
extern int enable_random_assembling;
extern int show_body_active_only;
extern int show_body_radius;
extern int show_body_polyhedrons;
extern int show_body_spheres;
extern int show_body_contacts;
extern int show_body_velocity;
extern int show_point_controlled_points;
extern int show_map_borders;
extern float show_vector_scale;
extern float show_map_x_scale;
extern float show_map_y_scale;
extern float show_map_x_offset;
extern float show_map_y_offset;
extern int camera_perspective;
extern int camera_world_share;
extern int triangle_show_m3d_objects;
extern int triangle_show_terrain;
extern int triangle_show_ForTestCollision;
extern int triangle_show_ForRayCasting;
extern int triangle_show_ForTestSpot;
extern int triangle_show_brush;
extern int triangle_show_grid_area;
extern int triangle_show_normal_by_color;
extern int triangle_show_by_normals;
extern float show_vector_zmin;
extern float show_vector_zmax;
extern int camera_show_track;
extern int camera_show_clip_buffer;
extern int camera_free_fly_by_key;
extern int camera_disable_zoom_by_speed;
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: DebugPrm
//////////////////////////////////////////////////////////////////////////////////////////////
