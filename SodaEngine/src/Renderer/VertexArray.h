#pragma once

#include "SD_PCH.h"

#include "Renderer/Bufferer.h"

namespace Soda
{
class VertexArray
{
public:
  VertexArray() {}
  virtual ~VertexArray() {}

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;
  virtual void AddIndexBuffer(const Ref<IndexBuffer> &indexBuffer) = 0;

  virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;
  virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

  static VertexArray *Create();
};
} // namespace Soda