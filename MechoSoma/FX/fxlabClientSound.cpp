#include "StdAfx.h"
#include "aci_parser.h"
#include "TERRA.H"
#include "sound.h"

#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientSound.h"

extern int mchPBEM_HiddenTime;

void fxlabClientImpulseSound::Open(void)
{
	fxlabClientEvolutionType::Open();
	SoundID = 0;
};

void fxlabClientImpulseSound::Start(void)
{
	fxlabClientEvolutionType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID);
};

//---------------------------------------

void fxlabClientImpulseSpaceSound::Open(void)
{
	fxlabClientSpaceType::Open();
	SoundID = 0;
};

void fxlabClientImpulseSpaceSound::Start(void)
{
	fxlabClientSpaceType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

//---------------------------------------

void fxlabClientLongSound::Open(void)
{
	fxlabClientEvolutionType::Open();
	SoundID = 0;
};

void fxlabClientLongSound::Start(void)
{
	fxlabClientEvolutionType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID);
};

void fxlabClientLongSound::Close(void)
{
	if(!mchPBEM_HiddenTime)
		mchStopSoundEFF(SoundID);
	fxlabClientEvolutionType::Close();
};

//---------------------------------------

void fxlabClientLongSpaceSound::Open(void)
{
	fxlabClientSpaceType::Open();
	SoundID = 0;
};

void fxlabClientLongSpaceSound::Start(void)
{
	fxlabClientSpaceType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

void fxlabClientLongSpaceSound::Quant(void)
{
	fxlabClientSpaceType::Quant();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

void fxlabClientLongSpaceSound::Close(void)
{
	if(!mchPBEM_HiddenTime)
		mchStopSoundEFF(SoundID);
	fxlabClientSpaceType::Close();
};
