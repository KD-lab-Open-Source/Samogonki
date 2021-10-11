#include "StdAfx.h"

#include "aci_parser.h"

#include "polymgr.h"

#include "fxlabID.h"
#include "fxlabResource.h"
#include "fxlabSprite.h"


void fxlabSpriteType::Open(scrDataBlock* data)
{
	scrDataBlock* p;
	p = data;
	while(p){
		switch(p->ID){
			case FXLAB_SCRIPT_SPRITE_PHASE_NUM:
				PhaseNum = *(p->i_dataPtr);
				break;
			case FXLAB_SCRIPT_SPRITE_KEY_ID:
				KeyID = *(p->i_dataPtr);
				break;
			case FXLAB_SCRIPT_SPRITE_TEXTURE_ID:
				TextureID = *(p->i_dataPtr);
				break;
			default:
				ErrH.Abort("Bad Sprite KeyWord in fxlabResource");
				break;
		};
		p = p->next;
	};

	SpriteData = new sSpriteFX[PhaseNum];
	Scale = (float)(PhaseNum - 1);
	InvScale = 1.0f / Scale;
};

void fxlabSpriteType::Close(void)
{
	delete SpriteData;
};

sSpriteFX* fxlabSpriteType::GetSpriteData(float phase)
{
	return SpriteData + round(phase * Scale);
};

void fxlabSpriteType::Link(void)
{
	int i;
	float t,dt;
	float buffer[FXLAB_RESOURCE_SPRITE_MAX];
	fxlabResourceObject* key;

	key = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,KeyID);
	if(key->GetKeyNum() != FXLAB_RESOURCE_SPRITE_MAX)
		ErrH.Abort("Bad Key Size for Sprite in fxlabResource");	

	t = 0;
	dt = key->GetScale() / (float)(PhaseNum);

	for(i = 0;i < PhaseNum;i++){
		key->GetKey(buffer,t);
		SpriteData[i].Init(sColor4f(buffer[FXLAB_RESOURCE_SPRITE_RED],buffer[FXLAB_RESOURCE_SPRITE_GREEN],buffer[FXLAB_RESOURCE_SPRITE_BLUE],buffer[FXLAB_RESOURCE_SPRITE_ALPHA]),buffer[FXLAB_RESOURCE_SPRITE_ANGLE],buffer[FXLAB_RESOURCE_SPRITE_SIZE]);
		t += dt;
	};
};
