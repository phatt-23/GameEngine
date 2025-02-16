#include "Renderer/OrthographicCameraController.h"
#include "Core/Core.h"
#include "Core/KeyCodes.h"
#include "Event/Event.h"
#include "Core/Input.h"

namespace Engine
{

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), 
    m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
    m_RotationEnabled(rotation)
{

}

OrthographicCameraController::~OrthographicCameraController()
{

}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
    EG_PROFILE_FUNCTION();

    const float translationSpeed = m_CameraTranslationSpeed * ts;
    const float rotationSpeed = m_CameraRotationSpeed * ts;

    if (Input::IsKeyPressed(EG_KEY_W))
        m_CameraPosition.y += translationSpeed;
    if (Input::IsKeyPressed(EG_KEY_A))
        m_CameraPosition.x -= translationSpeed;
    if (Input::IsKeyPressed(EG_KEY_S))
        m_CameraPosition.y -= translationSpeed;
    if (Input::IsKeyPressed(EG_KEY_D))
        m_CameraPosition.x += translationSpeed;

    if (m_RotationEnabled)
    {
        if (Input::IsKeyPressed(EG_KEY_Q))
            m_CameraRotation += rotationSpeed;
        if (Input::IsKeyPressed(EG_KEY_E))
            m_CameraRotation -= rotationSpeed;
    }

    m_Camera.SetPosition(m_CameraPosition);
    m_Camera.SetRotation(m_CameraRotation);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)
{
    EG_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(EG_FORWARD_EVENT_TO_MEM_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
    EG_PROFILE_FUNCTION();

    m_ZoomLevel -= e.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
    EG_PROFILE_FUNCTION();

    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}


}
