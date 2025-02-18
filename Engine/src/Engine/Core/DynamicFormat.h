//
// Created by phatt on 2/15/25.
//
#pragma once

#include "Core/Log.h"

namespace Engine
{
    /// Provides dynamic string formatting (C++ formatting is static - const at compile time)
    class DynamicFormat
    {
    public:
        template<typename... Args>
        inline static std::string ConstructFormatString(Args&&... args)
        {
            std::stringstream formatString;
            (formatString << ... << args);
            return formatString.str();
        }

        template<typename... Args>
        inline static std::string DynamicStringFormat(const std::source_location& location, const std::string_view& formatString, Args&&... args)
        {
            std::string result;
            try 
            {
                result = std::vformat(formatString, std::make_format_args(args...));
            } 
            catch (const std::exception& error) 
            {
                EG_CORE_ERROR("Error thrown at: `{}:{}:{}`", location.function_name(), location.line(), location.column());
                EG_CORE_ERROR("String formatter failed: {}", error.what());
            }
            return result;
        }

        template<typename... Args>
        inline static std::string DynamicStringFormatLn(const std::source_location& location, const std::string_view& formatString, Args&&... args)
        {
            return DynamicStringFormat(location, formatString, std::forward<Args>(args)...) + "\n";
        }

        template<typename... Args>
        inline static std::string DynamicStringFormat(const std::source_location& location, const std::stringstream& formatStringStream, Args&&... args)
        {
            return DynamicStringFormat(location, formatStringStream.str(), std::forward<Args>(args)...);
        }

        template<typename... Args>
        inline static std::string DynamicStringFormatLn(const std::source_location& location, const std::stringstream& formatStringStream, Args&&... args)
        {
            return DynamicStringFormat(location, formatStringStream.str(), std::forward<Args>(args)...) + "\n";
        }
    };

}

#define EG_DYN_FORMAT(...)      ::Engine::DynamicFormat::DynamicStringFormat(std::source_location::current(), __VA_ARGS__)
#define EG_DYN_FORMATLN(...)    ::Engine::DynamicFormat::DynamicStringFormatLn(std::source_location::current(), __VA_ARGS__)
#define EG_FORMAT_STRING(...)   ::Engine::DynamicFormat::ConstructFormatString(__VA_ARGS__)


