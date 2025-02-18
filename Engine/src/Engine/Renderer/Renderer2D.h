//
// Created by phatt on 2/14/25.
//
#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
namespace Engine
{

// Another test:
// Move the transformation calculation out from the CPU to the GPU.
// Send transformation matrices to the GPU.
//
// #define MAX_QUAD_COUNT 1024 * 4
// uniform mat4 u_Transforms[MAX_QUAD_COUNT];
//
// Matrices are big and mostly empty, better solution might be
// affine decomposition. Decompose the transformation matrix to
// its 3 basic components - float3 translation, quarternion rotation, float3 scale 
// and then compose them inside the GPU.

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
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, 
                             const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, 
                             const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));

        // colored with rotation
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
        
        // with texture and rotation
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture>& texture, 
                                    const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture>& texture, 
                                    const float tilingFactor = 1.0f, const glm::vec4& tintingColor = glm::vec4(1.0f));

        struct Statistics
        {
            u32 DrawCalls;
            u32 QuadCount;
            u32 GetTotalVertexCount() const { return QuadCount * 4; }
            u32 GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static inline void ResetStatistics() { memset(&s_Stats, 0, sizeof(Statistics)); }
        static const Statistics& GetStatistics() { return s_Stats; }

    private:
        inline static Statistics s_Stats{};
    };

}
