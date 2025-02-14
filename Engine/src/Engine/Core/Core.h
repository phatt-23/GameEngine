//
// Created by phatt on 2/9/25.
//
#pragma once


#ifdef ENGINE_ENABLE_LOGGING
    #include "Core/Log.h"
#endif


/// Assertions
#if defined(ENGINE_ENABLE_ASSERTS)
    #include <source_location>
    #include <stacktrace>
    #include <sstream>

    /// __builtin_trap is an GCC intrinsic.
    #define EG_CORE_ASSERT(x, ...) {                                                \
        if (!(x)) {                                                                 \
            auto loc = std::source_location::current();                             \
            EG_CORE_ERROR("Assertion failed at: {}({}:{}) `{}`!",                   \
                loc.file_name(), loc.line(), loc.column(), loc.function_name());    \
            EG_CORE_ERROR(__VA_ARGS__);                                             \
            __builtin_trap();                                                       \
        }                                                                           \
    }

    /// __builtin_trap is an GCC intrinsic.
    #define EG_ASSERT(x, ...) {                                                     \
        if (!(x)) {                                                                 \
            auto loc = std::source_location::current();                             \
            EG_ERROR("Assertion failed at: {}({}:{}) `{}`!",                        \
                loc.file_name(), loc.line(), loc.column(), loc.function_name());    \
            EG_ERROR(__VA_ARGS__);                                                  \
            __builtin_trap();                                                       \
        }                                                                           \
    }
#else 
    #define EG_CORE_ASSERT(x, ...)
    #define EG_ASSERT(x, ...)
#endif


/// Bit-field
#define BIT(x) (1 << (x))


/// Create curried member-function with current object instance bounded.
#ifdef ENGINE_FUNCTION_CURRYING_USE_LAMBDA
    #define EG_FORWARD_EVENT_TO_MEM_FN(fn) ([this](auto&& e) -> bool { return fn(std::forward<decltype(e)>(e)); })
#else
    #define EG_FORWARD_EVENT_TO_MEM_FN(fn) (std::bind(&(fn), this, std::placeholders::_1))
#endif


namespace Engine
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
}

#define EG_IM_UNUSED(_PARAM) ((void)_PARAM)


