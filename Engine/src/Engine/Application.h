//
// Created by phatt on 2/8/25.
//
#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

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

        static Application& Get();
    private:
        bool OnWindowClose(Event& event);
        bool OnWindowResize(WindowResizeEvent& event) const;

    private:
        bool m_Running = true;
        LayerStack m_LayerStack;
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Shader> m_Shader;
    };

    // DEFINED IN CLIENT
    Application* CreateApplication();
}
