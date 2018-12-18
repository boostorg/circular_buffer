// boost::circular_buffer serialization

#include <boost/circular_buffer.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/smart_ptr/scoped_array.hpp>

namespace boost { namespace serialization {
template <class Archive, class T>
void save(Archive& ar, const circular_buffer<T>& b, const unsigned int /* version */)
{
    ar << b.capacity();
    ar << b.size();
    const typename circular_buffer<T>::const_array_range one = b.array_one();
    const typename circular_buffer<T>::const_array_range two = b.array_two();
    ar.save_binary(one.first, one.second*sizeof(T));
    ar.save_binary(two.first, two.second*sizeof(T));
}

template <class Archive, class T>
void load(Archive& ar, circular_buffer<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer<T>::capacity_type capacity;
    typename circular_buffer<T>::size_type size;
    ar >> capacity;
    ar >> size;
    b.set_capacity(capacity);
    const scoped_array<typename circular_buffer<T>::value_type> buff(new T[size]);
    ar.load_binary(buff.get(), size*sizeof(T));
    b.insert(b.begin(), buff.get(), buff.get()+size);
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
    const typename circular_buffer_space_optimized<T>::const_array_range one = b.array_one();
    const typename circular_buffer_space_optimized<T>::const_array_range two = b.array_two();
    ar.save_binary(one.first, one.second*sizeof(T));
    ar.save_binary(two.first, two.second*sizeof(T));
}

template <class Archive, class T>
void load(Archive& ar, circular_buffer_space_optimized<T>& b, const unsigned int /* version */)
{
    b.clear();
    typename circular_buffer_space_optimized<T>::size_type capacity;
    typename circular_buffer_space_optimized<T>::size_type min_capacity;
    typename circular_buffer_space_optimized<T>::size_type size;
    ar >> capacity;
    ar >> min_capacity;
    ar >> size;
    const typename circular_buffer_space_optimized<T>::capacity_type capacity_control(capacity, min_capacity);
    b.set_capacity(capacity_control);
    const scoped_array<typename circular_buffer_space_optimized<T>::value_type> buff(new T[size]);
    ar.load_binary(buff.get(), size*sizeof(T));
    b.insert(b.begin(), buff.get(), buff.get()+size);
}

template<class Archive, class T>
inline void serialize(Archive & ar, circular_buffer_space_optimized<T>& b, const unsigned int version)
{
    split_free(ar, b, version);
}

} } // end namespace boost::serialization

