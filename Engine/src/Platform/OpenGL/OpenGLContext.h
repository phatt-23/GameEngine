//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Engine/Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Engine
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        ~OpenGLContext() override;

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}


