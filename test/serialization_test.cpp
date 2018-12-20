// Test of the circular buffer container serialization

#include <boost/circular_buffer.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>

using namespace boost::unit_test;

const int buffer_size = 100000;

void basic_test() {
    boost::circular_buffer<double> cb1(buffer_size);
    for (int i = 0; i < buffer_size; ++i) {
        cb1.push_back(i);
    }

    {
        // in-memory storage 
        std::stringstream ss;

        boost::archive::text_oarchive oa(ss);
        boost::serialization::serialize(oa, cb1, 0);

        boost::circular_buffer<double> cb2(0);
        boost::archive::text_iarchive ia(ss);
        boost::serialization::serialize(ia, cb2, 0);
        BOOST_CHECK(cb1 == cb2);
    }
}

void spaced_optimized_test() {
    boost::circular_buffer_space_optimized<double> cb1(buffer_size);
    for (int i = 0; i < buffer_size; ++i) {
        cb1.push_back(i);
    }

    {
        // in-memory storage 
        std::stringstream ss;

        boost::archive::text_oarchive oa(ss);
        boost::serialization::serialize(oa, cb1, 0);

        boost::circular_buffer_space_optimized<double> cb2(0);
        boost::archive::text_iarchive ia(ss);
        boost::serialization::serialize(ia, cb2, 0);
        BOOST_CHECK(cb1 == cb2);
    }
}

// test main
test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for circular_buffer serialization.");

    tests->add(BOOST_TEST_CASE(&basic_test));
    tests->add(BOOST_TEST_CASE(&spaced_optimized_test));

    return tests;
}


