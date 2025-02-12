//
// Created by phatt on 2/10/25.
//

#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine/Core.h"

namespace Engine
{

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        EG_CORE_ASSERT(m_WindowHandle != nullptr, "Window handle is null!");
    }

    OpenGLContext::~OpenGLContext() = default;

    void OpenGLContext::Init()
    {
        // Loading OpenGL with GLAD
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Glad failed to initialize.");

        EG_CORE_INFO("OpenGL Info:");
        EG_CORE_INFO("  Vendor: {}", (char*)glGetString(GL_VENDOR));
        EG_CORE_INFO("  Renderer: {}", (char*)glGetString(GL_RENDERER));
        EG_CORE_INFO("  Version: {}", (char*)glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
