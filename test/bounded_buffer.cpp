
#include <boost/circular_buffer.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/bind.hpp>
#include <iostream>

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
		m_buffer.push_back(item);
		++m_unread;
		m_empty_condition.notify_one();
	}
	
	value_type pop_front() {
		boost::mutex::scoped_lock lock(m_mutex);
		m_empty_condition.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
		value_type& item = m_buffer[m_buffer.size() - (m_unread--)];
		m_full_condition.notify_one();
		return item;
	}

private:
	
	bool is_not_empty() const { return m_unread > 0; }
	bool is_not_full() const { return m_unread < m_buffer.capacity(); }
	
	size_type m_unread;
	buffer_type m_buffer;
	boost::mutex m_mutex;
	boost::condition m_empty_condition;
	boost::condition m_full_condition;
};

void go_sleep(unsigned int sec) {
	boost::xtime t;
	if (boost::TIME_UTC != boost::xtime_get(&t, boost::TIME_UTC))
		throw boost::unsupported_thread_option();
	t.sec += sec;
	boost::thread::sleep(t);
}

void produce(bounded_buffer<int>* buffer) {
	go_sleep(1);
	std::cout << "producer thread: will write first 50 items";
	std::cout << std::endl << std::endl;
	int i = 1;
	for (; i <= 50; ++i) {
		buffer->push_back(i);
	}
	go_sleep(1);
	std::cout << std::endl << std::endl;
	std::cout << "producer thread: will wait for 5 seconds and then write another 50 items" << std::endl;
	std::cout << std::endl << std::endl;
	go_sleep(5);
	for (; i <= 100; ++i) {
		buffer->push_back(i);
	}
}

void consume(bounded_buffer<int>* buffer) {
	std::cout << "consumer thread: will wait for 5 seconds" << std::endl;
	go_sleep(5);
	for (int i = 1; i <= 100; ++i) {
		std::cout << buffer->pop_front() << ' ';
	}
	std::cout << std::endl << std::endl;
	std::cout << "consumer thread: just have finished reading all 100 items";
	std::cout << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
	bounded_buffer<int> buffer(10);

	boost::thread consumer(boost::bind(&consume, &buffer));
	boost::thread producer(boost::bind(&produce, &buffer));

	consumer.join();
	producer.join();

	return 0;
}
