#pragma once

#include "glm/glm.hpp"

#include "Core/Timestep.h"

#include "Renderer/Camera.h"
#include "Renderer/RendererCamera.h"

#include "Core/Events/AppEvents.h"
#include "Core/Events/Events.h"
#include "Core/Events/MouseEvents.h"

namespace Soda
{
class EditorCamera : public RendererCamera
{
public:
  EditorCamera(float aspectRatio);

  void OnUpdate(float dt);
  void OnEvent(Event &event);
  void OnResize(float width, float height);

  // GET SET ERS //
  // camera controls
  float GetCameraSpeed() const { return m_cameraMoveSpeed; }
  void SetCameraSpeed(const float &speed) { m_cameraMoveSpeed = speed; }
  float GetCameraSensi() const { return m_cameraSensitivity; }
  void SetCameraSensi(float sensi) { m_cameraSensitivity = sensi; }
  float GetZoomLevel() const { return m_zoomLevel; }
  void SetZoomLevel(const float &zoomLevel) { m_zoomLevel = zoomLevel; }

  // camera settings
  float GetAspectRatio() const { return m_aspectRatio; }
  void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
  float GetFOV() const { return m_fov; }
  void SetFOV(float fov) { m_fov = fov; }
  float GetNearPlane() const { return m_nearPlane; }
  float GetFarPlane() const { return m_farPlane; }
  void SetNearAndFarPlane(float nearPlane, float farPlane)
  {
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
  }

  // camera properties
  const glm::mat4 &GetViewMat() const { return m_viewMat; }
  const glm::mat4 &GetProjectionMat() const { return m_Projection; }
  const glm::mat4 &GetProjectionViewMat() const { return m_viewProjectionMat; }
  // GET SET ERS //

private:
  bool OnMouseScrolled(MouseScrollEvent &msEvent);
  bool OnWindowResized(WindowResizeEvent &wrEvent);
  bool OnMouseMove(MouseMoveEvent &mmEvent);

  void SetProjectionMat(float fov, float aspectRatio, float nearPlane,
                        float farPlane);
  void RecalculateMatrices();

private:
  glm::mat4 m_viewMat;
  glm::mat4 m_viewProjectionMat;

  float m_cameraMoveSpeed = 5.0f;
  float m_cameraSensitivity = 0.5f;

  float m_aspectRatio;
  float m_fov = 45.0f;
  float m_nearPlane = 0.01f;
  float m_farPlane = 1000.0f;

  float m_zoomLevel = 10.0f;
  glm::vec3 m_forwardVec = {0.0f, 0.0f, -1.0f};
  glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};

  float m_yaw, m_pitch;
  float m_lastX, m_lastY;
  bool m_firstMouse;
};
} // namespace Soda