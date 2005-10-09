// Common tests for the circular buffer and its adaptor.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

void generic_test(CB_CONTAINER<Integer>& cb) {
    
    vector<int> v;
    v.push_back(11);
    v.push_back(12);
    v.push_back(13);
    v.push_back(14);
    v.push_back(15);
    v.push_back(16);
    v.push_back(17);
        
    if (cb.capacity() == 0) {
    
        cb.insert(cb.begin(), 1);
        cb.insert(cb.begin(), v.begin(), v.end());
        cb.rinsert(cb.end(), 2);
        cb.rinsert(cb.end(), v.begin(), v.end());
        cb.push_back(3);
        cb.push_front(4);
        cb.linearize();
        
        BOOST_CHECK(cb.empty());
        BOOST_CHECK(cb.full());
      
    } else {
    
        cb.insert(cb.end(), 1);
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[cb.size() - 1] == 1);
        
        size_t size = cb.size();
        cb.rerase(cb.end() - 1, cb.end());
        BOOST_CHECK(size == cb.size() + 1);
        
        cb.insert(cb.end(), v.begin(), v.end());
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[cb.size() - 1] == 17);
        
        size = cb.size();
        cb.erase(cb.end() - 1, cb.end());
        BOOST_CHECK(size == cb.size() + 1);

        size = cb.size();
        cb.rinsert(cb.begin(), 2);
        BOOST_CHECK(size + 1 == cb.size());
        BOOST_CHECK(cb[0] == 2);
        
        size = cb.size();
        cb.erase(cb.begin());
        BOOST_CHECK(size == cb.size() + 1);
        
        cb.rinsert(cb.begin(), v.begin(), v.end());
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[0] == 11);
        
        size = cb.size();
        cb.pop_front();
        BOOST_CHECK(size == cb.size() + 1);
        
        cb.push_back(3);
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[cb.size() - 1] == 3);
        
        size = cb.size();
        cb.pop_back();
        BOOST_CHECK(size == cb.size() + 1);
        
        cb.push_front(4);
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[0] == 4);
        
        cb.linearize();
        BOOST_CHECK(!cb.empty());
        BOOST_CHECK(cb[0] == 4);
        
        size = cb.size();
        cb.rerase(cb.begin());
        BOOST_CHECK(size == cb.size() + 1);
    }
}

void basic_test() {

    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
    CB_CONTAINER<Integer> cb1(3, v.begin(), v.end());
    CB_CONTAINER<Integer> cb2(10, v.begin(), v.end());
    CB_CONTAINER<Integer> cb3(7, v.begin(), v.end());

    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1.capacity() == 3);
    BOOST_CHECK(cb1.size() == 3);
    BOOST_CHECK(cb1[0] == 5);
    BOOST_CHECK(cb1[2] == 7);
    BOOST_CHECK(!cb2.full());
    BOOST_CHECK(cb2[2] == 3);
    BOOST_CHECK(cb3.full());
    BOOST_CHECK(cb3[0] == 1);
    BOOST_CHECK(cb3[6] == 7);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
}

void constructor_and_element_access_test() {

    CB_CONTAINER<int> cb(5, 3);
    cb[1] = 10;

    BOOST_CHECK(cb.full());
    BOOST_CHECK(cb[1] == 10);
    BOOST_CHECK(cb[4] == 3);
}

void size_test() {

    CB_CONTAINER<Integer> cb1(3);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    CB_CONTAINER<Integer> cb2(5);

    BOOST_CHECK(cb1.size() == 3);
    BOOST_CHECK(cb2.size() == 0);
    BOOST_CHECK(cb1.max_size() == cb2.max_size());
    
    generic_test(cb1);
    generic_test(cb2);
}

void boundary_capacity_test() {

    CB_CONTAINER<Integer> cb(0);
    cb.push_back(1);

    BOOST_CHECK(cb.size() == 0);
    BOOST_CHECK(cb.full());
    BOOST_CHECK(cb.empty());
    
    generic_test(cb);
}

void begin_and_end_test() {

    CB_CONTAINER<Integer> cb1(10);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.insert(cb1.begin(), 3);
    int i = 0;
    CB_CONTAINER<Integer>::const_iterator it = cb1.begin();
    for (; it != cb1.end(); it++) {
        i += *it;
    }
    CB_CONTAINER<Integer> cb2(20);

    BOOST_CHECK(i == 6);
    BOOST_CHECK(cb2.begin() == cb2.end());
    
    generic_test(cb1);
    generic_test(cb2);
}

void rbegin_and_rend_test() {

    CB_CONTAINER<Integer> cb1(3);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.insert(cb1.begin(), 3);
    cb1.push_back(1);
    int i = 0;
    CB_CONTAINER<Integer>::reverse_iterator it = cb1.rbegin();
    for (; it != cb1.rend(); it++) {
        i += *it;
    }
    CB_CONTAINER<Integer> cb2(20);

    BOOST_CHECK(i == 4);
    BOOST_CHECK(cb2.rbegin() == cb2.rend());
    
    generic_test(cb1);
    generic_test(cb2);
}

void element_access_and_insert_test() {

    CB_CONTAINER<Integer> cb(3);
    cb.push_back(1);
    cb.push_back(2);
    cb.insert(cb.begin(), 3);
    cb.push_back(4);
    const CB_CONTAINER<Integer> ccb(3, 2);

    BOOST_CHECK(cb[0] == 1);
    BOOST_CHECK(cb[1] == 2);
    BOOST_CHECK(cb[2] == 4);
    BOOST_CHECK(ccb[2] == 2);
    
    generic_test(cb);
}

void at_test() {

#if !defined(BOOST_NO_EXCEPTIONS)

    CB_CONTAINER<Integer> cb(3);
    cb.push_back(1);

    try {
        BOOST_CHECK(cb.at(0) == 1);
    }
    catch (out_of_range&) {
        BOOST_ERROR("An unexpected exception has been thrown!");
    }

    BOOST_CHECK_THROW(cb.at(2), out_of_range);
    
    generic_test(cb);

#endif // #if !defined(BOOST_NO_EXCEPTIONS)
}

void front_and_back_test() {

    CB_CONTAINER<Integer> cb(1);
    cb.push_back(2);
    cb.push_back(3);

    BOOST_CHECK(cb.front() == cb.back());
    BOOST_CHECK(cb.back() == 3);
    
    generic_test(cb);
}

void array_test() {
    // TODO
    CB_CONTAINER<Integer> cb(1);
    CB_CONTAINER<Integer>::array_range a1 = cb.array_one();
    CB_CONTAINER<Integer>::const_array_range a2 = cb.array_two();

    BOOST_CHECK(a1.second == 0);
    BOOST_CHECK(a2.second == 0);
    
    generic_test(cb);
}

void linearize_test() {

	vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	v.push_back(10);
	v.push_back(11);
	v.push_back(12);
    CB_CONTAINER<Integer> cb1(10, v.begin(), v.begin() + 10);
    cb1.push_back(11);
    cb1.push_back(12);
    cb1.push_back(13);
    CB_CONTAINER<Integer> cb2(10, v.begin(), v.begin() + 10);
    cb2.push_back(11);
    cb2.push_back(12);
    cb2.push_back(13);
    cb2.push_back(14);
    cb2.push_back(15);
    cb2.push_back(16);
    cb2.push_back(17);
    CB_CONTAINER<Integer> cb3(10, v.begin(), v.begin() + 10);
    cb3.push_back(11);
    cb3.push_back(12);
    cb3.push_back(13);
    cb3.pop_front();
    cb3.pop_front();
    CB_CONTAINER<Integer> cb4(5);
	CB_CONTAINER<Integer> cb5(12, v.begin(), v.end());
    cb5.push_back(13);
    cb5.push_back(14);
    cb5.push_back(15);
    cb5.pop_front();
    cb5.pop_front();
	cb5.pop_front();
	cb5.pop_front();
	cb5.pop_front();
	cb5.pop_front();
	CB_CONTAINER<Integer> cb6(6);
	cb6.push_back(-2);
	cb6.push_back(-1);
	cb6.push_back(0);
	cb6.push_back(1);
	cb6.push_back(2);
	cb6.push_back(3);
    cb6.push_back(4);
    cb6.push_back(5);
	cb6.push_back(6);
	cb6.pop_back();

    BOOST_CHECK(*cb1.linearize() == 4);
	BOOST_CHECK(&cb1[0] < &cb1[1]
		&& &cb1[1] < &cb1[2]
		&& &cb1[2] < &cb1[3]
		&& &cb1[3] < &cb1[4]
		&& &cb1[4] < &cb1[5]
		&& &cb1[5] < &cb1[6]
		&& &cb1[6] < &cb1[7]
		&& &cb1[7] < &cb1[8]
		&& &cb1[8] < &cb1[9]);
    BOOST_CHECK(*(cb1.linearize() + 1) == 5);
	BOOST_CHECK(*(cb1.linearize() + 2) == 6);
	BOOST_CHECK(*(cb1.linearize() + 3) == 7);
	BOOST_CHECK(*(cb1.linearize() + 4) == 8);
	BOOST_CHECK(*(cb1.linearize() + 5) == 9);
	BOOST_CHECK(*(cb1.linearize() + 6) == 10);
    BOOST_CHECK(*(cb1.linearize() + 7) == 11);
    BOOST_CHECK(*(cb1.linearize() + 8) == 12);
    BOOST_CHECK(*(cb1.linearize() + 9) == 13);
    BOOST_CHECK(*cb2.linearize() == 8);
	BOOST_CHECK(&cb2[0] < &cb2[1]
		&& &cb2[1] < &cb2[2]
		&& &cb2[2] < &cb2[3]
		&& &cb2[3] < &cb2[4]
		&& &cb2[4] < &cb2[5]
		&& &cb2[5] < &cb2[6]
		&& &cb2[6] < &cb2[7]
		&& &cb2[7] < &cb2[8]
		&& &cb2[8] < &cb2[9]);
    BOOST_CHECK(*(cb2.linearize() + 1) == 9);
    BOOST_CHECK(*(cb2.linearize() + 2) == 10);
    BOOST_CHECK(*(cb2.linearize() + 3) == 11);
    BOOST_CHECK(*(cb2.linearize() + 4) == 12);
    BOOST_CHECK(*(cb2.linearize() + 5) == 13);
    BOOST_CHECK(*(cb2.linearize() + 6) == 14);
    BOOST_CHECK(*(cb2.linearize() + 7) == 15);
    BOOST_CHECK(*(cb2.linearize() + 8) == 16);
    BOOST_CHECK(*(cb2.linearize() + 9) == 17);
    BOOST_CHECK(*cb3.linearize() == 6);
	BOOST_CHECK(&cb3[0] < &cb3[1]
		&& &cb3[1] < &cb3[2]
		&& &cb3[2] < &cb3[3]
		&& &cb3[3] < &cb3[4]
		&& &cb3[4] < &cb3[5]
		&& &cb3[5] < &cb3[6]
		&& &cb3[6] < &cb3[7]);
    BOOST_CHECK(*(cb3.linearize() + 1) == 7);
    BOOST_CHECK(*(cb3.linearize() + 2) == 8);
    BOOST_CHECK(*(cb3.linearize() + 3) == 9);
    BOOST_CHECK(*(cb3.linearize() + 4) == 10);
    BOOST_CHECK(*(cb3.linearize() + 5) == 11);
    BOOST_CHECK(*(cb3.linearize() + 6) == 12);
    BOOST_CHECK(*(cb3.linearize() + 7) == 13);
    BOOST_CHECK(cb4.linearize() == 0);
	BOOST_CHECK(*cb5.linearize() == 10);
	BOOST_CHECK(&cb5[0] < &cb5[1]
		&& &cb5[1] < &cb5[2]
		&& &cb5[2] < &cb5[3]
		&& &cb5[3] < &cb5[4]
		&& &cb5[4] < &cb5[5]);
    BOOST_CHECK(*(cb5.linearize() + 1) == 11);
    BOOST_CHECK(*(cb5.linearize() + 2) == 12);
    BOOST_CHECK(*(cb5.linearize() + 3) == 13);
    BOOST_CHECK(*(cb5.linearize() + 4) == 14);
    BOOST_CHECK(*(cb5.linearize() + 5) == 15);
	BOOST_CHECK(*cb6.linearize() == 1);
	BOOST_CHECK(&cb6[0] < &cb6[1]
		&& &cb6[1] < &cb6[2]
		&& &cb6[2] < &cb6[3]
		&& &cb6[3] < &cb6[4]);
    BOOST_CHECK(*(cb6.linearize() + 1) == 2);
    BOOST_CHECK(*(cb6.linearize() + 2) == 3);
    BOOST_CHECK(*(cb6.linearize() + 3) == 4);
    BOOST_CHECK(*(cb6.linearize() + 4) == 5);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
    generic_test(cb5);
    generic_test(cb6);
}

void capacity_test() {

    CB_CONTAINER<Integer> cb1(0);
    CB_CONTAINER<Integer> cb2(1);

    BOOST_CHECK(cb1.capacity() == 0);
    BOOST_CHECK(cb2.capacity() == 1);
    
    generic_test(cb1);
    generic_test(cb2);
}

void full_and_empty_test() {

    CB_CONTAINER<Integer> cb1(10);
    CB_CONTAINER<Integer> cb2(3);
    CB_CONTAINER<Integer> cb3(2);
    CB_CONTAINER<Integer> cb4(2);
    cb2.push_back(1);
    cb2.push_back(3);
    cb2.push_back(1);
    cb2.push_back(1);
    cb2.push_back(1);
    cb3.push_back(3);
    cb3.push_back(1);
    cb4.push_back(1);

    BOOST_CHECK(cb1.empty());
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb3.full());
    BOOST_CHECK(!cb4.empty());
    BOOST_CHECK(!cb4.full());
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void set_capacity_test() {

    CB_CONTAINER<Integer> cb1(10);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(1);
    cb1.set_capacity(5);
    CB_CONTAINER<Integer> cb2(3);
    cb2.push_back(2);
    cb2.push_back(3);
    cb2.push_back(1);
    cb2.set_capacity(10);
    CB_CONTAINER<Integer> cb3(5);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.push_back(1);
    cb3.set_capacity(2);
    cb3.set_capacity(2);

    BOOST_CHECK(cb1.size() == 3);
    BOOST_CHECK(cb1[0] == 2);
    BOOST_CHECK(cb1.capacity() == 5);
    BOOST_CHECK(cb2.size() == 3);
    BOOST_CHECK(cb2[0] == 2);
    BOOST_CHECK(cb2.capacity() == 10);
    BOOST_CHECK(cb3.size() == 2);
    BOOST_CHECK(cb3[0] == 3);
    BOOST_CHECK(cb3.capacity() == 2);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
}

void rset_capacity_test() {

    CB_CONTAINER<Integer> cb(10);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(1);
    cb.rset_capacity(2);

    BOOST_CHECK(cb.size() == 2);
    BOOST_CHECK(cb[0] == 2);
    BOOST_CHECK(cb.capacity() == 2);
    
    generic_test(cb);
}

void resize_test() {

    CB_CONTAINER<Integer> cb1(10);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    cb1.resize(20, 5);
    CB_CONTAINER<Integer> cb2(10);
    cb2.push_back(1);
    cb2.push_back(2);
    cb2.push_back(3);
    cb2.push_back(4);
    cb2.resize(2);
    CB_CONTAINER<Integer> cb3(10, 1);
    cb3.resize(0);
    CB_CONTAINER<Integer> cb4(10, 1);
    cb4.resize(10);

    BOOST_CHECK(cb1.size() == 20);
    BOOST_CHECK(cb1.capacity() == 20);
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[3] == 4);
    BOOST_CHECK(cb1[4] == 5);
    BOOST_CHECK(cb1[19] == 5);
    BOOST_CHECK(cb2.size() == 2);
    BOOST_CHECK(cb2.capacity() == 10);
    BOOST_CHECK(cb2[0] == 3);
    BOOST_CHECK(cb2[1] == 4);
    BOOST_CHECK(cb3.size() == 0);
    BOOST_CHECK(cb3.capacity() == 10);
    BOOST_CHECK(cb4.size() == 10);
    BOOST_CHECK(cb4.capacity() == 10);
    BOOST_CHECK(cb4[0] == 1);
    BOOST_CHECK(cb4[9] == 1);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void rresize_test() {

    CB_CONTAINER<Integer> cb(10);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);
    cb.rresize(2);

    BOOST_CHECK(cb.size() == 2);
    BOOST_CHECK(cb.capacity() == 10);
    BOOST_CHECK(cb[0] == 1);
    BOOST_CHECK(cb[1] == 2);
    
    generic_test(cb);
}

void constructor_test() {

    CB_CONTAINER<Integer> cb1(3);
    CB_CONTAINER<Integer> cb2(3, 2);

    BOOST_CHECK(cb1.size() == 0);
    BOOST_CHECK(cb1.capacity() == 3);
    BOOST_CHECK(cb2[0] == 2);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb2[0] == 2);
    BOOST_CHECK(cb2[1] == 2);
    BOOST_CHECK(cb2[2] == 2);
    
    generic_test(cb1);
    generic_test(cb2);
}

void assign_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    cb1.push_back(5);
    cb1.assign(3, 8);
    BOOST_CHECK(cb1.size() == 3);
    BOOST_CHECK(cb1.capacity() == 3);
    BOOST_CHECK(cb1[0] == 8);
    BOOST_CHECK(cb1[2] == 8);

    cb1.assign(6, 7);
    BOOST_CHECK(cb1.size() == 6);
    BOOST_CHECK(cb1.capacity() == 6);
    BOOST_CHECK(cb1[0] == 7);
    BOOST_CHECK(cb1[5] == 7);

    CB_CONTAINER<float> cb2(4);
    cb2.assign(3, 1.1f);
    BOOST_CHECK(cb2[0] == 1.1f);

    CB_CONTAINER<Integer> cb3(5);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.assign((size_t)10, 1); // The size_t cast is not needed. It is present here just because of testing purposes.
    BOOST_CHECK(cb3[0] == 1);
    BOOST_CHECK(cb3[9] == 1);
    BOOST_CHECK(cb3.size() == 10);
    BOOST_CHECK(cb3.capacity() == 10);
    
    generic_test(cb1);
    generic_test(cb3);
}

void copy_constructor_and_assign_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    cb1.push_back(5);
    CB_CONTAINER<Integer> cb2 = cb1;

    BOOST_CHECK(cb1 == cb2);

    CB_CONTAINER<Integer> cb3(20);
    cb1.pop_back();
    CB_CONTAINER<Integer> cb4(3);
    cb3 = cb2;
    cb3 = cb3;
    cb4 = cb1;

    BOOST_CHECK(cb3 == cb2);
    BOOST_CHECK(cb4 == cb1);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb2[0] == 2);
    BOOST_CHECK(cb3.full());
    BOOST_CHECK(cb3.capacity() == 4);
    BOOST_CHECK(cb4.capacity() == 4);
    BOOST_CHECK(!cb4.full());
    BOOST_CHECK(*(cb4.end() - 1) == 4);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void swap_test() {

    CB_CONTAINER<Integer> cb1(2);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    CB_CONTAINER<Integer> cb2(5);
    cb2.push_back(8);
    cb2.swap(cb1);
    cb2.swap(cb2);

    BOOST_CHECK(cb2.capacity() == 2);
    BOOST_CHECK(cb2[0] == 2);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb1.capacity() == 5);
    BOOST_CHECK(cb1[0] == 8);
    BOOST_CHECK(cb1.size() == 1);
    
    generic_test(cb1);
    generic_test(cb2);
}

void push_back_test() {

    CB_CONTAINER<X> cb1(5);
    cb1.push_back();
    cb1.push_back(X(2));
    BOOST_CHECK(cb1[0].m_n == 1);
    BOOST_CHECK(cb1[1].m_n == 2);

    CB_CONTAINER<Integer> cb2(5);
    cb2.push_back();
    BOOST_CHECK(cb2.back() == CB_CONTAINER<Integer>::value_type());

    cb2.push_back(1);
    BOOST_CHECK(cb2.back() == 1);
    
    generic_test(cb2);
}

void pop_back_test() {

    CB_CONTAINER<Integer> cb(4);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);
    cb.push_back(5);
    cb.pop_back();

    BOOST_CHECK(cb.size() == 3);
    BOOST_CHECK(!cb.full());
    BOOST_CHECK(cb[0] == 2);
    
    generic_test(cb);
}

void insert_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    CB_CONTAINER<Integer>::iterator it1 = cb1.begin() + 1;
    it1 = cb1.insert(it1, 10);
    CB_CONTAINER<Integer> cb2(4);
    cb2.push_back(1);
    cb2.insert(cb2.begin());
    cb2.insert(cb2.begin(), -1);
    CB_CONTAINER<Integer>::iterator it2 = cb2.begin() + 1;
    it2 = cb2.insert(it2, 5);
    CB_CONTAINER<Integer> cb3(2);
    cb3.insert(cb3.end(), 10);
    cb3.insert(cb3.end(), 20);
    cb3.insert(cb3.begin(), 30);
    cb3.insert(cb3.end(), 40);

    BOOST_CHECK(cb1[1] == 10);
    BOOST_CHECK(*it1 == 10);
    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb2[1] == 5);
    BOOST_CHECK(*it2 == 5);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb3[0] == 20);
    BOOST_CHECK(cb3[1] == 40);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
}

void insert_n_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.insert(cb1.begin() + 1, 2, 10);
    CB_CONTAINER<Integer> cb2(2, 3);
    cb2.insert(cb2.begin(), 10, 5);
    CB_CONTAINER<Integer> cb3(4);
    cb3.insert(cb3.end(), 1, 6);
    CB_CONTAINER<Integer> cb4(6);
    cb4.push_back(1);
    cb4.push_back(2);
    cb4.push_back(3);
    cb4.push_back(4);
    cb4.insert(cb4.begin() + 2, 5, 6);
    cb4.insert(cb4.begin() + 2, 0, 7);

    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1[0] == 10);
    BOOST_CHECK(cb1[1] == 10);
    BOOST_CHECK(cb1[2] == 2);
    BOOST_CHECK(cb1[3] == 3);
    BOOST_CHECK(cb2[0] == 3);
    BOOST_CHECK(cb2[1] == 3);
    BOOST_CHECK(cb3[0] == 6);
    BOOST_CHECK(cb3.size() == 1);
    BOOST_CHECK(cb4.size() == 6);
    BOOST_CHECK(cb4[0] == 6);
    BOOST_CHECK(cb4[1] == 6);
    BOOST_CHECK(cb4[2] == 6);
    BOOST_CHECK(cb4[3] == 6);
    BOOST_CHECK(cb4[4] == 3);
    BOOST_CHECK(cb4[5] == 4);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void insert_range_test() {

    vector<int> v;
    v.push_back(11);
    v.push_back(12);
    v.push_back(13);
    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.insert(cb1.begin() + 1, v.begin(), v.end());
    CB_CONTAINER<Integer> cb2(2, 2);
    cb2.insert(cb2.end(), v.begin(), v.end());
    CB_CONTAINER<Integer> cb3(5);
    cb3.insert(cb3.end(), v.end(), v.end());
    CB_CONTAINER<Integer> cb4(5);
    cb4.insert(cb4.end(), v.begin(), v.begin() + 1);
    Integer array[] = { 5, 6, 7, 8, 9 };
    CB_CONTAINER<Integer> cb5(6);
    cb5.push_back(1);
    cb5.push_back(2);
    cb5.push_back(3);
    cb5.push_back(4);
    cb5.insert(cb5.begin() + 2, array, array + 5);
    cb5.insert(cb5.begin(), array, array + 5);

    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1[0] == 12);
    BOOST_CHECK(cb1[1] == 13);
    BOOST_CHECK(cb1[2] == 2);
    BOOST_CHECK(cb1[3] == 3);
    BOOST_CHECK(cb2[0] == 12);
    BOOST_CHECK(cb2[1] == 13);
    BOOST_CHECK(cb3.empty());
    BOOST_CHECK(cb4[0] == 11);
    BOOST_CHECK(cb4.size() == 1);
    BOOST_CHECK(cb5.size() == 6);
    BOOST_CHECK(cb5[0] == 6);
    BOOST_CHECK(cb5[1] == 7);
    BOOST_CHECK(cb5[2] == 8);
    BOOST_CHECK(cb5[3] == 9);
    BOOST_CHECK(cb5[4] == 3);
    BOOST_CHECK(cb5[5] == 4);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
    generic_test(cb5);
}

void push_front_test() {

    CB_CONTAINER<X> cb1(5);
    cb1.push_front();
    cb1.push_front(X(2));
    BOOST_CHECK(cb1[0].m_n == 2);
    BOOST_CHECK(cb1[1].m_n == 1);

    CB_CONTAINER<Integer> cb2(5);
    cb2.push_front();
    BOOST_CHECK(cb2.front() == CB_CONTAINER<Integer>::value_type());

    cb2.push_front(1);
    BOOST_CHECK(cb2.front() == 1);

    CB_CONTAINER<Integer> cb3(0);
    cb3.push_front(10);
    BOOST_CHECK(cb3.empty());
    
    generic_test(cb2);
    generic_test(cb3);
}

void pop_front_test() {

    CB_CONTAINER<Integer> cb(4);
    cb.push_front(1);
    cb.push_front(2);
    cb.push_front(3);
    cb.push_front(4);
    cb.push_front(5);
    cb.pop_front();

    BOOST_CHECK(cb.size() == 3);
    BOOST_CHECK(!cb.full());
    BOOST_CHECK(cb[0] == 4);
    
    generic_test(cb);
}

void rinsert_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_front(1);
    cb1.push_front(2);
    cb1.push_front(3);
    CB_CONTAINER<Integer>::iterator it1 = cb1.begin() + 1;
    it1 = cb1.rinsert(it1, 10);
    CB_CONTAINER<Integer> cb2(4);
    cb2.push_front(1);
    cb2.rinsert(cb2.begin());
    cb2.rinsert(cb2.begin(), -1);
    CB_CONTAINER<Integer>::iterator it2 = cb2.end() - 2;
    it2 = cb2.rinsert(it2, 5);
    CB_CONTAINER<Integer> cb3(2);
    cb3.rinsert(cb3.begin(), 10);
    cb3.rinsert(cb3.begin(), 20);
    cb3.rinsert(cb3.end(), 30);
    cb3.rinsert(cb3.begin(), 40);
    CB_CONTAINER<Integer> cb4(4);
    cb4.push_back(1);
    cb4.push_back(2);
    cb4.push_back(3);
    CB_CONTAINER<Integer>::iterator it3 = cb4.begin() + 1;
    it3 = cb4.rinsert(it3, 10);
    CB_CONTAINER<Integer> cb5(4);
    cb5.push_back(1);
    cb5.rinsert(cb5.begin(), 0);
    cb5.rinsert(cb5.begin(), -1);
    CB_CONTAINER<Integer>::iterator it4 = cb5.begin() + 1;
    it4 = cb5.rinsert(it4, 5);
    CB_CONTAINER<Integer> cb6(2);
    cb6.rinsert(cb6.end(), 10);
    cb6.rinsert(cb6.end(), 20);
    cb6.rinsert(cb6.begin(), 30);
    cb6.rinsert(cb6.end(), 40);
    CB_CONTAINER<Integer> cb7(6);
    cb7.push_back(1);
    cb7.push_back(2);
    cb7.push_back(3);
    cb7.push_back(4);
    cb7.rinsert(cb7.begin() + 2, 5, 6);

    BOOST_CHECK(cb1[1] == 10);
    BOOST_CHECK(*it1 == 10);
    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb2[1] == 5);
    BOOST_CHECK(*it2 == 5);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb2[3] == 1);
    BOOST_CHECK(cb3[0] == 40);
    BOOST_CHECK(cb3[1] == 20);
    BOOST_CHECK(cb4[1] == 10);
    BOOST_CHECK(*it3 == 10);
    BOOST_CHECK(cb4.full());
    BOOST_CHECK(cb5[1] == 5);
    BOOST_CHECK(*it4 == 5);
    BOOST_CHECK(cb5.full());
    BOOST_CHECK(cb6[0] == 30);
    BOOST_CHECK(cb6[1] == 10);
    BOOST_CHECK(cb7.size() == 6);
    BOOST_CHECK(cb7[0] == 1);
    BOOST_CHECK(cb7[1] == 2);
    BOOST_CHECK(cb7[2] == 6);
    BOOST_CHECK(cb7[3] == 6);
    BOOST_CHECK(cb7[4] == 6);
    BOOST_CHECK(cb7[5] == 6);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
    generic_test(cb5);
    generic_test(cb6);
    generic_test(cb7);
}

void rinsert_n_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_front(1);
    cb1.push_front(2);
    cb1.push_front(3);
    cb1.rinsert(cb1.begin() + 1, 2, 10);
    CB_CONTAINER<Integer> cb2(2, 3);
    cb2.rinsert(cb2.begin(), 10, 5);
    CB_CONTAINER<Integer> cb3(4);
    cb3.rinsert(cb3.end(), 1, 6);
    CB_CONTAINER<Integer> cb4(4);
    cb4.push_back(1);
    cb4.push_back(2);
    cb4.push_back(3);
    cb4.rinsert(cb4.begin() + 1, 2, 10);
    Integer array[] = { 5, 6, 7, 8, 9 };
    CB_CONTAINER<Integer> cb5(6);
    cb5.push_back(1);
    cb5.push_back(2);
    cb5.push_back(3);
    cb5.push_back(4);
    cb5.rinsert(cb5.begin() + 2, array, array + 5);
    cb5.rinsert(cb5.end(), array, array + 5);

    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1[0] == 3);
    BOOST_CHECK(cb1[1] == 10);
    BOOST_CHECK(cb1[2] == 10);
    BOOST_CHECK(cb1[3] == 2);
    BOOST_CHECK(cb2[0] == 5);
    BOOST_CHECK(cb2[1] == 5);
    BOOST_CHECK(cb3[0] == 6);
    BOOST_CHECK(cb3.size() == 1);
    BOOST_CHECK(cb4.full());
    BOOST_CHECK(cb4[0] == 1);
    BOOST_CHECK(cb4[1] == 10);
    BOOST_CHECK(cb4[2] == 10);
    BOOST_CHECK(cb4[3] == 2);
    BOOST_CHECK(cb5.size() == 6);
    BOOST_CHECK(cb5[0] == 1);
    BOOST_CHECK(cb5[1] == 2);
    BOOST_CHECK(cb5[2] == 5);
    BOOST_CHECK(cb5[3] == 6);
    BOOST_CHECK(cb5[4] == 7);
    BOOST_CHECK(cb5[5] == 8);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
    generic_test(cb5);
}

void rinsert_range_test() {

    vector<int> v;
    v.push_back(11);
    v.push_back(12);
    v.push_back(13);
    v.push_back(14);
    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.rinsert(cb1.begin() + 1, v.begin(), v.end());
    CB_CONTAINER<Integer> cb2(2, 2);
    cb2.rinsert(cb2.begin(), v.begin(), v.end());
    CB_CONTAINER<Integer> cb3(5);
    cb3.rinsert(cb3.begin(), v.end(), v.end());
    CB_CONTAINER<Integer> cb4(5);
    cb4.rinsert(cb4.begin(), v.begin(), v.begin() + 1);

    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[1] == 11);
    BOOST_CHECK(cb1[2] == 12);
    BOOST_CHECK(cb1[3] == 13);
    BOOST_CHECK(cb2[0] == 11);
    BOOST_CHECK(cb2[1] == 12);
    BOOST_CHECK(cb3.empty());
    BOOST_CHECK(cb4[0] == 11);
    BOOST_CHECK(cb4.size() == 1);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void erase_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    CB_CONTAINER<Integer>::iterator it1 = cb1.erase(cb1.begin() + 1);

    CB_CONTAINER<Integer> cb2(1, 1);
    CB_CONTAINER<Integer>::iterator it2 = cb2.erase(cb2.begin());

    CB_CONTAINER<Integer> cb3(4);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    CB_CONTAINER<Integer>::iterator it3 = cb3.erase(cb3.begin() + 2);

    BOOST_CHECK(cb1.size() == 2);
    BOOST_CHECK(cb1.capacity() == 4);
    BOOST_CHECK(*it1 == 3);
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[1] == 3);
    BOOST_CHECK(cb2.size() == 0);
    BOOST_CHECK(cb2.capacity() == 1);
    BOOST_CHECK(it2 == cb2.end());
    BOOST_CHECK(cb3.size() == 2);
    BOOST_CHECK(cb3.capacity() == 4);
    BOOST_CHECK(it3 == cb3.end());
    BOOST_CHECK(cb3[0] == 1);
    BOOST_CHECK(cb3[1] == 2);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
}

void erase_range_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    CB_CONTAINER<Integer>::iterator it1 = cb1.erase(cb1.begin() + 1, cb1.begin() + 3);

    CB_CONTAINER<Integer> cb2(4);
    cb2.push_back(1);
    cb2.push_back(2);
    cb2.push_back(3);
    cb2.push_back(4);
    CB_CONTAINER<Integer>::iterator it2 = cb2.erase(cb2.begin(), cb2.begin());

    CB_CONTAINER<Integer> cb3(4);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.push_back(4);
    CB_CONTAINER<Integer>::iterator it3 = cb3.erase(cb3.begin() + 2, cb3.end());

    CB_CONTAINER<Integer> cb4(10, 1);
    CB_CONTAINER<Integer>::iterator it4 = cb4.erase(cb4.begin(), cb4.end());

    BOOST_CHECK(cb1.size() == 2);
    BOOST_CHECK(cb1.capacity() == 4);
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[1] == 4);
    BOOST_CHECK(*it1 == 4);
    BOOST_CHECK(cb2.size() == 4);
    BOOST_CHECK(cb2[0] == 1);
    BOOST_CHECK(cb2[3] == 4);
    BOOST_CHECK(*it2 == 1);
    BOOST_CHECK(cb3.size() == 2);
    BOOST_CHECK(cb3.capacity() == 4);
    BOOST_CHECK(cb3[0] == 1);
    BOOST_CHECK(cb3[1] == 2);
    BOOST_CHECK(it3 == cb3.end());
    BOOST_CHECK(cb4.size() == 0);
    BOOST_CHECK(cb4.capacity() == 10);
    BOOST_CHECK(it4 == cb4.end());
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void rerase_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    CB_CONTAINER<Integer>::iterator it1 = cb1.rerase(cb1.begin() + 1);

    CB_CONTAINER<Integer> cb2(1, 1);
    CB_CONTAINER<Integer>::iterator it2 = cb2.rerase(cb2.begin());

    CB_CONTAINER<Integer> cb3(4);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    CB_CONTAINER<Integer>::iterator it3 = cb3.rerase(cb3.begin());

    BOOST_CHECK(cb1.size() == 2);
    BOOST_CHECK(cb1.capacity() == 4);
    BOOST_CHECK(*it1 == 1);
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[1] == 3);
    BOOST_CHECK(cb2.size() == 0);
    BOOST_CHECK(cb2.capacity() == 1);
    BOOST_CHECK(it2 == cb2.begin());
    BOOST_CHECK(cb3.size() == 2);
    BOOST_CHECK(cb3.capacity() == 4);
    BOOST_CHECK(it3 == cb3.begin());
    BOOST_CHECK(*it3 == 2);
    BOOST_CHECK(cb3[0] == 2);
    BOOST_CHECK(cb3[1] == 3);
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
}

void rerase_range_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    CB_CONTAINER<Integer>::iterator it1 = cb1.rerase(cb1.begin() + 1, cb1.begin() + 3);

    CB_CONTAINER<Integer> cb2(4);
    cb2.push_back(1);
    cb2.push_back(2);
    cb2.push_back(3);
    cb2.push_back(4);
    CB_CONTAINER<Integer>::iterator it2 = cb2.rerase(cb2.begin(), cb2.begin());

    CB_CONTAINER<Integer> cb3(4);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.push_back(4);
    CB_CONTAINER<Integer>::iterator it3 = cb3.rerase(cb3.begin(), cb3.begin() + 2);

    CB_CONTAINER<Integer> cb4(10, 1);
    CB_CONTAINER<Integer>::iterator it4 = cb4.rerase(cb4.begin(), cb4.end());

    BOOST_CHECK(cb1.size() == 2);
    BOOST_CHECK(cb1.capacity() == 4);
    BOOST_CHECK(cb1[0] == 1);
    BOOST_CHECK(cb1[1] == 4);
    BOOST_CHECK(*it1 == 1);
    BOOST_CHECK(cb2.size() == 4);
    BOOST_CHECK(cb2[0] == 1);
    BOOST_CHECK(cb2[3] == 4);
    BOOST_CHECK(*it2 == 1);
    BOOST_CHECK(cb3.size() == 2);
    BOOST_CHECK(cb3.capacity() == 4);
    BOOST_CHECK(cb3[0] == 3);
    BOOST_CHECK(cb3[1] == 4);
    BOOST_CHECK(it3 == cb3.begin());
    BOOST_CHECK(cb4.size() == 0);
    BOOST_CHECK(cb4.capacity() == 10);
    BOOST_CHECK(it4 == cb4.begin());
    
    generic_test(cb1);
    generic_test(cb2);
    generic_test(cb3);
    generic_test(cb4);
}

void clear_test() {

    CB_CONTAINER<Integer> cb(4);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);
    cb.clear();

    BOOST_CHECK(cb.empty());
    
    generic_test(cb);
}

void equality_test() {

    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    CB_CONTAINER<Integer> cb2(10);
    cb2.push_back(1);
    cb2.push_back(2);
    cb2.push_back(3);
    cb2.push_back(4);

    BOOST_CHECK(cb1 == cb2);
    BOOST_CHECK(!(cb2 != cb1));
    
    generic_test(cb1);
    generic_test(cb2);
}

void lexicographical_comparison_test() {

    CB_CONTAINER<char> cb1(10);
    cb1.push_back('a');
    cb1.push_back('d');
    cb1.push_back('a');
    cb1.push_back('m');
    CB_CONTAINER<char> cb2(5);
    cb2.push_back('j');
    cb2.push_back('o');
    cb2.push_back('h');
    cb2.push_back('n');

    BOOST_CHECK(cb2 > cb1);
    BOOST_CHECK(cb1 < cb2);
}

void assign_range_test() {

    vector<int> v;
    v.push_back(11);
    v.push_back(12);
    v.push_back(13);
    CB_CONTAINER<Integer> cb1(4);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.assign(v.begin() + 1, v.end());
    CB_CONTAINER<Integer> cb2(2);
    cb2.push_back(1);
    cb2.push_back(2);
    cb2.assign(v.begin(), v.end());

    BOOST_CHECK(cb1.capacity() == 2);
    BOOST_CHECK(cb1[0] == 12);
    BOOST_CHECK(cb1[1] == 13);
    BOOST_CHECK(cb2.full());
    BOOST_CHECK(cb2.capacity() == 3);
    BOOST_CHECK(cb2[0] == 11);
    BOOST_CHECK(cb2[1] == 12);
    BOOST_CHECK(cb2[2] == 13);
    BOOST_CHECK(cb2.size() == (size_t)distance(v.begin(), v.end()));
    
    generic_test(cb1);
    generic_test(cb2);
}

// test of the example (introduced in the documentation)
void example_test() {

    CB_CONTAINER<int> cb1(3);
    cb1.push_back(1);
    cb1.push_back(2);
    cb1.push_back(3);
    cb1.push_back(4);
    int sum = accumulate(cb1.begin(), cb1.end(), 0);

    BOOST_CHECK(cb1[0] == 2);
    BOOST_CHECK(cb1[1] == 3);
    BOOST_CHECK(cb1[2] == 4);
    BOOST_CHECK(sum == 9);
    BOOST_CHECK(cb1.full());
    BOOST_CHECK(cb1.size() == 3);
    BOOST_CHECK(cb1.capacity() == 3);

    CB_CONTAINER<int> cb2(5, 1);
    cb2.insert(cb2.begin(), 2);

    BOOST_CHECK(cb2[0] == 1);
    BOOST_CHECK(cb2[1] == 1);
    BOOST_CHECK(cb2[2] == 1);
    BOOST_CHECK(cb2[3] == 1);
    BOOST_CHECK(cb2[4] == 1);

    vector<int> v;
    v.push_back(100);
    v.push_back(200);
    v.push_back(300);
    cb2.insert(cb2.begin() + 1, v.begin(), v.end());

    BOOST_CHECK(cb2[0] == 300);
    BOOST_CHECK(cb2[1] == 1);
    BOOST_CHECK(cb2[2] == 1);
    BOOST_CHECK(cb2[3] == 1);
    BOOST_CHECK(cb2[4] == 1);

    CB_CONTAINER<int> cb3(3);
    cb3.push_back(1);
    cb3.push_back(2);
    cb3.push_back(3);
    cb3.push_back(4);
    cb3.push_back(5);
    BOOST_CHECK(cb3[0] == 3);
    BOOST_CHECK(cb3[1] == 4);
    BOOST_CHECK(cb3[2] == 5);

    cb3.pop_back();
    cb3.pop_front();
    BOOST_CHECK(cb3[0] == 4);
}

void element_destruction_test() {

    CB_CONTAINER<Y> cb(5);
    cb.push_back(Y());
    cb.push_back(Y());
    cb.push_back(Y());
    int prevCount = Y::count();
    cb.clear();

    BOOST_CHECK(cb.empty());
    BOOST_CHECK(prevCount == 3);
    BOOST_CHECK(Y::count() == 0);
}

void const_methods_test() {

    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    const CB_CONTAINER<Integer> cb(5, v.begin(), v.end());

    BOOST_CHECK(*cb.begin() == 1);
    BOOST_CHECK(*(cb.end() - 1) == 5);
    BOOST_CHECK(*cb.rbegin() == 5);
    BOOST_CHECK(*(cb.rend() - 1) == 1);
    BOOST_CHECK(cb[0] == 1);
    BOOST_CHECK(cb.at(1) == 2);
    BOOST_CHECK(cb.front() == 1);
    BOOST_CHECK(cb.back() == 5);
}

void adaptor_test() {
    int array[] = {0, 1, 2, 3};
    Adaptor<int> container(5);
    container.insert(container.begin(), array, array + 4);

    BOOST_CHECK(container.size() == 4);
    BOOST_CHECK(container[0] == 0);
    BOOST_CHECK(container[1] == 1);
    BOOST_CHECK(container[2] == 2);
    BOOST_CHECK(container[3] == 3);

    container.insert(container.begin() + 1, array, array + 4);

    BOOST_CHECK(container.size() == 8);
    BOOST_CHECK(container[0] == 0);
    BOOST_CHECK(container[1] == 0);
    BOOST_CHECK(container[2] == 1);
    BOOST_CHECK(container[3] == 2);
    BOOST_CHECK(container[4] == 3);
    BOOST_CHECK(container[5] == 1);
    BOOST_CHECK(container[6] == 2);
    BOOST_CHECK(container[7] == 3);
}

int Integer::ms_exception_trigger = 0;
int Y::ms_count = 0;

// add common tests into a test suite
void add_common_tests(test_suite* tests) {
    tests->add(BOOST_TEST_CASE(&basic_test));
    tests->add(BOOST_TEST_CASE(&constructor_and_element_access_test));
    tests->add(BOOST_TEST_CASE(&size_test));
    tests->add(BOOST_TEST_CASE(&boundary_capacity_test));
    tests->add(BOOST_TEST_CASE(&begin_and_end_test));
    tests->add(BOOST_TEST_CASE(&rbegin_and_rend_test));
    tests->add(BOOST_TEST_CASE(&element_access_and_insert_test));
    tests->add(BOOST_TEST_CASE(&at_test));
    tests->add(BOOST_TEST_CASE(&front_and_back_test));
    tests->add(BOOST_TEST_CASE(&array_test));
    tests->add(BOOST_TEST_CASE(&linearize_test));
    tests->add(BOOST_TEST_CASE(&capacity_test));
    tests->add(BOOST_TEST_CASE(&full_and_empty_test));
    tests->add(BOOST_TEST_CASE(&set_capacity_test));
    tests->add(BOOST_TEST_CASE(&rset_capacity_test));
    tests->add(BOOST_TEST_CASE(&resize_test));
    tests->add(BOOST_TEST_CASE(&rresize_test));
    tests->add(BOOST_TEST_CASE(&constructor_test));
    tests->add(BOOST_TEST_CASE(&assign_test));
    tests->add(BOOST_TEST_CASE(&copy_constructor_and_assign_test));
    tests->add(BOOST_TEST_CASE(&swap_test));
    tests->add(BOOST_TEST_CASE(&push_back_test));
    tests->add(BOOST_TEST_CASE(&pop_back_test));
    tests->add(BOOST_TEST_CASE(&insert_test));
    tests->add(BOOST_TEST_CASE(&insert_n_test));
    tests->add(BOOST_TEST_CASE(&insert_range_test));
    tests->add(BOOST_TEST_CASE(&push_front_test));
    tests->add(BOOST_TEST_CASE(&pop_front_test));
    tests->add(BOOST_TEST_CASE(&rinsert_test));
    tests->add(BOOST_TEST_CASE(&rinsert_n_test));
    tests->add(BOOST_TEST_CASE(&rinsert_range_test));
    tests->add(BOOST_TEST_CASE(&erase_test));
    tests->add(BOOST_TEST_CASE(&erase_range_test));
    tests->add(BOOST_TEST_CASE(&rerase_test));
    tests->add(BOOST_TEST_CASE(&rerase_range_test));
    tests->add(BOOST_TEST_CASE(&clear_test));
    tests->add(BOOST_TEST_CASE(&equality_test));
    tests->add(BOOST_TEST_CASE(&lexicographical_comparison_test));
    tests->add(BOOST_TEST_CASE(&assign_range_test));
    tests->add(BOOST_TEST_CASE(&example_test));
    tests->add(BOOST_TEST_CASE(&element_destruction_test));
    tests->add(BOOST_TEST_CASE(&const_methods_test));
    tests->add(BOOST_TEST_CASE(&adaptor_test));
}
