//
// Created by phatt on 2/11/25.
//
#pragma once

#include "Engine/Renderer/Buffer.h"

namespace Engine
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// VertexBuffer //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, unsigned int count);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        const BufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(BufferLayout&& layout) override { m_Layout = layout; }

    private:
        unsigned int m_RendererID;
        BufferLayout m_Layout;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        unsigned int GetCount() const override;
    private:
        unsigned int m_Count;
        unsigned int m_RendererID;
    };
}

