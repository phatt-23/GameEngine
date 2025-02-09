//
// Created by phatt on 2/9/25.
//

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        // Color timestamp name-of-logger message
        spdlog::set_pattern("%^[%T] [%n]: %v%$");

        s_CoreLogger = spdlog::stdout_color_mt("Engine");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("App");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}
