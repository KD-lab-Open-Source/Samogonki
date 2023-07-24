
#ifndef __LCP_CONTACTS_H__
#define __LCP_CONTACTS_H__

#include "LCP_Solver.h"
#include "Body.h"
#include "MultibodyContact.h"

class LCP_ContactSolver : public LCP_Solver
{
public:

	void resolve_impulse_and_force(ContactingBodies& bodies);

private:
	void transfer_impulse_information();
	void transfer_force_information();

	int compute_frictionless_impulses();
	int compute_frictionless_forces();

	int compute_frictional_impulses();
	int compute_frictional_forces();

	int compute_frictional_forces(int enable_dynamic_friction);

	void calculate_M();
	void calculate_M_frictional();

	ContactingBodies* bodies_;
	TotalContactList total_contacts;
	TotalContactList frictional_contacts;
	std::vector<int> frictional_contacts_indices;
	int M_is_calculated;
	int frictional_bodies;
};

extern LCP_ContactSolver LCP_solver;

#endif  // __LCP_CONTACTS_H__
