//
// Created by phatt on 2/11/25.
//

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Engine
{

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }


}
