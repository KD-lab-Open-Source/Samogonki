#include "StdAfx.h"

#include "aci_parser.h"
#include "win32f.h"

#include "handle.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "OwnerProtection.h"
#include "Meteorite.h"

#include "mesh3ds.h"
#include "Levin.h"
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
#include "fxlabClientSound.h"

#include "fxlabServerGeneral.h"
#include "fxlabServerStuff.h"
#include "fxlabServerArcane.h"

//extern XBuffer bout;

void fxlabGeneralObjectDispatcher::Open(void)
{
	if(CreateList.size() != 0)
		ErrH.Abort("Bad fxlabGeneral::CreateList Open");

	if(ObjectList.size() != 0)
		ErrH.Abort("Bad fxlabGeneral::ObjectList Open");
};

void fxlabGeneralObjectDispatcher::Close(void)
{
	list<fxlabGeneralObjectType*>::iterator p;

	p = CreateList.begin();
	while(p != CreateList.end()){
		(*p)->Close();
		delete (*p);
		p = CreateList.erase(p);
	};

	p = ObjectList.begin();
	while(p != ObjectList.end()){
		(*p)->Close();
		delete (*p);
		p = ObjectList.erase(p);
	};

	if(CreateList.size() != 0)
		ErrH.Abort("Bad fxlabGeneral::CreateList Open");

	if(ObjectList.size() != 0)
		ErrH.Abort("Bad fxlabGeneral::ObjectList Open");
};

//XStream fff("fff.fff",XS_OUT);

extern int mchFreezeTime;
extern int fxlabStopTime;

void fxlabGeneralObjectDispatcher::Quant(void)
{
	list<fxlabGeneralObjectType*>::iterator p;

	if(mchFreezeTime || fxlabStopTime){
		p = ObjectList.begin();
		while(p != ObjectList.end()){
			(*p)->StopQuant();
			p++;
		};
	}else{
		p = ObjectList.begin();
		while(p != ObjectList.end()){
			(*p)->Quant();
			p++;
		};
	};
};


void fxlabGeneralObjectDispatcher::CreateQuant(void)
{
	list<fxlabGeneralObjectType*> tmp_list;
	list<fxlabGeneralObjectType*>::iterator p;

	tmp_list = CreateList;
	CreateList.clear();

	p = tmp_list.begin();
	while(p != tmp_list.end()){
		(*p)->Quant();
		++p;
	};

	p = tmp_list.begin();
	while(p != tmp_list.end()){
		ObjectList.push_back((*p));
		p = tmp_list.erase(p);
	};

/*
	int i,id,sz = CreateList.size();
	i = 0;
	while(p != CreateList.end()){
		ObjectList.push_back((*p));
		fff <= i < "\t " <= (*p)->GetType() < " , " <= (*p)->GetID() < "\n";
		id = (*p)->GetType();
		(*p)->Quant();
		p = CreateList.erase(p);

		if(++i > sz) ErrH.Abort("++i > sz");
	};*/
};

fxlabGeneralObjectType* fxlabGeneralObjectDispatcher::CreateObject(int type)
{
	ErrH.Abort("CreateObject function not Defined");
	return NULL;
};

fxlabGeneralObjectType* fxlabGeneralObjectDispatcher::SearchObject(int type)
{
	list<fxlabGeneralObjectType*>::iterator p;

	FOR_EACH(ObjectList,p){
		if((*p)->Type == type)
			return (*p);
	};

	FOR_EACH(CreateList,p){
		if((*p)->Type == type)
			return (*p);
	};
	return NULL;
};

//------------------------------------------

void fxlabServerObjectDispatcher::Open(void)
{
	fxlabGeneralObjectDispatcher::Open();
	ActionOpen();
	LockList = 0;
	RootID = 0;
};

void fxlabServerObjectDispatcher::Close(void)
{
	ActionClose();
	fxlabGeneralObjectDispatcher::Close();
};

void fxlabServerObjectDispatcher::CreateQuant(void)
{
	fxlabGeneralObjectDispatcher::CreateQuant();
	ActionQuant();
};
	
void fxlabServerObjectDispatcher::Start(void)
{
	fxlabGeneralObjectDispatcher::Start();
	Time = global_time();
	LockList = 0;
	ActionList.clear();
	RootID = 0;
};

void fxlabServerObjectDispatcher::Quant(void)
{
	list<fxlabGeneralObjectType*>::iterator p;

	if(!mchFreezeTime && !fxlabStopTime){
		mch_raceD->ClearTrackFlags(0);

		LockList = 1;
		Time = global_time();

//		fxlabGeneralObjectDispatcher::Quant();
//		bout < "FX_server " <= mchFreezeTime < " , " <= fxlabStopTime < " , " <= global_time() < "\n";
//		bout < "FX_server " <= global_time() < "\n";

		p = ObjectList.begin();
		while(p != ObjectList.end()){
			(*p)->Quant();
			p++;
		};

		p = ObjectList.begin();
		while(p != ObjectList.end()){
			if((*p)->GetAlive())
				++p;
			else{
				(*p)->Close();
				delete (*p);
				p = ObjectList.erase(p);
			};
		};

		fxlabServerD->CreateQuant();
		LockList = 0;
	};
//	fff < "Server " <= ObjectList.size() < " , " <= CreateList.size() < "\n";
};

fxlabGeneralObjectType* fxlabServerObjectDispatcher::CreateObject(int type)
{
	fxlabGeneralObjectType* p = NULL;
	switch(type){
		case FXLAB_SERVER_PROCESS_DAMAGE_SENSOR:
			p = new fxlabDamageSensorType;
			break;
		case FXLAB_SERVER_PROCESS_DAMAGE_LINE:
			p = new fxlabDamageLineType;
			break;
		case FXLAB_SERVER_PROCESS_BODY_LINK:
			p = new fxlabServerBodyLinkType;
			break;
/*		case FXLAB_SERVER_PROCESS_BONUS:
			p = new fxlabServerBonusType;
			break;
		case FXLAB_SERVER_PROCESS_BONUS_SINGLE:
			p = new fxlabServerBonusSingle;
			break;
		case FXLAB_SERVER_PROCESS_BONUS_MARKET:
			p = new fxlabServerBonusMarket;
			break;*/
		case FXLAB_SERVER_PROCESS_DRAGON_FIRE:
			p = new fxlabServerDragonFire;
			break;
		case FXLAB_SERVER_PROCESS_SUCK_DAMAGE:
			p = new fxlabServerSuckDamage;
			break;
		case FXLAB_SERVER_PROCESS_WILD_CLAW:
			p = new fxlabServerWildClaw;
			break;
		case FXLAB_SERVER_PROCESS_PLASMA_CLAW:
			p = new fxlabServerPlasmaClaw;
			break;
		case FXLAB_SERVER_PROCESS_BOMB_EXPLOSION:
			p = new fxlabServerBombExplosion;
			break;
		case FXLAB_SERVER_PROCESS_VOLCANIC_LAUNCHER:
			p = new fxlabServerVolcanoLauncher;
			break;
		case FXLAB_SERVER_PROCESS_DRAGON_ROSE:
			p = new fxlabServerDragonRose;
			break;

		case FXLAB_SERVER_PROCESS_VORTEX_FIELD:
			p = new fxlabVortexField;
			break;
		case FXLAB_SERVER_PROCESS_MAGNETIC_FIELD:
			p = new fxlabMagneticField;
			break;
		case FXLAB_SERVER_PROCESS_FLUX_FIELD:
			p = new fxlabFluxField;
			break;
		case FXLAB_SERVER_PROCESS_WIND_FIELD:
			p = new fxlabWindField;
			break;

		case FXLAB_SERVER_PROCESS_WILD_THICKET_CIRCLE:
			p = new fxlabWildThicketCircle;
			break;
		case FXLAB_SERVER_PROCESS_RAGE_SLIME_CIRCLE:
			p = new fxlabRageSlimeCircle;
			break;
		case FXLAB_SERVER_PROCESS_RED_TRACK_CIRCLE:
			p = new fxlabRedTrackCircle;
			break;
		case FXLAB_SERVER_PROCESS_GREEN_SLIME_CIRCLE:
			p = new fxlabGreenSlimeCircle;
			break;
		case FXLAB_SERVER_PROCESS_YELLOW_SLIME_CIRCLE:
			p = new fxlabYellowSlimeCircle;
			break;
		case FXLAB_SERVER_PROCESS_BLUE_SLIME_CIRCLE:
			p = new fxlabBlueSlimeCircle;
			break;
		case FXLAB_SERVER_PROCESS_DAMAGE_CIRCLE:
			p = new fxlabDamageCircleType;
			break;
		case FXLAB_SERVER_PROCESS_BOW_MACHINE_DAMAGE:
			p = new fxlabServerBowMachineDamage;
			break;
		case FXLAB_SERVER_PROCESS_LIGHTNING_SEED_DAMAGE:
			p = new fxlabServerLightningSeedDamage;
			break;
		case FXLAB_SERVER_PROCESS_MASS_SHIFTER:
			p = new fxlabServerMassShifter;
			break;
		case FXLAB_SERVER_PROCESS_CHARGE_ENERGY:
			p = new fxlabServerChargeEnergy;
			break;
		case FXLAB_SERVER_PROCESS_CHARGE_MANA:
			p = new fxlabServerChargeMana;
			break;
		case FXLAB_SERVER_PROCESS_MECHOS_FIRE:
			p = new fxlabServerMechosFire;
			break;
		case FXLAB_SERVER_PROCESS_DRAGON_HEAD_FIRE:
			p = new fxlabServerDragonHeadFire;
			break;
		case FXLAB_SERVER_PROCESS_TRAP_SPHERE:
			p = new fxlabServerTrapSphere;
			break;
		case FXLAB_SERVER_PROCESS_TRAP_PROCESS:
			p = new fxlabServerTrapProcess;
			break;
		case FXLAB_SERVER_PROCESS_SKIN_PROTECTION:
			p = new fxlabServerSkinProtection;
			break;
		case FXLAB_SERVER_PROCESS_ISOLATION_PROTECTION:
			p = new fxlabServerIsolationProtection;
			break;
		case FXLAB_SERVER_PROCESS_ARMOR_PROTECTION:
			p = new fxlabServerArmorProtection;
			break;
		case FXLAB_SERVER_PROCESS_BIO_PROTECTION:
			p = new fxlabServerBioProtection;
			break;
		case FXLAB_SERVER_PROCESS_STATIC_TELEPORT:
			p = new fxlabServerStaticTeleport;
			break;
		case FXLAB_SERVER_PROCESS_SERVER_VOODOO_HEAD:
			p = new fxlabServerVoodooHead;
			break;
		case FXLAB_SERVER_PROCESS_MECHOS_FROZEN:
			p = new fxlabServerFrozenType;
			break;

		case FXLAB_SERVER_PROCESS_PERESTROYKA_CLAW:
			p = new fxlabServerPerestroykaClaw;
			break;

//Arcanes
		case FXLAB_SERVER_PROCESS_ARCANE_DRAGON_WIND:
			p = new fxlabArcaneDragonWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_DIRIGIBLE_WIND:
			p = new fxlabArcaneDirigibleWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_BUTTERFLY_WIND:
			p = new fxlabArcaneButterflyWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FROG_WIND:
			p = new fxlabArcaneFrogWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_HELICOPTER:
			p = new fxlabArcaneHelicopterWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_PLASMA_CLAW:
			p = new fxlabArcanePlasmaClaw;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_BOW_MACHINE:
			p = new fxlabArcaneBowMachine;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SHAFT_MACHINE:
			p = new fxlabArcaneShaftMachine;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FORMICARY_WIND:
			p = new fxlabArcaneFormicaryWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_IRON_SIDE:
			p = new fxlabArcaneIronSide;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_BALL_WIND:
			p = new fxlabArcaneBallWind;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SUICIDE_WARRIOR:
			p = new fxlabArcaneSuicideWarrior;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CLOWN_WARRIOR:
			p = new fxlabArcaneClownWarrior;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_EYE_WARRIOR:
			p = new fxlabArcaneEyeWarrior;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_TOMATO_WARRIOR:
			p = new fxlabArcaneTomatoWarrior;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FIREBALL:
			p = new fxlabArcaneFireBall;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_LIGHTNING_BOLT:
			p = new fxlabArcaneLightningBolt;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_JUMP_BALL:
			p = new fxlabArcaneJumpBall;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SPIDER_SPAWN:
			p = new fxlabArcaneSpiderSpawn;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CATERPILLAR_SPAWN:
			p = new fxlabArcaneCaterpillarSpawn;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FIRE_STONE:
			p = new fxlabArcaneFireStone;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SNOW_FALL:
			p = new fxlabArcaneSnowFall;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_ELECTRIC_STORM:
			p = new fxlabArcaneElecricStorm;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FIRE_GARDEN:
			p = new fxlabArcaneFireGarden;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_RAGE_SLIME:
			p = new fxlabArcaneRageSlime;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_FURY_FUNGUS:
			p = new fxlabArcaneFuryFungus;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_VOODOO_MASTER:
			p = new fxlabArcaneVoodooMaster;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_KINETIC_HIT:
			p = new fxlabArcaneKineticHit;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_LIGHNING_SEED:
			p = new fxlabArcaneLightningSeed;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_CRYSTAL_SPIKE:
			p = new fxlabArcaneCrystalSpike;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_REVOLT_SPACE:
			p = new fxlabRevoltSpace;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_MASS_SHIFTER:
			p = new fxlabArcaneMassShifter;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_BUBBLE_RUSH:
			p = new fxlabArcaneBubbleRush;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SHIFT_TRAP:
			p = new fxlabArcaneShiftTrap;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_MORTAR_MINE:
			p = new fxlabArcaneMortarMine;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_PETARD:
			p = new fxlabArcanePetard;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_SCARLET_TRACK:
			p = new fxlabArcaneScarletTrack;
			break;
		case FXLAB_SERVER_PROCESS_ARCANE_RED_TRACK:
			p = new fxlabArcaneRedTrack;
			break;
		default:
			p = fxlabGeneralObjectDispatcher::CreateObject(type);
			break;
	};

	if(LockList)
		CreateList.push_back(p);
	else
		ObjectList.push_back(p);
	p->Open();
	p->SetType(type);
	p->SetID(++RootID);
	return p;
};

void fxlabServerObjectDispatcher::AddAction(fxlabGeneralObjectType* p,int id)
{
	ActionList.push_back(fxlabGeneralActionType(p,id));
};

void fxlabServerObjectDispatcher::ActionQuant(void)
{
	fxlabGeneralObjectType* l;
	fxlabGeneralObjectType* own;
	list<fxlabGeneralObjectType* >::iterator p;
	list<fxlabGeneralActionType>::iterator ev;
	int id;

	l = NULL;
	ev = ActionList.begin();
	while(ev != ActionList.end()){
		own = ev->Point;
		id = ev->ID;
		if(l != own){
			l = NULL;
			p = ObjectList.begin();
			while(p != ObjectList.end()){
				if(*p == own && (*p)->GetID() == id){
					l = *p;
					break;
				};
				++p;
			};
		};

/*		if(l){
			bout < "fxAA Action " <= id < " , " <= l->GetType() < "\n";
			l->ArcaneAction();
		}else
			bout < "fxAA Ignore " <= id < "\n";*/

		if(l) l->ArcaneAction();
		ev = ActionList.erase(ev);
	};
};

void fxlabServerObjectDispatcher::ActionOpen(void)
{
	if(ActionList.size() != 0)
		ErrH.Abort("Bad fxlabServerObjectDispatcher ActionList");
};

void fxlabServerObjectDispatcher::ActionClose(void)
{
	list<fxlabGeneralActionType>::iterator ev;

	ev = ActionList.begin();
	while(ev != ActionList.end()){
		ev = ActionList.erase(ev);
	};
};

void fxlabServerObjectDispatcher::StopArcane(int id,int owner_id)
{
	list<fxlabGeneralObjectType*>::iterator p;

	FOR_EACH(ObjectList,p){
		if((*p)->GetType() > FXLAB_SERVER_PROCESS_ARCANE)
			(*p)->SetArcaneAlive(id,owner_id);
	};
};

//-------------------------------------------------

void fxlabClientObjectDispatcher::Start(void)
{
	fxlabGeneralObjectDispatcher::Start();
	Time = global_time();
	DeltaTimer.start();
	DeltaTime = 0;
};

void fxlabClientObjectDispatcher::Quant(void)
{
	Time = global_time();
	DeltaTime = DeltaTimer();
	DeltaTimer.start();

	fxlabGeneralObjectDispatcher::Quant();

//	fff < "Client " <= ObjectList.size() < " , " <= CreateList.size() < "\n";
};

fxlabGeneralObjectType* fxlabClientObjectDispatcher::CreateObject(int type)
{
	fxlabGeneralObjectType* p = NULL;
	switch(type){
		case FXLAB_CLIENT_PROCESS_TWISTER:
			p = new fxlabCrazyTwister;
			break;
		case FXLAB_CLIENT_PROCESS_RUBBER_SNAKE:
			p = new fxlabRubberSnake;
			break;
		case FXLAB_CLIENT_PROCESS_EXPLODE:
			p = new fxlabCrazyExplode;
			break;
		case FXLAB_CLIENT_PROCESS_TAIL:
			p = new fxlabCrazyTail;
			break;
		case FXLAB_CLIENT_PROCESS_DUST:
			p = new fxlabCrazyDust;
			break;
		case FXLAB_CLIENT_PROCESS_FEAR_FIRE:
			p = new fxlabFearFire;
			break;
		case FXLAB_CLIENT_PROCESS_SPARK_GENERATOR:
			p = new fxlabSparkGenerator;
			break;
		case FXLAB_CLIENT_PROCESS_FEAR_STREAM:
			p = new fxlabFearStream;
			break;
		case FXLAB_CLIENT_PROCESS_TAIL_WATER_CLIP:
			p = new fxlabCrazyTailWaterClip;
			break;
		case FXLAB_CLIENT_PROCESS_DUST_WATER_CLIP:
			p = new fxlabCrazyDustWaterClip;
			break;
		case FXLAB_CLIENT_PROCESS_SPARK_WATER_CLIP:
			p = new fxlabSparkWaterClip;
			break;
		case FXLAB_CLIENT_PROCESS_COLOR_MODEL:
			p = new fxlabColorModel;
			break;
		case FXLAB_CLIENT_PROCESS_CONTROL_MODEL:
			p = new fxlabControlModel;
			break;
		case FXLAB_CLIENT_PROCESS_CHECK_POINT:
			p = new fxlabCheckPoint;
			break;
		case FXLAB_CLIENT_PROCESS_POLAR_MODEL:
			p = new fxlabPolarModel;
			break;
		case FXLAB_CLIENT_PROCESS_CONTROL_OMNI:
			p = new fxlabControlOmni;
			break;
		case FXLAB_CLIENT_PROCESS_OBJECT_SET:
			p = new fxlabClientSetObjectType;
			break;
		case FXLAB_CLIENT_PROCESS_BODY_LINK:
			p = new fxlabClientBodyLinkType;
			break;
		case FXLAB_CLIENT_PROCESS_WAVE_GROUND:
			p = new fxlabWaveGround;
			break;
		case FXLAB_CLINET_PROCESS_BONUS_MODEL:
			p = new fxlabBonusModel;
			break;
		case FXLAB_CLINET_PROCESS_BONUS_PART:
			p = new fxlabBonusPart;
			break;
		case FXLAB_CLIENT_PROCESS_OBJECT_LAUNCHER:
			p = new fxlabClientObjectLauncher;
			break;
		case FXLAB_CLIENT_PROCESS_OBJECT_SWITCHER:
			p = new fxlabClientObjectSwitcher;
			break;
		case FXLAB_CLIENT_PROCESS_REMOTE_SWITCHER:
			p = new fxlabClientRemoteSwitcher;
			break;

		case FXLAB_CLIENT_PROCESS_MECHOS_LINK:
			p = new fxlabClientMechosLinkType;
			break;
		case FXLAB_CLIENT_PROCESS_FRONT_LINK:
			p = new fxlabClientFrontLink;
			break;
		case FXLAB_CLIENT_PROCESS_BACK_LINK:
			p = new fxlabClinetBackLink;
			break;
		case FXLAB_CLIENT_PROCESS_LEFTFORWARD_WHEEL_LINK:
			p = new fxlabClinetLeftForwardWheelLink;
			break;
		case FXLAB_CLIENT_PROCESS_RIGHTFORWARD_WHEEL_LINK:
			p = new fxlabClientRightForwardWheelLink;
			break;
		case FXLAB_CLIENT_PROCESS_LEFTBACK_WHEEL_LINK:
			p = new fxlabClientLeftBackWheelLink;
			break;
		case FXLAB_CLIENT_PROCESS_RIGHTBACK_WHEEL_LINK:
			p = new fxlabClientRightBackWheelLink;
			break;

		case FXLAB_CLIENT_PROCESS_WILD_CLAW:
			p = new fxlabClientWildClaw;
			break;

		case FXLAB_CLIENT_PROCESS_STREAM_CLUSTER:
			p = new fxlabStreamCluster;
			break;

		case FXLAB_CLIENT_PROCESS_SPARK_WIND:
			p = new fxlabSparkWind;
			break;
		case FXLAB_CLIENT_PROCESS_SPARK_WIND_WATER_CLIP:
			p = new fxlabSparkWindWaterClip;
			break;

		case FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE:
			p = new fxlabLightningSphere;
			break;
		case FXLAB_CLIENT_PROCESS_LIGHTNING_LINE:
			p = new fxlabLightningLine;
			break;
		case FXLAB_CLIENT_PROCESS_LIGHTNING_RADIUS:
			p = new fxlabLightningRadius;
			break;
		case FXLAB_CLIENT_PROCESS_BOW_MACHINE:
			p = new fxlabClientBowMachine;
			break;
		case FXLAB_CLIENT_PROCESS_BOW_MACHINE_FRONT_LINK:
			p = new fxlabClientBowMachineFrontLink;
			break;
		case FXLAB_CLIENT_PROCESS_WILD_CLAW_FRONT_LINK:
			p = new fxlabClientWildClawFrontLink;
			break;
		case FXLAB_CLIENT_PROCESS_LEFT_BALL_WIND_LINK:
			p = new fxlabClientBallWindLinkLeft;
			break;
		case FXLAB_CLIENT_PROCESS_RIGHT_BALL_WIND_LINK:
			p = new fxlabClientBallWindLinkRight;
			break;
		case FXLAB_CLIENT_PROCESS_SUICIDE_LINK:
			p = new fxlabClientSuicideLink;
			break;
		case FXLAB_CLIENT_PROCESS_SLIME_MODEL:
			p = new fxlabSlimeModel;
			break;
		case FXLAB_CLIENT_PROCESS_ANIMATION_MODEL:
			p = new fxlabAnimationModel;
			break;
		case FXLAB_CLIENT_PROCESS_LASH_MODEL:
			p = new fxlabLashModel;
			break;
		case FXLAB_CLIENT_PROCESS_VOODOO_HEAD_MODEL:
			p = new fxlabVoodooHeadModel;
			break;
		case FXLAB_CLIENT_PROCESS_VOODOO_MASTER:
			p = new fxlabClientVoodooMaster;
			break;
		case FXLAB_CLIENT_PROCESS_TERRAIN_DEFORMATOR:
			p = new fxlabTerrainDeformator;
			break;
		case FXLAB_CLIENT_PROCESS_SPARK_GENERATOR_EXTERNAL:
			p = new fxlabSparkGeneratorExternal;
			break;
		case FXLAB_CLIENT_PROCESS_PRESSURE_MODEL:
			p = new fxlabPressureModel;
			break;
		case FXLAB_CLIENT_PROCESS_REVOLT_SPACE_LINK:
			p = new fxlabClientRevoltSpaceLink;
			break;
		case FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MODEL:
			p = new fxlabMassShifterModel;
			break;
		case FXLAB_CLIENT_PROCESS_MASS_SHIFTER:
			p = new fxlabClientMassShifter;
			break;

		case FXLAB_CLIENT_PROCESS_ACTION_JUMP_LEFT:
			p = new fxlabClientJumpActionLeft;
			break;
		case FXLAB_CLIENT_PROCESS_ACTION_JUMP_RIGHT:
			p = new fxlabClientJumpActionRight;
			break;
		case FXLAB_CLIENT_PROCESS_ACTION_NITRO_LEFT:
			p = new fxlabClientNitroActionLeft;
			break;
		case FXLAB_CLIENT_PROCESS_ACTION_NITRO_RIGHT:
			p = new fxlabClientNitroActionRight;
			break;
		case FXLAB_CLIENT_PROCESS_MECHOS_FIRE:
			p = new fxlabClientMechosFire;
			break;
		case FXLAB_CLIENT_PROCESS_MECHOS_CONTROLLER:
			p = new fxlabClientMechosController;
			break;
		case FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER:
			p = new fxlabClientColorController;
			break;

		case FXLAB_CLIENT_PROCESS_SOUND_IMPULSE:
			p = new fxlabClientImpulseSound;
			break;
		case FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE:
			p = new fxlabClientImpulseSpaceSound;
			break;
		case FXLAB_CLIENT_PROCESS_SOUND_LONG:
			p = new fxlabClientLongSound;
			break;
		case FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG:
			p = new fxlabClientLongSpaceSound;
			break;

		case FXLAB_CLIENT_PROCESS_BOLIDE_LINK:
			p = new fxlabClientBolideLink;
			break;
		case FXLAB_CLIENT_PROCESS_HEAD_DUST_LAUNCHER:
			p = new fxlabClientHeadDustLauncher;
			break;

		case FXLAB_CLIENT_PROCESS_FIRE_BALL_LINK:
			p = new fxlabClientFireBallLink;
			break;
		case FXLAB_CLIENT_PROCESS_LIGHTNING_BOLT_LINK:
			p = new fxlabClientLightningBoltLink;
			break;
		case FXLAB_CLIENT_PROCESS_LIGHTNING_SEED:
			p = new fxlabClientLightningSeed;
			break;
		case FXLAB_CLIENT_PROCESS_SHAFT_MECHINE_FRONT_LINK:
			p = new fxlabClientShaftMachineLink;
			break;
		case FXLAB_CLIENT_PROCESS_VOLCANIC_STONE_LINK:
			p = new fxlabClientVolcanicStoneLink;
			break;
		case FXLAB_CLIENT_PROCESS_BOMB_EXPLODE_LINK:
			p = new fxlabClientBombExplodeLink;
			break;

		case FXLAB_CLIENT_PROCESS_DRAGON_FIRE_SWITCHER:
			p = new fxlabClientDragonFireSwitcher;
			break;
		case FXLAB_CLIENT_PROCESS_DRAGON_HEAD_FIRE_SWITCHER:
			p = new fxlabClientDragonHeadFireSwitcher;
			break;
		case FXLAB_CLIENT_PROCESS_REVOLT_MODEL:
			p = new fxlabRevoltSpaceModel;
			break;
		case FXLAB_CLIENT_PROCESS_GROUND_MODEL:
			p = new fxlabTrapGroundModel;
			break;
		case FXLAB_CLIENT_PROCESS_MORTAR_BODY_LINK:
			p = new fxlabClientMortarBodyLink;
			break;
		case FXLAB_CLIENT_PROCESS_PETARD_MODEL:
			p = new fxlabPetardModel;
			break;
		case FXLAB_CLIENT_PROCESS_SPARK_LINE:
			p = new fxlabSparkLine;
			break;
		case FXLAB_CLIENT_PROCESS_PART_CONTROLLER:
			p = new fxlabClientPartController;
			break;

		case FXLAB_CLIENT_PROCESS_TELEPORT_COLOR_CONTROLLER:
			p = new fxlabClientTeleportColorController;
			break;
		case FXLAB_CLIENT_PROCESS_TELEPORT_OMNI_CONTROLLER:
			p = new fxlabClientTeleportOmniController;
			break;
		case FXLAB_CLIENT_PROCESS_SNOW_LAYER:
			p = new fxlabSnowLayer;
			break;
		case FXLAB_CLIENT_PROCESS_BONUS_BODY_LINK:
			p = new fxlabClientBonusBodyLink;
			break;

		case FXLAB_CLIENT_PROCESS_SNOW_FALL_LINK:
			p = new fxlabClientSnowFallLink;
			break;
		case FXLAB_CLIENT_PROCESS_MECHOS_BURN_LINK:
			p = new fxlabClientMechosBurnLink;
			break;
		case FXLAB_CLIENT_PROCESS_BOMB_SPLINTER_LINK:
			p = new fxlabClientBombSplinterLink;
			break;

		case FXLAB_CLIENT_PROCESS_JUMP_ACTION_MODEL:
			p = new fxlabClientJumpActionModel;
			break;
		case FXLAB_CLIENT_PROCESS_JUMP_ACTION_LINK:
			p = new fxlabClientJumpActionLink;
			break;
		case FXLAB_CLIENT_PROCESS_NITRO_ACTION_LINK:
			p = new fxlabClientNitroActionLink;
			break;
		case FXLAB_CLIENT_PROCESS_WATER_WAVE_MODEL:
			p = new fxlabClientWaterWaveModel;
			break;

		case FXLAB_CLIENT_PROCESS_MASS_SHIFTER_NITRO:
			p = new fxlabClientMassShifterNitro;
			break;

		case FXLAB_CLIENT_PROCESS_BULLET_SNOW_MODEL:
			p = new fxlabSnowBulletModel;
			break;
		case FXLAB_CLIENT_PROCESS_BULLET_FIRE_MODEL:
			p = new fxlabFireBulletModel;
			break;
		case FXLAB_CLIENT_PROCESS_VOODOO_MASTER_LINK:
			p = new fxlabClientVoodooMasterLink;
			break;
		case FXLAB_CLIENT_PROCESS_WORLD_IRRADIATE:
			p = new fxlabClientWorldIrradiate;
			break;

		case FXLAB_CLIENT_PROCESS_MASS_SHIFTER_CHARACTER:
			p = new fxlabClientMassShifterCharacter;
			break;
		case FXLAB_CLIENT_PROCESS_SPLINE_WAY:
			p = new fxlabSplineWay;
			break;

		case FXLAB_CLIENT_PROCESS_WIND_SOUND:
			p = new fxlabClientWindSound;
			break;

		case FXLAB_CLIENT_PROCESS_STREAM_FOUNTAIN:
			p = new fxlabClientStreamFountain;					
			break;

		case FXLAB_CLIENT_PROCESS_BOSS_STEAM_GENERATOR:
			p = new fxlabClientCharacterSteamGenerator;
			break;

		case FXLAB_CLIENT_PROCESS_BOSS_SMOKE_GENERATOR:
			p = new fxlabClientCharacterSmokeGenerator;
			break;

		case FXLAB_CLIENT_PROCESS_BOSS_WATERFALL_GENERATOR:
			p = new fxlabClientCharacterWaterfallGenerator;
			break;

//---------------------- Movies ----------------------

		case FXLAB_CLIENT_PROCESS_SET_FACE_FIRE:
			p = new fxlabClientSetFaceFire;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_BOW_REPEATER:
			p = new fxlabClientMovieBowRepeater;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_WAVE_REPEATER:
			p = new fxlabClientMovieWaveRepeater;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_QUAD_LIGHT:
			p = new fxlabClientMovieQuandLight;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_CLOUDS:
			p = new fxlabClientMovieClouds;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_SHIFTING:
			p = new fxlabClientMovieShifting;
			break;
		case FXLAB_CLIENT_PROCESS_MASS_SHIFTER_MOVIE:
			p = new fxlabClientMassShifterMovie;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_TELEPORT:
			p = new fxlabClientMovieTeleport;
			break;
		case FXLAB_CLIENT_PROCESS_FIRE_TREE_MOVIE:
			p = new fxlabClientFireTreeMovie;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_SPOT:
			p = new fxlabClientMovieSpot;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_BOSS_CHANGE:
			p = new fxlabClientMovieBossChange;
			break;
		case FXLAB_CLIENT_PROCESS_MOVIE_DRAGON_FIRE:
			p = new fxlabClientMovieDragonFire;
			break;

		case FXLAB_CLIENT_PROCESS_CHARACTER_WAVE_GENERATOR:
			p = new fxlabClientCharacterWaveGenerator;
			break;

		case FXLAB_CLIENT_PROCESS_BOSS_WORLD_BURST:
			p = new fxlabClientBossWorldBurst;
			break;

		case FXLAB_CLIENT_PROCESS_BOSS_WORLD_EXPLOSION:
			p = new fxlabClientBossWorldExplosion;
			break;

		case FXLAB_CLIENT_PROCESS_MOVIE_ROCKET_FIRE:
			p = new fxlabClientBossRocketFire;
			break;


		default:
			p = fxlabGeneralObjectDispatcher::CreateObject(type);
			break;
	};
	CreateList.push_back(p);
	p->Open();
	p->SetType(type);
	return p;
};

//---------------------------------------

void fxlabRndGenerator::Open(void)
{
	fxlabRNDVAL = /*clock()*/0x83838383;
};

//---------------------------------------


