//
// Created by phatt on 2/11/25.
//

#include "EnginePCH.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{

    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

}
