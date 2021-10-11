#include <StdAfx.h>
#include "gen_xreal.h"
#include "gen_game.h"

// AnimalArcans.prm

extern Vect3f bubble_dragV;
extern Vect3f bubble_dragW;

// Arcans.prm

extern Vect3f cannon_ball_dragV;
extern Vect3f cannon_ball_dragW;
extern Vect3f jabasoma_dragV;
extern Vect3f jabasoma_dragW;
extern Vect3f jump_pad_drag;
extern Vect3f magnetic_cushion_dragV;
extern Vect3f magnetic_cushion_dragW;

// Mechos.prm

extern Vect3f mechos_dragV_wheel;
extern Vect3f mechos_dragW_wheel;
extern Vect3f mechos_dragV_flying;
extern Vect3f mechos_dragW_flying;
extern Vect3f mechos_dragV_water;
extern Vect3f mechos_dragW_water;
extern Vect3f personage_dragV;
extern Vect3f personage_dragW;
extern Vect3f standing_personage_dragV;
extern Vect3f standing_personage_dragW;

// Camera.prm

extern Vect3f free_fly_dragV;
extern Vect3f free_fly_torque_factor;
extern Vect3f free_fly_force_factor;


// Animals.prm
extern Vect3f dragon_dragV;
extern Vect3f dragon_dragW;

extern Vect3f spider_dragV;
extern Vect3f spider_dragW;
extern Vect3f spider_normal_target;

extern Vect3f whale_dragV;
extern Vect3f whale_dragW;

extern int whale_jump_points_size;
extern Vect3f whale_jump_points[];

extern Vect3f dog_sensor_offset;

// DebugPrm.h

extern int smart_look_up_table_size;
extern int smart_look_up_table[];

#ifndef __XScript_struct_RandomPart__
#define __XScript_struct_RandomPart__
struct RandomPart {
	int size;
	int n_excluded;
	int excluded[3];
};
#endif  // __XScript_struct_RandomPart__
extern RandomPart random_assembling_kinds[4];
extern int show_body_active_only;