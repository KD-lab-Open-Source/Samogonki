// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



// Header file for scalar math functions

#ifndef TNTMATH_H
#define TNTMATH_H

// conventional functions required by several matrix algorithms



namespace TNT
{

inline double abs(double t)
{
    return ( t > 0 ? t : -t);
}

inline double min(double a, double b)
{
    return (a < b ? a : b);
}

inline double max(double a, double b)
{
    return (a > b ? a : b);
}

inline float abs(float t)
{
    return ( t > 0 ? t : -t);
}

inline float min(float a, float b)
{
    return (a < b ? a : b);
}

inline float max(float a, float b)
{
    return (a > b ? a : b);
}

inline double sign(double a)
{
    return (a > 0 ? 1 : -1);
}



inline float sign(float a)
{
    return (a > 0 ? 1.f : -1.f);
}

} /* namespace TNT */


#endif
// TNTMATH_H
