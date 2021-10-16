/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "aci_parser.h"
#include "intro_movie.h"
#include "mch_script.h"

#include "sound.h"

#include "PlayMpeg.h"
#include "md3d.h"

#include "iText.h"
#include "hFont.h"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern MpegSound* mpeg_player;

extern cInterfaceGraph3d* gb_IGraph3d;
extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];
extern HFont** acsFntTable;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);
unsigned short* loadJPG(XStream *Xfile, int &IMGwidth, int &IMGheight);

int acsTextStrLenMax(int fnt,unsigned char* str,int space);
int acsTextHeight(int fnt,unsigned char* str,int space);

void mchA_DrawTextWindow(int x,int y,int sx,int sy,int wnd_alpha,int border_col,int border_alpha);

void im_d3dSaveSprite(int spr);
void im_d3dOutSprite(float x,float y,float sx,float sy,int spr,int col,int alpha,float rotate,int center_flag);

/* --------------------------- DEFINITION SECTION --------------------------- */

//const float IMG_SCALE_X = 0.625f;
//const float IMG_SCALE_Y = 0.781f;

const int IMG_PX = 5;
const int IMG_PY = 3;

const float IMG_SCALE_X = 640.0f / (256.0f * float(IMG_PX)) + 0.005f;
const float IMG_SCALE_Y = 480.0f / (256.0f * float(IMG_PY)) + 0.005f;

//#define _INTRO_D3D_DEBUG_

mchIntroMovieDispatcher* mch_introMovieD = NULL;
RandomGenerator im_random_generator;

DWORD im_d3dTexMode = D3DTEXFMT_RGB565;
M3DTEXTUREFORMAT im_d3dTexFmt;
DWORD im_d3dSprite[IMG_PX * IMG_PY * 2];

int mchIntroMovieActive = 0;

mchIntroMovieImage::mchIntroMovieImage(void* scr_p)
{
	scrDataBlock* p = (scrDataBlock*)scr_p,*p1;

	ID = -1;
	list = NULL;

	scale = 1.0f;

	SX = SY = 0;
	buf = NULL;
	fname = NULL;

	ID = *p -> i_dataPtr;
	p1 = p -> find(MCH_SCR_FILE);
	if(p1) set_file(p1 -> c_dataPtr);
}

mchIntroMovieImage::~mchIntroMovieImage(void)
{
	free_image();
}

int mchIntroMovieImage::size_x(void)
{ 
	return round(mchA_d3dResX * scale * IMG_SCALE_X * 256.0f * float(IMG_PX));
}

int mchIntroMovieImage::size_y(void)
{ 
	return round(mchA_d3dResY * scale * IMG_SCALE_Y * 256.0f * float(IMG_PY));
}

void mchIntroMovieImage::draw(int x,int y,int alpha)
{
	int i,j;
	float xx,yy;

	float scale_x = IMG_SCALE_X * scale * mchA_d3dResX;
	float scale_y = IMG_SCALE_Y * scale * mchA_d3dResY;

	for(j = 0; j < IMG_PY; j ++){
		for(i = 0; i < IMG_PX; i ++){
			xx = float(i << 8) * scale_x;
			yy = float(j << 8) * scale_y;
			im_d3dOutSprite(float(x) + xx,float(y) + yy,scale_x,scale_y,im_d3dSprite[i + j * IMG_PX + slot * IMG_PX * IMG_PY],0xFFFFFF,alpha,0,0);
		}
	}
}

void mchIntroMovieImage::load_image(int sl)
{
	XStream fh;

	slot = sl;

	free_image();
	acsOpenResource(fname,fh);

	buf = loadJPG(&fh,SX,SY);

	fh.close();

	DWORD pitch;
	void* spr_buf;

	unsigned short* uspr_buf;

	int i,j,x,y,idx,idx0;
	unsigned col0,col,r,g,b;

	for(i = 0; i < IMG_PY; i ++){
		for(j = 0; j < IMG_PX; j ++){
			d3dLockSprite(im_d3dSprite[j + i * IMG_PX + slot * IMG_PX * IMG_PY],&spr_buf,&pitch);
			uspr_buf = (unsigned short*)spr_buf;

			idx = 0;
			idx0 = j * 256 + i * 256 * SX;
			for(y = 0; y < 256; y ++){
				for(x = 0; x < 256; x ++){
					col0 = buf[idx0 + x];		
					r = XGR_64KR(col0);
					g = XGR_64KG(col0);
					b = XGR_64KB(col0);

					if(im_d3dTexMode == D3DTEXFMT_RGB555) g >>= 1;

					col = (r << im_d3dTexFmt.dwRBitShift) | (g << im_d3dTexFmt.dwGBitShift) | (b << im_d3dTexFmt.dwBBitShift);
					uspr_buf[idx + x] = col;
				}
				idx += 256;
				idx0 += SX;
			}
			d3dUnlockSprite(im_d3dSprite[j + i * IMG_PX + slot * IMG_PX * IMG_PY]);
		}
	}

	free_image();
}

void mchIntroMovieImage::free_image(void)
{
	if(buf) delete buf;
	buf = NULL;
	SX = SY = 0;
}

void mchIntroMovieImage::clip(Vect2f& coords)
{
	int minx,miny;

	minx = XGR_MAXX - size_x();
	miny = XGR_MAXY - size_y();

	if(coords.x < minx) coords.x = minx;
	if(coords.y < miny) coords.y = miny;
	if(coords.x > 0) coords.x = 0;
	if(coords.y > 0) coords.y = 0;
}

void mchIntroMovieImage::set_file(char* fn)
{
	if(fname) free(fname);
	fname = strdup(fn);
}

mchIntroMovieAction::mchIntroMovieAction(void* scr_p)
{
	int id = 0,cid = 0,sid = 0;
	scrDataBlock* p = (scrDataBlock*)scr_p,*p1;

	ID = -1;
	list = NULL;
	duration = cur_time = 0;
	prev_coords = -1;

	alpha = 0;

	slot = 0;
	scale[0] = scale[1] = 0;

	coords[0] = Vect2f(0,0);
	coords[1] = Vect2f(0,0);

	Type = *p -> i_dataPtr;

	if(p -> nextLevel){
		p1 = p -> nextLevel -> first();
		while(p1){
			switch(p1 -> ID){
				case MCH_SCR_TIME:
					duration = p1 -> i_dataPtr[0];
					break;
				case MCH_SCR_SLOT:
					slot = p1 -> i_dataPtr[0];
					break;
				case MCH_SCR_ALPHA:
					alpha = p1 -> i_dataPtr[0];
					break;
				case MCH_SCR_IMAGE:
					img_ids[id ++] = p1 -> i_dataPtr[0];
					break;
				case MCH_SCR_SCALE:
					scale[sid ++] = p1 -> d_dataPtr[0];
					break;
				case MCH_SCR_PREV_COORDS:
					prev_coords = p1 -> i_dataPtr[0];
					break;
				case MCH_SCR_COORDS:
					coords[cid].x = float(p1 -> i_dataPtr[0]);
					coords[cid].y = float(p1 -> i_dataPtr[1]);
					cid ++;
					break;
				case MCH_SCR_ID:
					ID = p1 -> i_dataPtr[0];
					break;
			}
			p1 = p1 -> next;
		}
	}
}

mchIntroMovieAction::~mchIntroMovieAction(void)
{
}

void mchIntroMovieAction::start(void)
{
	mchIntroMovieImage* p;

	cur_time = 0;

	if(prev_coords != -1){
		if(prev_coords & 0x01)
			coords[0] = prev -> coords[0];
		if(prev_coords & 0x02)
			coords[1] = prev -> coords[1];
	}

	switch(Type){
		case ia_LOAD:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> load_image(slot);
			break;
		case ia_FREE:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> free_image();
			break;
		case ia_SET_SCALE:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> set_scale(scale[0]);
			break;
	}
}

void mchIntroMovieAction::quant(int dt)
{
	Vect2f dr;
	mchIntroMovieImage* p;

	cur_time += dt;

	if(Type == ia_RND_SCROLL){
		p = owner -> get_image(img_ids[0]);
		if(!p) return;

		dr = coords[1] * float(dt) / 1000.0f;

		if(coords[0].x < 0.0f && coords[0].x + dr.x > 0.0f)
			coords[1].x = -coords[1].x;

		if(coords[0].x > XGR_MAXX - p -> size_x() && coords[0].x + dr.x < XGR_MAXX - p -> size_x())
			coords[1].x = -coords[1].x;

		if(coords[0].y < 0.0f && coords[0].y + dr.y > 0.0f)
			coords[1].y = -coords[1].y;

		if(coords[0].y > XGR_MAXY - p -> size_y() && coords[0].y + dr.y < XGR_MAXY - p -> size_y())
			coords[1].y = -coords[1].y;

		coords[0] += dr;

		if(coords[0].x > 0.0f) coords[0].x = -0.01f;
		if(coords[0].x < XGR_MAXX - p -> size_x()) coords[0].x = XGR_MAXX - p -> size_x();

		if(coords[0].y > 0.0f) coords[0].y = -0.01f;
		if(coords[0].y < XGR_MAXY - p -> size_y()) coords[0].y = XGR_MAXY - p -> size_y();
	}
}

void mchIntroMovieAction::draw(void)
{
	float sc;
	int v,x,y;
	mchIntroMovieImage* p,*p1;

	switch(Type){
		case ia_SCROLL:
			p = owner -> get_image(img_ids[0]);
			if(p){ 
				x = round(coords[0].x + (coords[1].x - coords[0].x) * float(cur_time) / float(duration));
				y = round(coords[0].y + (coords[1].y - coords[0].y) * float(cur_time) / float(duration));
				p -> draw(x,y);
			}
			break;
		case ia_SCALE:
			sc = scale[0] + (scale[1] - scale[0]) * float(cur_time) / float(duration);
			p = owner -> get_image(img_ids[0]);
			if(p){
				p -> set_scale(sc);
				x = round(coords[0].x + (coords[1].x - coords[0].x) * float(cur_time) / float(duration));
				y = round(coords[0].y + (coords[1].y - coords[0].y) * float(cur_time) / float(duration));
				p -> draw(x,y);
			}
			break;
		case ia_DRAW:
		case ia_RND_SCROLL:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> draw(coords[0].x,coords[0].y);

			if(cur_time < alpha){
				sc = 1.0f + float(cur_time) / float(alpha);
				v = 255 - round(255.0f * float(cur_time) / float(alpha));
				if(v < 0) v = 0;
				if(v > 255) v = 255;

				p = owner -> get_image(img_ids[1]);
				if(p){ 
					p -> set_scale(sc);
					x = (XGR_MAXX - p -> size_x()) / 2;
					y = (XGR_MAXY - p -> size_y()) / 2;
					p -> draw(x,y,v);
				}
			}
			break;
		case ia_ALPHA_SHOW:
			p = owner -> get_image(img_ids[0]);
			p1 = owner -> get_image(img_ids[1]);
			if(!p || !p1) break;

			v = round(255.0f * float(cur_time) / float(duration));
			p -> draw(0,0);
			p1 -> draw(0,0,v);
			break;
		case ia_SHOW:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> draw(coords[0].x,coords[0].y);

			v = 255 - round(float(cur_time) * 255.0f / float(duration));
			if(v < 0) v = 0;

			gb_IGraph3d -> SetViewColor(0,0,0,v);
			break;
		case ia_HIDE:
			p = owner -> get_image(img_ids[0]);
			if(p) p -> draw(coords[0].x,coords[0].y);

			v = round(float(cur_time) * 255.0f / float(duration));
			if(v > 255) v = 255;

			gb_IGraph3d -> SetViewColor(0,0,0,v);
			break;
	}
}

mchIntroMovieDispatcher::mchIntroMovieDispatcher(void)
{
	curAcn = NULL;
	curTxt = NULL;
}

void mchIntroMovieDispatcher::init(void)
{
	scrDataBlock* root,*p;

	mchIntroMovieImage* img;
	mchIntroMovieAction* acn;

	root = loadScript("RESOURCE\\intro_movie.scb");
	p = root -> nextLevel -> first();
	while(p){
		switch(p -> ID){
			case MCH_SCR_IMAGE:
				img = new mchIntroMovieImage(p);
				imgList.append(img);
				break;
			case MCH_SCR_ACTION:
				acn = new mchIntroMovieAction(p);
				acnList.append(acn);
				acn -> set_owner(this);
				break;
		}
		p = p -> next;
	}

	init_texts();

	delete root;

	int i;
	if(d3dGetTextureFormatData(im_d3dTexMode,&im_d3dTexFmt)){
		im_d3dTexMode = D3DTEXFMT_RGB555;
		if(d3dGetTextureFormatData(im_d3dTexMode,&im_d3dTexFmt))
			ErrH.Abort("Error: Texture format not supported");
	}

	for(i = 0; i < IMG_PX * IMG_PY * 2; i ++)
		d3dCreateSprite(256,256,im_d3dTexMode,MD3DSP_USEALPHABLEND | MD3DSP_USEALPHATEST,&im_d3dSprite[i]);
}

void mchIntroMovieDispatcher::text_quant(int dt)
{
	mchIntroMovieText* p;

	if(!curTxt){
		if(!curAcn || curAcn -> ID == -1) return;

		p = txtList.first();
		while(p){
			if(p -> action() == curAcn -> ID && p -> start_time() >= curAcn -> time()){
				p -> start();
				curTxt = p;

				return;
			}
			p = p -> next;
		}
	}
	else
		curTxt -> quant(dt);
}

mchIntroMovieDispatcher::~mchIntroMovieDispatcher(void)
{
	mchIntroMovieImage* img;
	mchIntroMovieAction* acn;
	mchIntroMovieText* txt;

	img = imgList.first();
	while(img){
		imgList.remove(img);
		delete img;
		img = imgList.first();
	}

	acn = acnList.first();
	while(acn){
		acnList.remove(acn);
		delete acn;
		acn = acnList.first();
	}

	txt = txtList.first();
	while(txt){
		txtList.remove(txt);
		delete txt;
		txt = txtList.first();
	}

	int i;
	for(i = 0; i < IMG_PX * IMG_PY * 2; i ++)
		d3dDeleteSprite(im_d3dSprite[i]);
}

void mchIntroMovieDispatcher::start(void)
{
	curTxt = NULL;
	curAcn = acnList.first();

	if(curAcn){ 
		curAcn -> start();
		while(curAcn -> is_finished()){
			curAcn = curAcn -> next;
			if(curAcn) curAcn -> start();
			else break;
		}
	}

	timer = clocki();
}

int mchIntroMovieDispatcher::quant(void)
{
	int dt = clocki() - timer;
	timer = clocki();

	if(!curAcn) return 1;

	gb_IGraph3d -> BeginScene();

	text_quant(dt);
	curAcn -> quant(dt);
	curAcn -> draw();

	if(curTxt){ 
		curTxt -> draw();

		if(curTxt -> is_finished())
			curTxt = NULL;
	}

	while(curAcn -> is_finished()){
		curAcn = curAcn -> next;
		if(curAcn){ 
			curAcn -> start();
			timer = clocki();
		}
		else 
			break;
	}

	gb_IGraph3d -> EndScene();

	return 0;
}

void mchIntroMovieDispatcher::set_time(void)
{
	timer = clocki();
}

void mchIntroMovieDispatcher::init_texts(void)
{
	int i,id,tm,len,len2;
	mchIntroMovieText* p,*p1;
	mchIntroMovieAction* ac;

	XBuffer mpeg_name(MAX_PATH);
	const int text_delay = 0;

	p = NULL;
	for(i = 0; i < 7; i ++){
		id = 0;
		init_text(iGetText(iTXT_INTRO_SCENE1 + i));
		if(!p) p = txtList.first();
		else p = p -> next;

		tm = 1000;
		p1 = p;
		if (!mpeg_player) {
			mpeg_player = new MpegSound();
		}
		while(p1){
			mpeg_name.init();
			mpeg_name < "Resource\\iScreen\\Intro_Movie\\Sound\\intro_" <= i + 1 < "_" <= id + 1 < ".mp+";
			int volume_temp = mpeg_player->GetVolume();
			mpeg_player->SetVolume(0);
			mpeg_player->OpenToPlay(mpeg_name, 0);
			mpeg_player->Stop();
			mpeg_player->SetVolume(volume_temp);
			len = mpeg_player->GetLen();

			if(len != -1)
				len = len * 1000 / 44100;
			else
				len = 8000;

			p1 -> set_mpeg_ids(i + 1,id + 1);

			if(p1 -> next){ 
				p1 -> set_action(im_SCENE1 + i);
				p1 -> set_start_time(tm);
				p1 -> set_duration(text_delay + len);

				tm += text_delay + len;
			}
			else {
				p1 -> set_action(im_SCENE1 + i + 10);
				p1 -> set_duration(text_delay + len);

				ac = acnList.search(im_SCENE1 + i + 10);

				if(ac){ 
					len2 = -ac -> total_time() + len + text_delay;
					ac = ac -> next;
				}

				if(ac){
					if(ac -> total_time() < len2)
						ac -> set_duration(len2);
				}
			}

			id ++;
			p1 = p1 -> next;
		}
		ac = acnList.search(im_SCENE1 + i);
		if(ac) ac -> set_duration(tm);

		p = txtList.last();
	}
}

int mchIntroMovieDispatcher::skip(void)
{
	int id;
	mchIntroMovieAction* p = curAcn;

	if(!p) return 0;

	while(p -> ID == -1) p = p -> prev;
	id = p -> ID;
	if(id >= 20) id -= 20;
	if(id >= 10) id -= 10;

	p = acnList.search(id + 1 + 20);
	if(!p) return 1;

	if (!mpeg_player) {
		mpeg_player = new MpegSound();
	}

	if(curTxt){
		if(!mchSoundMute)
			mpeg_player->Stop();
		curTxt = NULL;
	}

	curAcn = p;

	curAcn -> start();
	while(curAcn -> is_finished()){
		curAcn = curAcn -> next;
		if(curAcn) curAcn -> start();
		else break;
	}

	return 0;
}

void mchIntroMovieDispatcher::init_text(char* txt)
{
	int i,sz,beg_idx;
	mchIntroMovieText* p;

	i = 0;
	beg_idx = 0;
	sz = strlen(txt);
	while(i < sz){
		if(txt[i] == '\n' && txt[i + 1] == '\r'){
			p = new mchIntroMovieText;
			p -> set_text(txt + beg_idx,i - beg_idx);

			txtList.append(p);
			p -> set_owner(this);

			while(txt[i] == '\r' || txt[i] == '\n'){ 
				if(++i >= sz) break;
			}
			beg_idx = i;
		}
		else 
			i ++;
	}
	if(sz - beg_idx > 2){
		p = new mchIntroMovieText;
		p -> set_text(txt + beg_idx,sz - beg_idx);

		txtList.append(p);
		p -> set_owner(this);
	}
}

mchIntroMovieText::mchIntroMovieText(void)
{
	ID = -1;
	list = NULL;

	buf = NULL;
	action_ID = -1;

	start_time_ = cur_time = duration = 0;

	owner = NULL;
}

mchIntroMovieText::~mchIntroMovieText(void)
{
	if(buf) delete buf;
}

void mchIntroMovieText::set_text(const char* txt,int txt_len)
{
	int i,sz;
	
	sz = (txt_len == -1) ? strlen(txt) + 1 : txt_len + 1;

	buf = new char[sz];
	buf[sz - 1] = 0;

	memcpy(buf,txt,sz - 1);

	for(i = sz - 2; i >= 0; i --){
		if(buf[i] == '\r' || buf[i] == '\n') buf[i] = 0;
		else break;
	}
}

void mchIntroMovieText::start(void)
{ 
	XBuffer mpeg_name(MAX_PATH);

	cur_time = 0; 

	if (!mpeg_player) {
		mpeg_player = new MpegSound();
	}

	mpeg_name < "Resource\\iScreen\\Intro_Movie\\Sound\\intro_" <= mpeg_actionID < "_" <= mpeg_phraseID < ".mp+";
	if(!mchSoundMute)
		mpeg_player->OpenToPlay(mpeg_name,0);
}

void mchIntroMovieText::draw(void)
{
	int i,idx,x,y,sx,sy,sz,alpha = 255;

	static char str[256];

	const int alpha_time = 300;
	const int w_alpha = 128;

	if(cur_time < alpha_time)
		alpha = round(255.0f * float(cur_time) / float(alpha_time));
	if(cur_time > duration - alpha_time)
		alpha = round(255.0f * float(duration - cur_time) / float(alpha_time));

	if(alpha > 255) alpha = 255;
	if(alpha < 0) alpha = 0;

	sx = acsTextStrLenMax(2,(unsigned char*)buf,0);
	sy = acsTextHeight(2,(unsigned char*)buf,-5);

	x = (640 - sx) / 2;
	y = 480 - sy - 10;

	mchA_DrawTextWindow(0,y - 5,640,sy + 16,round(float(w_alpha) * float(alpha) / 255.0f),0,0);
//	mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],buf,mchA_ColorF[7],alpha,2,0,1.0f,1,-6);

	idx = 0;
	str[0] = 0;
	sz = strlen(buf);
	for(i = 0; i < sz; i ++){
		if(buf[i] == '\n'){
			str[idx] = 0;
			sx = acsTextStrLenMax(2,(unsigned char*)str,0);
			x = (640 - sx) / 2;
			mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],str,mchA_ColorF[7],alpha,2,0,1.0f,1);
			y += acsFntTable[2] -> SizeY - 5;
			idx = 0;
		}
		else {
			if(buf[i] != '\r')
				str[idx ++] = buf[i];
		}
	}
	str[idx] = 0;
	sx = acsTextStrLenMax(2,(unsigned char*)str,0);
	x = (640 - sx) / 2;
	mchA_d3dOutString(x,y,mchA_FontScaleX[2],mchA_FontScaleY[2],str,mchA_ColorF[7],alpha,2,0,1.0f,1);
}

void im_d3dOutSprite(float x,float y,float sx,float sy,int spr,int col,int alpha,float rotate,int center_flag)
{
	int mode = (center_flag) ? MD3DORG_CENTER : MD3DORG_TOPLEFT;

	d3dSetSpriteMode(spr,MD3DSP_COLORFACTOR,col);
	d3dSetSpriteMode(spr,MD3DSP_ALPHAFACTOR,alpha);
	d3dSetSpriteMode(spr,MD3DSP_ALPHAREF,0);

	d3dSetTextureStageState(0,D3DTSS_ADDRESS,D3DTADDRESS_CLAMP);
	d3dDrawSprite(spr,(float)x,(float)y,mode,sx,sy,rotate);
	d3dSetTextureStageState(0,D3DTSS_ADDRESS,D3DTADDRESS_WRAP);
}

void im_d3dSaveSprite(int spr)
{
#ifdef _INTRO_D3D_DEBUG_
	DWORD pitch;
	void* spr_buf;
	XBuffer XBuf;
	XStream fh;
	int x,y,_idx = 0,idx0 = 0;
	unsigned cl;
	char buf[256 * 256];

	d3dLockSprite(im_d3dSprite[spr],&spr_buf,&pitch);

	for(y = 0; y < 256; y ++){
		for(x = 0; x < 256; x ++){
			cl = *((unsigned short*)((char*)spr_buf + idx0 + x * 2));
			buf[_idx + x] = (((cl & im_d3dTexFmt.dwRBitMask) >> im_d3dTexFmt.dwRBitShift) + ((cl & im_d3dTexFmt.dwGBitMask) >> im_d3dTexFmt.dwGBitShift)/2 + ((cl & im_d3dTexFmt.dwBBitMask) >> im_d3dTexFmt.dwBBitShift))/3;
		}
		_idx += 256;
		idx0 += pitch;
	}

	XBuf < "\\a\\spr00" <= spr < ".bmp";
	fh.open(XBuf,XS_OUT);
	fh < (short)256 < (short)256;
	fh.write(buf,256 * 256);
	fh.close();

	d3dUnlockSprite(im_d3dSprite[spr]);
#endif
}
