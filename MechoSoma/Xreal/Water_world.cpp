#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "XrealWorlds.h"
#include "Mechos.h"
#include "Meteorite.h"
#include "Cannon.h"
#include "Mesh3ds.h"
#include "IVisGeneric.h"


WaterWorld::WaterWorld(int track_)
: WorldDispatcher(track_)
{      
	// Форт
	cMesh* fort = (cMesh*)xrealIVG -> FindObject("Fortress");
	if(fort){
		fort -> SetAnimation(5000);
		attachBaseObject(new Fortress(fort));
		}

	cMesh* pump = ((cMesh*)xrealIVG -> FindObject("Cylinder08")) -> FindMesh("Head");
	if(pump){
		pump -> SetAnimation(5000);
		attachBaseObject(new WaterPump(pump));
		}

	if(track() == 4)
		is_snow = 1;

	if(track() == 5){
		boss_track = 1;
		max_traction = WaterBossMaxTraction;
		}
 }
