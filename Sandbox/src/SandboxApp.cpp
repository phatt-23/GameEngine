#include "Engine.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Engine::Layer
{
public:
    ExampleLayer()
        : Layer("Layer"),
        m_Camera(-2.0, 2.0, -2.0, 2.0)
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

        std::string vertexShader = R"(
            #version 410 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;
            uniform mat4 u_ViewProjection;
            out vec2 v_TexCoord;
            void main() {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentShader = R"(
            #version 410 core
            in vec2 v_TexCoord;
            uniform vec3 u_Color;
            uniform sampler2D u_Texture;
            out vec4 f_Color;
            void main() {
                f_Color = texture(u_Texture, v_TexCoord);
                // f_Color = vec4(v_TexCoord, 0.0f, 1.0f) + vec4(u_Color, 0.2f);
            }
        )";

        m_Shader = Engine::Shader::Create(vertexShader, fragmentShader);
        m_Texture = Engine::Texture2D::Create("assets/textures/ISO_C++_Logo.png");
        m_Texture->Bind(0);

        m_Shader->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Engine::Timestep ts) override
    {
        // EG_INFO("Timestep: {}s, {}ms (fps={})", (float)ts, ts.GetMilliseconds(), 1.0f/ts);

        const float moveSpeed = 2.0f * ts;
        const float rotSpeed = 30.0f * ts;


        if (Engine::Input::IsKeyPressed(EG_KEY_LEFT))
            m_Position.x -= moveSpeed;
        if (Engine::Input::IsKeyPressed(EG_KEY_RIGHT))
            m_Position.x += moveSpeed;
        if (Engine::Input::IsKeyPressed(EG_KEY_DOWN))
            m_Position.y -= moveSpeed;
        if (Engine::Input::IsKeyPressed(EG_KEY_UP))
            m_Position.y += moveSpeed;
        if (Engine::Input::IsKeyPressed(EG_KEY_A))
            m_Rotation += rotSpeed;
        if (Engine::Input::IsKeyPressed(EG_KEY_D))
            m_Rotation -= rotSpeed;


        m_Camera.SetPosition(m_Position);
        m_Camera.SetRotation(m_Rotation);
           
        m_Shader->Bind();
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_Color);

        Engine::RenderCommand::SetClearColor({0.1, 0.1, 0.1, 0.1});
        Engine::RenderCommand::Clear();
        Engine::Renderer::BeginScene(m_Camera);
        Engine::Renderer::Submit(m_Shader, m_VertexArray);
        Engine::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Sandbox");
        ImGui::DragFloat("rotation", &m_Rotation);
        ImGui::DragFloat3("position", glm::value_ptr(m_Position));
        ImGui::ColorPicker3("color", glm::value_ptr(m_Color));
        ImGui::End();
    }

    void OnEvent(Engine::Event& event) override
    {
    }

private:
    Engine::OrthographicCamera m_Camera;
    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::Ref<Engine::Shader> m_Shader;
    Engine::Ref<Engine::Texture2D> m_Texture;
    float m_Rotation = 0.0f;
    glm::vec3 m_Position = {0.0, 0.0, 0.0};
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

