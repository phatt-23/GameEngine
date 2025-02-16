#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"
#include "OpenGLCore.h"

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D(unsigned int width, unsigned int height)
        : m_Path({}), m_RendererID(), m_Width(width), m_Height(height), 
        m_Channels(4), m_DataFormat(GL_RGBA), m_InternalFormat(GL_RGBA8) // TODO: Paramtrize this.
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
        EG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }


    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path), m_RendererID(), m_Width(), m_Height(), m_Channels()
    {
        EG_PROFILE_FUNCTION();
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        unsigned char* imageData = stbi_load(m_Path.value().c_str(), &width, &height, &channels, 0);
        stbi_set_flip_vertically_on_load(false);

        m_Width = width;
        m_Height = height; 
        m_Channels = channels;

        EG_CORE_ASSERT(imageData != nullptr, "Failed to load image ({})!", m_Path.value());

        auto[m_DataFormat, m_InternalFormat] = [&] -> std::pair<unsigned int, unsigned int> {
            switch (m_Channels)
            {
                case 1: return {GL_RED, GL_R8};
                case 2: return {GL_RG, GL_RG8};
                case 3: return {GL_RGB, GL_RGB8};
                case 4: return {GL_RGBA, GL_RGBA8};
                default: 
                    EG_CORE_ASSERT(false, "Unknown texture format (number of channels) for image '{}'!", m_Path.value()); 
                    return {0, 0};
            }
        }();

        EG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
        EG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

        EG_CORE_WARN("Image size in bits: {} ({}x{}x{}) and bytes: {}", 
                     width * height * channels, width, height, channels, (width * height * channels)/8);

        // WARN: OpenGL (on Linux) leaks memory here (using libgallium).
        EG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imageData));
        stbi_image_free(imageData);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
    }

    void OpenGLTexture2D::SetData(void* data, unsigned int size)
    {
        EG_PROFILE_FUNCTION();
        EG_CORE_ASSERT(size == m_Width * m_Height * m_Channels, "Size ({}) doesn't match the size of the texture ({})!", size, m_Width * m_Height * m_Channels);
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    unsigned int OpenGLTexture2D::GetWidth() const
    {
        return m_Width;
    }

    unsigned int OpenGLTexture2D::GetHeight() const
    {
        return m_Height;
    }

    void OpenGLTexture2D::Bind(unsigned int slot) const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindTextureUnit(slot, m_RendererID));
    }

    void OpenGLTexture2D::Unbind() const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindTextureUnit(0, m_RendererID));
    }
}
