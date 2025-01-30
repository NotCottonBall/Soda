#include "SD_PCH.h"

#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Soda
{
OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo &framebufferInfo)
    : m_FramebufferInfo(framebufferInfo)
{
  Refresh();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
  glDeleteFramebuffers(1, &m_FramebufferID);
  glDeleteTextures(1, &m_FrameTextureID);
  glDeleteTextures(1, &m_DepthBufferID);
}

void OpenGLFramebuffer::Redo(uint32_t width, uint32_t height)
{
  glDeleteFramebuffers(1, &m_FramebufferID);
  glDeleteTextures(1, &m_FrameTextureID);
  glDeleteTextures(1, &m_DepthBufferID);

  m_FramebufferInfo.width = width;
  m_FramebufferInfo.height = height;

  Refresh();
}

void OpenGLFramebuffer::Bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
  glViewport(0, 0, m_FramebufferInfo.width, m_FramebufferInfo.height);
}

void OpenGLFramebuffer::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::Refresh()
{
  // this is the texture that gets     into our framebuffer and on the screen
  glCreateFramebuffers(1, &m_FramebufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

  glCreateTextures(GL_TEXTURE_2D, 1, &m_FrameTextureID);
  glBindTexture(GL_TEXTURE_2D, m_FrameTextureID);
  // we use glTexImage2D because it allows us to ssample our texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferInfo.width,
               m_FramebufferInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_FrameTextureID, 0);

  // this is the depth part of the framebuffer
  glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferID);
  glBindTexture(GL_TEXTURE_2D, m_DepthBufferID);
  // because we dont wanna mod our depth part of the texture we do the normal
  // glTexStorage2D which just stores the texture?
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_FramebufferInfo.width,
                 m_FramebufferInfo.height);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, m_DepthBufferID, 0);

  // @FIX: this gets called when the scene viewport is < 0 pixels in width or
  // height
  SD_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                       GL_FRAMEBUFFER_COMPLETE,
                   "The framebuffer was not created properly");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace Soda