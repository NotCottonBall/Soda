#include "SD_PCH.h"

#include "OpenGLRenderer.h"
#include "Tools/Debug/Logger.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Soda
{
void OpenGLRenderer::Init(unsigned int width, unsigned int height)
{
  glViewport(0, 0, width, height);

  glEnable(GL_BLEND);
  // this should be enabled in 3D only.
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::ClearScreen(const glm::vec4 &color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::SetViewport(unsigned int x, unsigned int y,
                                 unsigned int width, unsigned int height)
{
  glViewport(x, y, width, height);
}

void OpenGLRenderer::DrawThis(const Ref<VertexArray> &VA, uint32_t indicesCount)
{
  if(indicesCount == 0)
    return;
  glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
}
} // namespace Soda