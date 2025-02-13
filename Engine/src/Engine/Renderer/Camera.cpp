//
// Created by phatt on 2/11/25.
//
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{


    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
        m_ViewMatrix(glm::identity<glm::mat4>()),
        m_Position(0.0),
        m_Rotation(0.0)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    OrthographicCamera::~OrthographicCamera() = default;

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::vec3 &OrthographicCamera::GetPosition() const
    {
        return m_Position;
    }

    void OrthographicCamera::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    float OrthographicCamera::GetRotation() const
    {
        return m_Rotation;
    }

    void OrthographicCamera::SetRotation(float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        auto transform = glm::translate(glm::identity<glm::mat4>(), m_Position)
                * glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
