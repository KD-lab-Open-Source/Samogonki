//-----------------------------------------------------------------------------
// boost variant/recursive_wrapper.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_RECURSIVE_WRAPPER_HPP
#define BOOST_VARIANT_RECURSIVE_WRAPPER_HPP

#include "boost/variant/recursive_wrapper_fwd.hpp"
#include "boost/checked_delete.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template recursive_wrapper
//
// See docs and recursive_wrapper_fwd.hpp for more information.
//

template <typename T>
class recursive_wrapper
{
public: // typedefs

    typedef T type;

private: // representation

    T* p_;

public: // structors

    ~recursive_wrapper();
    recursive_wrapper();

    recursive_wrapper(const recursive_wrapper& operand);
    recursive_wrapper(const T& operand);

private: // helpers, for modifiers (below)

    void assign(const T& rhs);

public: // modifiers

    recursive_wrapper& operator=(const recursive_wrapper& rhs)
    {
        assign( rhs.get() );
        return *this;
    }

    recursive_wrapper& operator=(const T& rhs)
    {
        assign( rhs );
        return *this;
    }

    void swap(recursive_wrapper& operand)
    {
        T* temp = operand.p_;
        operand.p_ = p_;
        p_ = temp;
    }

public: // queries

    T& get() { return *get_pointer(); }
    const T& get() const { return *get_pointer(); }

    T* get_pointer() { return p_; }
    const T* get_pointer() const { return p_; }

};

template <typename T>
recursive_wrapper<T>::~recursive_wrapper()
{
    boost::checked_delete(p_);
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper()
    : p_(new T)
{
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(const recursive_wrapper& operand)
    : p_(new T( operand.get() ))
{
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(const T& operand)
    : p_(new T(operand))
{
}

template <typename T>
void recursive_wrapper<T>::assign(const T& rhs)
{
    this->get() = rhs;
}

// function template swap
//
// Swaps two recursive_wrapper<T> objects of the same type T.
//
template <typename T>
inline void swap(recursive_wrapper<T>& lhs, recursive_wrapper<T>& rhs)
{
    lhs.swap(rhs);
}

} // namespace boost

#endif // BOOST_VARIANT_RECURSIVE_WRAPPER_HPP
