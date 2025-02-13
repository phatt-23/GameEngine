//
// Created by phatt on 2/11/25.
//
#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera();

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const;
        void SetPosition(const glm::vec3& position);

        float GetRotation() const;
        void SetRotation(float rotation);

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::vec3 m_Position;
        float m_Rotation;
    };
}
