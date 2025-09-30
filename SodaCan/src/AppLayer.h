#pragma once

#include "Panels/Panels.h"
#include "Soda.h"

namespace Soda
{
// @TODO: This should be modularized into guizmo code
enum class GizmoTransformMode
{
  None = 0,
  MouseOnly, // Wont have any gizmos on the screen. keeping it clean
  Translation,
  Rotation,
  Scale,
  Universal // Will have all the operation modes shown on the screen
};
enum class GizmoOperationMode
{
  None = 0,
  Local,
  Global
};

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
  bool OnMouseClicked(MouseClickedEvent &mouseClick);
  bool OnKeyPressed(KeyPressEvent &keyPress);

  // TODO: These Fucntions Should Be Modularized
  void CreateNewScene();
  void OpenScene();
  void SaveScene();
  void SaveSceneAs();

private:
  EditorCamera m_EditorCamera;
  Ref<Framebuffer> m_GameFramebuffer;
  Ref<Framebuffer> m_EditorFramebuffer;
  Ref<Scene> m_Scene;

  glm::vec2 m_GameViewportSize = glm::vec2(0.0f);
  glm::vec2 m_EditorViewportSize = glm::vec2(0.0f);
  glm::vec2 m_SceneViewportBounds[2];
  Panels m_Panels;

  GizmoTransformMode m_GizmoTransformMode = GizmoTransformMode::Translation;
  GizmoOperationMode m_GizmoOperationMode = GizmoOperationMode::Local;

  // game viewport
  bool m_IsGamePanelFocused = false;
  bool m_IsGamePanelHovered = false;
  // Scene Viewport
  bool m_IsScenePanelFocused = false;
  bool m_IsScenePanelHovered = false;
};
} // namespace Soda