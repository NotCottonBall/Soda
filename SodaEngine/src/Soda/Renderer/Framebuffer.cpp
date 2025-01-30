#include "SD_PCH.h"

#include "Framebuffer.h"

#include "OpenGL/OpenGLFramebuffer.h"
#include "Soda/Renderer/Render.h"

namespace Soda
{
Ref<Framebuffer> Framebuffer::Create(const FramebufferInfo &framebufferInfo)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    // this is the first time this new CreateRef<> is being used (27-02-2024)
    // if i take time to make everything a CreateRef instead of returning a
    // pointer, i will delete this comment
    return CreateRef<OpenGLFramebuffer>(framebufferInfo);
    // i can just check the commit if i wanna know when i added the comment.
    // but this is just a look away
  }

  SD_ENGINE_ASSERT(!nullptr, "Invalid API Selection :O");
  return nullptr;
}
} // namespace Soda