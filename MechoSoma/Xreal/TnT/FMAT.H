// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Fortran-compatible matrix: column oriented, 1-based (i,j) indexing

#ifndef FMAT_H
#define FMAT_H

#include "SUBSCRPT.H"
#include "VEC.H"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <strstream>
#ifdef TNT_USE_REGIONS
#include "REGION2D.H"
#endif

// simple 1-based, column oriented Matrix class

namespace TNT
{

template <class T>
class Fortran_Matrix
{


  public:

    typedef	    T	value_type;
    typedef	    T	element_type;
    typedef	    T*	pointer;
    typedef	    T*	iterator;
    typedef	    T&	reference;
    typedef const   T*	const_iterator;
    typedef const   T&	const_reference;

    Subscript lbound() const { return 1;}

  protected:
    T* v_;		    // these are adjusted to simulate 1-offset
    Subscript m_;
    Subscript n_;
    T** col_;		// these are adjusted to simulate 1-offset

    // internal helper function to create the array
    // of row pointers

    void initialize(Subscript M, Subscript N)
    {
	// adjust col_[] pointers so that they are 1-offset:
	//   col_[j][i] is really col_[j-1][i-1];
	//
	// v_[] is the internal contiguous array, it is still 0-offset
	//
#ifndef TNT_MEM_HEAP
	v_ = new T[M*N];
	col_ = new T*[N];
#else
	v_ = new(tnt_mem_heap) T[M*N];
	col_ = new(tnt_mem_heap) T*[N];
#endif

	assert(v_  != NULL);
	assert(col_ != NULL);


	m_ = M;
	n_ = N;
	T* p = v_ - 1;
	for (Subscript i=0; i<N; i++)
	{
	    col_[i] = p;
	    p += M ;

	}
	col_ --;
    }

    void copy(const T*	v)
    {
	Subscript N = m_ * n_;
	Subscript i;

#ifdef TNT_UNROLL_LOOPS
	Subscript Nmod4 = N & 3;
	Subscript N4 = N - Nmod4;

	for (i=0; i<N4; i+=4)
	{
	    v_[i] = v[i];
	    v_[i+1] = v[i+1];
	    v_[i+2] = v[i+2];
	    v_[i+3] = v[i+3];
	}

	for (i=N4; i< N; i++)
	    v_[i] = v[i];
#else

	for (i=0; i< N; i++)
	    v_[i] = v[i];
#endif
    }

    void set(const T& val)
    {
	Subscript N = m_ * n_;
	Subscript i;

#ifdef TNT_UNROLL_LOOPS
	Subscript Nmod4 = N & 3;
	Subscript N4 = N - Nmod4;

	for (i=0; i<N4; i+=4)
	{
	    v_[i] = val;
	    v_[i+1] = val;
	    v_[i+2] = val;
	    v_[i+3] = val;
	}

	for (i=N4; i< N; i++)
	    v_[i] = val;
#else

	for (i=0; i< N; i++)
	    v_[i] = val;

#endif
    }



    void destroy()
    {
	/* do nothing, if no memory has been previously allocated */
	if (v_ == NULL) return ;

	/* if we are here, then matrix was previously allocated */
#ifndef TNT_MEM_HEAP
	delete [] (v_);
	col_ ++;		// changed back to 0-offset
	delete [] (col_);
#endif
	v_ = NULL;
    }


  public:

    T* begin() { return v_; }
    const T* begin() const { return v_;}

    T* end() { return v_ + m_*n_; }
    const T* end() const { return v_ + m_*n_; }


    // constructors

    Fortran_Matrix() : v_(0), m_(0), n_(0), col_(0)  {};
    Fortran_Matrix(const Fortran_Matrix<T> &A)
    {
	initialize(A.m_, A.n_);
	copy(A.v_);
    }

    Fortran_Matrix(Subscript M, Subscript N, const T& value = T(0))
    {
	initialize(M,N);
	set(value);
    }

    Fortran_Matrix(Subscript M, Subscript N, const T* v)
    {
	initialize(M,N);
	copy(v);
    }


    Fortran_Matrix(Subscript M, Subscript N, char *s)
    {
	initialize(M,N);
	std::istrstream ins(s);

	Subscript i, j;

	for (i=1; i<=M; i++)
	    for (j=1; j<=N; j++)
		ins >> (*this)(i,j);
    }

    // destructor
    ~Fortran_Matrix()
    {
	destroy();
    }


    // assignments
    //
    Fortran_Matrix<T>& operator=(const Fortran_Matrix<T> &A)
    {
	if (v_ == A.v_)
	    return *this;

	if (m_ == A.m_	&& n_ == A.n_)	    // no need to re-alloc
	    copy(A.v_);

	else
	{
	    destroy();
	    initialize(A.m_, A.n_);
	    copy(A.v_);
	}

	return *this;
    }

    Fortran_Matrix<T>& operator=(const T& scalar)
    {
	set(scalar);
	return *this;
    }


    Subscript dim(Subscript d) const
    {
#ifdef TNT_BOUNDS_CHECK
       assert( d >= 1);
	assert( d <= 2);
#endif
	return (d==1) ? m_ : ((d==2) ? n_ : 0);
    }

    Subscript num_rows() const { return m_; }
    Subscript num_cols() const { return n_; }

    Fortran_Matrix<T>& newsize(Subscript M, Subscript N)
    {
#ifndef TNT_MEM_HEAP
	if (num_rows() == M && num_cols() == N)
	   return *this;
#endif

	destroy();
	initialize(M,N);

	return *this;
    }



    // 1-based element access
    //
    inline reference operator()(Subscript i, Subscript j)
    {
#ifdef TNT_BOUNDS_CHECK
	assert(1<=i);
	assert(i <= m_) ;
	assert(1<=j);
	assert(j <= n_);
#endif
	return col_[j][i];
    }

    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
	assert(1<=i);
	assert(i <= m_) ;
	assert(1<=j);
	assert(j <= n_);
#endif
	return col_[j][i];
    }

    friend istream& operator>> (istream &s, Fortran_Matrix<T> &A);

#ifdef TNT_USE_REGIONS

    typedef Region2D<Fortran_Matrix<T> > Region;
    typedef const_Region2D< Fortran_Matrix<T> > const_Region;

    Region operator()(const Index1D &I, const Index1D &J)
    {
	return Region(*this, I,J);
    }

    const_Region operator()(const Index1D &I, const Index1D &J) const
    {
	return const_Region(*this, I,J);
    }

#endif


};


/* ***************************	I/O  ********************************/

template <class T>
ostream& operator<<(ostream &s, const Fortran_Matrix<T> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << "\n";

    for (Subscript i=1; i<=M; i++)
    {
	for (Subscript j=1; j<=N; j++)
	{
	    s << A(i,j) << " ";
	}
	s << "\n";
    }


    return s;
}

template <class T>
istream& operator>>(istream &s, Fortran_Matrix<T> &A)
{

    Subscript M, N;

    s >> M >> N;

    if ( !(M == A.m_ && N == A.n_) )
    {
	A.destroy();
	A.initialize(M,N);
    }


    for (Subscript i=1; i<=M; i++)
	for (Subscript j=1; j<=N; j++)
	{
	    s >>  A(i,j);
	}


    return s;
}

// *******************[ basic matrix algorithms ]***************************


template <class T>
Fortran_Matrix<T> operator+(const Fortran_Matrix<T> &A,
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
	for (j=1; j<=N; j++)
	    tmp(i,j) = A(i,j) + B(i,j);

    return tmp;
}

template <class T>
Fortran_Matrix<T> operator-(const Fortran_Matrix<T> &A,
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
	for (j=1; j<=N; j++)
	    tmp(i,j) = A(i,j) - B(i,j);

    return tmp;
}

// element-wise multiplication	(use matmult() below for matrix
// multiplication in the linear algebra sense.)
//
//
template <class T>
Fortran_Matrix<T> mult_element(const Fortran_Matrix<T> &A,
    const Fortran_Matrix<T> &B)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M==B.num_rows());
    assert(N==B.num_cols());

    Fortran_Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=1; i<=M; i++)
	for (j=1; j<=N; j++)
	    tmp(i,j) = A(i,j) * B(i,j);

    return tmp;
}


template <class T>
Fortran_Matrix<T> transpose(const Fortran_Matrix<T> &A)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Fortran_Matrix<T> S(N,M);
    Subscript i, j;

    for (i=1; i<=M; i++)
	for (j=1; j<=N; j++)
	    S(j,i) = A(i,j);

    return S;
}



template <class T>
inline Fortran_Matrix<T> matmult(const Fortran_Matrix<T>  &A,
    const Fortran_Matrix<T> &B)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == B.num_rows());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    Fortran_Matrix<T> tmp(M,K);
    T sum;

    for (Subscript i=1; i<=M; i++)
    for (Subscript k=1; k<=K; k++)
    {
	sum = 0;
	for (Subscript j=1; j<=N; j++)
	    sum = sum +  A(i,j) * B(j,k);

	tmp(i,k) = sum;
    }

    return tmp;
}

template <class T>
inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T> &A,
    const Fortran_Matrix<T> &B)
{
    return matmult(A,B);
}

template <class T>
inline int matmult(Fortran_Matrix<T>& C, const Fortran_Matrix<T>  &A,
    const Fortran_Matrix<T> &B)
{

    assert(A.num_cols() == B.num_rows());

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    C.newsize(M,K);	    // adjust shape of C, if necessary


    T sum;

    const T* row_i;
    const T* col_k;

    for (Subscript i=1; i<=M; i++)
    {
	for (Subscript k=1; k<=K; k++)
	{
	    row_i = &A(i,1);
	    col_k = &B(1,k);
	    sum = 0;
	    for (Subscript j=1; j<=N; j++)
	    {
		sum +=	*row_i * *col_k;
		row_i += M;
		col_k ++;
	    }

	    C(i,k) = sum;
	}

    }

    return 0;
}


template <class T>
Vector<T> matmult(const Fortran_Matrix<T>  &A, const Vector<T> &x)
{

#ifdef TNT_BOUNDS_CHECK
    assert(A.num_cols() == x.dim());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Vector<T> tmp(M);
    T sum;

    for (Subscript i=1; i<=M; i++)
    {
	sum = 0;
	for (Subscript j=1; j<=N; j++)
	    sum = sum +  A(i,j) * x(j);

	tmp(i) = sum;
    }

    return tmp;
}

template <class T>
inline Vector<T> operator*(const Fortran_Matrix<T>  &A, const Vector<T> &x)
{
    return matmult(A,x);
}

template <class T>
inline Fortran_Matrix<T> operator*(const Fortran_Matrix<T>  &A, const T &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Subscript MN = M*N;

    Fortran_Matrix<T> res(M,N);
    const T* a = A.begin();
    T* t = res.begin();
    T* tend = res.end();

    for (t=res.begin(); t < tend; t++, a++)
	*t = *a * x;

    return res;
}

}  // namespace TNT
#endif
// FMAT_H
