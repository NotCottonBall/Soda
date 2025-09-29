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
  ShowEditorSettings = BIT(0)
};
enum ViewWindowsEnum : char
{
  ViewWindowsEnum_None = 0,
  ShowRenderStatsWindow = BIT(0),
  ShowProfilerWindow = BIT(1) // for future settings
};

class Panels
{
public:
  Panels() = default;

  void SetScene(const Ref<Scene> &Scene);
  void OnImGuiRender();

  void ToggleEditWindows(EditWindowsEnum option);
  void ToggleViewWindows(ViewWindowsEnum option);

  SceneListPanel &GetSceneListPanel() { return m_SceneListsPanel; }
  const SceneListPanel &GetSceneListPanel() const { return m_SceneListsPanel; }

private:
  SceneListPanel m_SceneListsPanel;
  EditWindowsEnum m_DefaultEditWindowOption =
      EditWindowsEnum::EditWindowsEnum_None;
  ViewWindowsEnum m_DefaultViewWindowOption =
      ViewWindowsEnum::ViewWindowsEnum_None;
};
} // namespace Soda