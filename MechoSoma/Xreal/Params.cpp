#include "Params.h"

// AnimalArcans.prm

// Vect3f bubble_dragV(0.5, 0.5, 0.5);
// Vect3f bubble_dragW(0.5, 0.5, 0.5);

// Arcans.prm

Vect3f cannon_ball_dragV(4, 0.1, 0.1);
Vect3f cannon_ball_dragW(5, 5, 5);
Vect3f jabasoma_dragV(4, 0.1, 0.1);
Vect3f jabasoma_dragW(2, 2, 2);
Vect3f jump_pad_drag(1., 1., 1.);
Vect3f magnetic_cushion_dragV(.4, 0.4, 0.4);
Vect3f magnetic_cushion_dragW(2, 2, 2);

// Mechos.prm

Vect3f mechos_dragV_wheel(0.0, 0.0, 0.0);
Vect3f mechos_dragW_wheel(2., 2., 2.);//{ 2, 2, 2.01);
Vect3f mechos_dragV_flying(0.05, 0.05, 0.05);
Vect3f mechos_dragW_flying(2, 2, 4);
Vect3f mechos_dragV_water(2, 0.12, 1);
Vect3f mechos_dragW_water(2, 2, 4);
Vect3f personage_dragV(2, 0.1, 0.2);
Vect3f personage_dragW(2, 2, 2);
Vect3f standing_personage_dragV(10, 10, 5);
Vect3f standing_personage_dragW(2, 2, 1);
Vect3f bubble_dragV(0.5, 0.5, 0.5);
Vect3f bubble_dragW(0.5, 0.5, 0.5);

// Camera.prm

Vect3f free_fly_dragV(4., 4., 4.);
Vect3f free_fly_torque_factor(0.0025,0.01,-0.0025);
Vect3f free_fly_force_factor(-200,-200,-200);

// Animals.prm

Vect3f dragon_dragV(8, 0.6, 0.7);
Vect3f dragon_dragW(1, 1, 8);

Vect3f spider_dragV(8, 0.6, 0.3);
Vect3f spider_dragW(1, 2, 2);
Vect3f spider_normal_target(0, 0.2, 1);

Vect3f whale_dragV(4, 0.6, 0.3);
Vect3f whale_dragW(0.8, 1.5, 4);

int whale_jump_points_size = 4;
Vect3f whale_jump_points[4] = {
	Vect3f( 467, 618, 0 ), 
	Vect3f( 1494, 1549, 0 ),  
	Vect3f( 742, 38, 0 ), 
	Vect3f( 1843, 1086, 0 )
};
Vect3f dog_sensor_offset(0, 1.2, .4);

// Debug.prm

int smart_look_up_table_size = 32;
int smart_look_up_table[32] = { 
	1870030148,
 543450484,
	1830842228,
 1919033465,
	1701344367,
 1833181298,
	1769108585,
 1867194489,
	1634298996,
 1460276594,
	1814065000,
 1684371055,
 1835099936,
 539784037,
544503138,
 544432488,
 1701736295,
 1869575200,
 1918985504,
 774797676,
 537529646,
 538976288,
 538976288,
538976288,
 538976288,
 538976288,
 1092624416,
 1635280236,
 1919247470,
 1953450784,
 1918986604,
 -842150656 
 };

extern RandomPart random_assembling_kinds[4] = {
	{ 10, 0, {} }, 
	{ 15, 0, {} }, 
	{ 8, 0, {} }, 
	{ 14, 3, { 6, 7, 8 } }
};