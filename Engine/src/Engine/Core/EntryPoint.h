//
// Created by phatt on 2/9/25.
//
#pragma once

/*  Include this file in your project where the entry point of the applicatoin should be.
    This file can be included only once. */

//////////////////////////////////////////////////////////////////////////
//// Ignore the errors! //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Implemented by the client.
extern Engine::Application* Engine::CreateApplication();

int main()
{
    auto currentWorkingDirectory = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentWorkingDirectory.string() << std::endl;

    Engine::Log::Init();
    EG_CORE_WARN("Initialized Log.", 0);

    auto* app = Engine::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
