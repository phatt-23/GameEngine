//
// Created by phatt on 2/11/25.
//
#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"

namespace Engine
{

    class Renderer {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };

}

