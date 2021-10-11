// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



// The requirements for a bare-bones vector class:
//
//
//   o) must have 0-based [] indexing for const and
//	    non-const objects  (i.e. operator[] defined)
//
//   o) must have size() method to denote the number of
//	    elements
//   o) must clean up after itself when destructed
//	    (i.e. no memory leaks)
//
//   -) must have begin() and end() methods  (The begin()
//	    method is necessary, because relying on
//	    &v_[0] may not work on a empty vector (i.e. v_ is NULL.)
//
//   o) must be templated
//   o) must have X::value_type defined to be the types of elements
//   o) must have X::X(const &x) copy constructor (by *value*)
//   o) must have X::X(int N) constructor to N-length vector
//	    (NOTE: this constructor need *NOT* initalize elements)
//
//   -) must have X::X(int N, T scalar) constructor to initalize
//	    elements to value of "scalar".
//
//	 ( removed, because valarray<> class uses (scalar, N)  rather
//		than (N, scalar) )
//   -) must have X::X(int N, const T* scalars) constructor to copy from
//		any C linear array
//
//	   ( removed, because of same reverse order of valarray<> )
//
//   o) must have assignment A=B, by value
//
//  NOTE: this class is *NOT* meant to be derived from,
//  so its methods (particularly indexing) need not be
//  declared virtual.
//
//
//  Some things it *DOES NOT* need to do are
//
//  o) bounds checking
//  o) array referencing (e.g. reference counting)
//  o) support () indexing
//  o) I/O
//
