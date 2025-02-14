//
// Created by phatt on 2/10/25.
//
#include "OpenGLShader.h"

#include "Core/EnginePCH.h"
#include "Core/Core.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Filepath(filepath)
    {
        std::size_t lastSlash = m_Filepath.find_last_of("/\\");
        std::size_t lastDot = m_Filepath.rfind(".");
        std::size_t nameBegin = lastSlash != std::string::npos ? lastSlash + 1 : 0;
        std::size_t nameEnd = lastDot != std::string::npos ? lastDot : filepath.size();
        m_Name = m_Filepath.substr(nameBegin, nameEnd - nameBegin);

        const std::string content = ReadFile(filepath);
        const auto sources = PreProcess(content);
        m_RendererID = Compile(sources);
    }


    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
        : m_Filepath(), m_Name(name)
    {
        std::unordered_map<unsigned int, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;
        m_RendererID = Compile(sources);
    }


    unsigned int OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& sources) const
    {
        EG_CORE_ASSERT(sources.size() >= 0 && sources.size() <= 3, "Can only have three shader sources (vertex, geometry, fragment)!");

        unsigned int program;
        EG_OPENGL_CALL(program = glCreateProgram());

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

            std::vector<char> errorLog(length);
            EG_OPENGL_CALL(glGetShaderInfoLog(shader, length, &length, errorLog.data()));
            EG_OPENGL_CALL(glDeleteShader(shader));
            
            struct ErrorMessage 
            {
                unsigned int Order;
                unsigned int Line;
                unsigned int Column; 
                std::string Description;
            };
            std::vector<ErrorMessage> errorMessages;

            std::istringstream messagesStream(std::string(errorLog.data()));
            std::regex pattern(R"((\d+):(\d+)\((\d+)\): error: \s*(.*)?)");
            std::smatch match;
            std::string message;
            while (std::getline(messagesStream, message))
            {
                if (std::regex_match(message, match, pattern))
                {
                    ErrorMessage m(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]), match[4]);
                    errorMessages.push_back(std::move(m));
                }
            }

            int lineNumber = 1;
            std::istringstream sourceStream(source);
            std::ostringstream numberedSource;
            std::string sourceLine;
        
            std::stringstream format;

            while (std::getline(sourceStream, sourceLine))
            {
                const unsigned int numberMargin = 8;
                const unsigned int lineMargin = 4;
                
                auto it = std::find_if(errorMessages.begin(), errorMessages.end(), [&](const auto& m){ return m.Line == lineNumber; });

                if (it == errorMessages.end())
                {
                    numberedSource << EG_DYN_FORMATLN(EG_FORMAT_STRING("{:", numberMargin, "}|{:", lineMargin - 1, + "}{}"), lineNumber, " ", sourceLine);
                    lineNumber++;
                    continue;
                }
                
                const char* arrow = "==>";
                numberedSource << EG_DYN_FORMATLN(EG_FORMAT_STRING("{}{:", numberMargin - strlen(arrow), "}|{:", lineMargin - 1, "}{}"), arrow, lineNumber, " ", sourceLine);

                while (it != errorMessages.end())
                {
                    numberedSource << EG_DYN_FORMATLN(EG_FORMAT_STRING("{:>", numberMargin + lineMargin + it->Column - 2, "} ^^^^ * {} *"), " ", it->Description);
                    it = std::find_if(std::next(it), errorMessages.end(), [&](const auto& m){ return m.Line == lineNumber; });
                }

                lineNumber++;
            }

            EG_CORE_ERROR("Shader compilation failed!");
            EG_CORE_ERROR("Error(s):\n{}", errorLog.data());
            EG_CORE_ERROR("Source ('{}'):\n{}", m_Filepath, numberedSource.str());
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

        EG_CORE_ASSERT(!content.empty(), "The file '{}' is empty!", filepath);
        return std::move(content);
    }
 
    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) const
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";

        std::size_t position = source.find(typeToken, 0);
        while (position != std::string::npos) 
        {
            const std::size_t endOfLine = std::min(source.find("\r\n", position), source.find('\n', position));
            const std::size_t afterTypeToken = position + strlen(typeToken);

            EG_CORE_ASSERT(source[afterTypeToken] == ' ', 
                "The `#type` and shader type specifier must be separated by space! Instead got `{}`.", 
                source.substr(position, endOfLine - position));

            const std::size_t typeBegin = source.find_first_not_of(' ', afterTypeToken);
            const std::size_t typeEnd = std::min(endOfLine, std::min(source.find_first_of(' ', typeBegin), source.find_first_of("//", typeBegin)));
            const std::size_t nextLinePos = std::min(source.find_first_not_of('\n', endOfLine), source.find_first_not_of("\r\n", endOfLine));
            const std::size_t nextPos = source.find(typeToken, nextLinePos);
            const std::string& shaderTypeStr = source.substr(typeBegin, typeEnd - typeBegin);

            unsigned int shaderType = [&]{
                if (shaderTypeStr == "vertex") 
                    return GL_VERTEX_SHADER;
                else if (shaderTypeStr == "fragment" || shaderTypeStr == "pixel") 
                    return GL_FRAGMENT_SHADER;
                return 0;
            }();

            EG_CORE_ASSERT(shaderType != 0, "Unknown shader type specified by `#type {}`", shaderTypeStr);
            EG_CORE_ASSERT(!shaderSources.contains(shaderType), "Only one `#type {}` can be in a single file!", shaderTypeStr);

            shaderSources[shaderType] = source.substr(nextLinePos, ((nextPos != std::string::npos) ? nextPos : source.size()) - nextLinePos);

            position = nextPos;
        }

        EG_CORE_ASSERT(!shaderSources.empty(), "No shader sources in '{}' were found! Did you specify them with `#type ...`?", m_Filepath);
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

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec)
    {
        UploadUniformFloat3(name, vec);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec)
    {
        UploadUniformFloat4(name, vec);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
    {
        UploadUniformMat4(name, mat);
    }

    const std::string& OpenGLShader::GetName() const
    {
        return m_Name;
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
