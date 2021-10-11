
#ifndef __AI_ARCANE_H__
#define __AI_ARCANE_H__

#include "ai_arcane_prm.h"

#define AI_ARC_PRM_MAX	5

struct mchAI_ArcanePrm
{
	int ID;
	int prmData[AI_ARC_PRM_MAX];

	void* list;
	mchAI_ArcanePrm* prev;
	mchAI_ArcanePrm* next;

	mchAI_ArcanePrm(scrDataBlock* p);
	~mchAI_ArcanePrm(void){ };
};

#endif // __AI_ARCANE_H__