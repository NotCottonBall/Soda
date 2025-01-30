#pragma once

#include "Renderer/VertexArray.h"

namespace Soda
{
class GLVertexArray : public VertexArray
{
public:
  GLVertexArray();
  virtual ~GLVertexArray() {}

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
  virtual void AddIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

  virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override
  {
    return m_vertexBuffers;
  }
  virtual Ref<IndexBuffer> GetIndexBuffer() const override
  {
    return m_indexBuffer;
  }

private:
  std::vector<Ref<VertexBuffer>> m_vertexBuffers;
  Ref<IndexBuffer> m_indexBuffer;

  uint32_t m_bufferID;
};
} // namespace Soda