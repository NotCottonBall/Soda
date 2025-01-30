#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Soda
{

// 2D CAMERA //
class OrthoCamera
{
public:
  OrthoCamera(float left, float right, float down, float up)
      : m_projectionMat(glm::ortho(left, right, down, up, -1.0f, 1.0f)),
        m_viewMat(1.0f), m_viewProjectionMat(m_projectionMat * m_viewMat)
  {
  }

  void SetProjection(float left, float right, float down, float up);

  void SetPosition(const glm::vec3 &position)
  {
    m_position = position;
    RecalculateMatrices();
  }
  const glm::vec3 &GetPosition() const { return m_position; }

  void SetRotation(float rotation)
  {
    m_rotation = rotation;
    RecalculateMatrices();
  }
  float GetRotation() const { return m_rotation; }

  const glm::mat4 &GetProjectionMat() const { return m_projectionMat; }
  const glm::mat4 &GetViewMat() const { return m_viewMat; }
  const glm::mat4 &GetProjectionViewMat() const { return m_viewProjectionMat; }

private:
  void RecalculateMatrices();

private:
  glm::mat4 m_projectionMat;
  glm::mat4 m_viewMat;
  glm::mat4 m_viewProjectionMat;

  glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
  float m_rotation = 0.0f;
};

// 3D Camera //
class PerspectiveCamera
{
public:
  PerspectiveCamera(float fov, float aspectRatio, float nearPlane,
                    float farPlane)
      : m_projectionMat(
            glm::perspective(fov, aspectRatio, nearPlane, farPlane)),
        m_viewMat(1.0f), m_viewProjectionMat(m_projectionMat * m_viewMat)
  {
  }

  void SetPosition(glm::vec3 &position)
  {
    m_position = position;
    RecalculateMatrices();
  }
  const glm::vec3 &GetPosition() const { return m_position; }

  void SetRotation(float rotation)
  {
    m_rotation = rotation;
    RecalculateMatrices();
  }
  float GetRotation() const { return m_rotation; }

  const glm::mat4 &GetProjectionMat() const { return m_projectionMat; }
  const glm::mat4 &GetViewMat() const { return m_viewMat; }
  const glm::mat4 &GetProjectionViewMat() const { return m_viewProjectionMat; }

  const glm::vec3 &GetTarget() const { return m_target; }
  void SetTarget(const glm::vec3 &target)
  {
    m_target = target;
    RecalculateMatrices();
  }

private:
  void RecalculateMatrices();
  void RecalculateMatricesForMouse();

private:
  glm::mat4 m_projectionMat;
  glm::mat4 m_viewMat;
  glm::mat4 m_viewProjectionMat;

  glm::vec3 m_target = {0.0f, 0.0f, -1.0f};

  glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
  float m_rotation = 0.0f;
};
} // namespace Soda