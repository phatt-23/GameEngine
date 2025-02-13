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

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
    }

    EG_CORE_ASSERT(false, "Unknown renderer API!");
    return nullptr;
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return m_Shaders.contains(name);
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
    EG_CORE_ASSERT(!Exists(name), "Shader with name '' already in the shader library!", name);
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    Add(shader->GetName(), shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    Ref<Shader> shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentShader)
{
    Ref<Shader> shader = Shader::Create(name, vertexSource, fragmentShader);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
    EG_CORE_ASSERT(Exists(name), "Shader library doesn't contain shader with name '{}'", name);
    return m_Shaders[name];
}

}
