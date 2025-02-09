//
// Created by phatt on 2/9/25.
//
#pragma once

extern Engine::Application* Engine::CreateApplication();

int main()
{
    Engine::Log::Init();

    auto* app = Engine::CreateApplication();
    app->run();
    delete app;
    return 0;
}
