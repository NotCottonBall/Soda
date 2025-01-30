#pragma once

#include "RenderCommand.h"

#include "Shaderer.h"
#include "Soda/Renderer/Camera.h"

#include "glm/glm.hpp"

namespace Soda
{
class Renderer
{
public:
  static void Init(unsigned int width, unsigned int height);

  static void StartScene(const OrthoCamera &camera);
  static void StartScene(const PerspectiveCamera &camera);
  static void StopScene();

  // Resize callback function
  static void OnWindowResize(unsigned int width, unsigned int height);

  static void PushThis(const Ref<VertexArray> &VA, const Ref<Shader> &shader,
                       const glm::mat4 &transform = glm::mat4(1.0f));

  static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

private:
  struct SceneData
  {
    glm::mat4 ProjectionViewMat;
    glm::vec3 CameraPosition;
  };

  static SceneData *m_SceneData;
};
} // namespace Soda