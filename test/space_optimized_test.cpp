// Test of the space optimized adaptor of the circular buffer.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CB_TEST

#include "test.hpp"

#define CB_CONTAINER circular_buffer_space_optimized

#include "common.ipp"

typedef circular_buffer_space_optimized<Integer> cb_space_optimized;
typedef cb_space_optimized::capacity_control capacity_ctrl;

// min_capacity test (it is useful to use a debug tool)
void min_capacity_test() {

    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    cb_space_optimized cb1(capacity_ctrl(10, 10));
    cb_space_optimized cb2(capacity_ctrl(10, 5), 1);
    cb_space_optimized cb3(capacity_ctrl(20, 10), v.begin(), v.end());

    BOOST_CHECK(cb1.size() == 0);
    BOOST_CHECK(cb1.capacity() == 10);
    BOOST_CHECK(cb1.min_capacity() == 10);
    BOOST_CHECK(cb2[0] == 1);
    BOOST_CHECK(cb2.size() == 10);
    BOOST_CHECK(cb2.capacity() == 10);
    BOOST_CHECK(cb2.min_capacity() == 5);
    BOOST_CHECK(cb3[0] == 1);
    BOOST_CHECK(cb3.size() == 5);
    BOOST_CHECK(cb3.capacity() == 20);
    BOOST_CHECK(cb3.min_capacity() == 10);
    BOOST_CHECK(cb1.min_capacity() <= cb1.internal_capacity());
    BOOST_CHECK(cb2.min_capacity() <= cb2.internal_capacity());
    BOOST_CHECK(cb3.min_capacity() <= cb3.internal_capacity());

    cb2.erase(cb2.begin() + 2, cb2.end());

    BOOST_CHECK(cb2.size() == 2);
    BOOST_CHECK(cb2.min_capacity() <= cb2.internal_capacity());

    cb2.clear();
    cb3.clear();

    BOOST_CHECK(cb2.empty());
    BOOST_CHECK(cb3.empty());
    BOOST_CHECK(cb2.min_capacity() <= cb2.internal_capacity());
    BOOST_CHECK(cb3.min_capacity() <= cb3.internal_capacity());
}

// test main
test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for the circular_buffer_space_optimized.");
    add_common_tests(tests);

    tests->add(BOOST_TEST_CASE(&min_capacity_test));

    return tests;
}
