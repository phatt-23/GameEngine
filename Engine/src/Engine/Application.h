//
// Created by phatt on 2/8/25.
//
#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine
{
    // Singleton
    class Application 
    {
        static inline Application* s_Instance = nullptr;
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        static Application& Get();
    private:
        bool OnWindowClose(Event& event);
        bool OnWindowResize(WindowResizeEvent& event);

    private:
        bool m_Running = true;
        bool m_Minimized = false;
        Scope<Window> m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        float m_LastFrameTime = 0.0f;
    };

    // DEFINED IN CLIENT
    Application* CreateApplication();
}
