// Test of the iterator of the circular buffer.
// Note: This test concentrates on interator validity only. Other iterator
//       tests are included in the base_test.cpp.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CB_DISABLE_DEBUG

#include "test.hpp"

// TODO
void validity_test() {

	circular_buffer_space_optimized<int> cb;
    
    BOOST_CHECK(cb.capacity() == cb.max_size());
}

// test main
test_suite* init_unit_test_suite(int argc, char* argv[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for the iterator of the circular_buffer.");

    tests->add(BOOST_TEST_CASE(&validity_test));

    return tests;
}
