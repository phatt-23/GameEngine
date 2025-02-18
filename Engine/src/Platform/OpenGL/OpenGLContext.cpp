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
        EG_PROFILE_FUNCTION();

        // Loading OpenGL with GLAD
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Glad failed to initialize.");

        i32 fragTextureMax, combinedTextureMax;
        EG_OPENGL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &fragTextureMax));
        EG_OPENGL_CALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedTextureMax));

        int versionMajor, versionMinor;
        EG_OPENGL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &versionMajor));
        EG_OPENGL_CALL(glGetIntegerv(GL_MINOR_VERSION, &versionMinor));

        EG_CORE_INFO("OpenGL Info:");
        EG_CORE_INFO("  Vendor: {}", (char*)glGetString(GL_VENDOR));
        EG_CORE_INFO("  Renderer: {}", (char*)glGetString(GL_RENDERER));
        EG_CORE_INFO("  Version: {}", (char*)glGetString(GL_VERSION));
        EG_CORE_INFO("  Fragment shader texture binding maximum: {}", fragTextureMax);
        EG_CORE_INFO("  Maximum texture count that can be used: {}", combinedTextureMax);

        EG_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
                       "Engine requires OpenGL version of at least 4.5.");
    }

    void OpenGLContext::SwapBuffers()
    {
        EG_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }

}
