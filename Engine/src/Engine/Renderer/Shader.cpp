//
// Created by phatt on 2/10/25.
//
#include "Engine/Core.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSource, fragmentSource);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

}
