//
// Created by phatt on 2/8/25.
//

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Renderer/RenderCommand.h"
#include "Core/Timestep.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Engine
{

    Application::Application()
    {
        EG_CORE_ASSERT(s_Instance == nullptr, "Application already exists.");
        s_Instance = this;

        // callback with instance reference `this` bounded
        m_Window.reset(Window::Create());
        m_Window->SetEventCallback([this](Event& e) -> void { OnEvent(e); });
        // m_Window->SetVSync(false);

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        m_LayerStack.PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (m_Running)
        {
            const auto time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        // EG_CORE_TRACE("{0}", event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OnWindowResize));
        // go backward, first the last-most layer gets the event go back to the first
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*(--it))->OnEvent(event);
            if (event.IsHandled())
                break;
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

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    Application& Application::Get()
    {
        EG_CORE_ASSERT(s_Instance != nullptr, "Application has not been instantiated yet.");
        return *s_Instance;
    }

}
