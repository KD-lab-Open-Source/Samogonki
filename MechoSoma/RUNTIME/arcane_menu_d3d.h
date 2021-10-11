
#define AE_D3D_FONT		2
#define AE_D3DSPR_MAX_PHASE	64

enum mchA_d3dSpriteID 
{
	AE_D3DSPR_MAP =	256,
	AE_D3DSPR_POINTER,		// 257
	AE_D3DSPR_MAP_POINTER,		// 258
	AE_D3DSPR_MAP_BAR,		// 259
	AE_D3DSPR_CIRCLE,		// 260
	AE_D3DSPR_SQUARE,		// 261
	AE_D3DSPR_TRIANGLE,		// 263
	AE_D3DSPR_DUMMY,		// 264
	AE_D3DSPR_MOUSE,		// 265
	AE_D3DSPR_BODY0,		// 266
	AE_D3DSPR_BODY1,		// 267
	AE_D3DSPR_BODY2,		// 268
	AE_D3DSPR_BODY_I0,		// 269
	AE_D3DSPR_BODY_I1,		// 270
	AE_D3DSPR_BODY_I2,		// 271
	AE_D3DSPR_MOUSE_CROSS,		// 272
	AE_D3DSPR_MOUSE_HAND,		// 273
	AE_D3DSPR_SQ_CORNER0,		// 274
	AE_D3DSPR_SQ_CORNER1,		// 275
	AE_D3DSPR_SQ_CORNER2,		// 276
	AE_D3DSPR_SQ_CORNER3,		// 277
	AE_D3DSPR_SQ_SIDE0,		// 278
	AE_D3DSPR_SQ_SIDE1,		// 279
	AE_D3DSPR_SQ_SIDE2,		// 280
	AE_D3DSPR_SQ_SIDE3,		// 281
	AE_D3DSPR_ZZZ,			// 282

	AE_D3DSPR_MAX_ID
};

const int AE_D3DSPR_MAX = AE_D3DSPR_MAX_ID + AE_D3DSPR_MAX_PHASE * 4;

void mchA_d3dInit(void);

void mchA_d3dOutSprite(int x,int y,float sx,float sy,int idx,int col,int alpha,float rotate = 0.0f,int center_flag = 0);
void mchA_d3dOutSpriteZ(int x,int y,int z,float sx,float sy,int idx,int col,int alpha,float rotate = 0.0f,int center_flag = 0);
void mchA_d3dDrawSprite(int x,int y,float sx,float sy,int handle,int col,int alpha,float rotate = 0.0f,int center_flag = 0);
void mchA_d3dDrawSpriteZ(int x,int y,int z,float sx,float sy,int handle,int col,int alpha,float rotate = 0.0f,int center_flag = 0);

void mchA_d3dOutString(int x,int y,float sx,float sy,void* str,int col,int alpha,int fnt,int space = 0,float mul = 1.0f,int pr_flag = 1,int vspace = 0);
void mchA_d3dOutStringZ(int x,int y,int z,float sx,float sy,void* str,int col,int alpha,int fnt,int space = 0,float mul = 1.0f,int pr_flag = 1);
void mchA_d3dSetClip(int x,int y,int sx,int sy);
void mchA_d3dClearClip(void);

void mchA_d3dScreenShot(void* buf,int sz);

void mchA_d3dLockSprite(int handle,void** p,unsigned& pitch);
void mchA_d3dUnlockSprite(int handle);

int mchA_d3dCreateSlot(int sx,int sy);
void mchA_d3dConvertSprite(int sx,int sy,void* src,void* dest,int src_colors);

void mchA_d3dToggleColorKey(int value);
void mchA_d3dCreateBackBuffer(void);
void mchA_d3dReleaseBackBuffer(void);
void mchA_d3dLockBackBuffer(void);
void mchA_d3dUnlockBackBuffer(void);
void mchA_d3dFlushBackBuffer(int x,int y,int sx,int sy);
void mchA_d3dFlip(void);
void mchA_d3dClear(unsigned cl);
void mchA_d3dBeginScene(void);
void mchA_d3dEndScene(void);
void mchA_d3dPrepare(void);

void mchA_d3dClipSprite(int handle,float l = 0.0f,float t = 0.0f,float r = 1.0f,float b = 1.0f);

void mchA_d3dRectangleSq(int x,int y,int sx,int sy,int col,int al);
void mchA_d3dRectangle(int x,int y,int sx,int sy,int col,int outline = 0);
void mchA_d3dRectangleZ(int x,int y,int z,int sx,int sy,int col,int outline = 0,int alpha = 255);
void mchA_d3dRectangle(int x,int y,int sx,int sy,float color_f,int outline = 0);
void mchA_d3dDrawScrollbar(int x,int y,int sx,int sy,int val,int max_val);

int mchA_d3dCheckMode(int mode,int color_depth);

unsigned mchA_d3dMakeColor(int r,int g,int b);

void mchA_d3dSetGamma(float v);
float mchA_d3dGetGamma(void);

void mchA_DarkenRect(int dwLeft,int dwTop,int dwRight,int dwBottom,int dwDarkness);

extern unsigned mchA_ColorF[];
extern unsigned mchA_d3dColorKey;
extern float mchA_d3dResX;
extern float mchA_d3dResY;
