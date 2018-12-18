#include <boost/circular_buffer.hpp>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

int main(int argc, char** argv)
{
    int buffer_size = 100000;

    if (argc == 2) {
        buffer_size = std::atoi(argv[1]);
        if (buffer_size <= 0) {
            std::cout << "invalid buffer size" << std::endl;
            return 1;
        }
    }

    // Seed with a real random value, if available
    std::random_device r;
 
    // Choose a randomly whether to push or pop
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uniform_dist(1, 10);

    {
        boost::circular_buffer<double> cb1(buffer_size);
        for (auto i = 0; i < buffer_size; ++i) {
            auto draw = uniform_dist(e);
            if (draw < 8 || cb1.empty())
                cb1.push_back(i);
            else
                cb1.pop_front();
        }

        {
            // in-memory storage 
            std::stringstream ss;

            boost::archive::text_oarchive oa(ss);
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            boost::serialization::serialize(oa, cb1, 0);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "in-memory serialization for buffer: " << buffer_size << " took: " << 
                std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;

            boost::circular_buffer<double> cb2(0);
            boost::archive::text_iarchive ia(ss);
            begin = std::chrono::steady_clock::now();
            boost::serialization::serialize(ia, cb2, 0);
            end = std::chrono::steady_clock::now();
            std::cout << "in-memory deserialization for buffer: " << buffer_size << " took: " << 
                std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;

            if (cb1 != cb2) {
                std::cout << "circular buffer did not recover correctly" << std::endl;
            }
        }

        {
            const std::string filename = "cb.tmp";
            // file storage
            {
                std::ofstream ofs(filename);

                boost::archive::text_oarchive oa(ofs);
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                boost::serialization::serialize(oa, cb1, 0);
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "file serialization for buffer: " << buffer_size << " took: " << 
                    std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;
            }
            {
                std::ifstream ifs(filename);

                boost::circular_buffer<double> cb2(0);
                boost::archive::text_iarchive ia(ifs);
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                boost::serialization::serialize(ia, cb2, 0);
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "file deserialization for buffer: " << buffer_size << " took: " << 
                    std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;

                if (cb1 != cb2) {
                    std::cout << "circular buffer did not recover correctly" << std::endl;
                }
            }
            std::remove(filename.c_str());
        }
    }

    {
        // space optimized
        boost::circular_buffer_space_optimized<double> cb1(buffer_size);
        for (auto i = 0; i < buffer_size; ++i) {
            auto draw = uniform_dist(e);
            if (draw < 8 || cb1.empty()) {
                cb1.push_back(i);
            } else {
                cb1.pop_front();
            }
        }

        {
            const std::string filename = "space_opt_cb.tmp";
            {
                std::ofstream ofs(filename);

                boost::archive::text_oarchive oa(ofs);
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                boost::serialization::serialize(oa, cb1, 0);
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "in-memory serialization for space optimized buffer: " << buffer_size << " took: " << 
                    std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;
            }

            {
                std::ifstream ifs(filename);

                boost::circular_buffer_space_optimized<double> cb2(0);
                boost::archive::text_iarchive ia(ifs);
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                boost::serialization::serialize(ia, cb2, 0);
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "in-memory deserialization for space optimized buffer: " << buffer_size << " took: " << 
                    std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " usec" << std::endl;

                if (cb1 != cb2) {
                    std::cout << "space optimized circular buffer did not recover correctly" << std::endl;
                }
            }
        }
    }

    return 0;
}

