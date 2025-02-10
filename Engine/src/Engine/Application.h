//
// Created by phatt on 2/8/25.
//
#pragma once

#include "Window.h"
#include "LayerStack.h"

namespace Engine
{
    // Singleton
    class Application {
        static inline Application* s_Instance = nullptr;
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(Event& event);

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;

    };

    // DEFINED IN CLIENT
    Application* CreateApplication();
}
