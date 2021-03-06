//-----------------------------------------------------------------------------
// boost variant/visitor_ptr.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_VISITOR_PTR_HPP
#define BOOST_VARIANT_VISITOR_PTR_HPP

#include "boost/variant/bad_visit.hpp"
#include "boost/variant/static_visitor.hpp"

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/type_traits/add_reference.hpp"
#include "boost/type_traits/is_reference.hpp"
#include "boost/type_traits/is_void.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template visitor_ptr
//
// Adapts a function pointer for use as visitor capable of handling
// values of a single type. Throws bad_visit if inappropriately applied.
//
template <typename T, typename R>
class visitor_ptr_t
    : public static_visitor<R>
{
private: // representation

    typedef R (*visitor_t)(T);

    visitor_t visitor_;

public: // typedefs

    typedef R result_type;

private: // private typedefs

    typedef typename mpl::apply_if<
          is_reference<T>
        , mpl::identity<T>
        , add_reference<const T>
        >::type argument_fwd_type;

public: // structors

    explicit visitor_ptr_t(visitor_t visitor)
      : visitor_(visitor)
    {
    }

public: // static visitor interfaces

    template <typename U>
    result_type operator()(const U&) const
    {
        throw bad_visit();
    }

#if !defined(BOOST_NO_VOID_RETURNS)

public: // static visitor interfaces, cont.

    result_type operator()(argument_fwd_type operand) const
    {
        return visitor_(operand);
    }

#else // defined(BOOST_NO_VOID_RETURNS)

private: // helpers, for static visitor interfaces (below)

    result_type execute_impl(argument_fwd_type operand, mpl::false_) const
    {
        return visitor_(operand);
    }

        BOOST_VARIANT_AUX_RETURN_VOID_TYPE
    execute_impl(argument_fwd_type operand, mpl::true_) const
    {
        visitor_(operand);
        BOOST_VARIANT_AUX_RETURN_VOID;
    }

public: // static visitor interfaces, cont.

        BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type)
    operator()(argument_fwd_type operand) const
    {
        typedef typename is_void<result_type>::type has_void_result;
        return execute_impl(operand, has_void_result());
    }

#endif // BOOST_NO_VOID_RETURNS workaround

};

template <typename R, typename T>
inline visitor_ptr_t<T,R> visitor_ptr(R (*visitor)(T))
{
    return visitor_ptr_t<T,R>(visitor);
}

} // namespace boost

#endif// BOOST_VISITOR_VISITOR_PTR_HPP
