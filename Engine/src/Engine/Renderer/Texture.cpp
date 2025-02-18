//
// Created by phatt on 2/10/25.
//
#include "Texture.h"

#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Texture2D /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// TextureLibrary /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    void TextureLibrary::Add(const Ref<Texture>& texture)
    {
        EG_CORE_ASSERT(!Exists(texture->GetName()), "Texture with name '{}' already exists in the texture library!", texture->GetName());
        m_Textures[texture->GetName()] = texture;
    }

    void TextureLibrary::Add(const std::string& name, const Ref<Texture>& texture)
    {
        EG_CORE_ASSERT(!Exists(name), "Texture with name '{}' already exists in the texture library!", name);
        m_Textures[name] = texture;
    }

    Ref<Texture> TextureLibrary::Get(const std::string& name)
    {
        EG_CORE_ASSERT(Exists(name), "Texture with name '{}' doesnt exist in the texture library!", name);
        return m_Textures[name];
    }

    bool TextureLibrary::Exists(const std::string& name) const
    {
        return m_Textures.contains(name);
    }
}
