//
// Created by phatt on 2/11/25.
//

#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderCommand.h"

namespace Engine
{
    void Renderer::Init()
    {
        EG_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        EG_PROFILE_FUNCTION();

        RenderCommand::Shutdown();
        Renderer2D::Shutdown();
    }

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        EG_PROFILE_FUNCTION();

        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
        EG_PROFILE_FUNCTION();
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        EG_PROFILE_FUNCTION();

        shader->Bind();
        shader->SetMat4("u_Model", transform);
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(int width, int height)
    {
        EG_PROFILE_FUNCTION();

        RenderCommand::SetViewport(0, 0, width, height);
    }
}
