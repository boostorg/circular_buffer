// Tests to track that emplace does not make copies.

// Copyright (c) 2023 Alexander van Gessel

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <boost/lexical_cast.hpp>

typedef class BaseConstructDestructTracker {
public:
    static size_t construct_count;
    static size_t destruct_count;

    BaseConstructDestructTracker() {
        ++construct_count;
    }
    ~BaseConstructDestructTracker() {
        ++destruct_count;
    }
    static void reset() {
        construct_count = 0;
        destruct_count = 0;
    }
} BCDT;

size_t BCDT::construct_count;
size_t BCDT::destruct_count;

class NoArgTracker : BaseConstructDestructTracker {};
template<typename A1, typename A2>
class TwoArgTracker : BaseConstructDestructTracker {
    A1 a1;
    A2 a2;

public:
    explicit TwoArgTracker(A1 a1, A2 a2) : a1(a1), a2(a2) {}
};

void track_base_test() {
    const size_t capacity = 8;
    const size_t iters = 16;
    BCDT::reset();
    circular_buffer<NoArgTracker> cb(capacity);
    BOOST_TEST(BCDT::construct_count == 0);
    BOOST_TEST(BCDT::destruct_count == 0);
    for(size_t i = 1; i <= iters; ++i) {
        cb.emplace_back();
        BOOST_TEST(BCDT::construct_count == i);
        BOOST_TEST(BCDT::destruct_count == i - cb.size());
    }
    for(size_t i = 1; i <= capacity; ++i) {
        cb.pop_back();
        BOOST_TEST(BCDT::construct_count == iters);
        BOOST_TEST(BCDT::destruct_count == iters - cb.size());
    }
    BOOST_TEST(BCDT::construct_count == BCDT::destruct_count);
    BOOST_TEST(cb.size() == 0);
    BCDT::reset();
    for(size_t i = 1; i <= iters; ++i) {
        cb.emplace_front();
        BOOST_TEST(BCDT::construct_count == i);
        BOOST_TEST(BCDT::destruct_count == i - cb.size());
    }
}

void track_twoarg_test() {
    const size_t capacity = 8;
    const size_t iters = 16;
    const std::string prefix = "iteration ";
    BCDT::reset();
    circular_buffer<TwoArgTracker<size_t, std::string>> cb(capacity);
    BOOST_TEST(BCDT::construct_count == 0);
    BOOST_TEST(BCDT::destruct_count == 0);
    for(size_t i = 1; i <= iters; ++i) {
        cb.emplace_back(i, prefix + boost::lexical_cast<std::string>(i));
        BOOST_TEST(BCDT::construct_count == i);
        BOOST_TEST(BCDT::destruct_count == i - cb.size());
    }
    for(size_t i = 1; i <= capacity; ++i) {
        cb.pop_back();
        BOOST_TEST(BCDT::construct_count == iters);
        BOOST_TEST(BCDT::destruct_count == iters - cb.size());
    }
    BOOST_TEST(BCDT::construct_count == BCDT::destruct_count);
    BOOST_TEST(cb.size() == 0);
    BCDT::reset();
    for(size_t i = 1; i <= iters; ++i) {
        cb.emplace_front(i, prefix + boost::lexical_cast<std::string>(i));
        BOOST_TEST(BCDT::construct_count == i);
        BOOST_TEST(BCDT::destruct_count == i - cb.size());
    }
}

// test main
int main()
{
    track_base_test();
    track_twoarg_test();
    return boost::report_errors();
}
