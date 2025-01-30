#pragma once

#include "SD_PCH.h"

namespace Soda
{
/* Data Types and size of the Types */

enum class ShaderDataType
{
  Int,
  Float,

  IVec2,
  Vec2,
  Mat2,
  IVec3,
  Vec3,
  Mat3,
  IVec4,
  Vec4,
  Mat4,
};
static uint32_t GetSDTSize(ShaderDataType sdt)
{
  switch(sdt)
  {
  case ShaderDataType::Int:
    return 4;
  case ShaderDataType::Float:
    return 4;

  case ShaderDataType::IVec2:
    return 4 * 2;
  case ShaderDataType::IVec3:
    return 4 * 3;
  case ShaderDataType::IVec4:
    return 4 * 4;

  case ShaderDataType::Vec2:
    return 4 * 2;
  case ShaderDataType::Vec3:
    return 4 * 3;
  case ShaderDataType::Vec4:
    return 4 * 4;

  case ShaderDataType::Mat2:
    return 4 * 2 * 2;
  case ShaderDataType::Mat3:
    return 4 * 3 * 3;
  case ShaderDataType::Mat4:
    return 4 * 4 * 4;
  }

  // SD_ENGINE_ASSERT("No Correct ShaderDataType Found");
}

// A Buffer's Attrib Values
struct BufferAttrib
{
  BufferAttrib() {}

  std::string m_name;
  ShaderDataType m_type;
  uint32_t m_size;
  uint32_t m_offset;
  bool m_normalize;

  BufferAttrib(const std::string &name, ShaderDataType shDataType,
               bool normalize = false)
      : m_name(name), m_type(shDataType), m_offset(0),
        m_size(GetSDTSize(m_type)), m_normalize(normalize)
  {
  }

  uint32_t GetAttribCount() const
  {
    switch(m_type)
    {
    case ShaderDataType::Int:
      return 1;
    case ShaderDataType::Float:
      return 1;

    case ShaderDataType::IVec2:
      return 2;
    case ShaderDataType::IVec3:
      return 3;
    case ShaderDataType::IVec4:
      return 4;

    case ShaderDataType::Vec2:
      return 2;
    case ShaderDataType::Vec3:
      return 3;
    case ShaderDataType::Vec4:
      return 4;

    case ShaderDataType::Mat2:
      return 2 * 2;
    case ShaderDataType::Mat3:
      return 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4;
    }

    // SD_ENGINE_ASSERT("No ShaderDataType Found");
  }
};

// The Loadout of the Buffer's Data
class BufferLoadout
{
public:
  BufferLoadout() {}

  BufferLoadout(const std::initializer_list<BufferAttrib> &bufferAttribs)
      : m_BufferAttribs(bufferAttribs)
  {
    CalcAttribValues();
  }

  inline const std::vector<BufferAttrib> &GetAttribs() const
  {
    return m_BufferAttribs;
  }

  std::vector<BufferAttrib>::iterator begin()
  {
    return m_BufferAttribs.begin();
  }
  std::vector<BufferAttrib>::iterator end() { return m_BufferAttribs.end(); }

  std::vector<BufferAttrib>::const_iterator begin() const
  {
    return m_BufferAttribs.begin();
  }
  std::vector<BufferAttrib>::const_iterator end() const
  {
    return m_BufferAttribs.end();
  }

  inline uint32_t GetStride() const { return m_stride; }

private:
  void CalcAttribValues()
  {
    uint32_t offset = 0;

    for(BufferAttrib &element : m_BufferAttribs)
    {
      element.m_offset = offset;
      offset += element.m_size;

      m_stride += element.m_size;
    }
  }

  std::vector<BufferAttrib> m_BufferAttribs;
  uint32_t m_stride = 0;
};

/* Buffers */

class VertexBuffer
{
public:
  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const void *data, uint32_t dataSize,
                       uint32_t offset = 0) = 0;

  virtual const BufferLoadout &GetLoadout() const = 0;
  virtual void SetLoadout(const BufferLoadout &loadout) = 0;

  static Ref<VertexBuffer> Create(uint32_t size);
  static Ref<VertexBuffer> Create(const float *vertices, uint32_t size);
};

class IndexBuffer
{
public:
  static Ref<IndexBuffer> Create(uint32_t *indices, uint32_t count);

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;
};
} // namespace Soda