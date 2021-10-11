#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "XrealWorlds.h"
#include "Mechos.h"
#include "SimpleClip.h"
#include "Stone.h"

MountainWorld::MountainWorld(int track_)
: WorldDispatcher(track_)
{
	float z0 = 120;
	float z1 = 180;
	switch(track()){
		case 2:
			z0 = 330;
			z1 = 390;
			break;
		case 4:
			is_snow = 1;
			z0 = 330;
			z1 = 390;
			break;
		case 5:
			z0 = 470;
			z1 = 530;
			water_damage = 1000;
			boss_track = 1;
			max_traction = MountainBossMaxTraction;
			break;
		}

	attachBaseObject(new StoneGun( Vect3f(1230, 968, z0), Vect3f(1261, 1009, z1) ) );
	attachBaseObject(new StoneGun( Vect3f(836, 782, z0), Vect3f(675, 786, z1) ) );
	attachBaseObject(new StoneGun( Vect3f(1158, 617, z0), Vect3f(1205, 520, z1) ) );
}
