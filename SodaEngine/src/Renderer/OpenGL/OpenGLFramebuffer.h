#pragma once

#include "Core/Core.h"
#include "Renderer/Framebuffer.h"

namespace Soda
{
class OpenGLFramebuffer : public Framebuffer
{
public:
  OpenGLFramebuffer(const FramebufferInfo &framebufferInfo);
  virtual ~OpenGLFramebuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void Refresh(uint32_t width, uint32_t height) override;

  virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override
  {
    SD_ENGINE_ASSERT(
        index < m_ColorAttachmentIDs.size(),
        "Trying To Access Index Of The Color Attachment That Is Greater Than "
        "The Current Size Of Color Attachment Indexes");
    return m_ColorAttachmentIDs[index];
  }
  virtual const FramebufferInfo &GetFramebufferInfo() const override
  {
    return m_FramebufferInfo;
  }

private:
  void Refresh();

private:
  FramebufferInfo m_FramebufferInfo;

  std::vector<FramebufferTextureSpecifications> m_ColorAttachmentsTextures;
  FramebufferTextureSpecifications m_DepthAttachmentTexture;

  uint32_t m_FramebufferID;
  std::vector<uint32_t> m_ColorAttachmentIDs;
  uint32_t m_DepthAttachmentID;
};
} // namespace Soda