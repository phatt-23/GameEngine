#include "Engine.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include <glm/glm.hpp>

class ExampleLayer : public Engine::Layer
{
public:
    ExampleLayer()
        : Layer("Layer") {}

    void OnUpdate() override
    {
        if (Engine::Input::IsKeyPressed(EG_KEY_TAB))
        {
            EG_INFO("Tab key is pressed!");
        }

        auto model = glm::mat4(1.0f);

        EG_CORE_DEBUG("{}", model[0][0]);
    }

    void OnEvent(Engine::Event& event) override
    {
    }
};


class Sandbox : public Engine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Engine::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};


Engine::Application *Engine::CreateApplication() {
    return new Sandbox;
}

