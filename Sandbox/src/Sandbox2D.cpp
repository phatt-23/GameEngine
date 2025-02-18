//
// Created by phatt 02/14/2025
//
#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLTexture.h"

Sandbox2D::Sandbox2D()
    : Layer("Layer"), m_CameraController(1.0f / 2.0f, true) 
{
}

void Sandbox2D::OnAttach() 
{
    m_TextureLibrary.Add("Checkerboard",     Engine::Texture2D::Create("assets/textures/Checkerboard.png"));
    m_TextureLibrary.Add("C++",     Engine::Texture2D::Create("assets/textures/ISO_C++_Logo.png"));
    m_TextureLibrary.Add("Jirka",   Engine::Texture2D::Create("assets/textures/jirka.jpg"));
    m_TextureLibrary.Add("Bitch",   Engine::Texture2D::Create("assets/textures/bi.jpg"));
    m_TextureLibrary.Add("Bitch2",  Engine::Texture2D::Create("assets/textures/bi2.jpg"));
    m_TextureLibrary.Add("Brick",   Engine::Texture2D::Create("assets/textures/brick.png"));
    m_TextureLibrary.Add("Planks",  Engine::Texture2D::Create("assets/textures/planks.png"));
    m_TextureLibrary.Add("Spec",    Engine::Texture2D::Create("assets/textures/planksSpec.png"));
    m_TextureLibrary.Add("Window",  Engine::Texture2D::Create("assets/textures/blending_transparent_window.png"));
    m_TextureLibrary.Add("Top",     Engine::Texture2D::Create("assets/textures/skybox/top.jpg"));
    m_TextureLibrary.Add("Bottom",  Engine::Texture2D::Create("assets/textures/skybox/bottom.jpg"));
    m_TextureLibrary.Add("Left",    Engine::Texture2D::Create("assets/textures/skybox/left.jpg"));
    m_TextureLibrary.Add("Right",   Engine::Texture2D::Create("assets/textures/skybox/right.jpg"));
    m_TextureLibrary.Add("Front",   Engine::Texture2D::Create("assets/textures/skybox/front.jpg"));
    m_TextureLibrary.Add("Back",    Engine::Texture2D::Create("assets/textures/skybox/back.jpg"));
}

void Sandbox2D::OnUpdate(Engine::Timestep ts) 
{
    EG_PROFILE_FUNCTION();

    Engine::RenderCommand::SetClearColor({0.1, 0.1, 0.1, 0.1});
    Engine::RenderCommand::Clear();

    m_Timestep = ts;
    m_CameraController.OnUpdate(ts);

    Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
    {
        Engine::Renderer2D::DrawQuad({0.0f, 0.0f}, {10.0f, 10.0f}, m_TextureLibrary.Get("Checkerboard"), 20.0f);

        Engine::Renderer2D::DrawQuad({0.0f, 0.0f}, m_QuadSize, m_QuadColor);
        Engine::Renderer2D::DrawQuad({1.0f, 0.0f}, m_QuadSize, m_TextureLibrary.Get("Jirka"));

        // Engine::Renderer2D::DrawQuad({0.5f, 0.5f}, m_QuadSize, m_TextureLibrary.Get("Bitch"));
        // Engine::Renderer2D::DrawQuad({-0.5f, 0.5f}, m_QuadSize, m_TextureLibrary.Get("Bitch2"));
        Engine::Renderer2D::DrawQuad({0.5f, -0.5f}, m_QuadSize, m_TextureLibrary.Get("Brick"));
        Engine::Renderer2D::DrawQuad({-0.5f, -0.5f}, m_QuadSize, m_TextureLibrary.Get("Window"));

        Engine::Renderer2D::DrawQuad({1.0f, 1.0f}, m_QuadSize, m_TextureLibrary.Get("Top"));
        Engine::Renderer2D::DrawQuad({1.0f, -1.0f}, m_QuadSize, m_TextureLibrary.Get("Bottom"));
        Engine::Renderer2D::DrawQuad({-1.0f, 1.0f}, m_QuadSize, m_TextureLibrary.Get("Left"));
        Engine::Renderer2D::DrawQuad({-1.0f, -1.0f}, m_QuadSize, m_TextureLibrary.Get("Right"));

        Engine::Renderer2D::DrawQuad({2.0f, 2.0f}, m_QuadSize, m_TextureLibrary.Get("Front"));
        Engine::Renderer2D::DrawQuad({-2.0f, 2.0f}, m_QuadSize, m_TextureLibrary.Get("Back"));
        Engine::Renderer2D::DrawQuad({2.0f, -2.5f}, m_QuadSize, m_TextureLibrary.Get("Planks"));
        Engine::Renderer2D::DrawQuad({-2.0f, -2.0f}, m_QuadSize, m_TextureLibrary.Get("Spec"));

        Engine::Renderer2D::DrawRotatedQuad(m_QuadPosition + glm::vec3(3.0f, 0.0f, 0.0f), m_QuadSize, glm::radians(30.0f), m_TextureLibrary.Get("C++"));


        constexpr float edgeValue = 8.0f;
        constexpr float quadPerRow = 32.0f;
        constexpr float step = edgeValue / quadPerRow;

        for (float y = -edgeValue; y < edgeValue; y += step)
        {
            for (float x = -edgeValue; x < edgeValue; x += step)
            {
                Engine::Renderer2D::DrawQuad({x,y}, {step*0.9f, step*0.9f}, 
                                             { (x + edgeValue)/quadPerRow, (0.4f), (y + edgeValue)/quadPerRow, 0.2f });
            }
        }
    }
    Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() 
{
    EG_PROFILE_FUNCTION();

    ImGui::Begin("Sandbox2D");

    ImGui::Text("FPS: %.4f", 1.0f/m_Timestep);
        
    ImGui::SliderFloat3("Quad Position", glm::value_ptr(m_QuadPosition), -10.0f, 10.0f);
    ImGui::SliderFloat2("Quad Size", glm::value_ptr(m_QuadSize), 0.1f, 10.0f);
    ImGui::ColorPicker4("Quad Color", glm::value_ptr(m_QuadColor));

    auto stats = Engine::Renderer2D::GetStatistics();

    ImGui::Text("Renderer 2D Statisctics:");
    ImGui::Text("Drawcalls:   %d", stats.DrawCalls);
    ImGui::Text("QuadCount:   %d", stats.QuadCount);
    ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
    ImGui::Text("IndexCount:  %d", stats.GetTotalIndexCount());
    ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event &event) 
{
    m_CameraController.OnEvent(event);
}

