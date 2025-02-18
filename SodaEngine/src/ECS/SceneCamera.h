#pragma once

#include "Renderer/RendererCamera.h"
#include "glm/trigonometric.hpp"

namespace Soda
{
class SceneCamera : public RendererCamera
{
public:
  enum class CameraType
  {
    Orthographic = 0,
    Perspective = 1
  };

public:
  SceneCamera();
  virtual ~SceneCamera() = default;

  void SetOrthoCamera(float size, float nearPlane, float farPlane);
  void SetPersCamera(float FOV, float nearPlane, float farPlane);
  void SetViewport(uint32_t width, uint32_t height);
  void SetViewport();

  // sets and gets for cam info
  const CameraType &GetCameraType() const { return m_CameraType; }
  void SetCameraType(const CameraType &cameraType)
  {
    m_CameraType = cameraType;
    RecalculateMatrix();
  }

  // sets and gets for ortho cam values
  const float GetOrthoCameraZoom() const { return m_OrthoCamZoom; }
  void SetOrthoCameraZoom(float size)
  {
    m_OrthoCamZoom = size;
    RecalculateMatrix();
  }

  const float GetOrthoNearPlane() const { return m_OrthoNearPlane; }
  void SetOrthoNearPlane(float nearPlane)
  {
    m_OrthoNearPlane = nearPlane;
    RecalculateMatrix();
  }

  const float GetOrthoFarPlane() const { return m_OrthoFarPlane; }
  void SetOrthoFarPlane(float farPlane)
  {
    m_OrthoFarPlane = farPlane;
    RecalculateMatrix();
  }

  // sets and gets for pers cam values
  const float GetPerspectiveFov() const { return m_PerspectiveFOV; }
  void SetPerspectiveFov(float fov)
  {
    m_PerspectiveFOV = fov;
    RecalculateMatrix();
  }

  const float GetPerspectiveNearPlane() const { return m_PersNearPlane; }
  void SetPerspectiveNearPlane(float nearPlane)
  {
    m_PersNearPlane = nearPlane;
    RecalculateMatrix();
  }

  const float GetPerspectiveFarPlane() const { return m_PersFarPlane; }
  void SetPerspectiveFarPlane(float farPlane)
  {
    m_PersFarPlane = farPlane;
    RecalculateMatrix();
  }

private:
  void RecalculateMatrix();

private:
  CameraType m_CameraType = CameraType::Orthographic;

  float m_AspectRatio = 0.0f;

  // orthographic cam properties
  float m_OrthoCamZoom = 10.0f;
  float m_OrthoNearPlane = 1.0f, m_OrthoFarPlane = -1.0f;

  // perspective cam properties
  float m_PerspectiveFOV = glm::radians(45.0f);
  float m_PersNearPlane = 0.01, m_PersFarPlane = 1000.0f;
};
} // namespace Soda