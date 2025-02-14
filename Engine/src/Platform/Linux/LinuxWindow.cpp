//
// Created by phatt on 2/9/25.
//

#include "LinuxWindow.h"

#include "Core/Core.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Engine
{

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int errorCode, const char* message)
    {
        EG_CORE_ERROR("GLFW Error ({}): {}", errorCode, message);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const Engine::WindowProps &props)
        : m_Window(nullptr), m_Context(nullptr)
    {
        Init(props);
    }

    LinuxWindow::~LinuxWindow() noexcept 
    {
        delete m_Context;
        Shutdown(); 
    };

    void LinuxWindow::Init(const Engine::WindowProps &props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.VSync = true;
        m_Data.EventCallback = nullptr;

        EG_CORE_INFO("Creating window '{}' (width={}, height={})",
                     m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            EG_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        // Creating GLFW window
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(),
                                    nullptr, nullptr);

        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);


        // set window callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowResizeEvent event(width, height);
            data->EventCallback(event);
            data->Width = width;
            data->Height = height;
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event{};
            data->EventCallback(event);
        });

        // set keyboard callbacks
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data->EventCallback(event);
                } break;
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data->EventCallback(event);
                } break;
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data->EventCallback(event);
                } break;
                default: break;
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int charCode) {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(charCode);
            data->EventCallback(event);
        });

        // set mouse callbacks
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data->EventCallback(event);
                } break;
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data->EventCallback(event);
                } break;
                default: break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) -> void {
            auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event((float)x, (float)y);
            data->EventCallback(event);
        });
    }

    void LinuxWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void LinuxWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void LinuxWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool LinuxWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void *LinuxWindow::GetNativeWindow() {
        return m_Window;
    }


}
