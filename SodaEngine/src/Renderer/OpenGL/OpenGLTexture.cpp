#include "SD_PCH.h"

#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace Soda
{
OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
    : m_Path(path), m_Width(0), m_Height(0), m_TextureID(0)
{
  stbi_set_flip_vertically_on_load(1);

  int width, height, channels;
  stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  SD_ENGINE_ASSERT(data, "Failed to load image!");

  m_Width = width;
  m_Height = height;

  int internalFormat = 0, externalFormat = 0;

  if(channels == 4)
  {
    internalFormat = GL_RGBA8;
    externalFormat = GL_RGBA;
  }
  else if(channels == 3)
  {
    internalFormat = GL_RGB8;
    externalFormat = GL_RGB;
  }

  m_InternalFormat = internalFormat;
  m_DataFormat = externalFormat;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
  glTextureStorage2D(m_TextureID, 1, GL_RGBA8, m_Width, m_Height);

  glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGBA,
                      GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
{
  m_DataFormat = GL_RGBA;
  m_InternalFormat = GL_RGBA8;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
  glTextureStorage2D(m_TextureID, 1, GL_RGBA8, m_Width, m_Height);

  glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLTexture2D::SetData(void *data, uint32_t size)
{
  uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
  SD_ENGINE_ASSERT(size == m_Width * m_Height * bytesPerPixel,
                   "Data must be entire texture!");
  glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_DataFormat,
                      GL_UNSIGNED_BYTE, data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_TextureID); }

void OpenGLTexture2D::Bind(uint32_t slot)
{
  glBindTextureUnit(slot, m_TextureID);
}

void OpenGLTexture2D::Unbind(uint32_t slot) { glBindTextureUnit(slot, 0); }
} // namespace Soda