//
// Created by phatt on 2/14/25.
//
#include "Renderer/Renderer2D.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

namespace Engine
{

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        f32 TextureIndex = 0.0f; // white texture
        f32 TilingFactor = 1.0f;
    };

    static inline struct Renderer2DData
    {
        constexpr static u32 MaxQuads = 1 << 14;
        constexpr static u32 MaxVertices = MaxQuads * 4;
        constexpr static u32 MaxIndices = MaxQuads * 6;
        constexpr static u32 MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray = nullptr;
        Ref<VertexBuffer> QuadVertexBuffer = nullptr;
        Ref<Shader> TextureShader = nullptr;
        Ref<Texture2D> WhiteTexture = nullptr; // at 0th index in TextureSlots

        QuadVertex* QuadVertexBufferData = nullptr;

        u32 QuadCount = 0;
        u32 QuadVertexBufferIndex = 0;
        u32 QuadIndexBufferIndex = 0;

        std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
        u32 TextureSlotIndex = 1;

        constexpr static glm::vec4 QuadVertexPositions[4] = {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };

        constexpr static glm::vec2 QuadTexCoords[4] = {
            { 0.0f, 0.0f }, 
            { 1.0f, 0.0f }, 
            { 1.0f, 1.0f }, 
            { 0.0f, 1.0f }
        };
    } s_Data;

    void Renderer2D::Init()
    {
        EG_PROFILE_FUNCTION();
        s_Data.QuadVertexArray = Engine::VertexArray::Create();


        s_Data.QuadVertexBufferData = new QuadVertex[Engine::Renderer2DData::MaxVertices];
        s_Data.QuadVertexBuffer = VertexBuffer::Create(Engine::Renderer2DData::MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3,   "a_Position" },
            { ShaderDataType::Float4,   "a_Color" },
            { ShaderDataType::Float2,   "a_TexCoord" },
            { ShaderDataType::Float,    "a_TextureIndex" },
            { ShaderDataType::Float,    "a_TilingFactor" },
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);


        u32* quadIndices = new u32[Engine::Renderer2DData::MaxIndices];
        for (u32 i = 0, offset = 0; i < Engine::Renderer2DData::MaxIndices; i += 6, offset += 4)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 3;
        }    
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Engine::Renderer2DData::MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;


        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");


        // will be set beforehand and then only texture will be bound
        std::array<i32, Renderer2DData::MaxTextureSlots> samplerValues{};
        std::ranges::copy(std::views::iota(0, (i32)samplerValues.size()), samplerValues.begin());

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplerValues.data(), samplerValues.size());

        u32 white = 0xffffffff;
        s_Data.WhiteTexture = Texture2D::Create("WhiteTexture1x1", 1, 1);
        s_Data.WhiteTexture->SetData(&white, sizeof(white));


        // texture slot 0 is reserved for the white texture
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        s_Data.WhiteTexture->Bind(0);
    }

    void Renderer2D::Shutdown()
    {
        EG_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        EG_PROFILE_FUNCTION();
        s_Data.TextureShader->Bind(); 
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        Reset();
        ResetStatistics();
    }

    void Renderer2D::EndScene()
    {
        EG_PROFILE_FUNCTION();
        Flush();
    }

    void Renderer2D::Reset()
    {
        EG_PROFILE_FUNCTION();
        s_Data.QuadCount = 0;
        s_Data.QuadVertexBufferIndex = 0;
        s_Data.QuadIndexBufferIndex = 0;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Flush()
    {
        EG_PROFILE_FUNCTION();
        EG_CORE_ASSERT(s_Data.QuadCount * 4 == s_Data.QuadVertexBufferIndex);
        EG_CORE_ASSERT(s_Data.QuadCount * 6 == s_Data.QuadIndexBufferIndex);

        if (s_Data.QuadCount == 0) return;

        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferData, s_Data.QuadVertexBufferIndex * sizeof(QuadVertex));

        for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i].get()->Bind(i);
        
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexBufferIndex);

        s_Stats.DrawCalls++;
    }

    // colored
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        EG_PROFILE_FUNCTION();
        DrawQuad(glm::vec3(position, 1.0f), size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        EG_PROFILE_FUNCTION();
        if (s_Data.QuadCount >= Renderer2DData::MaxQuads)
        {
            Flush();
            Reset();
        }

        const auto transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0));

        constexpr u32 quadVertexCount = 4;
        for (u32 i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferData[s_Data.QuadVertexBufferIndex++] = { 
                .Position = glm::vec3(transform * Engine::Renderer2DData::QuadVertexPositions[i]),
                .Color = color, 
                .TexCoord = Engine::Renderer2DData::QuadTexCoords[i],
            };
        }
        
        s_Data.QuadIndexBufferIndex += 6;
        s_Data.QuadCount++;
        
        s_Stats.QuadCount++;
    }
    
    // with texture
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, 
                              const float tilingFactor, const glm::vec4& tintColor)
    {
        EG_PROFILE_FUNCTION();
        DrawQuad(glm::vec3(position, 1.0f), size, texture, tilingFactor, tintColor); 
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, 
                              const float tilingFactor, const glm::vec4& tintColor)
    {
        EG_PROFILE_FUNCTION();
        if (s_Data.QuadCount >= Renderer2DData::MaxQuads)
        {
            Flush();
            Reset();
        }

        const auto transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0));
        
        u32 textureIndex = 0;
        
        // if texture already slotted, use it
        for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = i;
                break;
            }
        }
        
        // if not slotted yet, slot it
        if (textureIndex == 0)
        {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            {
                Flush();
                Reset();
            }
            
            textureIndex = s_Data.TextureSlotIndex;
            s_Data.TextureSlots[textureIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        constexpr auto color = glm::vec4(1.0f);

        constexpr u32 quadVertexCount = 4;
        for (u32 i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferData[s_Data.QuadVertexBufferIndex++] = { 
                .Position = transform * Renderer2DData::QuadVertexPositions[i],
                .Color = glm::vec4(1.0f), 
                .TexCoord = Renderer2DData::QuadTexCoords[i],
                .TextureIndex = (f32)textureIndex,
                .TilingFactor = tilingFactor,
            };
        }
        
        s_Data.QuadIndexBufferIndex += 6;
        s_Data.QuadCount++;

        s_Stats.QuadCount++;
    }

    // colored with rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotationAngle, const glm::vec4& color)
    {
        EG_PROFILE_FUNCTION();
        DrawRotatedQuad(glm::vec3(position, 1.0f), size, rotationAngle, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotationAngle, const glm::vec4& color)
    {
        EG_PROFILE_FUNCTION();
        if (s_Data.QuadCount >= Renderer2DData::MaxQuads)
        {
            Flush();
            Reset();
        }

        const auto transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotationAngle, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0));
        
        constexpr u32 quadVertexCount = 4;
        for (u32 i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferData[s_Data.QuadVertexBufferIndex++] = { 
                .Position = transform * Renderer2DData::QuadVertexPositions[i],
                .Color = color, 
                .TexCoord = Renderer2DData::QuadTexCoords[i],
            };
        }
        
        s_Data.QuadIndexBufferIndex += 6;
        s_Data.QuadCount++;

        s_Stats.QuadCount++;
    }
    
    // with texture and rotation
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotationAngle, 
                                     const Ref<Texture>& texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad(glm::vec3(position, 1.0f), size, rotationAngle, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotationAngle, 
                                     const Ref<Texture>& texture, const float tilingFactor, const glm::vec4& tintColor)
    {
        EG_PROFILE_FUNCTION();
        if (s_Data.QuadCount >= Renderer2DData::MaxQuads)
        {
            Flush();
            Reset();
        }

        const auto transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), rotationAngle, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0));
        
        u32 textureIndex = 0;
        
        // if texture already slotted, use it
        for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = i;
                break;
            }
        }
        
        // if not slotted yet, slot it
        if (textureIndex == 0)
        {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            {
                Flush();
                Reset();
            }
            
            textureIndex = s_Data.TextureSlotIndex;
            s_Data.TextureSlots[textureIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        constexpr auto color = glm::vec4(1.0f);

        constexpr u32 quadVertexCount = 4;
        for (u32 i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferData[s_Data.QuadVertexBufferIndex++] = { 
                .Position = transform * Renderer2DData::QuadVertexPositions[i],
                .Color = glm::vec4(1.0f), 
                .TexCoord = Renderer2DData::QuadTexCoords[i],
                .TextureIndex = (f32)textureIndex,
                .TilingFactor = tilingFactor,
            };
        }
        
        s_Data.QuadIndexBufferIndex += 6;
        s_Data.QuadCount++;

        s_Stats.QuadCount++;
    }

}
