// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Vector/Matrix/Array Index Module

#ifndef INDEX_H
#define INDEX_H

#include "subscrpt.h"

namespace TNT
{

class Index1D
{
    Subscript lbound_;
    Subscript ubound_;

    public:

    Subscript lbound() const { return lbound_; }
    Subscript ubound() const { return ubound_; }

    Index1D(const Index1D &D) : lbound_(D.lbound_), ubound_(D.ubound_) {}
    Index1D(Subscript i1, Subscript i2) : lbound_(i1), ubound_(i2) {}

    Index1D & operator=(const Index1D &D)
    {
	lbound_ = D.lbound_;
	ubound_ = D.ubound_;
	return *this;
    }

};

inline Index1D operator+(const Index1D &D, Subscript i)
{
    return Index1D(i+D.lbound(), i+D.ubound());
}

inline Index1D operator+(Subscript i, const Index1D &D)
{
    return Index1D(i+D.lbound(), i+D.ubound());
}



inline Index1D operator-(Index1D &D, Subscript i)
{
    return Index1D(D.lbound()-i, D.ubound()-i);
}

inline Index1D operator-(Subscript i, Index1D &D)
{
    return Index1D(i-D.lbound(), i-D.ubound());
}

} // namespace TNT

#endif

