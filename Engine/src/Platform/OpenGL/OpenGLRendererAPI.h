//
// Created by phatt on 2/11/25.
//
#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void SetViewport(int x, int y, int width, int height) override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };

}
