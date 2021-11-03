#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "XrealWorlds.h"
#include "Xreal_utl.h"
#include "SST_Reader.h"
#include "PointControlledObject.h"
#include "BonusMan.h"
#include "DebugPrm.h"


///////////////////////////////////////////////////////////////////////////////////////////
//		Animals Reading
///////////////////////////////////////////////////////////////////////////////////////////
struct AnimalRecord : DirectedRecord 
{
	enum { type_len = 32 };
	int z;
	int random_angle;
	char type[type_len];
	void read(XBuffer& b) 
	{	
		DirectedRecord::read(b);
		b >= z >= random_angle;
		while(isspace(b()))
			++b;
		char* s = type;
		while(b() && !isspace(b())){
			*s = b();
			++b; s++;
			}
		*s = 0;
		if(type - s > type_len)
			ErrH.Abort("Body Type Name too big:", XERR_USER, type - s, type);
	}
};				    

typedef SST_Reader<AnimalRecord> AnimalReader;


////////////////////////////////////////////////////////////////////////////////
//	WorldDispatcher
////////////////////////////////////////////////////////////////////////////////
WorldDispatcher::WorldDispatcher(int track)
{
	boss_track = 0;
	is_snow = 0;
	water_damage = 0;
	track_ = track;
	max_traction = 2;
	if(environment_disabled)
		return;
	AnimalReader animals(GetTargetName("animals.sst"));
	AnimalReader::iterator ai;
	FOR_EACH(animals, ai)
		createBody(ai -> type, Vect3f(ai -> x, ai -> y, ai -> z), ai -> random_angle ? random(360) : ai -> angle);
}

////////////////////////////////////////////////////////////////////////////////
//	Utils
////////////////////////////////////////////////////////////////////////////////
void WorldDispatcher::createBonusMen(char* str_cfg)
{
	if(environment_disabled || isBossTrack())
		return;

	XBuffer XBuf(str_cfg,strlen(str_cfg));

	int i, n;
	XBuf >= n;
	for(i = 0; i < n; i++)
		Mdisp -> createBody("BonusMan1", Vect3f(random(2048), random(2048), 0), random(360) );

	XBuf >= n;
	for(i = 0; i < n; i++)
		Mdisp -> createBody("BonusMan2", Vect3f(random(2048), random(2048), 0), random(360) );

	XBuf >= n;
	for(i = 0; i < n; i++)
		Mdisp -> createBody("BonusMan3", Vect3f(random(2048), random(2048), 0), random(360) );

	XBuf >= n;
	for(i = 0; i < n; i++)
		Mdisp -> createBody("BonusMan4", Vect3f(random(2048), random(2048), 0), random(360) );
}	

///////////////////////////////////////////////////////////////////////////////////////////
//		Beep
///////////////////////////////////////////////////////////////////////////////////////////
void WorldDispatcher::beep(const Vect3f& pos)
{
	BodyList::iterator i;
	FOR_EACH(total_bodies, i)
		if((*i) -> type() == Body::BONUS_MAN)
			dynamic_cast<BonusMan&>(**i).beep(pos);
}
