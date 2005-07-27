// Debug support for the circular buffer library.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_DEBUG_HPP)
#define BOOST_CIRCULAR_BUFFER_DEBUG_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

namespace boost {

namespace cb_details {

#if BOOST_CB_ENABLE_DEBUG

// The value the uninitialized memory is filled with.
const int UNINITIALIZED = 0xcc;

class iterator_registry;

/*!
    \class iterator_base
    \brief Registers/unregisters iterators into the registry of valid iterators.

    This class is intended to be a base class of an iterator.
*/
class iterator_base {

private:
// Members

    //! Iterator registry.
    mutable const iterator_registry* m_registry;

    //! Next iterator in the iterator chain.
    mutable const iterator_base* m_next;

public:
// Construction/destruction

    //! Default constructor.
    iterator_base();

    //! Constructor taking the iterator registry as a parameter.
    iterator_base(const iterator_registry* registry);

    //! Copy constructor.
    iterator_base(const iterator_base& rhs);

    //! Destructor.
    ~iterator_base();

// Methods

    //! Assign operator.
    iterator_base& operator = (const iterator_base& rhs);

    //! Is the iterator valid?
    bool is_valid() const;

    //! Invalidate the iterator.
    /*!
        \note The method is const in order to invalidate const iterators, too.
    */
    void invalidate() const;

    //! Return the next iterator in the iterator chain.
    const iterator_base* next() const;

    //! Set the next iterator in the iterator chain.
    /*!
        \note The method is const in order to set a next iterator to a const iterator, too.
    */
    void set_next(const iterator_base* it) const;

private:
// Helpers

    //! Register self as a valid iterator.
    void register_self();

    //! Unregister self from valid iterators.
    void unregister_self();
};

/*!
    \class iterator_registry
    \brief Registry of valid iterators.

    This class is intended to be a base class of a container.
*/
class iterator_registry {

    //! Pointer to the chain of valid iterators.
    mutable const iterator_base* m_iterators;

public:
// Methods

    //! Default constructor.
    iterator_registry() : m_iterators(0) {}

    //! Register an iterator into the list of valid iterators.
    /*!
        \note The method is const in order to register iterators into const containers, too.
    */
    void register_iterator(const iterator_base* it) const {
        it->set_next(m_iterators);
        m_iterators = it;
    }

    //! Unregister an iterator from the list of valid iterators.
    /*!
        \note The method is const in order to unregister iterators from const containers, too.
    */
    void unregister_iterator(const iterator_base* it) const {
        const iterator_base* previous = 0;
        for (const iterator_base* p = m_iterators; p != it; previous = p, p = p->next());
        remove(it, previous);
    }

    //! Invalidate all iterators.
    void invalidate_all_iterators() {
        for (const iterator_base* p = m_iterators; p != 0; p = p->next())
            p->invalidate();
        m_iterators = 0;
    }

    //! Invalidate every iterator conforming to the condition.
    template <class Iterator>
    void invalidate_iterators(const Iterator& it) {
        const iterator_base* previous = 0;
        for (const iterator_base* p = m_iterators; p != 0; p = p->next()) {
            if (((Iterator*)p)->m_it == it.m_it) {
                p->invalidate();
                remove(p, previous);
                continue;
            }
            previous = p;
        }
    }

private:
// Helpers

    //! Remove the current iterator from the iterator chain.
    void remove(const iterator_base* current,
                const iterator_base* previous) const {
        if (previous == 0)
            m_iterators = m_iterators->next();
        else
            previous->set_next(current->next());
    }
};

// Implementation of the iterator_base methods.

inline iterator_base::iterator_base() : m_registry(0), m_next(0) {}

inline iterator_base::iterator_base(const iterator_registry* registry)
: m_registry(registry), m_next(0) {
    register_self();
}

inline iterator_base::iterator_base(const iterator_base& rhs)
: m_registry(rhs.m_registry), m_next(0) {
    register_self();
}

inline iterator_base::~iterator_base() { unregister_self(); }

inline iterator_base& iterator_base::operator = (const iterator_base& rhs) {
    if (m_registry == rhs.m_registry)
        return *this;
    unregister_self();
    m_registry = rhs.m_registry;
    register_self();
    return *this;
}

inline bool iterator_base::is_valid() const { return m_registry != 0; }

inline void iterator_base::invalidate() const { m_registry = 0; }

inline const iterator_base* iterator_base::next() const { return m_next; }

inline void iterator_base::set_next(const iterator_base* it) const { m_next = it; }

inline void iterator_base::register_self() {
    if (m_registry != 0)
        m_registry->register_iterator(this);
}

inline void iterator_base::unregister_self() {
    if (m_registry != 0)
        m_registry->unregister_iterator(this);
}

#else // #if BOOST_CB_ENABLE_DEBUG

class iterator_registry {
#if BOOST_WORKAROUND(__BORLANDC__, < 0x6000)
    char dummy_; // BCB: by default empty structure has 8 bytes
#endif
};

class iterator_base {
#if BOOST_WORKAROUND(__BORLANDC__, < 0x6000)
    char dummy_; // BCB: by default empty structure has 8 bytes
#endif

public:
    iterator_base() {}
    iterator_base(const iterator_registry*) {}
};

#endif // #if BOOST_CB_ENABLE_DEBUG

} // namespace cb_details

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_DEBUG_HPP)
