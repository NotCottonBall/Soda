#pragma once

#include "Renderer/RenderContext.h"

namespace Soda
{
class SodaGL : public RenderContext
{
public:
  virtual void Init() override;
};
} // namespace Soda