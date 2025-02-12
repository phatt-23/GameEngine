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

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int count)
        : m_RendererID(0)
    {
        EG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, (GLsizei)count * sizeof(float), vertices, GL_STATIC_DRAW));
        // EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        EG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        EG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count)
        : m_RendererID(), m_Count(count)
    {
        EG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        EG_OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)m_Count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        EG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        EG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    unsigned int OpenGLIndexBuffer::GetCount() const {
        return m_Count;
    }

}
