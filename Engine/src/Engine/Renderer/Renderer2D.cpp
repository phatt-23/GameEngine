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
    };

    static inline struct Renderer2DData
    {
        const u32 MaxQuads = 10000;
        const u32 MaxVertices = MaxQuads * 4;
        const u32 MaxIndices = MaxQuads * 6;

        Ref<VertexArray> QuadVertexArray = nullptr;
        Ref<VertexBuffer> QuadVertexBuffer = nullptr;
        Ref<IndexBuffer> QuadIndexBuffer = nullptr;

        QuadVertex* QuadVertexBufferData = nullptr;

        u32 QuadCount = 0;
        u32 QuadVertexBufferIndex = 0;
        u32 QuadIndexBufferIndex = 0;

        Ref<Shader> TextureShader = nullptr;
        Ref<Texture2D> WhiteTexture = nullptr;
    } *s_Data = nullptr;

    void Renderer2D::Init()
    {
        EG_PROFILE_FUNCTION();

        EG_CORE_ASSERT(s_Data == nullptr, "Renderer2D can be initialized only once!");
        s_Data = new Renderer2DData;

        s_Data->QuadVertexArray = Engine::VertexArray::Create();

        s_Data->QuadVertexBufferData = new QuadVertex[s_Data->MaxVertices];

        s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
        s_Data->QuadVertexBuffer->SetLayout({
            { Engine::ShaderDataType::Float3, "a_Position" },
            { Engine::ShaderDataType::Float4, "a_Color" },
            { Engine::ShaderDataType::Float2, "a_TexCoord" },
        });
        s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);

        u32* quadIndices = new u32[s_Data->MaxIndices];
        for (u32 i = 0, offset = 0; i < s_Data->MaxIndices; i += 6, offset += 4)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 3;
        }
        s_Data->QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
        s_Data->QuadVertexArray->SetIndexBuffer(s_Data->QuadIndexBuffer);

        delete[] quadIndices;

        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

        std::uint32_t white = 0xffffffff;
        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        s_Data->WhiteTexture->SetData(&white, sizeof(white));
    }

    void Renderer2D::Shutdown()
    {
        EG_PROFILE_FUNCTION();
        delete[] s_Data->QuadVertexBufferData;
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        EG_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind(); 
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_Data->TextureShader->SetInt("u_Texture", 0);

        s_Data->QuadCount = 0;
        s_Data->QuadVertexBufferIndex = 0;
        s_Data->QuadIndexBufferIndex = 0;
    }

    void Renderer2D::EndScene()
    {
        EG_PROFILE_FUNCTION();
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferData, s_Data->QuadVertexBufferIndex * sizeof(QuadVertex));
        Flush();
    }

    void Renderer2D::Flush()
    {
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexBufferIndex);
    }

    void Renderer2D::DrawQuad(const DrawQuadProps& props)
    {
        EG_PROFILE_FUNCTION();

        s_Data->QuadCount++;

        const QuadVertex vertices[4] = {
            { .Position = props.Position,                                               .Color = props.Color, .TexCoord = {0.0f, 0.0f} },
            { .Position = props.Position + glm::vec3(props.Size.x, 0.0f, 0.0f),         .Color = props.Color, .TexCoord = {1.0f, 0.0f} },
            { .Position = props.Position + glm::vec3(props.Size.x, props.Size.y, 0.0f), .Color = props.Color, .TexCoord = {1.0f, 1.0f} },
            { .Position = props.Position + glm::vec3(0.0f, props.Size.y, 0.0f),         .Color = props.Color, .TexCoord = {0.0f, 1.0f} }
        };

        s_Data->QuadVertexBufferData[s_Data->QuadVertexBufferIndex++] = vertices[0];
        s_Data->QuadVertexBufferData[s_Data->QuadVertexBufferIndex++] = vertices[1];
        s_Data->QuadVertexBufferData[s_Data->QuadVertexBufferIndex++] = vertices[2];
        s_Data->QuadVertexBufferData[s_Data->QuadVertexBufferIndex++] = vertices[3];

        s_Data->QuadIndexBufferIndex += 6;

        // if (props.Texture)
        //     props.Texture->Bind();
        // else
        //     s_Data->WhiteTexture->Bind(0);
        //
        // s_Data->TextureShader->SetFloat4("u_Color", props.Color);
        // s_Data->TextureShader->SetFloat("u_TilingFactor", props.TilingFactor);
        //
        // auto transform = glm::translate(glm::identity<glm::mat4>(), props.Position) 
        //     * glm::rotate(glm::identity<glm::mat4>(), props.RotationAngle, {0.0f, 0.0f, 1.0f})
        //     * glm::scale(glm::identity<glm::mat4>(), glm::vec3(props.Size, 1.0));
        //
        // s_Data->TextureShader->SetMat4("u_Transform", transform);
        //
        // s_Data->QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

}
