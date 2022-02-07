/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"
#include "ctl_point.h"
#include "Mesh3ds.h"

#include "arcane.h"
#include "arcane_menu.h"

#include "CameraDispatcher.h"

#include "race.h"
#include "sound.h"

#include "TERRA.H"

#include "mch_common.h" // For far target

#pragma warning( disable : 4305 )

/* ----------------------------- STRUCT SECTION ----------------------------- */

// flags...
#define MCH_MOUSE_3D_SET		0x01

struct mchMouse3D
{
	int flags;
	int Visible;

	Vect3f position;

	cMesh* m3dPtr;

	void set(const Vect3f& pos);
	void remove(void);
	void redraw(void);
	void setOrientation(const Vect3f& target);

	mchMouse3D(void);
	~mchMouse3D(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern char* mchA_StopTimeStr;
extern char* mchA_DesintegrateStr;

extern int mchLeftMousePress;

extern int mchCamera3D;

extern int mchPartID[];

extern cInterfaceVisGeneric	*gb_IVisGeneric;


/* --------------------------- DEFINITION SECTION --------------------------- */

#define MCH_MOUSE_DELTA_Z	20

int mchMaxCpDelta = 10;
int mchMinCpDelta = 0;
int mchCpSpeed = 2;
int mchCurCpDelta = 0;

float mchCpLight0[3] = { 0.5, 0.5, 0.5 };
float mchCpLight1[3] = { 0.1, 0.1, 0.1 };
float mchCpLightCur[3] = { 0.0, 0.0, 0.0 };
float mchCpLightRadius = 5.0;
float mchCpLightRadius1 = 4.0;
int mchCpLightPeriod = 5;
int mchCpLightTimeDelta = 1;
int mchCpLightTime = 0;
int mchCpLight = -1;

mchMouse3D* mch_mouse3D = NULL;

const int MCH_CP_Version = 100;

mchMouse3D::mchMouse3D(void)
{
	flags = 0;
	Visible = 0;
	position.set(0.f);
	m3dPtr = (cMesh*)gb_IVisGeneric -> CreateObject(M3D_KIND(M3D_MOUSE3D),M3D_TYPE(M3D_MOUSE3D));
	Vect3f v1(0,0,0);
	Vect3f v2(0,0,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)m3dPtr,&v1,&v2);
}

mchMouse3D::~mchMouse3D(void)
{
}

void mchMouse3D::set(const Vect3f& pos)
{
	position = pos;

	Vect3f v1(90,0,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)m3dPtr, &position, &v1);
	gb_IVisGeneric -> SetObjectVisible((cUnknownClass*)m3dPtr,0);

	if(mchTurnBasedGame && mchTimeMode == MCH_TIME_STOPPED && !camera_dispatcher -> isStartScenarioRunning() && !(mchGameFlags & MCH_START_TIME_FLAG))
		Visible = 1;
	else 
		Visible = 0;

	flags |= MCH_MOUSE_3D_SET;
}

void mchMouse3D::redraw(void)
{
}

void mchMouse3D::remove(void)
{
	flags &= ~MCH_MOUSE_3D_SET;
}	

void mchMouse3D::setOrientation(const Vect3f& target)
{
	if(target == position)
		return;
	Vect3f r = getDist(target, position);
	r.z += MCH_MOUSE_DELTA_Z;
	Mat3f M = Mat3f(M_PI/2 + r.psi(), Z_AXIS)*Mat3f(r.theta(), X_AXIS);
	m3dPtr -> SetOrientation(M);
}

void mchInitMouse3D(void)
{
	if(mch_mouse3D) delete mch_mouse3D;

	mch_mouse3D = new mchMouse3D;
}

void mchSetMouse3D(const Vect3f& pos)
{
	mch_mouse3D -> set(pos);
}

void mchShowMouse3D(void)
{
	mch_mouse3D -> redraw();
}

void mchLightCPoint(int& x,int& y,int& z)
{
	int sz;

	if(!mch_mouse3D -> Visible) return;

	sz = round(16.0f * camera_dispatcher -> getScale(Vect3f(x,y,z)));

	mchControlPoint* p = mch_raceD -> activeRacer -> seedLst.first();

	mch_raceD -> activeRacer -> mouseSeed = NULL;

	while(p){
		if(abs(getDistX(p -> PosX,x)) < sz && abs(getDistY(p -> PosY,y)) < sz){
			mch_raceD -> activeRacer -> mouseSeed = p;
			x = p -> PosX;
			y = p -> PosY;
			z = p -> PosZ;
			if(p -> mPtr) z -= round((p -> mPtr -> zmaxReal() - p -> mPtr -> zminReal()) / 2.0);
			mch_mouse3D -> set(Vect3f(x,y,z));

			return;
		}
		p = p -> next;
	}
}

mchControlPoint::mchControlPoint(void)
{ 
	PosX = PosY = PosZ = 0; 
	mPtr = NULL; 
	list = 0; 

	trackEl = NULL;

	arcaneData.ID = -1; 
}

mchControlPoint::mchControlPoint(int x,int y,int z,int tp)
{ 
	PosX = x; 
	PosY = y; 
	PosZ = z; 

	list = 0;
	mPtr = NULL;

	trackEl = NULL;

	arcaneData.ID = -1;
}

void mchControlPoint::createObj(void)
{
//	if(mchDebugMode || mchTurnBasedGame){
	if(mchDebugMode){
		mPtr = (cMesh*)gb_IVisGeneric -> CreateObject(M3D_KIND(M3D_CONTROL_POINT),M3D_TYPE(M3D_CONTROL_POINT));
		Vect3f v1((float)PosX,(float)PosY,(float)PosZ);
		Vect3f v2(0,0,0);
		gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)mPtr,
			&v1,&v2);
		mPtr -> SetAttribute(MESH_NOT_SHADE | MESH_LIGHTING_MAX);
	}
}

void mchControlPoint::deleteObj(void)
{
	if(mPtr)
		gb_IVisGeneric -> ReleaseObject((cUnknownClass*)mPtr);
	mPtr = NULL;
}

mchControlPoint::~mchControlPoint(void)
{ 
	deleteObj();
}

void mchControlPoint::redraw(void)
{
}
/*
void mchControlPointDispatcher::Quant(void)
{
	mchControlPoint* p = pointsLst.first();
	while(p){
		if(p -> mPtr)
			M3D -> SetPosition(p -> mPtr,(float)p -> PosX,(float)p -> PosY,(float)p -> PosZ,0,0,0);
		p = p -> next;
	}

	mchCurCpDelta += mchCpSpeed;
	if(mchCurCpDelta > mchMaxCpDelta){
		mchCurCpDelta = mchMaxCpDelta;
		mchCpSpeed = -mchCpSpeed;
	}
	if(mchCurCpDelta < mchMinCpDelta){
		mchCurCpDelta = mchMinCpDelta;
		mchCpSpeed = -mchCpSpeed;
	}
	if(curPoint){
		if(curPoint -> mPtr)
			M3D -> SetPosition(curPoint -> mPtr,(float)curPoint -> PosX,(float)curPoint -> PosY,(float)(curPoint -> PosZ + mchCurCpDelta),0,0,0);

		mchCpLightTime += mchCpLightTimeDelta;
		if(mchCpLightTime > mchCpLightPeriod){
			mchCpLightTime = mchCpLightPeriod;
			mchCpLightTimeDelta = -mchCpLightTimeDelta;
		}
		if(mchCpLightTime < 0){
			mchCpLightTime = 0;
			mchCpLightTimeDelta = -mchCpLightTimeDelta;
		}
		mchCpLightCur[0] = mchCpLight0[0] + (mchCpLight1[0] - mchCpLight0[0]) * mchCpLightTime / mchCpLightPeriod; 
		mchCpLightCur[1] = mchCpLight0[1] + (mchCpLight1[1] - mchCpLight0[1]) * mchCpLightTime / mchCpLightPeriod; 
		mchCpLightCur[2] = mchCpLight0[2] + (mchCpLight1[2] - mchCpLight0[2]) * mchCpLightTime / mchCpLightPeriod; 
	}
	if(mchCpLight != -1){
		if(mchLeftMousePress)
			SetOmni(mchCpLight,mchCpLightRadius + (mchCpLightRadius1 - mchCpLightRadius) * mchCpLightTime / mchCpLightPeriod,mchCpLightCur[0],mchCpLightCur[1],mchCpLightCur[2]);
		else
			SetOmni(mchCpLight,mchCpLightRadius,mchCpLight0[0],mchCpLight0[1],mchCpLight0[2]);
	}
}
*/
void mchGetMouseCoords3D(int& x,int& y,int& z)
{
	x = mch_mouse3D -> position.xi();
	y = mch_mouse3D -> position.yi();
	z = mch_mouse3D -> position.zi();
}

void mchControlPoint::SetArcane(mchArcaneData& p,int tp,int p_id)
{
	memcpy((char*)(&arcaneData),(char*)(&p),sizeof(mchArcaneData));
}

void mchControlPoint::RemoveArcane(int mode)
{
	arcaneData.ID = -1;
}

int mchMouse3D_Visible(void)
{
	return mch_mouse3D -> Visible;
}

void mchControlPoint::save(XBuffer& xb)
{
	xb < PosX < PosY < PosZ < arcaneData.ID;
/*
	xb < MCH_CP_Version < ID < PosX < PosY < PosZ < arcaneData.ID;

	if(trackEl) xb < trackEl -> ID;
	else xb < (int)-1;
*/
}

void mchControlPoint::load(XBuffer& xb)
{
	xb > PosX > PosY > PosZ > arcaneData.ID;
/*
	int ver,id;
	xb > ver > ID > PosX > PosY > PosZ > arcaneData.ID > id;
	if(id != -1)
		trackEl = mchGetTrackEl(id);
*/
}
