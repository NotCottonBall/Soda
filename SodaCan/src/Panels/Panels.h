#pragma once

#include "Soda.h"

#include "SceneListPanel.h"

// dont knwo if its a bad practice but i made a pnels class as a container for
// all the panels so that i can just create a panels object in the EditorClass
namespace Soda
{
enum EditWindowsEnum : char
{
  EditWindowsEnum_None = 0,
  EditWindowsEnum_ShowEditorSettings = BIT(1)
};

class Panels
{
public:
  Panels() = default;

  void SetScene(const Ref<Scene> &Scene);
  void OnImGuiRender();

  void ToggleEditWindows(EditWindowsEnum option);

private:
  SceneListPanel m_SceneListsPanel;
  EditWindowsEnum m_DefaultEditWindowOption =
      EditWindowsEnum::EditWindowsEnum_None;
};
} // namespace Soda