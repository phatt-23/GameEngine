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

    Ref<Shader> Shader::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(path);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

}
