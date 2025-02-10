//
// Created by phatt on 2/9/25.
//
#pragma once

extern Engine::Application* Engine::CreateApplication();

int main()
{
    Engine::Log::Init();
    EG_CORE_WARN("Initialized Log.", 0);

    auto* app = Engine::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
