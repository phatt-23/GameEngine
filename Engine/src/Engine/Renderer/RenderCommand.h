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
        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void SetViewport(int x, int y, int width, int height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };
}
