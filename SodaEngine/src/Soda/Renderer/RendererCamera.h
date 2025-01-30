#pragma once

#include "glm/glm.hpp"

namespace Soda
{
class RendererCamera
{
public:
  RendererCamera() = default;
  RendererCamera(const glm::mat4 &projection) : m_Projection(projection) {}
  virtual ~RendererCamera() = default;

  const glm::mat4 &GetProjection() const { return m_Projection; }

protected:
  glm::mat4 m_Projection = glm::mat4(1.0f);
};
} // namespace Soda