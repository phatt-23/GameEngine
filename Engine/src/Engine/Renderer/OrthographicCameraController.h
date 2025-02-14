//
// Created by phatt on 2/13/25.
//
#pragma once

#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"

namespace Engine
{

class OrthographicCameraController
{
public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);
    ~OrthographicCameraController();

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;

    bool m_RotationEnabled;

    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f;
    float m_CameraTranslationSpeed = 2.0f;
    float m_CameraRotationSpeed = 30.0f;
};

}
