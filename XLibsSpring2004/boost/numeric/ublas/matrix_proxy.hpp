//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef BOOST_UBLAS_MATRIX_PROXY_H
#define BOOST_UBLAS_MATRIX_PROXY_H

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/minmax.hpp>

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numeric { namespace ublas {

    // Matrix based row vector class
    template<class M>
    class matrix_row:
        public vector_expression<matrix_row<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<matrix_row<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_row<matrix_type> const_self_type;
        typedef matrix_row<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_iterator2 const_iterator_type;
        typedef typename M::iterator2 iterator_type;
#else
        typedef typename M::const_iterator2 const_iterator_type;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_iterator2,
                                          typename M::iterator2>::type iterator_type;
#endif
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_row ():
            data_ (nil_), i_ () {}
        BOOST_UBLAS_INLINE
        matrix_row (matrix_type &data, size_type i):
            data_ (data), i_ (i) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (i_ < data_.size1 (), bad_index ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return data_.size2 ();
        }
        BOOST_UBLAS_INLINE
        size_type index () const {
            return i_;
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, size_type i) {
            // data_ = data;
            data_.reset (data);
            i_ = i;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type j) const {
            return data () (i_, j);
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type j) {
            return data () (i_, j);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type j) const {
            return (*this) (j);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type j) {
            return (*this) (j);
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type j) const {
            return data () (i_, j);
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type j) const {
            return (*this) (j);
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_row &operator = (const matrix_row &mr) {
            // FIXME: the rows could be differently sized.
            // std::copy (mr.begin (), mr.end (), begin ());
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (mr));
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_row &assign_temporary (matrix_row &mr) {
            // FIXME: this is suboptimal.
            // return *this = mr;
            vector_assign (scalar_assign<reference, value_type> (), *this, mr);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &operator = (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &operator += (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &plus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &operator -= (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_row &minus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_row &operator *= (const AT &at) {
            vector_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_row &operator /= (const AT &at) {
            vector_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_row &mr) const {
            return (*this).data () == mr.data () && index () == mr.index ();
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_row mr) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mr, external_logic ());
            if (this != &mr) {
                BOOST_UBLAS_CHECK (size () == mr.size (), bad_size ());
                // Sparse ranges may be nonconformant now.
                // std::swap_ranges (begin (), end (), mr.begin ());
                vector_swap (scalar_swap<reference, reference> (), *this, mr);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_row mr1, matrix_row mr2) {
            mr1.swap (mr2);
        }
#endif

#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<matrix_row<matrix_type>,
                                 BOOST_UBLAS_TYPENAME iterator_type::iterator_category> iterator;
        typedef indexed_const_iterator<matrix_row<matrix_type>,
                                       BOOST_UBLAS_TYPENAME const_iterator_type::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type j) const {
            const_iterator_type it2 (data ().find2 (1, i_, j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it2.index2 ());
#else
            return const_iterator (*this, it2);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type j) {
            iterator_type it2 (data ().find2 (1, i_, j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator (*this, it2.index2 ());
#else
            return iterator (*this, it2);
#endif
        }

        // Iterators simply are pointers.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<matrix_row>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename const_iterator_type::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename const_iterator_type::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename const_iterator_type::iterator_category iterator_category;
            typedef typename const_iterator_type::difference_type difference_type;
            typedef typename const_iterator_type::value_type value_type;
            typedef typename const_iterator_type::reference reference;
            typedef typename const_iterator_type::pointer pointer;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &mr, const const_iterator_type &it):
                container_const_reference<self_type> (mr), it_ (it) {}
#ifndef BOOST_UBLAS_QUALIFIED_TYPENAME
            BOOST_UBLAS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}
#else
            BOOST_UBLAS_INLINE
            const_iterator (const typename self_type::iterator &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}
#endif

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                BOOST_UBLAS_CHECK (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<matrix_row>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_type::iterator_category>::template
                        iterator_base<iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_type::iterator_category,
                                               iterator, value_type> {
#endif
        public:
            typedef typename iterator_type::iterator_category iterator_category;
            typedef typename iterator_type::difference_type difference_type;
            typedef typename iterator_type::value_type value_type;
            typedef typename iterator_type::reference reference;
            typedef typename iterator_type::pointer pointer;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator ():
                container_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            iterator (self_type &mr, const iterator_type &it):
                container_reference<self_type> (mr), it_ (it) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                BOOST_UBLAS_CHECK (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it_.index2 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator begin () {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        matrix_closure_type data_;
        size_type i_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_row<M>::matrix_type matrix_row<M>::nil_;

    // Projections
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_row<M> row (M &data, std::size_t i) {
        return matrix_row<M> (data, i);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_row<const M> row_const (const M &data, std::size_t i) {
        return matrix_row<const M> (data, i);
    }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_row<M> row (const M &data, std::size_t i) {
        return matrix_row<M> (const_cast<M &> (data), i);
    }
#endif

    // Matrix based column vector class
    template<class M>
    class matrix_column:
        public vector_expression<matrix_column<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<matrix_column<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_column<matrix_type> const_self_type;
        typedef matrix_column<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_iterator1 const_iterator_type;
        typedef typename M::iterator1 iterator_type;
#else
        typedef typename M::const_iterator1 const_iterator_type;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_iterator1,
                                          typename M::iterator1>::type iterator_type;
#endif
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_column ():
            data_ (nil_), j_ () {}
        BOOST_UBLAS_INLINE
        matrix_column (matrix_type &data, size_type j):
            data_ (data), j_ (j) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (j_ < data_.size2 (), bad_index ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return data_.size1 ();
        }
        BOOST_UBLAS_INLINE
        size_type index () const {
            return j_;
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, size_type j) {
            // data_ = data;
            data_.reset (data);
            j_ = j;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return data () (i, j_);
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            return data () (i, j_);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return (*this) (i);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            return (*this) (i);
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return data () (i, j_);
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return (*this) (i);
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_column &operator = (const matrix_column &mc) { 
            // FIXME: the columns could be differently sized.
            // std::copy (mc.begin (), mc.end (), begin ());
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (mc));
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_column &assign_temporary (matrix_column &mc) {
            // FIXME: this is suboptimal.
            // return *this = mc;
            vector_assign (scalar_assign<reference, value_type> (), *this, mc);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &operator = (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &operator += (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &plus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &operator -= (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_column &minus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_column &operator *= (const AT &at) {
            vector_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_column &operator /= (const AT &at) {
            vector_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_column &mc) const {
            return (*this).data () == mc.data () && index () == mc.index ();
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_column mc) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mc, external_logic ());
            if (this != &mc) {
                BOOST_UBLAS_CHECK (size () == mc.size (), bad_size ());
                // Sparse ranges may be nonconformant now.
                // std::swap_ranges (begin (), end (), mc.begin ());
                vector_swap (scalar_swap<reference, reference> (), *this, mc);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_column mc1, matrix_column mc2) {
            mc1.swap (mc2);
        }
#endif

#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_iterator<matrix_column<matrix_type>,
                                 BOOST_UBLAS_TYPENAME iterator_type::iterator_category> iterator;
        typedef indexed_const_iterator<matrix_column<matrix_type>,
                                       BOOST_UBLAS_TYPENAME const_iterator_type::iterator_category> const_iterator;
#else
        class const_iterator;
        class iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            const_iterator_type it1 (data ().find1 (1, i, j_));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator (*this, it1.index1 ());
#else
            return const_iterator (*this, it1);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type i) {
            iterator_type it1 (data ().find1 (1, i, j_));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator (*this, it1.index1 ());
#else
            return iterator (*this, it1);
#endif
        }

        // Iterators simply are pointers.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<matrix_column>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename const_iterator_type::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename const_iterator_type::iterator_category,
                                               const_iterator, value_type> {
#endif                                               
        public:
            typedef typename const_iterator_type::iterator_category iterator_category;
            typedef typename const_iterator_type::difference_type difference_type;
            typedef typename const_iterator_type::value_type value_type;
            typedef typename const_iterator_type::reference reference;
            typedef typename const_iterator_type::pointer pointer;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &mc, const const_iterator_type &it):
                container_const_reference<self_type> (mc), it_ (it) {}
#ifndef BOOST_UBLAS_QUALIFIED_TYPENAME
            BOOST_UBLAS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}
#else
            BOOST_UBLAS_INLINE
            const_iterator (const typename self_type::iterator &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}
#endif

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                BOOST_UBLAS_CHECK (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator:
            public container_reference<matrix_column>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_type::iterator_category>::template
                        iterator_base<iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_type::iterator_category,
                                               iterator, value_type> {
#endif                                               
        public:
            typedef typename iterator_type::iterator_category iterator_category;
            typedef typename iterator_type::difference_type difference_type;
            typedef typename iterator_type::value_type value_type;
            typedef typename iterator_type::reference reference;
            typedef typename iterator_type::pointer pointer;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator ():
                container_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            iterator (self_type &mc, const iterator_type &it):
                container_reference<self_type> (mc), it_ (it) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                BOOST_UBLAS_CHECK (index () < (*this) ().size (), bad_index ());
                return *it_;
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it_.index1 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator_type it_;

            friend class const_iterator;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator begin () {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        matrix_closure_type data_;
        size_type j_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_column<M>::matrix_type matrix_column<M>::nil_;

    // Projections
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_column<M> column (M &data, std::size_t j) {
        return matrix_column<M> (data, j);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_column<const M> column_const (const M &data, std::size_t j) {
        return matrix_column<const M> (data, j);
    }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_column<M> column (const M &data, std::size_t j) {
        return matrix_column<M> (const_cast<M &> (data), j);
    }
#endif

    // Matrix based vector range class
    template<class M>
    class matrix_vector_range:
        public vector_expression<matrix_vector_range<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<matrix_vector_range<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_vector_range<matrix_type> const_self_type;
        typedef matrix_vector_range<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
        typedef range::const_iterator const_iterator1_type;
        typedef range::const_iterator iterator1_type;
        typedef range::const_iterator const_iterator2_type;
        typedef range::const_iterator iterator2_type;
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_vector_range ():
            data_ (nil_), r1_ (), r2_ () {}
        BOOST_UBLAS_INLINE
        matrix_vector_range (matrix_type &data, const range &r1, const range &r2):
            data_ (data), r1_ (r1.preprocess (data.size1 ())), r2_ (r2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (r1_.start () <= data_.size1 () &&
                               r1_.start () + r1_.size () <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (r2_.start () <= data_.size2 () &&
                               r2_.start () + r2_.size () <= data_.size2 (), bad_index ());
            BOOST_UBLAS_CHECK (r1_.size () == r2_.size (), bad_size ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type start1 () const {
            return r1_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type start2 () const {
            return r2_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type size () const {
            return BOOST_UBLAS_SAME (r1_.size (), r2_.size ());
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const range &r1, const range &r2) {
            // data_ = data;
            data_.reset (data);
            r1_ = r1;
            r2_ = r2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return data () (r1_ (i), r2_ (i));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            return data () (r1_ (i), r2_ (i));
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return (*this) (i);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            return (*this) (i);
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return data () (r1_ (i), r2_ (i));
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return (*this) (i);
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator = (const matrix_vector_range &mvr) { 
            // FIXME: the ranges could be differently sized.
            // std::copy (mvr.begin (), mvr.end (), begin ());
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (mvr));
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_vector_range &assign_temporary (matrix_vector_range &mvr) {
            // FIXME: this is suboptimal.
            // return *this = mvr;
            vector_assign (scalar_assign<reference, value_type> (), *this, mvr);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator = (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator += (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &plus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator -= (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_range &minus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator *= (const AT &at) {
            vector_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_range &operator /= (const AT &at) {
            vector_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_vector_range &mvr) const {
            return (*this).data () == mvr.data () && r1_ == mvr.r1_ && r2_ == mvr.r2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_vector_range mvr) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mvr, external_logic ());
            if (this != &mvr) {
                BOOST_UBLAS_CHECK (size () == mvr.size (), bad_size ());
                // Sparse ranges may be nonconformant now.
                // std::swap_ranges (begin (), end (), mvr.begin ());
                vector_swap (scalar_swap<reference, reference> (), *this, mvr);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_vector_range mvr1, matrix_vector_range mvr2) {
            mvr1.swap (mvr2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, r1_.begin () + i, r2_.begin () + i);
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type i) {
            return iterator (*this, r1_.begin () + i, r2_.begin () + i);
        }

        // Iterators simply are indices.

        class const_iterator:
            public container_const_reference<matrix_vector_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                          typename M::const_iterator2::iterator_category>::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                                 typename M::const_iterator2::iterator_category>::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                      typename M::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename matrix_vector_range::difference_type difference_type;
            typedef typename matrix_vector_range::value_type value_type;
            typedef typename matrix_vector_range::const_reference reference;
            typedef typename matrix_vector_range::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &mvr, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (mvr), it1_ (it1), it2_ (it2) {}
#ifndef BOOST_UBLAS_QUALIFIED_TYPENAME
            BOOST_UBLAS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#else
            BOOST_UBLAS_INLINE
            const_iterator (const typename self_type::iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#endif

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type  index () const {
                return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_vector_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                          typename M::iterator2::iterator_category>::iterator_category>::template
                        iterator_base<iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                                 typename M::iterator2::iterator_category>::iterator_category,
                                               iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                      typename M::iterator2::iterator_category>::iterator_category iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix_vector_range::difference_type difference_type;
            typedef typename matrix_vector_range::value_type value_type;
            typedef typename matrix_vector_range::reference reference;
            typedef typename matrix_vector_range::pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator (self_type &mvr, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (mvr), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_); 
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        BOOST_UBLAS_INLINE
        iterator begin () {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        matrix_closure_type data_;
        range r1_;
        range r2_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_vector_range<M>::matrix_type matrix_vector_range<M>::nil_;

    // Matrix based vector slice class
    template<class M>
    class matrix_vector_slice:
        public vector_expression<matrix_vector_slice<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<matrix_vector_slice<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_vector_slice<matrix_type> const_self_type;
        typedef matrix_vector_slice<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_vector_slice ():
            data_ (nil_), s1_ (), s2_ () {}
        BOOST_UBLAS_INLINE
        matrix_vector_slice (matrix_type &data, const slice &s1, const slice &s2):
            data_ (data), s1_ (s1.preprocess (data.size1 ())), s2_ (s2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (s1_.start () <= data_.size1 () &&
                               s1_.start () + s1_.stride () * (s1_.size () - (s1_.size () > 0)) <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (s2_.start () <= data_.size2 () &&
                               s2_.start () + s2_.stride () * (s2_.size () - (s2_.size () > 0)) <= data_.size2 (), bad_index ());
            // One of the slices may be stationary.
            // Thanks to Michael Stevens for this extension.
            BOOST_UBLAS_CHECK ((s1_.stride () != 0 || s2_.stride () != 0) &&
                               s1_.size () == s2_.size (), bad_size ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type start1 () const {
            return s1_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type start2 () const {
            return s2_.start ();
        }
        BOOST_UBLAS_INLINE
        difference_type stride1 () const {
            return s1_.stride ();
        }
        BOOST_UBLAS_INLINE
        difference_type stride2 () const {
            return s2_.stride ();
        }
        BOOST_UBLAS_INLINE
        size_type size () const {
            return BOOST_UBLAS_SAME (s1_.size (), s2_.size ());
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const slice &s1, const slice &s2) {
            // data_ = data;
            data_.reset (data);
            s1_ = s1;
            s2_ = s2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return data () (s1_ (i), s2_ (i));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            return data () (s1_ (i), s2_ (i));
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return (*this) (i);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            return (*this) (i);
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return data () (s1_ (i), s2_ (i));
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return (*this) (i);
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator = (const matrix_vector_slice &mvs) { 
            // FIXME: the slices could be differently sized.
            // std::copy (mvs.begin (), mvs.end (), begin ());
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (mvs));
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_vector_slice &assign_temporary (matrix_vector_slice &mvs) {
            // FIXME: this is suboptimal.
            // return *this = mvs;
            vector_assign (scalar_assign<reference, value_type> (), *this, mvs);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator = (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator += (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &plus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator -= (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &minus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator *= (const AT &at) {
            vector_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_slice &operator /= (const AT &at) {
            vector_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_vector_slice &mvs) const {
            return (*this).data () == mvs.data () && s1_ == mvs.s1_ && s2_ == mvs.s2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_vector_slice mvs) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mvs, external_logic ());
            if (this != &mvs) {
                BOOST_UBLAS_CHECK (size () == mvs.size (), bad_size ());
                // Sparse ranges may be nonconformant now.
                // std::swap_ranges (begin (), end (), mvs.begin ());
                vector_swap (scalar_swap<reference, reference> (), *this, mvs);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_vector_slice mvs1, matrix_vector_slice mvs2) {
            mvs1.swap (mvs2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, s1_.begin () + i, s2_.begin () + i);
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type i) {
            return iterator (*this, s1_.begin () + i, s2_.begin () + i);
        }

        // Iterators simply are indices.

        class const_iterator:
            public container_const_reference<matrix_vector_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                          typename M::const_iterator2::iterator_category>::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                                 typename M::const_iterator2::iterator_category>::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                      typename M::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename matrix_vector_slice::difference_type difference_type;
            typedef typename matrix_vector_slice::value_type value_type;
            typedef typename matrix_vector_slice::const_reference reference;
            typedef typename matrix_vector_slice::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &mvs, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (mvs), it1_ (it1), it2_ (it2) {}
#ifndef BOOST_UBLAS_QUALIFIED_TYPENAME
            BOOST_UBLAS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#else
            BOOST_UBLAS_INLINE
            const_iterator (const typename self_type::iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#endif

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                // One of the slices may be stationary.
                // Thanks to Michael Stevens for this extension.
                // return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
                return std_max (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type  index () const {
                // One of the slices may be stationary.
                // Thanks to Michael Stevens for this extension.
                // return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
                return std_max (it1_.index (), it2_.index ());
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_vector_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                          typename M::iterator2::iterator_category>::iterator_category>::template
                        iterator_base<iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                                 typename M::iterator2::iterator_category>::iterator_category,
                                               iterator, value_type> {
#endif                                               
        public:
            typedef typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                      typename M::iterator2::iterator_category>::iterator_category iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix_vector_slice::difference_type difference_type;
            typedef typename matrix_vector_slice::value_type value_type;
            typedef typename matrix_vector_slice::reference reference;
            typedef typename matrix_vector_slice::pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator (self_type &mvs, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (mvs), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                // One of the slices may be stationary.
                // Thanks to Michael Stevens for this extension.
                // return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
                return std_max (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                // One of the slices may be stationary.
                // Thanks to Michael Stevens for this extension.
                // return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
                return std_max (it1_.index (), it2_.index ());
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        BOOST_UBLAS_INLINE
        iterator begin () {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        matrix_closure_type data_;
        slice s1_;
        slice s2_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_vector_slice<M>::matrix_type matrix_vector_slice<M>::nil_;

    // Matrix based vector indirection class
    template<class M, class IA>
    class matrix_vector_indirect:
        public vector_expression<matrix_vector_indirect<M, IA> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<matrix_vector_indirect<M, IA> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef const IA const_indirect_array_type;
        typedef IA indirect_array_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_vector_indirect<matrix_type, indirect_array_type> const_self_type;
        typedef matrix_vector_indirect<matrix_type, indirect_array_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
        typedef typename IA::const_iterator const_iterator1_type;
        typedef typename IA::const_iterator iterator1_type;
        typedef typename IA::const_iterator const_iterator2_type;
        typedef typename IA::const_iterator iterator2_type;
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_vector_indirect ():
            data_ (nil_), ia1_ (), ia2_ () {}
        BOOST_UBLAS_INLINE
        matrix_vector_indirect (matrix_type &data, size_type size):
            data_ (data), ia1_ (size), ia2_ (size) {}
        BOOST_UBLAS_INLINE
        matrix_vector_indirect (matrix_type &data, const indirect_array_type &ia1, const indirect_array_type &ia2):
            data_ (data), ia1_ (ia1), ia2_ (ia2) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (ia1_.size () == ia2_.size (), bad_size ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return BOOST_UBLAS_SAME (ia1_.size (), ia2_.size ());
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }
        BOOST_UBLAS_INLINE
        const_indirect_array_type &indirect1 () const {
            return ia1_;
        }
        BOOST_UBLAS_INLINE
        indirect_array_type &indirect1 () {
            return ia1_;
        }
        BOOST_UBLAS_INLINE
        const_indirect_array_type &indirect2 () const {
            return ia2_;
        }
        BOOST_UBLAS_INLINE
        indirect_array_type &indirect2 () {
            return ia2_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const indirect_array_type &ia1, const indirect_array_type &ia2) {
            // data_ = data;
            data_.reset (data);
            ia1_ = ia1;
            ia2_ = ia2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return data () (ia1_ (i), ia2_ (i));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            return data () (ia1_ (i), ia2_ (i));
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return (*this) (i);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            return (*this) (i);
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return data () (ia1_ (i), ia2_ (i));
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return (*this) (i);
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator = (const matrix_vector_indirect &mvi) {
            // FIXME: the indirects could be differently sized.
            // std::copy (mvi.begin (), mvi.end (), begin ());
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (mvi));
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &assign_temporary (matrix_vector_indirect &mvi) {
            // FIXME: this is suboptimal.
            // return *this = mvi;
            vector_assign (scalar_assign<reference, value_type> (), *this, mvi);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator = (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator += (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &plus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator -= (const vector_expression<AE> &ae) {
            vector_assign (scalar_assign<reference, value_type> (), *this, vector<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &minus_assign (const vector_expression<AE> &ae) {
            vector_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator *= (const AT &at) {
            vector_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_vector_indirect &operator /= (const AT &at) {
            vector_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_vector_indirect &mvi) const {
            return (*this).data () == mvi.data () && ia1_ == mvi.ia1_ && ia2_ == mvi.ia2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_vector_indirect mvi) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mvi, external_logic ());
            if (this != &mvi) {
                BOOST_UBLAS_CHECK (size () == mvi.size (), bad_size ());
                // Sparse ranges may be nonconformant now.
                // std::swap_ranges (begin (), end (), mvi.begin ());
                vector_swap (scalar_swap<reference, reference> (), *this, mvi);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_vector_indirect mvi1, matrix_vector_indirect mvi2) {
            mvi1.swap (mvi2);
        }
#endif

        class const_iterator;
        class iterator;

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            return const_iterator (*this, ia1_.begin () + i, ia2_.begin () + i);
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type i) {
            return iterator (*this, ia1_.begin () + i, ia2_.begin () + i);
        }

        // Iterators simply are indices.

        class const_iterator:
            public container_const_reference<matrix_vector_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                          typename M::const_iterator2::iterator_category>::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                                                 typename M::const_iterator2::iterator_category>::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename M::const_iterator1::iterator_category,
                                                      typename M::const_iterator2::iterator_category>::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename matrix_vector_indirect::difference_type difference_type;
            typedef typename matrix_vector_indirect::value_type value_type;
            typedef typename matrix_vector_indirect::const_reference reference;
            typedef typename matrix_vector_indirect::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &mvi, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (mvi), it1_ (it1), it2_ (it2) {}
#ifndef BOOST_UBLAS_QUALIFIED_TYPENAME
            BOOST_UBLAS_INLINE
            const_iterator (const iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#else
            BOOST_UBLAS_INLINE
            const_iterator (const typename self_type::iterator &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}
#endif

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type  index () const {
                return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        class iterator:
            public container_reference<matrix_vector_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                          typename M::iterator2::iterator_category>::iterator_category>::template
                        iterator_base<iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                                                 typename M::iterator2::iterator_category>::iterator_category,
                                               iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename M::iterator1::iterator_category,
                                                      typename M::iterator2::iterator_category>::iterator_category iterator_category;
#ifndef BOOST_MSVC_STD_ITERATOR
            typedef typename matrix_vector_indirect::difference_type difference_type;
            typedef typename matrix_vector_indirect::value_type value_type;
            typedef typename matrix_vector_indirect::reference reference;
            typedef typename matrix_vector_indirect::pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator (self_type &mvi, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (mvi), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator &operator ++ () {
                ++ it1_;
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -- () {
                -- it1_;
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator += (difference_type n) {
                it1_ += n;
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator &operator -= (difference_type n) {
                it1_ -= n;
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return BOOST_UBLAS_SAME (it1_ - it.it1_, it2_ - it.it2_);
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return BOOST_UBLAS_SAME (it1_.index (), it2_.index ());
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator &operator = (const iterator &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ == it.it1_ && it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it1_ < it.it1_ && it2_ < it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator;
        };

        BOOST_UBLAS_INLINE
        iterator begin () {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        matrix_closure_type data_;
        indirect_array_type ia1_;
        indirect_array_type ia2_;
        static matrix_type nil_;
    };

    template<class M, class IA>
    typename matrix_vector_indirect<M, IA>::matrix_type matrix_vector_indirect<M, IA>::nil_;

    // Matrix based range class
    template<class M>
    class matrix_range:
        public matrix_expression<matrix_range<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING matrix_expression<matrix_range<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_range<matrix_type> const_self_type;
        typedef matrix_range<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename M::iterator1 iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename M::iterator2 iterator2_type;
#else
        typedef typename M::const_iterator1 const_iterator1_type;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_iterator1,
                                          typename M::iterator1>::type iterator1_type;
        typedef typename M::const_iterator2 const_iterator2_type;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_iterator2,
                                          typename M::iterator2>::type iterator2_type;
#endif
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_range ():
            data_ (nil_), r1_ (), r2_ () {}
        BOOST_UBLAS_INLINE
        matrix_range (matrix_type &data, const range &r1, const range &r2):
            data_ (data), r1_ (r1.preprocess (data.size1 ())), r2_ (r2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (r1_.start () <= data_.size1 () &&
                               r1_.start () + r1_.size () <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (r2_.start () <= data_.size2 () &&
                               r2_.start () + r2_.size () <= data_.size2 (), bad_index ());
        }
        BOOST_UBLAS_INLINE
        matrix_range (const matrix_closure_type &data, const range &r1, const range &r2, int):
            data_ (data), r1_ (r1.preprocess (data.size1 ())), r2_ (r2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (r1_.start () <= data_.size1 () &&
                               r1_.start () + r1_.size () <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (r2_.start () <= data_.size2 () &&
                               r2_.start () + r2_.size () <= data_.size2 (), bad_index ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type start1 () const {
            return r1_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type size1 () const {
            return r1_.size ();
        }
        BOOST_UBLAS_INLINE
        size_type start2() const {
            return r2_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type size2 () const {
            return r2_.size ();
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const range &r1, const range &r2) {
            // data_ = data;
            data_.reset (data);
            r1_ = r1;
            r2_ = r2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            return data () (r1_ (i), r2_ (j));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) {
            return data () (r1_ (i), r2_ (j));
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) const {
            return data () (r1_ (i), r2_ (j));
        }
#endif

        BOOST_UBLAS_INLINE
        matrix_range<matrix_type> project (const range &r1, const range &r2) const {
            return matrix_range<matrix_type>  (data_, r1_.compose (r1.preprocess (data_.size1 ())), r2_.compose (r2.preprocess (data_.size2 ())), 0);
        }

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_range &operator = (const matrix_range &mr) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, mr);
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_range &assign_temporary (matrix_range &mr) {
            return *this = mr;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range &operator = (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range &assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range& operator += (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae); 
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_range &minus_assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_range& operator *= (const AT &at) {
            matrix_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_range& operator /= (const AT &at) {
            matrix_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_range &mr) const {
            return (*this).data () == mr.data () && r1_ == mr.r1_ && r2_ == mr.r2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_range mr) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mr, external_logic ());
            if (this != &mr) {
                BOOST_UBLAS_CHECK (size1 () == mr.size1 (), bad_size ());
                BOOST_UBLAS_CHECK (size2 () == mr.size2 (), bad_size ());
                matrix_swap (scalar_swap<reference, reference> (), *this, mr);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_range mr1, matrix_range mr2) {
            mr1.swap (mr2);
        }
#endif

#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<matrix_range<matrix_type>,
                                  BOOST_UBLAS_TYPENAME iterator1_type::iterator_category> iterator1;
        typedef indexed_iterator2<matrix_range<matrix_type>,
                                  BOOST_UBLAS_TYPENAME iterator2_type::iterator_category> iterator2;
        typedef indexed_const_iterator1<matrix_range<matrix_type>,
                                        BOOST_UBLAS_TYPENAME const_iterator1_type::iterator_category> const_iterator1;
        typedef indexed_const_iterator2<matrix_range<matrix_type>,
                                        BOOST_UBLAS_TYPENAME const_iterator2_type::iterator_category> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base1<const_iterator1, value_type, const_reference> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2, value_type, const_reference> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator_base1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2> reverse_iterator2;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator1 find1 (int rank, size_type i, size_type j) const {
            const_iterator1_type it1 (data ().find1 (rank, start1 () + i, start2 () + j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return const_iterator1 (*this, it1);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator1 find1 (int rank, size_type i, size_type j) {
            iterator1_type it1 (data ().find1 (rank, start1 () + i, start2 () + j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator1 (*this, it1.index1 (), it1.index2 ());
#else
            return iterator1 (*this, it1);
#endif
        }
        BOOST_UBLAS_INLINE
        const_iterator2 find2 (int rank, size_type i, size_type j) const {
            const_iterator2_type it2 (data ().find2 (rank, start1 () + i, start2 () + j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return const_iterator2 (*this, it2);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator2 find2 (int rank, size_type i, size_type j) {
            iterator2_type it2 (data ().find2 (rank, start1 () + i, start2 () + j));
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator2 (*this, it2.index1 (), it2.index2 ());
#else
            return iterator2 (*this, it2);
#endif
        }

        // Iterators simply are pointers.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator1:
            public container_const_reference<matrix_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename const_iterator1_type::iterator_category>::template
                        iterator_base<const_iterator1, value_type>::type {
#else
#ifndef BOOST_MSVC
            public random_access_iterator_base<typename const_iterator1_type::iterator_category,
                                               const_iterator1, value_type> {
#else
            public random_access_iterator_base<typename M::const_iterator1::iterator_category,
                                               const_iterator1, value_type> {
#endif
#endif
        public:
            typedef typename const_iterator1_type::iterator_category iterator_category;
            typedef typename const_iterator1_type::difference_type difference_type;
            typedef typename const_iterator1_type::value_type value_type;
            typedef typename const_iterator1_type::reference reference;
            typedef typename const_iterator1_type::pointer pointer;
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator1 ():
                container_const_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const self_type &mr, const const_iterator1_type &it):
                container_const_reference<self_type> (mr), it_ (it) {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return *it_;
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 begin () const {
                const self_type &mr = (*this) ();
                return mr.find2 (1, index1 (), 0);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 end () const {
                const self_type &mr = (*this) ();
                return mr.find2 (1, index1 (), mr.size2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator1_type it_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator1 end1 () const {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator1:
            public container_reference<matrix_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator1_type::iterator_category>::template
                        iterator_base<iterator1, value_type>::type {
#else
#ifndef BOOST_MSVC
            public random_access_iterator_base<typename iterator1_type::iterator_category,
                                               iterator1, value_type> {
#else
            public random_access_iterator_base<typename M::iterator1::iterator_category,
                                               iterator1, value_type> {
#endif
#endif
        public:
            typedef typename iterator1_type::iterator_category iterator_category;
            typedef typename iterator1_type::difference_type difference_type;
            typedef typename iterator1_type::value_type value_type;
            typedef typename iterator1_type::reference reference;
            typedef typename iterator1_type::pointer pointer;
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator1 ():
                container_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            iterator1 (self_type &mr, const iterator1_type &it):
                container_reference<self_type> (mr), it_ (it) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator1 &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return *it_;
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 begin () const {
                self_type &mr = (*this) ();
                return mr.find2 (1, index1 (), 0);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 end () const {
                self_type &mr = (*this) ();
                return mr.find2 (1, index1 (), mr.size2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator1_type it_;

            friend class const_iterator1;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator1 end1 () {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator2:
            public container_const_reference<matrix_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename const_iterator2_type::iterator_category>::template
                        iterator_base<const_iterator2, value_type>::type {
#else
#ifndef BOOST_MSVC
            public random_access_iterator_base<typename const_iterator2_type::iterator_category,
                                               const_iterator2, value_type> {
#else
            public random_access_iterator_base<typename M::const_iterator2::iterator_category,
                                               const_iterator2, value_type> {
#endif
#endif                                               
        public:
            typedef typename const_iterator2_type::iterator_category iterator_category;
            typedef typename const_iterator2_type::difference_type difference_type;
            typedef typename const_iterator2_type::value_type value_type;
            typedef typename const_iterator2_type::reference reference;
            typedef typename const_iterator2_type::pointer pointer;
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator2 ():
                container_const_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const self_type &mr, const const_iterator2_type &it):
                container_const_reference<self_type> (mr), it_ (it) {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<self_type> (it ()), it_ (it.it_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return *it_;
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 begin () const {
                const self_type &mr = (*this) ();
                return mr.find1 (1, 0, index2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 end () const {
                const self_type &mr = (*this) ();
                return mr.find1 (1, mr.size1 (), index2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator2_type it_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, 0, size2 ());
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator2:
            public container_reference<matrix_range>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator2_type::iterator_category>::template
                        iterator_base<iterator2, value_type>::type {
#else
#ifndef BOOST_MSVC
            public random_access_iterator_base<typename iterator2_type::iterator_category,
                                               iterator2, value_type> {
#else
            public random_access_iterator_base<typename M::iterator2::iterator_category,
                                               iterator2, value_type> {
#endif
#endif
        public:
            typedef typename iterator2_type::iterator_category iterator_category;
            typedef typename iterator2_type::difference_type difference_type;
            typedef typename iterator2_type::value_type value_type;
            typedef typename iterator2_type::reference reference;
            typedef typename iterator2_type::pointer pointer;
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator2 ():
                container_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            iterator2 (self_type &mr, const iterator2_type &it):
                container_reference<self_type> (mr), it_ (it) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator2 &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return *it_;
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 begin () const {
                self_type &mr = (*this) ();
                return mr.find1 (1, 0, index2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 end () const {
                self_type &mr = (*this) ();
                return mr.find1 (1, mr.size1 (), index2 ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it_.index1 () - (*this) ().start1 ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it_.index2 () - (*this) ().start2 ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                return it_ < it.it_;
            }

        private:
            iterator2_type it_;

            friend class const_iterator2;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator2 end2 () {
            return find2 (0, 0, size2 ());
        }

        // Reverse iterators

        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        matrix_closure_type data_;
        range r1_;
        range r2_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_range<M>::matrix_type matrix_range<M>::nil_;

    // Projections
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_range<M> project (M &data, const range &r1, const range &r2) {
        return matrix_range<M> (data, r1, r2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_range<const M> project_const (const M &data, const range &r1, const range &r2) {
        return matrix_range<const M> (data, r1, r2);
    }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_range<M> project (const M &data, const range &r1, const range &r2) {
        return matrix_range<M> (const_cast<M &> (data), r1, r2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_range<M> project (matrix_range<M> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_range<M> project (const matrix_range<M> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
#endif

    // Matrix based slice class
    template<class M>
    class matrix_slice:
        public matrix_expression<matrix_slice<M> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING matrix_expression<matrix_slice<M> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef typename M::simd_category simd_category;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_slice<matrix_type> const_self_type;
        typedef matrix_slice<matrix_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
        typedef slice::const_iterator const_iterator1_type;
        typedef slice::const_iterator iterator1_type;
        typedef slice::const_iterator const_iterator2_type;
        typedef slice::const_iterator iterator2_type;
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_slice ():
            data_ (nil_), s1_ (), s2_ () {}
        BOOST_UBLAS_INLINE
        matrix_slice (matrix_type &data, const slice &s1, const slice &s2):
            data_ (data), s1_ (s1.preprocess (data.size1 ())), s2_ (s2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (s1_.start () <= data_.size1 () &&
                               s1_.start () + s1_.stride () * (s1_.size () - (s1_.size () > 0)) <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (s2_.start () <= data_.size2 () &&
                               s2_.start () + s2_.stride () * (s2_.size () - (s2_.size () > 0)) <= data_.size2 (), bad_index ());
        }
        BOOST_UBLAS_INLINE
        matrix_slice (const matrix_closure_type &data, const slice &s1, const slice &s2, int):
            data_ (data), s1_ (s1.preprocess (data.size1 ())), s2_ (s2.preprocess (data.size2 ())) {
            // Early checking of preconditions.
            BOOST_UBLAS_CHECK (s1_.start () <= data_.size1 () &&
                               s1_.start () + s1_.stride () * (s1_.size () - (s1_.size () > 0)) <= data_.size1 (), bad_index ());
            BOOST_UBLAS_CHECK (s2_.start () <= data_.size2 () &&
                               s2_.start () + s2_.stride () * (s2_.size () - (s2_.size () > 0)) <= data_.size2 (), bad_index ());
        }

        // Accessors
        BOOST_UBLAS_INLINE
        size_type start1 () const {
            return s1_.start ();
        }
        BOOST_UBLAS_INLINE
        size_type start2 () const {
            return s2_.start ();
        }
        BOOST_UBLAS_INLINE
        difference_type stride1 () const {
            return s1_.stride ();
        }
        BOOST_UBLAS_INLINE
        difference_type stride2 () const {
            return s2_.stride ();
        }
        BOOST_UBLAS_INLINE
        size_type size1 () const {
            return s1_.size ();
        }
        BOOST_UBLAS_INLINE
        size_type size2 () const {
            return s2_.size ();
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const slice &s1, const slice &s2) {
            // data_ = data;
            data_.reset (data);
            s1_ = s1;
            s2_ = s2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            return data () (s1_ (i), s2_ (j));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) {
            return data () (s1_ (i), s2_ (j));
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) const {
            return data () (s1_ (i), s2_ (j));
        }
#endif

        BOOST_UBLAS_INLINE
        matrix_slice<matrix_type> project (const range &r1, const range &r2) const {
            return matrix_slice<matrix_type>  (data_, s1_.compose (r1.preprocess (data_.size1 ())), s2_.compose (r2.preprocess (data_.size2 ())), 0);
        }
        BOOST_UBLAS_INLINE
        matrix_slice<matrix_type> project (const slice &s1, const slice &s2) const {
            return matrix_slice<matrix_type>  (data_, s1_.compose (s1.preprocess (data_.size1 ())), s2_.compose (s2.preprocess (data_.size2 ())), 0);
        }

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_slice &operator = (const matrix_slice &ms) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, ms); 
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_slice &assign_temporary (matrix_slice &ms) { 
            return *this = ms;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice &operator = (const matrix_expression<AE> &ae) { 
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (ae)); 
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice &assign (const matrix_expression<AE> &ae) { 
            matrix_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae); 
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice& operator += (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this + ae)); 
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice &plus_assign (const matrix_expression<AE> &ae) { 
            matrix_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae); 
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_slice &minus_assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_slice& operator *= (const AT &at) {
            matrix_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_slice& operator /= (const AT &at) {
            matrix_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_slice &ms) const {
            return (*this).data () == ms.data () && s1_ == ms.s1_ && s2_ == ms.s2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_slice ms) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &ms, external_logic ());
            if (this != &ms) {
                BOOST_UBLAS_CHECK (size1 () == ms.size1 (), bad_size ());
                BOOST_UBLAS_CHECK (size2 () == ms.size2 (), bad_size ());
                matrix_swap (scalar_swap<reference, reference> (), *this, ms);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_slice ms1, matrix_slice ms2) {
            ms1.swap (ms2);
        }
#endif

#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<matrix_slice<matrix_type>,
                                  BOOST_UBLAS_TYPENAME matrix_type::iterator1::iterator_category> iterator1;
        typedef indexed_iterator2<matrix_slice<matrix_type>,
                                  BOOST_UBLAS_TYPENAME matrix_type::iterator2::iterator_category> iterator2;
        typedef indexed_const_iterator1<matrix_slice<matrix_type>,
                                        BOOST_UBLAS_TYPENAME matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef indexed_const_iterator2<matrix_slice<matrix_type>,
                                        BOOST_UBLAS_TYPENAME matrix_type::const_iterator2::iterator_category> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base1<const_iterator1, value_type, const_reference> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2, value_type, const_reference> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator_base1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2> reverse_iterator2;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator1 find1 (int /* rank */, size_type i, size_type j) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (*this, s1_.begin () + i, s2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator1 find1 (int /* rank */, size_type i, size_type j) {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (*this, s1_.begin () + i, s2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        const_iterator2 find2 (int /* rank */, size_type i, size_type j) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (*this, s1_.begin () + i, s2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator2 find2 (int /* rank */, size_type i, size_type j) {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (*this, s1_.begin () + i, s2_.begin () + j);
#endif
        }

        // Iterators simply are indices.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator1:
            public container_const_reference<matrix_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::const_iterator1::iterator_category>::template
                        iterator_base<const_iterator1, value_type>::type {
#else
            public random_access_iterator_base<typename M::const_iterator1::iterator_category,
                                               const_iterator1, value_type> {
#endif
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;
            typedef typename M::const_iterator1::difference_type difference_type;
            typedef typename M::const_iterator1::value_type value_type;
            typedef typename M::const_iterator1::reference reference;
            typedef typename M::const_iterator1::pointer pointer;
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator1 ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const self_type &ms, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (ms), it1_ (it1), it2_ (it2) {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ - it.it1_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ == it.it1_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ < it.it1_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator1 end1 () const {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator1:
            public container_reference<matrix_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::iterator1::iterator_category>::template
                        iterator_base<iterator1, value_type>::type {
#else
            public random_access_iterator_base<typename M::iterator1::iterator_category,
                                               iterator1, value_type> {
#endif
        public:
            typedef typename M::iterator1::iterator_category iterator_category;
            typedef typename M::iterator1::difference_type difference_type;
            typedef typename M::iterator1::value_type value_type;
            typedef typename M::iterator1::reference reference;
            typedef typename M::iterator1::pointer pointer;
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator1 ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator1 (self_type &ms, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (ms), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ - it.it1_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 begin () const {
                return iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 end () const {
                return iterator2 ((*this) (), it1_, it2_ ().end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ == it.it1_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ < it.it1_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator1;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator1 end1 () {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator2:
            public container_const_reference<matrix_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::const_iterator2::iterator_category>::template
                        iterator_base<const_iterator2, value_type>::type {
#else
            public random_access_iterator_base<typename M::const_iterator2::iterator_category,
                                               const_iterator2, value_type> {
#endif                                               
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;
            typedef typename M::const_iterator2::difference_type difference_type;
            typedef typename M::const_iterator2::value_type value_type;
            typedef typename M::const_iterator2::reference reference;
            typedef typename M::const_iterator2::pointer pointer;
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator2 ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const self_type &ms, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (ms), it1_ (it1), it2_ (it2) {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ - it.it2_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it1_ ().begin (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it1_ ().end (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, 0, size2 ());
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator2:
            public container_reference<matrix_slice>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::iterator2::iterator_category>::template
                        iterator_base<iterator2, value_type>::type {
#else
            public random_access_iterator_base<typename M::iterator2::iterator_category,
                                               iterator2, value_type> {
#endif                                               
        public:
            typedef typename M::iterator2::iterator_category iterator_category;
            typedef typename M::iterator2::difference_type difference_type;
            typedef typename M::iterator2::value_type value_type;
            typedef typename M::iterator2::reference reference;
            typedef typename M::iterator2::pointer pointer;
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator2 ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator2 (self_type &ms, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (ms), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ - it.it2_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 begin () const {
                return iterator1 ((*this) (), it1_ ().begin (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 end () const {
                return iterator1 ((*this) (), it1_ ().end (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ < it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator2;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator2 end2 () {
            return find2 (0, 0, size2 ());
        }

        // Reverse iterators

        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        matrix_closure_type data_;
        slice s1_;
        slice s2_;
        static matrix_type nil_;
    };

    template<class M>
    typename matrix_slice<M>::matrix_type matrix_slice<M>::nil_;

    // Projections
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (matrix_slice<M> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (const matrix_slice<M> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
#endif
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (M &data, const slice &s1, const slice &s2) {
        return matrix_slice<M> (data, s1, s2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<const M> project_const (const M &data, const slice &s1, const slice &s2) {
        return matrix_slice<const M> (data, s1, s2);
    }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (const M &data, const slice &s1, const slice &s2) {
        return matrix_slice<M> (const_cast<M &> (data), s1, s2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (matrix_slice<M> &data, const slice &s1, const slice &s2) {
        return data.project (s1, s2);
    }
    template<class M>
    BOOST_UBLAS_INLINE
    matrix_slice<M> project (const matrix_slice<M> &data, const slice &s1, const slice &s2) {
        return data.project (s1, s2);
    }
#endif

    // Matrix based indirection class
    // Contributed by Toon Knapen.
    // Extended and optimized by Kresimir Fresl.
    template<class M, class IA>
    class matrix_indirect:
        public matrix_expression<matrix_indirect<M, IA> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING matrix_expression<matrix_indirect<M, IA> >::operator ();
#endif
        typedef const M const_matrix_type;
        typedef M matrix_type;
        typedef const IA const_indirect_array_type;
        typedef IA indirect_array_type;
        typedef typename M::size_type size_type;
        typedef typename M::difference_type difference_type;
        typedef typename M::value_type value_type;
#ifndef BOOST_UBLAS_CT_PROXY_BASE_TYPEDEFS
        typedef typename M::const_reference const_reference;
        typedef typename M::reference reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename M::pointer pointer;
#else
        typedef typename M::const_reference const_reference;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_reference,
                                          typename M::reference>::type reference;
        typedef typename M::const_pointer const_pointer;
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_pointer,
                                          typename M::pointer>::type pointer;
#endif
#ifndef BOOST_UBLAS_CT_PROXY_CLOSURE_TYPEDEFS
        typedef typename M::closure_type matrix_closure_type;
#else
        typedef typename boost::mpl::if_c<boost::is_const<M>::value,
                                          typename M::const_closure_type,
                                          typename M::closure_type>::type matrix_closure_type;
#endif
        typedef const matrix_indirect<matrix_type, indirect_array_type> const_self_type;
        typedef matrix_indirect<matrix_type, indirect_array_type> self_type;
        typedef const_self_type const_closure_type;
        typedef self_type closure_type;
        typedef typename IA::const_iterator const_iterator1_type;
        typedef typename IA::const_iterator iterator1_type;
        typedef typename IA::const_iterator const_iterator2_type;
        typedef typename IA::const_iterator iterator2_type;
        typedef typename storage_restrict_traits<typename M::storage_category,
                                                 dense_proxy_tag>::storage_category storage_category;
        typedef typename M::orientation_category orientation_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        matrix_indirect ():
            data_ (nil_), ia1_ (), ia2_ () {}
        BOOST_UBLAS_INLINE
        matrix_indirect (matrix_type &data, size_type size1, size_type size2):
            data_ (data), ia1_ (size1), ia2_ (size2) {}
        BOOST_UBLAS_INLINE
        matrix_indirect (matrix_type &data, const indirect_array_type &ia1, const indirect_array_type &ia2):
            data_ (data), ia1_ (ia1.preprocess (data.size1 ())), ia2_ (ia2.preprocess (data.size2 ())) {}
        BOOST_UBLAS_INLINE
        matrix_indirect (const matrix_closure_type &data, const indirect_array_type &ia1, const indirect_array_type &ia2, int):
            data_ (data), ia1_ (ia1.preprocess (data.size1 ())), ia2_ (ia2.preprocess (data.size2 ())) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size1 () const {
            return ia1_.size ();
        }
        BOOST_UBLAS_INLINE
        size_type size2 () const {
            return ia2_.size ();
        }
        BOOST_UBLAS_INLINE
        const matrix_closure_type &data () const {
            return data_;
        }
        BOOST_UBLAS_INLINE
        matrix_closure_type &data () {
            return data_;
        }
        BOOST_UBLAS_INLINE
        const_indirect_array_type &indirect1 () const {
            return ia1_;
        }
        BOOST_UBLAS_INLINE
        indirect_array_type &indirect1 () {
            return ia1_;
        }
        BOOST_UBLAS_INLINE
        const_indirect_array_type &indirect2 () const {
            return ia2_;
        }
        BOOST_UBLAS_INLINE
        indirect_array_type &indirect2 () {
            return ia2_;
        }

#ifdef BOOST_UBLAS_DEPRECATED
        // Resetting
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data) {
            // data_ = data;
            data_.reset (data);
        }
        BOOST_UBLAS_INLINE
        void reset (matrix_type &data, const indirect_array_type &ia1, const indirect_array_type &ia2) {
            // data_ = data;
            data_.reset (data);
            ia1_ = ia1;
            ia2_ = ia2;
        }
#endif

        // Element access
#ifndef BOOST_UBLAS_PROXY_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i, size_type j) const {
            return data () (ia1_ (i), ia2_ (j));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) {
            return data () (ia1_ (i), ia2_ (j));
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i, size_type j) const {
            return data () (ia1_ (i), ia2_ (j));
        }
#endif

        BOOST_UBLAS_INLINE
        matrix_indirect<matrix_type, indirect_array_type> project (const range &r1, const range &r2) const {
            return matrix_indirect<matrix_type, indirect_array_type> (data_, ia1_.compose (r1.preprocess (data_.size1 ())), ia2_.compose (r2.preprocess (data_.size2 ())), 0);
        }
        BOOST_UBLAS_INLINE
        matrix_indirect<matrix_type, indirect_array_type> project (const slice &s1, const slice &s2) const {
            return matrix_indirect<matrix_type, indirect_array_type> (data_, ia1_.compose (s1.preprocess (data_.size1 ())), ia2_.compose (s2.preprocess (data_.size2 ())), 0);
        }
        BOOST_UBLAS_INLINE
        matrix_indirect<matrix_type, indirect_array_type> project (const indirect_array_type &ia1, const indirect_array_type &ia2) const {
            return matrix_indirect<matrix_type, indirect_array_type> (data_, ia1_.compose (ia1.preprocess (data_.size1 ())), ia2_.compose (ia2.preprocess (data_.size2 ())), 0);
        }

        // Assignment
        BOOST_UBLAS_INLINE
        matrix_indirect &operator = (const matrix_indirect &mi) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, mi);
            return *this;
        }
        BOOST_UBLAS_INLINE
        matrix_indirect &assign_temporary (matrix_indirect &mi) {
            return *this = mi;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect &operator = (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect &assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect& operator += (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this + ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect &plus_assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_plus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect& operator -= (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_assign<reference, value_type> (), *this, matrix<value_type> (*this - ae));
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        matrix_indirect &minus_assign (const matrix_expression<AE> &ae) {
            matrix_assign (scalar_minus_assign<reference, BOOST_UBLAS_TYPENAME AE::value_type> (), *this, ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_indirect& operator *= (const AT &at) {
            matrix_assign_scalar (scalar_multiplies_assign<reference, AT> (), *this, at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        matrix_indirect& operator /= (const AT &at) {
            matrix_assign_scalar (scalar_divides_assign<reference, AT> (), *this, at);
            return *this;
        }

        // Comparison
        BOOST_UBLAS_INLINE
        bool operator == (const matrix_indirect &mi) const {
            return (*this).data () == mi.data () && ia1_ == mi.ia1_ && ia2_ == mi.ia2_;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (matrix_indirect mi) {
            // Too unusual semantic.
            // BOOST_UBLAS_CHECK (this != &mi, external_logic ());
            if (this != &mi) {
                BOOST_UBLAS_CHECK (size1 () == mi.size1 (), bad_size ());
                BOOST_UBLAS_CHECK (size2 () == mi.size2 (), bad_size ());
                matrix_swap (scalar_swap<reference, reference> (), *this, mi);
            }
        }
#ifndef BOOST_UBLAS_NO_MEMBER_FRIENDS
        BOOST_UBLAS_INLINE
        friend void swap (matrix_indirect mi1, matrix_indirect mi2) {
            mi1.swap (mi2);
        }
#endif

#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_iterator1<matrix_indirect<matrix_type, indirect_array_type>,
                                  BOOST_UBLAS_TYPENAME matrix_type::iterator1::iterator_category> iterator1;
        typedef indexed_iterator2<matrix_indirect<matrix_type, indirect_array_type>,
                                  BOOST_UBLAS_TYPENAME matrix_type::iterator2::iterator_category> iterator2;
        typedef indexed_const_iterator1<matrix_indirect<matrix_type, indirect_array_type>,
                                        BOOST_UBLAS_TYPENAME matrix_type::const_iterator1::iterator_category> const_iterator1;
        typedef indexed_const_iterator2<matrix_indirect<matrix_type, indirect_array_type>,
                                        BOOST_UBLAS_TYPENAME matrix_type::const_iterator2::iterator_category> const_iterator2;
#else
        class const_iterator1;
        class iterator1;
        class const_iterator2;
        class iterator2;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base1<const_iterator1, value_type, const_reference> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1, value_type, reference> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2, value_type, const_reference> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2, value_type, reference> reverse_iterator2;
#else
        typedef reverse_iterator_base1<const_iterator1> const_reverse_iterator1;
        typedef reverse_iterator_base1<iterator1> reverse_iterator1;
        typedef reverse_iterator_base2<const_iterator2> const_reverse_iterator2;
        typedef reverse_iterator_base2<iterator2> reverse_iterator2;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator1 find1 (int /* rank */, size_type i, size_type j) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator1 (*this, i, j);
#else
            return const_iterator1 (*this, ia1_.begin () + i, ia2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator1 find1 (int /* rank */, size_type i, size_type j) {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator1 (*this, i, j);
#else
            return iterator1 (*this, ia1_.begin () + i, ia2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        const_iterator2 find2 (int /* rank */, size_type i, size_type j) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator2 (*this, i, j);
#else
            return const_iterator2 (*this, ia1_.begin () + i, ia2_.begin () + j);
#endif
        }
        BOOST_UBLAS_INLINE
        iterator2 find2 (int /* rank */, size_type i, size_type j) {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return iterator2 (*this, i, j);
#else
            return iterator2 (*this, ia1_.begin () + i, ia2_.begin () + j);
#endif
        }

        // Iterators simply are indices.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator1:
            public container_const_reference<matrix_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::const_iterator1::iterator_category>::template
                        iterator_base<const_iterator1, value_type>::type {
#else
            public random_access_iterator_base<typename M::const_iterator1::iterator_category,
                                               const_iterator1, value_type> {
#endif
        public:
            typedef typename M::const_iterator1::iterator_category iterator_category;
            typedef typename M::const_iterator1::difference_type difference_type;
            typedef typename M::const_iterator1::value_type value_type;
            typedef typename M::const_iterator1::reference reference;
            typedef typename M::const_iterator1::pointer pointer;
            typedef const_iterator2 dual_iterator_type;
            typedef const_reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator1 ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const self_type &mi, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (mi), it1_ (it1), it2_ (it2) {}
            BOOST_UBLAS_INLINE
            const_iterator1 (const iterator1 &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ - it.it1_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 begin () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator2 end () const {
                return const_iterator2 ((*this) (), it1_, it2_ ().end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rbegin () const {
                return const_reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator2 rend () const {
                return const_reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator1 &operator = (const const_iterator1 &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ == it.it1_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ < it.it1_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator1 begin1 () const {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator1 end1 () const {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator1:
            public container_reference<matrix_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::iterator1::iterator_category>::template
                        iterator_base<iterator1, value_type>::type {
#else
            public random_access_iterator_base<typename M::iterator1::iterator_category,
                                               iterator1, value_type> {
#endif
        public:
            typedef typename M::iterator1::iterator_category iterator_category;
            typedef typename M::iterator1::difference_type difference_type;
            typedef typename M::iterator1::value_type value_type;
            typedef typename M::iterator1::reference reference;
            typedef typename M::iterator1::pointer pointer;
            typedef iterator2 dual_iterator_type;
            typedef reverse_iterator2 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator1 ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator1 (self_type &mi, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (mi), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator1 &operator ++ () {
                ++ it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -- () {
                -- it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator1 &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ - it.it1_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 begin () const {
                return iterator2 ((*this) (), it1_, it2_ ().begin ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator2 end () const {
                return iterator2 ((*this) (), it1_, it2_ ().end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rbegin () const {
                return reverse_iterator2 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator2 rend () const {
                return reverse_iterator2 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator1 &operator = (const iterator1 &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ == it.it1_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator1 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ < it.it1_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator1;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator1 begin1 () {
            return find1 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator1 end1 () {
            return find1 (0, size1 (), 0);
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator2:
            public container_const_reference<matrix_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::const_iterator2::iterator_category>::template
                        iterator_base<const_iterator2, value_type>::type {
#else
            public random_access_iterator_base<typename M::const_iterator2::iterator_category,
                                               const_iterator2, value_type> {
#endif
        public:
            typedef typename M::const_iterator2::iterator_category iterator_category;
            typedef typename M::const_iterator2::difference_type difference_type;
            typedef typename M::const_iterator2::value_type value_type;
            typedef typename M::const_iterator2::reference reference;
            typedef typename M::const_iterator2::pointer pointer;
            typedef const_iterator1 dual_iterator_type;
            typedef const_reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator2 ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const self_type &mi, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (mi), it1_ (it1), it2_ (it2) {}
            BOOST_UBLAS_INLINE
            const_iterator2 (const iterator2 &it):
                container_const_reference<self_type> (it ()), it1_ (it.it1_), it2_ (it.it2_) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ - it.it2_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 begin () const {
                return const_iterator1 ((*this) (), it1_ ().begin (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_iterator1 end () const {
                return const_iterator1 ((*this) (), it1_ ().end (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rbegin () const {
                return const_reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            const_reverse_iterator1 rend () const {
                return const_reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator2 &operator = (const const_iterator2 &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator2 begin2 () const {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        const_iterator2 end2 () const {
            return find2 (0, 0, size2 ());
        }

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class iterator2:
            public container_reference<matrix_indirect>,
#ifdef BOOST_UBLAS_USE_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename M::iterator2::iterator_category>::template
                        iterator_base<iterator2, value_type>::type {
#else
            public random_access_iterator_base<typename M::iterator2::iterator_category,
                                               iterator2, value_type> {
#endif
        public:
            typedef typename M::iterator2::iterator_category iterator_category;
            typedef typename M::iterator2::difference_type difference_type;
            typedef typename M::iterator2::value_type value_type;
            typedef typename M::iterator2::reference reference;
            typedef typename M::iterator2::pointer pointer;
            typedef iterator1 dual_iterator_type;
            typedef reverse_iterator1 dual_reverse_iterator_type;

            // Construction and destruction
            BOOST_UBLAS_INLINE
            iterator2 ():
                container_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            iterator2 (self_type &mi, const iterator1_type &it1, const iterator2_type &it2):
                container_reference<self_type> (mi), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            iterator2 &operator ++ () {
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -- () {
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            iterator2 &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ - it.it2_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            reference operator * () const {
                return (*this) ().data () (*it1_, *it2_);
            }

#ifndef BOOST_UBLAS_NO_NESTED_CLASS_RELATION
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 begin () const {
                return iterator1 ((*this) (), it1_ ().begin (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            iterator1 end () const {
                return iterator1 ((*this) (), it1_ ().end (), it2_);
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rbegin () const {
                return reverse_iterator1 (end ());
            }
            BOOST_UBLAS_INLINE
#ifdef BOOST_UBLAS_MSVC_NESTED_CLASS_RELATION
            typename self_type::
#endif
            reverse_iterator1 rend () const {
                return reverse_iterator1 (begin ());
            }
#endif

            // Indices
            BOOST_UBLAS_INLINE
            size_type index1 () const {
                return it1_.index ();
            }
            BOOST_UBLAS_INLINE
            size_type index2 () const {
                return it2_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            iterator2 &operator = (const iterator2 &it) {
                container_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const iterator2 &it) const {
                BOOST_UBLAS_CHECK ((*this) () == it (), external_logic ());
                BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ < it.it2_;
            }

        private:
            iterator1_type it1_;
            iterator2_type it2_;

            friend class const_iterator2;
        };
#endif

        BOOST_UBLAS_INLINE
        iterator2 begin2 () {
            return find2 (0, 0, 0);
        }
        BOOST_UBLAS_INLINE
        iterator2 end2 () {
            return find2 (0, 0, size2 ());
        }

        // Reverse iterators

        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rbegin1 () const {
            return const_reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator1 rend1 () const {
            return const_reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator1 rbegin1 () {
            return reverse_iterator1 (end1 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator1 rend1 () {
            return reverse_iterator1 (begin1 ());
        }

        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rbegin2 () const {
            return const_reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator2 rend2 () const {
            return const_reverse_iterator2 (begin2 ());
        }

        BOOST_UBLAS_INLINE
        reverse_iterator2 rbegin2 () {
            return reverse_iterator2 (end2 ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator2 rend2 () {
            return reverse_iterator2 (begin2 ());
        }

    private:
        matrix_closure_type data_;
        indirect_array_type ia1_;
        indirect_array_type ia2_;
        static matrix_type nil_;
    };

    template<class M, class IA>
    typename matrix_indirect<M, IA>::matrix_type matrix_indirect<M, IA>::nil_;

    // Projections
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M, class IA>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, IA> project (matrix_indirect<M, IA> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
    template<class M, class IA>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, IA> project (const matrix_indirect<M, IA> &data, const range &r1, const range &r2) {
        return data.project (r1, r2);
    }
    template<class M, class IA>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, IA> project (matrix_indirect<M, IA> &data, const slice &s1, const slice &s2) {
        return data.project (s1, s2);
    }
    template<class M, class IA>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, IA> project (const matrix_indirect<M, IA> &data, const slice &s1, const slice &s2) {
        return data.project (s1, s2);
    }
#endif
    // These signatures are too general for MSVC
    // template<class M, class IA>
    // BOOST_UBLAS_INLINE
    // matrix_indirect<M, IA> project (M &data, const IA &ia1, const IA &ia2) {
    //     return matrix_indirect<M, IA> (data, ia1, ia2);
    // }
    // template<class M, class IA>
    // BOOST_UBLAS_INLINE
    // matrix_indirect<const M, IA> project_const (const M &data, const IA &ia1, const IA &ia2) {
    //     return matrix_indirect<const M, IA> (data, ia1, ia2);
    // }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    // template<class M, class IA>
    // BOOST_UBLAS_INLINE
    // matrix_indirect<M, IA> project (const M &data, const IA &ia1, const IA &ia2) {
    //     return matrix_indirect<M, IA> (const_cast<M &> (data), ia1, ia2);
    // }
    // template<class M, class IA>
    // BOOST_UBLAS_INLINE
    // matrix_indirect<M, IA> project (matrix_indirect<M, IA> &data, const IA &ia1, const IA &ia2) {
    //     return data.project (ia1, ia2);
    // }
    // template<class M, class IA>
    // BOOST_UBLAS_INLINE
    // matrix_indirect<M, IA> project (const matrix_indirect<M, IA> &data, const IA &ia1, const IA &ia2) {
    //     return data.project (ia1, ia2);
    // }
#endif
    template<class M, class A>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, indirect_array<A> > project (M &data, const indirect_array<A> &ia1, const indirect_array<A> &ia2) {
        return matrix_indirect<M, indirect_array<A> > (data, ia1, ia2);
    }
    template<class M, class A>
    BOOST_UBLAS_INLINE
    matrix_indirect<const M, indirect_array<A> > project_const (const M &data, const indirect_array<A> &ia1, const indirect_array<A> &ia2) {
        return matrix_indirect<const M, indirect_array<A> > (data, ia1, ia2);
    }
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    template<class M, class A>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, indirect_array<A> > project (const M &data, const indirect_array<A> &ia1, const indirect_array<A> &ia2) {
        return matrix_indirect<M, indirect_array<A> > (const_cast<M &> (data), ia1, ia2);
    }
    template<class M, class A>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, indirect_array<A> > project (matrix_indirect<M, indirect_array<A> > &data, const indirect_array<A> &ia1, const indirect_array<A> &ia2) {
        return data.project (ia1, ia2);
    }
    template<class M, class A>
    BOOST_UBLAS_INLINE
    matrix_indirect<M, indirect_array<A> > project (const matrix_indirect<M, indirect_array<A> > &data, const indirect_array<A> &ia1, const indirect_array<A> &ia2) {
        return data.project (ia1, ia2);
    }
#endif

}}}

#endif






