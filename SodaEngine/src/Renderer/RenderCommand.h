#pragma once

#include "RenderAPI.h"

namespace Soda
{
class RenderCommand
{
public:
  inline static void Init(unsigned int width, unsigned int height)
  {
    m_renderAPI->Init(width, height);
  }
  inline static void ClearScreen(const glm::vec4 &color = {1.0f, 0.0f, 0.8f,
                                                           1.0f})
  {
    m_renderAPI->ClearScreen(color);
  }
  inline static void SetViewport(unsigned int x, unsigned int y,
                                 unsigned int width, unsigned int height)
  {
    m_renderAPI->SetViewport(0, 0, width, height);
  }
  inline static void DrawThis(const Ref<VertexArray> &VA,
                              uint32_t indicesCount = 0)
  {
    m_renderAPI->DrawThis(VA);
  }

private:
  static RenderAPI *m_renderAPI;
};
} // namespace Soda