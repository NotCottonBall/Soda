#include "SceneListPanel.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#
#include "imgui.h"

#include "../GUI/SodaGui.h"

namespace Soda
{
void SceneListPanel::DrawListObject(Object obj, const std::string &name)
{
  ImGuiTreeNodeFlags flags =
      ((m_SelectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) |
      ImGuiTreeNodeFlags_OpenOnArrow;

  bool opened =
      ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)obj, flags, name.c_str());
  if(ImGui::IsItemClicked())
  {
    m_SelectedObj = obj;
  }

  bool m_DeleteQueued = false;
  // when an item is right clicked
  if(ImGui::BeginPopupContextItem())
  {
    if(ImGui::MenuItem("Delete Object"))
      m_DeleteQueued = true;

    ImGui::EndPopup();
  }

  if(opened)
  {
    ImGui::TreePop();
  }

  if(m_DeleteQueued)
  {
    m_CurrentScene->DestroyObject(obj);
    m_SelectedObj = {};
  }
}

void SceneListPanel::DrawObjectProperties(Object obj)
{
  if(obj.HasComponent<NameComponent>())
  {
    auto &name = obj.GetComponent<NameComponent>().Name;

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    strcpy_s(buffer, sizeof(buffer), name.c_str());

    if(ImGui::InputText("Name", buffer, sizeof(buffer)))
      name = std::string(buffer);

    ImGui::Spacing();
  }

  if(obj.HasComponent<TransformComponent>())
  {
    if(ImGui::TreeNodeEx((void *)typeid(TransformComponent).hash_code(),
                         ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
    {
      auto &transform = obj.GetComponent<TransformComponent>();

      glm::vec3 rotation = glm::degrees(transform.Rotation);

      SodaGui::DrawVec3Gui("Position", transform.Position, 0.0f);
      SodaGui::DrawVec3Gui("Rotation", rotation, 0.0f);
      SodaGui::DrawVec3Gui("Scale", transform.Scale, 1.0f);

      transform.Rotation = glm::radians(rotation);

      ImGui::TreePop();
    }
  }

  if(obj.HasComponent<CameraComponent>())
  {
    // @LEARN: why the fuck do we do what we did in this if condition?
    if(ImGui::TreeNodeEx((void *)typeid(CameraComponent).hash_code(),
                         ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
    {
      auto &camera = obj.GetComponent<CameraComponent>();

      if(ImGui::Button("Recalculate Camera"))
      {
        obj.GetComponent<CameraComponent>().Camera.SetViewport();
      }
      ImGui::Checkbox("Primary", &camera.PrimaryCamera);
      ImGui::Checkbox("Fixed Aspect Ratio", &camera.FixedAspectRatio);

      const char *cameraTypes[] = {"Orthographic", "Perspective"};
      const char *cameraTypeToString =
          cameraTypes[(int)camera.Camera.GetCameraType()];

      if(ImGui::BeginCombo("Type", cameraTypeToString))
      {
        for(int i = 0; i < 2; i++)
        {
          if(ImGui::Selectable(cameraTypes[i]))
          {
            cameraTypeToString = cameraTypes[i];
            camera.Camera.SetCameraType((SceneCamera::CameraType)i);
          }
        }

        ImGui::EndCombo();
      }

      if(camera.Camera.GetCameraType() == SceneCamera::CameraType::Orthographic)
      {
        float orthoCamSize = camera.Camera.GetOrthoCameraSize();
        if(ImGui::DragFloat("View Size", &orthoCamSize))
          camera.Camera.SetOrthoCameraSize(orthoCamSize);

        float orthoNearPlane = camera.Camera.GetOrthoNearPlane();
        if(ImGui::DragFloat("Near Plane", &orthoNearPlane))
          camera.Camera.SetOrthoNearPlane(orthoNearPlane);

        float orthoFarPlane = camera.Camera.GetOrthoFarPlane();
        if(ImGui::DragFloat("Far Plane", &orthoFarPlane))
          camera.Camera.SetOrthoFarPlane(orthoFarPlane);
      }
      else
      {
        float perspFov = glm::degrees(camera.Camera.GetPerspectiveFov());
        if(ImGui::DragFloat("Fov", &perspFov))
          camera.Camera.SetPerspectiveFov(perspFov);

        float perspNearPlane = camera.Camera.GetPerspectiveNearPlane();
        if(ImGui::DragFloat("Near Plane", &perspNearPlane))
          camera.Camera.SetPerspectiveNearPlane(perspNearPlane);

        float perspFarPlane = camera.Camera.GetPerspectiveFarPlane();
        if(ImGui::DragFloat("Far Plane", &perspFarPlane))
          camera.Camera.SetPerspectiveFarPlane(perspFarPlane);
      }

      ImGui::TreePop();
    }
  }

  if(obj.HasComponent<SpriteComponent>())
  {
    bool removeComponent = false;

    bool isOpen = ImGui::TreeNodeEx((void *)typeid(SpriteComponent).hash_code(),
                                    ImGuiTreeNodeFlags_DefaultOpen, "Sprite");

    ImGui::SameLine(ImGui::GetWindowWidth() - 45.0f);
    if(ImGui::Button("-"))
      removeComponent = true;

    ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
    if(ImGui::Button("+"))
    {
      if(ImGui::BeginMenu("ItemOptions"))
      {
        if(ImGui::MenuItem("Remove Component"))
          removeComponent = true;
      }
    }

    if(isOpen)
    {
      auto &sprite = obj.GetComponent<SpriteComponent>();

      if(ImGui::ColorEdit4("color", glm::value_ptr(sprite.Color)))
        obj.GetComponent<SpriteComponent>().Color = sprite.Color;

      ImGui::Text("Texture");
      if(ImGui::ImageButton(
             sprite.Texture ? (void *)sprite.Texture->GetTextureID() : nullptr,
             ImVec2(50.0f, 50.0f), ImVec2(0, 1), ImVec2(1, 0), 1))
      {
        // @TODO: we need a dialog to select a texture
      }
      ImGui::TreePop();
    }

    if(removeComponent)
    {
      obj.DeleteComponent<SpriteComponent>();
      removeComponent = false;
    }
  }
}
} // namespace Soda