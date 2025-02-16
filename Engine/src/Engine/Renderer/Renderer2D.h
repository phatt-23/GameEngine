//
// Created by phatt on 2/14/25.
//
#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Texture.h"

namespace Engine
{

struct DrawQuadProps
{
    glm::vec3 Position;
    glm::vec2 Size = glm::vec2(1.0f);
    glm::vec4 Color = glm::vec4(1.0f);
    Ref<Texture2D> Texture = nullptr;
    float RotationAngle = 0.0f;
    float TilingFactor = 1.0f;
};


class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();
    static void Flush();
 
    static void DrawQuad(const DrawQuadProps& props);
};

}
