//
// Created by phatt on 2/9/25.
//
#pragma once


#include "Window.h"
#include "Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Engine
{

    class LinuxWindow : public Window
    {
    public:
        explicit LinuxWindow(const WindowProps& props);
        ~LinuxWindow() noexcept override;

        void OnUpdate() override;

        unsigned int GetWidth() const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        void SetEventCallback(EventCallbackFn&& callback) override
        {
            m_Data.EventCallback = std::move(callback);
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        } m_Data;
    };

}

