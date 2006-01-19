// Comparison of bounded buffers based on different containers.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CB_DISABLE_DEBUG 1

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <string>
#include <iostream>

const unsigned long QUEUE_SIZE     = 1000L;
const unsigned long TOTAL_ELEMENTS = QUEUE_SIZE * 1000L;

template <class T>
class bounded_buffer {
public:

	typedef boost::circular_buffer<T> buffer_type;	
	typedef typename buffer_type::size_type size_type;
	typedef typename buffer_type::value_type value_type;

	bounded_buffer(size_type capacity) : m_unread(0), m_buffer(capacity) {}
	
	void push_front(const value_type& item) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
		m_not_empty.notify_one();
		m_buffer.push_front(item);
		++m_unread;
	}
	
	void pop_back(value_type* pItem) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
		m_not_full.notify_one(); // Wakes up one of the threads waiting for the buffer to free a space
		                         // for the next item. However the woken-up thread has to regain the mutex, which
		                         // means it will not proceed until the pop_front() method returns.
		*pItem = m_buffer[--m_unread];
	}

private:
	bounded_buffer(const bounded_buffer&);              // Disabled copy constructor
	bounded_buffer& operator = (const bounded_buffer&); // Disabled assign operator

	bool is_not_empty() const { return m_unread > 0; }
	bool is_not_full() const { return m_unread < m_buffer.capacity(); }

	size_type m_unread;
	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_not_empty;
	boost::condition m_not_full;
};

template <class T>
class bounded_buffer_space_optimized {
public:

	typedef boost::circular_buffer_space_optimized<T> buffer_type;	
	typedef typename buffer_type::size_type size_type;
	typedef typename buffer_type::value_type value_type;

	bounded_buffer_space_optimized(size_type capacity) : m_buffer(capacity) {}

	void push_front(const value_type& item) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_full.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_full, this));
		m_not_empty.notify_one();
		m_buffer.push_front(item);
	}

	void pop_back(value_type* pItem) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_empty.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_empty, this));
		m_not_full.notify_one();
		*pItem = m_buffer.back();
		m_buffer.pop_back();
	}

private:

	bounded_buffer_space_optimized(const bounded_buffer_space_optimized&);              // Disabled copy constructor
	bounded_buffer_space_optimized& operator = (const bounded_buffer_space_optimized&); // Disabled assign operator

	bool is_not_empty() const { return m_buffer.size() > 0; }
	bool is_not_full() const { return m_buffer.size() < m_buffer.capacity(); }

	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_not_empty;
	boost::condition m_not_full;
};

template <class T>
class bounded_buffer_deque_based {
public:

	typedef std::deque<T> buffer_type;	
	typedef typename buffer_type::size_type size_type;
	typedef typename buffer_type::value_type value_type;

	bounded_buffer_deque_based(size_type capacity) : m_capacity(capacity) {}
	
	void push_front(const value_type& item) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_full.wait(lock, boost::bind(&bounded_buffer_deque_based<value_type>::is_not_full, this));
		m_not_empty.notify_one();
		m_buffer.push_front(item);
	}
	
	void pop_back(value_type* pItem) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_not_empty.wait(lock, boost::bind(&bounded_buffer_deque_based<value_type>::is_not_empty, this));
		m_not_full.notify_one();
		*pItem = m_buffer.back();
		m_buffer.pop_back();
	}

private:
	
	bounded_buffer_deque_based(const bounded_buffer_deque_based&);              // Disabled copy constructor
	bounded_buffer_deque_based& operator = (const bounded_buffer_deque_based&); // Disabled assign operator

	bool is_not_empty() const { return m_buffer.size() > 0; }
	bool is_not_full() const { return m_buffer.size() < m_capacity; }
	
	const size_type m_capacity;
	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_not_empty;
	boost::condition m_not_full;
};

template<class Buffer>
class Consumer {

	typedef typename Buffer::value_type value_type;
    Buffer* m_buffer;
	value_type m_item;

public:
    Consumer(Buffer* buffer) : m_buffer(buffer) {}
    
    void operator()() {
		for (unsigned long i = 0L; i < TOTAL_ELEMENTS; ++i) {
			m_buffer->pop_back(&m_item);
		}
    }
};

template<class Buffer>
class Producer {

    typedef typename Buffer::value_type value_type;
    Buffer* m_buffer;

public:
    Producer(Buffer* buffer) : m_buffer(buffer) {}
    
    void operator()() {
		for (unsigned long i = 0L; i < TOTAL_ELEMENTS; ++i) {
			m_buffer->push_front(value_type());
		}
    }
};

template<class Buffer>
void fifo_test(Buffer* buffer) {

	// Start of measurement
	boost::progress_timer progress;

	// Initialize the buffer with some values before launching producer and consumer threads.
	for (unsigned long i = QUEUE_SIZE / 2L; i > 0; --i) {
		buffer->push_front(BOOST_DEDUCED_TYPENAME Buffer::value_type());
	}

	Consumer<Buffer> consumer(buffer);
	Producer<Buffer> producer(buffer);

	// Start the threads.
	boost::thread consume(consumer);
	boost::thread produce(producer);
	
	// Wait for completion.
	consume.join();
	produce.join();

	// End of measurement
}

int main(int /*argc*/, char* /*argv*/[]) {

	bounded_buffer<int> bb_int(QUEUE_SIZE);
	std::cout << "bounded_buffer<int> ";
    fifo_test(&bb_int);

	bounded_buffer_space_optimized<int> bb_space_optimized_int(QUEUE_SIZE);
	std::cout << "bounded_buffer_space_optimized<int> ";
	fifo_test(&bb_space_optimized_int);
    
	bounded_buffer_deque_based<int> bb_deque_based_int(QUEUE_SIZE);
	std::cout << "bounded_buffer_deque_based<int> ";
	fifo_test(&bb_deque_based_int);

	bounded_buffer<std::string> bb_string(QUEUE_SIZE);
	std::cout << "bounded_buffer<std::string> ";
    fifo_test(&bb_string);

	bounded_buffer_space_optimized<std::string> bb_space_optimized_string(QUEUE_SIZE);
	std::cout << "bounded_buffer_space_optimized<std::string> ";
	fifo_test(&bb_space_optimized_string);
    
	bounded_buffer_deque_based<std::string> bb_deque_based_string(QUEUE_SIZE);
	std::cout << "bounded_buffer_deque_based<std::string> ";
	fifo_test(&bb_deque_based_string);

	return 0;
}
