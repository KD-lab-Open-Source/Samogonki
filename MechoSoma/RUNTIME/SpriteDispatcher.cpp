/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "SpriteDispatcher.h"
#include "arcane_menu_d3d.h"

#include "scr_defs.h"
#include "aci_parser.h"

#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int mchA_d3dSpriteParent;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);
void mchLoadTGA(XStream& fh,void** p,int& sx,int& sy,int& colors);

/* --------------------------- DEFINITION SECTION --------------------------- */

mchA_SpriteDispatcher* mchA_SprD = NULL;

mchA_SpriteSlot::mchA_SpriteSlot(int id,int handle,int sx,int sy)
{
	ID = id;
	d3dHandle = handle;

	spriteID = -1;

	inactiveTimer = 0;

	SizeX = sx;
	SizeY = sy;

	list = NULL;
}

mchA_SpriteSlot::mchA_SpriteSlot(int id)
{
	ID = id;
	SizeX = SizeY = 0;

	spriteID = -1;

	list = NULL;

	d3dHandle = 0;
}

mchA_SpriteSlot::~mchA_SpriteSlot(void)
{
}

void mchA_SpriteSlot::init(int sx,int sy)
{
	SizeX = sx;
	SizeY = sy;

	d3dHandle = mchA_d3dCreateSlot(sx,sy);
}

void mchA_SpriteSlot::loadSprite(void* p,int id)
{
	int i,idx = 0;
	spriteID = id;

	unsigned pitch;
	void* spr_buf;

	if(RenderMode == XGRAPH_HICOLOR) return;

//	mchA_d3dEndScene();
	mchA_d3dLockSprite(d3dHandle,&spr_buf,pitch);
	for(i = 0; i < SizeY; i ++){
		memcpy((char*)spr_buf + idx,(char*)p + idx,SizeX * 2); 
		idx += pitch;
	}
	mchA_d3dUnlockSprite(d3dHandle);
//	mchA_d3dBeginScene();
}

void mchA_SpriteSlot::loadSprite(int x,int y,int sx,int sy,void* p,int id)
{
	int i,idx,src_idx = 0;
	spriteID = id;

	unsigned pitch;
	void* spr_buf;

	if(RenderMode == XGRAPH_HICOLOR) return;

//	mchA_d3dEndScene();
	mchA_d3dLockSprite(d3dHandle,&spr_buf,pitch);

	idx = x * 2 + y * pitch;
	sx *= 2;

	for(i = 0; i < SizeY; i ++){
		memcpy((char*)spr_buf + idx,(char*)p + src_idx,sx); 
		idx += pitch;
		src_idx += sx;
	}
	mchA_d3dUnlockSprite(d3dHandle);
//	mchA_d3dBeginScene();
}

mchA_Sprite::mchA_Sprite(char* fname,int id)
{
	ID = id;
	slotID = -1;
	SizeX = SizeY = 0;
	data = NULL;

	fileName = strdup(fname);

	list = NULL;
}

mchA_Sprite::mchA_Sprite(int id)
{
	ID = id;
	slotID = -1;
	slotX = slotY = 0;
	SizeX = SizeY = 0;
	data = NULL;

	fileName = NULL;

	list = NULL;
}

mchA_Sprite::~mchA_Sprite(void)
{
	if(fileName)
		free(fileName);

	if(data)
		delete data;
}

void mchA_Sprite::load(void)
{
	int colors;
	char* p;

	XStream fh;
	if(!fileName) return;

	acsOpenResource(fileName,fh);
	mchLoadTGA(fh,(void**)(&p),SizeX,SizeY,colors);

	data = new char[SizeX * SizeY * 2];

	mchA_d3dConvertSprite(SizeX,SizeY,p,data,colors);
	delete p;

	calc_bound();
}

void mchA_Sprite::calc_bound(void)
{
	int x,y,sz,dx0,dx1,dy0,dy1;
	int idx = 0,idx1 = 0;

	unsigned short* p = (unsigned short*)data;

	dx0 = dx1 = SizeX/2;
	dy0 = dy1 = SizeY/2 * SizeX;

	for(y = 0; y < SizeY; y ++){
		x = 0;
		while(x < SizeX && !p[idx + x]) x ++;
		if(x < dx0) dx0 = x;
		idx += SizeX - 1;

		x = 0;
		while(x < SizeX && !p[idx - x]) x ++;
		if(x < dx1) dx1 = x;
		idx ++;
	}

	idx = 0;
	sz = SizeX * (SizeY - 1);
	for(x = 0; x < SizeX; x ++){
		y = 0;
		while(y <= sz && !p[idx + y]) y += SizeX;
		if(y < dy0) dy0 = y;

		y = 0;
		while(y <= sz && !p[idx - y + sz]) y += SizeX;
		if(y < dy1) dy1 = y;
		idx ++;
	}

	bound[0] = dx0;
	bound[2] = dx1;
	bound[1] = dy0 / SizeX;
	bound[3] = dy1 / SizeX;
}

mchA_SpriteDispatcher::mchA_SpriteDispatcher(void)
{
/*
	mchA_SpriteSlot* p;

	p = new mchA_SpriteSlot(100,mchA_d3dSpriteParent,256,256);
	slotLst.append(p);
	p = new mchA_SpriteSlot(101,mchA_d3dSpriteParent,256,256);
	slotLst.append(p);
*/
}

mchA_SpriteDispatcher::~mchA_SpriteDispatcher(void)
{
	mchA_Sprite* sp;
	mchA_SpriteSlot* slp;

	sp = spriteLst.first();
	while(sp){
		spriteLst.remove(sp);
		delete sp;
		sp = spriteLst.first();
	}

	slp = slotLst.first();
	while(sp){
		slotLst.remove(slp);
		delete slp;
		slp = slotLst.first();
	}
}

void mchA_SpriteDispatcher::AddSlot(int sx,int sy,int id)
{
	mchA_SpriteSlot* p = new mchA_SpriteSlot(id);
	p -> init(sx,sy);

	slotLst.append(p);
}

void mchA_SpriteDispatcher::AddSprite(int id,char* fname)
{
	mchA_Sprite* p = new mchA_Sprite(fname,id);
	p -> load();

	spriteLst.append(p);
}

void mchA_InitSpriteDispatcher(void)
{
	mchA_SprD = new mchA_SpriteDispatcher;
	mchA_SprD -> LoadScript("sprites.scb");
}

void mchA_FinitSpriteDispatcher(void)
{
	delete mchA_SprD;
	mchA_SprD = NULL;
}

void mchA_SpriteDispatcher::DrawSpriteClip(int x,int y,float sx,float sy,float l,float t,float r,float b,int id,int col,int alpha,float angle,int center)
{
	int handle;
	mchA_Sprite* p = spriteLst.search(id);
	if(!p) id = 1;

	handle = GetSlot(id);

	if(RenderMode == XGRAPH_HICOLOR){
		p = spriteLst.search(id);
		if(p)
			XGR_PutSpr16(x,y,p -> SizeX,p -> SizeY,p -> data,XGR_HIDDEN_FON);
		return;
	}

	if(handle != -1){
		mchA_d3dClipSprite(handle,l,t,r,b);
		mchA_d3dDrawSprite(x,y,sx,sy,handle,col,alpha,angle,center);
		mchA_d3dClipSprite(handle);
	}
}

void mchA_SpriteDispatcher::DrawSpriteClipZ(int x,int y,int z,float sx,float sy,float l,float t,float r,float b,int id,int col,int alpha,float angle,int center)
{
	int handle;
	mchA_Sprite* p = spriteLst.search(id);
	if(!p) id = 1;

	handle = GetSlot(id);

	if(RenderMode == XGRAPH_HICOLOR){
		p = spriteLst.search(id);
		if(p)
			XGR_PutSpr16(x,y,p -> SizeX,p -> SizeY,p -> data,XGR_HIDDEN_FON);
		return;
	}

	if(handle != -1){
		mchA_d3dClipSprite(handle,l,t,r,b);
		mchA_d3dDrawSpriteZ(x,y,z,sx,sy,handle,col,alpha,angle,center);
		mchA_d3dClipSprite(handle);
	}
}

void mchA_SpriteDispatcher::DrawSprite(int x,int y,float sx,float sy,int id,int col,int alpha,float angle,int center)
{
	int handle;
	mchA_Sprite* p = spriteLst.search(id);
	if(!p) id = 1;

	handle = GetSlot(id);

	if(RenderMode == XGRAPH_HICOLOR){
		p = spriteLst.search(id);
		if(p)
			XGR_PutSpr16(x,y,p -> SizeX,p -> SizeY,p -> data,XGR_HIDDEN_FON);
		return;
	}

	if(handle != -1)
		mchA_d3dDrawSprite(x,y,sx,sy,handle,col,alpha,angle,center);
}

void mchA_SpriteDispatcher::DrawSpriteZ(int x,int y,int z,float sx,float sy,int id,int col,int alpha,float angle,int center)
{
	int handle;
	mchA_Sprite* p = spriteLst.search(id);
	if(!p) id = 1;

	handle = GetSlot(id);

	if(RenderMode == XGRAPH_HICOLOR){
		p = spriteLst.search(id);
		if(p)
			XGR_PutSpr16(x,y,p -> SizeX,p -> SizeY,p -> data,XGR_HIDDEN_FON);
		return;
	}

	if(handle != -1)
		mchA_d3dDrawSpriteZ(x,y,z,sx,sy,handle,col,alpha,angle,center);
}

int mchA_SpriteDispatcher::GetSlot(int spriteID)
{
	mchA_Sprite* sp;
	mchA_SpriteSlot* p = slotLst.first();
	while(p){
		if(p -> spriteID == spriteID){
			p -> inactiveTimer = 0;
			return p -> d3dHandle;
		}
		p = p -> next;
	}

	sp = spriteLst.search(spriteID);
	if(!sp) 
		return -1;

	if(sp -> slotID != -1){
		p = slotLst.search(sp -> slotID);
		if(p){ 
			if(p -> spriteID != sp -> ID)
				p -> loadSprite(sp -> slotX,sp -> slotY,sp -> SizeX,sp -> SizeY,sp -> data,sp -> ID);

			p -> inactiveTimer = 0;
			return p -> d3dHandle;
		}
		return -1;
	}

	p = slotLst.first();
	while(p){
		if(p -> spriteID == -1 && p -> SizeX == sp -> SizeX && p -> SizeY == sp -> SizeY)
			break;
		p = p -> next;
	}
	if(!p){ 
//		return -1;
		p = slotLst.first();
		while(p){
			if(p -> inactiveTimer > 2 && p -> SizeX == sp -> SizeX && p -> SizeY == sp -> SizeY)
				break;
			p = p -> next;
		}
	}

	if(!p){
		p = slotLst.first();
		while(p -> SizeX != sp -> SizeX || p -> SizeY != sp -> SizeY){
			p = p -> next;
			if(!p) return -1;
		}
	}

	p -> loadSprite(sp -> data,sp -> ID);
	p -> inactiveTimer = 0;
	return p -> d3dHandle;
}

void mchA_SpriteDispatcher::LoadScript(char* fname)
{
	int sx,sy;
	char* str;

	XStream fh;

	mchA_Sprite* spr;

	scrDataBlock* p,*p1;

	acsOpenResource(fname,fh);
	scrDataBlock* root = loadScript(fh);

	p = root -> nextLevel -> first();
	while(p){
		switch(p -> ID){
			case SPRD_SLOT:
				sx = sy = 0;
				p1 = p -> nextLevel -> first();
				while(p1){
					if(p1 -> ID == ASCR_SIZE_X)
						sx = *p1 -> i_dataPtr;
					if(p1 -> ID == ASCR_SIZE_Y)
						sy = *p1 -> i_dataPtr;
					p1 = p1 -> next;
				}
				AddSlot(sx,sy,*p -> i_dataPtr);
				break;
			case SPRD_SPRITE:
				sx = sy = 0;
				str = NULL;

				p1 = p -> nextLevel -> first();
				while(p1){
					if(p1 -> ID == ASCR_FILE_NAME)
						str = p1 -> c_dataPtr;
					p1 = p1 -> next;
				}
				AddSprite(*p -> i_dataPtr,str);

				spr = spriteLst.last();

				p1 = p -> nextLevel -> first();
				while(p1){
					if(p1 -> ID == ASCR_POS_X)
						spr -> slotX = *p1 -> i_dataPtr;
					if(p1 -> ID == ASCR_POS_Y)
						spr -> slotY = *p1 -> i_dataPtr;
					if(p1 -> ID == ASCR_ID)
						spr -> slotID = *p1 -> i_dataPtr;
					p1 = p1 -> next;
				}
				break;
		}
		p = p -> next;
	}
}

void mchA_SpriteDispatcher::DrawArcaneSymbol(int x,int y,int id,float phase,float angle,int color,int alpha,float scale,int center)
{
	int csy,col,col0,col1,alpha1;
	float sx,sy,dy;

	mchA_Sprite* p,*p0;

	p = spriteLst.search(id);
	p0 = spriteLst.search(0);

	if(!p0) return;

	col0 = mchA_d3dMakeColor(160,160,160);
	col1 = mchA_d3dMakeColor(255,255,255);

	col = (color >= 2) ? col1 : col0;
	alpha1 = (color >= 2) ? alpha : alpha - 130;

	sx = sy = 0.78f * scale;

	if(phase != 1.0f){
		dy = ((float)p0 -> bound[1] + (1.0f - phase) * (float)(p0 -> SizeY - p0 -> bound[1] - p0 -> bound[3])) / (float)p0 -> SizeY; 
		csy = round(sy * dy * (float)p0 -> SizeY);
		DrawSpriteClip(x,y + csy,sx,sy,0.0f,dy,1.0f,1.0f,0,col1,alpha,0.0f,center);
	}
	else
		DrawSprite(x,y,sx,sy,0,col1,alpha,0.0f,center);

	if(p)
		DrawSprite(x,y,sx,sy,id,col1,alpha1,angle,center);
	else
		DrawSprite(x,y,sx,sy,1,col1,alpha1,angle,center);
}

void mchA_SpriteDispatcher::DrawArcaneSymbolZ(int x,int y,int z,int id,float phase,float angle,int color,int alpha,float scale,int center)
{
	int csy,col,col0,col1,alpha1;
	float sx,sy,dy;

	mchA_Sprite* p,*p0;

	p = spriteLst.search(id);
	p0 = spriteLst.search(0);

	if(!p0) return;

	col0 = mchA_d3dMakeColor(160,160,160);
	col1 = mchA_d3dMakeColor(255,255,255);

	col = (color >= 2) ? col1 : col0;
	alpha1 = (color >= 2) ? alpha : alpha - 130;

	sx = sy = 0.78f * scale;

	if(phase != 1.0f){
		dy = ((float)p0 -> bound[1] + (1.0f - phase) * (float)(p0 -> SizeY - p0 -> bound[1] - p0 -> bound[3])) / (float)p0 -> SizeY; 
		csy = round(sy * dy * (float)p0 -> SizeY);
		DrawSpriteClipZ(x,y + csy,z,sx,sy,0.0f,dy,1.0f,1.0f,0,col1,alpha,0.0f,center);
	}
	else
		DrawSpriteZ(x,y,z,sx,sy,0,col1,alpha,0.0f,center);

	if(p)
		DrawSpriteZ(x,y,z,sx,sy,id,col1,alpha1,angle,center);
	else
		DrawSpriteZ(x,y,z,sx,sy,1,col1,alpha1,angle,center);
}

void mchA_SpriteDispatcher::DrawEssence(int x,int y,int id,float phase,float angle,int color,int alpha)
{
	int csy,col;
	float sx,sy,dy;

	mchA_Sprite* p,*p0;

	p = spriteLst.search(100 + id);
	p0 = spriteLst.search(200 + id);

	if(!p || !p0) return;

	col = mchA_d3dMakeColor(255,255,255);
	sx = sy = 1.0f;

	if(phase != 1.0f){
		dy = ((float)p0 -> bound[1] + (1.0f - phase) * (float)(p0 -> SizeY - p0 -> bound[1] - p0 -> bound[3])) / (float)p0 -> SizeY; 
		csy = round(sy * dy * (float)p0 -> SizeY);
		DrawSpriteClip(x,y + csy,sx,sy,0.0f,dy,1.0f,1.0f,100 + id,col,alpha);
	}
	else
		DrawSprite(x,y,sx,sy,100 + id,col,alpha);

	DrawSprite(x,y,sx,sy,200 + id,col,alpha,angle);
}

void mchA_SpriteDispatcher::Quant(void)
{
	mchA_SpriteSlot* p = slotLst.first();

	while(p){
		p -> inactiveTimer ++;
		p = p -> next;
	}
}

