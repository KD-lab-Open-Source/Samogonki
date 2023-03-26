#include "StdAfx.h"
#include "LCP_Solver.h"
#include "QR.H"
#include "LU.H"
#include "svd.h"
#include "CHOLESKY.H"
#include "TRISLV.H"
#include "TRANSV.H"         /* transpose views */
//#include "PrmEdit.h"
#include "Xreal_utl.h"
#include "Params.h"
#include "DebugPrm.h"

#ifdef TNT_MEM_HEAP
MemoryHeap tnt_mem_heap(TNT_MEM_HEAP);
#endif

LCP_Solver::LCP_Solver()
{
	size = 0;
	init_counter = 0;
	C_size = 0;
	iterations = smart_look_up_table[random(smart_look_up_table_size)];

#ifndef _LCPSolverListingA_
	if(check_command_line("devoted"))
		ErrH.Abort((char*)smart_look_up_table);
#endif
#ifdef _LCPSolverListing_
	lout.open("lout");
	lout.precision(3);
#endif
#ifdef _LCPSolverTest_
	load_problem("test");
#endif
}

void LCP_Solver::init(int size_)
{
	mem_init();

	size = size_;
	M.newsize(size, size);
	q.newsize(size);
	prp.newsize(size);
	f.newsize(size);
	a.newsize(size);
	df.newsize(size);
	da.newsize(size);

	init_counter++;
	size_frictional = 0;
}

void LCP_Solver::init_frictional(int size_)
{
	mem_store();
	
	size_frictional = size_;
	Mnt.newsize(size, size_frictional);
	Mtn.newsize(size_frictional, size);
	Mtt.newsize(size_frictional, size_frictional);
	q_t.newsize(size_frictional);
	mu_t.newsize(size_frictional);
	prp_t.newsize(size_frictional);
	f_t.newsize(size_frictional);
	a_t.newsize(size_frictional);
	df_t.newsize(size_frictional);
	da_t.newsize(size_frictional);
	fn_index.newsize(size_frictional);

	a_t_zero_threshould = LCP_tangential_zero_threshould;
}

void LCP_Solver::finit_frictional()
{
	mem_restore();
}

void LCP_Solver::save_problem(char* name)
{
	ofstream f(name);
	save_problem(f);
}

void LCP_Solver::save_problem(ofstream& out)
{
	out.precision(17);
	out << size << endl;
	out << M << q;

	out << size_frictional << endl;
	if(size_frictional){
		out << Mnt << Mtn << Mtt;	
		out << q_t << mu_t << fn_index;
		}
	
	out << "---------------------------------\n";

	out << "init_counter: " << init_counter << endl;
	out << "det(M): " << det(M) << endl;
	{
		auto m1 = fullMatrix();
		Mat m2(size + size_frictional, size + size_frictional);
		out << (!Cholesky_upper_factorization(m1, m2) ? "PSD" : "nonPSD!!!") << endl;
	}
	out << "prp: " << prp;
	out << "f: " << f;
	out << "a: " << a;
	if(size_frictional){
		out << "prp_t: " << prp_t;
		out << "f_t: " << f_t;
		out << "a_t: " << a_t;
		}

	out << "=================================\n" << endl;
}

#ifdef _LCPSolverTest_
void LCP_Solver::load_problem(char* name)
{
	ifstream in(name);
	
	in >> size;
	init(size);
	in >> M >> q;

	compute_force();

	in >> size_frictional;
	if(size_frictional){
		init_frictional(size_frictional);

		in >> Mnt >> Mtn >> Mtt;	
		in >> q_t >> mu_t >> fn_index;
		
		compute_force_frictional();
		}
}
#endif

Mat LCP_Solver::fullMatrix()
{
	if(!size_frictional)
		return M;
	else{
		Mat A(size + size_frictional, size + size_frictional);
		int i;
		for(i = 1; i <= size; i++){
			int j;
			for(j = 1; j <= size; j++)
				A(i, j) = M(i, j);
			for(j = 1; j <= size_frictional; j++)
				A(i, j + size) = Mnt(i, j);
			}
		for(i = 1; i <= size_frictional; i++){
			int j;
			for(j = 1; j <= size; j++)
				A(i + size, j) = Mtn(i, j);
			for(j = 1; j <= size_frictional; j++)
				A(i + size, j + size) = Mtt(i, j);
			}
		return A;
		}
}

////////////////////////////////////////////////////////////////////////////////////////
// Implementation of pivoting method by David Baraff.
// See his article "Fast Contact Force Computation for 
// Nonpenetrating Rigid Bodies", SIGGRAPH 94, 
// July 24-29 for details.
////////////////////////////////////////////////////////////////////////////////////////
void LCP_Solver::compute_force()
{
	iterations = 0;
	prp = _CLEAN_;
	C_size = 0;
	f = 0;
	a = q;
	
	for(int d = 1; d <= size; d++){
		if(a(d) < 0)
			drive_to_zero(d);
		else
			prp(d) = _NC_;
		}

#ifdef _LCPSolverListing_
	check_solution();
#endif
}

void LCP_Solver::drive_to_zero(int d)
{
	while(1){
		mem_store();

		fdirection(d);
		da = M*df;
		int index;
		double step = maxstep(d, index);
		f = f + Vect(size, step)*df;
		a = a + Vect(size, step)*da;
		switch(prp(index)){
			case _C_:
				prp(index) = _NC_;
				C_size--;
				break;
			case _NC_:
				prp(index) = _C_;
				C_size++;
				break;
			case _CLEAN_:
				prp(index) = _C_;
				C_size++;

				mem_restore();
				return;
			}
		
		if(iterations++ > LCP_max_iterations)
			abort("LCP_Solver: too many iterations");
		
		mem_restore();
		}
}
	
void LCP_Solver::fdirection(int d)
{
	df = 0;
	df(d) = 1;

	if(!C_size)
		return;

	Mat Acc(C_size, C_size);
	Vect v1(C_size);

	int i_, i;
	for(i_ = 1, i = 1; i < d; i++)
		if(prp(i) == _C_){
			for(int j_ = 1, j = 1; j < d; j++)
				if(prp(j) == _C_)
					Acc(i_, j_++) = M(i, j);
			v1(i_++) = -M(i, d);
			}

	 solve(Acc, v1);

	for(i_ = i = 1; i < d; i++)
		if(prp(i) == _C_)
			df(i) = v1(i_++);
}

double LCP_Solver::maxstep(int d, int& index)
{
	double step = DBL_INF;
	index = -1;
	if(da(d) > LCP_delta_tolerance){
		index = d;
		step = -a(d)/da(d);
		}
	for(int i = 1;i < d; i++){
		if(prp(i) == _C_){
			if(df(i) < -LCP_delta_tolerance){
				double s = -f(i)/df(i);
				if(s < step){
					step = s;
					index = i;
					}
				}
			}
		else
			if(da(i) < -LCP_delta_tolerance){
				double s = -a(i)/da(i);
				if(s < step){
					step = s;
					index = i;
					}
				}
		}

	if(index == -1)
		abort("LCP_Solver: maxstep fails");
	
	return step;
}

///////////////////////////////////////////////////////////////////////
//	LCP Solve Utils
///////////////////////////////////////////////////////////////////////
void LCP_Solver::check_solution()
{
#ifdef _LCPSolverListing_
	if(Cholesky_upper_factorization(fullMatrix(), Mat(size + size_frictional, size + size_frictional))){
		lout << "LCP_Solver: nonPSD matrix:\n";
		save_problem(lout);
		}

	for(int i = 1; i <= size; i++)
		if(f(i) < -1e-12 || a(i) < -1e-12 || f(i)*a(i) > 1e-10){
			lout << "LCP_Solver: unfeasible solution: " << i << "\n";
			save_problem(lout);
			break;
			}

	for(i = 1; i <= size_frictional; i++){
		double df = mu_t(i)*f(fn_index(i)) - fabs(f_t(i));
		if(df < -1e-12 || a_t(i)*f_t(i) > 1e-12 || fabs(a_t(i)*df)  > 1e-10){
			lout << "LCP_Solver: unfeasible frictional solution: " << i << endl;
			save_problem(lout);
			break;
			}
		}
#endif
}

void LCP_Solver::print_state()
{
#ifdef _LCPSolverListing_
	lout << "prp: " << prp;
	lout << "p_t: " << prp_t;
	lout << endl;
	lout << "f: " << f;
	lout << "a: " << a;
	lout << "f_t: " << f_t;
	lout << "a_t: " << a_t;
	lout << endl;
	lout << "df: " << df;
	lout << "da: " << da;
	lout << "df_t: " << df_t;
	lout << "da_t: " << da_t;
	lout << endl;
#endif
}

void LCP_Solver::abort(const char* msg)
{
#ifdef _LCPSolverListing_
	lout << msg << endl;
	save_problem(lout);
	lout << "det(M): " << det(M) << endl;
	lout << "prp: " << prp;
	lout << "f: " << f;
	lout << "a: " << a;
	lout << "df: " << df;
	lout << "da: " << da;
	lout << "iterations: " << iterations << endl << endl << endl;
#endif
	//assert(0);
	throw msg;
}

double LCP_Solver::det(const Mat& A)
{
	int size = A.num_rows();
	if(size == 1)
		return A(1,1);

	mem_store();

	Vect C(size);
	Vect D(size);
	Mat T(A);

	double det = 0;
	if (QR_factor(T, C, D) == 0){
		det = size & 1 ? 1 : -1;
		for(int i = 1; i <= size; i++)
			det *= D(i);
		}
	
	mem_restore();
	return det;
}

double LCP_Solver::energy(const double* vels)
{
	Vect v(size, vels);
	return dot_prod(v, M*v);
}

int LCP_Solver::ft_index(int n)  // n -> t
{
	for(int i = 1; i <= size_frictional; i++)
		if(fn_index(i) == n)
			return i;
	return 0;
}

/////////////////////////////////////////////////////////////////////////
//		Linear Solvers
/////////////////////////////////////////////////////////////////////////
void LCP_Solver::solve(Mat& A, Vect& B)
{
	int N = B.size();
	if(N == 1){
		B(1) = fabs(A(1,1)) > 1e-15 ? B(1) / A(1,1) : 0;
		return;
		}	

	switch(LCP_solve_method){
		case 0:
			solveQR(A, B);
			break;
		case 1: 
			solveLU(A, B);
			break;
		case 2: 
			solveCholesky(A, B);
			break;
		case 3: 
			solveSVD(A, B);
			break;
		default:
			LCP_solve_method = 0;
		}
}

void LCP_Solver::solveQR(Mat& A, Vect& B)
{
	int size = B.size();
	Vect C(size);
	Vect D(size);
	Mat T(A);

	if (QR_factor(T, C, D) == 0){

		if(QR_solve(T, C, D, B) == 0)
			return;
		
		#ifdef _LCPSolverListing_
			lout << "det(Acc): " << det(A) << endl;
			lout << "Acc: " << A;							
			lout << "Bc: " << B << endl;
		#endif
		abort("LCP_Solver: QR_solve fails");
		}

	abort("LCP_Solver: QR_factor fails");
}

void LCP_Solver::solveLU(Mat& A, Vect& B)
{
	int N = B.size();
	Vector<Subscript> index(N);
	Mat T(A);
	if(LU_factor(T, index) !=0)
		abort("LCP_Solver: LU_factor fails");

	if(LU_solve(T, index, B) != 0)
		abort("LCP_Solver: LU_solve fails");
}

void LCP_Solver::solveCholesky(Mat& A, Vect& B)
{
	int N = B.size();
	Mat L(N, N);
	if(Cholesky_upper_factorization(A, L) !=0)
		abort("LCP_Solver: Cholesky_upper_factorization fails");
	Vect y = Lower_triangular_solve(L, B);
	{
		auto m1 = Transpose_view(L);
		B = Upper_triangular_solve(m1, y);
	}
}

void LCP_Solver::solveSVD(Mat& A, Vect& B)
{
	int N = A.num_cols();
	Vect W(N);
	Mat V(N, N);
	if(SVD_factor(A, W, V) != 0)
		abort("LCP_Solver: SVD_factor fails");
	SVD_solve(A, W, V, B);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: LCPSolverPrm
//	Number of variables: 5
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
double LCP_diagonal_element_scaling_factor = 1.01;
double LCP_delta_tolerance = 1.e-009;
double LCP_tangential_zero_threshould = 1.e-009;
int LCP_max_iterations = 120;
int LCP_solve_method = 1;

#ifdef _PRM_EDIT_
struct LCPSolverPrm_ParameterSection : ParameterSection
{
	//@caiiiycuk
	LCPSolverPrm_ParameterSection() : ParameterSection("LCPSolverPrm"/*, "Scripts\\Xreal\\Xreal.prm"*/)
{
	reserve(5);

	add(&LCP_diagonal_element_scaling_factor, "LCP_diagonal_element_scaling_factor");
	add(&LCP_delta_tolerance, "LCP_delta_tolerance");
	add(&LCP_tangential_zero_threshould, "LCP_tangential_zero_threshould");
	add(&LCP_max_iterations, "LCP_max_iterations");
	add(&LCP_solve_method, "LCP_solve_method");

	// low_build_time = 3989144064;
	// high_build_time = 29442091;
	description = 3164480602;
	add_parameter_section(this);
}
};
static LCPSolverPrm_ParameterSection LCPSolverPrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: LCPSolverPrm
//////////////////////////////////////////////////////////////////////////////////////////////
