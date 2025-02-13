
//
// Created by phatt on 2/10/25.
//
#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include "Engine/Renderer/Shader.h"

namespace Engine
{

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        const std::string& GetName() const override;

        int GetUniformLocation(const std::string& name);
        void UploadUniformMat4(const std::string& name, const glm::mat4& mat);

        void UploadUniformFloat(const std::string& name, const float scalar);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& vec);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& vec);

        void UploadUniformInt(const std::string& name, const int scalar);
        void UploadUniformInt2(const std::string& name, const glm::i32vec2& vec);
        void UploadUniformInt3(const std::string& name, const glm::i32vec3& vec);
        void UploadUniformInt4(const std::string& name, const glm::i32vec4& vec);

    private:
        [[nodiscard]] 
        std::string ReadFile(const std::string& filepath) const;

        [[nodiscard]] 
        std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source) const;

        [[nodiscard]] 
        unsigned int Compile(const std::unordered_map<unsigned int, std::string>& sources) const;

        [[nodiscard]] 
        unsigned int CompileSource(unsigned int type, const std::string &source) const;

    private:
        unsigned int m_RendererID;
        std::string m_Name;
        std::string m_Filepath;
        std::unordered_map<std::string, int> m_LocationCache;
    };

}
