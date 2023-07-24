// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology

#ifndef QR_H
#define QR_H

// Classical QR factorization example, based on Stewart[1973].
//
//
// This algorithm computes the factorization of a matrix A
// into a product of an orthognal matrix (Q) and an upper triangular
// matrix (R), such that QR = A.
//
// Parameters:
//
//  A	(in):	    Matrix(1:N, 1:N)
//
//  Q	(output):   Matrix(1:N, 1:N), collection of Householder
//			column vectors Q1, Q2, ... QN
//
//  R	(output):   upper triangular Matrix(1:N, 1:N)
//
// Returns:
//
//  0 if successful, 1 if A is detected to be singular
//


#include <cmath>      //for sqrt() & fabs()
#include "TNTMATH.H"  // for sign()

// Classical QR factorization, based on Stewart[1973].
//
//
// This algorithm computes the factorization of a matrix A
// into a product of an orthognal matrix (Q) and an upper triangular
// matrix (R), such that QR = A.
//
// Parameters:
//
//  A	(in/out):  On input, A is square, Matrix(1:N, 1:N), that represents
//		    the matrix to be factored.
//
//		   On output, Q and R is encoded in the same Matrix(1:N,1:N)
//		   in the following manner:
//
//		    R is contained in the upper triangular section of A,
//		    except that R's main diagonal is in D.  The lower
//		    triangular section of A represents Q, where each
//		    column j is the vector  Qj = I - uj*uj'/pi_j.
//
//  C  (output):    vector of Pi[j]
//  D  (output):    main diagonal of R, i.e. D(i) is R(i,i)
//
// Returns:
//
//  0 if successful, 1 if A is detected to be singular
//

namespace TNT
{

template <class MatrixType, class Vector>
int QR_factor(MatrixType &A, Vector& C, Vector &D)
{
    assert(A.lbound() == 1);	    // ensure these are all
    assert(C.lbound() == 1);	    // 1-based arrays and vectors
    assert(D.lbound() == 1);

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(M == N);		    // make sure A is square

    Subscript i,j,k;
    typename MatrixType::element_type eta, sigma, sum;

    // adjust the shape of C and D, if needed...

    if (N != C.size())	C.newsize(N);
    if (N != D.size())	D.newsize(N);

    for (k=1; k<N; k++)
    {
	// eta = max |M(i,k)|,	for k <= i <= n
	//
	eta = 0;
	for (i=k; i<=N; i++)
	{
	    double absA = fabs(A(i,k));
	    eta = ( absA >  eta ? absA : eta );
	}

	if (eta == 0)		// matrix is singular
	{
	    std::cerr << "QR: k=" << k << "\n";
	    return 1;
	}

	// form Qk and premiltiply M by it
	//
	for(i=k; i<=N; i++)
	    A(i,k)  /=	eta;

	sum = 0;
	for (i=k; i<=N; i++)
	    sum += A(i,k)*A(i,k);
	sigma = sign(A(k,k)) *	sqrt(sum);

	A(k,k) += sigma;
	C(k) = sigma * A(k,k);
	D(k) = -eta * sigma;

	for (j=k+1; j<=N; j++)
	{
	    sum = 0;
	    for (i=k; i<=N; i++)
		sum += A(i,k)*A(i,j);
	    sum /= C(k);

	    for (i=k; i<=N; i++)
		A(i,j) -= sum * A(i,k);
	}

	D(N) = A(N,N);
    }

    return 0;
}

// modified form of upper triangular solve, except that the main diagonal
// of R (upper portion of A) is in D.
//
template <class MatrixType, class Vector>
int R_solve(const MatrixType &A, /*const*/ Vector &D, Vector &b)
{
    assert(A.lbound() == 1);	    // ensure these are all
    assert(D.lbound() == 1);	    // 1-based arrays and vectors
    assert(b.lbound() == 1);

    Subscript i,j;
    Subscript N = A.num_rows();

    assert(N == A.num_cols());
    assert(N == D.dim());
    assert(N == b.dim());

    typename MatrixType::element_type sum;

    if (D(N) == 0)
	return 1;

    b(N) = b(N) /
	    D(N);

    for (i=N-1; i>=1; i--)
    {
	if (D(i) == 0)
	    return 1;
	sum = 0;
	for (j=i+1; j<=N; j++)
	    sum = sum + A(i,j)*b(j);
	b(i) = ( b(i) - sum ) /
	    D(i);
    }

    return 0;
}


template <class MatrixType, class Vector>
int QR_solve(const MatrixType &A, const Vector &c, /*const*/ Vector &d,
	Vector &b)
{
    assert(A.lbound() == 1);	    // ensure these are all
    assert(c.lbound() == 1);	    // 1-based arrays and vectors
    assert(d.lbound() == 1);

    Subscript N=A.num_rows();

    assert(N == A.num_cols());
    assert(N == c.dim());
    assert(N == d.dim());
    assert(N == b.dim());

    Subscript i,j;
    typename MatrixType::element_type sum, tau;

    for (j=1; j<N; j++)
    {
	// form Q'*b
	sum = 0;
	for (i=j; i<=N; i++)
	    sum = sum + A(i,j)*b(i);
	if (c(j) == 0)
	    return 1;
	tau = sum / c(j);
       for (i=j; i<=N; i++)
	    b(i) = b(i) - tau * A(i,j);
    }
    return R_solve(A, d, b);	    // solve Rx = Q'b
}

} // namespace TNT

#endif
// QR_H
