
#ifndef __SPRITEDISPATCHER_H__
#define __SPRITEDISPATCHER_H__

struct mchA_SpriteSlot
{
	int ID;

	int SizeX;
	int SizeY;

	int spriteID;
	int inactiveTimer;

	unsigned d3dHandle;

	void* list;
	mchA_SpriteSlot* next;
	mchA_SpriteSlot* prev;

	void loadSprite(void* p,int id);
	void loadSprite(int x,int y,int sx,int sy,void* p,int id);

	void init(int sx,int sy);

	mchA_SpriteSlot(int id = 0);
	mchA_SpriteSlot(int id,int handle,int sx,int sy);
	~mchA_SpriteSlot(void);
};

struct mchA_Sprite
{
	int ID;

	int slotID;
	int slotX;
	int slotY;

	int SizeX;
	int SizeY;

	int bound[4];

	char* fileName;
	char* data;

	void* list;
	mchA_Sprite* prev;
	mchA_Sprite* next;

	void load(void);
	void load(char* fname);
	void calc_bound(void);

	mchA_Sprite(int id = 0);

	mchA_Sprite(char* fname,int id = 0);
	~mchA_Sprite(void);
};

struct mchA_SpriteDispatcher
{
	xtList<mchA_Sprite> spriteLst;
	xtList<mchA_SpriteSlot> slotLst;

	void Quant(void);

	void LoadScript(char* fname);

	void AddSlot(int sx,int sy,int id);
	void AddSprite(int id,char* fname);

	int GetSlot(int spriteID);

	void DrawSprite(int x,int y,float sx,float sy,int id,int col = -1,int alpha = 255,float angle = 0.0f,int center = 0);
	void DrawSpriteClip(int x,int y,float sx,float sy,float l,float t,float r,float b,int id,int col = -1,int alpha = 255,float angle = 0.0f,int center = 0);
	void DrawSpriteZ(int x,int y,int z,float sx,float sy,int id,int col = -1,int alpha = 255,float angle = 0.0f,int center = 0);
	void DrawSpriteClipZ(int x,int y,int z,float sx,float sy,float l,float t,float r,float b,int id,int col = -1,int alpha = 255,float angle = 0.0f,int center = 0);

	void DrawArcaneSymbol(int x,int y,int id,float phase,float angle,int color,int alpha = 255,float scale = 1.0f,int center = 0);
	void DrawArcaneSymbolZ(int x,int y,int z,int id,float phase,float angle,int color,int alpha = 255,float scale = 1.0f,int center = 0);
	void DrawEssence(int x,int y,int id,float phase,float angle,int color,int alpha = 255);

	mchA_SpriteDispatcher(void);
	~mchA_SpriteDispatcher(void);
};

extern mchA_SpriteDispatcher* mchA_SprD;

void mchA_InitSpriteDispatcher(void);
void mchA_FinitSpriteDispatcher(void);

#endif // __SPRITEDISPATCHER_H__