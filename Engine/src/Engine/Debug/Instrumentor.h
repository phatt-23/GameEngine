//
// Created by phatt 02/15/2025
//
#pragma once

#include "Debug/Assert.h"

namespace Engine
{

    struct ProfileInfo
    {
        std::string Name;
        long long Start;
        long long End;
        std::uint32_t ThreadID;
    };


    struct InstrumentorSession
    {
        std::string Name;
    };


    class Instrumentor
    {
    private:
        inline Instrumentor() 
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {
        }

    public:
        inline ~Instrumentor()
        {
            if (m_CurrentSession) 
                delete m_CurrentSession;
        }

        static Instrumentor& Get()
        {
            static Instrumentor* instance = new Instrumentor();
            return *instance;
        }
        
        inline void BeginSession(const std::string& name, const std::string& filepath = "ProfileResult.json")
        {
            EG_CORE_ASSERT(m_CurrentSession == nullptr, "Session begin called while still in an active session.");
            m_CurrentSession = new InstrumentorSession(name);

            // Write header.
            m_OutputStream.open(filepath);
            m_OutputStream << R"({"otherData": {},)"; 
            m_OutputStream << R"("traceEvents": [)";
            m_OutputStream.flush();
        }

        inline void EndSession()
        {
            EG_CORE_ASSERT(m_CurrentSession != nullptr, "No session to end.");
            delete m_CurrentSession;
            m_CurrentSession = nullptr;

            // Write footer.
            m_OutputStream << "]";
            m_OutputStream << "}";
            m_OutputStream.flush();
            m_OutputStream.close();
            m_ProfileCount = 0;
        }

        inline void WriteProfile(const ProfileInfo& profile)
        {
            if (m_ProfileCount++ > 0)
            {
                m_OutputStream << ",";
            }

            std::string name = profile.Name;
            std::replace(name.begin(), name.end(), '"', '\"');

            m_OutputStream << R"({)";
            m_OutputStream << R"("cat": "function",)";
            m_OutputStream << std::format(R"("name": "{}",)", name);
            m_OutputStream << std::format(R"("ts": "{}",)", profile.Start);
            m_OutputStream << std::format(R"("dur": "{}",)", profile.End - profile.Start);
            m_OutputStream << R"("ph": "X",)";
            m_OutputStream << R"("pid": "0",)";
            m_OutputStream << std::format(R"("tid": "{}")", profile.ThreadID);
            m_OutputStream << R"(})";

            m_OutputStream.flush();
        }

    private:
        InstrumentorSession* m_CurrentSession;
        unsigned int m_ProfileCount;
        std::ofstream m_OutputStream;
    };



    class InstrumentorTimer 
    {
    public:
        inline InstrumentorTimer(const std::source_location& location = std::source_location::current())
            : m_Stopped(false), m_StopCallback({})
        {
            m_Name = std::format("In '{}' at `{}:{}:{}`", location.file_name(), location.function_name(), location.line(), location.column());
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        inline InstrumentorTimer(std::optional<std::function<void(void)>>&& stopCallback, const std::source_location& location = std::source_location::current())
            : m_Stopped(false), m_StopCallback(stopCallback)
        {
            m_Name = std::format("In '{}' at `{}:{}:{}`", location.file_name(), location.function_name(), location.line(), location.column());
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        inline InstrumentorTimer(std::string&& name)
            : m_Name(name), m_Stopped(false), m_StopCallback({})
        {
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        inline InstrumentorTimer(std::string&& name, std::optional<std::function<void(void)>>&& stopCallback)
            : m_Name(name), m_Stopped(false), m_StopCallback(stopCallback)
        {
            m_StartTimepoint = std::chrono::steady_clock::now();
        }


        inline ~InstrumentorTimer()
        {
            if (!m_Stopped) 
                Stop();
        }

        inline void Stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

            std::uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });        

            if (m_StopCallback) 
                m_StopCallback.value()();

            m_Stopped = true;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        std::string m_Name;
        bool m_Stopped;
        std::optional<std::function<void(void)>> m_StopCallback;
    };

}


#define EG_CONCATENATE_DETAIL(x, y)         x##y
#define EG_CONCATENATE(x, y)                EG_CONCATENATE_DETAIL(x, y)
#define EG_SCOPE_UNIQUE_VAR_NAME(base)      EG_CONCATENATE(base, __COUNTER__)


#if ENGINE_ENABLE_PROFILING
    #define EG_PROFILE_BEGIN_SESSION(name, filepath)    ::Engine::Instrumentor::Get().BeginSession(name, filepath)
    #define EG_PROFILE_END_SESSION()                    ::Engine::Instrumentor::Get().EndSession()
    #define EG_PROFILE_SCOPE(name)                      ::Engine::InstrumentorTimer EG_SCOPE_UNIQUE_VAR_NAME(__timer)(name)
    #define EG_PROFILE_FUNCTION()                       EG_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define EG_PROFILE_BEGIN_SESSION(name, filepath)
    #define EG_PROFILE_END_SESSION()
    #define EG_PROFILE_SCOPE()
    #define EG_PROFILE_FUNCTION()
#endif


