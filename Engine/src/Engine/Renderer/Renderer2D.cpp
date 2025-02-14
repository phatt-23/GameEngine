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
    Ref<Shader> FlatColorShader;
} *s_Data = nullptr;

void Renderer2D::Init()
{
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

    s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(*indices));
    s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

    s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
}

void Renderer2D::Shutdown()
{
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_Data->FlatColorShader->Bind(); 
    s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene()
{

}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawQuad(glm::vec3(position, 0.0f), size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    auto transform = glm::translate(glm::identity<glm::mat4>(), position) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0));

    s_Data->FlatColorShader->Bind();
    s_Data->FlatColorShader->SetMat4("u_Transform", transform);
    s_Data->FlatColorShader->SetFloat4("u_Color", color);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

}
