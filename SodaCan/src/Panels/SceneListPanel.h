#pragma once

#include "Soda.h"

namespace Soda
{
class SceneListPanel
{
public:
  SceneListPanel() = default;

  void DrawListObject(Object obj, const std::string &name);

  // @TEMP: move every panel to a different class
  void DrawObjectProperties(Object obj);

private:
  Ref<Systems> m_CurrentSystem;
  Object m_SelectedObj;

  friend class Panels;
};
} // namespace Soda