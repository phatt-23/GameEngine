//
// Created by phatt on 2/10/25.
//

#include "OpenGLContext.h"
#include "OpenGLCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/Core.h"

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

        int versionMajor, versionMinor;
        EG_OPENGL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &versionMajor));
        EG_OPENGL_CALL(glGetIntegerv(GL_MINOR_VERSION, &versionMinor));
        EG_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
                       "Engine requires OpenGL version of at least 4.5.");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
