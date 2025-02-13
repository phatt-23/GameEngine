//
// Created by phatt on 2/10/25.
//
#include "OpenGLShader.h"

#include "Engine/EnginePCH.h"
#include "Engine/Core.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    template<typename... Args>
    static std::string dynPrint(std::string_view const& runtimeFormat, Args&&... args)
    {
        return std::vformat(runtimeFormat, std::make_format_args(args...));

    }


    template<typename... Args>
    static std::string dynPrintln(std::string_view const& runtimeFormat, Args&&... args)
    {
        return std::vformat(runtimeFormat, std::make_format_args(args...)) + "\n";
    }


    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Filepath(filepath)
    {
        const std::string content = ReadFile(filepath);
        EG_CORE_ASSERT(!content.empty(), "The file '{}' is empty!", filepath);
        
        const auto sources = PreProcess(content);
        EG_CORE_ASSERT(!sources.empty(), "No shader sources in '{}' were found! Did you specify them with `#type ...`?");
        
        m_RendererID = CompileSources(sources);
    }


    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
        : m_Filepath()
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
            
            std::vector<ErrorMessage> errorMessages;

            std::istringstream messageStream(std::string(errorLog.data()));
            std::regex pattern(R"((\d+):(\d+)\((\d+)\): error: \s*(.*)?)");
            std::smatch match;
            std::string message;

            while (std::getline(messageStream, message))
            {
                if (std::regex_match(message, match, pattern))
                {
                    ErrorMessage m(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]), match[4]);
                    EG_CORE_DEBUG("order: {}, line: {}, column: {}, desc: '{}'", m.Order, m.Line, m.Column, m.Description);
                    errorMessages.push_back(std::move(m));
                }
            }

            std::istringstream sourceStream(source);
            std::ostringstream numberedSource;
            std::string sourceLine;
            int lineNumber = 1;

            while (std::getline(sourceStream, sourceLine))
            {
                const unsigned int numberMargin = 8;
                const unsigned int lineMargin = 4;
                
                auto it = std::find_if(errorMessages.begin(), errorMessages.end(), [&](const auto& m){ return m.Line == lineNumber; });
                if (it == errorMessages.end())
                {
                    numberedSource << dynPrintln("{:" + std::to_string(numberMargin) + "}|{:" + std::to_string(lineMargin - 1) + "}{}", lineNumber, " ", sourceLine);
                    lineNumber++;
                    continue;
                }
                
                const char* arrow = "==>";
                numberedSource << dynPrintln("{}{:" + std::to_string(numberMargin - strlen(arrow)) + "}|{:" + std::to_string(lineMargin - 1) + "}{}", arrow, lineNumber, " ", sourceLine);

                while (it != errorMessages.end())
                {
                    const char* underline = "^^^^";
                    numberedSource << dynPrintln("{:>" + std::to_string(numberMargin + lineMargin + it->Column - 1) + "}{}", " ", underline);
                    numberedSource << dynPrintln("{:>" + std::to_string(numberMargin + lineMargin + it->Column - 1) + "}**** {} ****", " ", it->Description);
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
            EG_CORE_ASSERT(source[afterTypeToken] == ' ', "The `#type` and shader type specifier must be separated by space! Instead got `{}`.", 
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
