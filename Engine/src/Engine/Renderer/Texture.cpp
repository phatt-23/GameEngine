//
// Created by phatt on 2/10/25.
//
#include "Texture.h"

#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine
{
    Ref<Texture2D> Texture2D::Create(const std::string& name, unsigned int width, unsigned int height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(name, width, height);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }


    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& name, const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(name, path);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }
}
