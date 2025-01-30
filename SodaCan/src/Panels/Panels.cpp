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

      if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

      if(ImGui::BeginPopup("AddComponent"))
      {
        if(ImGui::MenuItem("Camera Component"))
        {
          m_SceneListsPanel.m_SelectedObj.AddComponent<CameraComponent>();
          ImGui::CloseCurrentPopup();
        }

        if(ImGui::MenuItem("Sprite Component"))
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

  // ImGui::ShowDemoWindow();
}
} // namespace Soda