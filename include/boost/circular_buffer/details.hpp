// Helper classes and functions for the circular buffer.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_DETAILS_HPP)
#define BOOST_CIRCULAR_BUFFER_DETAILS_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include <boost/iterator.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <iterator>

namespace boost {

namespace cb_details {

// The value the uninitialized memory is filled with.
const int CB_Unitialized = 0xcc;

/*!
    \struct cb_int_iterator_tag
    \brief Identifying tag for integer types (not for iterators).
*/
struct cb_int_iterator_tag {
#if BOOST_WORKAROUND(__BORLANDC__, < 0x6000)
    char dummy_; // BCB: by default empty structure has 8 bytes
#endif
};

/*!
    \struct cb_iterator_category
    \brief Defines iterator category.
*/
template <bool>
struct cb_iterator_category {
    //! Represents iterators.
    typedef std::input_iterator_tag iterator_category;
};

template <>
struct cb_iterator_category<true> {
    //! Represents integral types (not iterators).
    typedef cb_int_iterator_tag iterator_category;
};

/*!
    \struct cb_iterator_category_traits
    \brief Defines the iterator category tag for the given iterator.
*/
template <class Iterator>
struct cb_iterator_category_traits {
    //! Iterator category tag type.
    /*!
        Depending on the template parameter the <code>tag</code> distinguishes
        between iterators and non-iterators. If the template parameter
        is an iterator, the <code>tag</code> is typedef for <code>std::input_iterator_tag</code>.
        If the parameter is not an iterator, the <code>tag</code> is typedef for
        <code>cb_int_iterator_tag</code>.
    */
    typedef typename cb_details::cb_iterator_category<
        is_integral<Iterator>::value>::iterator_category tag;
};

template <class Traits> struct cb_nonconst_traits;

/*!
    \struct cb_const_traits
    \brief Defines the data types for a const iterator.
    \param Traits Defines the basic types.
*/
template <class Traits>
struct cb_const_traits {
    // Basic types
    typedef typename Traits::value_type value_type;
    typedef typename Traits::const_pointer pointer;
    typedef typename Traits::const_reference reference;
    typedef typename Traits::size_type size_type;
    typedef typename Traits::difference_type difference_type;

    // Non-const traits
    typedef cb_nonconst_traits<Traits> nonconst_traits;
};

/*!
    \struct cb_nonconst_traits
    \brief Defines the data types for a non-const iterator.
    \param Traits Defines the basic types.
*/
template <class Traits>
struct cb_nonconst_traits {
    // Basic types
    typedef typename Traits::value_type value_type;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::reference reference;
    typedef typename Traits::size_type size_type;
    typedef typename Traits::difference_type difference_type;

    // Non-const traits
    typedef cb_nonconst_traits<Traits> nonconst_traits;
};

/*!
    \struct cb_helper_pointer
    \brief Helper pointer used in the cb_iterator.
*/
template <class Traits0>
struct cb_helper_pointer {
    bool m_end;
    typename Traits0::pointer m_it;
};

/*!
    \class cb_iterator
    \brief Random access iterator for the circular buffer.
    \param Buff The type of the underlying circular buffer.
    \param Traits Defines basic iterator types.
    \note This iterator is not circular. It was designed
          for iterating from begin() to end() of the circular buffer.
*/
template <class Buff, class Traits>
class cb_iterator :
    public boost::iterator<
    std::random_access_iterator_tag,
    typename Traits::value_type,
    typename Traits::difference_type,
    typename Traits::pointer,
    typename Traits::reference>,
    public cb_iterator_base
{
private:
// Helper types

    //! Base iterator.
    typedef boost::iterator<
        std::random_access_iterator_tag,
        typename Traits::value_type,
        typename Traits::difference_type,
        typename Traits::pointer,
        typename Traits::reference> base_type;

    //! Non-const iterator.
    typedef cb_iterator<Buff, typename Traits::nonconst_traits> nonconst_self;

public:
// Basic types

    //! The type of the elements stored in the circular buffer.
    typedef typename base_type::value_type value_type;

    //! Pointer to the element.
    typedef typename base_type::pointer pointer;

    //! Reference to the element.
    typedef typename base_type::reference reference;

    //! Size type.
    typedef typename Traits::size_type size_type;

    //! Difference type.
    typedef typename base_type::difference_type difference_type;

// Member variables

    //! The circular buffer where the iterator points to.
    const Buff* m_buff;

    //! An internal iterator.
    pointer m_it;

// Construction & assignment

    // Default copy constructor.

    //! Default constructor.
    cb_iterator() : m_buff(0), m_it(0) {}

    //! Copy constructor (used for converting from a non-const to a const iterator).
    cb_iterator(const nonconst_self& it) : cb_iterator_base(it), m_buff(it.m_buff), m_it(it.m_it) {}

    //! Internal constructor.
    /*!
        \note This constructor is not intended to be used directly by the user.
    */
    cb_iterator(const Buff* cb, const pointer p) : cb_iterator_base(cb), m_buff(cb), m_it(p) {}

    // Assign operator.
    cb_iterator& operator = (const cb_iterator& it) {
        if (this == &it)
            return *this;
        cb_iterator_base::operator =(it);
        m_buff = it.m_buff;
        m_it = it.m_it;
        return *this;
    }

// Random access iterator methods

    //! Dereferencing operator.
    reference operator * () const {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != 0);  // check for iterator pointing to end()
        return *m_it;
    }

    //! Dereferencing operator.
    pointer operator -> () const { return &(operator*()); }

    //! Difference operator.
    difference_type operator - (const cb_iterator& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        cb_helper_pointer<Traits> lhs = create_helper_pointer(*this);
        cb_helper_pointer<Traits> rhs = create_helper_pointer(it);
        if (less(rhs, lhs) && lhs.m_it <= rhs.m_it)
            return lhs.m_it + m_buff->capacity() - rhs.m_it;
        if (less(lhs, rhs) && lhs.m_it >= rhs.m_it)
            return lhs.m_it - m_buff->capacity() - rhs.m_it;
        return lhs.m_it - rhs.m_it;
    }

    //! Increment operator (prefix).
    cb_iterator& operator ++ () {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != 0);  // check for iterator pointing to end()
        m_buff->increment(m_it);
        if (m_it == m_buff->m_last)
            m_it = 0;
        return *this;
    }

    //! Increment operator (postfix).
    cb_iterator operator ++ (int) {
        cb_iterator<Buff, Traits> tmp = *this;
        ++*this;
        return tmp;
    }

    //! Decrement operator (prefix).
    cb_iterator& operator -- () {
        BOOST_CB_ASSERT(is_valid());              // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != m_buff->m_first); // check for iterator pointing to begin()
        if (m_it == 0)
            m_it = m_buff->m_last;
        m_buff->decrement(m_it);
        return *this;
    }

    //! Decrement operator (postfix).
    cb_iterator operator -- (int) {
        cb_iterator<Buff, Traits> tmp = *this;
        --*this;
        return tmp;
    }

    //! Iterator addition.
    cb_iterator& operator += (difference_type n) {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        if (n > 0) {
            BOOST_CB_ASSERT(m_buff->end() - *this >= n); // check for too large n
            m_it = m_buff->add(m_it, n);
            if (m_it == m_buff->m_last)
                m_it = 0;
        } else if (n < 0) {
            *this -= -n;
        }
        return *this;
    }

    //! Iterator addition.
    cb_iterator operator + (difference_type n) const { return cb_iterator<Buff, Traits>(*this) += n; }

    //! Iterator subtraction.
    cb_iterator& operator -= (difference_type n) {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        if (n > 0) {
            BOOST_CB_ASSERT(m_buff->begin() - *this <= -n); // check for too large n
            m_it = m_buff->sub(m_it == 0 ? m_buff->m_last : m_it, n);
        } else if (n < 0) {
            *this += -n;
        }
        return *this;
    }

    //! Iterator subtraction.
    cb_iterator operator - (difference_type n) const { return cb_iterator<Buff, Traits>(*this) -= n; }

    //! Element access operator.
    reference operator [] (difference_type n) const { return *(*this + n); }

// Equality & comparison

    //! Equality.
    template <class Traits0>
    bool operator == (const cb_iterator<Buff, Traits0>& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        return m_it == it.m_it;
    }

    //! Inequality.
    template <class Traits0>
    bool operator != (const cb_iterator<Buff, Traits0>& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        return m_it != it.m_it;
    }

    //! Less.
    template <class Traits0>
    bool operator < (const cb_iterator<Buff, Traits0>& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        return less(create_helper_pointer(*this), create_helper_pointer(it));
    }

    //! Greater.
    template <class Traits0>
    bool operator > (const cb_iterator<Buff, Traits0>& it) const  { return it < *this; }

    //! Less or equal.
    template <class Traits0>
    bool operator <= (const cb_iterator<Buff, Traits0>& it) const { return !(it < *this); }

    //! Greater or equal.
    template <class Traits0>
    bool operator >= (const cb_iterator<Buff, Traits0>& it) const { return !(*this < it); }

private:
// Helpers

    //! Create helper pointer.
    template <class Traits0>
    cb_helper_pointer<Traits0> create_helper_pointer(const cb_iterator<Buff, Traits0>& it) const {
        cb_helper_pointer<Traits0> helper;
        helper.m_end = (it.m_it == 0);
        helper.m_it = helper.m_end ? m_buff->m_last : it.m_it;
        return helper;
    }

    //! Compare two pointers.
    /*!
    \return 1 if p1 is greater than p2.
    \return 0 if p1 is equal to p2.
    \return -1 if p1 is lower than p2.
    */
    template <class Pointer0, class Pointer1>
    static difference_type compare(Pointer0 p1, Pointer1 p2) {
        return p1 < p2 ? -1 : (p1 > p2 ? 1 : 0);
    }

    //! Less.
    template <class InternalIterator0, class InternalIterator1>
    bool less(const InternalIterator0& lhs, const InternalIterator1& rhs) const {
        switch (compare(lhs.m_it, m_buff->m_first)) {
        case -1:
            switch (compare(rhs.m_it, m_buff->m_first)) {
            case -1: return lhs.m_it < rhs.m_it;
            case 0: return rhs.m_end;
            case 1: return false;
            }
        case 0:
            switch (compare(rhs.m_it, m_buff->m_first)) {
            case -1: return !lhs.m_end;
            case 0: return !lhs.m_end && rhs.m_end;
            case 1: return !lhs.m_end;
            }
        case 1:
            switch (compare(rhs.m_it, m_buff->m_first)) {
            case -1: return true;
            case 0: return rhs.m_end;
            case 1: return lhs.m_it < rhs.m_it;
            }
        }
        return false;
    }
};

//! Iterator addition.
template <class Buff, class Traits>
inline cb_iterator<Buff, Traits>
operator + (typename Traits::difference_type n, const cb_iterator<Buff, Traits>& it) {
    return it + n;
}

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR)

//! Iterator category.
template <class Buff, class Traits>
inline std::random_access_iterator_tag iterator_category(const cb_iterator<Buff, Traits>&) {
    return std::random_access_iterator_tag();
}

//! The type of the elements stored in the circular buffer.
template <class Buff, class Traits>
inline typename Traits::value_type* value_type(const cb_iterator<Buff, Traits>&) { return 0; }

//! Distance type.
template <class Buff, class Traits>
inline typename Traits::difference_type* distance_type(const cb_iterator<Buff, Traits>&) { return 0; }

#endif // #if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR)

/*!
    \fn FwdIterator uninitialized_copy(InputIterator first, InputIterator last, FwdIterator dest, Alloc& alloc)
    \brief Equivalent of <code>std::uninitialized_copy</code> with allocator.
*/
template<class InputIterator, class FwdIterator, class Alloc>
inline FwdIterator uninitialized_copy(InputIterator first, InputIterator last, FwdIterator dest, Alloc& alloc) {
    FwdIterator next = dest;
    BOOST_CB_TRY
    for (; first != last; ++first, ++dest)
        alloc.construct(dest, *first);
    BOOST_CB_UNWIND(
        for (; next != dest; ++next)
            alloc.destroy(next);
    )
    return dest;
}

/*!
    \fn void uninitialized_fill_n(FwdIterator first, Diff n, const T& item, Alloc& alloc)
    \brief Equivalent of <code>std::uninitialized_fill_n</code> with allocator.
*/
template<class FwdIterator, class Diff, class T, class Alloc>
inline void uninitialized_fill_n(FwdIterator first, Diff n, const T& item, Alloc& alloc) {
    FwdIterator next = first;
    BOOST_CB_TRY
        for (; n > 0; ++first, --n)
            alloc.construct(first, item);
    BOOST_CB_UNWIND(
        for (; next != first; ++next)
            alloc.destroy(next);
    )
}

} // namespace cb_details

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_DETAILS_HPP)
