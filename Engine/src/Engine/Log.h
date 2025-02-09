//
// Created by phatt on 2/9/25.
//
#pragma once

#include "EnginePCH.h"
#include "Core.h"
#include <spdlog/spdlog.h>

namespace Engine
{


    class Log {
    public:
        static void Init();

        [[nodiscard]]
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }

        [[nodiscard]]
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };


}
