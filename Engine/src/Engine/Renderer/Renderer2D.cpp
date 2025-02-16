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

static inline struct Renderer2DStorage 
{
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;
} *s_Data = nullptr;

void Renderer2D::Init()
{
    EG_PROFILE_FUNCTION();


    EG_CORE_ASSERT(s_Data == nullptr, "Can initialize Renderer2D only once!");
    s_Data = new Renderer2DStorage;

    s_Data->QuadVertexArray = Engine::VertexArray::Create();

    float vertices[] = {
        -0.5, -0.5, 0.0, 0.0, 0.0, 
        0.5,  -0.5, 0.0, 1.0, 0.0,
        0.5,  0.5,  0.0, 1.0, 1.0, 
        -0.5, 0.5,  0.0, 0.0, 1.0,
    };
    auto vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices)/sizeof(*vertices));
    vertexBuffer->SetLayout({
        {Engine::ShaderDataType::Float3, "a_Position"},
        {Engine::ShaderDataType::Float2, "a_TexCoord"},
    });

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(*indices));

    s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);
    s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

    s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data->WhiteTexture = Texture2D::Create(1, 1);
    std::uint32_t white = 0xffffffff;
    s_Data->WhiteTexture->SetData(&white, sizeof(white));
}

void Renderer2D::Shutdown()
{
    EG_PROFILE_FUNCTION();

    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    EG_PROFILE_FUNCTION();

    s_Data->TextureShader->Bind(); 
    s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    s_Data->TextureShader->SetInt("u_Texture", 0);
}

void Renderer2D::EndScene()
{
    EG_PROFILE_FUNCTION();
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    EG_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(position, 0.0f), size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    EG_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->WhiteTexture->Bind(0);

    auto transform = glm::translate(glm::identity<glm::mat4>(), position) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0));
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)
{
    EG_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(position, 0.0f), size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
{
    EG_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind(0);

    auto transform = glm::translate(glm::identity<glm::mat4>(), position) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0));
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

}
