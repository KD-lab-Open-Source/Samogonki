/////////////////////////////////////////////////////////////////////
//		Worlds
/////////////////////////////////////////////////////////////////////
#include "Params.h"

class WaterWorld : public WorldDispatcher 
{
public:
	WaterWorld(int track);
};

class LavaWorld : public WorldDispatcher 
{
public:
	LavaWorld(int track_): WorldDispatcher(track_) { water_damage = 1000; if(track() == 4){ boss_track = 1; max_traction = LavaBossMaxTraction; } }
};

class ForestWorld : public WorldDispatcher 
{
	DurationTimer quant_latency;
	void pre_quant();
public:
	ForestWorld(int track);
};

class WatchWorld : public WorldDispatcher 
{
public:
	WatchWorld(int track_) : WorldDispatcher(track_) { if(track() == 5){ boss_track = 1; max_traction = WatchBossMaxTraction; } }
};

class MountainWorld : public WorldDispatcher 
{
public:
	MountainWorld(int track);
};

class TownWorld : public WorldDispatcher 
{
public:
	TownWorld(int track) : WorldDispatcher(track) {}
};

class InterfaceWorld : public WorldDispatcher 
{
public:
	InterfaceWorld(int track) : WorldDispatcher(track) {}
};


//	Utils
char* GetTargetName(char* name);
