#include "StdAfx.h"
#include "Body.h"
#include "SimpleClip.h"
#include "TERRA.H"
#include "Params.h"

void Body::calc_archimedean_force()
{
	submersion_ = 0;
	SphereList::iterator fi;
	FOR_EACH(spheres, fi){
		const Vect3f& r = fi -> center;
		Vect3f rg;
		pose().xformPoint(r, rg);
		int x = XCYCL(rg.xi());
		int y = YCYCL(rg.yi());
		int hw = GetWH(x, y);
		if(hw){
			//float dz = hw - rg.z + fi -> radius + archimedean_force_wave_amlitude*(archimedean_force_wave_zero + sin(archimedean_force_wave_time_factor*global_time() - archimedean_force_wave_distance_factor*sqrt(sqr(rg.x) + sqr(rg.y))));
			float dz = hw - rg.z + fi -> radius;
			if(dz > 0){
				float f = k_archimedean*dz;
				if(f > 1.2*Gravity)
					f = 1.2*Gravity;
				float vz = Vglobal().z + Wglobal().x*(rg.y - R().y) - Wglobal().y*(rg.x - R().x);
				f -= vz*archimedean_force_damping;
				applyGlobalForce(f, Z_AXIS, r);
				submersion_ += dz*k_submersion;
				}
			}
		}	
	if(submersion_ > 1)
		submersion_ = 1;
}	

void Body::set_archimedean_force(float factor)
{
	k_archimedean = spheres.size() ? Gravity*factor/spheres.size() : 0;
	k_submersion = 0;
	SphereList::iterator si;
	FOR_EACH(spheres, si)
		k_submersion += 2*si -> radius;
	if(k_archimedean > FLT_EPS)
		k_submersion = min(k_submersion, (float)(Gravity/k_archimedean));
	if(k_submersion > FLT_EPS)
		k_submersion = 1.f/k_submersion;
}

void Body::place_on_the_water()
{
	if(!is_floating())
		return;

	float z_old = R().z;
	int hw = GetWH(XCYCL(R().xi()), YCYCL(R().yi()));
	if(!hw)
		return;
	setTrans(hw, Z_AXIS);
	if(test_collision(0))
		setTrans(z_old, Z_AXIS);
}