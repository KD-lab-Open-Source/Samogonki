// Template Numerical Toolkit (TNT) for Linear Algebra
//
// R. Pozo
// Applied and Computational Mathematics Division
// National Institute of Standards and Technology


#ifndef SUBSCRPT_H
#define SUBSCRPT_H


//---------------------------------------------------------------------
// This definition describes the default TNT data type used for
// indexing into TNT matrices and vectors.  The data type should
// be wide enough to index into large arrays.  It defaults to an
// "int", but can be overriden at compile time redefining TNT_SUBSCRIPT_TYPE,
// e.g.
//
//	g++ -DTNT_SUBSCRIPT_TYPE='unsigned int'  ...
//
//---------------------------------------------------------------------
//

#ifndef TNT_SUBSCRIPT_TYPE
#define TNT_SUBSCRIPT_TYPE int
#endif

namespace TNT
{
    typedef TNT_SUBSCRIPT_TYPE Subscript;
}


// () indexing in TNT means 1-offset, i.e. x(1) and A(1,1) are the
// first elements.  This offset is left as a macro for future
// purposes, but should not be changed in the current release.
//
//
#define TNT_BASE_OFFSET (1)

#endif
