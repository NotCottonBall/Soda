#pragma once

#include <string>

#include "imgui.h"
#include "imgui_internal.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Soda
{
// Gui Spesific code
class SodaGui
{
public:
  static void DrawVec3Gui(const std::string &lable, glm::vec3 &vector,
                          float resetValue = 0.0f, float colWidth = 100.0f);

  static void OpenPopup(const std::string &lable);

private:
  static void Popups();
};
} // namespace Soda