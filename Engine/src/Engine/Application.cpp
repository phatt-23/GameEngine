//
// Created by phatt on 2/8/25.
//

#include "EnginePCH.h"
#include "Application.h"
#include "Input.h"
#include "Event/ApplicationEvent.h"
#include "Renderer/Buffer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include <memory>

namespace Engine
{

    Application::Application()
    {
        EG_CORE_ASSERT(s_Instance == nullptr, "Application already exists.");
        s_Instance = this;

        m_Window.reset(Window::Create());
        // callback with instance reference `this` bounded
        m_Window->SetEventCallback([this](Event& e) -> void { OnEvent(e); });

        m_ImGuiLayer = new ImGuiLayer();
        m_LayerStack.PushOverlay(m_ImGuiLayer);




        m_VertexArray.reset(VertexArray::Create());

        float vertices[] = {
            -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
            0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
            0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0,
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset( VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(*vertices)) );
        vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
        });

        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset( IndexBuffer::Create(indices, sizeof(indices) / sizeof(*indices)) );

        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexShader = R"(
            #version 410 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            out vec4 v_Color;
            void main() {
                gl_Position = vec4(a_Position, 1.0);
                v_Color = a_Color;
            }
        )";

        std::string fragmentShader = R"(
            #version 410 core
            in vec4 v_Color;
            out vec4 f_Color;
            void main() {
                f_Color = v_Color;
            }
        )";

        m_Shader = std::make_shared<Shader>(vertexShader, fragmentShader);
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            RenderCommand::SetClearColor({0.1, 0.1, 0.1, 0.1});
            RenderCommand::Clear();

            Renderer::BeginScene();

            m_Shader->Bind();
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EG_CORE_TRACE("{0}", event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OnWindowResize));

        // go backward, first the last-most layer gets the event go back to the first
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            Layer* layer = *(--it); // TODO: Maybe move up
            layer->OnEvent(event);

            if (event.IsHandled()) {
                break;
            }
        }
    }

    bool Application::OnWindowResize(WindowResizeEvent& event) const
    {
        RenderCommand::SetViewport(0, 0, (int)event.GetWidth(), (int)event.GetHeight());
        return false;
    }

    bool Application::OnWindowClose(Event& event)
    {
        m_Running = false;
        return true;
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    Application& Application::Get()
    {
        EG_CORE_ASSERT(s_Instance != nullptr, "Application has not been instantiated yet.");
        return *s_Instance;
    }

}
