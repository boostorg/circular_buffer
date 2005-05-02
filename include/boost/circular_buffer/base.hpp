// Implementation of the base circular buffer.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_BASE_HPP)
#define BOOST_CIRCULAR_BUFFER_BASE_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include <boost/call_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/throw_exception.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <algorithm>
#if !defined(BOOST_NO_EXCEPTIONS)
    #include <stdexcept>
#endif
#if BOOST_CB_ENABLE_DEBUG
    #include <cstring>
#endif

namespace boost {

/*!
    \class circular_buffer
    \brief Circular buffer - a STL compliant container.
    \param T The type of the elements stored in the circular buffer.
    \param Alloc The allocator type used for all internal memory management.
           Default: std::allocator<T>
    \author <a href="mailto:jano_gaspar[at]yahoo.com">Jan Gaspar</a>
    \version 3.7
    \date 2005

    For more information how to use the circular buffer see the
    <a href="../circular_buffer.html">documentation</a>.
*/
template <class T, class Alloc>
class circular_buffer : cb_details::cb_iterator_registry {

// Requirements
    BOOST_CLASS_REQUIRE(T, boost, SGIAssignableConcept);

public:
// Basic types

    //! The type of the elements stored in the circular buffer.
    typedef typename Alloc::value_type value_type;

    //! Pointer to the element.
    typedef typename Alloc::pointer pointer;

    //! Const pointer to the element.
    typedef typename Alloc::const_pointer const_pointer;

    //! Reference to the element.
    typedef typename Alloc::reference reference;

    //! Const reference to the element.
    typedef typename Alloc::const_reference const_reference;

    //! Distance type.
    /*!
        A signed integral type used to represent the distance between two iterators.
    */
    typedef typename Alloc::difference_type difference_type;

    //! Size type.
    /*!
        An unsigned integral type that can represent any nonnegative value of the container's distance type.
    */
    typedef typename Alloc::size_type size_type;

    //! The type of the allocator used in the circular buffer.
    typedef Alloc allocator_type;

    //! Return the allocator.
    allocator_type get_allocator() const { return m_alloc; }

    //! Return the allocator.
    /*!
        \note This method was added in order to optimize obtaining of the allocator with a state,
              although use of stateful allocators in STL is discouraged.
    */
    allocator_type& get_allocator() { return m_alloc; }

// Helper types

    // Define a type that represents the "best" way to pass the value_type to a method.
    typedef typename call_traits<value_type>::param_type param_value_type;

    // Define a type that represents the "best" way to return the value_type from a const method.
    typedef typename call_traits<value_type>::param_type return_value_type;

// Iterators

    //! Const (random access) iterator used to iterate through a circular buffer.
    typedef cb_details::cb_iterator< circular_buffer<T, Alloc>, cb_details::cb_const_traits<Alloc> > const_iterator;

    //! Iterator (random access) used to iterate through a circular buffer.
    typedef cb_details::cb_iterator< circular_buffer<T, Alloc>, cb_details::cb_nonconst_traits<Alloc> > iterator;

    //! Const iterator used to iterate backwards through a circular buffer.
    typedef reverse_iterator<const_iterator> const_reverse_iterator;

    //! Iterator used to iterate backwards through a circular buffer.
    typedef reverse_iterator<iterator> reverse_iterator;

private:
// Member variables

    //! The internal buffer used for storing elements in the circular buffer.
    pointer m_buff;

    //! The internal buffer's end (end of the storage space).
    pointer m_end;

    //! The virtual beginning of the circular buffer (the leftmost element).
    pointer m_first;

    //! The virtual end of the circular buffer (one behind the rightmost element).
    pointer m_last;

    //! The number of items currently stored in the circular buffer.
    size_type m_size;

    //! The allocator.
    allocator_type m_alloc;

// Friends
#if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    friend iterator;
    friend const_iterator;
#else
    template <class Buff, class Traits> friend struct cb_details::cb_iterator;
#endif

public:
// Element access

    //! Return an iterator pointing to the beginning of the circular buffer.
    iterator begin() { return iterator(this, empty() ? 0 : m_first); }

    //! Return an iterator pointing to the end of the circular buffer.
    iterator end() { return iterator(this, 0); }

    //! Return a const iterator pointing to the beginning of the circular buffer.
    const_iterator begin() const { return const_iterator(this, empty() ? 0 : m_first); }

    //! Return a const iterator pointing to the end of the circular buffer.
    const_iterator end() const { return const_iterator(this, 0); }

    //! Return a reverse iterator pointing to the beginning of the reversed circular buffer.
    reverse_iterator rbegin() { return reverse_iterator(end()); }

    //! Return a reverse iterator pointing to the end of the reversed circular buffer.
    reverse_iterator rend() { return reverse_iterator(begin()); }

    //! Return a const reverse iterator pointing to the beginning of the reversed circular buffer.
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    //! Return a const reverse iterator pointing to the end of the reversed circular buffer.
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    //! Return the element at the <code>index</code> position.
    /*!
        \pre <code>*(this).size() > index</code>
    */
    reference operator [] (size_type index) {
        BOOST_CB_ASSERT(index < size()); // check for invalid index
        return *add(m_first, index);
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \pre <code>*(this).size() > index</code>
    */
    return_value_type operator [] (size_type index) const {
        BOOST_CB_ASSERT(index < size()); // check for invalid index
        return *add(m_first, index);
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \throws std::out_of_range thrown when the <code>index</code> is invalid.
    */
    reference at(size_type index) {
        check_position(index);
        return (*this)[index];
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \throws std::out_of_range thrown when the <code>index</code> is invalid.
    */
    return_value_type at(size_type index) const {
        check_position(index);
        return (*this)[index];
    }

    //! Return the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    reference front() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        return *m_first;
    }

    //! Return the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    reference back() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        return *((m_last == m_buff ? m_end : m_last) - 1);
    }

    //! Return the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    return_value_type front() const {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        return *m_first;
    }

    //! Return the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    return_value_type back() const {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        return *((m_last == m_buff ? m_end : m_last) - 1);
    }

    //! Return pointer to data stored in the circular buffer as a continuous array of values.
    /*!
        This method can be useful e.g. when passing the stored data into the legacy C API.
        \post <code>\&(*this)[0] \< \&(*this)[1] \< ... \< \&(*this).back()</code>
        \return 0 if empty.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    pointer data() {
        if (empty())
            return 0;
        if (m_first < m_last || m_last == m_buff)
            return m_first;
        size_type constructed = 0;
        pointer src = m_first;
        pointer dest = m_buff;
        pointer tmp = 0;
        BOOST_CB_TRY
        tmp = allocate(1);
        for (pointer first = m_first; dest < src; src = first) {
            for (size_type ii = 0; src < m_end; ++src, ++dest, ++ii) {
                if (dest == first) {
                    first += ii;
                    break;
                }
                if (is_uninitialized(dest)) {
                    m_alloc.construct(dest, *src);
                    ++constructed;
                } else {
                    m_alloc.construct(tmp, *src);
                    BOOST_CB_TRY
                    replace(src, *dest);
                    BOOST_CB_UNWIND(
                        destroy_item(tmp);
                        tidy(src);
                    )
                    BOOST_CB_TRY
                    replace(dest, *tmp);
                    BOOST_CB_UNWIND(
                        destroy_item(tmp);
                        tidy(dest);
                    )
                    destroy_item(tmp);
                }
            }
        }
        deallocate(tmp, 1);
        BOOST_CB_UNWIND(
            deallocate(tmp, 1);
            m_last += constructed;
            m_size += constructed;
        )
        for (dest = m_buff + size(); dest < m_end; ++dest)
            destroy_item(dest);
        m_first = m_buff;
        m_last = add(m_buff, size());
        return m_buff;
    }

// Size and capacity

    //! Return the number of elements currently stored in the circular buffer.
    size_type size() const { return m_size; }

    //! Return the largest possible size (or capacity) of the circular buffer.
    size_type max_size() const { return m_alloc.max_size(); }

    //! Is the circular buffer empty?
    /*!
        \return <code>true</code> if there are no elements stored in the circular buffer.
        \return <code>false</code> otherwise.
    */
    bool empty() const { return size() == 0; }

    //! Is the circular buffer full?
    /*!
        \return <code>true</code> if the number of elements stored in the circular buffer
                equals the capacity of the circular buffer.
        \return <code>false</code> otherwise.
    */
    bool full() const { return size() == capacity(); }

    //! Return the capacity of the circular buffer.
    size_type capacity() const { return m_end - m_buff; }

    //! Change the capacity of the circular buffer.
    /*!
        \param new_capacity The new capacity.
        \param remove_front This parameter plays its role only if the
               current number of elements stored in the circular buffer
               is greater than the desired new capacity. If set to
               <code>true</code> then the first (leftmost) elements
               will be removed. If set to <code>false</code> then the
               last (leftmost) elements will be removed.
        \post <code>(*this).capacity() == new_capacity</code><br>
              If the current number of elements stored in the circular
              buffer is greater than the desired new capacity then
              <code>((*this).size() - new_capacity)</code> elements
              will be removed according to the <code>remove_front</code>
              parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void set_capacity(size_type new_capacity, bool remove_front = true) {
        if (new_capacity == capacity())
            return;
        pointer buff = allocate(new_capacity);
        size_type new_size = std::min(new_capacity, size());
        BOOST_CB_TRY
        if (remove_front)
            cb_details::uninitialized_copy(end() - new_size, end(), buff, m_alloc);
        else
            cb_details::uninitialized_copy(begin(), begin() + new_size, buff, m_alloc);
        BOOST_CB_UNWIND(deallocate(buff, new_capacity))
        destroy();
        m_size = new_size;
        m_buff = m_first = buff;
        m_end = m_buff + new_capacity;
        m_last = add(m_buff, size());
    }

    //! Change the size of the circular buffer.
    /*!
        \param new_size The new size.
        \param item See the postcondition.
        \param remove_front This parameter plays its role only if the
               current number of elements stored in the circular buffer
               is greater than the desired new capacity. If set to
               <code>true</code> then the first (leftmost) elements
               will be removed. If set to <code>false</code> then the
               last (leftmost) elements will be removed.
        \post <code>(*this).size() == new_size</code><br>
              If the new size is greater than the current size, the rest
              of the circular buffer is filled with copies of <code>item</code>.
              In case the resulting size exceeds the current capacity
              the capacity is set to <code>new_size</code>.
              If the new size is lower than the current size then
              <code>((*this).size() - new_size)</code> elements will be removed
              according to the <code>remove_front</code> parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void resize(size_type new_size, param_value_type item = T(), bool remove_front = true) {
        if (new_size > size()) {
            if (new_size > capacity())
                set_capacity(new_size);
            insert(end(), new_size - size(), item);
        } else {
            if (remove_front)
                erase(begin(), end() - new_size);
            else
                erase(begin() + new_size, end());
        }
    }

// Construction/Destruction

    //! Create an empty circular buffer with a given capacity.
    /*!
        \post <code>(*this).capacity() == capacity \&\& (*this).size == 0</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
    */
    explicit circular_buffer(
        size_type capacity,
        const allocator_type& alloc = allocator_type())
    : m_size(0), m_alloc(alloc) {
        m_first = m_last = m_buff = allocate(capacity);
        m_end = m_buff + capacity;
    }

    //! Create a full circular buffer with a given capacity and filled with copies of <code>item</code>.
    /*!
        \post <code>(*this).size() == capacity \&\& (*this)[0] == (*this)[1] == ... == (*this).back() == item</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    circular_buffer(
        size_type capacity,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : m_size(capacity), m_alloc(alloc) {
        m_first = m_last = m_buff = allocate(capacity);
        m_end = m_buff + capacity;
        BOOST_CB_TRY
        cb_details::uninitialized_fill_n(m_buff, size(), item, m_alloc);
        BOOST_CB_UNWIND(deallocate(m_buff, capacity))
    }

    //! Copy constructor.
    /*!
        \post <code>*this == cb</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    circular_buffer(const circular_buffer<T, Alloc>& cb)
    : m_size(cb.size()), m_alloc(cb.get_allocator()) {
        m_first = m_last = m_buff = allocate(cb.capacity());
        BOOST_CB_TRY
        m_end = cb_details::uninitialized_copy(cb.begin(), cb.end(), m_buff, m_alloc);
        BOOST_CB_UNWIND(deallocate(m_buff, cb.capacity()))
    }

    //! Create a circular buffer with a copy of a range.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \post <code>(*this).capacity() == capacity</code><br>
              If the number of items to copy from the range
              <code>[first, last)</code> is greater than the specified
              <code>capacity</code> then only elements from the range
              <code>[last - capacity, last)</code> will be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    template <class InputIterator>
    circular_buffer(
        size_type capacity,
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : m_alloc(alloc) {
        BOOST_CB_IS_CONVERTIBLE(InputIterator, value_type);
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        m_first = m_buff = allocate(capacity);
        m_end = m_buff + capacity;
        size_type diff = std::distance(first, last);
        if (diff > capacity) {
            std::advance(first, diff - capacity);
            m_size = capacity;
            m_last = m_buff;
        } else {
            m_size = diff;
            if (diff == capacity)
                m_last = m_buff;
            else
                m_last = m_buff + size();
        }
        BOOST_CB_TRY
        cb_details::uninitialized_copy(first, last, m_buff, m_alloc);
        BOOST_CB_UNWIND(deallocate(m_buff, capacity))
    }

    //! Destructor.
    ~circular_buffer() { destroy(); }

private:
// Helper functors

    // Functor for assigning n items.
    struct assign_n {
        size_type m_n;
        param_value_type m_item;
        allocator_type& m_alloc;
        explicit assign_n(size_type n, param_value_type item, allocator_type& alloc) : m_n(n), m_item(item), m_alloc(alloc) {}
        void operator () (pointer p) const {
            cb_details::uninitialized_fill_n(p, m_n, m_item, m_alloc);
        }
    private:
        assign_n& operator = (const assign_n&); // do not generate
    };

    // Functor for assigning range of items.
    template <class InputIterator>
    struct assign_range {
        InputIterator m_first;
        InputIterator m_last;
        allocator_type& m_alloc;
        explicit assign_range(InputIterator first, InputIterator last, allocator_type& alloc) : m_first(first), m_last(last), m_alloc(alloc) {}
        void operator () (pointer p) const {
            cb_details::uninitialized_copy(m_first, m_last, p, m_alloc);
        }
    private:
        assign_range& operator = (const assign_range&); // do not generate
    };

public:
// Assign methods

    //! Assignment operator.
    /*!
        \post <code>*this == cb</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    circular_buffer<T, Alloc>& operator = (const circular_buffer<T, Alloc>& cb) {
        if (this == &cb)
            return *this;
        pointer buff = allocate(cb.capacity());
        BOOST_CB_TRY
        pointer last = cb_details::uninitialized_copy(cb.begin(), cb.end(), buff, m_alloc);
        destroy();
        m_size = cb.size();
        m_first = m_buff = buff;
        m_end = m_buff + cb.capacity();
        m_last = full() ? m_buff : last;
        BOOST_CB_UNWIND(deallocate(buff, cb.capacity()))
        return *this;
    }

    //! Assign <code>n</code> items into the circular buffer.
    /*!
        \post <code>(*this).size() == n \&\&
              (*this)[0] == (*this)[1] == ... == (*this).back() == item</code><br>
              If the number of items to be assigned exceeds
              the capacity of the circular buffer the capacity
              is increased to <code>n</code> otherwise it stays unchanged.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void assign(size_type n, param_value_type item) { do_assign(n, assign_n(n, item, m_alloc)); }

    //! Assign a copy of range.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \post <code>(*this).size() == std::distance(first, last)</code><br>
              If the number of items to be assigned exceeds
              the capacity of the circular buffer the capacity
              is set to that number otherwise is stays unchanged.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        assign(first, last, BOOST_DEDUCED_TYPENAME cb_details::cb_iterator_category_traits<InputIterator>::tag());
    }

    //! Swap the contents of two circular buffers.
    /*!
        \post <code>this</code> contains elements of <code>cb</code> and vice versa.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void swap(circular_buffer<T, Alloc>& cb) {
        std::swap(m_alloc, cb.m_alloc); // in general this is not necessary,
                                        // because allocators should not have state
        std::swap(m_buff, cb.m_buff);
        std::swap(m_end, cb.m_end);
        std::swap(m_first, cb.m_first);
        std::swap(m_last, cb.m_last);
        std::swap(m_size, cb.m_size);
#if BOOST_CB_ENABLE_DEBUG
        invalidate_all_iterators();
        cb.invalidate_all_iterators();
#endif
    }

// push and pop

    //! Insert a new element at the end.
    /*!
        \post <code>(*this).back() == item</code><br>
              If the circular buffer is full, the first (leftmost) element will be removed.
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void push_back(param_value_type item = value_type()) {
        if (full()) {
            if (empty())
                return;
            replace_last(item);
            increment(m_last);
            m_first = m_last;
        } else {
            m_alloc.construct(m_last, item);
            increment(m_last);
            ++m_size;
        }
    }

    //! Insert a new element at the start.
    /*!
        \post <code>(*this).front() == item</code><br>
              If the circular buffer is full, the last (rightmost) element will be removed.
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void push_front(param_value_type item = value_type()) {
        if (full()) {
            if (empty())
                return;
            replace_first(item);
            m_last = m_first;
        } else {
            decrement(m_first);
            BOOST_CB_TRY
            m_alloc.construct(m_first, item);
            BOOST_CB_UNWIND(increment(m_first))
            ++m_size;
        }
    }

    //! Remove the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
        \pre <code>iterator it = ((*this).end() - 1)</code>
        \post <code>((*this).end() - 1) != it</code>
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void pop_back() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        decrement(m_last);
        destroy_item(m_last);
        --m_size;
    }

    //! Remove the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
        \pre <code>iterator it = (*this).begin()</code>
        \post <code>(*this).begin() != it</code>
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void pop_front() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        destroy_item(m_first);
        increment(m_first);
        --m_size;
    }

private:
// Helper wrappers

    // Iterator dereference wrapper.
    template <class InputIterator>
    struct iterator_wrapper {
        mutable InputIterator m_it;
        explicit iterator_wrapper(InputIterator it) : m_it(it) {}
        InputIterator get_reference() const { return m_it++; }
    };

    // Item dereference wrapper.
    struct item_wrapper {
        const_pointer m_item;
        explicit item_wrapper(param_value_type item) : m_item(&item) {}
        const_pointer get_reference() const { return m_item; }
    };

public:
// Insert

    //! Insert the <code>item</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post The <code>item</code> will be inserted at the position <code>pos</code>.<br>
              If the circular buffer is full, the first (leftmost) element will be removed.
        \return iterator to the inserted element.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator insert(iterator pos, param_value_type item = value_type()) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (full() && pos == begin())
            return begin();
        if (pos.m_it == 0) {
            if (full())
                replace_last(item);
            else
                m_alloc.construct(m_last, item);
            pos.m_it = m_last;
        } else {
            pointer src = m_last;
            pointer dest = m_last;
            BOOST_CB_TRY
            while (src != pos.m_it) {
                decrement(src);
                if (dest == m_last && !full())
                    m_alloc.construct(dest, *src);
                else
                    replace(dest, *src);
                decrement(dest);
            }
            replace(pos.m_it, item);
            BOOST_CB_UNWIND(
                if (dest == m_last) {
                    if (full()) {
                        increment(m_first);
                        --m_size;
                    }
                } else {
                    if (!full()) {
                        increment(m_last);
                        ++m_size;
                    }
                    tidy(dest);
                }
            )
        }
        increment(m_last);
        if (full())
            m_first = m_last;
        else
            ++m_size;
        return iterator(this, pos.m_it);
    }

    //! Insert <code>n</code> copies of the item before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the beginning (left) of the circular buffer will be removed
              or not all <code>n</code> elements will be inserted or both.<code><br>
              Example:<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, (size_t)5, 6);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|6|6|6|6|6|3|4|)<br>
                RESULTING circular buffer |6|6|6|6|3|4| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void insert(iterator pos, size_type n, param_value_type item) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (n == 0)
            return;
        size_type copy = capacity() - (end() - pos);
        if (copy == 0)
            return;
        if (n > copy)
            n = copy;
        insert_n_item(pos, n, item_wrapper(item));
    }

    //! Insert the range <code>[first, last)</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator and valid range <code>[first, last)</code>.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the beginning (left) of the circular buffer will be removed
              or not the whole range will be inserted or both.
              In case the whole range cannot be inserted it will be inserted just
              some elements from the end (right) of the range (see the example).<code><br>
              Example:<br>
                array to insert: int array[] = { 5, 6, 7, 8, 9 };<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, array, array + 5);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|5|6|7|8|9|3|4|)<br>
                RESULTING circular buffer |6|7|8|9|3|4| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        insert(pos, first, last, BOOST_DEDUCED_TYPENAME cb_details::cb_iterator_category_traits<InputIterator>::tag());
    }

    //! Insert an <code>item</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post The <code>item</code> will be inserted at the position <code>pos</code>.<br>
              If the circular buffer is full, the last element (rightmost) will be removed.
        \return iterator to the inserted element.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rinsert(iterator pos, param_value_type item = value_type()) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (full() && pos == end())
            return end();
        if (pos == begin()) {
            if (full()) {
                replace_first(item);
            } else {
                decrement(m_first);
                BOOST_CB_TRY
                m_alloc.construct(m_first, item);
                BOOST_CB_UNWIND(increment(m_first))
            }
        } else {
            pointer src = m_first;
            pointer dest = m_first;
            decrement(dest);
            pointer it = map_pointer(pos.m_it);
            pointer first = m_first;
            decrement(first);
            BOOST_CB_TRY
            while (src != it) {
                if (dest == first && !full())
                    m_alloc.construct(dest, *src);
                else
                    replace(dest, *src);
                increment(src);
                increment(dest);
            }
            replace((--pos).m_it, item);
            BOOST_CB_UNWIND(
                if (dest == first) {
                    if (full()) {
                        decrement(m_last);
                        --m_size;
                    }
                } else {
                    if (!full()) {
                        m_first = first;
                        ++m_size;
                    }
                    tidy(dest);
                }
            )
            decrement(m_first);
        }
        if (full())
            m_last = m_first;
        else
            ++m_size;
        return iterator(this, pos.m_it);
    }

    //! Insert <code>n</code> copies of the item before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the end (right) of the circular buffer will be removed
              or not all <code>n</code> elements will be inserted or both.<code><br>
              Example:<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, (size_t)5, 6);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|6|6|6|6|6|3|4|)<br>
                RESULTING circular buffer |1|2|6|6|6|6| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void rinsert(iterator pos, size_type n, param_value_type item) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        rinsert_n_item(pos, n, item_wrapper(item));
    }

    //! Insert the range <code>[first, last)</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator and valid range <code>[first, last)</code>.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the end (right) of the circular buffer will be removed
              or not the whole range will be inserted or both.
              In case the whole range cannot be inserted it will be inserted just
              some elements from the beginning (left) of the range (see the example).<code><br>
              Example:<br>
                array to insert: int array[] = { 5, 6, 7, 8, 9 };<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, array, array + 5);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|5|6|7|8|9|3|4|)<br>
                RESULTING circular buffer |1|2|5|6|7|8| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        rinsert(pos, first, last, BOOST_DEDUCED_TYPENAME cb_details::cb_iterator_category_traits<InputIterator>::tag());
    }

// Erase

    //! Erase the element at the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - 1</code><br>
              Removes an element at the position <code>pos</code>.
        \return iterator to the first element remaining beyond the removed
                element or <code>(*this).end()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator erase(iterator pos) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(pos.m_it != 0);  // check for iterator pointing to end()
        pointer next = pos.m_it;
        increment(next);
        for (pointer p = pos.m_it; next != m_last; p = next, increment(next))
            replace(p, *next);
        decrement(m_last);
        destroy_item(m_last);
        --m_size;
#if BOOST_CB_ENABLE_DEBUG
        return m_last == pos.m_it ? end() : iterator(this, pos.m_it);
#else
        return m_last == pos.m_it ? end() : pos;
#endif
    }

    //! Erase the range <code>[first, last)</code>.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - std::distance(first, last)</code><br>
              Removes the elements from the range <code>[first, last)</code>.
        \return iterator to the first element remaining beyond the removed
                element or <code>(*this).end()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator erase(iterator first, iterator last) {
        BOOST_CB_ASSERT(first.is_valid());            // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(last.is_valid());             // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(first.m_buff == last.m_buff); // check for iterators of different containers
        BOOST_CB_ASSERT(first <= last);               // check for wrong range
        if (first == last)
            return first;
        pointer p = first.m_it;
        while (last.m_it != 0)
            replace((first++).m_it, *last++);
        do {
            decrement(m_last);
            destroy_item(m_last);
            --m_size;
        } while(m_last != first.m_it);
        return m_last == p ? end() : iterator(this, p);
    }

    //! Erase the element at the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - 1</code><br>
              Removes an element at the position <code>pos</code>.
        \return iterator to the first element remaining in front of the removed
                element or <code>(*this).begin()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rerase(iterator pos) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(pos.m_it != 0);  // check for iterator pointing to end()
        pointer prev = pos.m_it;
        pointer p = prev;
        for (decrement(prev); p != m_first; p = prev, decrement(prev))
            replace(p, *prev);
        destroy_item(m_first);
        increment(m_first);
        --m_size;
#if BOOST_CB_ENABLE_DEBUG
        return p == pos.m_it ? begin() : iterator(this, pos.m_it);
#else
        return p == pos.m_it ? begin() : pos;
#endif
    }

    //! Erase the range <code>[first, last)</code>.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - std::distance(first, last)</code><br>
              Removes the elements from the range <code>[first, last)</code>.
        \return iterator to the first element remaining in front of the removed
                element or <code>(*this).begin()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rerase(iterator first, iterator last) {
        BOOST_CB_ASSERT(first.is_valid());            // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(last.is_valid());             // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(first.m_buff == last.m_buff); // check for iterators of different containers
        BOOST_CB_ASSERT(first <= last);               // check for wrong range
        if (first == last)
            return first;
        pointer p = map_pointer(last.m_it);
        last.m_it = p;
        while (first.m_it != m_first) {
            decrement(first.m_it);
            decrement(p);
            replace(p, *first.m_it);
        }
        do {
            destroy_item(m_first);
            increment(m_first);
            --m_size;
        } while(m_first != p);
        if (m_first == last.m_it)
            return begin();
        decrement(last.m_it);
        return iterator(this, last.m_it);
    }

    //! Erase all stored elements.
    /*!
        \post (*this).size() == 0
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void clear() {
        destroy_content();
        m_first = m_last = m_buff;
        m_size = 0;
    }

private:
// Helper methods

    //! Check if the <code>index</code> is valid.
    void check_position(size_type index) const {
        if (index >= size())
            throw_exception(std::out_of_range("circular_buffer"));
    }

    //! Increment the pointer.
    template <class Pointer0>
    void increment(Pointer0& p) const {
        if (++p == m_end)
            p = m_buff;
    }

    //! Decrement the pointer.
    template <class Pointer0>
    void decrement(Pointer0& p) const {
        if (p == m_buff)
            p = m_end;
        --p;
    }

    //! Add <code>n</code> to the pointer.
    template <class Pointer0>
    Pointer0 add(Pointer0 p, difference_type n) const {
        return p + (n < (m_end - p) ? n : n - capacity());
    }

    //! Subtract <code>n</code> from the pointer.
    template <class Pointer0>
    Pointer0 sub(Pointer0 p, difference_type n) const {
        return p - (n > (p - m_buff) ? n - capacity() : n);
    }

    //! Map the null pointer to virtual end of circular buffer.
    pointer map_pointer(pointer p) const { return p == 0 ? m_last : p; }

    //! Does the pointer point to the uninitialized memory?
    bool is_uninitialized(const_pointer p) const {
        return p >= m_last && (m_first < m_last || p < m_first);
    }

    //! Create a copy of the <code>item</code> at the given position.
    /*!
        The copy is created either at uninitialized memory or replaces the old item.
    */
    void create_or_replace(pointer pos, param_value_type item) {
        if (is_uninitialized(pos))
            m_alloc.construct(pos, item);
        else
            replace(pos, item);
    }

    //! Destroy an item in case it has been created.
    /*!
        Called when create_or_replace fails.
    */
    void destroy_created(pointer pos) {
        if (is_uninitialized(pos))
            destroy_item(pos);
    }

    //! Replace an element.
    void replace(pointer pos, param_value_type item) {
        *pos = item;
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators(iterator(this, pos));
#endif
    }

    //! Replace the first element in the full buffer.
    void replace_first(param_value_type item) {
        decrement(m_first);
        BOOST_CB_TRY
        replace(m_first, item);
        BOOST_CB_UNWIND(
            increment(m_first);
            decrement(m_last);
            --m_size;
        )
    }

    //! Replace the last element in the full buffer.
    void replace_last(param_value_type item) {
        BOOST_CB_TRY
        replace(m_last, item);
        BOOST_CB_UNWIND(
            decrement(m_last);
            --m_size;
        )
    }

    //! Tidy up after an exception is thrown.
    void tidy(pointer p) {
        for (; m_first != p; increment(m_first), --m_size)
            destroy_item(m_first);
        increment(m_first);
        --m_size;
    }

    //! Allocate memory.
    pointer allocate(size_type n) {
        if (n > max_size())
            throw_exception(std::length_error("circular_buffer"));
#if BOOST_CB_ENABLE_DEBUG
        pointer p = (n == 0) ? 0 : m_alloc.allocate(n, 0);
        ::memset(p, cb_details::CB_Unitialized, sizeof(value_type) * n);
        return p;
#else
        return (n == 0) ? 0 : m_alloc.allocate(n, 0);
#endif
    }

    //! Deallocate memory.
    void deallocate(pointer p, size_type n) {
        if (p != 0)
            m_alloc.deallocate(p, n);
    }

    //! Destroy an item.
    void destroy_item(pointer p) {
        m_alloc.destroy(p);
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators(iterator(this, p));
        ::memset(p, cb_details::CB_Unitialized, sizeof(value_type));
#endif
    }

    //! Destroy the whole content of the circular buffer.
    void destroy_content() {
        for (size_type ii = 0; ii < size(); ++ii, increment(m_first))
            destroy_item(m_first);
    }

    //! Destroy content and free allocated memory.
    void destroy() {
        destroy_content();
        deallocate(m_buff, capacity());
#if BOOST_CB_ENABLE_DEBUG
        invalidate_all_iterators(); // invalidate iterators pointing to end()
        m_buff = 0;
        m_first = 0;
        m_last = 0;
        m_end = 0;
#endif
    }

    //! Specialized assign method.
    template <class InputIterator>
    void assign(InputIterator n, InputIterator item, cb_details::cb_int_iterator_tag) {
        assign((size_type)n, item);
    }

    //! Specialized assign method.
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last, std::input_iterator_tag) {
        BOOST_CB_IS_CONVERTIBLE(InputIterator, value_type);
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        do_assign(std::distance(first, last), assign_range<InputIterator>(first, last, m_alloc));
    }

    //! Helper assign method.
    template <class Functor>
    void do_assign(size_type n, const Functor& fnc) {
        if (n > capacity()) {
            pointer buff = allocate(n);
            BOOST_CB_TRY
            fnc(buff);
            BOOST_CB_UNWIND(deallocate(buff, n))
            destroy();
            m_buff = buff;
            m_end = m_buff + n;
        } else {
            destroy_content();
            BOOST_CB_TRY
            fnc(m_buff);
            BOOST_CB_UNWIND(m_size = 0)
        }
        m_size = n;
        m_first = m_buff;
        m_last = add(m_buff, size());
    }

    //! Specialized insert method.
    template <class InputIterator>
    void insert(iterator pos, InputIterator n, InputIterator item, cb_details::cb_int_iterator_tag) {
        insert(pos, (size_type)n, item);
    }

    //! Specialized insert method.
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, std::input_iterator_tag) {
        BOOST_CB_IS_CONVERTIBLE(InputIterator, value_type);
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        difference_type n = std::distance(first, last);
        if (n == 0)
            return;
        difference_type copy = capacity() - (end() - pos);
        if (copy == 0)
            return;
        if (n > copy) {
            std::advance(first, n - copy);
            n = copy;
        }
        insert_n_item(pos, n, iterator_wrapper<InputIterator>(first));
    }

    //! Helper insert method.
    template <class Wrapper>
    void insert_n_item(iterator pos, size_type n, const Wrapper& wrapper) {
        size_type construct = capacity() - size();
        if (construct > n)
            construct = n;
        if (pos.m_it == 0) {
            size_type ii = 0;
            pointer p = m_last;
            BOOST_CB_TRY
            for (; ii < construct; ++ii, increment(p))
                m_alloc.construct(p, *wrapper.get_reference());
            for (;ii < n; ++ii, increment(p))
                replace(p, *wrapper.get_reference());
            BOOST_CB_UNWIND(
                size_type constructed = std::min(ii, construct);
                m_last = add(m_last, constructed);
                m_size += constructed;
                if (ii >= construct)
                    tidy(p);
            )
        } else {
            pointer src = m_last;
            pointer dest = add(m_last, n - 1);
            size_type ii = 0;
            BOOST_CB_TRY
            while (src != pos.m_it) {
                decrement(src);
                create_or_replace(dest, *src);
                decrement(dest);
            }
            for (dest = pos.m_it; ii < n; ++ii, increment(dest))
                create_or_replace(dest, *wrapper.get_reference());
            BOOST_CB_UNWIND(
                for (pointer p1 = m_last, p2 = add(m_last, n - 1); p1 != src; decrement(p2)) {
                    decrement(p1);
                    destroy_created(p2);
                }
                for (n = 0, src = pos.m_it; n < ii; ++n, increment(src))
                    destroy_created(src);
                if (!is_uninitialized(dest))
                    tidy(dest);
            )
        }
        m_last = add(m_last, n);
        m_first = add(m_first, n - construct);
        m_size += construct;
    }

    //! Specialized rinsert method.
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator n, InputIterator item, cb_details::cb_int_iterator_tag) {
        rinsert(pos, (size_type)n, item);
    }

    //! Specialized rinsert method.
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last, std::input_iterator_tag) {
        BOOST_CB_IS_CONVERTIBLE(InputIterator, value_type);
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        rinsert_n_item(pos, std::distance(first, last), iterator_wrapper<InputIterator>(first));
    }

    //! Helper rinsert method.
    template <class Wrapper>
    void rinsert_n_item(iterator pos, size_type n, const Wrapper& wrapper) {
        if (n == 0)
            return;
        size_type copy = capacity() - (pos - begin());
        if (copy == 0)
            return;
        if (n > copy)
            n = copy;
        size_type construct = capacity() - size();
        if (construct > n)
            construct = n;
        if (pos == begin()) {
            pointer p = sub(map_pointer(pos.m_it), n);
            size_type ii = n;
            BOOST_CB_TRY
            for (;ii > construct; --ii, increment(p))
                replace(p, *wrapper.get_reference());
            for (; ii > 0; --ii, increment(p))
                m_alloc.construct(p, *wrapper.get_reference());
            BOOST_CB_UNWIND(
                size_type unwind = ii < construct ? construct - ii : 0;
                pointer tmp = sub(map_pointer(pos.m_it), construct);
                for (n = 0; n < unwind; ++n, increment(tmp))
                    destroy_item(tmp);
                if (ii > construct)
                    tidy(p);
            )
        } else {
            pointer src = m_first;
            pointer dest = sub(m_first, n);
            pointer p = map_pointer(pos.m_it);
            size_type ii = 0;
            BOOST_CB_TRY
            while (src != p) {
                create_or_replace(dest, *src);
                increment(src);
                increment(dest);
            }
            for (dest = sub(p, n); ii < n; ++ii, increment(dest))
                create_or_replace(dest, *wrapper.get_reference());
            BOOST_CB_UNWIND(
                for (pointer p1 = m_first, p2 = sub(m_first, n); p1 != src; increment(p1), increment(p2))
                    destroy_created(p2);
                p = sub(p, n);
                for (n = 0; n < ii; ++n, increment(p))
                    destroy_created(p);
                if (!is_uninitialized(dest))
                    tidy(dest);
            )
        }
        m_first = sub(m_first, n);
        m_last = sub(m_last, n - construct);
        m_size += construct;
    }
};

// Non-member functions

//! Test two circular buffers for equality.
template <class T, class Alloc>
inline bool operator == (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator < (const circular_buffer<T, Alloc>& lhs,
                        const circular_buffer<T, Alloc>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || defined(BOOST_MSVC)

//! Test two circular buffers for non-equality.
template <class T, class Alloc>
inline bool operator != (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator > (const circular_buffer<T, Alloc>& lhs,
                        const circular_buffer<T, Alloc>& rhs) {
    return rhs < lhs;
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator <= (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator >= (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

//! Swap the contents of two circular buffers.
template <class T, class Alloc>
inline void swap(circular_buffer<T, Alloc>& lhs, circular_buffer<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

#endif // #if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || defined(BOOST_MSVC)

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_BASE_HPP)
