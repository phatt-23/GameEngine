//
// Created by phatt on 2/11/25.
//

#include "EnginePCH.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{

    VertexArray *VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

}
