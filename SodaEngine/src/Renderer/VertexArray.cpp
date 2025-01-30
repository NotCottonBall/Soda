#include "SD_PCH.h"

#include "OpenGL/OpenGLVertexArray.h"
#include "VertexArray.h"

#include "RenderAPI.h"

namespace Soda
{
VertexArray *VertexArray::Create()
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return new GLVertexArray();
  }

  SD_ENGINE_ASSERT(!nullptr, "Invalid API Selection :O");
  return nullptr;
}
} // namespace Soda