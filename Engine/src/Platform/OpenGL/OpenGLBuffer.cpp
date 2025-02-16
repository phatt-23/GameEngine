//
// Created by phatt on 2/11/25.
//

#include "OpenGLBuffer.h"

#include <glad/glad.h>
#include "OpenGLCore.h"

namespace Engine
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// VertexBuffer //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(const u32 size)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, (GLsizei)size, nullptr, GL_DYNAMIC_DRAW));
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const f32* vertices, const u32 count)
        : m_RendererID(0)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, (GLsizei)count * sizeof(float), vertices, GL_STATIC_DRAW));
        // EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLVertexBuffer::SetData(const void* data, const u32 size)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    OpenGLIndexBuffer::OpenGLIndexBuffer(const u32* indices, const u32 count)
        : m_RendererID(), m_Count(count)
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)m_Count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        EG_PROFILE_FUNCTION();
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    u32 OpenGLIndexBuffer::GetCount() const 
    {
        return m_Count;
    }

}
