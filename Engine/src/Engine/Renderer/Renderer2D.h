//
// Created by phatt on 2/14/25.
//
#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
namespace Engine
{

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Reset();
        static void Flush();

        // colored
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        // with texture
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
                             const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
                             const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));

        // colored with rotation
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
        
        // with texture and rotation
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, 
                                    const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, 
                                    const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));

    };

}
