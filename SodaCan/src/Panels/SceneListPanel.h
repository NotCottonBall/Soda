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

  Object GetSelectedObject() const { return m_SelectedObj; }
  void SetSelectedObject(const Object &obj) { m_SelectedObj = obj; }

private:
  Ref<Scene> m_CurrentScene;
  Object m_SelectedObj;

  friend class Panels;
};
} // namespace Soda