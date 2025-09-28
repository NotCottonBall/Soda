#include "SD_PCH.h"

#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Soda
{
static GLenum ChangeDataTypeToGLType(ShaderDataType sdt)
{
  switch(sdt)
  {
  case ShaderDataType::Int:
    return GL_INT;
  case ShaderDataType::Float:
    return GL_FLOAT;

  case ShaderDataType::IVec2:
    return GL_INT;
  case ShaderDataType::IVec3:
    return GL_INT;
  case ShaderDataType::IVec4:
    return GL_INT;

  case ShaderDataType::Vec2:
    return GL_FLOAT;
  case ShaderDataType::Vec3:
    return GL_FLOAT;
  case ShaderDataType::Vec4:
    return GL_FLOAT;

  case ShaderDataType::Mat2:
    return GL_FLOAT;
  case ShaderDataType::Mat3:
    return GL_FLOAT;
  case ShaderDataType::Mat4:
    return GL_FLOAT;
  }

  SD_ENGINE_ASSERT(false, "Unknown ShaderDataType");
}

GLVertexArray::GLVertexArray() { glCreateVertexArrays(1, &m_bufferID); }

void GLVertexArray::Bind() const { glBindVertexArray(m_bufferID); }

void GLVertexArray::Unbind() const { glBindVertexArray(0); }

void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
{
  SD_ENGINE_ASSERT(vertexBuffer->GetLoadout().GetAttribs().size(),
                   "You Gave Me NO LOADOUT :(! Did You Call SetLoadout First");

  glBindVertexArray(m_bufferID);
  vertexBuffer->Bind();

  uint32_t index = 0;
  const BufferLoadout &loadout = vertexBuffer->GetLoadout();
  for(const BufferAttrib &attrib : loadout)
  {
    switch(attrib.m_type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Vec2:
    case ShaderDataType::Vec3:
    case ShaderDataType::Vec4:
    case ShaderDataType::Mat2:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, attrib.GetAttribCount(),
                            ChangeDataTypeToGLType(attrib.m_type),
                            attrib.m_normalize ? GL_TRUE : GL_FALSE,
                            loadout.GetStride(), (const void *)attrib.m_offset);
      index++;
      break;
    }
    case ShaderDataType::Int:
    case ShaderDataType::IVec2:
    case ShaderDataType::IVec3:
    case ShaderDataType::IVec4: {
      glEnableVertexAttribArray(index);
      glVertexAttribIPointer(
          index, attrib.GetAttribCount(), ChangeDataTypeToGLType(attrib.m_type),
          loadout.GetStride(), (const void *)attrib.m_offset);
      index++;
      break;
    }
    }
  }

  m_vertexBuffers.push_back(vertexBuffer);
}
void GLVertexArray::AddIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
{
  glBindVertexArray(m_bufferID);
  indexBuffer->Bind();

  m_indexBuffer = indexBuffer;
}
} // namespace Soda