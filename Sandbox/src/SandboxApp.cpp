#include "Engine.h"
#include "OrthographicCameraController.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "imgui.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Engine::Layer
{
public:
    ExampleLayer()
        : Layer("Layer"), m_CameraController(1.0f/2.0f, true)
    {}

    ~ExampleLayer() override = default;

    void OnAttach() override
    {
        m_VertexArray = Engine::VertexArray::Create();

        float vertices[] = {
                -0.5, -0.5, 0.0, 0.0, 0.0,
                0.5, -0.5, 0.0, 1.0, 0.0,
                0.5, 0.5, 0.0, 1.0, 1.0,
                -0.5, 0.5, 0.0, 0.0, 1.0,
        };

        Engine::Ref<Engine::VertexBuffer> vertexBuffer;
        vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(*vertices));
        vertexBuffer->SetLayout({
            { Engine::ShaderDataType::Float3, "a_Position" },
            { Engine::ShaderDataType::Float2, "a_TexCoord" },
        });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
        Engine::Ref<Engine::IndexBuffer> indexBuffer;
        indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(*indices));
        m_VertexArray->SetIndexBuffer(indexBuffer);
        
        m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

        m_Texture = Engine::Texture2D::Create("assets/textures/ISO_C++_Logo.png");
        m_AlphaTexture = Engine::Texture2D::Create("assets/textures/blending_transparent_window.png");

        m_ShaderLibrary.Get("Texture")->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Engine::Timestep ts) override
    {
        m_CameraController.OnUpdate(ts);

        m_ShaderLibrary.Get("Texture")->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformFloat3("u_Color", m_Color);

        Engine::RenderCommand::SetClearColor({0.1, 0.1, 0.1, 0.1});
        Engine::RenderCommand::Clear();
        Engine::Renderer::BeginScene(m_CameraController.GetCamera());
        m_Texture->Bind(0);
        Engine::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_VertexArray, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.3, 0.3, 0.0)));
        m_AlphaTexture->Bind(0);
        Engine::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_VertexArray);
        Engine::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Sandbox");
        ImGui::ColorPicker3("color", glm::value_ptr(m_Color));
        ImGui::End();
    }

    void OnEvent(Engine::Event& event) override
    {
        m_CameraController.OnEvent(event);
    }

private:
    Engine::OrthographicCameraController m_CameraController;
    Engine::ShaderLibrary m_ShaderLibrary;
    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::Ref<Engine::Texture2D> m_Texture, m_AlphaTexture;
    glm::vec3 m_Color = {1.0, 0.0, 0.0};
};


class Sandbox : public Engine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() override = default;
};


Engine::Application* Engine::CreateApplication()
{
    return new Sandbox;
}

