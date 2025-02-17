//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Engine
{

    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;

        virtual void SetInt(const std::string& name, const i32 value) = 0;
        virtual void SetInt2(const std::string& name, const glm::i32vec2& vec) = 0;
        virtual void SetInt3(const std::string& name, const glm::i32vec3& vec) = 0;
        virtual void SetInt4(const std::string& name, const glm::i32vec4& vec) = 0;

        virtual void SetFloat(const std::string& name, const f32 scalar) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& vec) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& vec) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& vec) = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3& mat) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;

        virtual void SetIntArray(const std::string& name, const i32* array, const u32 count) = 0;

        static Ref<Shader> Create(const std::string& path);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    };


    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        Ref<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}

