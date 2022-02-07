
#ifndef __ARCANE_H__
#define __ARCANE_H__

#include "fxlabInterface.h"

enum mchArcaneID {
	ARC_STOP_TIME = 0,
	ARC_DESINTEGRATE,

	ARC_MAX_ID
};

enum mchArcaneStatus
{
	ARC_STATUS_DISABLE = 0,
	ARC_STATUS_ENABLE = 1
};

struct mchArcaneData
{
	int ID;
	int ownerID;

	char* name;
	char* text;

	int Status;

	float MaxPower;
	float Power;
	float DeltaPower;
	float ActivatePower;
	float ChargePower;
	int Interruption;

	fxlabProcessInterface fxlabFace;

	void init(void* p);
	void fxlabInit(void){ fxlabFace.Init(); };
	void fxlabStart(void);
	void fxlabStop(void);
};

// mchBonusArcaneData::flags
#define ARC_COPY	0x01

struct mchBonusArcaneData 
{
	int ID;
	int Type;
	float Timer;

	int flags;

	char* name;

	void* list;
	mchBonusArcaneData* next;
	mchBonusArcaneData* prev;

	void init(void* p);
	void copy(mchBonusArcaneData* p);

	mchBonusArcaneData(void);
	~mchBonusArcaneData(void);
};

enum fxRacerEventType
{
	FX_RACER_EVENT_NONE = 0,
	FX_RACER_EVENT_RIGHT_CHECK_POINT = 1,
	FX_RACER_EVENT_WRONG_CHECK_POINT = 2,
	FX_RACER_EVENT_LAP = 4,
	FX_RACER_EVENT_SEED = 8,
	FX_RACER_EVENT_DESTRUCTION = 16,
	FX_RACER_EVENT_FINISH = 32
};

void fxlabRacerEvent(int type,void* point);//mchRacer* point
mchBonusArcaneData* mchGetBonusData(int id);
mchBonusArcaneData* mchGetBonusDataByType(int tp);

void fxlabBonusEvent(int id,struct mchRacer* owner);

#endif // __ARCANE_H__