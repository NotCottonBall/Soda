#include "SD_PCH.h"

#include "OpenGLFramebuffer.h"

#include "Core/Events/Events.h"
#include "Tools/Debug/Logger.h"

#include "glad/glad.h"

namespace Soda
{
// File Functions
static GLenum GetTextureTarget(bool multisample)
{
  return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateFramebufferTexture(const size_t index, const uint32_t samples,
                                     const uint32_t id,
                                     const GLenum internalFormat,
                                     const uint32_t width,
                                     const uint32_t height)
{
  bool multisampled = samples > 1;
  if(multisampled)
  {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat,
                            width, height, GL_FALSE);
  }
  else
  {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         GetTextureTarget(multisampled), id, 0);
}

static void CreateFramebufferDepthTexture(
    const uint32_t id, const uint32_t samples, const GLenum internalFormat,
    const GLenum attachmentType, const uint32_t width, const uint32_t height)
{
  bool multisampled = samples > 1;
  if(multisampled)
  {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat,
                            width, height, GL_FALSE);
  }
  else
  {
    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType,
                         GetTextureTarget(multisampled), id, 0);
}

// CLass Functions
OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo &framebufferInfo)
    : m_FramebufferInfo(framebufferInfo)
{
  for(auto &tex : m_FramebufferInfo.Attachments.Textures)
  {
    if(tex.Format < FramebufferTextureFormat::Depth24Stensil8)
      m_ColorAttachmentsTextures.emplace_back(tex);
    else
      m_DepthAttachmentTexture = tex;
  }

  Refresh();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
  glDeleteFramebuffers(1, &m_FramebufferID);
  glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
  glDeleteTextures(1, &m_DepthAttachmentID);
}

void OpenGLFramebuffer::Refresh(uint32_t width, uint32_t height)
{
  m_FramebufferInfo.width = width;
  m_FramebufferInfo.height = height;

  if(m_FramebufferID)
  {
    glDeleteFramebuffers(1, &m_FramebufferID);
    glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
    glDeleteTextures(1, &m_DepthAttachmentID);

    m_ColorAttachmentIDs.clear();
    m_DepthAttachmentID = 0;
  }

  Refresh();
}

void OpenGLFramebuffer::Refresh()
{
  // this is the texture that gets into our framebuffer and on the screen
  glCreateFramebuffers(1, &m_FramebufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
  bool multisampled = m_FramebufferInfo.samples > 1;

  // Creating Framebuffer and it's color attachments
  if(!m_ColorAttachmentsTextures.empty())
  {
    m_ColorAttachmentIDs.resize(m_ColorAttachmentsTextures.size());
    glCreateTextures(GetTextureTarget(multisampled),
                     m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());

    for(size_t i = 0; i < m_ColorAttachmentIDs.size(); i++)
    {
      glBindTexture(GetTextureTarget(multisampled), m_ColorAttachmentIDs[i]);
      switch(m_ColorAttachmentsTextures[i].Format)
      {
      case FramebufferTextureFormat::RGBA8: {
        CreateFramebufferTexture(
            i, m_FramebufferInfo.samples, m_ColorAttachmentIDs[i], GL_RGBA8,
            m_FramebufferInfo.width, m_FramebufferInfo.height);
        break;
      }
      }
    }
  }

  // Creating the depth attachment if needed
  if(m_DepthAttachmentTexture.Format != FramebufferTextureFormat::None)
  {
    glCreateTextures(GetTextureTarget(multisampled), 1, &m_DepthAttachmentID);
    glBindTexture(GetTextureTarget(multisampled), m_DepthAttachmentID);

    switch(m_DepthAttachmentTexture.Format)
    {
    case FramebufferTextureFormat::Depth24Stensil8: {
      CreateFramebufferDepthTexture(
          m_DepthAttachmentID, m_FramebufferInfo.samples, GL_DEPTH24_STENCIL8,
          GL_DEPTH_STENCIL_ATTACHMENT, m_FramebufferInfo.width,
          m_FramebufferInfo.height);
      break;
    }
    }
  }

  if(m_ColorAttachmentIDs.size() > 1)
  {
    SD_ENGINE_ASSERT(
        m_ColorAttachmentIDs.size() <= 4,
        "Trying To Create More Than 4 Color Attachments Is Not Allowed!");

    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(m_ColorAttachmentIDs.size(), buffers);
  }
  else if(m_ColorAttachmentIDs.empty())
  {
    glDrawBuffers(0, nullptr);
  }

  // @FIX: this gets called when the scene viewport is < 0 pixels in width
  // or height
  SD_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                       GL_FRAMEBUFFER_COMPLETE,
                   "The framebuffer was not created properly");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
  glViewport(0, 0, m_FramebufferInfo.width, m_FramebufferInfo.height);
}

void OpenGLFramebuffer::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
} // namespace Soda