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
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glEnable(GL_BLEND));
        EG_OPENGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void OpenGLRendererAPI::Shutdown()
    {
        EG_PROFILE_FUNCTION();
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glClearColor(color.r, color.g, color.b, color.a));
    }

    void OpenGLRendererAPI::Clear()
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glViewport(x, y, width, height));
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, const u32 indexCount)
    {
        EG_PROFILE_FUNCTION();
        const u32 count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
        EG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }


}
