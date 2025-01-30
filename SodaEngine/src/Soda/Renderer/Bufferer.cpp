#include "SD_PCH.h"

#include "Bufferer.h"
#include "RenderAPI.h"

#include "OpenGL/OpenGLBuffer.h"

namespace Soda
{
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return CreateRef<GLVertexBuffer>(size);
  }
}

Ref<VertexBuffer> VertexBuffer::Create(const float *vertices, uint32_t size)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return CreateRef<GLVertexBuffer>(vertices, size);
  }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
{
  switch(RenderAPI::GetAPI())
  {
  case RenderAPI::API::None:
    SD_ENGINE_ASSERT(!nullptr, "No API Selected :/");

  case RenderAPI::API::OpenGL:
    return CreateRef<GLIndexBuffer>(indices, count);
  }
}
} // namespace Soda