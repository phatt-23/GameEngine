//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Engine/Core.h"

namespace Engine
{
    enum class ShaderType
    {
        None = 0,
        VertexShader,
        FragmentShader,
    };

    struct ShaderSource
    {
        std::string VertexShader;
        std::string FragmentShader;
    };

    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<Shader> Create(const std::string& path);
        static Ref<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);
    };

}

