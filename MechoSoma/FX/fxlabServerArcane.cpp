#include "StdAfx.h"

#include "aci_parser.h"
#include "TERRA.H"

#include "BodyDispatcher.h"
#include "Mechos.h"
#include "CustomFieldSources.h"
#include "KEYS.H"
#include "ctl_point.h"
#include "race.h"
#include "arcane_menu.h"
#include "sound.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabServerGeneral.h"
#include "fxlabServerStuff.h"
#include "fxlabServerArcane.h"
#include "Mesh3ds.h"

#include "M3d_id.h"
#include "M3d_effects.h"
#include "fxlabBodyInterface.h"

#include "TrackIterator.h"
#include "AnimalArcansPrm.h"


const float FXLAB_REALOBJECT_OFFSET_Z = 20.0f;

MatXf getPlacementPose(const Vect3f& center);
void fxlabGetArcanePosition(Mechos* owner,const Vect3f& offset,Vect3f& pos);

extern int mchCurrentWorld;

void fxlabGeneralArcaneType::Open()
{
	fxlabServerSpaceType::Open();

	LinkPoint = NULL;
	Owner = NULL;
	InterruptNum = -1;
	ActionPoint = -1;
	LifeTime = 1.0f;
};

void fxlabGeneralArcaneType::Start()
{
	int i;
	float time;
	fxlabServerSpaceType::Start();

	if(!LinkPoint)
		ErrH.Abort("UnDefined Link in fxlabArcane");
	if(!Owner)
		ErrH.Abort("UnDefined Owner in fxlabArcane");
	if(InterruptNum == -1)
		ErrH.Abort("UnDefined InterruptNum in fxlabArcane");

//	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD)
//		LinkPoint->Power = LinkPoint->MaxPower;

	ActionTimer.start(0);
	ProcessPoint = &(LinkPoint->fxlabFace);

	RacerID = LinkPoint->ownerID;
	ArcaneID = LinkPoint->ID;
	MechosID = Owner->Mechos::ID;

	PartNumber = -1;
	for(i = 0;i < NUM_PARTS;i++){
		 if(LinkPoint == &(Owner->part_by_index(i)->arcaneData))
			 PartNumber = i;
	};

	if(PartNumber == -1)
		ErrH.Abort("Bad Calc Part's Number in fxlabArcane");

	GetOwnerID() = RacerID;
	GetArcaneID() = PartNumber;
	GetArcaneOwner() = this;
	GetObjectID() = ID;

	ActionStatus = 0;
	LinkPoint->Status = 0;
	
//	time = LinkPoint->DeltaPower / fxlabServerQuantPeriod;
	time = LifeTime / fxlabServerQuantPeriod;
	PowerTime = LinkPoint->Power * time / LinkPoint->MaxPower;
	DeltaPower = LinkPoint->MaxPower / time;

	if(InterruptNum > 1)
		InterruptBorder = time / (float)(InterruptNum - 1);
	else
		InterruptBorder = time + 1.0f;

	InterruptTime = InterruptBorder;

/*	fff < "\n\nPower = " <= LinkPoint->Power;
	fff < "\nMaxPower = " <= LinkPoint->MaxPower;
	fff < "\nDeltaPower = " <= LinkPoint->DeltaPower;
	fff < "\nTime = " <= time;
	fff < "\nPowerTime = " <= PowerTime;
	fff < "\nDecDeltaPower = " <= DeltaPower;*/

	fxlabArcaneStart(static_cast<ArcaneStatisticsInfo*>(this));

	CurrentProtectionTimer.start(ProtectionTime);
};

void fxlabGeneralArcaneType::Close()
{
//	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD)
//		LinkPoint->Power = LinkPoint->MaxPower;

	fxlabArcaneStop(static_cast<ArcaneStatisticsInfo*>(this));
	fxlabServerSpaceType::Close();
};

void fxlabGeneralArcaneType::Quant()
{
	fxlabServerSpaceType::Quant();
	InterruptUse();
	Condition();
	PowerUse();
	if(ActionStatus && !ActionTimer()){
		ActionStatus = 0;
		ActionPoint--;
		if(ActionPoint <= 0)
			ActionZero();
	};
};

void fxlabGeneralArcaneType::InterruptUse()
{
	InterruptTime += 1.0f;
	while(InterruptTime >= InterruptBorder){
		InterruptAction();
		InterruptTime -= InterruptBorder;
	};
};

void fxlabGeneralArcaneType::PowerUse()
{
	PowerTime -= 1.0f;
	LinkPoint->Power -= DeltaPower;

	if(LinkPoint->Power <= 0)
		LinkPowerZero();

	if(PowerTime <= 0)
		PowerZero();
};

void fxlabGeneralArcaneType::ArcaneAction()
{
	if(ActionPoint > 0){
		ActionStatus = 1;
		ActionTimer.start(ActionDelay);
	};
};

void fxlabGeneralArcaneType::ArcaneFatality()
{
	if(ActionPoint > 0){
		ActionPoint--;
		if(ActionPoint <= 0)
			ActionZero();
	};
};

void fxlabGeneralArcaneType::SetArcaneAlive(int arcane_id,int owner_id)
{
	if(ArcaneID == arcane_id && RacerID == owner_id)
		SetAlive(0);
};

void fxlabGeneralArcaneType::LinkPowerZero()
{
	LinkPoint->Power = 0;
};

//------------------------------------------------------

void fxlabInternalArcane::Start()
{
	fxlabGeneralArcaneType::Start();
	Position = Owner->R();
};

void fxlabInternalArcane::Quant()
{
	fxlabGeneralArcaneType::Quant();
	Position = Owner->R();
};

void fxlabInternalArcane::Close()
{
	LinkPoint->Status = 1;
	fxlabGeneralArcaneType::Close();
};

void fxlabInternalArcane::Condition()
{
	fxlabGeneralArcaneType::Condition();
	if(!(Owner->completed()))
		SetAlive(0);
};

void fxlabInternalArcane::LinkPowerZero()
{
	fxlabGeneralArcaneType::LinkPowerZero();
	SetAlive(0); 
};

void fxlabInternalArcane::ActionZero()
{ 	
	fxlabGeneralArcaneType::ActionZero();
	SetAlive(0); 
};

//------------------------------------------------------

void fxlabArcaneDragonWind::Start()
{
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::DRAGON_POWER);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_DRAGON_POWER_ADD);
	t->SetPosition(Position);
	t->Start();
};

void fxlabArcaneDragonWind::Close()
{
	Owner->reset_feature(Mechos::DRAGON_POWER);
	fxlabInternalArcane::Close();
};

void fxlabArcaneDirigibleWind::Start()
{
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::DIRIGIBLE);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_DIRIGIBLE);
	t->SetPosition(Position);
	t->Start();
};

void fxlabArcaneDirigibleWind::Close()
{
	Owner->reset_feature(Mechos::DIRIGIBLE);
	fxlabInternalArcane::Close();
};


void fxlabArcaneButterflyWind::Start()
{
	fxlabGeneralObjectType* t;
	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::BUTTERFLY);
	
	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_WING_BUTTERFLY);
	t->SetPosition(Position);
	t->Start();
};

void fxlabArcaneButterflyWind::Close()
{
	Owner->reset_feature(Mechos::BUTTERFLY);
	fxlabInternalArcane::Close();
};


void fxlabArcaneFrogWind::Start()
{
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::JABASOMA);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_FROG);
	t->SetPosition(Position);
	t->Start();
};

void fxlabArcaneFrogWind::Close()
{
	Owner->reset_feature(Mechos::JABASOMA);
	fxlabInternalArcane::Close();
};


void fxlabArcaneHelicopterWind::Start()
{
	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::HELICOPTER);
};

void fxlabArcaneHelicopterWind::Close()
{
	Owner->reset_feature(Mechos::HELICOPTER);
	fxlabInternalArcane::Close();
};

//-----------------------

void fxlabRevoltSpace::Start()
{
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::MAGNETIC_CUSHION);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_REVOLT_SPACE_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_REVOLT_SPACE_LINK);
	t->SetPosition(Position);
	t->SetBody(Owner);
	t->SetProcessInterface(&ClientPoint);
	t->Start();
};

void fxlabRevoltSpace::Close()
{
	Owner->reset_feature(Mechos::MAGNETIC_CUSHION);
	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);
	fxlabInternalArcane::Close();
};

//-----------------------------------

void fxlabArcanePlasmaClaw::Start()
{
	fxlabGeneralObjectType* ct;
	fxlabGeneralObjectType* st;

	fxlabInternalArcane::Start();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_WILD_CLAW_FRONT_LINK);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetKeyID(FXLAB_ID_KEY_WILD_CLAW);
		ct->SetPosition(Position);
		ct->SetBody(Owner);
		ct->Start();

		st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_PLASMA_CLAW);
		st->SetStartTime(fxlabServerD->GetTime());
		st->SetKeyID(FXLAB_ID_KEY_PLASMA_CLAW_CONTROL);
		st->SetPosition(Position);
		st->SetRemoteObject(ct);
		st->SetMechosPoint(Owner);
		st->SetProtection(ProtectionID,ProtectionTime);
		st->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		st->SetProcessInterface(&ServerPoint);
		st->Start();

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetSoundID(EFF_TENTACLE);
		ct->SetPosition(Position);
		ct->Start();
	};
};

void fxlabArcanePlasmaClaw::Quant()
{
	fxlabGeneralArcaneType::Quant();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
		if(ServerPoint.Process)
			ServerPoint.Process->SetPosition(Position);
	};
};

void fxlabArcanePlasmaClaw::Close()
{
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);

	fxlabInternalArcane::Close();
};

//-----------------------------------

void fxlabArcaneBowMachine::Start()
{
	fxlabGeneralObjectType* ct;
	fxlabGeneralObjectType* st;

	fxlabInternalArcane::Start();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BOW_MACHINE_FRONT_LINK);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetKeyID(FXLAB_ID_KEY_BOW_MACHINE);
		ct->SetPosition(Position);
		ct->SetBody(Owner);
		ct->Start();

		st = fxlabServerD->CreateObject(/*FXLAB_SERVER_PROCESS_BOW_MACHINE_DAMAGE*/FXLAB_SERVER_PROCESS_SUCK_DAMAGE);
		st->SetStartTime(fxlabServerD->GetTime());
		st->SetKeyID(FXLAB_ID_KEY_BOW_MACHINE_DAMAGE);
		st->SetPosition(Position);
		st->SetRemoteObject(ct);
		st->SetProtection(ProtectionID,ProtectionTime);
		st->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		st->SetProcessInterface(&ServerPoint);
		st->Start();
	};
};

void fxlabArcaneBowMachine::Quant()
{
	fxlabGeneralArcaneType::Quant();

	if(ServerPoint.Process){
		if(Owner->completed()){
			Position = Owner->part_coords(M3D_FRONT);
			ServerPoint.Process->SetPosition(Position);
		};
	}else SetAlive(0);
};

void fxlabArcaneBowMachine::Close()
{
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);

	fxlabInternalArcane::Close();
};

//---------------------------------------------

void fxlabArcaneShaftMachine::Start()
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	fxlabInternalArcane::Start();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
//		v = getDist(Position,Vect3f(Owner->R()));
//		v.normalize();

		v = Vect3f(0,1.0f,0.15f) * Owner->Alg();
		v.Normalize();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SHAFT_MECHINE_FRONT_LINK);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_SHAFT_MACHINE_LINK);
		t->SetPosition(Position);
		t->SetBody(Owner);
		t->SetProcessInterface(&ClientPoint);
		t->Start();

		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_LINE);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_SHAFT_MACHINE_DAMAGE);
		t->SetPosition(Position);
		t->SetVelocity(v);
		t->SetProtection(ProtectionID,ProtectionTime);
		t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		t->SetProcessInterface(&ServerPoint);
		t->Start();
	};
};

void fxlabArcaneShaftMachine::Quant()
{
	Vect3f v;
	fxlabGeneralArcaneType::Quant();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
		v = Vect3f(0,1.0f,0.15f) * Owner->Alg();
		v.Normalize();
		ServerPoint.Process->SetPosition(Position);
		ServerPoint.Process->SetVelocity(v);
	};
};

void fxlabArcaneShaftMachine::Close()
{
	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);

	fxlabInternalArcane::Close();
};

//------------------------------------------

/*void fxlabArcaneVoodooMaster::Start()
{
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FRONT_LINK);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_VOODOO_MASTER_LINK);
		t->SetPosition(Position);
		t->SetBody(Owner);
		t->SetProcessInterface(&ClientPoint);
		t->Start();

		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_CIRCLE);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_VOODOO_MASTER_DAMAGE);
		t->SetPosition(Position);
		t->SetProtection(ProtectionID,ProtectionTime);
		t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		t->SetProcessInterface(&ServerPoint);
		t->Start();
	};
};

void fxlabArcaneVoodooMaster::Quant()
{
	fxlabGeneralArcaneType::Quant();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
		ServerPoint.Process->SetPosition(Position);
	};
};

void fxlabArcaneVoodooMaster::Close()
{
	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);

	fxlabInternalArcane::Close();
};*/


void fxlabArcaneVoodooMaster::Start()
{
	int i;
	float a,da;
	fxlabGeneralObjectType* t;

	fxlabInternalArcane::Start();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
		a = M_PI * fxlabServerRND.UnitRND();
		da = 2.0f * M_PI / FXLAB_VOOODOO_MASTER_OBJECT_NUM;
		for(i = 0;i < FXLAB_VOOODOO_MASTER_OBJECT_NUM;i++){
			t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_SERVER_VOODOO_HEAD);
			t->SetStartTime(fxlabServerD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_VOODOO_MASTER_DAMAGE);
			t->SetPosition(Position);
			t->SetAngle(a);
			t->SetMechosPoint(Owner);
			t->SetProtection(ProtectionID,ProtectionTime);
			t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
			t->SetProcessInterface(&(ServerPoint[i]));
			t->Start();
			a += da;
		};
	};
};

void fxlabArcaneVoodooMaster::Quant()
{
	int i,cnt;
	fxlabGeneralArcaneType::Quant();

	if(Owner->completed()){
		Position = Owner->part_coords(M3D_FRONT);
		cnt = 0;
		for(i = 0;i < FXLAB_VOOODOO_MASTER_OBJECT_NUM;i++){
			if(ServerPoint[i].Process){
				ServerPoint[i].Process->SetPosition(Position);
				cnt++;
			};
		};
		if(!cnt)
			SetAlive(0);
	};
};

void fxlabArcaneVoodooMaster::Close()
{
	int i;
	for(i = 0;i < FXLAB_VOOODOO_MASTER_OBJECT_NUM;i++){
		if(ServerPoint[i].Process)
			ServerPoint[i].Process->SetAlive(0);
	};
	fxlabInternalArcane::Close();
};

//-----------------------------------------

void fxlabArcaneIronSide::Start()
{
	fxlabInternalArcane::Start();
	Owner->set_feature(Mechos::IRON_HIP);
};

void fxlabArcaneIronSide::Close()
{
	Owner->reset_feature(Mechos::IRON_HIP);
	fxlabInternalArcane::Close();
};

//--------------------------------------------

void fxlabArcaneBallWind::Start()
{
	fxlabGeneralObjectType* t;
	fxlabInternalArcane::Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LEFT_BALL_WIND_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BALL_WIND_LEFT_LINK);
	t->SetPosition(Position);
	t->SetBody(Owner);
	t->SetProcessInterface(&LeftPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_RIGHT_BALL_WIND_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_BALL_WIND_RIGHT_LINK);
	t->SetPosition(Position);
	t->SetBody(Owner);
	t->SetProcessInterface(&RightPoint);
	t->Start();

	Owner->cannon_ball_flight();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_BULLET_FLIGHT);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_BULLET_FLIGHT_ADD);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabArcaneBallWind::Quant()
{
	fxlabInternalArcane::Quant();

	if(SoundPoint.Process)
		SoundPoint.Process->SetPosition(Position);
};

void fxlabArcaneBallWind::Close()
{
	if(LeftPoint.Process)
		LeftPoint.Process->SetAlive(0);
	if(RightPoint.Process)
		RightPoint.Process->SetAlive(0);
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabInternalArcane::Close();
};

//----------------------

void fxlabImpulseArcane::Quant()
{
	fxlabGeneralArcaneType::Quant();
	SetAlive(0);
};

void fxlabImpulseArcane::Close()
{
	LinkPoint->Status = 1;
	fxlabGeneralArcaneType::Close();
};

//-----------------

void fxlabArcaneFormicaryWind::Start()
{
	fxlabImpulseArcane::Start();

	Vect3f p0(0), p1(0),v;
	if(mchCurrentWorld == FXLAB_ARCANE_DEMO_WORLD){
		v = Vect3f(0,Owner->radius() * 10,0) * Mat3f(Owner->psi(),Z_AXIS);
		p0.x = XCYCL(round(v.x + Owner->R().x));
		p0.y = YCYCL(round(v.y + Owner->R().y));

		p1 = v;
		p1 *= 2.0f;
		p1 += Owner->R();
		CYCLE(p1);
	}else{
		mchCheckpoint* cp = mch_raceD -> getCP(Owner->stPtr->Checkpoint);
		if(cp){
			p0.x = XCYCL(cp->PosX0 + getDistX(cp->PosX1,cp->PosX0) / 2);
			p0.y = YCYCL(cp->PosY0 + getDistY(cp->PosY1,cp->PosY0) / 2);
		}else
			p0 = Owner -> R();

		TrackIterator ti;
		ti.set(Owner->stPtr->Checkpoint);
		p1 = ti += 3;
	};

	v = getDist(p0,p1);
	v.Normalize(Owner->radius() * 1.5f);
	p0 += v;
	CYCLE(p0);

	Owner->formicTransport(p0, p1);
	if(Owner->flags & MCH_AI){
		Owner->RemoveSeeds();
		Owner->AddSeed(p0);
	};
};

//-----------------------

void fxlabArcaneSuicideWarrior::Start()
{
	Body *p;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;
	Vect3f v;
	float d;

	fxlabImpulseArcane::Start();

	Position = Owner->R();
	v = Owner->Yglobal();
	d = Owner->length() * 1.5f;
	Position.x = XCYCL(round(Position.x + v.x * d));
	Position.y = YCYCL(round(Position.y + v.y * d));
	Position.z = Position.z + v.z * d + FXLAB_REALOBJECT_OFFSET_Z;

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp->createBody("Suicide",Position,Owner->psi() * 180.0f / M_PI);
	
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SUICIDE_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
//!!!!!
//	t->SetKeyID(FXLAB_ID_KEY_SUICIDE_LINK);
	t->SetPosition(Position);
	t->SetBody(p);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetKeyID(FXLAB_ID_KEY_CLOWN);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->Start();
};


void fxlabArcaneClownWarrior::Start()
{
	Vect3f v;
	float d;
	Body *p;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;

	fxlabImpulseArcane::Start();

	Position = Owner->R();
	v = Owner->Yglobal();
	d = Owner->length() * 1.5f;
	Position.x = XCYCL(round(Position.x + v.x * d));
	Position.y = YCYCL(round(Position.y + v.y * d));
	Position.z = Position.z + v.z * d + FXLAB_REALOBJECT_OFFSET_Z;

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp->createBody("Clown",Position,Owner->psi() * 180.0f / M_PI);
	
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_CLOWN);
	t->SetPosition(Position);
	t->Start();
};


void fxlabArcaneEyeWarrior::Start()
{
	Vect3f v;
	float d;
	Body *p;
	OwnerProtection* op;

	fxlabImpulseArcane::Start();

	Position = Owner->R();
	v = Owner->Yglobal();
	d = Owner->length() * 0.5f;
	Position.x = XCYCL(round(Position.x - v.x * d));
	Position.y = YCYCL(round(Position.y - v.y * d));
	Position.z = Position.z - v.z * d + FXLAB_REALOBJECT_OFFSET_Z;

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp->createBody("EvilEye",Position,Owner->psi() * 180.0f / M_PI + 180.0f);
	
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);
};

void fxlabArcaneTomatoWarrior::Start()
{
	Body *p;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;

	fxlabImpulseArcane::Start();

	Position = Owner->R();
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp->createBody("Tomato",Position,Owner->psi() * 180.0f / M_PI + 180.0f);
	
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_TOMATO);
	t->SetPosition(Position);
	t->Start();
};

//--------------------------

void fxlabArcaneKineticHit::Start()
{
	fxlabGeneralObjectType* t;

	fxlabImpulseArcane::Start();
	Position = Owner->R();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_KINETIC_HIT_WAVE);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TERRAIN_DEFORMATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_KINETIC_HIT_DEFORMATION);
	t->SetPosition(Position);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_KINETIC_HIT_DAMAGE);
	t->SetPosition(Position);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_KINETIC_HIT);
	t->SetPosition(Position);
	t->Start();
};

//--------------------------------

void fxlabArcaneFireBall::InterruptAction()
{
	Body *n;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;
	Vect3f v;
	float d;

	Position = Owner->R();
	v = Owner->Yglobal();
	d = Owner->length() * 0.5f;
	Position.x = XCYCL(round(Position.x + v.x * d));
	Position.y = YCYCL(round(Position.y + v.y * d));
	Position.z = Position.z + v.z * d + 12.0f;

	ArcaneStatisticsService::setCurrentArcane(*this);
	n = Mdisp->createBody("SmartFireBall",Position,Owner->psi() * 180.0f / M_PI);
	
	op = dynamic_cast<OwnerProtection*>(n);
	if(op) 
		op->set_owner(ProtectionID,ProtectionTime);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FIRE_BALL_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
//!!!!!
//	t->SetKeyID(FXLAB_ID_KEY_FIREBALL_LINK);
	t->SetPosition(Position);
	t->SetBody(n);
	t->Start();

/*	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BODY_LINK);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SERVER_FIREBALL_LINK);
	t->SetPosition(Position);
	t->SetBody(n);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_FIREBALLS);
	t->SetPosition(Position);
	t->Start();
};

//--------------------

void fxlabArcaneLightningBolt::InterruptAction()
{
	Body *n;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;
	Vect3f v;
	float d;

	Position = Owner->R();
	v = Owner->Yglobal();
	d = Owner->length() * 0.5f;
	Position.x = XCYCL(round(Position.x + v.x * d));
	Position.y = YCYCL(round(Position.y + v.y * d));
	Position.z = Position.z + v.z * d + 12.0f;

	ArcaneStatisticsService::setCurrentArcane(*this);
	n = Mdisp->createBody("SmartLightningBolt",Position,Owner->psi() * 180.0f / M_PI);
	
	op = dynamic_cast<OwnerProtection*>(n);
	if(op)
		op->set_owner(ProtectionID,ProtectionTime);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_BOLT_LINK);
	t->SetStartTime(fxlabClientD->GetTime());
//!!!!!
//	t->SetKeyID(FXLAB_ID_KEY_LIGHTNING_BOLT_LINK);
	t->SetPosition(Position);
	t->SetBody(n);
	t->Start();

/*	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_BODY_LINK);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SERVER_LIGHTNING_BOLT_LINK);
	t->SetPosition(Position);
	t->SetBody(n);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();*/

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_LIGHT_BOLT);
	t->SetPosition(Position);
	t->Start();
};

//--------------------

/*void fxlabArcaneJumpBall::InterruptAction()
{
	fxlabGeneralObjectType* t;
	if(Owner->completed()){
		ArcaneStatisticsService::setCurrentArcane(*this);
		Owner->throwTitaniumBall(titanium_ball_damage);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_TITANIUM_BALLS);
		t->SetPosition(Position);
		t->Start();
	};
};*/

void fxlabArcaneJumpBall::Start()
{
	fxlabGeneralObjectType* t;

	fxlabImpulseArcane::Start();

	if(Owner->completed()){
		Position = Owner->R();

		ArcaneStatisticsService::setCurrentArcane(*this);
		Owner->throwTitaniumBall(titanium_ball_damage);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_TITANIUM_BALLS);
		t->SetPosition(Position);
		t->Start();
	};
};

//----------------------

void fxlabArcaneSpiderSpawn::InterruptAction()
{
	Body* b;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;

	if(Owner->completed()){		
		ArcaneStatisticsService::setCurrentArcane(*this);
		b = Mdisp->createBody("LittleSpider",Owner->part_coords(M3D_LB_WHEEL) + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z),random(360));
		op = dynamic_cast<OwnerProtection*>(b);
		if(op) op->set_owner(ProtectionID,ProtectionTime);

		ArcaneStatisticsService::setCurrentArcane(*this);
		b = Mdisp->createBody("LittleSpider",Owner->part_coords(M3D_RB_WHEEL) + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z),random(360));
		op = dynamic_cast<OwnerProtection*>(b);
		if(op) op->set_owner(ProtectionID,ProtectionTime);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_SPIDERS_ATTACK);
		t->SetPosition(Position);
		t->Start();
	};
};

//---------------------

void fxlabArcaneCaterpillarSpawn::InterruptAction()
{
	Body* b;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;

	if(Owner->completed()){		
		ArcaneStatisticsService::setCurrentArcane(*this);
		b = Mdisp->createBody("Caterpillar",Owner->part_coords(M3D_LB_WHEEL) + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z),random(360));
		op = dynamic_cast<OwnerProtection*>(b);
		if(op) op->set_owner(ProtectionID,ProtectionTime);

		ArcaneStatisticsService::setCurrentArcane(*this);
		b = Mdisp->createBody("Caterpillar",Owner->part_coords(M3D_RB_WHEEL) + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z),random(360));
		op = dynamic_cast<OwnerProtection*>(b);
		if(op) op->set_owner(ProtectionID,ProtectionTime);

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetSoundID(EFF_CATERPILLAR);
		t->SetPosition(Position);
		t->Start();
	};
};

//-------------------------------------

void fxlabArcaneMassShifter::Start()
{
	fxlabGeneralObjectType* t;

	fxlabImpulseArcane::Start();
	Position = Owner->R();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_MASS_SHIFTER);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetPosition(Position);
	t->SetMechosPoint(Owner);
	t->Start();
};

//------------------------------

void fxlabExternalPowerArcane::Start()
{
	fxlabGeneralArcaneType::Start();
	LinkPoint->Power = 0;
	LinkPoint->Status = 1;
};

void fxlabExternalPowerArcane::PowerUse()
{
	PowerTime -= 1.0f;
	if(PowerTime <= 0)
		PowerZero();
};

void fxlabExternalPowerArcane::LinkPowerZero()
{
};

void fxlabExternalPowerArcane::ActionZero()
{
	SetAlive(0);
};

void fxlabExternalPowerArcane::PowerZero()
{
	SetAlive(0);
};

//------------------------------------------

void fxlabArcaneFireStone::Start()
{
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Velocity = Owner->Yglobal() * 2.0f;
	Velocity += Owner->Zglobal();
	Velocity.Normalize(40.0f);
//	Velocity += Owner->Yglobal() * Owner->actualSpeed() / 20.0f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BULLET_FIRE_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_FIRE_BULLET_MODEL);
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&BulletPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_STAR_SHOWER);
	t->SetPosition(Position);
	t->Start();

	BulletTimer.start(400);
};

void fxlabArcaneFireStone::Close()
{
	if(CloudPoint.Process)
		CloudPoint.Process->SetAlive(0);

	if(BulletPoint.Process)
		BulletPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

/*void fxlabArcaneFireStone::InterruptAction()
{
//!!!!
	if(!BulletTimer()){
		fxlabCreateSplinter<fxlabFireBolide>(5,5,Position,7,fxlabServerRND.UnitRND()*M_PI,M_PI*0.5f,0,50.0f * fxlabServerRND.UnitRND(),0,
			FXLAB_CLIENT_PROCESS_BOLIDE_LINK,
			static_cast<ArcaneStatisticsInfo*>(this),static_cast<fxlabProtectionType*>(this),NULL);
	};
};*/

const float FXLAB_FIRE_STONE_RADIUS = 70.0f;

void fxlabArcaneFireStone::Quant()
{
	float a,r;
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Quant();

	if(!BulletTimer()){
		if(BulletPoint.Process){
			BulletPoint.Process->SetAlive(0);

			Position += Velocity * (CurrentTime * fxlabGlobalTimeRate);
			Position = CYCLE(Position);

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TWISTER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_FIRE_CLOUDS);
			t->SetPosition(Position);
			t->SetProcessInterface(&CloudPoint);
			t->Start();
		};

		a = fxlabServerRND.UnitRND() * M_PI * 2.0f;
		r = fxlabServerRND.UnitRND() * FXLAB_FIRE_STONE_RADIUS;
		//!!!!!!
		if(!fxlabServerRND.RND(2)){
			fxlabCreateBolide<fxlabFireBolide>(FXLAB_MODEL_INTERFACE_ID_BOLIDE_START,FXLAB_MODEL_INTERFACE_ID_BOLIDE_NOISE,Vect3f(XCYCL(round(Position.x + cosf(a) * r)),YCYCL(round(Position.y + sinf(a) * r)),Position.z),Vect3f(0,0,0),25.0f,
				FXLAB_CLIENT_PROCESS_BOLIDE_LINK,
				static_cast<ArcaneStatisticsInfo*>(this),static_cast<fxlabProtectionType*>(this),NULL);
		};
	};
};

//------------------------------------------

const float FXLAB_SNOW_FALL_RADIUS = 70.0f;

void fxlabArcaneSnowFall::Start()
{
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Velocity = Owner->Yglobal() * 1.8f;
	Velocity += Owner->Zglobal();
	Velocity.Normalize(40.0f);
//	Velocity += Owner->Yglobal() * Owner->actualSpeed() / 20.0f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BULLET_SNOW_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SNOW_BULLET_MODEL);
	t->SetPosition(Position);
	t->SetVelocity(Velocity);
	t->SetProcessInterface(&BulletPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_SNOW_FALL);
	t->SetPosition(Position);
	t->Start();

	BulletTimer.start(400);
};

void fxlabArcaneSnowFall::Close()
{
	if(CloudPoint.Process)
		CloudPoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	if(BulletPoint.Process)
		BulletPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

void fxlabArcaneSnowFall::Quant()
{
	float a,r;
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Quant();

	if(!BulletTimer()){
		if(BulletPoint.Process){
			BulletPoint.Process->SetAlive(0);

			Position += Velocity * (CurrentTime * fxlabGlobalTimeRate);
			Position = CYCLE(Position);

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TWISTER);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_SNOW_CLOUDS);
			t->SetPosition(Position);
			t->SetProcessInterface(&CloudPoint);
			t->Start();

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetSoundID(EFF_SNOW_FALL_ADD);
			t->SetPosition(Position);
			t->SetProcessInterface(&SoundPoint);
			t->Start();
		};

//		if(!fxlabServerRND.RND(2)){
			a = fxlabServerRND.UnitRND() * M_PI * 2.0f;
			r = fxlabServerRND.UnitRND() * FXLAB_SNOW_FALL_RADIUS;
		//!!!!!!
			fxlabCreateBolide<fxlabSnowBall>(FXLAB_MODEL_INTERFACE_ID_SNOW_START,FXLAB_MODEL_INTERFACE_ID_SNOW_NOISE,Vect3f(XCYCL(round(Position.x + cosf(a) * r)),YCYCL(round(Position.y + sinf(a) * r)),Position.z),Vect3f(0,0,0),25.0f,
				FXLAB_CLIENT_PROCESS_SNOW_FALL_LINK,
				static_cast<ArcaneStatisticsInfo*>(this),static_cast<fxlabProtectionType*>(this),NULL);
//		};
	};
};

//--------------------------

void fxlabArcaneElecricStorm::Start()
{
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();
	Position = Owner->R();

	Position.z = fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Position.z + Owner->radius());

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_ELECTRIC_STORM_MARKER_OMNI);
	t->SetPosition(Position);
	t->SetProcessInterface(&MarkerPoint);
	t->Start();

	Position.z += 150.0f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_TWISTER);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_STORM_CLOUDS);
	t->SetPosition(Position);
	t->SetProcessInterface(&CloudPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_THUNDERSTORM_ADD);
	t->SetPosition(Position);
	t->Start();	
};

void fxlabArcaneElecricStorm::Close()
{
	if(CloudPoint.Process)
		CloudPoint.Process->SetAlive(0);
	if(MarkerPoint.Process)
		MarkerPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

void fxlabArcaneElecricStorm::InterruptAction()
{
	fxlabGeneralObjectType* t;
	Vect3f v;

	v = Position;
	v.z -= 150.0f;

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_LINE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_ELECTRIC_STORM_BOW);
	t->SetPosition(v);
	t->SetVelocity(Vect3f(0,0,1.0f));
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_ELECTRIC_STORM_LIGHT);
	t->SetPosition(v);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_ELECTRIC_STORM_DAMAGE);
	t->SetPosition(v);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_THUNDERSTORM);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_FEAR_STREAM);
	t->SetKeyID(FXLAB_ID_KEY_STORM_SPARK);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Vect3f(Position.x,Position.y,Position.z - 150.0f));
	t->SetVelocity(Vect3f(0,0,0));
	t->Start();

};


//------------------------------------------

const float FXLAB_FIRE_GARDEN_ROSE_LEN = 200.0f;
const float FXLAB_FIRE_GARDEN_POSITION = 0;

void fxlabArcaneFireGarden::Start()
{
	int i;
	Vect3f v,dv;
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Position.z = fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Position.z + Owner->radius());
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	v = Owner->Yglobal() * FXLAB_FIRE_GARDEN_POSITION;
	v += Owner->Xglobal() * FXLAB_FIRE_GARDEN_ROSE_LEN * 0.5f;
	dv = Owner->Xglobal() * FXLAB_FIRE_GARDEN_ROSE_LEN / (float)(-FXLAB_FIRE_GARDEN_ROSE_NUM);
	v += Position;

	for(i = 0;i < FXLAB_FIRE_GARDEN_ROSE_NUM;i++){
		CYCLE(v);
		v += dv;
		v.z = fxlabGetLevel(v.x,v.y,v.z + Owner->radius());
	
		t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_DAMAGE_SENSOR);
		t->SetStartTime(fxlabServerD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_FIRE_GARDEN_DAMAGE);
		t->SetPosition(v);
		t->SetProtection(ProtectionID,ProtectionTime);
		t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
		t->SetProcessInterface(&ServerPoint[i]);
		t->Start();

		t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_OBJECT_SET);
		t->SetStartTime(fxlabClientD->GetTime());
		t->SetKeyID(FXLAB_ID_KEY_ROSE_FIRE_SET);
		t->SetPosition(v);
		t->SetProcessInterface(&ClientPoint[i]);
		t->Start();
	};

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_LURID_BILLOW);
	t->SetPosition(Position);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_LURID_BILLOW_ADD);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabArcaneFireGarden::Close()
{
	int i;
	for(i = 0;i < FXLAB_FIRE_GARDEN_ROSE_NUM;i++){	
		if(ClientPoint[i].Process)
			ClientPoint[i].Process->SetAlive(0);
		if(ServerPoint[i].Process)
			ServerPoint[i].Process->SetAlive(0);
	};

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

//-----------------------------------------

	
void fxlabArcaneRageSlime::Start()
{
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Position.z = fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Owner->R().z + Owner->radius());
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SLIME_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_RAGE_SLIME_MODEL);
	t->SetPosition(Position);
	t->SetMatrix(getPlacementPose(Position));
	t->SetProcessInterface(&ClientPoint);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_RAGE_SLIME_CIRCLE);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_RAGE_SLIME_CIRCLE);
	t->SetPosition(Position);
	t->SetProcessInterface(&ServerPoint);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetPosition(Position);
	t->SetSoundID(EFF_SLIME_FURY);
	t->SetProcessInterface(&SoundPoint);
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_SLIME_FURY_ADD);
	t->SetPosition(Position);
	t->Start();

	WaitTimer.start(1500);
};


void fxlabArcaneRageSlime::Close()
{
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);

	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

void fxlabArcaneRageSlime::Quant()
{
	fxlabExternalPowerArcane::Quant();
	if(!WaitTimer && (!fxlabGetWorldReflectionEnable()) && Position.z < vMap->LevelH2O && (GetAt(XCYCL(round(Position.x)),YCYCL(round(Position.y))) & At_WATER))
		SetAlive(0);
};

//-------------------------------------------

const float FXLAB_FURY_FUNGUS_RADIUS = 35.0f;

void fxlabArcaneFuryFungus::Open()
{
	fxlabExternalPowerArcane::Open();
	CheckLife = 0;
};

void fxlabArcaneFuryFungus::Start()
{
	int i;
	float a,r;
	float da;
	Vect3f v;
	fxlabGeneralObjectType* t;

	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Position.z += Owner->radius();
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	a = fxlabClientRND.UnitRND() * M_PI * 2.0f;
	da = M_PI * 2.0f / 7.0f;
	r = FXLAB_FURY_FUNGUS_RADIUS;

	for(i = 0;i < FXLAB_FURY_FUNGUS_NUM;i++){
		if(i < 7){
			v = Position;
			v += Vect3f(r * cosf(a),r * sinf(a),0);
			CYCLE(v);
			v.z = fxlabGetLevel(v.x,v.y,v.z + Owner->radius()) + 10.0f;
			a += da;

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LASH_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_FURY_FUNGUS_LASH);
			t->SetPosition(v);
			t->SetProcessInterface(&LashPoint[i]);
			t->Start();
		}else{
			v = Position;
			a = fxlabClientRND.UnitRND() * M_PI * 2.0f;
			r = FXLAB_FURY_FUNGUS_RADIUS * fxlabClientRND.UnitRND();
			v += Vect3f(r * cosf(a),r * sinf(a),0);
			CYCLE(v);
			v.z = fxlabGetLevel(v.x,v.y,v.z + Owner->radius()) + 10.0f;

			t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LASH_MODEL);
			t->SetStartTime(fxlabClientD->GetTime());
			t->SetKeyID(FXLAB_ID_KEY_FURY_FUNGUS_LASH);
			t->SetPosition(v);
			t->SetProcessInterface(&LashPoint[i]);
			t->Start();
		};
	};

	v = Position;
	CYCLE(v);
	v.z = fxlabGetLevel(v.x,v.y,v.z + Owner->radius());

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_FURY_FUNGUS_FOG);
	t->SetPosition(v);
	t->SetProcessInterface(&FogPoint);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_WILD_THICKET_CIRCLE);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_FURY_FUNGUS_CIRCLE);
	t->SetPosition(v);
	t->SetProcessInterface(&CirclePoint);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_WILD_FUNGUS);
	t->SetPosition(v);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabArcaneFuryFungus::Close()
{
	int i;
	for(i = 0;i < FXLAB_FURY_FUNGUS_NUM;i++){
		if(LashPoint[i].Process)
			LashPoint[i].Process->SetAlive(0);
	};

	if(FogPoint.Process)
		FogPoint.Process->SetAlive(0);

	if(CirclePoint.Process)
		CirclePoint.Process->SetAlive(0);

	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);

	fxlabExternalPowerArcane::Close();
};

void fxlabArcaneFuryFungus::ArcaneAction()
{
	int i;
	if(!CheckLife){
		for(i = 0;i < FXLAB_FURY_FUNGUS_NUM;i++){
			if(LashPoint[i].Process)
				LashPoint[i].Process->SetScaleAnimation(1.0f,8.0f,2000);
		};

		LifeTimer.start(4000);
		CheckLife = 1;
	};
};

void fxlabArcaneFuryFungus::Quant()
{
//	int i;
	fxlabExternalPowerArcane::Quant();

	if(CheckLife && !LifeTimer())
		SetAlive(0);

	if((!fxlabGetWorldReflectionEnable()) && Position.z < vMap->LevelH2O && (GetAt(XCYCL(round(Position.x)),YCYCL(round(Position.y))) & At_WATER))
		SetAlive(0);

/*	for(i = 0;i < FXLAB_FURY_FUNGUS_NUM;i++){
		if(LashPoint[i].Process)
			break;
	};
	if(i >= FXLAB_FURY_FUNGUS_NUM)
		SetAlive(0);*/
};

//-----------------------------------------

void fxlabArcaneShiftTrap::Start()
{
	fxlabGeneralObjectType* t;
	fxlabExternalPowerArcane::Start();

	Position = Owner->R();
	Position.z = fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Owner->R().z + Owner->radius());
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_GROUND_MODEL);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetKeyID(FXLAB_ID_KEY_SHIFT_TRAP_MODEL);
	t->SetPosition(Position);
	MatXf pose = getPlacementPose(Position);
	t->SetMatrix(pose);
	Position = pose.trans();
	t->SetProcessInterface(&ClientPoint);
	t->Start();

	t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_TRAP_SPHERE);
	t->SetStartTime(fxlabServerD->GetTime());
	t->SetKeyID(FLXAB_ID_KEY_SHIFT_TRAP_SPHERE);
	t->SetPosition(Position);
	t->SetProcessInterface(&ServerPoint);
	t->SetProtection(ProtectionID,ProtectionTime);
	t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	t->Start();

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_LONG);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_TELEPORT);
	t->SetPosition(Position);
	t->SetProcessInterface(&SoundPoint);
	t->Start();
};

void fxlabArcaneShiftTrap::Close()
{
	if(ServerPoint.Process)
		ServerPoint.Process->SetAlive(0);
	if(ClientPoint.Process)
		ClientPoint.Process->SetAlive(0);
	if(SoundPoint.Process)
		SoundPoint.Process->SetAlive(0);
	fxlabExternalPowerArcane::Close();
};

//------------------------------------

void fxlabExternalMindArcane::Open()
{
	fxlabGeneralArcaneType::Open();
	InterruptCount = 0;
	InterruptEnable = 1;
	ExternalTime = 1;
};

void fxlabExternalMindArcane::Start()
{
	fxlabGeneralArcaneType::Start();
	PowerTime = ExternalTime / fxlabServerQuantPeriod;
};

void fxlabExternalMindArcane::Condition()
{
	if(!Owner->completed()){
		if(InterruptEnable){
			LinkPoint->Status = 1;
			InterruptEnable = 0;
		};
	};
};

void fxlabExternalMindArcane::InterruptUse()
{
	if(InterruptEnable){
		InterruptTime += 1.0f;
		while(InterruptTime >= InterruptBorder){
			InterruptAction();
			InterruptCount++;
			if(InterruptCount >= InterruptNum){
				LinkPoint->Status = 1;
				InterruptEnable = 0;
				break;
			};
			InterruptTime -= InterruptBorder;
		};
	};
};

void fxlabExternalMindArcane::PowerUse()
{
	if(InterruptEnable){
		LinkPoint->Power -= DeltaPower;
		if(LinkPoint->Power <= 0)
			LinkPowerZero();
	};

	PowerTime -= 1.0f;
	if(PowerTime <= 0)
		PowerZero();
};


void fxlabExternalMindArcane::LinkPowerZero()
{
	if(InterruptEnable){
		LinkPoint->Power = 0;
		LinkPoint->Status = 1;
		InterruptEnable = 0;
	};
};

void fxlabExternalMindArcane::ActionZero()
{
	SetAlive(0);
};

void fxlabExternalMindArcane::PowerZero()
{
	SetAlive(0);
};

void fxlabExternalMindArcane::Close()
{
	if(InterruptEnable){
		LinkPoint->Status = 1;
		InterruptEnable = 0;
	};
	fxlabGeneralArcaneType::Close();
};

//-----------------------------------------------------

void fxlabArcaneLightningSeed::Start()
{
	fxlabExternalMindArcane::Start();
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	ServerPoint = new fxlabProcessInterface[InterruptNum];
};

void fxlabArcaneLightningSeed::Close()
{
	int i;
	for(i = 0;i < InterruptNum;i++){
		if(ServerPoint[i].Process)
			ServerPoint[i].Process->SetAlive(0);
	};
	delete ServerPoint;
	fxlabExternalMindArcane::Close();
};

void fxlabArcaneLightningSeed::Quant()
{
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	fxlabExternalMindArcane::Quant();
};

void fxlabArcaneLightningSeed::InterruptAction()
{
	fxlabGeneralObjectType* ct;
	fxlabGeneralObjectType* st;

	Position = Owner->R();
	Position.z = fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Owner->R().z + Owner->radius());
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_LIGHTNING_SEED);
	ct->SetStartTime(fxlabClientD->GetTime());
	ct->SetKeyID(FXLAB_ID_KEY_LIGHTNING_SEEDS);
	ct->SetPosition(Position + Vect3f(0,0,Owner->radius() * 0.33f));
	ct->Start();

	st = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_LIGHTNING_SEED_DAMAGE);
	st->SetStartTime(fxlabServerD->GetTime());
	st->SetKeyID(FXLAB_ID_KEY_LIGHTNING_SEEDS_DAMAGE);
	st->SetPosition(Position + Vect3f(0,0,Owner->radius() * 0.33f));
	st->SetRemoteObject(ct);
	st->SetProtection(ProtectionID,ProtectionTime);
	st->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
	st->SetProcessInterface(&ServerPoint[InterruptCount]);
	st->Start();
};

void fxlabArcaneLightningSeed::Condition()
{
	int i,cnt;

	fxlabExternalMindArcane::Condition();
	if(!InterruptEnable){
		cnt = 0;
		for(i = 0;i < InterruptNum;i++){
			if(ServerPoint[i].Process)
				cnt++;
		};
		if(!cnt)
			SetAlive(0);
	};
};

//---------------------------------------------------------

void fxlabArcaneCrystalSpike::Start()
{
	fxlabExternalMindArcane::Start();
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	SpikePoint.reserve(InterruptNum);
};

void fxlabArcaneCrystalSpike::Close()
{
	vector<ShareHandle<Body> >::iterator it;

	FOR_EACH(SpikePoint, it){
		if(*it && (*it)->alive())
			(*it) -> kill();
	};
	SpikePoint.clear();
	fxlabExternalMindArcane::Close();
};

void fxlabArcaneCrystalSpike::Quant()
{
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	fxlabExternalMindArcane::Quant();
};

void fxlabArcaneCrystalSpike::InterruptAction()
{
	Body* p;
	OwnerProtection* op;
	fxlabGeneralObjectType* ct;

	Position = Owner->R();
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp -> createBody("Crystal",Position + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z));
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);
//	Mdisp -> attachBody(p);	

	SpikePoint.push_back(p);

	ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	ct->SetStartTime(fxlabClientD->GetTime());
	ct->SetSoundID(EFF_CRYSTALLIZATION);
	ct->SetPosition(Position);
	ct->Start();
};

void fxlabArcaneCrystalSpike::Condition()
{
	int cnt;
	vector<ShareHandle<Body> >::iterator it;

	fxlabExternalMindArcane::Condition();
	if(!InterruptEnable){
		cnt = 0;
		FOR_EACH(SpikePoint,it){
			if(*it && (*it) -> alive())
				cnt++;
		};
		
		if(!cnt)
			SetAlive(0);
	};
};

//----------------------------

void fxlabArcaneBubbleRush::InterruptAction()
{
	Body* p;
	OwnerProtection* op;
	fxlabGeneralObjectType* t;

	Position = Owner->R();
	fxlabGetArcanePosition(Owner,Vect3f(0,Owner->radius()*1.5f,0),Position);

	ArcaneStatisticsService::setCurrentArcane(*this);
	p = Mdisp -> createBody("Bubble",Position + Vect3f(0,0,FXLAB_REALOBJECT_OFFSET_Z));
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);
//	Mdisp -> attachBody(p);	

	t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	t->SetStartTime(fxlabClientD->GetTime());
	t->SetSoundID(EFF_SOAP_BUBBLE);
	t->SetPosition(Position);
	t->Start();
};

//---------------------------------------------------------

void fxlabArcaneMortarMine::Start()
{
	fxlabExternalMindArcane::Start();
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);

	MortarPoint.reserve(InterruptNum);
	ClientPoint = new fxlabProcessInterface[InterruptNum];
};

void fxlabArcaneMortarMine::Close()
{
	int i;
	vector<ShareHandle<Body> >::iterator it;

	for(i = 0;i < InterruptNum;i++){
		if(ClientPoint[i].Process)
			ClientPoint[i].Process->SetAlive(0);
	};

	FOR_EACH(MortarPoint, it){
		if(*it && (*it)->alive())
			(*it) -> kill();
	};
	MortarPoint.clear();
	delete ClientPoint;

	fxlabExternalMindArcane::Close();
};

void fxlabArcaneMortarMine::Quant()
{
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	fxlabExternalMindArcane::Quant();
};

#include "Meteorite.h"

void fxlabArcaneMortarMine::InterruptAction()
{
	Body* p;
	OwnerProtection* op;
	fxlabGeneralObjectType* ct;

	ArcaneStatisticsService::setCurrentArcane(*this);

	p = new fxlabSleepyMortar(M3D_MORTAR,Position,Vect3f(10.0f - fxlabServerRND.UnitRND() * 20.0f,10.0f - fxlabServerRND.UnitRND() * 20.0f,30.0f),33.3f,-1);
	op = dynamic_cast<OwnerProtection*>(p);
	if(op) op->set_owner(ProtectionID,ProtectionTime);
	Mdisp -> attachBody(p);

	MortarPoint.push_back(p);

	ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_MORTAR_BODY_LINK);
	ct->SetStartTime(fxlabClientD->GetTime());
//!!!!!!!
//	ct->SetKeyID(FXLAB_ID_KEY_MINE_CLIENT_LINK);
	ct->SetPosition(Position);
	ct->SetBody(p);
	ct->SetProcessInterface(&ClientPoint[InterruptCount]);
	ct->Start();

	ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
	ct->SetStartTime(fxlabClientD->GetTime());
	ct->SetSoundID(EFF_MINES);
	ct->SetPosition(Position);
	ct->Start();
};

void fxlabArcaneMortarMine::Condition()
{
	int cnt;
	vector<ShareHandle<Body> >::iterator it;

	fxlabExternalMindArcane::Condition();
	if(!InterruptEnable){
		cnt = 0;
		FOR_EACH(MortarPoint,it){
			if(*it && (*it) -> alive())
				cnt++;
		};
		if(!cnt)
			SetAlive(0);
	};
};

//---------------------------------

void fxlabArcanePetard::Start()
{
	fxlabExternalMindArcane::Start();
	if(Owner->completed())
		Position = Owner->part_coords(M3D_FRONT);
};

void fxlabArcanePetard::Close()
{
	int i;
	vector<ShareHandle<Body> >::iterator it;

	for(i = 0;i < FXLAB_PETARD_BONUS_NUM;i++){
		if(ClientPoint[i].Process)
			ClientPoint[i].Process->SetAlive(0);
	};

	FOR_EACH(BonusPoint, it){
		if(*it && (*it)->alive())
			(*it) -> kill();
	};
	BonusPoint.clear();

	if(PetardPoint.Process)
		PetardPoint.Process->SetAlive(0);

	fxlabExternalMindArcane::Close();
};

void fxlabArcanePetard::Quant()
{
	if(Owner->completed())
		Position = Owner->part_coords(M3D_FRONT);
	fxlabExternalMindArcane::Quant();
};

void fxlabArcanePetard::InterruptAction()
{
	Body* p;
	OwnerProtection* op;
	fxlabGeneralObjectType* ct;
	int i;
	float a,da;

	if(InterruptCount){
		if(PetardPoint.Process)
			PetardPoint.Process->SetAlive(0);

		ExplodePosition += Velocity * (CurrentTime * fxlabGlobalTimeRate);
		ExplodePosition = CYCLE(ExplodePosition);

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_GENERATOR);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetPosition(ExplodePosition);
		ct->SetKeyID(FXLAB_ID_KEY_PETARD_BURN);
		ct->Start();

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_OMNI);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetPosition(ExplodePosition);
		ct->SetKeyID(FXLAB_ID_KEY_PETARD_BURN_OMNI);
		ct->Start();

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_CONTROL_MODEL);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetPosition(ExplodePosition);
		ct->SetKeyID(FXLAB_ID_KEY_PETARD_BURN_MODEL);
		ct->Start();

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetSoundID(EFF_PETARD_EXPLOSION);
		ct->SetPosition(ExplodePosition);
		ct->Start();

		a = fxlabServerRND.UnitRND() * M_PI * 2.0f;
		da = 2.0f * M_PI / FXLAB_PETARD_BONUS_NUM;
		for(i = 0;i < FXLAB_PETARD_BONUS_NUM;i++){
			if(!fxlabServerRND.RND(2)){
				ArcaneStatisticsService::setCurrentArcane(*this);

				p = new fxlabMinorBonus(M3D_TOTAL_TYPE(FXLAB_MODEL_INTERFACE_ID_BOX_START + fxlabServerRND.RND(FXLAB_MODEL_INTERFACE_ID_BOX_NOISE),M3D_XREAL),ExplodePosition,Vect3f(20.0f * cosf(a),20.0f * sinf(a),0.0),0,-1);
				op = dynamic_cast<OwnerProtection*>(p);
				if(op) op->set_owner(ProtectionID,ProtectionTime);
				Mdisp -> attachBody(p);	

				ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BONUS_BODY_LINK);
				ct->SetStartTime(fxlabClientD->GetTime());
//!!!
//				ct->SetKeyID(FXLAB_ID_KEY_MINOR_BONUS_CLIENT_LINK);
				ct->SetPosition(ExplodePosition);
				ct->SetBody(p);
				ct->SetProcessInterface(&ClientPoint[i]);
				ct->Start();
			}else{
				ArcaneStatisticsService::setCurrentArcane(*this);

				p = new fxlabMajorBonus(M3D_TOTAL_TYPE(FXLAB_MODEL_INTERFACE_ID_BOX_START + fxlabServerRND.RND(FXLAB_MODEL_INTERFACE_ID_BOX_NOISE),M3D_XREAL),ExplodePosition,Vect3f(20.0f * cosf(a),20.0f * sinf(a),0.0),0,-1);
				op = dynamic_cast<OwnerProtection*>(p);
				if(op) op->set_owner(ProtectionID,ProtectionTime);
				Mdisp -> attachBody(p);

				ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_BONUS_BODY_LINK);
				ct->SetStartTime(fxlabClientD->GetTime());
//!!!!!
//				ct->SetKeyID(FXLAB_ID_KEY_MAJOR_BONUS_CLIENT_LINK);
				ct->SetPosition(ExplodePosition);
				ct->SetBody(p);
				ct->SetProcessInterface(&ClientPoint[i]);
				ct->Start();
			};
			BonusPoint.push_back(p);
			a += da;
		};
	}else{
		Velocity = Owner->Yglobal() * 3.0f;
		Velocity += Owner->Zglobal();
		Velocity.Normalize(20.f);
//		Velocity += Owner->Yglobal() * Owner->actualSpeed() / 20.0f;

		ExplodePosition = Position;
		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_PETARD_MODEL);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetKeyID(FXLAB_ID_KEY_PETARD_MODEL);
		ct->SetPosition(Position);
		ct->SetVelocity(Velocity);
		ct->SetProcessInterface(&PetardPoint);
		ct->Start();

		ct = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SOUND_SPACE_IMPULSE);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetSoundID(EFF_PETARD_START);
		ct->SetPosition(Position);
		ct->Start();
	};
};

void fxlabArcanePetard::Condition()
{
	int cnt;
	vector<ShareHandle<Body> >::iterator it;

	fxlabExternalMindArcane::Condition();
	if(!InterruptEnable){
		cnt = 0;

		FOR_EACH(BonusPoint,it){
			if(*it && (*it) -> alive())
				cnt++;
		};

		if(!cnt)
			SetAlive(0);
	};
};

//------------------------------

const float FXLAB_SCARLET_TRACK_DENSITY = 20.0f;

void fxlabArcaneScarletTrack::Start()
{
	fxlabExternalMindArcane::Start();
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	LastPosition = Position;
	ServerPoint = new fxlabProcessInterface[InterruptNum];
	ClientPoint = new fxlabProcessInterface[InterruptNum];
};

void fxlabArcaneScarletTrack::Close()
{
	int i;
	for(i = 0;i < InterruptNum;i++){
		if(ServerPoint[i].Process)
			ServerPoint[i].Process->SetAlive(0);
		if(ClientPoint[i].Process)
			ClientPoint[i].Process->SetAlive(0);
	};
	delete ServerPoint;
	delete ClientPoint;
	fxlabExternalMindArcane::Close();
};

void fxlabArcaneScarletTrack::Quant()
{
	if(Owner->completed())
		Position = Owner->part_coords(M3D_ENGINE);
	fxlabExternalMindArcane::Quant();
};

void fxlabArcaneScarletTrack::InterruptUse()
{
	Vect3f v;
	fxlabGeneralObjectType* t;

	if(InterruptEnable){
		if(InterruptCount < InterruptNum){
			v = getDist(LastPosition,Position);
			if(v.norm() > FXLAB_SCARLET_TRACK_DENSITY){
				t = fxlabClientD->CreateObject(FXLAB_CLIENT_PROCESS_SPARK_LINE);
				t->SetStartTime(fxlabClientD->GetTime());
				t->SetPosition(Vect3f(Position.x,Position.y,fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Position.z + Owner->radius())));
				t->SetVelocity(v);
				t->SetKeyID(FXLAB_ID_KEY_SCARLET_TRACK_LINE);
				t->SetProcessInterface(&(ClientPoint[InterruptCount]));
				t->Start();


  				t = fxlabServerD->CreateObject(FXLAB_SERVER_PROCESS_RED_TRACK_CIRCLE);
				t->SetStartTime(fxlabServerD->GetTime());
				t->SetPosition(Vect3f(Position.x,Position.y,fxlabGetLevel(XCYCL(round(Position.x)),YCYCL(round(Position.y)),Position.z + Owner->radius())));
				t->SetKeyID(FXLAB_ID_KEY_SCARLET_TRACK_DAMAGE);
				t->SetProcessInterface(&(ServerPoint[InterruptCount]));
				t->SetProtection(ProtectionID,ProtectionTime);
				t->SetStatistic(GetOwnerID(),GetArcaneID(),GetArcaneOwner(),GetObjectID());
				t->Start();

				LastPosition = Position;
				InterruptCount++;
				if(InterruptCount >= InterruptNum){
					LinkPoint->Status = 1;
					InterruptEnable = 0;
				};
			};
		}else{
			LinkPoint->Status = 1;
			InterruptEnable = 0;
		};
	};
};

//---------------------------------------------------------

void fxlabArcaneRedTrack::Start()
{
	fxlabInternalArcane::Start();
	ArcaneStatisticsService::setCurrentArcane(*this);
	Owner->set_feature(Mechos::RED_TRACK_FEATURE);
};

void fxlabArcaneRedTrack::Close()
{
	Owner->reset_feature(Mechos::RED_TRACK_FEATURE);
	fxlabInternalArcane::Close();
};


void fxlabGetArcanePosition(Mechos* owner,const Vect3f& offset,Vect3f& pos)
{
	Vect3f AxisX,AxisY,AxisZ;

	AxisX = owner->Xglobal();
	AxisY = owner->Yglobal();
	AxisZ = owner->Zglobal();

	pos.x += offset.x * AxisX.x + offset.y * AxisY.x + offset.z * AxisZ.x;
	pos.y += offset.x * AxisX.y + offset.y * AxisY.y + offset.z * AxisZ.y;
	pos.z += offset.x * AxisX.z + offset.y * AxisY.z + offset.z * AxisZ.z;

	pos = CYCLE(pos);
};
