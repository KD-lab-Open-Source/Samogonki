enum fxlabResourceSpriteField
{
	FXLAB_RESOURCE_SPRITE_RED = 0,
	FXLAB_RESOURCE_SPRITE_GREEN,
	FXLAB_RESOURCE_SPRITE_BLUE,
	FXLAB_RESOURCE_SPRITE_ALPHA,
	FXLAB_RESOURCE_SPRITE_SIZE,
	FXLAB_RESOURCE_SPRITE_ANGLE,
	FXLAB_RESOURCE_SPRITE_FRAME,
	FXLAB_RESOURCE_SPRITE_MAX
};

struct fxlabSpriteType : fxlabResourceObject
{
	int PhaseNum;
	int TextureID;
	int KeyID;

	struct sSpriteFX* SpriteData;

	void Open(scrDataBlock* data);
	void Close(void);
	void Link(void);

	float GetScale(void){ return Scale; };
	float GetInvScale(void){ return InvScale; };

	int GetSpriteID(void){ return TextureID; };
	sSpriteFX* GetSpriteData(float phase);
};
