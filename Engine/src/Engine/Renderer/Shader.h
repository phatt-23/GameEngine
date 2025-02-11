//
// Created by phatt on 2/10/25.
//
#pragma once

namespace Engine
{
    enum class ShaderType
    {
        None = 0,
        VertexShader,
        FragmentShader,
    };

    class Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        static unsigned int CompileSource(ShaderType type, const std::string& source);

    private:
        unsigned int m_RendererID;
    };

    struct ShaderSource
    {
        std::string VertexShader;
        std::string FragmentShader;
    };

}

