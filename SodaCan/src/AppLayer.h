#pragma once

#include "Panels/Panels.h"
#include "Soda.h"

namespace Soda
{
class SodaCan : public Layer
{
public:
  SodaCan();
  virtual ~SodaCan() = default;

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate(Timestep dt) override;
  void OnEvent(Event &event) override;
  void OnImGuiUpdate() override;

  void OnResize(uint32_t width, uint32_t height) override;

private:
  OrthoCameraController m_EditorCamera;
  Ref<Framebuffer> m_Framebuffer;
  Ref<Scene> m_Scene;

  Object m_Square;

  Object m_GameCamera;
  Object m_SecondCam;

  glm::vec2 m_ViewportSize = glm::vec2(0.0f);
  Panels m_Panels;

private:
  bool m_IsPanelFocused = false;
  bool m_IsPanelHovered = false;
};
} // namespace Soda