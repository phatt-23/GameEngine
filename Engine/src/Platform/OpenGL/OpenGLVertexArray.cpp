//
// Created by phatt on 2/11/25.
//

#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include "OpenGLCore.h"

namespace Engine
{
    static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool:      return GL_BOOL;
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::None:      break;
        }

        EG_CORE_ASSERT(false, "Unknown type!");
        return 0;
    }


    OpenGLVertexArray::OpenGLVertexArray()
    {
        EG_OPENGL_CALL(glCreateVertexArrays(1, &m_RendererID));
        // EG_OPENGL_CALL(glBindVertexArray(0));
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
    }

    void OpenGLVertexArray::Bind() const
    {
        EG_OPENGL_CALL(glBindVertexArray(m_RendererID));
    }

    void OpenGLVertexArray::Unbind() const
    {
        EG_OPENGL_CALL(glBindVertexArray(0));
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        EG_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer doesn't have a layout.");

        EG_OPENGL_CALL(glBindVertexArray(m_RendererID));
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();

        int index = 0;
        for (const auto& element : layout)
        {
            EG_OPENGL_CALL(glEnableVertexAttribArray(index));
            EG_OPENGL_CALL(glVertexAttribPointer(index,
                    element.GetElementCount(),
                    ShaderDataTypeToOpenGLType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    (GLsizei)layout.GetStride(),
                    reinterpret_cast<const void*>(element.Offset)));
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
        EG_OPENGL_CALL(glBindVertexArray(0));
        vertexBuffer->Unbind();
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
        EG_OPENGL_CALL(glBindVertexArray(m_RendererID));
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

}
