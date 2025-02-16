//
// Created by phatt on 2/9/25.
//
#pragma once

#include "Core/Core.h"
#include "Engine.h"

/*  Include this file in your project where the entry point of the applicatoin should be.
    This file can be included only once. */

//////////////////////////////////////////////////////////////////////////
//// Ignore the errors! //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Implemented by the client.
extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
    EG_IM_UNUSED(argc);
    EG_IM_UNUSED(argv);

    Engine::Log::Init();
    EG_CORE_WARN("Current working directory: '{}'", std::filesystem::current_path().string());
    EG_CORE_WARN("Initialized Log.", 0);

    EG_PROFILE_BEGIN_SESSION("Application creation", "Engine-ApplicationCreation.json");
    auto* app = Engine::CreateApplication();
    EG_PROFILE_END_SESSION();

    EG_PROFILE_BEGIN_SESSION("Application creation", "Engine-ApplicationRun.json");
    app->Run();
    EG_PROFILE_END_SESSION();

    EG_PROFILE_BEGIN_SESSION("Application deletion", "Engine-ApplicationDeletion.json");
    delete app;
    EG_PROFILE_END_SESSION();

    return 0;
}
