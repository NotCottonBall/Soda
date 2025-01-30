#pragma once

#include "Soda.h"

#include "SceneListPanel.h"

// dont knwo if its a bad practice but i made a pnels class as a container for
// all the panels so that i can just create a panels object in the EditorClass
namespace Soda
{
class Panels
{
public:
  Panels() = default;

  void SetSystem(const Ref<Systems> &system);
  void OnImGuiRender();

private:
  SceneListPanel m_SceneListsPanel;
};
} // namespace Soda