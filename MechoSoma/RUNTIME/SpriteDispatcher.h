
#ifndef __SPRITEDISPATCHER_H__
#define __SPRITEDISPATCHER_H__

struct mchA_Sprite
{
	int ID;

	int slotX;
	int slotY;

	int SizeX;
	int SizeY;

	int bound[4];

	char* fileName;
	int d3dHandle;

	void* list;
	mchA_Sprite* prev;
	mchA_Sprite* next;

	void load(void);
	void load(char* fname);
	void calc_bound(char* data);

	mchA_Sprite(int id = 0);

	mchA_Sprite(const char* fname,int id = 0);
	~mchA_Sprite(void);
};

struct mchA_SpriteDispatcher
{
	xtList<mchA_Sprite> spriteLst;

	void LoadScript(const char* fname);

	void AddSprite(int id,const char* fname);

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