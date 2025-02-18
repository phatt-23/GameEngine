//
// Created by phatt 02/14/2025
//
#include "Engine.h"

#include <glm/glm.hpp>

class Sandbox2D : public Engine::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void OnAttach() override;
    void OnEvent(Engine::Event &event) override;
    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

private:
    Engine::OrthographicCameraController m_CameraController;
    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::ShaderLibrary m_ShaderLibrary;
    Engine::TextureLibrary m_TextureLibrary;
    glm::vec3 m_QuadPosition = {0.0f, 0.0f, 0.0f};
    glm::vec2 m_QuadSize = {1.0f, 1.0f};
    glm::vec4 m_QuadColor = {1.0f, 0.0f, 0.0f, 1.0f};
    Engine::Timestep m_Timestep;
};
