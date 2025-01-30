#pragma once

#include "glm/glm.hpp"

#include "VertexArray.h"

namespace Soda
{
class RenderAPI
{
public:
  enum class API
  {
    None = 0,
    OpenGL = 1
  };

public:
  virtual void Init(unsigned int width, unsigned int height) = 0;
  virtual void ClearScreen(const glm::vec4 &color = {1.0f, 0.0f, 0.8f,
                                                     1.0f}) = 0;
  virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height) = 0;
  virtual void DrawThis(const Ref<VertexArray> &VA,
                        uint32_t indicesCount = 0) = 0;
  inline static API GetAPI() { return m_API; }

private:
  static API m_API;
};
} // namespace Soda