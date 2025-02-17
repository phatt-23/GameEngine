#include "Engine/Renderer/Texture.h"

namespace Engine
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(const std::string& name, const std::string& path);
        OpenGLTexture2D(const std::string& name, unsigned int width, unsigned int height);
        ~OpenGLTexture2D() override;

        void SetData(void* data, unsigned int size) override;

        const std::string& GetName() const override;
        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        void Bind(unsigned int slot = 0) const override;
        void Unbind() const override;

    private:
        void LoadTexture(const std::string& path);
        
    private:
        unsigned int m_RendererID{};
        std::optional<std::string> m_Path;
        std::string m_Name;
        unsigned int m_Width, m_Height, m_Channels;
        unsigned int m_DataFormat, m_InternalFormat;
    };
}
