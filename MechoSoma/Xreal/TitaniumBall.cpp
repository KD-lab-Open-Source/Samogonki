////////////////////////////////////////////////////////////////////
//	��������� �����
////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "TitaniumBall.h"
#include "Params.h"
#include "Mesh3ds.h"
#include "M3d_effects.h"
#include "sound.h"
#include "AnimalArcansPrm.h"

void fxlabTitaniumExplosion(class Body* p);

TitaniumBall::TitaniumBall(const Vect3f& R_, const Vect3f& V_, float damage__, int owner_ID)
: Body(myM3DType(41)),
GrowingBody(0.1f, titanium_ball_time_to_grow, 0, titanium_ball_time_to_live),
OwnerProtection(owner_ID)
{
	type_ = TITANIUM_BALL;
	restitution_plus_one = titanium_ball_restitution_plus_one;
	setMass(2);
	dragV.set(titanium_ball_damping);
	dragW.set(titanium_ball_damping);
	friction = impact_friction_factor;

	damage_ability_ = damage__;
	setTrans(R_);
	setGlobalVelocity(V_*(Gravity*0.1f));
	Vect3f w;
	w.x = frnd(meteorite_W);
	w.y = frnd(meteorite_W);
	w.z = frnd(meteorite_W);
	w.Normalize(meteorite_W);
	setGlobalAngularVelocity(w);
	max_velocity_threshould = max_velocity_threshould_meteorite;
}	

void TitaniumBall::kill()
{
	fxlabTitaniumExplosion(this);
	startSound(EFF_TITANIUM_BALLS_ADD);
	Body::kill();
}


void Mechos::throwTitaniumBall(int damage)
{	
	cMesh& front = *part_by_index(Front);
	Vect3f R0 = front.GlobalMatrix.trans();
	R0.z += front.zmaxReal();
	Vect3f dx(front.xmaxReal()/2, 0, 0);
	Alg().xform(dx);
	float k = 10.f/Gravity;
	Vect3f Vl = Alg()*(Vlocal()*k + Vect3f(-titanium_ball_Vxy*0.2, titanium_ball_Vxy, titanium_ball_Vz));
	Mdisp -> attachBody(new TitaniumBall(R0 - dx, Vl,  damage, ID));
	Vect3f Vm = Alg()*(Vlocal()*k + Vect3f(0, titanium_ball_Vxy, titanium_ball_Vz));
	Mdisp -> attachBody(new TitaniumBall(R0, Vm,  damage, ID));
	Vect3f Vr = Alg()*(Vlocal()*k + Vect3f(titanium_ball_Vxy*0.2, titanium_ball_Vxy, titanium_ball_Vz));
	Mdisp -> attachBody(new TitaniumBall(R0 + dx, Vr,  damage, ID));
}

