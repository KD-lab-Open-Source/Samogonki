#include "StdAfx.h"
#include "Body.h"
#include "Meteorite.h"
#include "Cannon.h"
#include "M3d_effects.h"
#include "Mesh3ds.h"
#include "Xreal_utl.h"

////////////////////////////////////////////////////////////////////
//	Пушка
////////////////////////////////////////////////////////////////////
Cannon::Cannon(cMesh* model_)
{
	model = model_;
	assert(model);

	A = model -> Dummies.Find("vectora");
	B = model -> Dummies.Find("vectorb");

	model -> SetAnimation(0);
}

void fxlabCannonFire(const Vect3f& pos,const Vect3f& vel,float pow);

void Cannon::fire()
{
	const MatXf& Xlg = model -> GlobalMatrix;
	Vect3f B_, A_;
	Xlg.xformPoint(A, A_);
	Xlg.xformPoint(B, B_);
	Vect3f dir = B_- A_;
	dir.normalize();

	fxlabCannonFire(Vect3f(B_),Vect3f(dir*(3 + fabsRnd(3))),2000);

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
	show_vector(A_, XCOL(CYAN, 100));
	show_vector(B_, XCOL(CYAN, 100));
}

////////////////////////////////////////////////////////////////////
//	Крепость
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
	model -> SetAnimation(0, global_time()/15425.f);
	if(!latency()){
		iterator ci;
		FOR_EACH(*this, ci)
			ci -> fire();
		latency.start(3500); //1840
		}
}

void Fortress::show() const
{
	const_iterator ci;
	FOR_EACH(*this, ci)
		ci -> show();
}


////////////////////////////////////////////////////////////////////
//	FuckingНасос
////////////////////////////////////////////////////////////////////
WaterPump::WaterPump(cMesh* model_) : FirePointList(model_)
{
	model = model_;
}

void WaterPump::quant()
{
	model -> SetAnimation(0, global_time()/30000.f);
	fire(model -> GlobalMatrix);
}
