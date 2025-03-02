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
  EditorCamera m_EditorCamera;
  Ref<Framebuffer> m_GameFramebuffer;
  Ref<Framebuffer> m_EditorFramebuffer;
  Ref<Scene> m_Scene;

  glm::vec2 m_GameViewportSize = glm::vec2(0.0f);
  glm::vec2 m_EditorViewportSize = glm::vec2(0.0f);
  Panels m_Panels;

private:
  // game viewport
  bool m_IsGamePanelFocused = false;
  bool m_IsGamePanelHovered = false;
  // Scene Viewport
  bool m_IsScenePanelFocused = false;
  bool m_IsScenePanelHovered = false;
};
} // namespace Soda