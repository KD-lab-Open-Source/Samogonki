// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Header file to define C/Fortran conventions (Platform specific)

#ifndef FORTRAN_H
#define FORTRAN_H

// help map between C/C++ data types and Fortran types

namespace TNT
{
    typedef int     Fortran_integer;
    typedef float   Fortran_float;
    typedef double  Fortran_double;
}

#ifndef TNT_SUBSCRIPT_TYPE
#define TNT_SUBSCRIPT_TYPE TNT::Fortran_integer
#endif


#endif
// FORTRAN_H
