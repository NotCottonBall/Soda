#pragma once

#include "Soda/Renderer/Bufferer.h"

namespace Soda
{
class GLVertexBuffer : public VertexBuffer
{
public:
  GLVertexBuffer(uint32_t size);
  GLVertexBuffer(const float *vertices, uint32_t size);
  ~GLVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void SetData(const void *data, uint32_t dataSize,
                       uint32_t offset = 0) override;

  virtual const BufferLoadout &GetLoadout() const override { return m_loadout; }
  virtual void SetLoadout(const BufferLoadout &loadout) override
  {
    m_loadout = loadout;
  }

private:
  uint32_t m_bufferID;

  BufferLoadout m_loadout;
};

class GLIndexBuffer : public IndexBuffer
{
public:
  GLIndexBuffer(uint32_t *indices, uint32_t size);
  ~GLIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual uint32_t GetCount() const override { return m_indexes; }

private:
  uint32_t m_bufferID;

  uint32_t m_indexes;
};
} // namespace Soda