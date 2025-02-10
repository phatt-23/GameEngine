//
// Created by phatt on 2/8/25.
//

#include "EnginePCH.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Input.h"

#include <glad/glad.h>

namespace Engine
{

    Application::Application()
    {
        EG_CORE_ASSERT(s_Instance == nullptr, "Application already exists.");
        s_Instance = this;

        m_Window.reset(Window::Create());

        // callback with instance reference `this` bounded
        m_Window->SetEventCallback([this](Event& e) -> void { OnEvent(e); });
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EG_CORE_TRACE("{0}", event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OnWindowClose));

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

    bool Application::OnWindowClose(Event& event)
    {
        m_Running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1.0, 0.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
    }

}
