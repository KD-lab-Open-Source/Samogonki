#include "StdAfx.h"
#include "Body.h"
#include "Contact.h"
#include "LCP_ContactSolver.h"
#include "Params.h"
#include "CHOLESKY.H"
#include "mechosoma.h"
#include "Statistics.h"
#include "Xreal_utl.h"

//#define _CHECK_RESOLVING_

#ifdef _FINAL_VERSION_
#undef _CHECK_RESOLVING_
#endif

LCP_ContactSolver LCP_solver;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		DynamicBody functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mat3f& DynamicBody::local_mass_matrix(const Vect3f& ri, const Vect3f& rj, Mat3f& K)
{
	float t1 = rj[2]*I_inv[1];
	float t3 = rj[1]*I_inv[2];
	float t6 = rj[0]*I_inv[2];
	float t8 = rj[0]*I_inv[1];
	float t11 = rj[2]*I_inv[0];
	float t15 = rj[1]*I_inv[0];
	K[0][0] = mass_inv+t1*ri[2]+t3*ri[1];
	K[0][1] = -t6*ri[1];
	K[0][2] = -t8*ri[2];
	K[1][0] = -t3*ri[0];
	K[1][1] = mass_inv+t11*ri[2]+t6*ri[0];
	K[1][2] = -t15*ri[2];
	K[2][0] = -t1*ri[0];
	K[2][1] = -t11*ri[1];
	K[2][2] = mass_inv+t15*ri[1]+t8*ri[0];
	return K;
}

Mat3f& DynamicBody::global_mass_matrix(const Vect3f& ri, const Vect3f& rj, Mat3f& K)
{
	float t96 = ri[2];
	float t95 = ri[1];
	float t94 = ri[0];
	float t93 = rj[2];
	float t92 = rj[1];
	float t91 = rj[0];
	float t90 = Ig_inv[2][1];
	float t89 = Ig_inv[2][2];
	float t88 = Ig_inv[1][1];
	float t87 = Ig_inv[1][2];
	float t86 = Ig_inv[2][0];
	float t85 = Ig_inv[0][0];
	float t84 = Ig_inv[0][1];
	float t83 = Ig_inv[0][2];
	float t82 = Ig_inv[1][0];
	float t81 = -t95*t84+t94*t88;
	float t80 = -t96*t82+t95*t86;
	float t79 = -t95*t85+t94*t82;
	float t78 = -t95*t83+t94*t87;
	float t77 = -t96*t88+t95*t90;
	float t76 = t96*t83-t94*t89;
	float t75 = -t96*t87+t95*t89;
	float t74 = t96*t85-t94*t86;
	float t73 = t96*t84-t94*t90;
	K[1][0] = -t73*t93+t76*t92;
	K[1][1] = mass_inv+t74*t93-t76*t91;
	K[1][2] = -t74*t92+t73*t91;
	K[2][0] = -t81*t93+t78*t92;
	K[2][1] = t79*t93-t78*t91;
	K[0][0] = mass_inv-t77*t93+t75*t92;
	K[2][2] = mass_inv-t79*t92+t81*t91;
	K[0][1] = t80*t93-t75*t91;
	K[0][2] = -t80*t92+t77*t91;
	return K;
}

void Body::calc_normal_mass_matrix(Mat& M)
{
	int normal_size = contacts.size() + contacts2.size();
	mass_matrix_array = new Mass_Matrix_Array(normal_size);
	
	normal_contacts.clear();

	ContactList::iterator ci;
	FOR_EACH(contacts, ci){
		normal_contacts.push_back(NormalContact());
		normal_contacts.back().r = ci -> cp1;
		Alg().invXform(ci -> normal, normal_contacts.back().n);
		normal_contacts.back().index = ci -> normal_index;
		}

	ContactList2::iterator ci2;
	FOR_EACH(contacts2, ci2){
		if((*ci2) -> normal_index != -1){
			normal_contacts.push_back(NormalContact());
			normal_contacts.back().r = (*ci2) -> cp2;
			Alg().invXform((*ci2) -> normal, normal_contacts.back().n);
			normal_contacts.back().n.negate();
			normal_contacts.back().index = (*ci2) -> normal_index;
			}
		else{
			//assert(0);
			normal_size--;
			}
		}

	for(int i = 0; i < normal_size; i++){
		NormalContact& ci = normal_contacts[i];
		float m = dot(ci.n, local_mass_matrix(ci.r, ci.r, (*mass_matrix_array)(i + 1, i + 1))*ci.n);
		M(ci.index, ci.index) += m*LCP_diagonal_element_scaling_factor;
		for(int j = i + 1; j < normal_size; j++){
			NormalContact& cj = normal_contacts[j];
			float m = dot(ci.n, local_mass_matrix(ci.r, cj.r, (*mass_matrix_array)(i + 1, j + 1))*cj.n);
			M(ci.index, cj.index) += m;
			M(cj.index, ci.index) += m;
			}
		}
}

void Body::calc_tangent_mass_matrix(Mat& Mnt, Mat& Mtn, Mat& Mtt)
{
	tangent_contacts.clear();

	int local_index = 1;
	ContactList::iterator ci;
	FOR_EACH(contacts, ci){
		if(ci -> is_frictional){
			tangent_contacts.push_back(TangentContact());
			Alg().invXform(ci -> tangent, tangent_contacts.back().t);
			tangent_contacts.back().index = ci -> tangent_index;
			tangent_contacts.back().local_index = local_index;
			}
		local_index++;
		}

	ContactList2::iterator ci2;
	FOR_EACH(contacts2, ci2){
		if((*ci2) -> is_frictional){
			tangent_contacts.push_back(TangentContact());
			Alg().invXform((*ci2) -> tangent, tangent_contacts.back().t);
			tangent_contacts.back().index = (*ci2) -> tangent_index;
			tangent_contacts.back().t.negate();
			tangent_contacts.back().local_index = local_index;
			}
		local_index++;
		}

	int tangent_size = tangent_contacts.size();
	if(!tangent_size)
		return;

	int normal_size = normal_contacts.size();
	int i;
	for(i = 0; i < normal_size; i++){
		NormalContact& ci = normal_contacts[i];
		for(int j = 0; j < tangent_size; j++){
			TangentContact& cj = tangent_contacts[j];
			float m = i + 1 <= cj.local_index ? dot(ci.n, (*mass_matrix_array)(i + 1, cj.local_index)*cj.t) : dot(cj.t, (*mass_matrix_array)(cj.local_index, i + 1)*ci.n);
			Mnt(ci.index, cj.index) += m;
			Mtn(cj.index, ci.index) += m;
			}
		}

	for(i = 0; i < tangent_size; i++){
		TangentContact& ci = tangent_contacts[i];
		float m = dot(ci.t, (*mass_matrix_array)(ci.local_index, ci.local_index)*ci.t);
		Mtt(ci.index, ci.index) += m*LCP_diagonal_element_scaling_factor;
		for(int j = i + 1; j < tangent_size; j++){
			TangentContact& cj = tangent_contacts[j];
			float m = dot(ci.t, (*mass_matrix_array)(ci.local_index, cj.local_index)*cj.t);
			Mtt(ci.index, cj.index) += m;
			Mtt(cj.index, ci.index) += m;
			}
		}
}

void Body::calc_dynamic_friction_mass_matrix(Mat& M)
{
	tangent_contacts.clear();

	int local_index = 1;
	ContactList::iterator ci;
	FOR_EACH(contacts, ci){
		if(ci -> is_dynamic_frictional){
			tangent_contacts.push_back(TangentContact());
			Alg().invXform(ci -> tangent, tangent_contacts.back().t);
			tangent_contacts.back().index = ci -> normal_index;
			tangent_contacts.back().local_index = local_index;
			}
		local_index++;
		}

	ContactList2::iterator ci2;
	FOR_EACH(contacts2, ci2){
		if((*ci2) -> is_dynamic_frictional){
			tangent_contacts.push_back(TangentContact());
			Alg().invXform((*ci2) -> tangent, tangent_contacts.back().t);
			tangent_contacts.back().t.negate();
			tangent_contacts.back().index = (*ci2) -> normal_index;
			tangent_contacts.back().local_index = local_index;
			}
		local_index++;
		}

	int tangent_size = tangent_contacts.size();
	if(!tangent_size)
		return;

	int normal_size = normal_contacts.size();
	for(int i = 0; i < normal_size; i++){
		NormalContact& ci = normal_contacts[i];
		for(int j = 0; j < tangent_size; j++){
			TangentContact& cj = tangent_contacts[j];
			float m = i + 1 <= cj.local_index ? dot(ci.n, (*mass_matrix_array)(i + 1, cj.local_index)*cj.t) : dot(cj.t, (*mass_matrix_array)(cj.local_index, i + 1)*ci.n);
			M(ci.index, cj.index) -= dynamic_friction_factor*m;
			}
		}
}

void LCP_ContactSolver::calculate_M()
{
	M = 0;
	ContactingBodies::iterator bi;
	FOR_EACH(*bodies_, bi)
		(*bi) -> calc_normal_mass_matrix(M);
	M_is_calculated = 1;
}

void LCP_ContactSolver::calculate_M_frictional()
{
	Mnt = 0;
	Mtn = 0;
	Mtt = 0;

	ContactingBodies::iterator bi;
	FOR_EACH(*bodies_, bi)
		(*bi) -> calc_tangent_mass_matrix(Mnt, Mtn, Mtt);
}

///////////////////////////////////////////////////////////////////////////
//			Frictionless
///////////////////////////////////////////////////////////////////////////
int LCP_ContactSolver::compute_frictionless_impulses()
{
	int do_calculate = 0;
	int i = 1;
	TotalContactList::iterator cpi;		     
	FOR_EACH(total_contacts, cpi){
		Contact& c = **cpi;
		float u_n = c.calc_normal_velocity();
		//if(u_n < velocity_relaxation_value)
		if(u_n < impulse_resolve_velocity_threshould)
			do_calculate = 1;
		q(i++) = (u_n < 0 ? c.restitution_plus_one*u_n : 0) - velocity_relaxation_value;
		}
		
	if(!do_calculate)
		return 0;

	calculate_M();

	compute_force();

	for(i = 0; i < size; i++)
		total_contacts[i] -> j_n = f[i];

	return 1;
}			
	
int LCP_ContactSolver::compute_frictionless_forces()
{
	int do_calculate = 0;
	int i = 1;
	TotalContactList::iterator cpi;		     
	FOR_EACH(total_contacts, cpi){
		Contact& c = **cpi;
		float a_n = c.calc_normal_acceleration();
		float relaxation = acceleration_relaxation_value - c.dist*penetration2acceleration_relaxation_factor;
		if(a_n < force_resolve_acceleration_threshould)
			do_calculate = 1;
		q(i++) = (a_n < 0 ? a_n : 0) - relaxation;
		}
	
	if(!do_calculate)
		return 0;

	calculate_M();

	compute_force();
	
	for(i = 0; i < size; i++)
		total_contacts[i] -> f_n = f[i];

	return 1;
}


/////////////////////////////////////////////////////////////////////////////////
//		Frictional Impulses
/////////////////////////////////////////////////////////////////////////////////

int LCP_ContactSolver::compute_frictional_impulses()
{
	if(constraint_impulse_calculation != 2 || !frictional_bodies)
		return constraint_impulse_calculation ? compute_frictionless_impulses() : 0;

	frictional_contacts.clear();
	frictional_contacts_indices.clear();

	int do_calculate = 0;
	int i = 1;
	int i_tangent = 1;
	TotalContactList::iterator cpi;		     
	FOR_EACH(total_contacts, cpi){
		Contact& ci = **cpi;
		ci.prepare_to_impulse_calculation();
		if(ci.is_frictional){
			frictional_contacts.push_back(&ci);
			frictional_contacts_indices.push_back(i);
			ci.tangent_index = i_tangent++;
			}
		if(ci.u_n < impulse_resolve_velocity_threshould)
			do_calculate = 1;
		q(i++) = (ci.u_n < 0 ? ci.restitution_plus_one*ci.u_n : 0) - velocity_relaxation_value;
		}
		
	if(!do_calculate)
		return 0;

	calculate_M();

	compute_force();

	if(frictional_contacts.size()){

		init_frictional(frictional_contacts.size());

		calculate_M_frictional();

		for(i = 0; i < size_frictional; i++){
			Contact& ci = *frictional_contacts[i];
			q_t[i] = ci.u_t;
			mu_t[i] = ci.impact_friction;
			fn_index[i] = frictional_contacts_indices[i];
			}

		compute_force_frictional();
		
		for(i = 0; i < size_frictional; i++)
			frictional_contacts[i] -> j_t = f_t[i];

		finit_frictional();
		}

	for(i = 0; i < size; i++)
		total_contacts[i] -> j_n = f[i];

	return 1;
}			


/////////////////////////////////////////////////////////////////////////////////
//		Frictional Forces
/////////////////////////////////////////////////////////////////////////////////

int LCP_ContactSolver::compute_frictional_forces(int enable_dynamic_friction)
{
	if(constraint_force_calculation != 2)
		return constraint_force_calculation == 1 ? compute_frictionless_forces() : 0;

	frictional_contacts.clear();
	frictional_contacts_indices.clear();

	int do_calculate = 0;
	int dynamic_friction = 0;
	int i = 1;
	int i_tangent = 1;
	TotalContactList::iterator cpi;		     
	FOR_EACH(total_contacts, cpi){
		Contact& ci = **cpi;
		ci.prepare_to_force_calculation(enable_dynamic_friction);
		if(ci.is_frictional){
			frictional_contacts.push_back(&ci);
			frictional_contacts_indices.push_back(i);
			ci.tangent_index = i_tangent++;
			}
		else
			if(ci.is_dynamic_frictional)
				dynamic_friction = 1;
		float relaxation = acceleration_relaxation_value - ci.dist*penetration2acceleration_relaxation_factor;
		if(ci.a_n < force_resolve_acceleration_threshould)
			do_calculate = 1;
		q(i++) = (ci.a_n < 0 ? ci.a_n : 0) - relaxation;
		}
		
	if(!do_calculate)
		return 0;

	calculate_M();
	
	if(dynamic_friction){
		ContactingBodies::iterator bi;
		FOR_EACH(*bodies_, bi)
			(*bi) -> calc_dynamic_friction_mass_matrix(M);
		}

	compute_force();

	if(frictional_contacts.size()){

		init_frictional(frictional_contacts.size());

		calculate_M_frictional();

		for(int i = 0; i < size_frictional; i++){
			Contact& ci = *frictional_contacts[i];
			q_t[i] = ci.a_t;
			mu_t[i] = ci.static_friction;
			fn_index[i] = frictional_contacts_indices[i];
			}

		compute_force_frictional();
		
		for(i = 0; i < size_frictional; i++)
			frictional_contacts[i] -> f_t = f_t[i];

		finit_frictional();
		}

	for(i = 0; i < size; i++)
		total_contacts[i] -> f_n = f[i];

	return 1;
}			

int LCP_ContactSolver::compute_frictional_forces()
{
	int success = 1;

	try {
		compute_frictional_forces(1);
		}

	catch(char*){
		M_is_calculated = 0;
		compute_frictional_forces(0);
		success = 0;
		}
		
//	static StaticStatisticalAnalizator stat("stat");
//	stat.add(success);
	
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
//  ContactingBodies::resolve_impulse_and_force()
/////////////////////////////////////////////////////////////////////////////
void LCP_ContactSolver::resolve_impulse_and_force(ContactingBodies& bodies)
{
try {

	M_is_calculated = 0;
	bodies_ = &bodies;

	////////////////////////////////////////////////////////////////
	//	Prepare contacts
	////////////////////////////////////////////////////////////////
	total_contacts.clear();

	int index = 1;
	frictional_bodies = 0;
	ContactingBodies::iterator bi;
	FOR_EACH(bodies, bi){
		Body& b = **bi;
		b.evolve_degeneracy = 0;
		b.dV_constraint = b.dW_constraint = Vect3f::ZERO; 
		b.F_constraint = b.K_constraint = Vect3f::ZERO; 
		ContactList::iterator ci;
		FOR_EACH(b.contacts, ci){
			total_contacts.push_back(&*ci);
			ci -> normal_index = index++;
			}
		if(b.friction)
			frictional_bodies++;
		}

	int n_contacts = total_contacts.size();
	statistics_add(total_contacts, STAT_NUMERIC, n_contacts);
	statistics_add(bodies_in_contacts, STAT_NUMERIC, bodies.size());

	////////////////////////////////////////////////////////////////
	// There are contacts - resolve	
	////////////////////////////////////////////////////////////////
	if(n_contacts){
		///////////////////////////////////////////////////////////
		//	Initialization frictionless problem
		//	for impulse and force both
		///////////////////////////////////////////////////////////
		init(n_contacts);
		
		//////////////////////////////////////////////////////
		//	Constraint impulses
		//////////////////////////////////////////////////////
		if(compute_frictional_impulses()){
			TotalContactList::iterator cpi;
			FOR_EACH(total_contacts, cpi){
				Contact& c = **cpi;
				Vect3f J = c.normal*c.j_n;
				if(c.is_frictional)
					J += c.tangent*c.j_t;
				c.body1 -> dV_constraint += J;
				c.body1 -> dW_constraint += c.cp1g % J;
				if(c.body2 && c.body2 -> is_dynamic()){
					c.body2 -> dV_constraint -= J;
					c.body2 -> dW_constraint -= c.cp2g % J;
					}
				}

			ContactingBodies::iterator bi;
			FOR_EACH(bodies, bi){
				Body& b = **bi;
				b.V += b.dV_constraint *= b.mass_inv;
				b.W += b.dW_constraint *= b.Ig_inv;
				}
			
			//transfer_impulse_information();
			}

		//////////////////////////////////////////////////////
		//	Constraint forces
		//////////////////////////////////////////////////////
		if(compute_frictional_forces()){
			TotalContactList::iterator cpi;
			FOR_EACH(total_contacts, cpi){
				Contact& c = **cpi;
				Vect3f F = c.normal*c.f_n;
				if(c.is_dynamic_frictional)
					F -= (c.dynamic_friction*c.f_n)*c.tangent;
				else
					if(c.is_frictional)
						F -= c.tangent*c.f_t;
				c.body1 -> F_constraint += F;
				c.body1 -> K_constraint += c.cp1g % F;
				if(c.body2 && c.body2 -> is_dynamic()){
					c.body2 -> F_constraint -= F;
					c.body2 -> K_constraint -= c.cp2g % F;
					}
				}

			FOR_EACH(bodies, bi){
				Body& b = **bi;
				b.F_total += b.F_constraint;
				b.K_total += b.K_constraint;
				}

			//transfer_force_information();
			}


		/////////////////////////////////
		//  Check resolving
		/////////////////////////////////
#ifdef _CHECK_RESOLVING_
		static ofstream fout("fout");
		fout << total_contacts.size() << "  ============\n";
		TotalContactList::iterator cpi;
		int log = 0;
		FOR_EACH(total_contacts, cpi){
			Contact& c = **cpi;
			if(!c.body2)
				continue;
			c.log_resolving(fout);
			log = 1;
			statistics_add(j_n, 5, c.j_n);
			statistics_add(f_n, 5, c.f_n);
			}
		save_problem(fout);
#endif

		}
}	

catch(char* str) {
	#ifndef _FINAL_VERSION_
	//	fout < "Exception in resolve_impulse_and_force(): " < str < "\n\n";
	//	fout < "n_contacts: " <= total_contacts.size() < "\n";
	//	fout < "n_bodies: " <= bodies.size() < "\n";
	#endif
	str = 0;
	ContactingBodies::iterator bi;
	FOR_EACH(bodies, bi)
		(*bi) -> degeneracy_remedy();
	}	
}

////////////////////////////////////////////////////////////////////////////////
//		Transfer information
////////////////////////////////////////////////////////////////////////////////
void LCP_ContactSolver::transfer_impulse_information()
{
	TotalContactList::iterator cp;
	FOR_EACH(total_contacts, cp){
		Contact& c = **cp;
		//statistics_add("u_n", 4, -c.u_n);
		if(-c.u_n <= hit_reaction_velocity_threshould)
			continue;
		//static StaticStatisticalAnalizator imp("imp");
		//imp.add(c.j);
		if(c.body2){
			static_cast<Body*>(c.body1) -> body_hit_reaction(static_cast<Body&>(*c.body2), c);
			static_cast<Body*>(c.body2) -> body_hit_reaction(static_cast<Body&>(*c.body1), c);
			}
		else
			static_cast<Body*>(c.body1) -> ground_hit_reaction(c);
		}
}

void LCP_ContactSolver::transfer_force_information()
{
	TotalContactList::iterator cp;
	FOR_EACH(total_contacts, cp){
		Contact& c = **cp;
		if(c.f_n <= press_reaction_force_threshould)
			continue;
		//static StaticStatisticalAnalizator imp("imp");
		//imp.add(c.j);
//		if(c.body2){
//			c.body1 -> body_press_reaction(*c.body2, c);
//			c.body2 -> body_press_reaction(*c.body1, c);
//			}
//		else
//			c.body1 -> ground_press_reaction(c);
		}
}
