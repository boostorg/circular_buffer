// Implementation of circular_buffer serialization

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/circular_buffer.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/smart_ptr/scoped_array.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace boost { namespace serialization {
template <class Archive, class T>
void save(Archive& ar, const circular_buffer<T>& b, const unsigned int /* version */)
{
    ar << b.capacity();
    ar << b.size();
    const typename circular_buffer<T>::const_iterator it_end = b.end();
    typename circular_buffer<T>::const_iterator it = b.begin();
    for (; it != it_end; ++it) {
        ar << *it;
    }
}

template <class T>
void save(archive::binary_oarchive& ar, const circular_buffer<T>& b, const unsigned int /* version */)
{
    ar << b.capacity();
    const typename circular_buffer<T>::const_array_range& one = b.array_one();
    const typename circular_buffer<T>::const_array_range& two = b.array_two();
    ar << one.second;
    ar << two.second;
    if (one.second) {
        ar.save_binary(one.first, one.second*sizeof(T));
    }
    if (two.second) {
        ar.save_binary(two.first, two.second*sizeof(T));
    }
}

template <class Archive, class T>
void load(Archive& ar, circular_buffer<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer<T>::capacity_type capacity;
    ar >> capacity;
    b.set_capacity(capacity);
    typename circular_buffer<T>::size_type size;
    ar >> size;
    while (size > 0) {
        T e;
        ar >> e;
        b.push_back(e);
        --size;
    }
}

template <class T>
void load(archive::binary_iarchive& ar, circular_buffer<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer<T>::capacity_type capacity;
    ar >> capacity;
    b.set_capacity(capacity);
    typename circular_buffer<T>::size_type size1;
    typename circular_buffer<T>::size_type size2;
    ar >> size1;
    ar >> size2;
    const scoped_array<typename circular_buffer<T>::value_type> buff(new T[size1+size2]);
    if (size1) {
        ar.load_binary(buff.get(), size1*sizeof(T));
    }
    if (size2) {
        ar.load_binary(&buff[size1], size2*sizeof(T));
    }
    b.insert(b.begin(), buff.get(), buff.get()+size1+size2);
}

template<class Archive, class T>
inline void serialize(Archive & ar, circular_buffer<T>& b, const unsigned int version)
{
    split_free(ar, b, version);
}

template <class Archive, class T>
void save(Archive& ar, const circular_buffer_space_optimized<T>& b, const unsigned int /* version */)
{
    ar << b.capacity().capacity();
    ar << b.capacity().min_capacity();
    ar << b.size();
    const typename circular_buffer_space_optimized<T>::const_iterator it_end = b.end();
    typename circular_buffer_space_optimized<T>::const_iterator it = b.begin();
    for (; it != it_end; ++it) {
        ar << *it;
    }
}

template <class T>
void save(archive::binary_oarchive& ar, const circular_buffer_space_optimized<T>& b, const unsigned int /* version */)
{
    ar << b.capacity().capacity();
    ar << b.capacity().min_capacity();
    const typename circular_buffer_space_optimized<T>::const_array_range one = b.array_one();
    const typename circular_buffer_space_optimized<T>::const_array_range two = b.array_two();
    ar << one.second;
    ar << two.second;
    if (one.second) {
        ar.save_binary(one.first, one.second*sizeof(T));
    }
    if (two.second) {
        ar.save_binary(two.first, two.second*sizeof(T));
    }
}

template <class Archive, class T>
void load(Archive& ar, circular_buffer_space_optimized<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer_space_optimized<T>::size_type capacity;
    typename circular_buffer_space_optimized<T>::size_type min_capacity;
    ar >> capacity;
    ar >> min_capacity;
    const typename circular_buffer_space_optimized<T>::capacity_type capacity_control(capacity, min_capacity);
    b.set_capacity(capacity_control);
    typename circular_buffer<T>::size_type size;
    ar >> size;
    while (size > 0) {
        T e;
        ar >> e;
        b.push_back(e);
        --size;
    }
}

template <class T>
void load(archive::binary_iarchive& ar, circular_buffer_space_optimized<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer_space_optimized<T>::size_type capacity;
    typename circular_buffer_space_optimized<T>::size_type min_capacity;
    ar >> capacity;
    ar >> min_capacity;
    const typename circular_buffer_space_optimized<T>::capacity_type capacity_control(capacity, min_capacity);
    b.set_capacity(capacity_control);
    typename circular_buffer<T>::size_type size1;
    typename circular_buffer<T>::size_type size2;
    ar >> size1;
    ar >> size2;
    const scoped_array<typename circular_buffer<T>::value_type> buff(new T[size1+size2]);
    if (size1) {
        ar.load_binary(buff.get(), size1*sizeof(T));
    }
    if (size2) {
        ar.load_binary(&buff[size1], size2*sizeof(T));
    }
    b.insert(b.begin(), buff.get(), buff.get()+size1+size2);
}

template<class Archive, class T>
inline void serialize(Archive & ar, circular_buffer_space_optimized<T>& b, const unsigned int version)
{
    split_free(ar, b, version);
}

} } // end namespace boost::serialization

