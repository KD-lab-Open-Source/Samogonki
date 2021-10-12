#include "StdAfx.h"
#include "Body.h"
#include "Meteorite.h"
#include "Cannon.h"
#include "m3d_effects.h"
#include "Mesh3ds.h"
#include "xreal_utl.h"
#include <xmath.h>
#pragma hdrstop

#define M3D_VECTOR_A		(1<< 14)
#define M3D_VECTOR_B		(1<< 15)
#define KFRAME_STOP		(1<<8)

////////////////////////////////////////////////////////////////////
//	�����
////////////////////////////////////////////////////////////////////
Cannon::Cannon(cMesh* model_)
{
	model = model_;
	assert(model);

	//A = model -> Dummies.Find(M3D_VECTOR_A);
	//B = model -> Dummies.Find(M3D_VECTOR_B);

	//@caiiiycuk
	//model -> SetFrame(KFRAME_STOP);
	model->SetFrame();
}

void fxCannonFire(Vect3f pos,Vect3f vel,float pow);

void Cannon::fire()
{
	const MatXf& Xlg = model -> GlobalMatrix;
	Vect3f B_, A_;
	Xlg.xformPoint(A, A_);
	Xlg.xformPoint(B, B_);
	Vect3f dir = B_- A_;
	dir.normalize();

	fxCannonFire(B_,dir*(3 + fabsRnd(3)),2000);

//	Body* bolid = new Meteorite(M3D_CANNON_BALL, B_, dir*(30 + fabsRnd(100)), 3);
//	Mdisp -> attachBody(bolid);
}

void Cannon::quant()
{
	if(!random(50))
		fire();
}

void Cannon::show() const
{
	Vect3f A_,B_;
	const MatXf& Xlg = model -> GlobalMatrix;
	Xlg.xformPoint(A, A_);
	Xlg.xformPoint(B, B_);
	show_vector(A_, 45);
	show_vector(B_, (63<< 6) | 31);
}

////////////////////////////////////////////////////////////////////
//	��������26gg2
////////////////////////////////////////////////////////////////////
Fortress::Fortress(cMesh* model_)
 {
	model = model_;
	push_back(model -> FindMesh("Cannon0"));
	push_back(model -> FindMesh("Cannon1"));
	push_back(model -> FindMesh("Cannon2"));
}

void Fortress::quant()
{
	//@caiiiycuk
	//model -> SetFrame(KFRAME_STOP, 0, clockf()/15425);
	model->SetFrame();
	model -> ReCalcMatrix();
	if(!latency()){
		iterator ci;
		FOR_EACH(*this, ci)
			ci -> fire();
		latency.start(3500); //1840
		}
}

void Fortress::show() const
{
	Fortress* self = const_cast<Fortress*>(this);
	iterator ci;
	FOR_EACH(*self, ci)
		ci -> show();
}


////////////////////////////////////////////////////////////////////
//	Fucking�����
////////////////////////////////////////////////////////////////////
WaterPump::WaterPump(cMesh* model_) : FirePointList(model_)
{
	model = model_;
}

void WaterPump::quant()
{
//@caiiiycuk
//	model -> SetFrame(KFRAME_STOP, 0, clockf()/ /*15425*/ 30000);
	model->SetFrame();
	model -> ReCalcMatrix();
	fire(model -> GlobalMatrix);
}
