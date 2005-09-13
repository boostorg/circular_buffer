// Circular buffer library header file.

// Copyright (c) 2003-2005 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_HPP)
#define BOOST_CIRCULAR_BUFFER_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include "circular_buffer_fwd.hpp"
#include <boost/detail/workaround.hpp>

// BOOST_CB_ENABLE_DEBUG - debug support control.
#if defined(NDEBUG) || defined(BOOST_DISABLE_CB_DEBUG)
    #define BOOST_CB_ENABLE_DEBUG 0
#else
    #define BOOST_CB_ENABLE_DEBUG 1
#endif

// BOOST_CB_ASSERT - runtime assertion.
#if BOOST_CB_ENABLE_DEBUG
    #include <boost/assert.hpp>
    #define BOOST_CB_ASSERT(Expr) BOOST_ASSERT(Expr)
#else
    #define BOOST_CB_ASSERT(Expr) ((void)0)
#endif

// BOOST_CB_STATIC_ASSERT - compile time assertion.
#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
    #define BOOST_CB_STATIC_ASSERT(Expr) ((void)0)
#else
    #include <boost/static_assert.hpp>
    #define BOOST_CB_STATIC_ASSERT(Expr) BOOST_STATIC_ASSERT(Expr)
#endif

// BOOST_CB_IS_CONVERTIBLE - check if Iterator::value_type is convertible to Type.
#if BOOST_WORKAROUND(__BORLANDC__, <= 0x0550) || BOOST_WORKAROUND(__MWERKS__, <= 0x2407) || BOOST_WORKAROUND(BOOST_MSVC, < 1300)
    #define BOOST_CB_IS_CONVERTIBLE(Iterator, Type) ((void)0)
#else
    #include <boost/detail/iterator.hpp>
    #include <boost/type_traits/is_convertible.hpp>
    #define BOOST_CB_IS_CONVERTIBLE(Iterator, Type) \
        BOOST_CB_STATIC_ASSERT((is_convertible<typename detail::iterator_traits<Iterator>::value_type, Type>::value))
#endif

// BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS - check if the STL
// provides templated iterator constructors for its containers.
#if defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
    #define BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS BOOST_CB_STATIC_ASSERT(false);
#else
    #define BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS ((void)0);
#endif

// BOOST_CB_TRY and BOOST_CB_UNWIND - exception handling.
#if defined(BOOST_NO_EXCEPTIONS)
    #define BOOST_CB_TRY {
    #define BOOST_CB_UNWIND(action) }
#else
    #define BOOST_CB_TRY try {
    #define BOOST_CB_UNWIND(action) } catch(...) { action; throw; }
#endif

#include "circular_buffer/debug.hpp"
#include "circular_buffer/details.hpp"
#include "circular_buffer/base.hpp"
#include "circular_buffer/space_optimized.hpp"

#undef BOOST_CB_UNWIND
#undef BOOST_CB_TRY
#undef BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS
#undef BOOST_CB_IS_CONVERTIBLE
#undef BOOST_CB_STATIC_ASSERT
#undef BOOST_CB_ASSERT
#undef BOOST_CB_ENABLE_DEBUG

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_HPP)
