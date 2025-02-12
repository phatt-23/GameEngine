#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"
#include "OpenGLCore.h"

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path), m_RendererID(), m_Height(), m_Width(), m_Channels()
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
        stbi_set_flip_vertically_on_load(false);

        m_Width = width;
        m_Height = height; 
        m_Channels = channels;

        EG_CORE_ASSERT(imageData != nullptr, "Failed to load image ({})!", path);

        GLenum format, internalFormat = 0;
        switch (m_Channels)
        {
            case 1: format = GL_RED; internalFormat = GL_R8; break;
            case 2: format = GL_RG; internalFormat = GL_RG8; break;
            case 3: format = GL_RGB; internalFormat = GL_RGB8; break;
            case 4: format = GL_RGBA; internalFormat = GL_RGBA8; break;
            default: EG_CORE_ASSERT(format, "Unknown texture format (number of channels)!"); break;
        }

        EG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
        EG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height));

        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        EG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, imageData));
        stbi_image_free(imageData);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        EG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
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
        EG_OPENGL_CALL(glBindTextureUnit(slot, m_RendererID));
    }

    void OpenGLTexture2D::Unbind() const
    {
        EG_OPENGL_CALL(glBindTextureUnit(0, m_RendererID));
    }
}
