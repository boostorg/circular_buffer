// Implementation of the circular buffer adaptor.

// Copyright (c) 2003-2007 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP)
#define BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

namespace boost {

/*!
    \class circular_buffer_space_optimized
    \brief Space optimized circular buffer container adaptor.

    For detailed documentation of the space_optimized_circular_buffer visit:
    http://www.boost.org/libs/circular_buffer/doc/circular_buffer_space_optimized.html
*/
template <class T, class Alloc>
class circular_buffer_space_optimized :
/*! \cond */
#if BOOST_CB_ENABLE_DEBUG
public
#endif
/*! \endcond */
circular_buffer<T, Alloc> {
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
    typedef typename circular_buffer<T, Alloc>::const_iterator const_iterator;
    typedef typename circular_buffer<T, Alloc>::iterator iterator;
    typedef typename circular_buffer<T, Alloc>::const_reverse_iterator const_reverse_iterator;
    typedef typename circular_buffer<T, Alloc>::reverse_iterator reverse_iterator;
    typedef typename circular_buffer<T, Alloc>::array_range array_range;
    typedef typename circular_buffer<T, Alloc>::const_array_range const_array_range;
    typedef typename circular_buffer<T, Alloc>::param_value_type param_value_type;
    typedef typename circular_buffer<T, Alloc>::return_value_type return_value_type;

#if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    //! Capacity controller of the space optimized circular buffer.
    /*!
        <p><pre>
class capacity_control {
   size_type m_capacity;
   size_type m_min_capacity;
public:
   capacity_control(size_type capacity, size_type min_capacity = 0) : m_capacity(capacity), m_min_capacity(min_capacity) {};
   size_type %capacity() const { return m_capacity; }
   size_type min_capacity() const { return m_min_capacity; }
   operator size_type() const { return m_capacity; }
};</pre></p>
        \pre <code>capacity >= min_capacity</code>
        <p>The <code>m_capacity</code> represents the capacity of the <code>circular_buffer_space_optimized</code> and
        the <code>m_min_capacity</code> determines the minimal allocated size of its internal buffer.</p>
        <p>The converting constructor of the <code>capacity_control</code> allows implicit conversion from
        <code>size_type</code>-like types which ensures compatibility of creating an instance of the
        <code>circular_buffer_space_optimized</code> with other STL containers. On the other hand the operator
        <code>%size_type()</code> (returning <code>m_capacity</code>) provides implicit conversion to the
        <code>size_type</code> which allows to treat the capacity of the <code>circular_buffer_space_optimized</code>
        the same way as in the <code><a href="circular_buffer.html">circular_buffer</a></code>.</p>
    */
    typedef cb_details::capacity_control<size_type, T, Alloc> capacity_type;
#else
    /*! \cond */
    typedef cb_details::capacity_control<size_type> capacity_type;
    /*! \endcond */
#endif // #if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

// Inherited

    using circular_buffer<T, Alloc>::get_allocator;
    using circular_buffer<T, Alloc>::begin;
    using circular_buffer<T, Alloc>::end;
    using circular_buffer<T, Alloc>::rbegin;
    using circular_buffer<T, Alloc>::rend;
    using circular_buffer<T, Alloc>::at;
    using circular_buffer<T, Alloc>::front;
    using circular_buffer<T, Alloc>::back;
    using circular_buffer<T, Alloc>::array_one;
    using circular_buffer<T, Alloc>::array_two;
    using circular_buffer<T, Alloc>::linearize;
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

    //! The capacity controller of the space optimized circular buffer.
    capacity_type m_capacity_ctrl;

public:
// Overridden

    //! Is the <code>circular_buffer_space_optimized</code> full?
    /*!
        \return <code>true</code> if the number of elements stored in the <code>circular_buffer_space_optimized</code>
                equals the capacity of the <code>circular_buffer_space_optimized</code>; <code>false</code> otherwise.
        \throws Nothing.
        \par Exception Safety
             No-throw.
        \par Iterator Invalidation
             Does not invalidate any iterators.
        \par Complexity
             Constant (in the size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>empty()</code>
    */
    bool full() const { return m_capacity_ctrl.capacity() == size(); }

    /*! \brief Get the maximum number of elements which can be inserted into the
               <code>circular_buffer_space_optimized</code> without overwriting any of already stored elements.
        \return <code>capacity() - size()</code>
        \throws Nothing.
        \par Exception Safety
             No-throw.
        \par Iterator Invalidation
             Does not invalidate any iterators.
        \par Complexity
             Constant (in the size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>capacity()</code>, <code>size()</code>, <code>max_size()</code>
    */
    size_type reserve() const { return m_capacity_ctrl.capacity() - size(); }

    //! Get the capacity of the <code>circular_buffer_space_optimized</code>.
    /*!
        \return The capacity controller representing the maximum number of elements which can be stored in the
                <code>circular_buffer_space_optimized</code> and the minimal allocated size of the internal buffer.
        \throws Nothing.
        \par Exception Safety
             No-throw.
        \par Iterator Invalidation
             Does not invalidate any iterators.
        \par Complexity
             Constant (in the size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>reserve()</code>, <code>size()</code>, <code>max_size()</code>, <code>set_capacity()</code>
    */
    const capacity_type& capacity() const { return m_capacity_ctrl; }

#if defined(BOOST_CB_TEST)

    // Return the current capacity of the adapted circular buffer.
    /*
       \note This method is not intended to be used directly by the user.
             It is defined only for testing purposes.
    */
    size_type internal_capacity() const { return circular_buffer<T, Alloc>::capacity(); }

#endif // #if defined(BOOST_CB_TEST)

    /*! \brief Change the capacity (and the minimal guaranteed amount of allocated memory) of the
               <code>circular_buffer_space_optimized</code>.
        \post <code>capacity() == capacity_ctrl \&\& size() \<= capacity_ctrl.capacity()</code><br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is greater
              than the desired new capacity then number of <code>[size() - capacity_ctrl.capacity()]</code> <b>last</b>
              elements will be removed and the new size will be equal to <code>capacity_ctrl.capacity()</code>.<br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is lower
              than than the new capacity the allocated memory (in the internal buffer) may be accommodated as necessary
              but it will never drop below <code>capacity_ctrl.min_capacity()</code>.
        \param capacity_ctrl The new capacity controller.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Strong.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the size/new capacity of the <code>circular_buffer_space_optimized</code>).
        \note To explicitly clear the extra allocated memory use the <b>shrink-to-fit</b> technique:<br><br>
              <code>boost::%circular_buffer_space_optimized\<int\> cb(1000);<br>
              ...<br>
              boost::%circular_buffer_space_optimized\<int\>(cb).swap(cb);</code><br><br>
              For more information about the shrink-to-fit technique in STL see
              <a href="http://www.gotw.ca/gotw/054.htm">http://www.gotw.ca/gotw/054.htm</a>.
        \sa <code>rset_capacity()</code>, <code>resize()</code>
    */
    void set_capacity(const capacity_type& capacity_ctrl) {
        m_capacity_ctrl = capacity_ctrl;
        if (capacity_ctrl.capacity() < circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(capacity_ctrl.capacity());
        set_min_capacity(capacity_ctrl.min_capacity());
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators_except(end());
#endif
    }

    //! Change the size of the <code>circular_buffer_space_optimized</code>.
    /*!
        \post <code>size() == new_size \&\& capacity().%capacity() >= new_size</code><br><br>
              If the new size is greater than the current size, copies of <code>item</code> will be inserted at the
              <b>back</b> of the of the <code>circular_buffer_space_optimized</code> in order to achieve the desired
              size. In the case the resulting size exceeds the current capacity the capacity will be set to
              <code>new_size</code>.<br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is greater
              than the desired new size then number of <code>[size() - new_size]</code> <b>last</b> elements will be
              removed. (The capacity will remain unchanged.)
        \param new_size The new size.
        \param item The element the <code>circular_buffer_space_optimized</code> will be filled with in order to gain
                    the requested size. (See the <i>Effect</i>.)
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the new size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>rresize()</code>, <code>set_capacity()</code>
    */
    void resize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size()) {
            if (new_size > capacity())
                m_capacity_ctrl.m_capacity = new_size;
            insert(end(), new_size - size(), item);
        } else {
            erase(end() - (size() - new_size), end());
        }
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators_except(end());
#endif
    }

    /*! \brief Change the capacity (and the minimal guaranteed amount of allocated memory) of the
               <code>circular_buffer_space_optimized</code>.
        \post <code>capacity() == capacity_ctrl \&\& size() \<= capacity_ctrl</code><br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is greater
              than the desired new capacity then number of <code>[size() - capacity_ctrl.capacity()]</code>
              <b>first</b> elements will be removed and the new size will be equal to
              <code>capacity_ctrl.capacity()</code>.<br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is lower
              than than the new capacity the allocated memory (in the internal buffer) may be accommodated as necessary
              but it will never drop below <code>capacity_ctrl.min_capacity()</code>.
        \param capacity_ctrl The new capacity controller.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Strong.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the size/new capacity of the <code>circular_buffer_space_optimized</code>).
        \sa <code>set_capacity()</code>, <code>rresize()</code>
    */
    void rset_capacity(const capacity_type& capacity_ctrl) {
        m_capacity_ctrl = capacity_ctrl;
        if (capacity_ctrl.capacity() < circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::rset_capacity(capacity_ctrl.capacity());
        set_min_capacity(capacity_ctrl.min_capacity());
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators_except(end());
#endif
    }

    //! Change the size of the <code>circular_buffer_space_optimized</code>.
    /*!
        \post <code>size() == new_size \&\& capacity().%capacity() >= new_size</code><br><br>
              If the new size is greater than the current size, copies of <code>item</code> will be inserted at the
              <b>front</b> of the of the <code>circular_buffer_space_optimized</code> in order to achieve the desired
              size. In the case the resulting size exceeds the current capacity the capacity will be set to
              <code>new_size</code>.<br><br>
              If the current number of elements stored in the <code>circular_buffer_space_optimized</code> is greater
              than the desired new size then number of <code>[size() - new_size]</code> <b>first</b> elements will be
              removed. (The capacity will remain unchanged.)
        \param new_size The new size.
        \param item The element the <code>circular_buffer_space_optimized</code> will be filled with in order to gain
                    the requested size. (See the <i>Effect</i>.)
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the new size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>rresize()</code>, <code>set_capacity()</code>
    */
    void rresize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size()) {
            if (new_size > capacity())
                m_capacity_ctrl.m_capacity = new_size;
            rinsert(begin(), new_size - size(), item);
        } else {
            rerase(begin(), end() - new_size);
        }
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators_except(end());
#endif
    }

    //! Create an empty space optimized circular buffer with a maximum capacity.
    /*!
        \post <code>capacity().%capacity() == max_size() \&\& capacity().min_capacity() == 0 \&\& size() == 0</code>
              <br><br>There is no memory allocated in the internal buffer after execution of this constructor.
        \param alloc The allocator.
        \throws Nothing.
        \par Complexity
             Constant.
    */
    explicit circular_buffer_space_optimized(
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(0, alloc)
    , m_capacity_ctrl(max_size()) {}

    //! Create an empty space optimized circular buffer with the specified capacity.
    /*!
        \post <code>capacity() == capacity_ctrl \&\& size() == 0</code>
        \param capacity_ctrl The capacity controller representing the maximum number of elements which can be stored in
                             the <code>circular_buffer_space_optimized</code> and the minimal allocated size of the
                             internal buffer.
        \param alloc The allocator.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \par Complexity
             Constant.
    */
    explicit circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(capacity_ctrl.m_min_capacity, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

    /*! \brief Create a full space optimized circular buffer with the specified capacity (and the minimal guaranteed
               amount of allocated memory) filled with <code>capacity_ctrl.%capacity()</code> copies of
               <code>item</code>.
        \post <code>capacity() == capacity_ctrl \&\& full() \&\& (*this)[0] == item \&\& (*this)[1] == item \&\& ...
              \&\& (*this) [capacity_ctrl.%capacity() - 1] == item </code>
        \param capacity_ctrl The capacity controller representing the maximum number of elements which can be stored in
                             the <code>circular_buffer_space_optimized</code> and the minimal allocated size of the
                             internal buffer.
        \param item The element the created <code>circular_buffer_space_optimized</code> will be filled with.
        \param alloc The allocator.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Complexity
             Linear (in the <code>capacity_ctrl.%capacity()</code>).
    */
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(capacity_ctrl.m_capacity, item, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

    /*! \brief Create a space optimized circular buffer with the specified capacity (and the minimal guaranteed amount
               of allocated memory) filled with <code>n</code> copies of <code>item</code>.
        \pre <code>capacity_ctrl.%capacity() >= n</code>
        \post <code>capacity() == capacity_ctrl \&\& size() == n \&\& (*this)[0] == item \&\& (*this)[1] == item
              \&\& ... \&\& (*this)[n - 1] == item</code><br><br>
              Allocates at least as much memory as specified by the <code>capacity_ctrl.min_capacity()</code>.
        \param capacity_ctrl The capacity controller representing the maximum number of elements which can be stored in
                             the <code>circular_buffer_space_optimized</code> and the minimal allocated size of the
                             internal buffer.
        \param n The number of elements the created <code>circular_buffer_space_optimized</code> will be filled with.
        \param item The element the created <code>circular_buffer_space_optimized</code> will be filled with.
        \param alloc The allocator.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Complexity
             Linear (in the <code>n</code>).
    */
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        size_type n,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(init_capacity(capacity_ctrl, n), n, item, alloc)
    , m_capacity_ctrl(capacity_ctrl) {}

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)

    /*! \cond */
    circular_buffer_space_optimized(const circular_buffer_space_optimized<T, Alloc>& cb)
    : circular_buffer<T, Alloc>(cb.begin(), cb.end())
    , m_capacity_ctrl(cb.m_capacity_ctrl) {}

    template <class InputIterator>
    circular_buffer_space_optimized(
        InputIterator first,
        InputIterator last)
    : circular_buffer<T, Alloc>(first, last)
    , m_capacity_ctrl(circular_buffer<T, Alloc>::capacity()) {}

    template <class InputIterator>
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        InputIterator first,
        InputIterator last)
    : circular_buffer<T, Alloc>(
        init_capacity(capacity_ctrl, first, last, is_integral<InputIterator>()),
        first, last)
    , m_capacity_ctrl(capacity_ctrl) {
        check_high_capacity(is_integral<InputIterator>());
    }
    /*! \endcond */

#else

    //! The copy constructor.
    /*!
        Creates a copy of the specified <code>circular_buffer_space_optimized</code>.
        \post <code>*this == cb</code><br><br>
              Allocates the exact amount of memory to store the content of <code>cb</code>.
        \param cb The <code>circular_buffer_space_optimized</code> to be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Complexity
             Linear (in the size of <code>cb</code>).
    */
    circular_buffer_space_optimized(const circular_buffer_space_optimized<T, Alloc>& cb)
    : circular_buffer<T, Alloc>(cb.begin(), cb.end(), cb.get_allocator())
    , m_capacity_ctrl(cb.m_capacity_ctrl) {}

    //! Create a full space optimized circular buffer filled with a copy of the range.
    /*!
        \pre Valid range <code>[first, last)</code>.<br>
             <code>first</code> and <code>last</code> have to meet the requirements of
             <a href="http://www.sgi.com/tech/stl/InputIterator.html">InputIterator</a>.
        \post <code>capacity().%capacity() == std::distance(first, last) \&\& capacity().min_capacity() == 0 \&\&
              full() \&\& (*this)[0]== *first \&\& (*this)[1] == *(first + 1) \&\& ... \&\&
              (*this)[std::distance(first, last) - 1] == *(last - 1)</code>
        \param first The beginning of the range to be copied.
        \param last The end of the range to be copied.
        \param alloc The allocator.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Complexity
             Linear (in the <code>std::distance(first, last)</code>).
    */
    template <class InputIterator>
    circular_buffer_space_optimized(
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(first, last, alloc)
    , m_capacity_ctrl(circular_buffer<T, Alloc>::capacity()) {}

    /*! \brief Create a space optimized circular buffer with the specified capacity (and the minimal guaranteed amount
               of allocated memory) filled with a copy of the range.
        \pre Valid range <code>[first, last)</code>.<br>
             <code>first</code> and <code>last</code> have to meet the requirements of
             <a href="http://www.sgi.com/tech/stl/InputIterator.html">InputIterator</a>.
        \post <code>capacity() == capacity_ctrl \&\& size() \<= std::distance(first, last) \&\& (*this)[0]==
              *(last - capacity_ctrl.%capacity()) \&\& (*this)[1] == *(last - capacity_ctrl.%capacity() + 1) \&\& ...
              \&\& (*this)[capacity_ctrl.%capacity() - 1] == *(last - 1)</code><br><br>
              Allocates at least as much memory as specified by the <code>capacity_ctrl.min_capacity()</code>.<br><br>
              If the number of items to be copied from the range <code>[first, last)</code> is greater than the
              specified <code>capacity_ctrl.%capacity()</code> then only elements from the range
              <code>[last - capacity_ctrl.%capacity(), last)</code> will be copied.
        \param capacity_ctrl The capacity controller representing the maximum number of elements which can be stored in
                             the <code>circular_buffer_space_optimized</code> and the minimal allocated size of the
                             internal buffer.
        \param first The beginning of the range to be copied.
        \param last The end of the range to be copied.
        \param alloc The allocator.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Complexity
             Linear (in the <code>capacity_ctrl.%capacity()</code>/<code>std::distance(first, last)</code>).
    */
    template <class InputIterator>
    circular_buffer_space_optimized(
        capacity_type capacity_ctrl,
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : circular_buffer<T, Alloc>(
        init_capacity(capacity_ctrl, first, last, is_integral<InputIterator>()),
        first, last, alloc)
    , m_capacity_ctrl(capacity_ctrl) {
        check_high_capacity(is_integral<InputIterator>());
    }

#endif // #if BOOST_WORKAROUND(BOOST_MSVC, < 1300)

#if defined(BOOST_CB_NEVER_DEFINED)
// This section will never be compiled - the default destructor and assignment operator will be generated instead.
// Declared only for documentation purpose.

    //! The destructor.
    /*!
        Destroys the <code>circular_buffer_space_optimized</code>.
        \throws Nothing.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (including
             iterators equal to end()).
        \par Complexity
             Linear (in the size of the <code>circular_buffer_space_optimized</code>).
        \sa <code>clear()</code>
    */
    ~circular_buffer_space_optimized();

    //! The assign operator.
    /*!
        Makes this <code>circular_buffer_space_optimized</code> to become a copy of the specified
        <code>circular_buffer_space_optimized</code>.
        \post <code>*this == cb</code>
        \param cb The <code>circular_buffer_space_optimized</code> to be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Strong.
        \par Iterator Invalidation
             Invalidates all iterators pointing to this <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the size of <code>cb</code>).
        \sa <code>\link assign(size_type, param_value_type) assign(size_type, const_reference)\endlink</code>,
            <code>\link assign(capacity_type, size_type, param_value_type)
            assign(capacity_type, size_type, const_reference)\endlink</code>,
            <code>assign(InputIterator, InputIterator)</code>,
            <code>assign(capacity_type, InputIterator, InputIterator)</code>
    */
    circular_buffer_space_optimized<T, Alloc>& operator = (const circular_buffer_space_optimized<T, Alloc>& cb);

#endif // #if defined(BOOST_CB_NEVER_DEFINED)

    //! Assign <code>n</code> items into the space optimized circular buffer.
    /*!
        The content of the <code>circular_buffer_space_optimized</code> will be removed and replaced with
        <code>n</code> copies of the <code>item</code>.
        \post <code>capacity().%capacity() == n \&\& capacity().min_capacity() == 0 \&\& size() == n \&\& (*this)[0] ==
              item \&\& (*this)[1] == item \&\& ... \&\& (*this) [n - 1] == item</code>
        \param n The number of elements the <code>circular_buffer_space_optimized</code> will be filled with.
        \param item The element the <code>circular_buffer_space_optimized</code> will be filled with.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the <code>n</code>).
        \sa <code>operator=</code>, <code>\link assign(capacity_type, size_type, param_value_type)
            assign(capacity_type, size_type, const_reference)\endlink</code>,
            <code>assign(InputIterator, InputIterator)</code>,
            <code>assign(capacity_type, InputIterator, InputIterator)</code>
    */
    void assign(size_type n, param_value_type item) {
        circular_buffer<T, Alloc>::assign(n, item);
        m_capacity_ctrl.m_capacity = n;
        m_capacity_ctrl.m_min_capacity = 0;
    }

    //! Assign <code>n</code> items into the space optimized circular buffer specifying the capacity.
    /*!
        The capacity of the <code>circular_buffer_space_optimized</code> will be set to the specified value and the
        content of the <code>circular_buffer_space_optimized</code> will be removed and replaced with <code>n</code>
        copies of the <code>item</code>.
        \pre <code>capacity_ctrl.%capacity() >= n</code>
        \post <code>capacity() == capacity_ctrl \&\& size() == n \&\& (*this)[0] == item \&\& (*this)[1] == item
              \&\& ... \&\& (*this) [n - 1] == item </code><br><br>
              The amount of allocated memory will be <code>max[n, capacity_ctrl.min_capacity()]</code>.
        \param capacity_ctrl The new capacity controller.
        \param n The number of elements the <code>circular_buffer_space_optimized</code> will be filled with.
        \param item The element the <code>circular_buffer_space_optimized</code> will be filled with.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the <code>n</code>).
        \sa <code>operator=</code>, <code>\link assign(size_type, param_value_type)
            assign(size_type, const_reference)\endlink</code>, <code>assign(InputIterator, InputIterator)</code>,
            <code>assign(capacity_type, InputIterator, InputIterator)</code>
    */
    void assign(capacity_type capacity_ctrl, size_type n, param_value_type item) {
       BOOST_CB_ASSERT(capacity_ctrl.m_capacity >= n); // check for new capacity lower than n
       circular_buffer<T, Alloc>::assign(std::max(capacity_ctrl.m_min_capacity, n), n, item);
       m_capacity_ctrl = capacity_ctrl;
    }

    //! Assign a copy of the range into the space optimized circular buffer.
    /*!
        The content of the <code>circular_buffer_space_optimized</code> will be removed and replaced with copies of
        elements from the specified range.
        \pre Valid range <code>[first, last)</code>.<br>
             <code>first</code> and <code>last</code> have to meet the requirements of
             <a href="http://www.sgi.com/tech/stl/InputIterator.html">InputIterator</a>.
        \post <code>capacity().%capacity() == std::distance(first, last) \&\& capacity().min_capacity() == 0 \&\&
              size() == std::distance(first, last) \&\& (*this)[0]== *first \&\& (*this)[1] == *(first + 1) \&\& ...
              \&\& (*this)[std::distance(first, last) - 1] == *(last - 1)</code>
        \param first The beginning of the range to be copied.
        \param last The end of the range to be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the <code>std::distance(first, last)</code>).
        \sa <code>operator=</code>, <code>\link assign(size_type, param_value_type)
            assign(size_type, const_reference)\endlink</code>,
            <code>\link assign(capacity_type, size_type, param_value_type)
            assign(capacity_type, size_type, const_reference)\endlink</code>,
            <code>assign(capacity_type, InputIterator, InputIterator)</code>
    */
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        circular_buffer<T, Alloc>::assign(first, last);
        m_capacity_ctrl.m_capacity = circular_buffer<T, Alloc>::capacity();
        m_capacity_ctrl.m_min_capacity = 0;
    }

    //! Assign a copy of the range into the space optimized circular buffer specifying the capacity.
    /*!
        The capacity of the <code>circular_buffer_space_optimized</code> will be set to the specified value and the
        content of the <code>circular_buffer_space_optimized</code> will be removed and replaced with copies of
        elements from the specified range.
        \pre Valid range <code>[first, last)</code>.<br>
             <code>first</code> and <code>last</code> have to meet the requirements of
             <a href="http://www.sgi.com/tech/stl/InputIterator.html">InputIterator</a>.
        \post <code>capacity() == capacity_ctrl \&\& size() \<= std::distance(first, last) \&\&
             (*this)[0]== *(last - capacity) \&\& (*this)[1] == *(last - capacity + 1) \&\& ... \&\&
             (*this)[capacity - 1] == *(last - 1)</code><br><br>
             If the number of items to be copied from the range <code>[first, last)</code> is greater than the
             specified <code>capacity</code> then only elements from the range <code>[last - capacity, last)</code>
             will be copied.<br><br> The amount of allocated memory will be
             <code>max[std::distance(first, last), capacity_ctrl.min_capacity()]</code>.
        \param capacity_ctrl The new capacity controller.
        \param first The beginning of the range to be copied.
        \param last The end of the range to be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is
                used).
        \throws Whatever <code>T::T(const T&)</code> throws.
        \par Exception Safety
             Basic.
        \par Iterator Invalidation
             Invalidates all iterators pointing to the <code>circular_buffer_space_optimized</code> (except iterators
             equal to end()).
        \par Complexity
             Linear (in the <code>std::distance(first, last)</code>).
        \sa <code>operator=</code>, <code>\link assign(size_type, param_value_type)
            assign(size_type, const_reference)\endlink</code>,
            <code>\link assign(capacity_type, size_type, param_value_type)
            assign(capacity_type, size_type, const_reference)\endlink</code>,
            <code>assign(InputIterator, InputIterator)</code>
    */
    template <class InputIterator>
    void assign(capacity_type capacity_ctrl, InputIterator first, InputIterator last) {
       m_capacity_ctrl = capacity_ctrl;
       circular_buffer<T, Alloc>::assign(capacity(), first, last);
       check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    void swap(circular_buffer_space_optimized<T, Alloc>& cb) {
        std::swap(m_capacity_ctrl, cb.m_capacity_ctrl);
        circular_buffer<T, Alloc>::swap(cb);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_back(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_back(item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void push_front(param_value_type item = value_type()) {
        check_low_capacity();
        circular_buffer<T, Alloc>::push_front(item);
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_back() {
        circular_buffer<T, Alloc>::pop_back();
        check_high_capacity();
    }

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    void pop_front() {
        circular_buffer<T, Alloc>::pop_front();
        check_high_capacity();
    }

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        insert(pos, first, last, is_integral<InputIterator>());
    }

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
    /*!
         \warning The rules for iterator invalidation differ from the original
                  circular_buffer. See the <a href="../circular_buffer_adaptor.html#invalidation">
                  documentation</a>.
    */
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last) {
        rinsert(pos, first, last, is_integral<InputIterator>());
    }

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
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

    //! See the circular_buffer source documentation.
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

    //! Change the minimal guaranteed amount of allocated memory.
    void set_min_capacity(size_type new_min_capacity) {
        if (new_min_capacity > circular_buffer<T, Alloc>::capacity())
            circular_buffer<T, Alloc>::set_capacity(new_min_capacity);
        else
            check_high_capacity();
    }

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
            if (new_capacity <= m_capacity_ctrl.m_min_capacity) {
                new_capacity = m_capacity_ctrl.m_min_capacity;
                break;
            }
        }
        circular_buffer<T, Alloc>::set_capacity(
            ensure_reserve(new_capacity, size()));
    }

    //! Specialized method for checking of the high capacity.
    void check_high_capacity(const true_type&) {}

    //! Specialized method for checking of the high capacity.
    void check_high_capacity(const false_type&) {
        check_high_capacity();
    }

    //! Determine the initial capacity.
    static size_type init_capacity(const capacity_type& capacity_ctrl, size_type n) {
        BOOST_CB_ASSERT(capacity_ctrl.m_capacity >= n); // check for capacity lower than n
        return std::max(capacity_ctrl.m_min_capacity, n);
    }

    //! Specialized method for determining the initial capacity.
    template <class IntegralType>
    static size_type init_capacity(const capacity_type& capacity_ctrl, IntegralType n, IntegralType item, const true_type&) {
        return init_capacity(capacity_ctrl, static_cast<size_type>(n));
    }

    //! Specialized method for determining the initial capacity.
    template <class Iterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        return init_capacity(capacity_ctrl, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }

    //! Specialized method for determining the initial capacity.
    template <class InputIterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        return capacity_ctrl.m_capacity;
    }

    //! Specialized method for determining the initial capacity.
    template <class ForwardIterator>
    static size_type init_capacity(const capacity_type& capacity_ctrl, ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        return std::min(capacity_ctrl.m_capacity, std::max(capacity_ctrl.m_min_capacity, static_cast<size_type>(std::distance(first, last))));
    }

    //! Specialized insert method.
    template <class IntegralType>
    void insert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        insert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized insert method.
    template <class Iterator>
    void insert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
        size_type index = pos - begin();
        check_low_capacity(std::distance(first, last));
        circular_buffer<T, Alloc>::insert(begin() + index, first, last);
    }

    //! Specialized rinsert method.
    template <class IntegralType>
    void rinsert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        rinsert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized rinsert method.
    template <class Iterator>
    void rinsert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
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

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_SPACE_OPTIMIZED_HPP)
