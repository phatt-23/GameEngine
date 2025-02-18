//
// Created by phatt 02/15/2025
//
#pragma once

#include "Core/Log.h"
#include <source_location>

namespace Engine
{
    class Assert
    {
    public:
        Assert() = default;
        ~Assert() = default;
        
        inline static const Assert& Get() 
        { 
            static Assert* s_Instance = new Assert;
            return *s_Instance; 
        }

        template<typename... Args>
        bool CoreAssert(std::source_location&& location, std::string&& expression, bool result, const std::string& format, Args&&... args) const
        {
            if (result) return true;
            std::string message = std::vformat(format, std::make_format_args(args...));
            EG_CORE_ERROR("Assertion `{}` failed in file: '{}' at: `{}:{}:{}` with message: {}", 
                          expression, 
                          location.file_name(), 
                          location.function_name(), 
                          location.line(), 
                          location.column(), 
                          message);
            return false;
        }

        template<typename... Args>
        bool ClientAssert(std::source_location&& location, std::string&& expression, bool result, const std::string& format, Args&&... args) const
        {
            if (result) return true;
            std::string message = std::vformat(format, std::make_format_args(args...));
            EG_ERROR("Assertion `{}` failed in file: '{}' at: `{}:{}:{}` with message: {}", 
                     expression, 
                     location.file_name(), 
                     location.function_name(), 
                     location.line(), 
                     location.column(), 
                     message);
            return false;
        }

        template<typename... Args>
        bool CoreAssert(std::source_location&& location, std::string&& expression, bool result) const
        {
            if (result) return true;
            EG_CORE_ERROR("Assertion `{}` failed in file: '{}' at: `{}:{}:{}`!", 
                          expression, 
                          location.file_name(), 
                          location.function_name(), 
                          location.line(), 
                          location.column());
            return false;
        }

        template<typename... Args>
        bool ClientAssert(std::source_location&& location, std::string&& expression, bool result) const
        {
            if (result) return true;
            EG_ERROR("Assertion `{}` failed in file: '{}' at: `{}:{}:{}`!",
                     expression, 
                     location.file_name(), 
                     location.function_name(), 
                     location.line(), 
                     location.column());
            return false;
        }
    };
}

#if ENGINE_ENABLE_ASSERTS
    #define EG_CORE_ASSERT(_EXPR, ...)     { if ( ! ::Engine::Assert::Get().CoreAssert( std::source_location::current(), #_EXPR, _EXPR __VA_OPT__(, __VA_ARGS__) ) ) __builtin_trap(); }
    #define EG_ASSERT(_EXPR, ...)          { if ( ! ::Engine::Assert::Get().ClientAssert(std::source_location::current(), #_EXPR, _EXPR __VA_OPT__(, __VA_ARGS__) ) ) __builtin_trap(); }
#else 
    #define EG_CORE_ASSERT(_EXPR, ...)
    #define EG_ASSERT(_EXPR, ...)
#endif

