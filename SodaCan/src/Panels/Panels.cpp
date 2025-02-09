#include "Panels.h"

#include "imgui.h"

#include "../GUI/SodaGui.h"

namespace Soda
{
void Panels::SetScene(const Ref<Scene> &scene)
{
  m_SceneListsPanel.m_CurrentScene = scene;
}

void Panels::OnImGuiRender()
{
  ImGui::Begin("Scene List");
  {
    // now because we need to mention nameComponent in the lambda
    // i decided to pass in the std::string Name of the object too instead of
    // making a new var in the DrawListObject() function
    m_SceneListsPanel.m_CurrentScene->m_Registry.view<NameComponent>().each(
        [&](auto obj, auto &nameComponent) {
          Object list(obj, m_SceneListsPanel.m_CurrentScene.get());
          m_SceneListsPanel.DrawListObject(
              list, nameComponent.Name); // dont really know if i should do this
                                         // or just pass in the object
        });

    if(ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
      m_SceneListsPanel.m_SelectedObj = {};

    // when the window region is right clicked
    if(ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
    {
      if(ImGui::BeginMenu("Create"))
      {
        if(ImGui::MenuItem("Empty Object"))
          m_SceneListsPanel.m_CurrentScene->CreateObject("Empty Object");

        if(ImGui::MenuItem("Camera Object"))
        {
          Object obj = m_SceneListsPanel.m_CurrentScene->CreateObject("Camera");
          obj.AddComponent<CameraComponent>();
        }

        if(ImGui::MenuItem("Object 2D"))
        {
          Object obj =
              m_SceneListsPanel.m_CurrentScene->CreateObject("2D Object");
          obj.AddComponent<SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }

        ImGui::EndPopup();
      }

      ImGui::EndPopup();
    }
  }
  ImGui::End();

  ImGui::Begin("Details");
  {
    if(m_SceneListsPanel.m_SelectedObj)
    {
      m_SceneListsPanel.DrawObjectProperties(m_SceneListsPanel.m_SelectedObj);

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Separator();
      ImVec2 regionSpace = ImGui::GetContentRegionAvail();
      ImGui::SetCursorPosX(regionSpace.x * 0.25f);
      // we basicly set the button to draw from the 1/4th of the panel region
      // and we draw our button as 2/4 of the screen region

      if(ImGui::Button("Add Component", ImVec2(regionSpace.x * 0.5f, 25.0f)))
        ImGui::OpenPopup("AddComponent");
      // ImGui::SetCursorPosX(0);

      if(ImGui::BeginPopup("AddComponent"))
      {
        if(ImGui::MenuItem("Transform"))
        {
          m_SceneListsPanel.m_SelectedObj.AddComponent<TransformComponent>();
          ImGui::CloseCurrentPopup();
        }

        if(ImGui::MenuItem("Camera"))
        {
          m_SceneListsPanel.m_SelectedObj.AddComponent<CameraComponent>();
          ImGui::CloseCurrentPopup();
        }

        if(ImGui::MenuItem("Sprite"))
        {
          m_SceneListsPanel.m_SelectedObj.AddComponent<SpriteComponent>(
              glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }
    }
  }
  ImGui::End();

  // other windows //
  if(m_DefaultEditWindowOption & EditWindowsEnum::ShowEditorSettings)
  {
    bool isWindowOpen = true;

    ImGui::Begin("Editor Settings", &isWindowOpen);
    ImGui::Text("This is where all the editor settings will reside");
    ImGui::End();

    if(!isWindowOpen)
      ToggleEditWindows(EditWindowsEnum::ShowEditorSettings);
  }
  if(m_DefaultViewWindowOption & ViewWindowsEnum::ShowRenderStatsWindow)
  {
    bool isWindowOpen = true;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::Begin("Renderer Stats", &isWindowOpen);
    {
      ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
      ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
      ImGui::Text("");
      // renderer2D stats
      Renderer2D::RendererStats stats = Renderer2D::GetRendererStats();

      ImGui::Text("Draw Calls: %d", stats.noOfDrawCalls);
      ImGui::Text("Textures: %d", stats.noIfTextures);
      ImGui::Text("Quads: %d", stats.noOfQuads);
      ImGui::Spacing();
      ImGui::Text("Triangles: %d", stats.QueryNoOfTriangles());
      ImGui::Text("Vertices: %d", stats.QueryNoOfVertices());
      ImGui::Text("Indices: %d", stats.QueryNoOfIndices());
    }
    ImGui::End();
    ImGui::PopStyleColor();

    if(!isWindowOpen)
      ToggleViewWindows(ViewWindowsEnum::ShowRenderStatsWindow);
  }
}

// other functions //
void Panels::ToggleEditWindows(EditWindowsEnum option)
{
  m_DefaultEditWindowOption =
      static_cast<EditWindowsEnum>(m_DefaultEditWindowOption ^ option);
}
void Panels::ToggleViewWindows(ViewWindowsEnum option)
{
  m_DefaultViewWindowOption =
      static_cast<ViewWindowsEnum>(m_DefaultViewWindowOption ^ option);
}
} // namespace Soda