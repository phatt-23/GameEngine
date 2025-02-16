//
// Created by phatt on 2/11/25.
//
#pragma once
#include "Core/Core.h"

namespace Engine
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// BufferLayout //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class ShaderDataType
    {
        None = 0,
        Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
    };

    static unsigned int SizeofShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Mat3: return 3 * 3 * 4;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::None:
                break;
        }

        EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        unsigned int Size;
        unsigned int Offset;
        bool Normalized;

        BufferElement(ShaderDataType type, std::string&& name, bool normalized = false)
            : Name(name), Type(type), Size(SizeofShaderDataType(type)), Offset(0), Normalized(normalized) {}

        int GetElementCount() const {
            switch (Type)
            {
                case ShaderDataType::Int:
                case ShaderDataType::Bool:
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Int2:
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Int3:
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Int4:
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;
                case ShaderDataType::None: break;
            }
            EG_CORE_ASSERT(false, "Unknown type!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout();
        BufferLayout(std::initializer_list<BufferElement>&& elements);
        ~BufferLayout();

        unsigned int GetStride() const { return m_Stride; }
        const std::vector<BufferElement> GetElements() const { return m_Elements; }

        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride();

    private:
        std::vector<BufferElement> m_Elements;
        unsigned int m_Stride;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// VertexBuffer //////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void SetData(const void* data, const u32 size) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(BufferLayout&& layout) = 0;

        static Ref<VertexBuffer> Create(const f32* vertices, const u32 count);
        static Ref<VertexBuffer> Create(const u32 size);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// IndexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetCount() const = 0;

        static Ref<IndexBuffer> Create(const u32* indices, u32 count);
    };

}
