#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "XrealWorlds.h"
#include "Mechos.h"
#include "CrawlingObject.h"
#include "Worm.h"
#include "Params.h"

#include <algorithm>

const int NumWorms = 12;

ForestWorld::ForestWorld(int track_)
: WorldDispatcher(track_)
{
	if(track() != 4){
		for(int i = 0;i < NumWorms; i++)
			createBody("Worm", Vect3f(random(2047), random(2047), 0));
		}
	else 
		is_snow = 1;
	if(track() == 5){
		boss_track = 1;
		water_damage = Forest5_water_damage;
		max_traction = ForestBossMaxTraction;
		}
}

struct IsWorm {
	bool operator()(const Body* b) { return typeid(*b) == typeid(Worm); }
};

void ForestWorld::pre_quant()
{
	BodyDispatcher::pre_quant();
	if(!isSnow() && !quant_latency){
		quant_latency.start(1000);
		int num = std::count_if(total_bodies.begin(), total_bodies.end(), IsWorm());
		if(num < NumWorms)
			createBody("Worm", Vect3f(random(2047), random(2047), 0));
		}
}		
