#include "StdAfx.h"
#include "Body.h"
#include "Brevno.h"
#include "SimpleClip.h"
#include "AnimalsPrm.h"

Brevno::Brevno()
: Body(myM3DType(26), FloatsBound, brevno_spheres_radius)
{
	type_ = BREVNO;
	setMass(brevno_mass);
	set_archimedean_force(brevno_buoyancy);
	dragV.set(brevno_linear_damping);
	dragW.set(brevno_angular_damping);
}

void Brevno::setPositionInit()
{
	R0 = R();
	Body::setPositionInit();
}

void Brevno::calc_forces_and_drags()
{
	if(submersion() > 0.1){
		Vect3f dR = getDist(R0, R());
		dR.Normalize(brevno_return_force);
		dR.z = 0;
		applyGlobalForce(dR);
		applyGlobalTorque(brevno_torque, Z_AXIS);
		}
}
	
Raft::Raft()
: Body(myM3DType(44), FloatsBound, raft_spheres_radius)
{
	setMass(raft_mass);
	set_archimedean_force(raft_buoyancy);
}
//void Raft::quant()
//{
//	set_archimedean_force(raft_buoyancy);
//}