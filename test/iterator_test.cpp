// Test of the iterator of the circular buffer.
// Note: This test concentrates on iterator validity only. Other iterator
//       tests are included in the base_test.cpp.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CB_DISABLE_DEBUG

#include "test.hpp"

int array[] = { 1, 2, 3, 4, 5 };

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

	circular_buffer<int> cb(4, array, array + 3);
	circular_buffer<int>::iterator it1 = cb.begin();
	circular_buffer<int>::iterator it2 = cb.begin() + 1;
	circular_buffer<int>::iterator it3 = cb.begin() + 2;
	
	// cb = {1, 2, 3}
	BOOST_CHECK(*it1 == 1);
	BOOST_CHECK(*it2 == 2);
	BOOST_CHECK(*it3 == 3);

	cb.insert(cb.begin() + 1, 4);
	
	// cb = {1, 4, 2, 3}
	BOOST_CHECK(*it1 == 1);
	BOOST_CHECK(*it2 == 4);
	BOOST_CHECK(*it3 == 2);

	cb.insert(cb.begin() + 1, 5);

	// cb = {3, 5, 4, 2}
	BOOST_CHECK(*it1 == 3);
	BOOST_CHECK(*it2 == 5);
	BOOST_CHECK(*it3 == 4);
}

void validity_insert_n_test() {

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

// test main
test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for the iterator of the circular_buffer.");

    tests->add(BOOST_TEST_CASE(&validity_example_test));
	tests->add(BOOST_TEST_CASE(&validity_insert_test));
	tests->add(BOOST_TEST_CASE(&validity_insert_n_test));

    return tests;
}
