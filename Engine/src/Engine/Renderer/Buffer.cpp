//
// Created by phatt on 2/11/25.
//
#include "Core/Core.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// BufferLayout //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BufferLayout::BufferLayout()
        : m_Elements(), m_Stride() {}

    BufferLayout::BufferLayout(std::initializer_list<BufferElement>&& elements)
        : m_Elements(elements), m_Stride()
    {
        CalculateOffsetsAndStride();
    }

    BufferLayout::~BufferLayout() = default;

    void BufferLayout::CalculateOffsetsAndStride()
    {
        unsigned int offset = 0;

        for (BufferElement& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
        }

        m_Stride = offset;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// VertexBuffer //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Ref<VertexBuffer> VertexBuffer::Create(float *vertices, unsigned int count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, count);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Ref<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:   EG_CORE_ASSERT(false, "RendererAPI::None not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        EG_CORE_ASSERT(false, "Unknown renderer API!");
        return nullptr;
    }

}
