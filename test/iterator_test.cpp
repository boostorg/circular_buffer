// Test of the iterator of the circular buffer.
// Note: This test concentrates on iterator validity only. Other iterator
//       tests are included in the base_test.cpp.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CB_DISABLE_DEBUG

#include "test.hpp"

// test of the example (introduced in the documentation)
void validity_example_test() {

	circular_buffer<int> cb(3);

	cb.push_back(1);
	cb.push_back(2);
	cb.push_back(3);

	circular_buffer<int>::iterator it = cb.begin();

	BOOST_CHECK(*it == 1);

	cb.push_back(4);

	BOOST_CHECK(*it == 4);
}

void validity_insert_test() {

	circular_buffer<int> cb1(4);
	cb1.push_back(1);
	cb1.push_back(2);
	cb1.push_back(3);
	cb1.push_back(4);
	circular_buffer<int>::iterator it1 = cb1.begin();
	BOOST_CHECK(*it1 == 1);
	cb1.insert(cb1.begin() + 1, 5);
	BOOST_CHECK(*it1 == 4);

	circular_buffer<int> cb2(4);
	cb2.push_back(1);
	cb2.push_back(2);
	cb2.push_back(3);
	cb2.push_back(4);
	circular_buffer<int>::iterator it2 = cb2.begin();
	BOOST_CHECK(*it2 == 1);
	cb2.insert(cb2.begin() + 1, 2, 5);
	BOOST_CHECK(*it2 == 4);
}

void validity_test() {
	// erase
	// rerase
	// insert
	// rinsert
	// push_back
	// push_front
	// linearize
	// swap
	// set_capacity
	// rset_capacity
	// resize
	// rresize
}

// test main
test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for the iterator of the circular_buffer.");

    tests->add(BOOST_TEST_CASE(&validity_example_test));
	tests->add(BOOST_TEST_CASE(&validity_insert_test));

    return tests;
}
