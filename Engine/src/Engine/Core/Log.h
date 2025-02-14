//
// Created by phatt on 2/9/25.
//
#pragma once

#include "Core/Core.h"
#include <spdlog/spdlog.h>
#include <memory>

namespace Engine
{

    class Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

#if ENGINE_ENABLE_LOGGING
    #define EG_CORE_TRACE(...)      ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define EG_CORE_DEBUG(...)      ::Engine::Log::GetCoreLogger()->debug(__VA_ARGS__)
    #define EG_CORE_INFO(...)       ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define EG_CORE_WARN(...)       ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define EG_CORE_ERROR(...)      ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define EG_CORE_CRITICAL(...)   ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)
    
    #define EG_TRACE(...)           ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define EG_DEBUG(...)           ::Engine::Log::GetClientLogger()->debug(__VA_ARGS__)
    #define EG_INFO(...)            ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
    #define EG_WARN(...)            ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define EG_ERROR(...)           ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
    #define EG_CRITICAL(...)        ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
    #define EG_CORE_TRACE(...)     
    #define EG_CORE_DEBUG(...)     
    #define EG_CORE_INFO(...)      
    #define EG_CORE_WARN(...)      
    #define EG_CORE_ERROR(...)     
    #define EG_CORE_CRITICAL(...)  
    
    #define EG_TRACE(...)     
    #define EG_DEBUG(...)     
    #define EG_INFO(...)      
    #define EG_WARN(...)      
    #define EG_ERROR(...)     
    #define EG_CRITICAL(...)  
#endif


// Dynamic string formatting (C++ formatting is static - const at compile time)
// Is in here because it is mostly used with the logging functions.
#define EG_DYN_FORMAT(...)      ::Engine::DynamicFormat::DynamicStringFormat(std::source_location::current(), __VA_ARGS__)
#define EG_DYN_FORMATLN(...)    ::Engine::DynamicFormat::DynamicStringFormatLn(std::source_location::current(), __VA_ARGS__)
#define EG_FORMAT_STRING(...)   ::Engine::DynamicFormat::ConstructFormatString(std::source_location::current(), __VA_ARGS__)

namespace Engine
{
    class DynamicFormat
    {
    public:
        template<typename... Args>
        inline static std::string ConstructFormatString(const std::source_location& location, Args&&... args)
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

