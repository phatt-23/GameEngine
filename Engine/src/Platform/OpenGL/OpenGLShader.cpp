//
// Created by phatt on 2/10/25.
//
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLCore.h"

namespace Engine
{
    unsigned int CompileSource(ShaderType type, const std::string &source) {
        const int openGLShaderType = [&]{
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
        EG_OPENGL_CALL(glShaderSource(shader, 1, &sourcePtr, nullptr));
        EG_OPENGL_CALL(glCompileShader(shader));
        
        int status;
        EG_OPENGL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

        if (status == GL_FALSE)
        {
            int length;
            EG_OPENGL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
            std::vector<char> message(length);
            EG_OPENGL_CALL(glGetShaderInfoLog(shader, length, &length, message.data()));
            EG_OPENGL_CALL(glDeleteShader(shader));

            EG_CORE_ERROR("{}", message.data());
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return 0;
        }

        return shader;
    }

    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        m_RendererID = glCreateProgram();
        const unsigned int vertexShader = CompileSource(ShaderType::VertexShader, vertexSource);
        const unsigned int fragmentShader = CompileSource(ShaderType::FragmentShader, fragmentSource);

        EG_OPENGL_CALL(glAttachShader(m_RendererID, vertexShader));
        EG_OPENGL_CALL(glAttachShader(m_RendererID, fragmentShader));
        EG_OPENGL_CALL(glLinkProgram(m_RendererID));

        int status;
        EG_OPENGL_CALL(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &status));

        if (status == GL_FALSE)
        {
            int length;
            EG_OPENGL_CALL(glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length));
            std::vector<char> message(length);
            EG_OPENGL_CALL(glGetProgramInfoLog(m_RendererID, length, &length, message.data()));

            EG_OPENGL_CALL(glDeleteShader(m_RendererID));
            EG_OPENGL_CALL(glDeleteShader(vertexShader));
            EG_OPENGL_CALL(glDeleteShader(fragmentShader));

            EG_CORE_ERROR("{}", message.data());
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return;
        }

        EG_OPENGL_CALL(glValidateProgram(m_RendererID));

        EG_OPENGL_CALL(glDetachShader(m_RendererID, vertexShader));
        EG_OPENGL_CALL(glDetachShader(m_RendererID, fragmentShader));
        EG_OPENGL_CALL(glDeleteShader(vertexShader));
        EG_OPENGL_CALL(glDeleteShader(fragmentShader));
    }

    OpenGLShader::~OpenGLShader() 
    {
        EG_OPENGL_CALL(glDeleteProgram(m_RendererID));
    }

    void OpenGLShader::Bind() const 
    {
        EG_OPENGL_CALL(glUseProgram(m_RendererID));
    }

    void OpenGLShader::Unbind() const 
    {
        EG_OPENGL_CALL(glUseProgram(0));
    }


    int OpenGLShader::GetUniformLocation(const std::string &name)
    {
        if (m_LocationCache.contains(name))
            return m_LocationCache[name];

        EG_OPENGL_CALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
        if (location == -1)
            EG_CORE_ERROR("Shader ({}): Uniform with name '{}' wasn't found!", m_RendererID, name);

        m_LocationCache[name] = location;
        return location;
    }

    void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &mat)
    {
        EG_OPENGL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, const float scalar)
    {
        EG_OPENGL_CALL(glUniform1fv(GetUniformLocation(name), 1, &scalar));
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
    {
        EG_OPENGL_CALL(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
    {
        EG_OPENGL_CALL(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
    {
        EG_OPENGL_CALL(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, const int scalar)
    {
        EG_OPENGL_CALL(glUniform1iv(GetUniformLocation(name), 1, &scalar));
    }
    void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::i32vec2& vec)
    {
        EG_OPENGL_CALL(glUniform2iv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

    void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::i32vec3& vec)
    {
        EG_OPENGL_CALL(glUniform3iv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

    void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::i32vec4& vec)
    {
        EG_OPENGL_CALL(glUniform4iv(GetUniformLocation(name), 1, glm::value_ptr(vec)));
    }

}
