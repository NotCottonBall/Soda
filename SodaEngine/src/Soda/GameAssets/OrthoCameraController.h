#pragma once

#include "glm/glm.hpp"

#include "Soda/_Main/Timestep.h"

#include "Soda/Renderer/Camera.h"

#include "Soda/Events/AppEvents.h"
#include "Soda/Events/Events.h"
#include "Soda/Events/MouseEvents.h"

namespace Soda
{
class OrthoCameraController
{
public:
  OrthoCameraController(float aspectRatio, bool rotation = false);

  void OnUpdate(float dt);
  void OnEvent(Event &event);

  void WhenResized(float width, float height);

  OrthoCamera &GetCamera() { return m_Camera; }
  const OrthoCamera &GetCamera() const { return m_Camera; }

  float GetCameraSpeed() const { return m_CameraTranslationSpeed; }
  void SetCameraSpeed(const float &speed) { m_CameraTranslationSpeed = speed; }

  float GetCameraRotationSpeed() const { return m_CameraRotationSpeed; }
  void SetCameraRotationSpeed(const float &speed)
  {
    m_CameraRotationSpeed = speed;
  }

  float GetZoomLevel() const { return m_ZoomLevel; }
  void SetZoomLevel(const float &zoomLevel) { m_ZoomLevel = zoomLevel; }

private:
  bool OnMouseScrolled(MouseScrollEvent &msEvent);
  bool OnWindowResized(WindowResizeEvent &wrEvent);

private:
  float m_AspectRatio;
  float m_ZoomLevel = 1.0f;
  OrthoCamera m_Camera;

  glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
  float m_CameraTranslationSpeed = 5.0f;

  bool m_Rotation;
  float m_CameraRotation = 0.0f;
  float m_CameraRotationSpeed = 180.0f;
};
} // namespace Soda