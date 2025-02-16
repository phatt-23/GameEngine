//
// Created by phatt 02/14/2025
//
#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Layer"), m_CameraController(1.0f / 2.0f, true) 
{
}

void Sandbox2D::OnAttach() 
{
    m_Texture = Engine::Texture2D::Create("assets/textures/ISO_C++_Logo.png");
}

void Sandbox2D::OnUpdate(Engine::Timestep ts) 
{
    EG_PROFILE_FUNCTION();

    Engine::RenderCommand::SetClearColor({0.1, 0.1, 0.1, 0.1});
    Engine::RenderCommand::Clear();

    m_CameraController.OnUpdate(ts);

    Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
    {
        Engine::Renderer2D::DrawQuad(m_QuadPosition, m_QuadSize, m_QuadColor);
        Engine::Renderer2D::DrawQuad(m_QuadPosition - 0.5f, m_QuadSize - 0.2f, m_QuadColor * 0.4f);
        Engine::Renderer2D::DrawQuad(m_QuadPosition + 0.5f, m_QuadSize, m_Texture);
    }
    Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() 
{
    EG_PROFILE_FUNCTION();

    ImGui::Begin("Sandbox2D");
    ImGui::SliderFloat3("Quad Position", glm::value_ptr(m_QuadPosition), -10.0f, 10.0f);
    ImGui::SliderFloat2("Quad Size", glm::value_ptr(m_QuadSize), 0.1f, 10.0f);
    ImGui::ColorPicker4("Quad Color", glm::value_ptr(m_QuadColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event &event) 
{
    m_CameraController.OnEvent(event);
}

