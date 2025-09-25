#pragma once

#include "Core/Core.h"

#include <glm/vec2.hpp>

namespace Soda
{
enum class FramebufferTextureFormat
{
  None = 0,

  // Colors
  RGBA8,
  RED_INT,

  // Depth/Stensil
  Depth24Stensil8, // always put this (Depth24Steensil8) first because this is
                   // what we check while assigning attachment texture inside
                   // API specific code.
};

struct FramebufferTextureSpecifications
{
  FramebufferTextureSpecifications() = default;
  FramebufferTextureSpecifications(FramebufferTextureFormat format)
      : Format(format)
  {
  }

  FramebufferTextureFormat Format = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecifications
{
  FramebufferAttachmentSpecifications() = default;
  FramebufferAttachmentSpecifications(
      std::initializer_list<FramebufferTextureSpecifications> textures)
      : Textures(textures)
  {
  }

  std::vector<FramebufferTextureSpecifications> Textures;
};

struct FramebufferInfo
{
  FramebufferAttachmentSpecifications Attachments;
  uint32_t width;
  uint32_t height;

  uint32_t samples = 1;
};

class Framebuffer
{
public:
  Framebuffer() {}
  virtual ~Framebuffer() {}

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual int Read(uint32_t attachment, const glm::vec2 &pos,
                   const glm::vec2 &viewport) = 0;
  virtual void Refresh(uint32_t width, uint32_t height) = 0;

  virtual uint32_t GetColorAttachmentID(uint32_t index) const = 0;
  virtual const FramebufferInfo &GetFramebufferInfo() const = 0;

  static Ref<Framebuffer> Create(const FramebufferInfo &framebufferInfo);
};
} // namespace Soda