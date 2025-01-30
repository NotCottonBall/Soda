#pragma once

#include "SD_PCH.h"

#include "Soda/_Main/Core.h"

namespace Soda
{
class Input
{
public:
  // keyboard stuff
  static bool IsKeyPressed(int keycode);

  // mouse stuff
  static bool IsMouseClicked(int button);

  static std::pair<float, float> GetMousePos();
};
} // namespace Soda