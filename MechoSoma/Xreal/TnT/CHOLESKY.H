// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



#ifndef CHOLESKY_H
#define CHOLESKY_H

#include <cmath>

// index method

namespace TNT
{

#if 1

// scalar point method
//
//
// Only upper part of A is used.  Cholesky factor is returned in
// lower part of L.
//
template <class SPDMatrix, class SymmMatrix>
int Cholesky_upper_factorization(SPDMatrix &A, SymmMatrix &L)
{
    Subscript M = A.dim(1);
    Subscript N = A.dim(2);

    assert(M == N);		    // make sure A is square

    // readjust size of L, if necessary

    if (M != L.dim(1) || N != L.dim(2))
	L = SymmMatrix(N,N);

    Subscript i,j,k;


    typename SPDMatrix::element_type dot=0;


    for (j=1; j<=N; j++)		// form column j of L
    {
	dot= 0;

	for (i=1; i<j; i++)		// for k= 1 TO j-1
	    dot = dot +  L(j,i)*L(j,i);

	L(j,j) = A(j,j) - dot;

	for (i=j+1; i<=N; i++)
	{
	    dot = 0;
	    for (k=1; k<j; k++)
		dot = dot +  L(i,k)*L(j,k);
	    L(i,j) = A(j,i) - dot;
	}

	if (L(j,j) <= 0.0) return 1;

	L(j,j) = sqrt( L(j,j) );

	for (i=j+1; i<=N; i++)
	    L(i,j) = L(i,j) / L(j,j);

    }

    return 0;
}

#else	    /* use vector/matrix index regions */

template <class Matrix, class Vector>
void Cholesky_lower_factorization(Matrix &A, Vector &b)
{
    Subscript m = A.dim(1);
    Subscript n = A.dim(2);
    Subscript i,j;

    Subscript N = (m < n ? n : m);	    // K = min(M,N);
    assert( N <= b.dim() );


    for (j=1; j<=N; j++)
    {
	Index J(1, j-1);

	L(j,j) = A(j,j) - dot_product(L(j,J), L(j,J));
	for (i=j+1; j<=N; j++)
	    L(i,j) = A(j,i) - dot_product( L(i,J), L(j,J));
	L(j,j) = sqrt( L(j,j) );
	for (i=j+1; j<=N; j++)
	    L(i,j) = L(i,j) / L(j,j);

    }

}


#endif
// TNT_USE_REGIONS

}  // namespace TNT

#endif
// CHOLESKY_H
