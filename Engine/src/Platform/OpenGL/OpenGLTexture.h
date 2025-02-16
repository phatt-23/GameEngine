#include "Engine/Renderer/Texture.h"

namespace Engine
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(unsigned int width, unsigned int height);
        OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        void SetData(void* data, unsigned int size) override;

        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        void Bind(unsigned int slot = 0) const override;
        void Unbind() const override;
    private:
        unsigned int m_RendererID{};
        std::optional<std::string> m_Path;
        unsigned int m_Width, m_Height, m_Channels;
        unsigned int m_DataFormat, m_InternalFormat;
    };
}
