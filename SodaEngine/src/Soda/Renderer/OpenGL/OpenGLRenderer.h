#pragma once

#include "Soda/Renderer/RenderAPI.h"

namespace Soda
{
class OpenGLRenderer : public RenderAPI
{
  virtual void Init(unsigned int width, unsigned int height) override;

  virtual void ClearScreen(const glm::vec4 &color = {1.0f, 0.0f, 0.8f,
                                                     1.0f}) override;

  virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height) override;

  virtual void DrawThis(const Ref<VertexArray> &VA,
                        uint32_t indicesCount = 0) override;
};
} // namespace Soda