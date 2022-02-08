
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "xmath.h"
#include "intro.h"

#include "IGraph3d.h"

#include "HFONT.H"
#include "hbm.h"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "SpriteDispatcher.h"

#include "sound.h"
#include "sound_api.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */

extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];

extern HFont** acsFntTable;

extern float mchA_d3dResX;
extern float mchA_d3dResY;

extern cInterfaceGraph3d	*gb_IGraph3d;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);
void mchScreenShot(int mode = 0);

/* --------------------------- DEFINITION SECTION --------------------------- */

//#define _NO_RND_MOVEMENT_

#ifndef _FINAL_VERSION_
//#define _FRAME_CLOCK_

#ifdef _MAKE_INTRO_MOVIE_
#ifndef _FRAME_CLOCK_
#define _FRAME_CLOCK_
#endif
#endif

#endif

const int INTRO_FNT		= 2;
const float INTRO_SCALE_X	= 0.7f;
const float INTRO_SCALE_Y	= 0.35f;
const int INTRO_SP		= -5;
const int INTRO_VSP		= -2;

const float INTRO_MAX_Z		= 100.0f;

int intro_ShadowCol = 0;

mchIntroDispatcher* mch_introD;

void mchInitIntro(void)
{
	int i,sz;
	short sx,sy;

	unsigned char* tp;
	unsigned char* mask;
	hbm_Image* bp;

	XStream fh;

	acsOpenResource("INTRO\\logo.hbm",fh);

	bp = new hbm_Image;
	bp -> load(fh,0);

	sz = bp -> SizeX * bp -> SizeY;
	tp = new unsigned char[sz];

	for(i = 0; i < sz; i ++)
		tp[i] = 'A' + XRnd('Z' - 'A');

	acsOpenResource("INTRO\\mask.bmp",fh);
	fh > sx > sy;

	mask = new unsigned char[sx * sy];
	fh.read(mask,sx * sy);
	fh.close();

	mch_introD = new mchIntroDispatcher;
	mch_introD -> Init(bp -> SizeX,bp -> SizeY,tp,bp -> frameTable[0] -> dataPtr,mask);

	delete tp;
	delete bp;
	delete mask;

	mchLoadIntroSound();
}

void mchFinitIntro(void)
{
	delete mch_introD;

	mchFreeWorldSound();
}

int mchIntroChar::isMoving(void)
{
	if(dest_phase > 0.99f) return 1;

	return 0;
}

void mchIntroChar::Init(int ch,int col,Vect2f r0,Vect2f r1)
{
	chr = ch;
	color = col;

	R0 = r0;
	R1 = r0;
	destR = r1;

	dR = Vect2f(0,0);

	phase = dest_phase = 0.0f;

	angle = 0.0f;
	angle_delta = float(XRnd(round(M_PI*20.0f))) / 10.0f;
	if(!XRnd(2)) angle_delta = -angle_delta;

	flags = 0;
	soundID = -1;
}

void mchIntroChar::Redraw(int shadow_flag)
{
	int x,y,col;
	float ssx,ssy,ang,sc;

	const float max_sc = 2.0f;

	sc = 1.0f;	// + (R().z * max_sc) / INTRO_MAX_Z;

	ssx = mchA_FontScaleX[INTRO_FNT] * INTRO_SCALE_X * sc;
	ssy = mchA_FontScaleY[INTRO_FNT] * INTRO_SCALE_Y * sc;

	x = round(R().x + dR.x);
	y = round(R().y + dR.y);

	if(shadow_flag){
		x -= 2;
		y += 2;
		col = intro_ShadowCol;
	}
	else
		col = color;

	if(isMoving())
		ang = M_PI * 4.0f * phase;
	else {
		if(CheckFlag(INTRO_CH_ROTATION))
			ang = angle;
		else
			ang = 0.0f;
	}

	mchA_d3dOutSprite(x,y,ssx,ssy,chr,col,250,ang,1);
}

void mchIntroChar::enableSound(void)
{
	int i;

	if(soundID != -1) return;

	for(i = EFF_LOGO_BEE_1; i <= EFF_LOGO_BEE_6; i ++){
		if(!mch_sndD -> GetStatusEFF(i)){
			soundID = i;
			return;
		}
	}
}

int mchIntroChar::isFinished(void)
{
	if(R1 == destR) return 1;

	return 0;
}

void mchIntroChar::setRndR(int dist_flag)
{
	float x,y,z;

	int max_z = round(INTRO_MAX_Z) * 10;
	const int delta = 100;

	if(!dist_flag){
		if(XRnd(2)){
			x = float(XRnd(640));
			y = XRnd(2) ? -float(XRnd(delta) + 20) : 480.0f + float(XRnd(delta) + 20);
		}
		else {
			x = XRnd(2) ? -float(XRnd(delta) + 20) : 640.0f + float(XRnd(delta) + 20);
			y = float(XRnd(480));
		}
	}
	else {
		if(R1.x < 0.0f){
			x = 640.0f + float(XRnd(delta) + 20);
			y = float(XRnd(480));
		}
		else {
			if(R1.x > 640.0f){
				x = -float(XRnd(delta) + 20);
				y = float(XRnd(480));
			}
			else {
				if(R1.y < 0.0f){
					x = float(XRnd(640));
					y = 480.0f + float(XRnd(delta) + 20);
				}
				else {
					x = float(XRnd(640));
					y = -float(XRnd(delta) + 20);
				}
			}
		}
	}

	z = float(XRnd(max_z)) / 10.0f;
	setR(Vect2f(x,y));
}

void mchIntroChar::Quant(float dt)
{
	const float dp0 = 0.01f;
//	const float dp = 0.4f;
	const float dp = 0.6f;

	const float max_angle = M_PI/4.0f;

	phase += (dest_phase > phase) ? dp * dt : -dp * dt;
	if(phase > 1.0f) phase = 1.0f;
	if(phase < 0.0f) phase = 0.0f;

	if(CheckFlag(INTRO_CH_ROTATION)){
		angle += angle_delta * dt;

		if(angle > max_angle){
			angle = max_angle;
			angle_delta = -fabs(angle_delta);
		}
		if(angle < -max_angle){
			angle = -max_angle;
			angle_delta = fabs(angle_delta);
		}
	}

	if(fabs(dest_phase - phase) <= dp0){
		phase = dest_phase;

		if(isMoving()){
			R0 = R1;
			phase = dest_phase = 0.0f;
			if(!CheckFlag(INTRO_CH_SPLINE))
				soundID = -1;
		}
	}

#ifdef _NO_RND_MOVEMENT_
	dR.x = dR.y = 0.0f;
	angle = 0.0f;
#endif

	if((isMoving() || CheckFlag(INTRO_CH_SPLINE)) && soundID != -1)
		mchStartSoundEFF(soundID,R().x,R().y);
}

mchIntroDispatcher::mchIntroDispatcher(void)
{
	numBorderChars = 0;
	borderCharsIdx = NULL;

	numChars = 0;
	data = NULL;

	curPhase = 0;
	curTime = startTime = stringStartTime = 0;

	stringAlphaDir = 0;

	startTime1 = -1;

	memset(stringStatus,0,6);
}

mchIntroDispatcher::~mchIntroDispatcher(void)
{
	if(data) delete data;
	if(borderCharsIdx) delete borderCharsIdx;
}

void mchIntroDispatcher::Redraw(void)
{
	int i,v,alpha;

	for(i = 0; i < numChars; i ++)
		data[i].Redraw(1);

	for(i = 0; i < numChars; i ++)
		data[i].Redraw();

	static int string_offs[6] = { 20,54,100,196,256,320 };

	if(stringAlphaDir){
		v = (curTime - stringStartTime);

		for(i = 0; i < 6; i ++){
			if(stringAlphaDir > 0){
				alpha = v - i * 255;
				if(alpha < 0) alpha = 0;
				if(alpha > 255) alpha = 255;

				if(alpha >= 128 && !stringStatus[i]){
					stringStatus[i] = 1;
					mchStopSoundEFF(EFF_LOGO_LETTER);
					mchStartSoundEFF(EFF_LOGO_LETTER);
				}
			}
			else {
				alpha = 255 - (v - i * 255);
				if(alpha < 0) alpha = 0;
				if(alpha > 255) alpha = 255;
/*
				if(alpha < 128 && !stringStatus[i]){
					stringStatus[i] = 1;
					mchStopSoundEFF(EFF_LOGO_LETTER);
					mchStartSoundEFF(EFF_LOGO_LETTER);
				}
*/
			}

			mchA_SprD -> DrawSprite(128 + string_offs[i],380,1.0f,1.0f,400 + i,mchA_ColorF[7],alpha,0.0f,0);
		}
	}
#ifdef _MAKE_INTRO_MOVIE_
	mchScreenShot();
#endif
}

void mchIntroDispatcher::Quant(void)
{
	int i,num,num1,cnt = 0;
	float x,y,dt;

	static int last_time = 0;

#ifdef _FRAME_CLOCK_
	curTime += 50;
#else
	curTime = clocki() - startTime;
#endif

	dt = float(curTime - prevTime) / 1000.0f;

	switch(curPhase){
		case 0:
			for(i = 0; i < numChars; i ++){
				if(data[i].isMoving())
					data[i].enableSound();
			}

#ifndef _MAKE_INTRO_MOVIE_
			if(curTime < 1000)
				last_time = curTime;
			else {
				if(curTime < 7000){
					num = 0;
					for(i = 0; i < numChars; i ++){
						if(data[i].isMoving())
							num ++;
					}
					if(!num){
						num = curTime / 1000;
						while(num){
							i = XRnd(numChars);
							if(!data[i].isMoving()){
								data[i].setPhaseDest(1.0f);
								data[i].setRndR(1);
								data[i].enableSound();
								num --;
							}
							if(++ cnt > 1000) break;
						}
					}
					last_time = curTime;
				}
			}
			if(curTime < 10000){
#else
			if(curTime < 5000){
#endif
				if(curTime - last_time > 100){
					num = 20;
					while(num){
						i = XRnd(numChars);
						if(!data[i].isMoving()){
							data[i].setPhaseDest(1.0f);
							data[i].setRndR();
							num --;
						}
						if(++ cnt > 1000) break;
					}
					last_time = curTime;
				}
			}
			else {
				AdvancePhase();
			}
			break;
		case 1:
			if(startTime1 != -1){
				if(curTime - 500 >= startTime1){
					mchStartSoundEFF(EFF_LOGO_SWARM_BEGIN);
					startTime1 = -1;
				}
			}

#ifndef _MAKE_INTRO_MOVIE_
			if(curTime > 30000){
				AdvancePhase();
				break;
			}
#else
			if(curTime > 20000){
				AdvancePhase();
				break;
			}
#endif

			if(curTime - last_time > 50){
				num = 30;
				while(num){
					i = XRnd(numChars);
					if(!data[i].isMoving() && !data[i].isFinished()){
						data[i].resetR();
						data[i].setPhaseDest(1.0f);
						num --;
					}
					if(++ cnt > 5000) break;
				}
				last_time = curTime;
			}

			num = numChars;
			while(num){
				i = XRnd(numChars);
				if(!data[i].isMoving() && data[i].isFinished()){
					x = float(XRnd(10)) / 5.0f - 0.5f;
					y = float(XRnd(10)) / 5.0f - 0.5f;
					data[i].set_dR(x,y);
				}
				num --;
			}

			num = 0;
			for(i = 0; i < numChars; i ++){
				if(!data[i].isMoving() && data[i].isFinished()){
					data[i].enableRotation();
					num ++;
				}
			}

			if(num >= numChars * 9 / 10){
				for(i = 0; i < INTRO_NUM_SPLINES; i ++){
					if(spline[i].isFree()){
						num = borderCharsIdx[XRnd(numBorderChars)];
						num1 = borderCharsIdx[XRnd(numBorderChars)];

						if(num != num1 && !data[num].isMoving() && data[num].isFinished() && !data[num1].isMoving() && data[num1].isFinished() && !data[num].CheckFlag(INTRO_CH_SPLINE) && !data[num1].CheckFlag(INTRO_CH_SPLINE))
							spline[i].Init(&data[num],&data[num1]);
					}
				}

				if(!stringAlphaDir){
					stringAlphaDir = 1;
					stringStartTime = curTime;
				}
			}

			if(num >= numChars / 2)
				mchStartSoundEFF(EFF_LOGO_SWARM_LOOP);

			for(i = 0; i < INTRO_NUM_SPLINES; i ++)
				spline[i].Quant(dt);

			break;
		case 2:
			if(curTime >= startFinitTime + 2000){ 
#ifdef _MAKE_INTRO_MOVIE_
				ErrH.Exit();
#endif
				AdvancePhase();
				break;
			}

			if(stringAlphaDir != -1){
				stringAlphaDir = -1;
				stringStartTime = curTime;
			}

			if(curTime - last_time > 25){
				num = 400;
				while(num){
					i = XRnd(numChars);
					if(data[i].isFinished()){
						data[i].setPhaseDest(1.0f);
						data[i].setRndR();
						num --;
					}
					if(++ cnt > 5000) break;
				}
				last_time = curTime;
			}
			num = 0;
			for(i = 0; i < numChars; i ++){
				data[i].DropFlag(INTRO_CH_SPLINE);

				if(data[i].isMoving() || data[i].isFinished())
					num ++;
			}
			if(!num) AdvancePhase();
			break;
	}

	for(i = 0; i < numChars; i ++)
		data[i].Quant(dt);

	Redraw();

	prevTime = curTime;
}

void mchIntroDispatcher::Init(int sx,int sy,void* text,void* bitmap,void* mask)
{
	int i,j,idx,col,sz,bsz;
	unsigned r,g,b;

	float x,y,x1,y1,fsx,fsy;

	unsigned char* tp = (unsigned char*)text;
	unsigned char* bp = (unsigned char*)bitmap;
	unsigned char* mp = (unsigned char*)mask;

	sz = bsz = idx = 0;
	for(i = 0; i < sy; i ++){
		for(j = 0; j < sx; j ++){
			r = bp[idx * 3 + 0];
			g = bp[idx * 3 + 1];
			b = bp[idx * 3 + 2];

			if(r || g || b){	
				sz ++;
				if(mp[idx]) bsz ++;
			}

			idx ++;
		}
	}

	numBorderChars = bsz;
	borderCharsIdx = new int[bsz];

	numChars = sz;
	data = new mchIntroChar[numChars];

	fsx = float(acsFntTable[INTRO_FNT] -> SizeX + INTRO_SP) * INTRO_SCALE_X;
	fsy = float(acsFntTable[INTRO_FNT] -> SizeY + INTRO_VSP) * INTRO_SCALE_Y;

	x = (640.0f - fsx * float(sx + 1)) / 2.0f;
	y = (480.0f - fsy * float(sy + 1)) / 2.0f - 30.0f;

	x1 = x;
	y1 = y;

	sz = bsz = idx = 0;
	for(i = 0; i < sy; i ++){
		for(j = 0; j < sx; j ++){
			r = bp[idx * 3 + 0];
			g = bp[idx * 3 + 1];
			b = bp[idx * 3 + 2];

			if(r || g || b){
				col = mchA_d3dMakeColor(r,g,b);
				data[sz].setRndR();
				data[sz].Init(tp[idx],col,data[sz].getR1(),Vect2f(x1,y1));
				data[sz].setRndR();

				if(mp[idx])
					borderCharsIdx[bsz ++] = sz;

				sz ++;
			}

			x1 += fsx;
			idx ++;
		}

		x1 = x;
		y1 += fsy;
	}

	r = g = b = 64;
	intro_ShadowCol = mchA_d3dMakeColor(r,g,b);

	startTime = clocki();
	curTime = prevTime = 0;
	curPhase = 0;
}

void mchIntroDispatcher::AdvancePhase(void)
{ 
	int i;

	switch(curPhase){
		case 0:
			startTime1 = curTime;
			break;
		case 1:
			mchStopSoundEFF(EFF_LOGO_SWARM_BEGIN);
			mchStopSoundEFF(EFF_LOGO_SWARM_LOOP);
			mchStartSoundEFF(EFF_LOGO_SWARM_END);
			memset(stringStatus,0,6);
			for(i = 0; i < INTRO_NUM_SPLINES; i ++)
				spline[i].Clear();

			startFinitTime = curTime;
			break;
	}

	curPhase ++; 
}

void mchIntroCharSpline::Init(mchIntroChar* p0, mchIntroChar* p1)
{
	float dist;
	const int dist0 = 10;

	Vect2f r,dr,r0;

	r = p0 -> getdestR() - p1 -> getdestR();
	dist = r.norm();

	if(dist > 120.0f || dist < 80.0f) return;

	r0 = Vect2f(320,240);

	phase = 0.0f;

	data0 = p0;
	data1 = p1;

	data0 -> SetFlag(INTRO_CH_SPLINE);
	data1 -> SetFlag(INTRO_CH_SPLINE);

	P0[1] = data0 -> getdestR();
	P0[2] = data1 -> getdestR();

	dr = ((P1[1] + P1[2]) / 2.0f - r0) * (float(10 - XRnd(8)) / 10.0f);
	P0[0] = r0 - dr;
	P0[3] = r0 - dr;

	P1[1] = data1 -> getdestR();
	P1[2] = data0 -> getdestR();

	dr = ((P1[1] + P1[2]) / 2.0f - r0) * (float(10 - XRnd(8)) / 10.0f);
	P1[0] = r0 - dr;
	P1[3] = r0 - dr;
}

void mchIntroCharSpline::Quant(float dt)
{
	int col,end = 0;
	const float dp = 0.6f;

	if(isFree()) return;

	phase += dp * dt;

	if(phase >= 1.0f){
		phase = 1.0f;
		end = 1;
	}

	data0 -> initR(R0());
	data1 -> initR(R1());

	data0 -> enableSound();
	data1 -> enableSound();

	if(end){
		col = data0 -> Color();
		data0 -> SetColor(data1 -> Color());
		data1 -> SetColor(col);

		Clear();
	}
}

Vect2f mchIntroCharSpline::R0(void)
{
	float t,t2,t3;

	t = phase;
	t2 = t * phase;
	t3 = t2 * phase;

	return P0[3]*((-t2+t3)/2.0f) + P0[0]*(-t/2.0+t2-t3/2.0) + P0[2]*(2.0*t2-3.0/2.0*t3+t/2.0) + P0[1]*(1.0-5.0/2.0*t2+3.0/2.0*t3);
}

Vect2f mchIntroCharSpline::R1(void)
{
	float t,t2,t3;

	t = phase;
	t2 = t * phase;
	t3 = t2 * phase;

	return P1[3]*((-t2+t3)/2.0f) + P1[0]*(-t/2.0+t2-t3/2.0) + P1[2]*(2.0*t2-3.0/2.0*t3+t/2.0) + P1[1]*(1.0-5.0/2.0*t2+3.0/2.0*t3);
}

void mchIntroCharSpline::Clear(void)
{ 
	if(data0) data0 -> DropFlag(INTRO_CH_SPLINE);
	if(data1) data1 -> DropFlag(INTRO_CH_SPLINE);

	data0 = data1 = NULL; 
	phase = 0.0f; 
}
