
template <int n, class real>
void RungeKutta4(real* coords, real* velocity, real const* input_coords, real const* mass_inv, real dt, real damping_factor = 1.9)
{
	typedef real real_v[n];

	// Constants
	const real h = dt;
	const real h2 = h/2.f;
	const real h3 = h/3.f;
	const real h6 = h/6.f;

	// Calc damping, initialization
	real_v damping;
	real_v R0, V0, R1, V1, R, V, dV;
	int i;
	for(i = 0; i < n; i++){
		damping[i] = (real)sqrt(mass_inv[i])*damping_factor;
		R[i] = R1[i] = R0[i] = coords[i];
		V[i] = V1[i] = V0[i] = velocity[i];
		}

	// First iteration
	for(i = 0; i < n; i++){
		dV[i] = (input_coords[i] - R[i])*mass_inv[i] - V[i]*damping[i];
		R1[i] += V[i]*h6;
		V1[i] += dV[i]*h6;
		}

	// Second iteration
	for(i = 0; i < n; i++){
		R[i] = R0[i] + V[i]*h2;
		V[i] = V0[i] + dV[i]*h2;
		dV[i] = (input_coords[i] - R[i])*mass_inv[i] - V[i]*damping[i];
		R1[i] += V[i]*h6;
		V1[i] += dV[i]*h6;
		}

	// Third iteration
	for(i = 0; i < n; i++){
		R[i] = R0[i] + V[i]*h2;
		V[i] = V0[i] + dV[i]*h2;
		dV[i] = (input_coords[i] - R[i])*mass_inv[i] - V[i]*damping[i];
		R1[i] += V[i]*h3;
		V1[i] += dV[i]*h3;
		}

	// Fourth iteration
	for(i = 0; i < n; i++){
		R[i] = R0[i] + V[i]*h;
		V[i] = V0[i] + dV[i]*h;
		dV[i] = (input_coords[i] - R[i])*mass_inv[i] - V[i]*damping[i];
		R1[i] += V[i]*h6;
		V1[i] += dV[i]*h6;
		}

	for(i = 0; i < n; i++){
		coords[i] = R1[i];
		velocity[i] = V1[i];
		}
}
