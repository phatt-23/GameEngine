#include "Engine.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sandbox2D : public Engine::Layer
{
public:
  Sandbox2D();

  ~Sandbox2D() override = default;

  void OnAttach() override;
  void OnUpdate(Engine::Timestep ts) override;
  void OnImGuiRender() override;
  void OnEvent(Engine::Event &event) override;

private:
    Engine::OrthographicCameraController m_CameraController;
    Engine::ShaderLibrary m_ShaderLibrary;
    Engine::Ref<Engine::VertexArray> m_VertexArray;
    Engine::Ref<Engine::Texture2D> m_Texture, m_AlphaTexture;
    glm::vec3 m_QuadPosition = {0.0f, 0.0f, 0.0f};
    glm::vec2 m_QuadSize = {1.0f, 1.0f};
    glm::vec4 m_QuadColor = {1.0f, 0.0f, 0.0f, 1.0f};
};
