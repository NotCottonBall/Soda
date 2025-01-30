#include "SD_PCH.h"

#include "Camera.h"

namespace Soda
{
void OrthoCamera::RecalculateMatrices()
{
  glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), m_position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation),
                                      glm::vec3(0, 0, 1));

  m_viewMat = glm::inverse(m_transform);
  m_viewProjectionMat = m_projectionMat * m_viewMat;
}

void OrthoCamera::SetProjection(float left, float right, float down, float up)
{
  m_projectionMat = glm::ortho(left, right, down, up, -1.0f, 1.0f);
  m_viewProjectionMat = m_projectionMat * m_viewMat;
}

void PerspectiveCamera::RecalculateMatrices()
{
  glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), m_position);

  m_viewMat = glm::lookAt(m_position, m_position + m_target,
                          glm::vec3(0.0f, 1.0f, 0.0f));
  m_viewProjectionMat = m_projectionMat * m_viewMat;
}
} // namespace Soda