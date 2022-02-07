// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Triangular Solves

#ifndef TRISLV_H
#define TRISLV_H


#include "TRIANG.H"

namespace TNT
{

template <class MaTriX, class VecToR>
VecToR Lower_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    Subscript N = A.num_rows();

    // make sure matrix sizes agree; A must be square

    assert(A.num_cols() == N);
    assert(b.dim() == N);

    VecToR x(N);

    Subscript i;
    for (i=1; i<=N; i++)
    {
	typename MaTriX::element_type tmp=0;

	for (Subscript j=1; j<i; j++)
		tmp = tmp + A(i,j)*x(j);

	x(i) =	(b(i) - tmp)/ A(i,i);
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR Unit_lower_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    Subscript N = A.num_rows();

    // make sure matrix sizes agree; A must be square

    assert(A.num_cols() == N);
    assert(b.dim() == N);

    VecToR x(N);

    Subscript i;
    for (i=1; i<=N; i++)
    {

	typename MaTriX::element_type tmp=0;

	for (Subscript j=1; j<i; j++)
		tmp = tmp + A(i,j)*x(j);

	x(i) =	b(i) - tmp;
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ LowerTriangularView<MaTriX> &A,
	    /*const*/ VecToR &b)
{
    return Lower_triangular_solve(A, b);
}

template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ UnitLowerTriangularView<MaTriX> &A,
	/*const*/ VecToR &b)
{
    return Unit_lower_triangular_solve(A, b);
}



//********************** Upper triangular section ****************

template <class MaTriX, class VecToR>
VecToR Upper_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    Subscript N = A.num_rows();

    // make sure matrix sizes agree; A must be square

    assert(A.num_cols() == N);
    assert(b.dim() == N);

    VecToR x(N);

    Subscript i;
    for (i=N; i>=1; i--)
    {

	typename MaTriX::element_type tmp=0;

	for (Subscript j=i+1; j<=N; j++)
		tmp = tmp + A(i,j)*x(j);

	x(i) =	(b(i) - tmp)/ A(i,i);
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR Unit_upper_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    Subscript N = A.num_rows();

    // make sure matrix sizes agree; A must be square

    assert(A.num_cols() == N);
    assert(b.dim() == N);

    VecToR x(N);

    Subscript i;
    for (i=N; i>=1; i--)
    {

	typename MaTriX::element_type tmp=0;

	for (Subscript j=i+1; j<i; j++)
		tmp = tmp + A(i,j)*x(j);

	x(i) =	b(i) - tmp;
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ UpperTriangularView<MaTriX> &A,
	/*const*/ VecToR &b)
{
    return Upper_triangular_solve(A, b);
}

template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ UnitUpperTriangularView<MaTriX> &A,
    /*const*/ VecToR &b)
{
    return Unit_upper_triangular_solve(A, b);
}


} // namespace TNT

#endif
// TRISLV_H
