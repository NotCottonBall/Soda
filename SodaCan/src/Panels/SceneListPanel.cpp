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
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

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

// Drawing Components //
template <typename T, typename UI>
static void DrawComponent(const std::string &name, Object obj, UI ui)
{
  ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth |
      ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding |
      ImGuiTreeNodeFlags_AllowOverlap;
  if(obj.HasComponent<T>())
  {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    float lineHeight =
        GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    bool removeComponent = false;
    bool isOpen =
        ImGui::TreeNodeEx((void *)typeid(T).hash_code(), flags, name.c_str());
    ImGui::PopStyleVar();

    ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
    if(ImGui::Button(":", ImVec2(lineHeight, lineHeight)))
      ImGui::OpenPopup("ComponentOptions");

    if(ImGui::BeginPopup("ComponentOptions"))
    {
      if(ImGui::MenuItem("Remove Component"))
        removeComponent = true;
      if(ImGui::MenuItem("Reset Component"))
      {
        obj.GetComponent<T>().Reset();
      }

      ImGui::EndPopup();
    }

    if(isOpen)
    {
      auto &component = obj.GetComponent<T>();
      ui(component);
      ImGui::TreePop();
    }

    if(removeComponent)
    {
      obj.DeleteComponent<T>();
      removeComponent = false;
    }
  }
}

void SceneListPanel::DrawObjectProperties(Object obj)
{
  // base components //
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
  if(obj.HasComponent<TagComponent>())
  {
    auto &tag = obj.GetComponent<TagComponent>().Tag;

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    strcpy_s(buffer, sizeof(buffer), tag.c_str());

    if(ImGui::InputText("Tag", buffer, sizeof(buffer)))
      tag = std::string(buffer);
  }

  // our components //
  DrawComponent<TransformComponent>("Transform", obj, [](auto &transform) {
    glm::vec3 rotation = glm::degrees(transform.Rotation);

    SodaGui::DrawVec3Gui("Position", transform.Position, 0.0f);
    SodaGui::DrawVec3Gui("Rotation", rotation, 0.0f);
    SodaGui::DrawVec3Gui("Scale", transform.Scale, 1.0f);

    transform.Rotation = glm::radians(rotation);
  });

  DrawComponent<CameraComponent>("Camera", obj, [](auto &camera) {
    if(ImGui::Button("Recalculate Camera"))
      camera.Camera.SetViewport();
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
  });

  DrawComponent<SpriteComponent>("Sprite", obj, [](auto &sprite) {
    ImGui::ColorEdit4("color", glm::value_ptr(sprite.Color));

    ImGui::Text("Texture");
    if(ImGui::ImageButton(
           sprite.Texture ? (void *)sprite.Texture->GetTextureID() : nullptr,
           ImVec2(50.0f, 50.0f), ImVec2(0, 1), ImVec2(1, 0), 1))
    {
      // @TODO: we need a dialog to select a texture
      Ref<Texture2D> m_tex =
          Texture2D::Create("SodaCan/assets/textures/TheSodaCan.png");
      sprite.Texture = m_tex;
    }
  });
}
} // namespace Soda