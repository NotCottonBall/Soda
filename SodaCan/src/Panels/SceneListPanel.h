#pragma once

#include "Soda.h"

namespace Soda
{
class SceneListPanel
{
public:
  SceneListPanel() = default;

  void DrawListObject(Object obj, const std::string &name);

  void DrawObjectProperties(Object obj);

  Object GetSelectedObject() const { return m_SelectedObj; }
  void SetSelectedObject(Object obj);

private:
  Ref<Scene> m_CurrentScene;
  Object m_SelectedObj;

  friend class Panels;
};
} // namespace Soda