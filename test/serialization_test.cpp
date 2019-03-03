// Test of the circular buffer container serialization

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/circular_buffer.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>

using namespace boost::unit_test;

const int buffer_size = 100000;

template<typename buffer_type>
void basic_test() {
    buffer_type cb1(buffer_size);
    for (int i = 0; i < buffer_size; ++i) {
        cb1.push_back(i);
    }

    {
        // in-memory storage 
        std::stringstream ss;

        boost::archive::text_oarchive oa(ss);
        boost::serialization::serialize(oa, cb1, 0);

        buffer_type cb2(0);
        boost::archive::text_iarchive ia(ss);
        boost::serialization::serialize(ia, cb2, 0);
        BOOST_CHECK(cb1 == cb2);
    }
}

template<typename buffer_type>
void fragmented_buffer_test() {

    buffer_type cb1(buffer_size);
    for (auto i = 0; i < buffer_size/4; ++i)
        cb1.push_back(i+1);
    for (auto i = 0; i < buffer_size/4; ++i)
        cb1.push_front((i+1)*100);

    // making sure we are testing a fragmented buffer
    BOOST_CHECK(!cb1.is_linearized());

    std::stringstream ss;

    boost::archive::text_oarchive oa(ss);
    boost::serialization::serialize(oa, cb1, 0);

    buffer_type cb2(0);
    boost::archive::text_iarchive ia(ss);
    boost::serialization::serialize(ia, cb2, 0);

    BOOST_CHECK(cb1 == cb2);
}

// test main
test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for circular_buffer serialization.");

    tests->add(BOOST_TEST_CASE(&basic_test<boost::circular_buffer<double>>));
    tests->add(BOOST_TEST_CASE(&basic_test<boost::circular_buffer_space_optimized<double>>));
    tests->add(BOOST_TEST_CASE(&fragmented_buffer_test<boost::circular_buffer<double>>));
    tests->add(BOOST_TEST_CASE(&fragmented_buffer_test<boost::circular_buffer_space_optimized<double>>));

    return tests;
}

