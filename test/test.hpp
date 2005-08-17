// Header file for the test of the circular buffer library.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_TEST_HPP)
#define BOOST_CIRCULAR_BUFFER_TEST_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include "../../../boost/circular_buffer.hpp"
#include <numeric>
#include <iterator>
#include <vector>
#if !defined(BOOST_NO_EXCEPTIONS)
    #include <exception>
#endif

// Integer - substitute for int - more appropriate for testing
class Integer {
private:
    int* m_pValue;
    static int ms_exception_trigger;
    void check_exception() {
        if (ms_exception_trigger > 0) {
            if (--ms_exception_trigger == 0) {
                delete m_pValue;
                m_pValue = 0;
#if !defined(BOOST_NO_EXCEPTIONS)
                throw std::exception();
#endif
            }
        }
    }
public:
    Integer() : m_pValue(new int(0)) { check_exception(); }
    Integer(int i) : m_pValue(new int(i)) { check_exception(); }
    Integer(const Integer& src) : m_pValue(new int(src)) { check_exception(); }
    ~Integer() { delete m_pValue; }
    Integer& operator = (const Integer& src) {
        if (this == &src)
            return *this;
        check_exception();
        delete m_pValue;
        m_pValue = new int(src);
        return *this;
    }
    operator int () const { return *m_pValue; }
    static void set_exception_trigger(int n) { ms_exception_trigger = n; }
};

struct X
{
    X() : m_n(1) {}
    X(int n) : m_n(n) {}
    int m_n;
};

class Y {
public:
    Y() { increment(); }
    Y(const Y& y) { y.increment(); }
    ~Y() { decrement(); }
    static int count() { return ms_count; }
private:
    void increment() const { ++ms_count; }
    void decrement() const { --ms_count; }
    static int ms_count;
};

class Z {
public:
    Z() : m_num(255) {}
    virtual int test_reference1() const { return m_num; }
    int test_reference2() const { return 255; }
private:
    int m_num;
};

template <class T> class Adaptor {
private:
    boost::circular_buffer<T> m_buff;
public:
    typedef typename boost::circular_buffer<T>::iterator iterator;
    typedef typename boost::circular_buffer<T>::size_type size_type;

    Adaptor(size_type capacity) : m_buff(capacity) {}
    template <class InputIterator>
    Adaptor(size_type capacity, InputIterator first, InputIterator last)
    : m_buff(capacity, first, last) {}

    iterator begin() { return m_buff.begin(); }
    iterator end() { return m_buff.end(); }
    size_type size() const { return m_buff.size(); }
    size_type capacity() const { return m_buff.capacity(); }
    T& operator [] (size_type index) { return m_buff[index]; }

    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        size_type new_size = size() + distance(first, last);
        if (new_size > capacity()) {
            boost::circular_buffer<T> buff(new_size, begin(), pos);
            buff.insert(buff.end(), first, last);
            buff.insert(buff.end(), pos, end());
            m_buff.swap(buff);
        } else {
            m_buff.insert(pos, first, last);
        }
    }
};

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_TEST_HPP)
