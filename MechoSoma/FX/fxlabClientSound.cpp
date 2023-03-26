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

void fxlabClientImpulseSound::Open()
{
	fxlabClientEvolutionType::Open();
	SoundID = 0;
};

void fxlabClientImpulseSound::Start()
{
	fxlabClientEvolutionType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID);
};

//---------------------------------------

void fxlabClientImpulseSpaceSound::Open()
{
	fxlabClientSpaceType::Open();
	SoundID = 0;
};

void fxlabClientImpulseSpaceSound::Start()
{
	fxlabClientSpaceType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

//---------------------------------------

void fxlabClientLongSound::Open()
{
	fxlabClientEvolutionType::Open();
	SoundID = 0;
};

void fxlabClientLongSound::Start()
{
	fxlabClientEvolutionType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID);
};

void fxlabClientLongSound::Close()
{
	if(!mchPBEM_HiddenTime)
		mchStopSoundEFF(SoundID);
	fxlabClientEvolutionType::Close();
};

//---------------------------------------

void fxlabClientLongSpaceSound::Open()
{
	fxlabClientSpaceType::Open();
	SoundID = 0;
};

void fxlabClientLongSpaceSound::Start()
{
	fxlabClientSpaceType::Start();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

void fxlabClientLongSpaceSound::Quant()
{
	fxlabClientSpaceType::Quant();
	if(!mchPBEM_HiddenTime)
		mchStartSoundEFF(SoundID,Position.x,Position.y);
};

void fxlabClientLongSpaceSound::Close()
{
	if(!mchPBEM_HiddenTime)
		mchStopSoundEFF(SoundID);
	fxlabClientSpaceType::Close();
};
