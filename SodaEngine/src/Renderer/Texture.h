#pragma once

#include "Core/Core.h"
#include "glm/glm.hpp"

namespace Soda
{
class Textrue
{
public:
  virtual ~Textrue() = default;

  virtual void Bind(uint32_t slot = 0) = 0;
  virtual void Unbind(uint32_t slot = 0) = 0;

  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;

  virtual void SetData(void *data, uint32_t size) = 0;

  virtual const std::string &GetFilePath() const = 0;
  virtual const uint32_t GetTextureID() const = 0;

  virtual const bool operator==(const Textrue &texture) const = 0;
};

class Texture2D : public Textrue
{
public:
  virtual ~Texture2D() = default;

  static std::unique_ptr<Texture2D> Create(uint32_t width, uint32_t height);
  static std::unique_ptr<Texture2D> Create(const std::string &path);
};
} // namespace Soda