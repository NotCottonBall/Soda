#pragma once

#include "glm/glm.hpp"

// @NOTE: this is the default camera, the camera base class
// i cant name it just camera because i want to name the variable in the ECS
// Camera (with a capital C) also i want to name all my classes with Capital
// alphabet at the start
namespace Soda
{
class TheCamera
{
public:
  TheCamera() = default;
  TheCamera(const glm::mat4 &projection) : m_Projection(projection) {}
  virtual ~TheCamera() = default;

  const glm::mat4 &GetProjection() const { return m_Projection; }

protected:
  glm::mat4 m_Projection = glm::mat4(1.0f);
};
} // namespace Soda