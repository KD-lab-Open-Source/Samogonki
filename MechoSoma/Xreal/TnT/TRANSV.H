// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Matrix Transpose Views

#ifndef TRANSV_H
#define TRANSV_H

#include <iostream>
#include <cassert>
#include "VEC.H"

namespace TNT
{

template <class Array2D>
class Transpose_View
{
    protected:

	const Array2D &  A_;

    public:

	typedef typename Array2D::element_type T;
	typedef 	T   value_type;
	typedef 	T   element_type;
	typedef 	T*  pointer;
	typedef 	T*  iterator;
	typedef 	T&  reference;
	typedef const	T*  const_iterator;
	typedef const	T&  const_reference;


	const Array2D & array()  const { return A_; }
	Subscript num_rows() const { return A_.num_cols();}
	Subscript num_cols() const { return A_.num_rows();}
	Subscript lbound() const { return A_.lbound(); }
	Subscript dim(Subscript i) const
	{
#ifdef TNT_BOUNDS_CHECK
	    assert( A_.lbound() <= i);
	    assert( i<= A_.lbound()+1);
#endif
	    if (i== A_.lbound())
		return num_rows();
	    else
		return num_cols();
	}


	Transpose_View(const Transpose_View<Array2D> &A) : A_(A.A_) {};
	Transpose_View(const Array2D &A) : A_(A) {};


	inline const typename Array2D::element_type & operator()(
	    Subscript i, Subscript j) const
	{
#ifdef TNT_BOUNDS_CHECK
	assert(lbound()<=i);
	assert(i<=A_.num_cols() + lbound() - 1);
	assert(lbound()<=j);
	assert(j<=A_.num_rows() + lbound() - 1);
#endif

	    return A_(j,i);
	}


};

template <class Matrix>
Transpose_View<Matrix> Transpose_view(const Matrix &A)
{
    return Transpose_View<Matrix>(A);
}

template <class Matrix, class T>
Vector<T> matmult(
    const Transpose_View<Matrix> & A,
    const Vector<T> &B)
{
    Subscript  M = A.num_rows();
    Subscript  N = A.num_cols();

    assert(B.dim() == N);

    Vector<T> x(N);

    Subscript i, j;
    T tmp = 0;

    for (i=1; i<=M; i++)
    {
	tmp = 0;
	for (j=1; j<=N; j++)
	    tmp += A(i,j) * B(j);
	x(i) = tmp;
    }

    return x;
}

template <class Matrix, class T>
inline Vector<T> operator*(const Transpose_View<Matrix> & A, const Vector<T> &B)
{
    return matmult(A,B);
}


template <class Matrix>
ostream& operator<<(ostream &s, const Transpose_View<Matrix> &A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() - 1;
    Subscript Nend = N + A.lbound() - 1;

    s << M << "  " << N << endl;
    for (Subscript i=start; i<=Mend; i++)
    {
	for (Subscript j=start; j<=Nend; j++)
	{
	    s << A(i,j) << " ";
	}
	s << endl;
    }


    return s;
}

} // namespace TNT

#endif
// TRANSV_H
