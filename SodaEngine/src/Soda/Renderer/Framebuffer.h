#pragma once

#include "Soda/_Main/Core.h"

namespace Soda
{
struct FramebufferInfo
{
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

  virtual void Redo(uint32_t width, uint32_t height) = 0;

  virtual uint32_t GetFrameTextureID() const = 0;
  virtual const FramebufferInfo &GetFramebufferInfo() const = 0;

  static Ref<Framebuffer> Create(const FramebufferInfo &framebufferInfo);
};
} // namespace Soda