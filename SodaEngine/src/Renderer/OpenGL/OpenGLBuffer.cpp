#include "SD_PCH.h"

#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Soda
{
/*****************************************************************/
/* Vertex Index **************************************************/
/*****************************************************************/

GLVertexBuffer::GLVertexBuffer(uint32_t size)
{
  glCreateBuffers(1, &m_bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);

  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(const float *vertices, uint32_t size)
{
  glCreateBuffers(1, &m_bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);

  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer() { glDeleteBuffers(1, &m_bufferID); }

void GLVertexBuffer::SetData(const void *data, uint32_t dataSize,
                             uint32_t offset)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
  glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

void GLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_bufferID); }

void GLVertexBuffer::Unbind() const { glBindBuffer(GL_VERTEX_ARRAY, 0); }

/*****************************************************************/
/* Index Buffer **************************************************/
/*****************************************************************/

GLIndexBuffer::GLIndexBuffer(uint32_t *indices, uint32_t count)
    : m_indexes(count)
{
  glCreateBuffers(1, &m_bufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() { glDeleteBuffers(1, &m_bufferID); }

void GLIndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void GLIndexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
} // namespace Soda