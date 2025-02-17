#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "OpenGLCore.h"

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path), m_RendererID(), m_Width(), m_Height(), m_Channels()
    {
        m_Name = path.substr(path.find_last_of("/\\") + 1, path.find_last_of('.') - path.find_last_of("/\\") - 1);
        LoadTexture(path);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& name, const std::string& path)
        : m_Path(path), m_Name(name), m_RendererID(), m_Width(), m_Height(), m_Channels()
    {
        LoadTexture(path);
    }

    void OpenGLTexture2D::LoadTexture(const std::string& path)
    {
        EG_PROFILE_FUNCTION();

        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        unsigned char* imageData = stbi_load(m_Path.value().c_str(), &width, &height, &channels, 0);
        stbi_set_flip_vertically_on_load(false);

        EG_CORE_ASSERT(imageData != nullptr, "Failed to load image ({})!", m_Path.value());

        m_Width = width;
        m_Height = height; 
        m_Channels = channels;

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
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

        EG_CORE_WARN("Image ({}) size in bits: {} ({}x{}x{}) and bytes: {}", 
                     m_Name, 
                     m_Width*m_Height*m_Channels, 
                     m_Width, m_Height, m_Channels, 
                     (m_Width*m_Height*m_Channels)/8);

        // WARN: OpenGL (on Linux) leaks memory here (using libgallium).
        EG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imageData));
        stbi_image_free(imageData);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& name, u32 width, u32 height)
        : m_Path({}), m_Name(name), m_RendererID(), m_Width(width), m_Height(height), 
        m_Channels(4), m_DataFormat(GL_RGBA), m_InternalFormat(GL_RGBA8) // TODO: Paramtrize this.
    {
        EG_PROFILE_FUNCTION();
        
        EG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
        EG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
        EG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));
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
        EG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data));
    }

    void OpenGLTexture2D::SaveToFile(const std::string& path) const
    {
        EG_CORE_WARN("Number of channels: {}", m_Channels);

        std::byte* pixels = new std::byte[m_Width * m_Height * m_Channels * sizeof(f32)];
        EG_OPENGL_CALL(glGetTextureImage(m_RendererID, 0, m_DataFormat, GL_UNSIGNED_BYTE, m_Width*m_Height*m_Channels, pixels));
        EG_CORE_ASSERT(pixels != nullptr);

        stbi_write_png(path.c_str(), m_Width, m_Height, m_Channels, pixels, 100);

        delete[] pixels;
    }

    const std::string& OpenGLTexture2D::GetName() const
    {
        return m_Name;
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
