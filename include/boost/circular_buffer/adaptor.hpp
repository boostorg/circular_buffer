// Implementation of the circular buffer adaptor.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_ADAPTOR_HPP)
#define BOOST_CIRCULAR_BUFFER_ADAPTOR_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

namespace boost {

/*!
    \class circular_buffer_space_optimized
    \brief Space optimized circular buffer container adaptor.
    \param T The type of the elements stored in the space optimized circular buffer.
    \param Alloc The allocator type used for all internal memory management.
           Default: std::allocator<T>
    \author <a href="mailto:jano_gaspar[at]yahoo.com">Jan Gaspar</a>
    \version 1.4
    \date 2005

    For more information how to use the space optimized circular
    buffer see the <a href="../circular_buffer_adaptor.html">
    documentation</a>.
*/
template<class T, class Alloc>
class circular_buffer_space_optimized : private circular_buffer<T, Alloc> {
public:
// Typedefs

    typedef typename circular_buffer<T, Alloc>::value_type value_type;
    typedef typename circular_buffer<T, Alloc>::pointer pointer;
    typedef typename circular_buffer<T, Alloc>::const_pointer const_pointer;
    typedef typename circular_buffer<T, Alloc>::reference reference;
    typedef typename circular_buffer<T, Alloc>::const_reference const_reference;
    typedef typename circular_buffer<T, Alloc>::size_type size_type;
    typedef typename circular_buffer<T, Alloc>::difference_type difference_type;
    typedef typename circular_buffer<T, Alloc>::allocator_type allocator_type;
    typedef typename circular_buffer<T, Alloc>::param_value_type param_value_type;
    typedef typename circular_buffer<T, Alloc>::return_value_type return_value_type;
    typedef typename circular_buffer<T, Alloc>::const_iterator const_iterator;
    typedef typename circular_buffer<T, Alloc>::iterator iterator;
    typedef typename circular_buffer<T, Alloc>::const_reverse_iterator const_reverse_iterator;
    typedef typename circular_buffer<T, Alloc>::reverse_iterator reverse_iterator;

// Inherited

    using circular_buffer<T, Alloc>::get_allocator;
    using circular_buffer<T, Alloc>::begin;
    using circular_buffer<T, Alloc>::end;
    using circular_buffer<T, Alloc>::rbegin;
    using circular_buffer<T, Alloc>::rend;
    using circular_buffer<T, Alloc>::at;
    using circular_buffer<T, Alloc>::front;
    using circular_buffer<T, Alloc>::back;
    using circular_buffer<T, Alloc>::data;
    using circular_buffer<T, Alloc>::size;
    using circular_buffer<T, Alloc>::max_size;
    using circular_buffer<T, Alloc>::empty;

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    reference operator [] (size_type n) { return circular_buffer<T, Alloc>::operator[](n); }
    return_value_type operator [] (size_type n) const { return circular_buffer<T, Alloc>::operator[](n); }
#else
    using circular_buffer<T, Alloc>::operator[];
#endif

private:
// Member variables

    //! The capacity of the optimized circular buffer.
    size_type m_capacity;

    //! The lowest guaranteed capacity of the adapted circular buffer.
    size_type m_min_capacity;

public:
// Overridden

    //! See the circular_buffer source documentation.
    bool full() const { return size() == capacity(); }

    //! Return the minimal guaranteed amount of allocated memory.
    /*
        The allocated memory will never drop under this value.
    */
    size_type min_capacity() const { return m_min_capacity; }

    //! Change the minimal guaranteed amount of allocated memory.
    /*!
        \pre <code>(*this).capacity() >= new_min_capacity</code>
        \post <code>(*this).min_capacity() == new_min_capacity</code>
              Allocates memory specified by the <code>new_min_capacity</code> parameter.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>new_min_capacity > (*this).capacity()</code>) and an assertion
              will be invoked in the debug mode.
    */
    void set_min_capacity(size_type new_min_capacity) {
        BOOST_CB_ASSERT(capacity() >= new_min_capacity); // check for too large new min_capacity
        m_min_capacity = new_min_capacity;
        if (new_min_capacity > circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(new_min_capacity);
        else
            check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    size_type capacity() const { return m_capacity; }

#if defined(BOOST_CB_TEST)

    // Return the current capacity of the adapted circular buffer.
    /*
       \note This method is not intended to be used directly by the user.
             It is defined only for testing purposes.
    */
    size_type internal_capacity() const { return circular_buffer<T, Alloc>::capacity(); }

#endif // #if defined(BOOST_CB_TEST)

    //!! See the circular_buffer source documentation.
    /*!
         \pre <code>(*this).min_capacity() <= new_capacity</code>
         \note It is considered as a bug if the precondition is not met (i.e. if
               <code>new_capacity > (*this).min_capacity()</code>) and an assertion
               will be invoked in the debug mode.
    */
    void set_capacity(size_type new_capacity, bool remove_front = true) {
        BOOST_CB_ASSERT(new_capacity >= min_capacity()); // check for too low new capacity
        if (new_capacity < circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(new_capacity, remove_front);
        m_capacity = new_capacity;
    }

    //! See the circular_buffer source documentation.
    void resize(size_type new_size, param_value_type item = T(), bool remove_front = true) {
        if (new_size > size()) {
            if (new_size > capacity())
                m_capacity = new_size;
            insert(end(), new_size - size(), item);
        } else {
            if (remove_front)
                erase(begin(), end() - new_size);
            else
                erase(begin() + new_size, end());
        }
    }

    //! Create an empty space optimized circular buffer with a given capacity.
    /*!
        \param capacity The capacity of the buffer.
        \param min_capacity The minimal guaranteed amount of allocated memory.
               (The metrics of the min_capacity is number of items.)
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code>
        \post <code>(*this).capacity() == capacity \&\& (*this).size == 0</code><br>
              Allocates memory specified by the <code>min_capacity</code> parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    explicit circular_buffer_space_optimized(
        size_type capacity,
        size_type min_capacity = 0,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(min_capacity, alloc)
    , m_capacity(capacity)
    , m_min_capacity(min_capacity) {
        BOOST_CB_ASSERT(capacity >= min_capacity); // check for capacity lower than min_capacity
    }

    //! Create a full space optimized circular buffer filled with copies of <code>item</code>.
    /*!
        \param capacity The capacity of the buffer.
        \param min_capacity The minimal guaranteed amount of allocated memory.
               (The metrics of the min_capacity is number of items.)
        \param item The item to be filled with.
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code>
        \post <code>(*this).size() == capacity \&\& (*this)[0] == (*this)[1] == ... == (*this).back() == item</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    circular_buffer_space_optimized(
        size_type capacity,
        size_type min_capacity,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(capacity, item, alloc)
    , m_capacity(capacity)
    , m_min_capacity(min_capacity) {
        BOOST_CB_ASSERT(capacity >= min_capacity); // check for capacity lower than min_capacity
    }

    // Default copy constructor

    //! Create a space optimized circular buffer with a copy of a range.
    /*!
        \param capacity The capacity of the buffer.
        \param min_capacity The minimal guaranteed amount of allocated memory.
              (The metrics of the min_capacity is number of items.)
        \param first The beginning of the range.
        \param last The end of the range.
        \param alloc The allocator.
        \pre <code>capacity >= min_capacity</code> and valid range <code>[first, last)</code>.
        \post <code>(*this).capacity() == capacity</code><br>
              Allocates at least as much memory as specified by the
              <code>min_capacity</code> parameter.<br>
              If the number of items to copy from the range
              <code>[first, last)</code> is greater than the specified
              <code>capacity</code> then only elements from the range
              <code>[last - capacity, last)</code> will be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note It is considered as a bug if the precondition is not met (i.e. if
              <code>capacity < min_capacity</code>) and an assertion will be invoked
              in the debug mode.
    */
    template <class InputIterator>
    circular_buffer_space_optimized(
        size_type capacity,
        size_type min_capacity,
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(
        init_capacity(capacity, min_capacity, first, last), first, last, alloc)
    , m_capacity(capacity)
    , m_min_capacity(min_capacity) {
        BOOST_CB_ASSERT(capacity >= min_capacity);        // check for capacity lower than min_capacity
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
    }

    // Default destructor

#if defined(BOOST_CB_NEVER_DEFINED)

    // Assignment operator - declared only for documentation purpose.
    /*
       \note This section will be never compiled. The default assignment
             operator will be generated instead.
    */
    circular_buffer_space_optimized<T, Alloc>& operator = (const circular_buffer_space_optimized<T, Alloc>& cb);

#endif // #if defined(BOOST_CB_NEVER_DEFINED)

    //! See the circular_buffer source documentation.
    void assign(size_type n, param_value_type item) {
        if (n > m_capacity)
            m_capacity = n;
        circular_buffer<T, Alloc>::assign(n, item);
    }

    //! See the circular_buffer source documentation.
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        circular_buffer<T, Alloc>::assign(first, last);
        size_type capacity = circular_buffer<T, Alloc>::capacity();
        if (capacity > m_capacity)
            m_capacity = capacity;
    }

    //! See the circular_buffer source documentation.
    void swap(circular_buffer_space_optimized<T, Alloc>& cb) {
        std::swap(m_capacity, cb.m_capacity);
        std::swap(m_min_capacity, cb.m_min_capacity);
        circular_buffer<T, Alloc>::swap(cb);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_back(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_back(item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_front(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_front(item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_back() {
        circular_buffer<T, Alloc>::pop_back();
        check_high_capacity();
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_front() {
        circular_buffer<T, Alloc>::pop_front();
        check_high_capacity();
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator insert(iterator pos, param_value_type item = value_type()) {
        size_type index = pos - begin();
        check_low_capacity();
        return circular_buffer<T, Alloc>::insert(begin() + index, item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void insert(iterator pos, size_type n, param_value_type item) {
        size_type index = pos - begin();
        check_low_capacity(n);
        circular_buffer<T, Alloc>::insert(begin() + index, n, item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        insert(pos, first, last, BOOST_DEDUCED_TYPENAME cb_details::iterator_cat_traits<InputIterator>::tag());
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rinsert(iterator pos, param_value_type item = value_type()) {
        size_type index = pos - begin();
        check_low_capacity();
        return circular_buffer<T, Alloc>::rinsert(begin() + index, item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void rinsert(iterator pos, size_type n, param_value_type item) {
        size_type index = pos - begin();
        check_low_capacity(n);
        circular_buffer<T, Alloc>::rinsert(begin() + index, n, item);
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last) {
        rinsert(pos, first, last, BOOST_DEDUCED_TYPENAME cb_details::iterator_cat_traits<InputIterator>::tag());
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator erase(iterator pos) {
        iterator it = circular_buffer<T, Alloc>::erase(pos);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator erase(iterator first, iterator last) {
        iterator it = circular_buffer<T, Alloc>::erase(first, last);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rerase(iterator pos) {
        iterator it = circular_buffer<T, Alloc>::rerase(pos);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //!! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    iterator rerase(iterator first, iterator last) {
        iterator it = circular_buffer<T, Alloc>::rerase(first, last);
        size_type index = it - begin();
        check_high_capacity();
        return begin() + index;
    }

    //! See the circular_buffer source documentation.
    void clear() { erase(begin(), end()); }

private:
// Helper methods

    //! Ensure the reserve for possible growth up.
    size_type ensure_reserve(size_type new_capacity, size_type size) const {
        if (size + new_capacity / 5 >= new_capacity)
            new_capacity *= 2; // ensure at least 20% reserve
        if (new_capacity > capacity())
            return capacity();
        return new_capacity;
    }

    //! Check for low capacity.
    /*
        \post If the capacity is low it will be increased.
    */
    void check_low_capacity(size_type n = 1) {
        size_type new_size = size() + n;
        size_type new_capacity = circular_buffer<T, Alloc>::capacity();
        if (new_size > new_capacity) {
            if (new_capacity == 0)
                new_capacity = 1;
            for (; new_size > new_capacity; new_capacity *= 2);
            circular_buffer<T, Alloc>::set_capacity(
                ensure_reserve(new_capacity, new_size));
        }
    }

    //! Check for high capacity.
    /*
        \post If the capacity is high it will be decreased.
    */
    void check_high_capacity() {
        size_type new_capacity = circular_buffer<T, Alloc>::capacity();
        while (new_capacity / 3 >= size()) { // (new_capacity / 3) -> avoid oscillations
            new_capacity /= 2;
            if (new_capacity <= min_capacity()) {
                new_capacity = min_capacity();
                break;
            }
        }
        circular_buffer<T, Alloc>::set_capacity(
            ensure_reserve(new_capacity, size()));
    }

    //! Determine the initial capacity.
    template <class InputIterator>
    static size_type init_capacity(size_type capacity, size_type min_capacity, InputIterator first, InputIterator last) {
        BOOST_CB_IS_CONVERTIBLE(InputIterator, value_type);
        return std::min(capacity, std::max(min_capacity,
            static_cast<size_type>(std::distance(first, last))));
    }

    //! Helper insert method.
    template <class InputIterator>
    void insert(iterator pos, InputIterator n, InputIterator item, cb_details::int_tag) {
        insert(pos, (size_type)n, item);
    }

    //! Helper insert method.
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, cb_details::iterator_tag) {
        size_type index = pos - begin();
        check_low_capacity(std::distance(first, last));
        circular_buffer<T, Alloc>::insert(begin() + index, first, last);
    }

    //! Helper rinsert method.
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator n, InputIterator item, cb_details::int_tag) {
        rinsert(pos, (size_type)n, item);
    }

    //! Helper rinsert method.
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last, cb_details::iterator_tag) {
        size_type index = pos - begin();
        check_low_capacity(std::distance(first, last));
        circular_buffer<T, Alloc>::rinsert(begin() + index, first, last);
    }
};

// Non-member functions

//! Test two space optimized circular buffers for equality.
template <class T, class Alloc>
inline bool operator == (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator < (const circular_buffer_space_optimized<T, Alloc>& lhs,
                        const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))

//! Test two space optimized circular buffers for non-equality.
template <class T, class Alloc>
inline bool operator != (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator > (const circular_buffer_space_optimized<T, Alloc>& lhs,
                        const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return rhs < lhs;
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator <= (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator >= (const circular_buffer_space_optimized<T, Alloc>& lhs,
                         const circular_buffer_space_optimized<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

//! Swap the contents of two space optimized circular buffers.
template <class T, class Alloc>
inline void swap(circular_buffer_space_optimized<T, Alloc>& lhs,
                 circular_buffer_space_optimized<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

#endif // #if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1310))

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_ADAPTOR_HPP)
