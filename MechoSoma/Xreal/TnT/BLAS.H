// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


#ifndef BLAS_H
#define BLAS_H
#include "fortran.h"


// This file incomplete and included here only to demonstrate the
// basic framework for linking with the Fortran or assembly BLAS.
//

#define fi_ Fortran_integer
#define fd_ Fortran_double
#define ff_ Fortran_float
#define fc_ Fortran_complex
#define fz_ Fortran_double_complex

// these are the Fortran mapping conventions for function names
// (NOTE: if C++ supported extern "Fortran", this wouldn't be necesary...)
//

#define F77_DASUM	dasum_
#define F77_DAXPY	daxpy_
#define F77_CAXPY	cazpy_
#define F77_DDOT	ddot_

extern "C"
{
    fd_ F77_DASUM(const fi_  *N , const fd_ *dx, const fi_* incx);

}

#undef fi_
#undef fd_
#undef ff_
#undef fc_
#undef fz_

#endif
// BLAS_H




