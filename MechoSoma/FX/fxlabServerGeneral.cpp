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


#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabServerGeneral.h"
#include "mesh3ds.h"


void fxlabServerEvolutionType::Start(void)
{
	CurrentTime = fxlabServerD->GetTime() - StartTime;	
	fxlabApplicationObjectType::Start();
};

void fxlabServerEvolutionType::Quant(void)
{
	CurrentTime = fxlabServerD->GetTime() - StartTime;	
	fxlabApplicationObjectType::Quant();
	ProtectionQuant();
};

void fxlabServerEvolutionType::SetProtection(int id,int time)
{
	fxlabProtectionType::SetProtection(id,time);
};

void fxlabServerEvolutionType::SetStatistic(int owner_id,int arcane_id,fxlabGeneralObjectType* p,int object_id)
{
	GetOwnerID() = owner_id;
	GetArcaneID() = arcane_id;
	GetArcaneOwner() = p;
	GetObjectID() = object_id;
};

//-----------------------------------------

void fxlabServerKeyObjectType::Start(void)
{
	fxlabServerSpaceType::Start();
	KeyTime = CurrentTime * fxlabGlobalTimeRate;

	if(KeyID >= 0){
		KeyPoint = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,KeyID);
		KeyData = new float[KeyPoint->GetKeyNum()];
		KeyUpdate();
		KeyCheck();
	};
};

void fxlabServerKeyObjectType::Close(void)
{
	if(KeyData) delete KeyData;
	fxlabServerSpaceType::Close();
};

void fxlabServerKeyObjectType::Quant(void)
{
	fxlabServerSpaceType::Quant();
	KeyTime = CurrentTime * fxlabGlobalTimeRate;

	if(KeyPoint){
		if(KeyPoint->GetKeyAttribute() & FXLAB_KEY_ATTRIBUTE_STORAGE)
			Alive &= KeyPoint->GetKeyStatus(KeyTime);
		else
			KeyUpdate();
	};
};

void fxlabServerKeyObjectType::KeyUpdate(void)
{
	Alive &= KeyPoint->GetKey(KeyData,KeyTime);
};

//------------------------------

/*fxlabTrackDataType::fxlabTrackDataType(void)
{
	TrackType = TRACK_EL_BUSY;
	TrackRadius = 0;
};

void fxlabTrackDataType::SetTrackInfo(void)
{
	TrackType = TRACK_EL_BUSY;
	TrackRadius = 0;
};

void fxlabTrackDataType::UpdateTrack(int x,int y)
{
	mch_trackMap->SetFlag(x,y,TrackRadius,TrackRadius,TrackType);
};*/

//-----------------------------------------

fxlabProtectionType::fxlabProtectionType(void)
{
	ProtectionID = -1;
	ProtectionTime = 0;
	ProtectionTimer.start(0);
};

void fxlabProtectionType::ProtectionQuant(void)
{
	if(!ProtectionTimer())
		ProtectionID = -1;
};

int fxlabProtectionType::CheckProtection(struct mchRacer* p)
{ 
	return (p->Body::ID != ProtectionID); 
};

//--------------------------------------

/*void fxlabArcaneStart(const class ArcaneStatisticsInfo* p)
{
	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && mch_raceD->activeRacer->ID == p->GetOwnerID())
		mch_arcStatD->Add(p->GetArcaneID(),p->GetArcaneID());
};*/

void fxlabArcaneStart(const class ArcaneStatisticsInfo* p)
{
	mchRacer* rp;

	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && mch_raceD->racerLst){
		rp = mch_raceD->racerLst->first();
		while(rp){
			if(rp->ID == p->GetOwnerID() && rp->Type == MCH_RACER_TYPE_MECHOS){
				if(rp->flags & MCH_ACTIVE_RACER)
					mchA_AddArcaneStats(p->GetArcaneID(),p->GetArcaneID(),(rp->flags & MCH_ACTIVE_RACER2));
			};
			rp = rp->next;
		};
	};
};


/*void fxlabArcaneStop(const class ArcaneStatisticsInfo* p)
{
	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && mch_raceD->activeRacer->ID == p->GetOwnerID())
		mch_arcStatD->HandleEvent(p->GetArcaneID(),AE_REMOVE);
};*/

void fxlabArcaneStop(const class ArcaneStatisticsInfo* p)
{
	mchRacer* rp;

	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && mch_raceD->racerLst){
		rp = mch_raceD->racerLst->first();
		while(rp){
			if(rp->ID == p->GetOwnerID() && rp->Type == MCH_RACER_TYPE_MECHOS){
				if(rp->flags & MCH_ACTIVE_RACER){
					mchA_HandleStatsEvent(p->GetArcaneID(),AE_REMOVE,NULL,(rp->flags & MCH_ACTIVE_RACER2));
				};
			};
			rp = rp->next;
		};
	};
};
						  
/*void fxlabArcaneAction(const class ArcaneStatisticsInfo* p)
{
	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && p->GetArcaneOwner()){
		if(mch_raceD->activeRacer->ID == p->GetOwnerID()){
			mch_arcStatD->HandleEvent(p->GetArcaneID(),AE_SET_ROTATE_ON);
			mch_arcStatD->HandleEvent(p->GetArcaneID(),AE_SET_ROTATE_OFF);
		};
		fxlabServerD->AddAction(p->GetArcaneOwner());
	};
};*/

void fxlabArcaneAction(const class ArcaneStatisticsInfo* p,int id)
{
	mchRacer* rp;
	Mechos* mp;

	if(!p) return;
	if(p->GetOwnerID() != -1 && p->GetArcaneID() != -1 && p->GetArcaneOwner()){
/*		if(mch_raceD->activeRacer->ID == p->GetOwnerID()){
			mch_arcStatD->HandleEvent(p->GetArcaneID(),AE_SET_ROTATE_ON);
			mch_arcStatD->HandleEvent(p->GetArcaneID(),AE_SET_ROTATE_OFF);
		};*/

		fxlabServerD->AddAction(p->GetArcaneOwner(),p->GetObjectID());

		if(mch_raceD->racerLst){
			rp = mch_raceD->racerLst->first();
			while(rp){
				if(rp->ID == p->GetOwnerID() && rp->Type == MCH_RACER_TYPE_MECHOS){
					if(rp->flags & MCH_ACTIVE_RACER){
						mchA_HandleStatsEvent(p->GetArcaneID(),AE_SET_ROTATE_ON,NULL,(rp->flags & MCH_ACTIVE_RACER2));
						mchA_HandleStatsEvent(p->GetArcaneID(),AE_SET_ROTATE_OFF,NULL,(rp->flags & MCH_ACTIVE_RACER2));
					};
					(static_cast<mchMechosRacer*>(rp))->startEffect(Mechos::ExultEffect);
				};
				rp = rp->next;
			};
		};

		mp = dynamic_cast<Mechos*>(Mdisp->searchBody(id));
		if(mp){
			mp->startEffect(Mechos::InfluenceOfArcaneEffect);
		};
	};
};

void fxlabArcaneStatisticInit(void)
{
/*	fxlabGeneralObjectType* l;
	list<fxlabGeneralObjectType*>::iterator p;

	if(!mch_raceD) return;
	if(!mch_raceD->activeRacer) return;

	FOR_EACH(fxlabServerD->ObjectList,p)
	{
		l = *p;
		if((*p)->GetType() > FXLAB_SERVER_PROCESS_ARCANE && (*p)->GetStatisticOwnerID() == mch_raceD->activeRacer->ID)
			fxlabArcaneStart((*p)->GetStatistic());
	};

	FOR_EACH(fxContext->CreateList,p)
	{
		l = *p;
		if((*p)->GetType() > FXLAB_SERVER_PROCESS_ARCANE && (*p)->GetStatisticOwnerID() == mch_raceD->activeRacer->ID)
			fxArcaneStart((*p)->GetStatistic());
	};*/
};
