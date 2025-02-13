//
// Created by phatt on 2/10/25.
//
#include "OpenGLShader.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include "Engine/EnginePCH.h"
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLCore.h"

namespace Engine
{

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        const std::string content = ReadFile(filepath);
        EG_CORE_ASSERT(!content.empty(), "The file '{}' is empty!", filepath);
        
        const auto sources = PreProcess(content);
        EG_CORE_ASSERT(!sources.empty(), "No shader sources in '{}' were found! Did you specify them with `#type ...`?");
        
        m_RendererID = CompileSources(sources);
    }

    unsigned int OpenGLShader::CompileSources(const std::unordered_map<unsigned int, std::string>& sources) const
    {
        EG_OPENGL_CALL(unsigned int program = glCreateProgram());

        std::vector<unsigned int> shaders(sources.size());
        for (const auto& [type, source] : sources) 
        {
            auto shader = CompileSource(type, source);
            shaders.push_back(shader);
            EG_OPENGL_CALL(glAttachShader(program, shader));
        }

        EG_OPENGL_CALL(glLinkProgram(program));

        int status;
        EG_OPENGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &status));
        if (status == GL_FALSE)
        {
            int length;
            EG_OPENGL_CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
            std::vector<char> message(length);
            EG_OPENGL_CALL(glGetProgramInfoLog(m_RendererID, length, &length, message.data()));

            EG_OPENGL_CALL(glDeleteShader(program));
            for (auto shader : shaders)
            {
                EG_OPENGL_CALL(glDeleteShader(shader));
            }

            EG_CORE_ERROR("{}", message.data());
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return 0;
        }

        EG_OPENGL_CALL(glValidateProgram(program));

        for (auto shader : shaders)
        {
            EG_OPENGL_CALL(glDeleteShader(shader));
        }

        return program;
    }

    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        m_RendererID = glCreateProgram();
        const unsigned int vertexShader = CompileSource(GL_VERTEX_SHADER, vertexSource);
        const unsigned int fragmentShader = CompileSource(GL_FRAGMENT_SHADER, fragmentSource);

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

    unsigned int OpenGLShader::CompileSource(unsigned int type, const std::string &source) const
    {
        const char* sourcePtr = source.c_str();

        EG_OPENGL_CALL(const unsigned int shader = glCreateShader(type));
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

            EG_CORE_ERROR("Shader compilation error(s):\n{}", message.data());
            EG_CORE_ERROR("Shader source:\n{}", source);
            EG_CORE_ASSERT(false, "Shader compilation failed!");

            return 0;
        }

        return shader;
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath) const 
    {
        std::ifstream input(filepath, std::ios::binary);
        if (!input.is_open())
        {
            EG_CORE_ERROR("Could not open file '{}'", filepath);
            return {};
        }
        
        std::string content;
        input.seekg(0, std::ios::end);
        content.resize(input.tellg());
        input.seekg(0, std::ios::beg);
        input.read(content.data(), content.size());
        input.close(); 

        return std::move(content);
    }
 
    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) const
    {
        const char* typeToken = "#type";
        std::unordered_map<GLenum, std::string> shaderSources;

        std::size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) 
        {
            std::size_t endOfLine = std::min(source.find("\r\n", pos), source.find('\n', pos));
            std::size_t begin = pos + strlen(typeToken);
            
            EG_CORE_ASSERT(source[begin] == ' ', "The `#type` and shader type specifier must be separated by space! Instead got `{}`.", source.substr(pos, endOfLine - pos));

            std::size_t typeBegin = source.find_first_not_of(' ', begin);
            std::size_t typeEnd = std::min(std::min(source.find_first_of(' ', typeBegin), source.find_first_of("//", typeBegin)), endOfLine);
            std::size_t nextLinePos = std::min(source.find_first_not_of('\n', endOfLine), source.find_first_not_of("\r\n", endOfLine));
            std::size_t nextPos = source.find(typeToken, nextLinePos);

            const std::string& shaderTypeStr = source.substr(typeBegin, typeEnd - typeBegin);

            EG_CORE_DEBUG("line: `{}`", source.substr(pos, endOfLine - pos));
            EG_CORE_DEBUG("pos: {}", pos);
            EG_CORE_DEBUG("eol: {}", endOfLine);
            EG_CORE_DEBUG("begin: {}", begin); 
            EG_CORE_DEBUG("typeBegin: {}", typeBegin); 
            EG_CORE_DEBUG("typeEnd: {}", typeEnd);
            EG_CORE_DEBUG("nextLine: {}", nextLinePos);
            EG_CORE_DEBUG("nexPos: {}", nextPos);

            unsigned int shaderType = 0;
            if (shaderTypeStr == "vertex") 
                shaderType = GL_VERTEX_SHADER;
            else if (shaderTypeStr == "fragment" || shaderTypeStr == "pixel") 
                shaderType = GL_FRAGMENT_SHADER;

            EG_CORE_ASSERT(shaderType != 0, "Unknown shader type specified by `#type ({})`", shaderTypeStr);
            EG_CORE_ASSERT(!shaderSources.contains(shaderType), "Can have only of `#type {}` in a single file!", shaderTypeStr);

            shaderSources[shaderType] = source.substr(nextLinePos, 
                    nextPos - ((nextPos != std::string::npos) ? nextLinePos : source.size()));
            pos = nextPos;
        }

        return shaderSources;
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
