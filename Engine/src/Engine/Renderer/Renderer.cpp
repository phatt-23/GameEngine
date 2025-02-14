//
// Created by phatt on 2/11/25.
//

#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer.h"
#include "RenderCommand.h"


namespace Engine
{
    void Renderer::Init()
    {
        s_SceneData = new SceneData;
        RenderCommand::Init();
    }

    void Renderer::Shutdown()
    {
        delete s_SceneData;
        RenderCommand::Shutdown();
    }

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", transform);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(int width, int height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
