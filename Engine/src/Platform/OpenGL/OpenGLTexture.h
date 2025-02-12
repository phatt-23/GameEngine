#include "Engine/Renderer/Texture.h"

namespace Engine
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        void Bind(unsigned int slot = 0) const override;
        void Unbind() const override;
    private:
        unsigned int m_RendererID{};
        std::string m_Path;
        unsigned int m_Width, m_Height, m_Channels;
    };
}
