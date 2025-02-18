#include "Engine/Renderer/Texture.h"

namespace Engine
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(const std::string& name, const std::string& path);
        OpenGLTexture2D(const std::string& name, u32 width, u32 height);
        ~OpenGLTexture2D() override;

        void SetData(void* data, unsigned int size) override;

        const std::string& GetName() const override;
        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        void Bind(unsigned int slot = 0) const override;
        void Unbind() const override;

        inline bool operator == (const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }

        void SaveToFile(const std::string& path) const;

    private:
        void LoadTexture(const std::string& path);
        
    private:
        u32 m_RendererID;
        std::optional<std::string> m_Path;
        std::string m_Name;
        unsigned int m_Width, m_Height, m_Channels;
        unsigned int m_DataFormat, m_InternalFormat;
    };
}
