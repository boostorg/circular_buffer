// Test of the base circular buffer container.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <boost/test/included/unit_test_framework.hpp>

using namespace boost;
using namespace std;
using unit_test_framework::test_suite;

#define CB_CONTAINER circular_buffer
#define CB_MIN_CAPACITY /* none */

#include "common.cpp"

void iterator_constructor_and_assign_test() {

    circular_buffer<Integer> cb(4, 3);
    circular_buffer<Integer>::iterator it = cb.begin();
    circular_buffer<Integer>::iterator itCopy;
    itCopy = it;
    it = it;
    circular_buffer<Integer>::const_iterator cit;
    cit = it;
    circular_buffer<Integer>::const_iterator end1 = cb.end();
    circular_buffer<Integer>::const_iterator end2 = end1;

    BOOST_CHECK(itCopy == it);
    BOOST_CHECK(cit == it);
    BOOST_CHECK(end1 == end2);
    BOOST_CHECK(it != end1);
    BOOST_CHECK(cit != end2);
}

void iterator_reference_test() {

    circular_buffer<C> cb(3, C());
    circular_buffer<C>::iterator it = cb.begin();
    circular_buffer<C>::const_iterator cit = cb.begin() + 1;

    BOOST_CHECK((*it).test_reference1() == it->test_reference2());
    BOOST_CHECK((*cit).test_reference2() == cit->test_reference1());
}

void iterator_difference_test() {

    circular_buffer<Integer> cb(5, 1);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it1 = cb.begin() + 2;
    circular_buffer<Integer>::iterator it2 = cb.begin() + 3;
    circular_buffer<Integer>::const_iterator begin = cb.begin();
    circular_buffer<Integer>::iterator end = cb.end();

    BOOST_CHECK(begin - begin == 0);
    BOOST_CHECK(end - cb.begin() == 5);
    BOOST_CHECK(end - end == 0);
    BOOST_CHECK(begin - cb.end() == -5);
    BOOST_CHECK(it1 - cb.begin() == 2);
    BOOST_CHECK(end - it1 == 3);
    BOOST_CHECK(it2 - it1 == 1);
    BOOST_CHECK(it1 - it2 == -1);
    BOOST_CHECK(it2 - it2 == 0);
}

void iterator_increment_test() {

    circular_buffer<Integer> cb(10, 1);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it1 = cb.begin();
    circular_buffer<Integer>::iterator it2 = cb.begin() + 5;
    circular_buffer<Integer>::iterator it3 = cb.begin() + 9;
    it1++;
    it2++;
    ++it3;

    BOOST_CHECK(it1 == cb.begin() + 1);
    BOOST_CHECK(it2 == cb.begin() + 6);
    BOOST_CHECK(it3 == cb.end());
}

void iterator_decrement_test() {

    circular_buffer<Integer> cb(10, 1);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it1= cb.end();
    circular_buffer<Integer>::iterator it2= cb.end() - 5;
    circular_buffer<Integer>::iterator it3= cb.end() - 9;
    --it1;
    it2--;
    --it3;

    BOOST_CHECK(it1 == cb.end() - 1);
    BOOST_CHECK(it2 == cb.end() - 6);
    BOOST_CHECK(it3 == cb.begin());
}

void iterator_addition_test() {

    circular_buffer<Integer> cb(10, 1);
    cb.push_back(2);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it1 = cb.begin() + 2;
    circular_buffer<Integer>::iterator it2 = cb.end();
    circular_buffer<Integer>::iterator it3 = cb.begin() + 5;
    circular_buffer<Integer>::iterator it4 = cb.begin() + 9;
    it1 += 3;
    it2 += 0;
    it3 += 5;
    it4 += -2;

    BOOST_CHECK(it1 == 5 + cb.begin());
    BOOST_CHECK(it2 == cb.end());
    BOOST_CHECK(it3 == cb.end());
    BOOST_CHECK(it4 + 3 == cb.end());
    BOOST_CHECK((-3) + it4 == cb.begin() + 4);
    BOOST_CHECK(cb.begin() + 0 == cb.begin());
}

void iterator_subtraction_test() {

    circular_buffer<Integer> cb(10, 1);
    cb.push_back(2);
    cb.push_back(2);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it1 = cb.begin();
    circular_buffer<Integer>::iterator it2 = cb.end();
    circular_buffer<Integer>::iterator it3 = cb.end() - 5;
    circular_buffer<Integer>::iterator it4 = cb.begin() + 7;
    it1 -= -2;
    it2 -= 0;
    it3 -= 5;

    BOOST_CHECK(it1 == cb.begin() + 2);
    BOOST_CHECK(it2 == cb.end());
    BOOST_CHECK(it3 == cb.begin());
    BOOST_CHECK(it4 - 7 == cb.begin());
    BOOST_CHECK(it4 - (-3) == cb.end());
    BOOST_CHECK(cb.begin() - 0 == cb.begin());
}

void iterator_element_access_test() {

    circular_buffer<Integer> cb(10);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);
    cb.push_back(5);
    cb.push_back(6);
    circular_buffer<Integer>::iterator it = cb.begin() + 1;

    BOOST_CHECK(it[0] == 2);
    BOOST_CHECK(it[-1] == 1);
    BOOST_CHECK(it[2] == 4);
}

void iterator_comparison_test() {

    circular_buffer<Integer> cb(5, 1);
    cb.push_back(2);
    circular_buffer<Integer>::iterator it = cb.begin() + 2;
    circular_buffer<Integer>::const_iterator begin = cb.begin();
    circular_buffer<Integer>::iterator end = cb.end();

    BOOST_CHECK(begin == begin);
    BOOST_CHECK(end > cb.begin());
    BOOST_CHECK(begin < end);
    BOOST_CHECK(end > begin);
    BOOST_CHECK(end == end);
    BOOST_CHECK(begin < cb.end());
    BOOST_CHECK(!(begin + 1 > cb.end()));
    BOOST_CHECK(it > cb.begin());
    BOOST_CHECK(end > it);
    BOOST_CHECK(begin >= begin);
    BOOST_CHECK(end >= cb.begin());
    BOOST_CHECK(end <= end);
    BOOST_CHECK(begin <= cb.end());
    BOOST_CHECK(it >= cb.begin());
    BOOST_CHECK(end >= it);
    BOOST_CHECK(!(begin + 4 < begin + 4));
    BOOST_CHECK(begin + 4 < begin + 5);
    BOOST_CHECK(!(begin + 5 < begin + 4));
    BOOST_CHECK(it < end - 1);
    BOOST_CHECK(!(end - 1 < it));
}

void iterator_invalidation_test() {

#if !defined(NDEBUG) && !defined(BOOST_DISABLE_CB_DEBUG)

    circular_buffer<Integer>::iterator it1;
    circular_buffer<Integer>::const_iterator it2;
    circular_buffer<Integer>::iterator it3;
    circular_buffer<Integer>::const_iterator it4;
    circular_buffer<Integer>::const_iterator it5;

    BOOST_CHECK(!it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(!it3.is_valid());
    BOOST_CHECK(!it4.is_valid());
    BOOST_CHECK(!it5.is_valid());

    {
        circular_buffer<Integer> cb(5, 0);
        const circular_buffer<Integer> ccb(5, 0);

        it1 = cb.begin();
        it2 = ccb.end();
        it3 = it1;
        it4 = it1;
        it5 = it2;

        BOOST_CHECK(it1.is_valid());
        BOOST_CHECK(it2.is_valid());
        BOOST_CHECK(it3.is_valid());
        BOOST_CHECK(it4.is_valid());
        BOOST_CHECK(it5.is_valid());
    }

    BOOST_CHECK(!it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(!it3.is_valid());
    BOOST_CHECK(!it4.is_valid());
    BOOST_CHECK(!it5.is_valid());

    circular_buffer<Integer> cb1(10, 0);
    circular_buffer<Integer> cb2(20, 0);
    it1 = cb1.end();
    it2 = cb2.begin();
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(it2.is_valid());

    cb1.swap(cb2);
    BOOST_CHECK(!it1.is_valid());
    BOOST_CHECK(!it2.is_valid());

    it1 = cb1.begin() + 3;
    it2 = cb1.begin();
    cb1.push_back(1);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(*it2.m_it == 1);

    circular_buffer<Integer> cb3(5);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.push_back(4);
    cb3.push_back(5);
    it1 = cb3.begin() + 2;
    it2 = cb3.begin();
    cb3.insert(cb3.begin() + 3, 6);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(*it2.m_it == 5);

    it1 = cb3.begin() + 3;
    it2 = cb3.end() - 1;
    cb3.push_front(7);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(*it2.m_it == 7);

    circular_buffer<Integer> cb4(5);
    cb4.push_back(1);
    cb4.push_back(2);
    cb4.push_back(3);
    cb4.push_back(4);
    cb4.push_back(5);
    it1 = cb4.begin() + 3;
    it2 = cb4.begin();
    cb4.rinsert(cb4.begin() + 2, 6);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(*it2.m_it == 2);

    it1 = cb1.begin() + 5;
    it2 = cb1.end() - 1;
    cb1.pop_back();
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());

    it1 = cb1.begin() + 5;
    it2 = cb1.begin();
    cb1.pop_front();
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());

    circular_buffer<Integer> cb5(20, 0);
    it1 = cb5.begin() + 5;
    it2 = it3 = cb5.begin() + 15;
    cb5.erase(cb5.begin() + 10);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(!it3.is_valid());

    it1 = cb5.begin() + 1;
    it2 = it3 = cb5.begin() + 8;
    cb5.erase(cb5.begin() + 3, cb5.begin() + 7);
    BOOST_CHECK(it1.is_valid());
    BOOST_CHECK(!it2.is_valid());
    BOOST_CHECK(!it3.is_valid());

    circular_buffer<Integer> cb6(20, 0);
    it4 = it1 = cb6.begin() + 5;
    it2 = cb6.begin() + 15;
    cb6.rerase(cb6.begin() + 10);
    BOOST_CHECK(!it1.is_valid());
    BOOST_CHECK(!it4.is_valid());
    BOOST_CHECK(it2.is_valid());

    it4 = it1 = cb6.begin() + 1;
    it2 = cb6.begin() + 8;
    cb6.rerase(cb6.begin() + 3, cb6.begin() + 7);
    BOOST_CHECK(!it1.is_valid());
    BOOST_CHECK(!it4.is_valid());
    BOOST_CHECK(it2.is_valid());

#endif // #if !defined(NDEBUG) && !defined(BOOST_DISABLE_CB_DEBUG)
}

// basic exception safety test (it is useful to use any memory-leak detection tool)
void exception_safety_test() {

#if !defined(BOOST_NO_EXCEPTIONS)

    circular_buffer<Integer> cb1(3, 5);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb1.set_capacity(5), exception);

    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(circular_buffer<Integer> cb2(5, 10), exception);

    circular_buffer<Integer> cb3(5, 10);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(circular_buffer<Integer> cb4(cb3), exception);

    vector<Integer> v(5, 10);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(circular_buffer<Integer> cb5(8, v.begin(), v.end()), exception);

    circular_buffer<Integer> cb6(5, 10);
    circular_buffer<Integer> cb7(8, 3);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb7 = cb6, exception);

    circular_buffer<Integer> cb8(5, 10);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb8.push_front(1), exception);

    circular_buffer<Integer> cb9(5);
    cb9.push_back(1);
    cb9.push_back(2);
    cb9.push_back(3);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb9.insert(cb9.begin() + 1, 4), exception);

    circular_buffer<Integer> cb10(5);
    cb10.push_back(1);
    cb10.push_back(2);
    cb10.push_back(3);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb10.rinsert(cb10.begin() + 1, 4), exception);

    circular_buffer<Integer> cb11(5);
    cb11.push_back(1);
    cb11.push_back(2);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb11.rinsert(cb11.begin(), 1), exception);

    circular_buffer<Integer> cb12(5, 1);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb12.assign(4, 2), exception);

    circular_buffer<Integer> cb13(5, 1);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb13.assign(6, 2), exception);

    circular_buffer<Integer> cb14(5);
    cb14.push_back(1);
    cb14.push_back(2);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb14.insert(cb14.begin(), 10, 3), exception);

    circular_buffer<Integer> cb15(5);
    cb15.push_back(1);
    cb15.push_back(2);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb15.insert(cb15.end(), 10, 3), exception);

    circular_buffer<Integer> cb16(5);
    cb16.push_back(1);
    cb16.push_back(2);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb16.rinsert(cb16.begin(), 10, 3), exception);

    circular_buffer<Integer> cb17(5);
    cb17.push_back(1);
    cb17.push_back(2);
    Integer::set_exception_trigger(3);
    BOOST_CHECK_THROW(cb17.rinsert(cb17.end(), 10, 3), exception);

    circular_buffer<Integer> cb18(5, 0);
    cb18.push_back(1);
    cb18.push_back(2);
    cb18.pop_front();
    Integer::set_exception_trigger(4);
    BOOST_CHECK_THROW(cb18.linearize(), exception);

    circular_buffer<Integer> cb19(5, 0);
    cb19.push_back(1);
    cb19.push_back(2);
    Integer::set_exception_trigger(5);
    BOOST_CHECK_THROW(cb19.linearize(), exception);

    circular_buffer<Integer> cb20(5, 0);
    cb20.push_back(1);
    cb20.push_back(2);
    Integer::set_exception_trigger(6);
    BOOST_CHECK_THROW(cb20.linearize(), exception);

    circular_buffer<Integer> cb21(5);
    cb21.push_back(1);
    cb21.push_back(2);
    cb21.push_back(3);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb21.insert(cb21.begin() + 1, 4), exception);

    circular_buffer<Integer> cb22(5);
    cb22.push_back(1);
    cb22.push_back(2);
    cb22.push_back(3);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb22.insert(cb22.end(), 4), exception);

    circular_buffer<Integer> cb23(5, 0);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb23.insert(cb23.begin() + 1, 4), exception);

    circular_buffer<Integer> cb24(5);
    cb24.push_back(1);
    cb24.push_back(2);
    cb24.push_back(3);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb24.rinsert(cb24.begin() + 1, 4), exception);

    circular_buffer<Integer> cb25(5, 0);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb25.rinsert(cb25.begin() + 3, 4), exception);

    circular_buffer<Integer> cb26(5);
    cb26.push_back(1);
    cb26.push_back(2);
    Integer::set_exception_trigger(5);
    BOOST_CHECK_THROW(cb26.insert(cb26.begin(), 10, 3), exception);

    circular_buffer<Integer> cb27(5);
    cb27.push_back(1);
    cb27.push_back(2);
    Integer::set_exception_trigger(5);
    BOOST_CHECK_THROW(cb27.insert(cb27.end(), 10, 3), exception);

    circular_buffer<Integer> cb28(5);
    cb28.push_back(1);
    cb28.push_back(2);
    Integer::set_exception_trigger(5);
    BOOST_CHECK_THROW(cb28.rinsert(cb28.begin(), 10, 3), exception);

    circular_buffer<Integer> cb29(5);
    cb29.push_back(1);
    cb29.push_back(2);
    Integer::set_exception_trigger(5);
    BOOST_CHECK_THROW(cb29.rinsert(cb29.end(), 10, 3), exception);

    circular_buffer<Integer> cb30(10);
    cb30.push_back(1);
    cb30.push_back(2);
    cb30.push_back(3);
    Integer::set_exception_trigger(2);
    BOOST_CHECK_THROW(cb30.rinsert(cb30.begin(), 10, 3), exception);

#endif // #if !defined(BOOST_NO_EXCEPTIONS)
}

// test main
test_suite* init_unit_test_suite(int argc, char * argv[]) {

    test_suite* tests = BOOST_TEST_SUITE("Unit tests for the circular_buffer.");

    add_common_tests(tests);

    tests->add(BOOST_TEST_CASE(&iterator_constructor_and_assign_test));
    tests->add(BOOST_TEST_CASE(&iterator_reference_test));
    tests->add(BOOST_TEST_CASE(&iterator_difference_test));
    tests->add(BOOST_TEST_CASE(&iterator_increment_test));
    tests->add(BOOST_TEST_CASE(&iterator_decrement_test));
    tests->add(BOOST_TEST_CASE(&iterator_addition_test));
    tests->add(BOOST_TEST_CASE(&iterator_subtraction_test));
    tests->add(BOOST_TEST_CASE(&iterator_element_access_test));
    tests->add(BOOST_TEST_CASE(&iterator_comparison_test));
    tests->add(BOOST_TEST_CASE(&iterator_invalidation_test));
    tests->add(BOOST_TEST_CASE(&exception_safety_test));

    return tests;
}
