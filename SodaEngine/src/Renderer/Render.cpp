#include "SD_PCH.h"

#include "Render.h"
#include "Renderer2D.h"

namespace Soda
{
Renderer::SceneData *Renderer::m_SceneData = new SceneData;

void Renderer::Init(unsigned int width, unsigned int height)
{
  RenderCommand::Init(width, height);
  Soda::Renderer2D::Init();
}

void Renderer::StartScene(const OrthoCamera &camera)
{
  m_SceneData->ProjectionViewMat = camera.GetProjectionViewMat();
}
void Renderer::StartScene(const PerspectiveCamera &camera)
{
  m_SceneData->ProjectionViewMat = camera.GetProjectionViewMat();
  m_SceneData->CameraPosition = camera.GetPosition();
}

void Renderer::StopScene() {}

void Renderer::OnWindowResize(unsigned int width, unsigned int height)
{
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::PushThis(const Ref<VertexArray> &VA, const Ref<Shader> &shader,
                        const glm::mat4 &transform)
{
  shader->Bind();
  VA->Bind();

  shader->SetUniformMat4("u_PVMat", m_SceneData->ProjectionViewMat);
  shader->SetUniformMat4("u_ModelMat", transform);
  shader->SetUniformVec3("u_ViewPos", m_SceneData->CameraPosition);

  RenderCommand::DrawThis(VA);
}
} // namespace Soda