//
// Created by phatt on 2/11/25.
//
#pragma once

#include "RendererAPI.h"

namespace Engine
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->Init();
        }

        inline static void Shutdown()
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->Shutdown();
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->Clear();
        }

        inline static void SetViewport(int x, int y, int width, int height)
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, const u32 indexCount = 0)
        {
            EG_PROFILE_FUNCTION();
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };
}
