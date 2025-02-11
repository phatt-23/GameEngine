//
// Created by phatt on 2/10/25.
//
#include "Engine/Core.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Engine
{
    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        m_RendererID = glCreateProgram();
        const unsigned int vertexShader = CompileSource(ShaderType::VertexShader, vertexSource);
        const unsigned int fragmentShader = CompileSource(ShaderType::FragmentShader, fragmentSource);

        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);
        glLinkProgram(m_RendererID);

        int status;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &status);

        if (status == GL_FALSE)
        {
            int length;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> message(length);
            glGetProgramInfoLog(m_RendererID, length, &length, message.data());

            glDeleteShader(m_RendererID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            EG_CORE_ERROR("{}", message.data());
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return;
        }

        glValidateProgram(m_RendererID);

        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
        glDeleteShader(m_RendererID);
    }

    void Shader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    unsigned int Shader::CompileSource(ShaderType type, const std::string &source) {
        const int openGLShaderType = [type]() -> int {
            switch (type) {
                case ShaderType::VertexShader: return GL_VERTEX_SHADER;
                case ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
                case ShaderType::None:
                default: break;
            }
            EG_CORE_ASSERT(false, "Uknown shader type while compiling!");
            return 0;
        }();

        const char* sourcePtr = source.c_str();

        const unsigned int shader = glCreateShader(openGLShaderType);
        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> message(length);
            glGetShaderInfoLog(shader, length, &length, message.data());
            glDeleteShader(shader);

            EG_CORE_ERROR("{}", message.data());
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return 0;
        }

        return shader;
    }


}
