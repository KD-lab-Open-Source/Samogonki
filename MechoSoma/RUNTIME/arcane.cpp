/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "arcane.h"

#include "aci_parser.h"
#include "m3dsetup.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

xtList<mchBonusArcaneData> mchBonusLst;

mchBonusArcaneData::mchBonusArcaneData(void)
{
	ID = -1;
	Type = -1;
	Timer = 0;
	flags = 0;
	name = NULL;
	list = NULL;
}

mchBonusArcaneData::~mchBonusArcaneData(void)
{
	if(!(flags & ARC_COPY) && name) 
		free(name);
}

void mchBonusArcaneData::init(void* p)
{
	scrDataBlock* sp,*sp1;
	sp = (scrDataBlock*)p;

	ID = *sp -> i_dataPtr;
	sp1 = sp -> nextLevel -> first();
	while(sp1){
		switch(sp1 -> ID){
			case M3D_ARCANE_NAME:
				name = strdup(sp1 -> c_dataPtr);
				break;
			case M3D_BONUS_TIMER:
				Timer = (float)(*sp1 -> i_dataPtr);
				break;
			case M3D_Type:
				Type = *sp1 -> i_dataPtr;
				break;
		}
		sp1 = sp1 -> next;
	}
}

void mchArcaneData::init(void* p)
{
	scrDataBlock* sp,*sp1;
	sp = (scrDataBlock*)p;

	Status = 1;

	if(!sp)
		name = strdup("NONE");
	ID = -1;

	Power = 0.0f;
	MaxPower = 100.0f;
	DeltaPower = 1.0f;
	ActivatePower = 100.0f;
	text = NULL;

	if(sp){
		ID = *sp -> i_dataPtr;
		sp1 = sp -> nextLevel -> first();
		while(sp1){
			switch(sp1 -> ID){
				case M3D_ARCANE_NAME:
					name = strdup(sp1 -> c_dataPtr);
					break;
				case M3D_ARCANE_POWER:
					MaxPower = (float)*sp1->d_dataPtr;
					break;
				case M3D_ARCANE_DELTA_POWER:
					DeltaPower = (float)*sp1->d_dataPtr;
					break;
				case M3D_ARCANE_ACTIVATE_POWER:
					ActivatePower = (float)*sp1->d_dataPtr;
					break;
				case M3D_ARCANE_CHARGE_POWER:
					if(mchDebugMode & 2)
						ChargePower = MaxPower;
					else
						ChargePower = (float)*sp1->d_dataPtr;
					break;
				case M3D_ARCANE_INTERRUPTION:
					Interruption = *sp1->i_dataPtr;
					break;
				case M3D_ARCANE_TEXT:
					text = strdup(sp1 -> c_dataPtr);
					break;
			}
			sp1 = sp1 -> next;
		}
	}
}

void mchCreateBonusArcane(void* p)
{
	mchBonusArcaneData* bp = new mchBonusArcaneData;
	bp -> init(p);

	mchBonusLst.append(bp);
}

mchBonusArcaneData* mchGetBonusDataByType(int tp)
{
	mchBonusArcaneData* p = mchBonusLst.first();
	while(p){
		if(p -> Type == tp) return p;
		p = p -> next;
	}
	return NULL;
}

mchBonusArcaneData* mchGetBonusData(int id)
{
	mchBonusArcaneData* p = mchBonusLst.search(id);
	return p;
}

void mchBonusArcaneData::copy(mchBonusArcaneData* p)
{
	p -> ID = ID;
	p -> Timer = Timer;
	p -> flags = flags | ARC_COPY;
	p -> Type = Type;
	p -> name = name;
}
