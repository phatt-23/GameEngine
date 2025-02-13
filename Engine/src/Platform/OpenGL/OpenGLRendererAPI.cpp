//
// Created by phatt on 2/11/25.
//

#include "OpenGLRendererAPI.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include <glad/glad.h>

namespace Engine
{

    void OpenGLRendererAPI::Init()
    {
        EG_OPENGL_CALL(glEnable(GL_BLEND));
        EG_OPENGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        EG_OPENGL_CALL(glClearColor(color.r, color.g, color.b, color.a));
    }

    void OpenGLRendererAPI::Clear()
    {
        EG_OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
    {
        EG_OPENGL_CALL(glViewport(x, y, width, height));
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray)
    {
        EG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
    }


}
