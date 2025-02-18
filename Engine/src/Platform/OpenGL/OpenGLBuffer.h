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
        OpenGLVertexBuffer(const u32 size);
        OpenGLVertexBuffer(const f32* vertices, const u32 count);
        ~OpenGLVertexBuffer() override;

        void SetData(const void* data, const u32 size) override;

        void Bind() const override;
        void Unbind() const override;

        const BufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(BufferLayout&& layout) override { m_Layout = layout; }

    private:
        u32 m_RendererID;
        BufferLayout m_Layout;
        u32 m_Size;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const u32* indices, const u32 count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetCount() const override;
    private:
        u32 m_Count;
        u32 m_RendererID;
    };
}

