
#define BOOST_CB_DISABLE_DEBUG 1


#include "boost/circular_buffer.hpp"
#include <boost/thread/xtime.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <string>
#include <iostream>

const unsigned int queue_size     = 500L;
const unsigned int total_elements = queue_size * 1000L;

template <class T>
class bounded_buffer {
public:

	typedef boost::circular_buffer<T> buffer_type;	
	typedef typename buffer_type::size_type size_type;
	typedef typename buffer_type::value_type value_type;

	bounded_buffer(size_type capacity) : m_unread(0), m_buffer(capacity) {}
	
	void push_back(const value_type& item) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_full_condition.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
		m_empty_condition.notify_one();
		m_buffer.push_back(item);
		++m_unread;
	}
	
	value_type pop_front() {
		boost::mutex::scoped_lock lock(m_mutex);
		m_empty_condition.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
		m_full_condition.notify_one();
		return m_buffer[m_buffer.size() - (m_unread--)];
	}

private:
	
	bool is_not_empty() const { return m_unread > 0;}
	bool is_not_full() const { return m_unread < m_buffer.capacity();}
	
	size_type m_unread;
	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_empty_condition;
	boost::condition m_full_condition;
};

template <class T>
class deque_bounded_buffer {
public:

	typedef std::deque<T> buffer_type;	
	typedef typename buffer_type::size_type size_type;
	typedef typename buffer_type::value_type value_type;

	deque_bounded_buffer(size_type capacity) : m_capacity(capacity), m_buffer(capacity) {}
	
	void push_back(const value_type& item) {
		boost::mutex::scoped_lock lock(m_mutex);
		m_full_condition.wait(lock, boost::bind(&deque_bounded_buffer<value_type>::is_not_full, this));
		m_empty_condition.notify_one();
		m_buffer.push_back(item);
	}
	
	value_type pop_front() {
		boost::mutex::scoped_lock lock(m_mutex);
		m_empty_condition.wait(lock, boost::bind(&deque_bounded_buffer<value_type>::is_not_empty, this));
		m_full_condition.notify_one();
		value_type item = m_buffer.front();
		m_buffer.pop_front();
		return item;
	}

private:
	
	bool is_not_empty() const { return m_buffer.size() > 0; }
	bool is_not_full() const { return m_buffer.size() < m_capacity; }
	
	const size_type m_capacity;
	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_empty_condition;
	boost::condition m_full_condition;
};

template<typename Queue>
class reader {

	typedef typename Queue::value_type value_type;
    Queue& m_q;

public:
    reader(Queue& q) : m_q(q) { }
    
    void operator()() {
		for (int i = 0; i < total_elements; ++i) {
			value_type item = m_q.pop_front();
		}
    }
};

template<typename Queue>
class writer {

    typedef typename Queue::value_type value_type;
    Queue& m_q;

public:
    writer(Queue& q) : m_q(q) {}
    
    void operator()() {
		for (int i = 0; i < total_elements; ++i) {
			m_q.push_back(value_type());
		}
    }
};

template<typename Queue>
void fifo_test()
{
	boost::progress_timer t;

    Queue q(queue_size);
    
	reader<Queue> reader(q);
	writer<Queue> writer(q);

	boost::thread read(reader);
    boost::thread write(writer);
        
    read.join();
    write.join();
}

int main(int argc, char* argv[])
{
	std::cout << "bounded_buffer<int>: ";
    fifo_test< bounded_buffer<int> >();
    
    std::cout << "deque_bounded_buffer<int>: ";
    fifo_test< deque_bounded_buffer<int> >();

    std::cout << "bounded_buffer<string>: ";
    fifo_test< bounded_buffer<std::string> >();
    
    std::cout << "deque_bounded_buffer<string>: ";
    fifo_test< deque_bounded_buffer<std::string> >();

	return 0;
}
