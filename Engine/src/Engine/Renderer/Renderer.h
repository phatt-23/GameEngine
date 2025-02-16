//
// Created by phatt on 2/11/25.
//
#pragma once

#include "Core/Core.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"

namespace Engine
{

    class Renderer 
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void OnWindowResize(int width, int height);
    private:
        struct SceneData 
        {
            glm::mat4 ViewProjectionMatrix;
        };

        inline static Scope<SceneData> s_SceneData = CreateScope<SceneData>();
    };

}

