//
// Created by phatt 02/14/2025
//

#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "Sandbox2D.h"

#include <filesystem>

class SandboxApp : public Engine::Application
{
public:
    SandboxApp()
    {
        PushLayer(new Sandbox2D());
    }

    ~SandboxApp() override = default;
};


Engine::Application* Engine::CreateApplication()
{
    return new SandboxApp;
}

