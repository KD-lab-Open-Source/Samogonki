#include "StdAfx.h"

#include "aci_parser.h"
#include "win32f.h"

#include "handle.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "OwnerProtection.h"
#include "Meteorite.h"

#include "Levin.h"
#include "TileMap.h"
#include "TileWater.h"
#include "IVisGeneric.h"
#include "terra.h"
#include "race.h"
#include "sound.h"

#include "sur_scr.h"
#include "SST_Reader.h"

#include "m3d_effects.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"

#include "fxlabApplication.h"

#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientEffect.h"
#include "fxlabClientLauncher.h"
#include "Hermit.h"
#include "fxlabClientLightning.h"
#include "fxlabClientModel.h"
#include "fxlabClientStuff.h"

#include "fxlabServerGeneral.h"
#include "fxlabServerStuff.h"
#include "fxlabServerArcane.h"
#include "fxlabBodyInterface.h"

#include "AnimalArcansPrm.h"

void fxlabBombExplosion(class Body* p);
void fxlabVolcanicExplosion(class Body* p);
void fxlabBolideExplosion(class Body* p);
void fxlabSnowBallExplosion(class Body* p);

void fxlabMortarExplosion(class Body* p);
void fxlabMinorBonusExplosion(class Body* p);
void fxlabMajorBonusExplosion(class Body* p);
void fxlabSnowBallAction(class Body* p);

//CannonBomb
void fxlabCannonBomb::kill()
{
	if(alive()){
		fxlabBombExplosion(this);
		mchStartSoundEFF(EFF_METEORITE_SWARM_ADD,R().x,R().y);
	};
	Body::kill();
};

//VolcanicBomb
void fxlabVolcanicBomb::kill()
{
	if(alive()){
		fxlabVolcanicExplosion(this);
		mchStartSoundEFF(EFF_METEORITE_SWARM_ADD,R().x,R().y);
	};
	Body::kill();
};

//FireBolide
void fxlabFireBolide::kill()
{
	if(alive()){
		fxlabBolideExplosion(this);
		mchStartSoundEFF(EFF_METEORITE_SWARM_ADD,R().x,R().y);
	};
	Body::kill();
};

//SnowBall
fxlabSnowBall::fxlabSnowBall(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID)
: StatisticalMeteorite( m3d_model, R_, V_, damage__, owner_ID) 
{ 
	scaleRadius(snow_ball_scale_radius); 
}

void fxlabSnowBall::calc_forces_and_drags()
{
	Meteorite::calc_forces_and_drags();
	dragV.set(snow_ball_dragV);
}

void fxlabSnowBall::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		putArcaneStatistics(body.ID);
		if(alive())
			fxlabSnowBallAction(&body);
		kill();
		}
}
	
void fxlabSnowBall::kill()
{
	if(alive())
		fxlabSnowBallExplosion(this);
	Body::kill();
};

//------------------------------
//Mortar

fxlabSleepyMortar::fxlabSleepyMortar(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID) : BonusMeteorite( m3d_model, R_, V_, damage__, owner_ID)
{
	color_time.start();
};

void fxlabSleepyMortar::kill()
{
	if(alive()){
		fxlabMortarExplosion(this);
		mchStartSoundEFF(EFF_METEORITE_SWARM_ADD,R().x,R().y);
	};
	Body::kill();
};

void fxlabSleepyMortar::quant()
{
	BonusMeteorite::quant();

	float t = sin(M_PI*(float)(color_time()) / 1000.0f);
	xrealIVG -> SetObjectColor(geometry,NULL,&sColor4f(fabs(t),0,0,1.0f));
};

//----------------------------

//MinorBonus
void fxlabMinorBonus::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		dynamic_cast<Mechos&>(body).set_feature(Mechos::HUMBLE);
		kill();
		}
}

void fxlabMinorBonus::kill()
{
	if(alive()){
		fxlabMinorBonusExplosion(this);
		//mchStartSoundEFF(EFF_METEORITE_SWARM_ADD,R().x,R().y);
	};
	Body::kill();
};

//MajorBonus
void fxlabMechosBio(Mechos* p);
void fxlabMajorBonus::body_overlap_reaction(Body& body)
{
	if(body.type() == MECHOS && body.ID != owner()){
		fxlabMechosBio(&dynamic_cast<Mechos&>(body));
		kill();
		}
}
void fxlabMajorBonus::kill()
{
	if(alive())
		fxlabMajorBonusExplosion(this);
	Body::kill();
};

