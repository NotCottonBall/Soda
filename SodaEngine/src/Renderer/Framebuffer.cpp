#include "SD_PCH.h"

#include "Framebuffer.h"

#include "OpenGL/OpenGLFramebuffer.h"
#include "Renderer/Render.h"

namespace Soda
{
Ref<Framebuffer> Framebuffer::Create(const FramebufferInfo &framebufferInfo)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return CreateRef<OpenGLFramebuffer>(framebufferInfo);
  }

  SD_ENGINE_ASSERT(!nullptr, "Invalid API Selection :O");
  return nullptr;
}
} // namespace Soda