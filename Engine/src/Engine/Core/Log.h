//
// Created by phatt on 2/9/25.
//
#pragma once

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

