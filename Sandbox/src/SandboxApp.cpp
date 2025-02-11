#include "Engine.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include <glm/glm.hpp>

class ExampleLayer : public Engine::Layer
{
public:
    ExampleLayer()
        : Layer("Layer") {}

    ~ExampleLayer() override = default;

    void OnUpdate() override
    {
    }

    void OnImGuiRender() override
    {
        static float f = 0;
        ImGui::InputFloat("float", &f);
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
    }

    ~Sandbox() override
    = default;
};


Engine::Application* Engine::CreateApplication()
{
    return new Sandbox;
}

