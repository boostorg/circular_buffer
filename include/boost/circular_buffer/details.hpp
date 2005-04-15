// Helper classes and functions for the circular buffer.

// Copyright (c) 2003-2004 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_DETAILS_HPP)
#define BOOST_CIRCULAR_BUFFER_DETAILS_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

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

/*!
    \struct cb_destroy_tag
    \brief Identifying tag for types which have to be destroyed when replaced.
*/
struct cb_destroy_tag {
#if BOOST_WORKAROUND(__BORLANDC__, < 0x6000)
    char dummy_; // BCB: by default empty structure has 8 bytes
#endif
};

/*!
    \struct cb_assign_tag
    \brief Identifying tag for types which do not have to be destroyed when replaced.
*/
struct cb_assign_tag {
#if BOOST_WORKAROUND(__BORLANDC__, < 0x6000)
    char dummy_; // BCB: by default empty structure has 8 bytes
#endif
};

/*!
    \struct cb_replace_category
    \brief Defines replace category for the given type.
*/
template <bool>
struct cb_replace_category {
    //! Represents types which have to be destroyed.
    typedef cb_destroy_tag replace_category;
};

template <>
struct cb_replace_category<true> {
    //! Represents types which do not have to be destroyed.
    typedef cb_assign_tag replace_category;
};

/*!
    \struct cb_replace_category_traits
    \brief Defines the replace category tag for the given type.
*/
template <class Type>
struct cb_replace_category_traits {
    //! Replace category tag type.
    /*!
        Depending on the template parameter the <code>tag</code> distinguishes
        between types which have to be destroyed (e.g. class) and types which
        do not have to be (e.g. integral type) when replaced.
    */
    typedef typename cb_details::cb_replace_category<
        is_scalar<Type>::value>::replace_category tag;
};

/*!
    \struct cb_helper_pointer
    \brief Helper pointer used in the cb_iterator.
*/
template <class Pointer>
struct cb_helper_pointer {
    bool m_end;
    Pointer m_it;
};

/*!
    \class cb_iterator
    \brief Random access iterator for the circular buffer.
    \param Buff The type of the underlying circular buffer.
    \param Traits Defines basic iterator types.
    \note This iterator is not circular. It was designed
          for iterating from begin() to end() of the circular buffer.
*/
template <class Buff, class Value> 
class cb_iterator : 
    public boost::iterator_facade<
        cb_iterator<Buff, Value>, Value, boost::random_access_traversal_tag>,
    public cb_iterator_base
{
private:
    // Helper types

    //! Base iterator.
    typedef boost::iterator_facade<
        cb_iterator, Value, boost::random_access_traversal_tag> base_type;

    typedef cb_iterator<Buff, typename boost::remove_const<Value>::type> nonconst_self;

public:
// Basic types

    //! The type of the elements stored in the circular buffer.
    typedef typename base_type::value_type value_type;

    //! Pointer to the element.
    typedef typename base_type::pointer pointer;

    //! Reference to the element.
    typedef typename base_type::reference reference;

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
    cb_iterator(const Buff* cb, const pointer it) : cb_iterator_base(cb), m_buff(cb), m_it(it) {}

    // Assign operator.
    cb_iterator& operator = (const cb_iterator& it) {
        if (this == &it)
            return *this;
        cb_iterator_base::operator =(it);
        m_buff = it.m_buff;
        m_it = it.m_it;
        return *this;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

// Friends
// !!! TODO
#if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    friend boost::iterator_core_access;
#else
    friend class boost::iterator_core_access;
#endif

private:
// Implementation of iterator_facade methods

    //! Dereferencing.
    reference dereference () const {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != 0);  // check for iterator pointing to end()
        return *m_it;
    }

    //! Difference.
    difference_type distance_to (const cb_iterator& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        cb_helper_pointer<pointer> rhs = create_helper_pointer(*this);
        cb_helper_pointer<pointer> lhs = create_helper_pointer(it);
        if (less(rhs, lhs) && lhs.m_it <= rhs.m_it)
            return (lhs.m_it - rhs.m_it) + static_cast<difference_type>(m_buff->capacity()) ;
        if (less(lhs, rhs) && lhs.m_it >= rhs.m_it)
            return (lhs.m_it - rhs.m_it) - static_cast<difference_type>(m_buff->capacity()) ;
        return lhs.m_it - rhs.m_it;
    }

    //! Increment.
    void increment () {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != 0);  // check for iterator pointing to end()
        m_buff->increment(m_it);
        if (m_it == m_buff->m_last)
            m_it = 0;
    }

    //! Decrement.
    void decrement () {
        BOOST_CB_ASSERT(is_valid());              // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_it != m_buff->m_first); // check for iterator pointing to begin()
        if (m_it == 0)
            m_it = m_buff->m_last;
        m_buff->decrement(m_it);
    }

    //! Advancing.
    void advance (difference_type n) {
        BOOST_CB_ASSERT(is_valid()); // check for uninitialized or invalidated iterator
        if (n > 0) {
            BOOST_CB_ASSERT(m_buff->end() - *this >= n); // check for too large n
            m_it = m_buff->add(m_it, n);
            if (m_it == m_buff->m_last)
                m_it = 0;
        } else if (n < 0) {
            BOOST_CB_ASSERT(m_buff->begin() - *this <= n); // check for too large n
            m_it = m_buff->sub(m_it == 0 ? m_buff->m_last : m_it, -n);
        }
    }

    //! Equality.
    template <class Value0>
    bool equal (const cb_iterator<Buff, Value0>& it) const {
        BOOST_CB_ASSERT(is_valid());          // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(it.is_valid());       // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(m_buff == it.m_buff); // check for iterators of different containers
        return m_it == it.m_it;
    }

// Helpers

    //! Create helper pointer.
    cb_helper_pointer<pointer> create_helper_pointer(const cb_iterator& it) const {
        cb_helper_pointer<pointer> helper;
        helper.m_end = (it.m_it == 0);
        helper.m_it = helper.m_end ? m_buff->m_last : it.m_it;
        return helper;
    }

    //! Less.
    template <class InternalIterator0, class InternalIterator1>
    bool less(const InternalIterator0& lhs, const InternalIterator1& rhs) const {
        difference_type ldiff = lhs.m_it - m_buff->m_first;
        difference_type rdiff = rhs.m_it - m_buff->m_first;
        if (ldiff < 0)
        {
            if (rdiff < 0)
                return lhs.m_it < rhs.m_it;
            else if (rdiff == 0)
                return rhs.m_end;
            else
                return false;
        }
        else if (ldiff == 0)
        {
            if (rdiff < 0)
                return !lhs.m_end;
            else if (rdiff == 0)
                return !lhs.m_end && rhs.m_end;
            else
                return !lhs.m_end;
        }
        else // ldiff > 0
        {
            if (rdiff < 0)
                return true;
            else if (rdiff == 0)
                return rhs.m_end;
            else
                return lhs.m_it < rhs.m_it;
        }
        return false;
    }
};

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
