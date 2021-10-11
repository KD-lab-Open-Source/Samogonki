#include "StdAfx.h"

#include "aci_parser.h"
#include "terra.h"

#include "BodyDispatcher.h"
#include "Mechos.h"
#include "CustomFieldSources.h"
#include "keys.h"
#include "ctl_point.h"
#include "race.h"
#include "arcane_menu.h"
#include "sound.h"

#include "I-World.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabServerGeneral.h"
#include "fxlabServerStuff.h"
#include "fxlabServerArcane.h"
#include "mesh3ds.h"

#include "win32f.h"
#include "mechosoma.h"
#include "fxlabBodyInterface.h"

#include "TrackIterator.h"

void fxlabChargeMana(class mchMechosRacer* p,float power);

void fxlabDamageSensorType::Open(void)
{
	fxlabServerKeyObjectType::Open();
	Position = Vect3f::ZERO;
};

void fxlabDamageSensorType::Quant(void)
{
	fxlabServerKeyObjectType::Quant();
	Action();
};

void fxlabDamageSensorType::Action(void)
{
	mchRacer* mp;
	int dx,dy,dz;
	int d;
	int x,y,z;
	int r2;
	mchMechosRacer* mrp;

	mp = mch_raceD->racerLst->first();
	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	switch(SensorType){
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_ENERGY:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(d < r2*r2){											
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						mrp->charge(Power);
						mrp->fxlabMechosInfo.EnergyTimer.start(FXLAB_MECHOS_IMFORMATION_ENERGY_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_MANA:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(d < r2*r2){											
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						fxlabChargeMana(mrp,Power);
						mrp->fxlabMechosInfo.ManaTimer.start(FXLAB_MECHOS_IMFORMATION_MANA_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_KINETIC:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(d < r2*r2){
						static_cast<mchMechosRacer*>(mp) -> kinetic_impulse(Position);
						fxlabArcaneAction(this, mp -> Body::ID);
					};
				};
				mp = mp->next;
			};
			break;
		default:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(d < r2*r2){
						mp->damage(Power,SensorType);
						fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(mp))->Mechos::ID);
					};
				};
				mp = mp->next;
			};
			FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,Radius,((SensorType == FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING) ? TRACK_EL_ELECTRICITY : TRACK_EL_FIRE));
			break;
	};
};

void fxlabDamageSensorType::KeyUpdate(void)
{
	fxlabServerKeyObjectType::KeyUpdate();
	Power = KeyData[FXLAB_DAMAGE_SENSOR_DATA_POWER] * fxlabServerTimeRate;
	Radius = KeyData[FXLAB_DAMAGE_SENSOR_DATA_RADIUS];
	SensorType = round(KeyData[FXLAB_DAMAGE_SENSOR_DATA_TYPE]);
};

void fxlabDamageSensorType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_DAMAGE_SENSOR_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabDamageSphere");
};

//-----------------------------------------------------------------

void fxlabDamageLineType::Open(void)
{
	fxlabDamageSensorType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabDamageLineType::Action(void)
{
	mchRacer* mp;
	int dx,dy,dz;
	int d;
	int x,y,z;
	int r2;
	Vect3f v;
	mchMechosRacer* mrp;

	mp = mch_raceD->racerLst->first();
	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	v = Velocity;
	v *= KeyData[FXLAB_DAMAGE_LINE_DATA_LENGTH];

	switch(SensorType){
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_ENERGY:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(fxlabLine2SphereDist(v,Vect3f((float)dx,(float)dy,(float)dz),r2)){
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						mrp->charge(Power);
						mrp->fxlabMechosInfo.EnergyTimer.start(FXLAB_MECHOS_IMFORMATION_ENERGY_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_MANA:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(fxlabLine2SphereDist(v,Vect3f((float)dx,(float)dy,(float)dz),r2)){
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						fxlabChargeMana(mrp,Power);
						mrp->fxlabMechosInfo.ManaTimer.start(FXLAB_MECHOS_IMFORMATION_MANA_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_KINETIC:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(fxlabLine2SphereDist(v,Vect3f((float)dx,(float)dy,(float)dz),r2)){
						static_cast<mchMechosRacer*>(mp) -> kinetic_impulse(Position);
						fxlabArcaneAction(this, mp -> Body::ID);
					};
				};
				mp = mp->next;
			};
			break;
		default:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					r2 = round(Radius + mp->radius());
					if(fxlabLine2SphereDist(v,Vect3f((float)dx,(float)dy,(float)dz),r2)){
						mp->damage(Power,SensorType);
						fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(mp))->Mechos::ID);
					};
				};
				mp = mp->next;
			};
			v *= 0.5f;
			v += Position;
			v = CYCLE(v);
			FXLAB_TRACK_AI_UPDATE(v.x,v.y,Radius,((SensorType == FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING) ? TRACK_EL_ELECTRICITY : TRACK_EL_FIRE));
			break;
	};
};

void fxlabDamageLineType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_DAMAGE_LINE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabDamageLineType");
};

//-----------------------------------------

void fxlabDamageCircleType::Action(void)
{
	mchRacer* mp;
	int dx,dy;
	int d;
	int x,y,z;
	int r2,mr2;
	mchMechosRacer* mrp;

	mp = mch_raceD->racerLst->first();
	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	switch(SensorType){
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_ENERGY:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS && abs(round(mp->R().z) - z) < Height){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					d = dx*dx + dy*dy;
					r2 = round(Radius + mp->radius());
					mr2 = round(MinRadius + mp->radius());
					if(d < r2*r2 && d > mr2*mr2){
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						mrp->charge(Power);
						mrp->fxlabMechosInfo.EnergyTimer.start(FXLAB_MECHOS_IMFORMATION_ENERGY_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_MANA:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS && abs(round(mp->R().z) - z) < Height){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					d = dx*dx + dy*dy;
					r2 = round(Radius + mp->radius());
					mr2 = round(MinRadius + mp->radius());
					if(d < r2*r2 && d > mr2*mr2){
						mrp = static_cast<mchMechosRacer*>(mp);
						fxlabArcaneAction(this,mrp->Mechos::ID);
						fxlabChargeMana(mrp,Power);
						mrp->fxlabMechosInfo.ManaTimer.start(FXLAB_MECHOS_IMFORMATION_MANA_DELAY);
					};
				};
				mp = mp->next;
			};
			break;
		case FXLAB_DAMAGE_SPHERE_TYPE_KINETIC:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS && abs(round(mp->R().z) - z) < Height){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					d = dx*dx + dy*dy;
					r2 = round(Radius + mp->radius());
					mr2 = round(MinRadius + mp->radius());
					if(d < r2*r2 && d > mr2*mr2){
						static_cast<mchMechosRacer*>(mp) -> kinetic_impulse(Position);
						fxlabArcaneAction(this, mp -> Body::ID);
					};
				};
				mp = mp->next;
			};
			break;
		default:
			while(mp){
				if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS && abs(round(mp->R().z) - z) < Height){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					d = dx*dx + dy*dy;
					r2 = round(Radius + mp->radius());
					mr2 = round(MinRadius + mp->radius());
					if(d < r2*r2 && d > mr2*mr2){
						mp->damage(Power,SensorType);
						fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(mp))->Mechos::ID);
					};
				};
				mp = mp->next;
			};
			FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,Radius,((SensorType == FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING) ? TRACK_EL_ELECTRICITY : TRACK_EL_FIRE));
			break;
	};
};

void fxlabDamageCircleType::KeyUpdate(void)
{
	fxlabDamageSensorType::KeyUpdate();
	MinRadius = KeyData[FXLAB_DAMAGE_CIRCLE_DATA_MIN_RADIUS];
	Height = KeyData[FXLAB_DAMAGE_CIRCLE_DATA_HEIGHT];
};

void fxlabDamageCircleType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_DAMAGE_CIRCLE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabDamageCircleType");
};

//-----------------------------------------

void fxlabServerBodyLinkType::Quant(void)
{
	fxlabServerKeyObjectType::Quant();
	if(Alive){
		if(Core){
			if(!(Core->alive())){
				Destroy();
				Alive = 0;
			}else
				Position = Core->R();
		}else Destroy();
	}else{
		if(Core && Core->alive())
			Destroy();
	};
};

void fxlabServerBodyLinkType::Destroy(void)
{
	fxlabGeneralObjectType* t;

	if(round(KeyData[FXLAB_SERVER_BODY_LINK_DATA_TYPE]) >= 0){
		t = fxlabServerD->CreateObject(round(KeyData[FXLAB_SERVER_BODY_LINK_DATA_TYPE]));
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetPosition(Position);
		t->SetKeyID(round(KeyData[FXLAB_SERVER_BODY_LINK_DATA_ID]));
		t->SetProtection(ProtectionID,ProtectionTime);
		t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		t->Start();
	};	
	Alive = 0;
};

void fxlabServerBodyLinkType::SetBody(class Body* p)
{
	Core = p;
	Position = Core->R();
};

void fxlabServerBodyLinkType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_BODY_LINK_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerBodyLinkType");
};

//--------------------------------------------
void fxlabTakeBonus(int type, const Vect3f& pos_)
{
	Vect3f pos(pos_);
	fxlabGeneralObjectType* t;
	
	switch(type){
		case 2://Charge
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_CHARGE);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 5://Jump
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_JUMP);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 3://Mana
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_MANA);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 4://Stars
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_STARS);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 1://Nitro
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_NITRO);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;

		case 10://FireProtection
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_FIRE);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 11://ElectricProtection
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_ELECTRIC);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 12://ArmorProtection
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_ARMOR);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
		case 13://BioProtection
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_BIO);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;

		default:
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
			t->SetKeyID(FXLAB_ID_KEY_PETARD_BONUS);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetPosition(pos);
			t->Start();
			break;
	};
};

/*void fxlabServerBonusType::Open(void)
{
	fxlabServerSpaceType::Open();
	LinkPoint = NULL;
	BonusModelType = -1;
	BonusModelID = -1;
	BonusActionID = ARCANE053;
	BonusPartID = 0;
};

void fxlabServerBonusType::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerSpaceType::Start();

	if(BonusModelType >= 0){
		t = fxlabClientD->CreateObject(BonusModelType);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetPosition(Position);
		t->SetKeyID(BonusModelID);
		t->SetProcessInterface(&ModelPoint);
		t->SetBonusPartID(BonusPartID);
		t->Start();
	};

	LinkPoint = Mdisp -> createBonus(round(Position.x),round(Position.y),round(Position.z),BonusActionID);
	LinkPoint->Process = this;
};

void fxlabServerBonusType::Close(void)
{
	if(ModelPoint.Process)
		ModelPoint.Process->SetAlive(0);

	if(LinkPoint){
		LinkPoint->Init();
		LinkPoint = NULL;
	};

	fxlabServerSpaceType::Close();
};

void fxlabServerBonusType::SetAlive(int alive)
{
	fxlabServerSpaceType::SetAlive(alive);
	if(!alive && LinkPoint){
		LinkPoint->Init();
		LinkPoint = NULL;
	};
};

void fxlabDestroyBonus(struct fxlabProcessInterface* p)
{
	if(p && p->Process)
		p->Process->SetAlive(0);
};

//-------------------------------------------

void fxlabServerBonusSingle::Open(void)
{
	fxlabServerSpaceType::Open();
	BonusActionID = ARCANE053;
	BonusPartID = 0;
};

void fxlabServerBonusSingle::Start(void)
{
	fxlabServerSpaceType::Start();
	Generate();
};

void fxlabServerBonusSingle::Close(void)
{
	if(BonusPoint.Process)
		BonusPoint.Process->SetAlive(0);
	fxlabServerSpaceType::Close();
};

void fxlabServerBonusSingle::SetBonusActionID(int id)
{ 
	BonusActionID = id; 
	if(BonusActionID == -1)
		BonusPartID = M3D_TOTAL_TYPE(fxlabServerRND.RND(5) + 1,M3D_ENGINE);
};

void fxlabServerBonusSingle::Generate(void)
{
	fxlabGeneralObjectType* t;

	switch(BonusActionID){
		case ARCANE053://Charge
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_MODEL);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_CHARGE);
			t->SetBonusActionID(BonusActionID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
		case ARCANE054://Jump
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_MODEL);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_JUMP);
			t->SetBonusActionID(BonusActionID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
		case ARCANE055://Nitro
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_MODEL);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_NITRO);
			t->SetBonusActionID(BonusActionID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
		case ARCANE062://Mana
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_MODEL);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_MANA);
			t->SetBonusActionID(BonusActionID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
		case ARCANE063://Stars
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_MODEL);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_STARS);
			t->SetBonusActionID(BonusActionID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
		case -1: //Part
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BONUS);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetPosition(Position);
			t->SetBonusModelType(FXLAB_CLINET_PROCESS_BONUS_PART);
			t->SetBonusModelID(FXLAB_ID_KEY_BONUS_MODEL_PART);
			t->SetBonusActionID(BonusActionID);
			t->SetBonusPartID(BonusPartID);
			t->SetProcessInterface(&BonusPoint);
			t->Start();
			break;
	};
};

void fxlabServerBonusMarket::Open(void)
{
	fxlabServerBonusSingle::Open();
	RespawnTime = 0;
	WaitFlag = 1;
};

void fxlabServerBonusMarket::Start(void)
{
	RespawnTimer.start(0);
	fxlabServerBonusSingle::Start();
};

void fxlabServerBonusMarket::Quant(void)
{
	fxlabServerBonusSingle::Quant();

	if(!BonusPoint.Process){
		if(WaitFlag){
			RespawnTimer.start(RespawnTime);
			WaitFlag = 0;
		}else{
			if(!RespawnTimer()){
				Generate();
				WaitFlag = 1;
			};
		};
	};
};
*/

//-------------------------------------------------

void fxlabServerDragonFire::Open(void)
{
	fxlabDamageLineType::Open();
	EnableFlag = 0;
};

void fxlabServerDragonFire::Close(void)
{
	if(RemotePoint.Process)
		RemotePoint.Process->SetAlive(0);
	fxlabDamageLineType::Close();
};

void fxlabServerDragonFire::Start(void)
{
	fxlabDamageLineType::Start();
	FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_FIRE_DATA_START_TIME]));
};

void fxlabServerDragonFire::Action(void)
{
	if(EnableFlag){
		if(FireTimer())
			fxlabDamageLineType::Action();
		else{
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(0);
			EnableFlag = 0;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_FIRE_DATA_DISABLE_TIME]));
		};
	}else{
		if(!FireTimer()){
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(1);
			EnableFlag = 1;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_FIRE_DATA_ENABLE_TIME]));
		};
	};
};

void fxlabServerDragonFire::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_DRAGON_FIRE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerDragonFire");
};

void fxlabServerDragonFire::SetRemoteObject(fxlabGeneralObjectType* p)
{
	p->SetRemoteInterface(&RemotePoint);
};


void fxlabDragonRose(const Vect3f& pos,const Vect3f& vel);

void fxlabServerDragonHeadFire::Action(void)
{
	if(EnableFlag){
		if(FireTimer()){
			fxlabDamageLineType::Action();
			if(!fxlabServerRND.RND(2)) fxlabDragonRose(Position,Velocity);
		}else{
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(0);

			EnableFlag = 0;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_FIRE_DATA_DISABLE_TIME]));
		};
	}else{
		if(!FireTimer()){
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(1);
			EnableFlag = 1;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_FIRE_DATA_ENABLE_TIME]));
		};
	};			
};

//------------------------------------------------------


void fxlabServerSuckDamage::Open(void)
{
	fxlabServerKeyObjectType::Open();
	Target = NULL;
};

void fxlabServerSuckDamage::Close(void)
{
	if(RemotePoint.Process)
		RemotePoint.Process->SetAlive(0);
	fxlabServerKeyObjectType::Close();
};

void fxlabServerSuckDamage::Quant(void)
{
	fxlabServerKeyObjectType::Quant();
	CheckTarget();
	if(Target) Action();
	CheckAlive();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_SERVER_SUCK_DAMAGE_PASSIVE_RADIUS],((DamageType == FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING) ? TRACK_EL_ELECTRICITY : TRACK_EL_FIRE));
};

void fxlabServerSuckDamage::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_SUCK_DAMAGE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerSuckDamage");
};

void fxlabServerSuckDamage::KeyUpdate(void)
{
	fxlabServerKeyObjectType::KeyUpdate();
	Power = KeyData[FXLAB_SERVER_SUCK_DAMAGE_POWER] * fxlabServerTimeRate;
	DamageType = round(KeyData[FXLAB_SERVER_SUCK_DAMAGE_TYPE]);

	ActiveRadius2 = KeyData[FXLAB_SERVER_SUCK_DAMAGE_ACTIVE_RADIUS];
	ActiveRadius2 *= ActiveRadius2;
	PassiveRadius2 = KeyData[FXLAB_SERVER_SUCK_DAMAGE_PASSIVE_RADIUS];
	PassiveRadius2 *= PassiveRadius2;
};

void fxlabServerSuckDamage::SetRemoteObject(fxlabGeneralObjectType* p)
{
	p->SetRemoteInterface(&RemotePoint);
};

void fxlabServerSuckDamage::CheckTarget(void)
{
	mchRacer* mp;
	int dx,dy,dz;
	int d,td;
	int x,y,z;

	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	if(Target){
		if(Target->completed() && (Target->Energy() > 0 || (static_cast<mchMechosRacer*>(Target)->features() & Mechos::NON_DESTRUCTING))){
			dx = getDistX(round(Target->R().x),x);
			dy = getDistY(round(Target->R().y),y);
			dz = round(Target->R().z) - z;
			td = dx*dx + dy*dy + dz*dz;
			if(td > ActiveRadius2)
				TargetLost();
		}else
			TargetLost();
	}else{
		mp = mch_raceD->racerLst->first();
		while(mp){
			if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
				dx = getDistX(round(mp->R().x),x);
				dy = getDistY(round(mp->R().y),y);
				dz = round(mp->R().z) - z;
				d = dx*dx + dy*dy + dz*dz;
				if(d < PassiveRadius2){
					if(!Target || d < td){
						td = d;
						Target = mp;
					};
				};
			};
			mp = mp->next;
		};
		if(Target)
			AssignTarget(Target);
	};

	if(RemotePoint.Process){
		if(Target){
			RemotePoint.Process->SetRemoteID(Target->ID);
		}else{
			RemotePoint.Process->SetRemoteID(-1);
		};
	};
};

void fxlabServerSuckDamage::Action(void)
{
	Target->damage(Power,DamageType);
	fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(Target))->Mechos::ID);
	if(Target->Energy() <= 0)
		TargetDestruction();
};

void fxlabServerSuckDamage::TargetDestruction(void)
{
	Target = NULL;
};

void fxlabServerSuckDamage::TargetLost(void)
{
	Target = NULL;
};

void fxlabServerSuckDamage::AssignTarget(struct mchRacer* Target)
{
};

void fxlabServerSuckDamage::CheckAlive(void)
{
};

//-------------------------------------

void fxlabServerWildClaw::Action(void)
{
	if(!ErectionTimer())
		fxlabServerSuckDamage::Action();
};

void fxlabServerWildClaw::AssignTarget(struct mchRacer* target)
{
	ErectionTimer.start(round(KeyData[FXLAB_SERVER_WILD_CLAW_ERECTION_TIME]));
};

void fxlabServerWildClaw::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_WILD_CLAW_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerWildClaw");
};

void fxlabServerPlasmaClaw::Open(void)
{
	fxlabServerWildClaw::Open();
	OwnerPoint = NULL;
};

void fxlabServerPlasmaClaw::Action(void)
{
	if(!ErectionTimer()){
		if(OwnerPoint){
			if(OwnerPoint->Energy() < OwnerPoint->energy_max()){
				OwnerPoint->charge(Power);
				Target->damage(Power,DamageType);
			};
		}else
			Target->damage(Power,DamageType);

		fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(Target))->Mechos::ID);
		if(Target->Energy() <= 0)
			TargetDestruction();
	};
};

//--------------------------------------------

void fxlabServerPerestroykaClaw::CheckTarget(void)
{
	mchRacer* mp;
	int dx,dy,dz;
	int d,td;
	int x,y,z;
	mchMechosRacer* m;

	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	if(Target){
		m = dynamic_cast<mchMechosRacer*>(Target);
		if(Target->completed() && m && (Target->Energy() > 0 || (m->features() & Mechos::NON_DESTRUCTING)) && m->isFlyingByArcane() && !(m->features() & (Mechos::JABASOMA | Mechos::FLYING_BY_JUMP_PAD))){
			dx = getDistX(round(Target->R().x),x);
			dy = getDistY(round(Target->R().y),y);
			dz = round(Target->R().z) - z;
			td = dx*dx + dy*dy + dz*dz;
			if(td > ActiveRadius2)
				TargetLost();
		}else
			TargetLost();
	}else{
		mp = mch_raceD->racerLst->first();
		while(mp){
			if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
				m = dynamic_cast<mchMechosRacer*>(mp);
				if(m && m->isFlyingByArcane() && !(m->features() & (Mechos::JABASOMA | Mechos::FLYING_BY_JUMP_PAD))){
					dx = getDistX(round(mp->R().x),x);
					dy = getDistY(round(mp->R().y),y);
					dz = round(mp->R().z) - z;
					d = dx*dx + dy*dy + dz*dz;
					if(d < PassiveRadius2){
						if(!Target || d < td){
							td = d;
							Target = mp;
						};
					};
				};
			};
			mp = mp->next;
		};
		if(Target)
			AssignTarget(Target);
	};

	if(RemotePoint.Process){
		if(Target){
			RemotePoint.Process->SetRemoteID(Target->ID);
		}else{
			RemotePoint.Process->SetRemoteID(-1);
		};
	};
};

//--------------------------------------------

void fxlabServerBombExplosion::Start(void)
{
	fxlabDamageSensorType::Start();
//!!!!!
	fxlabCreateSplinter<Meteorite>(FXLAB_MODEL_INTERFACE_ID_SPLINTER_START,FXLAB_MODEL_INTERFACE_ID_SPLINTER_NOISE,Position,5,fxlabServerRND.UnitRND()*M_PI,M_PI*0.05f,0,50.0f,10.0f,FXLAB_CLIENT_PROCESS_BOMB_SPLINTER_LINK,static_cast<ArcaneStatisticsInfo*>(this),static_cast<fxlabProtectionType*>(this),NULL);
};

//--------------------------------------------

void fxlabServerStoneLauncher::Quant(void)
{
	fxlabServerKeyObjectType::Quant();
	Action();
};

void fxlabServerStoneLauncher::Action(void)
{
	int i,n;
	float f,r;

	r = KeyData[FXLAB_SERVER_STONE_LAUNCHER_DATA_RATE] * fxlabServerTimeRate;
	f = floor(r);
	if(fxlabServerRND.UnitRND() < r - f)
		n = round(f) + 1;
	else
		n = round(f);

	for(i = 0;i < n;i++)
		Generate();
};

void fxlabServerStoneLauncher::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_STONE_LAUNCHER_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerStoneLauncher");
};

//----------------------------------------------


void fxlabServerVolcanoLauncher::Generate(void)
{
	Vect3f v;
	v.setSpherical(fxlabServerRND.UnitRND()*2.0f*M_PI,KeyData[FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_THETTA] + fxlabServerRND.UnitRND() * KeyData[FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_DELTA_THETTA],KeyData[FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_SPEED] + fxlabServerRND.UnitRND() * KeyData[FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_DELTA_SPEED]);
//!!!!!
	fxlabCreateBolide<fxlabVolcanicBomb>(FXLAB_MODEL_INTERFACE_ID_VOLCANO_START,FXLAB_MODEL_INTERFACE_ID_VOLCANO_NOISE,Position,v,10.0,FXLAB_CLIENT_PROCESS_VOLCANIC_STONE_LINK,NULL,NULL,NULL);
};

void fxlabServerVolcanoLauncher::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_VOLCANO_LAUNCHER_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerVolcanoLauncher");
};

//------------------------------------------

void fxlabServerDragonRose::Open(void)
{
	fxlabDamageSensorType::Open();
	EnableFlag = 0;
};

void fxlabServerDragonRose::Close(void)
{
	if(RemotePoint.Process)
		RemotePoint.Process->SetAlive(0);
	fxlabDamageSensorType::Close();
};

void fxlabServerDragonRose::Start(void)
{
	fxlabDamageSensorType::Start();
	FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_ROSE_DATA_START_TIME]));
};

void fxlabServerDragonRose::Action(void)
{
	if(EnableFlag){
		if(FireTimer())
			fxlabDamageSensorType::Action();
		else{
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(0);
			EnableFlag = 0;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_ROSE_DATA_DISABLE_TIME]));
		};
	}else{
		if(!FireTimer()){
			if(RemotePoint.Process)
				RemotePoint.Process->SetRemoteID(1);
			EnableFlag = 1;
			FireTimer.start(round(KeyData[FXLAB_SERVER_DRAGON_ROSE_DATA_ENABLE_TIME]));
		};
	};			
};

void fxlabServerDragonRose::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_DRAGON_ROSE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerDragonRose");
};

void fxlabServerDragonRose::SetRemoteObject(fxlabGeneralObjectType* p)
{
	p->SetRemoteInterface(&RemotePoint);
};

//--------------------------------------------------

void fxlabFieldLinkType::Open(void)
{
	fxlabServerKeyObjectType::Open();
	FieldPoint = NULL;
};

void fxlabFieldLinkType::Close(void)
{
	if(FieldPoint) 
		Mdisp -> deleteFieldSource(FieldPoint);
	fxlabServerKeyObjectType::Close();
};

void fxlabFieldLinkType::Start(void)
{
	OwnerProtection* op;

	fxlabServerKeyObjectType::Start();
	ArcaneStatisticsService::setCurrentArcane(*this);
	Generate();
	op = dynamic_cast<OwnerProtection*>(FieldPoint);
	if(op) op->set_owner(ProtectionID,ProtectionTime);
	Mdisp -> attachFieldSource(FieldPoint);
};

void fxlabFieldLinkType::Quant(void)
{
	fxlabServerKeyObjectType::Quant();
	Mdisp -> changeFieldSource(FieldPoint,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS]);	
};


void fxlabFieldLinkType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FIELD_LINK_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabFieldLinkType");
};

void fxlabFieldLinkType::SetPosition(Vect3f& v)
{
	fxlabServerKeyObjectType::SetPosition(v);
	if(FieldPoint) 
		Mdisp -> moveFieldSource(FieldPoint,Position);
};


void fxlabCircleLinkType::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_CIRCLE_LINK_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabCircleLinkType");
};

void fxlabVortexField::Generate(void)
{
	FieldPoint = new VortexFieldSource(Position,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],ProtectionID);
};

void fxlabMagneticField::Generate(void)
{
	FieldPoint = new MagneticFieldSource(Position,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],ProtectionID);
};

//-------------------------------------------

void fxlabStreamField::Open(void)
{
	fxlabFieldLinkType::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabStreamField::Quant(void)
{
	fxlabFieldLinkType::Quant();
	(static_cast<UniformFieldSource*>(FieldPoint))->setForce(Velocity * KeyData[FXLAB_FIELD_LINK_DATA_POWER]);
};


void fxlabFluxField::Generate(void)
{
	FieldPoint = new StreamFieldSource(Position,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],Velocity);
	(static_cast<UniformFieldSource*>(FieldPoint))->setForce(Velocity * KeyData[FXLAB_FIELD_LINK_DATA_POWER]);
};

void fxlabFluxField::Quant(void)
{
	fxlabStreamField::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};

void fxlabWindField::Generate(void)
{
	FieldPoint = new WindFieldSource(Position,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],Velocity);
	(static_cast<UniformFieldSource*>(FieldPoint))->setForce(Velocity * KeyData[FXLAB_FIELD_LINK_DATA_POWER]);
};

void fxlabWindField::Quant(void)
{
	fxlabStreamField::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};

//--------------------------------------------------

void fxlabWildThicketCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::WILD_THICKET,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabWildThicketCircle::Quant(void)
{
	fxlabCircleLinkType::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};


void fxlabRageSlimeCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::RAGE_OF_SLIME,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabRageSlimeCircle::Quant(void)
{
	fxlabCircleLinkType::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};

void fxlabRedTrackCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::RED_TRACK,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabGreenSlimeCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::GREEN_SLIME,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabGreenSlimeCircle::Quant(void)
{
	fxlabCircleLinkType::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};


void fxlabYellowSlimeCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::YELLOW_SLIME,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabYellowSlimeCircle::Quant(void)
{
	fxlabCircleLinkType::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};

void fxlabBlueSlimeCircle::Generate(void)
{
	FieldPoint = new CircleMapSource(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],FieldSource::BLUE_SLIME,ProtectionID,KeyData[FXLAB_CIRCLE_LINK_DATA_MIN_Z] + Position.z,KeyData[FXLAB_CIRCLE_LINK_DATA_MAX_Z] + Position.z);
};

void fxlabBlueSlimeCircle::Quant(void)
{
	fxlabCircleLinkType::Quant();
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,KeyData[FXLAB_FIELD_LINK_DATA_RADIUS],TRACK_EL_SLIME);
};

//-----------------------------------------------------

void fxlabServerBowMachineDamage::TargetDestruction(void)
{
	fxlabServerSuckDamage::TargetDestruction();
	SetAlive(0);
};

//--------------------------------------------------------
  
void fxlabServerLightningSeedDamage::Action(void)
{
	if(!LifeTimer())
		SetAlive(0);
	else
		fxlabServerBowMachineDamage::Action();
};

void fxlabServerLightningSeedDamage::AssignTarget(struct mchRacer* target)
{
	fxlabServerBowMachineDamage::AssignTarget(target);
	LifeTimer.start(2000);
};

void fxlabServerLightningSeedDamage::TargetLost(void)
{
	fxlabServerBowMachineDamage::TargetLost();
	SetAlive(0);
};

//----------------------------

const float FXLAB_SERVER_MASS_SHIFTER_TIME = 20.0f;

void fxlabServerMassShifter::Open(void)
{
	fxlabServerSpaceType::Open();
	Owner = NULL;
};

void fxlabServerMassShifter::Start(void)
{
	fxlabServerSpaceType::Start();
	CalcBranch();
	Generate();

	Owner->set_feature(Mechos::STATIC_NULIFICATOR);
	Owner->set_feature(Mechos::PROTECT_FROM_DAMAGE);
};


void fxlabServerMassShifter::Generate(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_NITRO);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_PRIMARY_MASS_SHIFTER);
	t->SetPosition(Position);
	t->Start();	

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER_NITRO);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SECONDARY_MASS_SHIFTER);
	t->SetPosition(Target);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI_NITRO);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI_NITRO);
	t->SetPosition(Target);
	t->Start();

/*	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
	t->SetPosition(Target);
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_START_TELEPORT);
	t->SetPosition(Position);
	t->Start();	
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_PASSAGE);
	t->SetMechosPoint(Owner);
	t->Start();
};

void fxlabServerMassShifter::Quant(void)
{
	fxlabGeneralObjectType* t;
	fxlabServerSpaceType::Quant();

	if(!CountDown()){
		Owner->setPosition(Target,Direction);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_TELEPORT_ADD);
		t->SetPosition(Target);
		t->Start();

		if(Owner->flags & MCH_AI){
			Owner->RemoveSeeds();
			Owner->AddSeed(Owner->R());
		};
		SetAlive(0);
	};
};

void fxlabServerMassShifter::Close(void)
{
	Owner->reset_feature(Mechos::STATIC_NULIFICATOR);
	Owner->reset_feature(Mechos::PROTECT_FROM_DAMAGE);
	fxlabServerSpaceType::Close();
};

void fxlabServerMassShifter::CalcBranch(void)
{
	int px,py;
	mchCheckpoint* next;
	TrackIterator ti;
	Vect3f v;

	CountDown.start(round(FXLAB_SERVER_MASS_SHIFTER_TIME / fxlabGlobalTimeRate));

	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD){
		v = Vect3f(0,Owner->radius() * 10,0) * Mat3f(Owner->psi(),Z_AXIS);
		px = XCYCL(round(v.x + Owner->R().x));
		py = YCYCL(round(v.y + Owner->R().y));
		Direction = v;
		Direction *= 2.0f;
		Direction += Owner->R();
		Direction = CYCLE(Direction);
	}else{
		next = mch_raceD->getCP(Owner->stPtr->Checkpoint);
		if(next){
			px = XCYCL(next->PosX0 + getDistX(next->PosX1,next->PosX0) / 2);
			py = YCYCL(next->PosY0 + getDistY(next->PosY1,next->PosY0) / 2);
		}else{
			px = Owner->R().x;
			py = Owner->R().y;
		};

		ti.set(Owner->stPtr->Checkpoint);
		ti += 3;
		Direction = ti;
	};

	Target = Vect3f(px,py,fxlabGetLevel(px,py,Owner->R().z));

	v = getDist(Target,Direction);
	v.Normalize(Owner->radius() * 1.5f);
	Target += v;
	CYCLE(Target);
	Target.z = fxlabGetLevel(XCYCL(round(Target.x)),YCYCL(round(Target.y)),Owner->R().z);
};

//-------------------------------

void fxlabServerChargeType::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
	Value = 0;
	ChargeTime = 1.0f;
};

void fxlabServerChargeType::Start(void)
{
	fxlabServerEvolutionType::Start();
	DeltaValue = Value / ChargeTime;
};

void fxlabServerChargeType::SetChargeTime(float t)
{
	ChargeTime = t / fxlabServerQuantPeriod; 
};

//-------------------------------

void fxlabServerChargeEnergy::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerChargeType::Start();

	if(Owner->active()){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_BONUS_ENERGY);
		t->SetPosition(Vect3f(Owner->R()));
		t->Start();
	};
};

void fxlabServerChargeEnergy::Close(void)
{
	if(ChargePoint.Process)
		ChargePoint.Process->SetAlive(0);
	fxlabServerChargeType::Close();
};

void fxlabServerChargeEnergy::Quant(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerChargeType::Quant();
	if(Owner->completed() && Owner->Energy() < Owner->energy_max()){
		if(!ChargePoint.Process){
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_LINK);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_CHARGE_ENERGY_CYCLE_LINK);
			t->SetBody(Owner);
			t->SetProcessInterface(&ChargePoint);
			t->Start();
		};
		Owner->charge(DeltaValue);
		Value -= DeltaValue;
		if(Value <= 0)
		   SetAlive(0);
	}else
		SetAlive(0);
};

//-------------------------------

void fxlabServerChargeMana::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerChargeType::Start();

	if(Owner->active()){
		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_BONUS_SPELL);
		t->SetPosition(Vect3f(Owner->R()));
		t->Start();
	};
};

void fxlabServerChargeMana::Close(void)
{
	if(ChargePoint.Process)
		ChargePoint.Process->SetAlive(0);
	fxlabServerChargeType::Close();
};

void fxlabServerChargeMana::Quant(void)
{
	int i,cnt;
	mchArcaneData* t;
	fxlabGeneralObjectType* n;

	fxlabServerChargeType::Quant();
	cnt = 0;
	if(Owner->completed()){
		for(i = 1;i < NUM_ACTIVE_PARTS;i++){
			t = &(Owner->part_by_index(i)->arcaneData);
//			if(t->Status){
				if(t->Power < t->MaxPower){
					if(!ChargePoint.Process){
						n = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_LINK);
						n->SetStartTime(fxlabClientD->GetTime());
						n->SetKeyID(FXLAB_ID_KEY_CHARGE_MANA_CYCLE_LINK);
						n->SetBody(Owner);
						n->SetProcessInterface(&ChargePoint);
						n->Start();
					};
					t->Power += DeltaValue;
					if(t->Power > t->MaxPower)
						t->Power = t->MaxPower;
					Value -= DeltaValue;
					if(Value <= 0){
					   SetAlive(0);
					   break;
					};
				}else cnt++;
//			}else cnt++;
		};
		if(cnt >= NUM_ACTIVE_PARTS - 1)
			SetAlive(0);
	}else{
		SetAlive(0);
	};
};

//---------------------------------------------

void fxlabServerMechosFire::Start(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerChargeType::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MECHOS_FIRE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_MECHOS_FIRE_LINK);
	t->SetBody(Owner);
	t->SetProcessInterface(&FirePoint);
	t->Start();
};

void fxlabServerMechosFire::Close(void)
{
	if(FirePoint.Process)
		FirePoint.Process->SetAlive(0);
	fxlabServerChargeType::Close();
};

void fxlabServerMechosFire::Quant(void)
{
	fxlabServerChargeType::Quant();	
	if(Owner->completed() && Owner->Energy() > 0 && !FXLAB_MECHOS_IN_WATER(Owner) && !(Owner->features() & Mechos::FIRE_PROTECTION)){
		Owner->damage(DeltaValue);
		Value -= DeltaValue;
		if(Value <= 0)
		   SetAlive(0);
	}else
		SetAlive(0);
};

//------------------------------------------------------

void fxlabServerTrapSphere::Quant(void)
{
	mchRacer* mp;
	float dx,dy,dz;
	float d;
	float r2;

	fxlabServerKeyObjectType::Quant();

	mp = mch_raceD->racerLst->first();

	while(mp){
		if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
			dz = mp->R().z - Position.z;
			if(fabs(dz) < Height && !(static_cast<mchMechosRacer*>(mp)->isFlyingByArcane())){
				dx = getDistX(mp->R().x,Position.x);
				dy = getDistY(mp->R().y,Position.y);
				d = dx*dx + dy*dy;
				r2 = Radius + mp->radius();
				if(d < r2*r2)
					Action(mp);
			};
		};
		mp = mp->next;
	};
	FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,Radius,TRACK_EL_TELEPORT);
};

void fxlabServerTrapSphere::Action(struct mchRacer* p)
{
	fxlabGeneralObjectType* t;

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_TRAP_PROCESS);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetPosition(Position);
	t->SetMechosPoint(static_cast<mchMechosRacer*>(p));
	t->Start();
	fxlabArcaneAction(this,(static_cast<mchMechosRacer*>(p))->Mechos::ID);
	SetAlive(0);
};

void fxlabServerTrapSphere::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_TRAP_SPHERE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerTrapSphere");
};

void fxlabServerTrapSphere::KeyUpdate(void)
{
	fxlabServerKeyObjectType::KeyUpdate();
	Radius = KeyData[FXLAB_TRAP_SPHERE_DATA_RADIUS];
	Height = KeyData[FXLAB_TRAP_SPHERE_DATA_HEIGHT];
};

//---------------------------------------------------------

void fxlabServerTrapProcess::Start(void)
{
	fxlabServerMassShifter::Start();
	Owner->RemoveSeeds();
};

void fxlabServerTrapProcess::Generate(void)
{
	fxlabGeneralObjectType* t;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_PRIMARY_MASS_SHIFTER);
	t->SetPosition(Position);
	t->Start();	

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MASS_SHIFTER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SECONDARY_MASS_SHIFTER);
	t->SetPosition(Target);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_OMNI);
	t->SetPosition(Target);
	t->Start();

/*	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_TELEPORT_BURN);
	t->SetPosition(Target);
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_START_TELEPORT);
	t->SetPosition(Position);
	t->Start();	
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_PASSAGE);
	t->SetMechosPoint(Owner);
	t->Start();
};

void fxlabServerTrapProcess::CalcBranch(void)
{
	int px,py;
	Vect3f v;
	TrackIterator ti;

	CountDown.start(round(FXLAB_SERVER_MASS_SHIFTER_TIME / fxlabGlobalTimeRate));

	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD){
		v = Vect3f(0,-Owner->radius() * 10,0) * Mat3f(Owner->psi(),Z_AXIS);
		px = XCYCL(round(v.x + Owner->R().x));
		py = YCYCL(round(v.y + Owner->R().y));
		Direction = v;
		Direction *= 2.0f;
		Direction += Owner->R();
		Direction = CYCLE(Direction);
		Target = Vect3f(px,py,fxlabGetLevel(px,py,Owner->R().z));
	}else{
/*		next = mch_raceD->getCP(Owner->stPtr->Checkpoint - 1);
		if(next){
			px = XCYCL(next->prev->PosX0 + getDistX(next->prev->PosX1,next->prev->PosX0) / 2);
			py = YCYCL(next->prev->PosY0 + getDistY(next->prev->PosY1,next->prev->PosY0) / 2);
		}else{
			px = XCYCL(mch_raceD->cpLst->last()->PosX0 + getDistX(mch_raceD->cpLst->last()->PosX1,mch_raceD->cpLst->last()->PosX0) / 2);
			py = YCYCL(mch_raceD->cpLst->last()->PosY0 + getDistY(mch_raceD->cpLst->last()->PosY1,mch_raceD->cpLst->last()->PosY0) / 2);
		};*/

		ti.set(Owner->stPtr->Checkpoint - 1);
		Target = ti;
		Target.z = fxlabGetLevel(XCYCL(round(Target.x)),YCYCL(round(Target.y)),Owner->R().z);
		ti += 3;
		Direction = ti;
	};
//	Target = Vect3f(px,py,fxlabGetLevel(px,py,Owner->R().z));
};

//------------------------------------

void fxlabServerSkinProtection::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
};

void fxlabServerSkinProtection::Start(void)
{
	fxlabServerEvolutionType::Start();
	Owner->set_feature(Mechos::FIRE_PROTECTION);
};

void fxlabServerSkinProtection::Close(void)
{
	Owner->reset_feature(Mechos::FIRE_PROTECTION);
	fxlabServerEvolutionType::Close();
};

void fxlabServerSkinProtection::Quant(void)
{
	fxlabServerEvolutionType::Quant();
	if(CurrentTime * fxlabGlobalTimeRate >= 200.0f || !Owner->completed() || !Owner->geometry)
		SetAlive(0);
};

//-----------------------------------------

void fxlabServerIsolationProtection::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
};

void fxlabServerIsolationProtection::Start(void)
{
	fxlabServerEvolutionType::Start();
	Owner->set_feature(Mechos::LIGHTNING_PROTECTION);
};

void fxlabServerIsolationProtection::Close(void)
{
	Owner->reset_feature(Mechos::LIGHTNING_PROTECTION);
	fxlabServerEvolutionType::Close();
};

void fxlabServerIsolationProtection::Quant(void)
{
	fxlabServerEvolutionType::Quant();
	if(CurrentTime * fxlabGlobalTimeRate >= 200.0f || !Owner->completed() || !Owner->geometry)
		SetAlive(0);
};

//-------------------------------------------

void fxlabServerArmorProtection::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
};

void fxlabServerArmorProtection::Start(void)
{
	fxlabServerEvolutionType::Start();
	Owner->set_feature(Mechos::NULIFICATOR);
};

void fxlabServerArmorProtection::Close(void)
{
	Owner->reset_feature(Mechos::NULIFICATOR);
	fxlabServerEvolutionType::Close();
};

void fxlabServerArmorProtection::Quant(void)
{
	fxlabServerEvolutionType::Quant();

	if(CurrentTime * fxlabGlobalTimeRate >= 200.0f || !Owner->completed() || !Owner->geometry)
		SetAlive(0);
};

//-------------------------------------------

void fxlabServerBioProtection::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
};

void fxlabServerBioProtection::Start(void)
{
	fxlabServerEvolutionType::Start();
	Owner->set_feature(Mechos::PROTECT_FROM_DAMAGE);
};

void fxlabServerBioProtection::Close(void)
{
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	Owner->reset_feature(Mechos::PROTECT_FROM_DAMAGE);
	fxlabServerEvolutionType::Close();
};

void fxlabServerBioProtection::Quant(void)
{
	fxlabGeneralObjectType* t;

	fxlabServerEvolutionType::Quant();

	if(CurrentTime * fxlabGlobalTimeRate >= 200.0f || !Owner->completed() || !Owner->geometry)
		SetAlive(0);
	else{
		if(SoundPoint.Process)
			SoundPoint.Process->SetPosition(Vect3f(Owner->R()));
		else{
			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetSoundID(EFF_PETARD_IMMORTALITY);
			t->SetPosition(Vect3f(Owner->R()));
			t->SetProcessInterface(&SoundPoint);
			t->Start();
		};
	};
};

//--------------------------------------------

void fxlabServerStaticTeleport::Open(void)
{
	fxlabServerSuckDamage::Open();
	WorldID = 0;
	TrackID = 0;
	TeleportStatus = 0;
	UseFlag = 1;
	OutFlag = 0;
	LastTarget = NULL;
};

void fxlabServerStaticTeleport::Close(void)
{
	if(OmniPoint.Process)
		OmniPoint.Process->SetAlive(0);

	if(ColorPoint.Process)
		ColorPoint.Process->SetAlive(0);

	fxlabServerSuckDamage::Close();
};

void fxlabServerStaticTeleport::Action(void)
{
/*	if(!PrepareTimer() && UseFlag){
	};
	if(!LockTimer())			
		TargetDestruction();*/
};

void fxlabServerStaticTeleport::TargetDestruction(void)
{
//	(static_cast<mchMechosRacer*>(Target))->reset_feature(Mechos::STATIC_NULIFICATOR);
//	UseFlag = 0;
	LastTarget = Target;
	fxlabServerSuckDamage::TargetDestruction();
};

void fxlabServerStaticTeleport::TargetLost(void)
{
//	(static_cast<mchMechosRacer*>(Target))->reset_feature(Mechos::STATIC_NULIFICATOR);
//	UseFlag = 0;
	LastTarget = Target;
	fxlabServerSuckDamage::TargetLost();
};

void fxlabServerStaticTeleport::AssignTarget(struct mchRacer* Target)
{
	fxlabGeneralObjectType* t;

//	(static_cast<mchMechosRacer*>(Target))->set_feature(Mechos::STATIC_NULIFICATOR);

	if(TeleportStatus <= 3){
		if(TeleportStatus){
			if(!(OmniPoint.Process)){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TELEPORT_OMNI_CONTROLLER);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_MENU_TELEPORT_OMNI);
				t->SetPosition(Position);
				t->SetProcessInterface(&OmniPoint);
				t->Start();
			};

			if(!(ColorPoint.Process)){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TELEPORT_COLOR_CONTROLLER);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetKeyID(FXLAB_ID_KEY_MENU_TELEPORT_COLOR_CONTROLLER);
				t->SetMechosPoint(static_cast<mchMechosRacer*>(Target));
				t->SetProcessInterface(&ColorPoint);
				t->Start();
			};
		};

//		PrepareTimer.start(1000);
//		LockTimer.start(2000);
		fxlabServerSuckDamage::AssignTarget(Target);

		iwSelect(Target -> flags & MCH_ACTIVE_RACER2);
		iwHandleEvent(IW_ENTER_TELEPORT_EV,WorldID,TrackID);
		UseFlag = 0;
	};
};

void fxlabServerStaticTeleport::CheckTarget(void)
{
	mchRacer* mp;
	int dx,dy;
	int d,td;
	int x,y;
	int cnt;

/*	if(!TeleportStatus){
		if(Target)
			TargetLost();
		return;
	};*/

	x = round(Position.x);
	y = round(Position.y);

	if(Target){
		if(Target->completed() && Target->Energy() > 0){
			dx = getDistX(round(Target->R().x),x);
			dy = getDistY(round(Target->R().y),y);
			td = dx*dx + dy*dy;
			if(td > ActiveRadius2)
				TargetLost();
		}else
			TargetLost();
	}else{
		cnt = 0;
		mp = mch_raceD->racerLst->first();
		while(mp){
			if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
				dx = getDistX(round(mp->R().x),x);
				dy = getDistY(round(mp->R().y),y);
				d = dx*dx + dy*dy;
				if(d < PassiveRadius2){
					if(!Target || d < td){
						td = d;
						Target = mp;
					};
					cnt++;
				};
			};
			mp = mp->next;
		};

/*		if(!UseFlag){
			if(!cnt){
				UseFlag = 1;
//				DelayTimer.start(0);
				OutFlag = 1;

				if(OmniPoint.Process)
					OmniPoint.Process->SetAlive(0);
				if(ColorPoint.Process)
					ColorPoint.Process->SetAlive(0);
			};
		};

		if(UseFlag){
//			if(!DelayTimer()){
				if(OutFlag){
					OutFlag = 0;
					if(LastTarget){
						iwSelect(LastTarget -> flags & MCH_ACTIVE_RACER2);
						LastTarget = NULL;
					};
					iwHandleEvent(IW_EXIT_TELEPORT_EV,WorldID,TrackID);
				}else{
					if(Target)
						AssignTarget(Target);
				};
//			};
		};*/

		if(UseFlag){
			if(!DelayTimer()){
				if(OutFlag){
					OutFlag = 0;
					if(LastTarget){
						iwSelect(LastTarget -> flags & MCH_ACTIVE_RACER2);
						LastTarget = NULL;
					};
					iwHandleEvent(IW_EXIT_TELEPORT_EV,WorldID,TrackID);
				}else{
					if(Target)
						AssignTarget(Target);
				};
			};
		}else{
			if(!cnt){
				UseFlag = 1;
				DelayTimer.start(0);
				OutFlag = 1;

				if(OmniPoint.Process)
					OmniPoint.Process->SetAlive(0);
				if(ColorPoint.Process)
					ColorPoint.Process->SetAlive(0);
			};
		};
	};

	if(RemotePoint.Process){
		if(Target){
			RemotePoint.Process->SetRemoteID(Target->ID);
		}else{
			RemotePoint.Process->SetRemoteID(-1);
		};
	};
};

//---------------------------------

void fxlabServerVoodooHead::Open(void)
{
	fxlabServerKeyObjectType::Open();
	OwnerPoint = NULL;
	Angle = 0;
	TargetPoint = NULL;
};

void fxlabServerVoodooHead::Start(void)
{	
	fxlabGeneralObjectType* t;
	fxlabServerKeyObjectType::Start();

	DeltaAngle = M_PI / 20.0f;
	Distance = 50.0f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_VOODOO_MASTER_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetAngle(Angle);
	t->SetDeltaAngle(DeltaAngle / fxlabServerTimeRate);
	t->SetRadius(Distance);
	t->SetMechosPoint(OwnerPoint);
	t->SetProcessInterface(&HeadPoint);
	t->Start();
};

void fxlabServerVoodooHead::Close(void)
{
	if(HeadPoint.Process)
		HeadPoint.Process->SetAlive(0);
	fxlabServerKeyObjectType::Close();
};

void fxlabServerVoodooHead::Quant(void)
{
	mchMechosRacer* m;
	mchRacer* mp;
	int dx,dy,dz;
	int d;
	int x,y,z;
	int r2;

	fxlabServerKeyObjectType::Quant();

	if(TargetPoint){
		TargetPoint->damage(Power);
		if(!FireTimer())
			SetAlive(0);
	}else{
		mp = mch_raceD->racerLst->first();
		x = XCYCL(round(Position.x + cosf(Angle) * Distance));
		y = YCYCL(round(Position.y + sinf(Angle) * Distance));
		z = round(Position.z);

		while(mp){
			if(CheckProtection(mp) && mp->completed() && mp->Type == MCH_RACER_TYPE_MECHOS){
				dx = getDistX(round(mp->R().x),x);
				dy = getDistY(round(mp->R().y),y);
				dz = round(mp->R().z) - z;
				d = dx*dx + dy*dy + dz*dz;
				r2 = round(Radius + mp->radius());
				if(d < r2*r2){
					m = static_cast<mchMechosRacer*>(mp);
					FireTimer.start(5000);
					TargetPoint = m;
					fxlabArcaneAction(this,TargetPoint->Mechos::ID);
					if(HeadPoint.Process)
						HeadPoint.Process->SetTarget(m);
					break;
				};
			};
			mp = mp->next;
		};
		FXLAB_TRACK_AI_UPDATE(Position.x,Position.y,Radius,TRACK_EL_FIRE);
		Angle += DeltaAngle;
	};
};

void fxlabServerVoodooHead::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SERVER_VOODOO_HEAD_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabServerVoodooHead");
};

void fxlabServerVoodooHead::KeyUpdate(void)
{
	fxlabServerKeyObjectType::KeyUpdate();
	Power = KeyData[FXLAB_DAMAGE_SENSOR_DATA_POWER] * fxlabServerTimeRate;
	Radius = KeyData[FXLAB_DAMAGE_SENSOR_DATA_RADIUS];
};


//----------------------------

void fxlabServerFrozenType::Open(void)
{
	fxlabServerEvolutionType::Open();
	Owner = NULL;
};

void fxlabServerFrozenType::Start(void)
{
	fxlabGeneralObjectType* t;
	fxlabServerEvolutionType::Start();

	Owner->set_feature(Mechos::FROZEN);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_COLOR_CONTROLLER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_COLOR_CONTROLLER_FROZEN);
	t->SetMechosPoint(Owner);
	t->SetProcessInterface(&ColorPoint);
	t->Start();

	ColorTimer.start(256 * 1000 / 20);
};

void fxlabServerFrozenType::Quant(void)
{
	fxlabServerEvolutionType::Quant();
	if(!ColorTimer())
		SetAlive(0);
	if(!(Owner->completed()))
		SetAlive(0);
//	if(!(ColorPoint.Process))
//		SetAlive(0);
};

void fxlabServerFrozenType::Close(void)
{
	Owner->reset_feature(Mechos::FROZEN);

	if(ColorPoint.Process)
		ColorPoint.Process->SetAlive(0);
	fxlabServerEvolutionType::Close();
};

