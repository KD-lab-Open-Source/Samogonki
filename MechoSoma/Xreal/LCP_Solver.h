#ifndef __LCP_SOLVER_H__
#define __LCP_SOLVER_H__
#include <iostream>
//////////////////////////////////////////////
//	Control defines
//////////////////////////////////////////////
// TODO: @caiiiycuk inevstigate this
#ifdef WTF
#define TNT_MEM_HEAP  200000
#define TNT_NO_BOUNDS_CHECK
#endif
//#define _LCPSolverListing_
//#define _LCPSolverTest_

//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript declaration
//	Section: LCPSolverPrm
//	Number of variables: 5
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
extern double LCP_diagonal_element_scaling_factor;
extern double LCP_delta_tolerance;
extern double LCP_tangential_zero_threshould;
extern int LCP_max_iterations;
extern int LCP_solve_method;
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: LCPSolverPrm
//////////////////////////////////////////////////////////////////////////////////////////////


#ifdef TNT_MEM_HEAP
#include "MemoryHeap.h"
extern MemoryHeap tnt_mem_heap;
#endif

// Logging
//extern class ofstream lout;
#ifdef _MECHOSOMA_
#undef _LCPSolverTest_
#ifdef _FINAL_VERSION_
#undef _LCPSolverListing_
#endif
#endif

#include "TNT.H"
#include "VEC.H"
#include "FMAT.H"


using namespace TNT;
class Mat : public Fortran_Matrix<double>
{
public:
	Mat(){}
	Mat(Subscript M, Subscript N, double f = 0) : Fortran_Matrix<double>(M, N, f){}
	Mat& operator=(const double& scalar) { set(scalar); return *this; }
};
class Mass_Matrix_Array : public Fortran_Matrix<Mat3f>
{
public:
	Mass_Matrix_Array(Subscript M) : Fortran_Matrix<Mat3f>(M, M, Mat3f::ZERO){}
#ifdef TNT_BOUNDS_CHECK
	inline const_reference operator() (Subscript i, Subscript j) const { assert(i <= j); return Fortran_Matrix<Mat3f>::operator()(i, j); }
	inline reference operator()(Subscript i, Subscript j){ assert(i <= j); return Fortran_Matrix<Mat3f>::operator()(i, j); }
#endif
};

//////////////////////////////////////////////////////////////////
//	LCP_Solver
//////////////////////////////////////////////////////////////////
class LCP_Solver
{
protected:
	enum ContactProperty {	
			_CLEAN_ = 0, // ������ �������
			_C_ = 1,	   // "Clamped" - ��������������� �������: f > 0, (a == 0)	
			_NC_ = 2,	 // "NonClamped" - ������������� �������: f == 0, (a > 0)

			_CLEAN_F_ = 4, // ������ �������, ������
			
			_CF_ = 8,	 // "ClampedFriction" 
			_NCF_ = 16,	 // "NonClampedFriction" 
			_P_ = 1,	// "+"
			_M_ = 2,	// "-"

			_NCP_ = _NCF_ | _P_,	 // "NonClamped +": f == k*fn, a.< 0
			_CP_ = _CF_ | _P_,	   // "Clamped +" - ��������������� ��� ������: 0 < f < k*fn, (a == 0)	
			_CM_ = _CF_ | _M_,	   // "Clamped -" - ��������������� ��� ������: -k*fn < f < 0, (a == 0)	
			_NCM_ = _NCF_ | _M_	 // "NonClamped -": f == -k*fn, a.> 0
		};

	typedef Vector<double> Vect;

public:

	LCP_Solver();

	void save_problem(char* name);
	void save_problem(ofstream& f);

#ifdef _LCPSolverTest_
	void load_problem(char* name);
#endif

protected:

	// frictionless	
	void init(int size);
	void compute_force();
	void drive_to_zero(int i);
	void fdirection(int i);
	double maxstep(int i, int& index);
	
	// frictional
	void init_frictional(int size);
	void finit_frictional();
	void compute_force_frictional();
	void drive_to_zero_frictional(int i);
	void fdirection_frictional(int i);
	double maxstep_frictional(int i, int& index, ContactProperty& is_friction_pivots);

	//  utils
	double energy(const double* vels);
	double det(const Mat& A);
	Mat fullMatrix();
	void abort(const char* msg); // throw
	void check_solution();
	void print_state();

	// Linear Solvers
	void solve(Mat& A, Vect& B);
	void solveQR(Mat& A, Vect& B);
	void solveLU(Mat& A, Vect& B);
	void solveCholesky(Mat& A, Vect& B);
	void solveSVD(Mat& A, Vect& B);

	// Memory heap utils
	#ifdef TNT_MEM_HEAP
	void mem_init() {  tnt_mem_heap.init(); }
	void mem_store() {  tnt_mem_heap.store(); }
	void mem_restore() {  tnt_mem_heap.restore(); }
	#else
	void mem_init() {}
	void mem_store() {}
	void mem_restore() {}
	#endif

	////////////////////////////////////////////
	//	Frictionless part
	////////////////////////////////////////////
	//	a = M*f + q >= 0 
	//	a >= 0, f >= 0, a*f = 0
	//
	int size;
	Mat M;	
	Vect q; 

	int C_size;
	Vector<int> prp;
	Vect f, a;
	Vect df, da;

	////////////////////////////////////////////
	//	Frictional addition
	////////////////////////////////////////////
	//		T
	//	|a_n|		|   M    Mnt  |    |f_n|
	//	|      |    =	 |		     | * |     |
	//	|a_t |		 |  Mtn   Mtt  |    |f_t |
	//
	int size_frictional;
	Mat Mnt, Mtn, Mtt;	
	Vect q_t; 
	Vect mu_t;
	Vector<int> fn_index; // t -> n
	int ft_index(int n);  // n -> t

	double a_t_zero_threshould;

	int C_size_frictional;
	Vector<int> prp_t;
	Vect f_t, a_t;
	Vect df_t, da_t;
	int drive_friction;

	////////////////////////////////////////////
	//	utils
	////////////////////////////////////////////
	int iterations;
	int init_counter;
public:
#ifdef _LCPSolverListing_
	ofstream lout;
#endif
};

#endif  // __LCP_SOLVER_H__
