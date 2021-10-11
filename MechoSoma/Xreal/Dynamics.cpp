#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "SimpleClip.h"
#include "MultibodyContact.h"
#include "params.h"
#include "xreal_utl.h"
#include "LCP_ContactSolver.h"
#include "Mechosoma.h"
#include "Statistics.h"
#include "QuatMath.h"

/////////////////////////////////////////////////////
//	Integration
/////////////////////////////////////////////////////
void DynamicBody::integrateEuler(float dt)
{
	R_prev = R(); 

	if(rest_system_enabled && ground_colliding && !in_contact && F_total.norm2() < rest_force2_threshould && K_total.norm2() < rest_torque2_threshould && V.norm2() < rest_velocity2_threshould && W.norm2() < rest_angular_velocity2_threshould)
		return;

	applyGlobalForce(-Gravity, Z_AXIS);

	Xlg.trans() += Vglobal()*dt;
	quat += derivation_by_Wglobal(quat, Wglobal()*dt);
	quat.normalize();

	Vect3f Vl = Vlocal();
	Vect3f Fl = F_total;
	Alg().invXform(Fl);
	Vl += (Fl*mass_inv - dragV*Vl)*dt;
	Alg().xform(Vl, V);

	Vect3f Wl = Wlocal();
	Vect3f Kl = K_total;
	Alg().invXform(Kl);
	Wl += (I_inv*Kl - dragW*Wl)*dt;
	Alg().xform(Wl, W);

//	V += (F_total*mass_inv - Alg()*(dragV_*Vlocal()))*dt;
//	W += (Ig_inv*K_total - Alg()*(dragW_*Wlocal()))*dt;

	Xlg.rot().set(quat);
	const Mat3f& R = Alg();
	float t1 = R[0][0]*R[0][0];
	float t3 = R[0][1]*R[0][1];
	float t5 = R[0][2]*R[0][2];
	float t8 = R[0][0]*I_inv.x;
	float t10 = R[0][1]*I_inv.y;
	float t12 = R[0][2]*I_inv.z;
	float t14 = t8*R[1][0]+t10*R[1][1]+t12*R[1][2];
	float t18 = t8*R[2][0]+t10*R[2][1]+t12*R[2][2];
	float t19 = R[1][0]*R[1][0];
	float t21 = R[1][1]*R[1][1];
	float t23 = R[1][2]*R[1][2];
	float t32 = R[1][0]*I_inv.x*R[2][0]+R[1][1]*I_inv.y*R[2][1]+R[1][2]*I_inv.z*R[2][2];
	float t33 = R[2][0]*R[2][0];
	float t35 = R[2][1]*R[2][1];
	float t37 = R[2][2]*R[2][2];
	Ig_inv[0][0] = t1*I_inv.x+t3*I_inv.y+t5*I_inv.z;
	Ig_inv[0][1] = t14;
	Ig_inv[0][2] = t18;
	Ig_inv[1][0] = t14;
	Ig_inv[1][1] = t19*I_inv.x+t21*I_inv.y+t23*I_inv.z;
	Ig_inv[1][2] = t32;
	Ig_inv[2][0] = t18;
	Ig_inv[2][1] = t32;
	Ig_inv[2][2] = t33*I_inv.x+t35*I_inv.y+t37*I_inv.z;

	boundVelocities();
}

void DynamicBody::boundVelocities()
{
	if(V.norm2() > sqr(max_velocity_threshould))
		V.normalize(max_velocity_threshould);
	if(W.norm2() > sqr(max_angular_velocity_threshould))
		W.normalize(max_angular_velocity_threshould);
}

void Body::integrate(float dt)
{
  	integrateEuler(dt);
	post_quant();
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < typeid(*this).name() < ": " <= R();
#endif
}

void Body::set_default_forces()
{
	clearForces();
	if(is_floating())
		calc_archimedean_force();
	else
		submersion_ = 0;
}

void Body::degeneracy_remedy()
{
	clearForces();
	clearVelocities();
	setTrans(R().z + 0.1, Z_AXIS);
	evolve_degeneracy = 1;
}

////////////////////////////////////////////////////////////////////////////////////
//	ContactingBodies
////////////////////////////////////////////////////////////////////////////////////
ContactingBodies::ContactingBodies()
{
	reserve(10);
}

void ContactingBodies::clear()
{
	vector<class Body*>::clear();
	total_contacts.clear();
}

void ContactingBodies::evolve(float dt)
{
	start_timer(resolve_constraints, STAT_BODYDISP);
	if(!UseCollisionHandler)
		LCP_solver.resolve_impulse_and_force(*this);
	stop_timer(resolve_constraints, STAT_BODYDISP);

	start_timer(integration, STAT_BODYDISP);
	iterator bi;
	FOR_EACH(*this, bi)
		(*bi) -> integrate(dt);
	stop_timer(integration, STAT_BODYDISP);
}


