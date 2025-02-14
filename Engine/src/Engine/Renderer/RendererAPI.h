//
// Created by phatt on 2/11/25.
//
#pragma once

#include "Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Engine
{


    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL,
        };

    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void SetViewport(int x, int y, int width, int height) = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }
    private:
        inline static API s_API = API::OpenGL;
    };

}

