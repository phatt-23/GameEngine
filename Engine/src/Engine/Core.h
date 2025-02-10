//
// Created by phatt on 2/9/25.
//
#pragma once

#include "Log.h"


/// Bit-field
#define BIT(x) (1 << (x))


/// Assertions
#ifdef ENGINE_ENABLE_ASSERTS
    /// __builtin_trap is an GCC intrinsic.
    #define EG_CORE_ASSERT(x, ...) {                                \
        if (!(x)) {                                                 \
            EG_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);    \
            __builtin_trap();                                       \
        }                                                           \
    }
    /// __builtin_trap is an GCC intrinsic.
    #define EG_ASSERT(x, ...) {                                     \
        if (!(x)) {                                                 \
            EG_ERROR("Assertion failed: {0}", __VA_ARGS__);         \
            __builtin_trap();                                       \
        }                                                           \
    }
#else
    #define EG_CORE_ASSERT(x, ...)
    #define EG_ASSERT(x, ...)
#endif


#define EG_FUNCTION_CURRYING_USE_LAMBDA

/// Create curried member-function with current object instance bounded.
#ifdef EG_FUNCTION_CURRYING_USE_LAMBDA
    #define EG_FORWARD_EVENT_TO_MEM_FN(fn) ([this](auto&& e) -> bool { return fn(e); })
#else
    #define EG_FORWARD_EVENT_TO_MEM_FN(fn) (std::bind(&(fn), this, std::placeholders::_1))
#endif
