
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "TrackRecorder.h"

#include "race.h"
#include "TERRA.H"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern mchArcaneBMP** mchA_ShapeBMP;
extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];

extern int mchFreezeTime;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int mchA_ConvertCoords(int x,int y,int z,int& sx,int& sy,int viewport = 0);
int acsStrLen(int fnt,unsigned char* str,int space);
int acsStrHeight(int fnt,unsigned char* str);

void mchA_CHQuant(int k);

/* --------------------------- DEFINITION SECTION --------------------------- */

#ifndef _FINAL_VERSION_
#define _TR_ENABLE_
#endif

int trkEnableFlag = 0;
trkRecorder* mch_trkRec = NULL;

trkBranch::~trkBranch(void)
{
	trkPoint* p;

	p = points.first();
	while(p){
		points.remove(p);
		delete p;
		p = points.first();
	}
}

trkRecorder::trkRecorder(void)
{
	mode = 0;
	enabled = 0;
	curBranch = NULL;
	curPoint = NULL;
}

trkRecorder::~trkRecorder(void)
{
	clear();
}

void trkRecorder::clear(void)
{
	trkBranch* p;

	p = branches.first();
	while(p){
		branches.remove(p);
		delete p;
		p = branches.first();
	}

	curBranch = NULL;
	curPoint = NULL;
}

void trkRecorder::alloc(int sz)
{
	int i;
	trkBranch* p;

	clear();
	for(i = 0; i < sz; i ++){
		p = new trkBranch;
		p -> ID = i;

		branches.append(p);
	}
}

void trkRecorder::RecPrepare(int cp)
{
	int sz = 0;
	trkBranch* p = branches.first(),*p0;
	mchCheckpoint* chp = mch_raceD -> getCP(cp);

	cp = chp -> ID;

	p0 = NULL;
	while(p){
		if(p -> ID == cp){
			sz ++;
			if(!p -> points.size()) p0 = p;
		}
		p = p -> next;
	}
	if(!p0){
		p0 = new trkBranch;
		p0 -> ID = cp;
		p0 -> sID = sz;

		branches.append(p0);
	}

	mode = 1;
	curBranch = p0;
	curPoint = NULL;
}

void trkRecorder::RecQuant(Vect3f r,int cp)
{
	float dist;
	Vect3f dr;
	mchCheckpoint* p;

	const float dist0 = 40.0f;

	if(!mode) return;

	p = mch_raceD -> getCP(cp);

	if(p -> ID == mch_raceD -> nextCp(curBranch -> ID)){
		mode = 0;
		return;
	}

	if(p -> ID != curBranch -> ID)
		return;

	if(!curBranch -> points.size()){
		curBranch -> insert(r);
	}
	else {
		dr = getDist(curBranch -> points.last() -> R(),r);
		dist = dr.norm();

		if(dist > dist0 || fabs(dist - dist0) < 1.0f)
			curBranch -> insert(r);
	}
}

void trkBranch::insert(Vect3f r)
{
	trkPoint* p = new trkPoint(r);
	points.append(p);
}

void trkRecorder::Redraw(void)
{
	int i,sx,sy,col,dx,dy;
	mchCheckpoint* p;

	const int x = 5;
	const int y = 430;
	const int sz = 25;
	const int rec_sz = 8;
	const int sp = 2;

	const float sc = 0.7f;

	static XBuffer str;

	if(!enabled || RenderMode != DIRECT3D_HICOLOR) return;

	p = mch_raceD -> getCP(mch_raceD -> activeRacer -> stPtr -> Checkpoint);
	if(!p) return;

	sx = mch_raceD -> cpLst -> size() * (sz + sp) + sp * 2;
	sy = 45;

	mchA_d3dOutSprite(x,y,float(sx) / 16.0f,float(sy) / 16.0f,AE_D3DSPR_DUMMY,0,250);

	for(i = 0; i < mch_raceD -> cpLst -> size(); i ++){
		col = (branches.search(i)) ? 2 : 0;
		mchA_d3dOutSprite(x + sp + (sz + sp) * i,y + sp,float(sz) / float(mchA_ShapeBMP[2] -> SizeX),float(sz) / float(mchA_ShapeBMP[2] -> SizeX),AE_D3DSPR_SQUARE,mchA_ColorF[col],250);

		str.init();
		str <= i;

		dx = (sz - acsStrLen(2,(unsigned char*)str.address(),0)) / 2;

		dx += dx - round(float(dx) * sc);
		dy = 6;

		col = (i == p -> ID) ? 2 : 0;
		mchA_d3dOutString(x + sp + (sz + sp) * i + dx,y + sp + dy,mchA_FontScaleX[2],mchA_FontScaleY[2],(unsigned char*)str.address(),mchA_ColorF[col],250,2,0,sc);
	}

	if(mode)
		mchA_d3dOutSprite(x + sp,y + sy - sp - rec_sz,float(rec_sz) / 16.0f,float(rec_sz) / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[4],250);
	
	if(curBranch)
		curBranch -> Draw();

	if(curPoint)
		curPoint -> Draw(1);
}

void trkRecorder::KeyTrap(int& k)
{
#ifdef _TR_ENABLE_	
	int cp,id,num;
	trkBranch* p;

	Vect3f dr;

	if(!enabled)
		return;

	switch(k){
		case VK_BACK:
			clear();
			k = 0;
			break;
		case VK_TAB:
			if(!mode){
				if(!XKey.Pressed(VK_SHIFT)){
					if(!curBranch)
						curBranch = branches.first();
					else
						curBranch = curBranch -> next;
				}
				else {
					if(!curBranch)
						curBranch = branches.first();
					else
						curBranch = curBranch -> prev;
				}

				if(!curBranch)
					curBranch = branches.first();

				if(curBranch)
					mch_raceD -> put2checkpoint(NULL,mch_raceD -> prevCp(curBranch -> ID));

				curPoint = NULL;
			}
			k = 0;
			break;
		case VK_DELETE:
			if(curBranch){
				num = 0;
				id = curBranch -> ID;
				branches.remove(curBranch);
				delete curBranch;
				curBranch = NULL;

				p = branches.first();
				while(p){
					if(p -> ID == id)
						p -> sID = num ++;
					p = p -> next;
				}
			}
			curPoint = NULL;
			mode = 0;
			k = 0;
			break;
		case VK_INSERT:
			if(!mode){
				cp = mch_raceD -> activeRacer -> stPtr-> Checkpoint;
				RecPrepare(cp);
			}
			k = 0;
			break;
		case VK_RETURN:
			if(mode){
				mode = 0;
				branches.remove(curBranch);
				delete curBranch;
				curBranch = NULL;
				curPoint = NULL;
			}
			save(GetTargetName("track.btr"));
			k = 0;

			if(XKey.Pressed(VK_CONTROL)){
				mchA_CHQuant('R');
				mchA_CHQuant('E');
				mchA_CHQuant('S');
				mchA_CHQuant('T');
				mchA_CHQuant('A');
				mchA_CHQuant('R');
				mchA_CHQuant('T');
			}

			break;
		case VK_HOME:
			mode = 0;
			load(GetTargetName("track.btr"));
			k = 0;
			break;
		case VK_SPACE:
			if(curPoint) curPoint -> toggle_flag(TRK_REMOVE); 
			k = 0;
			break;
		case VK_LEFT:
			if(XKey.Pressed(VK_CONTROL)){
				mch_raceD -> activeRacer -> setPosition(mch_raceD -> activeRacer -> R(),mch_raceD -> activeRacer -> psi() + 0.05f);
				k = 0;
			}
			break;
		case VK_RIGHT:
			if(XKey.Pressed(VK_CONTROL)){
				mch_raceD -> activeRacer -> setPosition(mch_raceD -> activeRacer -> R(),mch_raceD -> activeRacer -> psi() - 0.05f);
				k = 0;
			}
			break;
		case VK_UP:
			if(XKey.Pressed(VK_SHIFT)){
				if(curBranch){
					if(!curPoint) curPoint = curBranch -> last();

					if(!curPoint) break;

					curPoint = curPoint -> next;
					if(!curPoint) curPoint = curBranch -> first();
				}
				k = 0;
			}
			if(XKey.Pressed(VK_CONTROL)){
				dr = Vect3f(10.0f * sin(mch_raceD -> activeRacer -> psi()),10.0f * cos(mch_raceD -> activeRacer -> psi()),0.0f);
				mch_raceD -> activeRacer -> setPosition(mch_raceD -> activeRacer -> R() + dr,mch_raceD -> activeRacer -> psi());
				k = 0;
			}
			break;
		case VK_DOWN:
			if(XKey.Pressed(VK_SHIFT)){
				if(curBranch){
					if(!curPoint) curPoint = curBranch -> first();

					curPoint = curPoint -> prev;
				}
				k = 0;
			}
			break;
		default:
			if(k >= '0' && k <= '9'){
				if(XKey.Pressed(VK_CONTROL)){
					mch_raceD -> put2checkpoint(NULL,mch_raceD -> prevCp(k - '0'));
					k = 0;
				}
			}
			break;
	}
#endif
}

void trkPoint::Draw(int sel) const 
{
	int x,y,z,col;

	col = (flags & TRK_REMOVE) ? 5 : 2;

	z = mchA_ConvertCoords(R_.xi(),R_.yi(),R_.zi() + 20,x,y);
	mchA_d3dOutSpriteZ(x,y,z,2.0f / 16.0f,8.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],200,0.0f,1);
	mchA_d3dOutSpriteZ(x,y,z,8.0f / 16.0f,2.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[col],200,0.0f,1);

	if(sel){
		mchA_d3dOutSpriteZ(x,y,z,16.0f / 16.0f,16.0f / 16.0f,AE_D3DSPR_DUMMY,mchA_ColorF[5],200,M_PI/4.0f,1);
	}
}

void trkBranch::Draw(void)
{
	trkPoint* p = points.first();

	while(p){
		p -> Draw();
		p = p -> next;
	}
}

void trkInitRecorder(void)
{
#ifdef _TR_ENABLE_	
	mch_trkRec = new trkRecorder;
	mch_trkRec -> Toggle(trkEnableFlag);
#endif	
}

void trkFinitRecorder(void)
{
	if(mch_trkRec)
		delete mch_trkRec;

	mch_trkRec = NULL;
}

const int trkSaveVersion = 2;
void trkRecorder::save(char* fname)
{
	trkBranch* p;
	XStream fh(fname,XS_OUT);

	fh < trkSaveVersion < branches.size();

	p = branches.first();
	while(p){
		p -> save(fh);
		p = p -> next;
	}
}

void trkRecorder::load(char* fname)
{
	int i,v,sz;
	trkBranch* p;

	XStream fh(0);

	clear();
	
	if(!fh.open(fname,XS_IN))
		return;

	fh > v > sz;

	for(i = 0; i < sz; i ++){
		p = new trkBranch;
		p -> load(fh,v);

		branches.append(p);
	}
}

void trkBranch::save(XStream& fh)
{
	trkPoint* p;
	fh < ID < sID < points.size();

	p = points.first();
	while(p){
		fh < p -> R().xi();
		fh < p -> R().yi();
		fh < p -> R().zi();
		fh < p -> get_flags();
		p = p -> next;
	}
}

void trkBranch::load(XStream& fh,int version)
{
	int i,sz,x,y,z,fl;
	trkPoint* p;
	fh > ID > sID > sz;

	if(version == 1){
		for(i = 0; i < sz; i ++){
			fh > x > y > z;
			p = new trkPoint(Vect3f(x,y,z));
			points.append(p);
		}
	}
	else {
		for(i = 0; i < sz; i ++){
			fh > x > y > z > fl;
			p = new trkPoint(Vect3f(x,y,z),fl);
			points.append(p);
		}
	}
}

void trkRecQuant(void)
{
#ifdef _TR_ENABLE_	
	if(trkEnableFlag){
		mch_trkRec -> RecQuant(mch_raceD -> activeRacer -> R(),mch_raceD -> activeRacer -> stPtr -> Checkpoint);

		if(mchFreezeTime){
			mch_raceD -> CP_Quant();
			mch_raceD -> activeRacer -> Quant();
		}
	}
#endif
}
