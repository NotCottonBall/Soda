#include "AppLayer.h"

#include <glm/glm.hpp>
#include <imgui.h>

#include "Editor/SceneSerializer.h"
#include "Panels/Panels.h"
#include "SodaInput.h"

namespace Soda
{
SodaCan::SodaCan() : Layer("SodaCan"), m_EditorCamera(1280.0f / 720.0f) {}

void SodaCan::OnAttach()
{
  FramebufferInfo m_FramebufferInfo;
  m_FramebufferInfo.width = 1280;
  m_FramebufferInfo.height = 720;
  m_EditorFramebuffer = Framebuffer::Create(m_FramebufferInfo);
  m_GameFramebuffer = Framebuffer::Create(m_FramebufferInfo);

  m_Scene = CreateRef<Scene>();

  // m_Square = m_Scene->CreateObject("Square");
  // m_Square.AddComponent<SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  // m_GameCamera = m_Scene->CreateObject("GameCamera");
  // m_GameCamera.AddComponent<CameraComponent>();

  // m_GameCamera.GetComponent<CameraComponent>().Camera.SetOrthoCameraZoom(15.0f);

  // // scripts
  // class CameraController : public ScriptEntity
  // {
  //   void OnStart() {}

  //   void OnUpdate(Timestep dt) {}

  //   void OnDestroy() {}
  // };
  // m_GameCamera.AddComponent<ScriptComponent>().Bind<CameraController>();
  m_Panels.SetScene(m_Scene);
  ImGuizmo::SetOrthographic(false);

  SceneSerializer sceneSerializer(m_Scene);
  sceneSerializer.Deserialize("SodaCan/assets/scenes/main.stscn");
}

void SodaCan::OnUpdate(Timestep dt)
{
  if(FramebufferInfo fInfo = m_GameFramebuffer->GetFramebufferInfo();
     m_GameViewportSize.x > 0.0f && m_GameViewportSize.y > 0.0f &&
     (fInfo.width != m_GameViewportSize.x ||
      fInfo.height != m_GameViewportSize.y))
  {
    m_GameFramebuffer->Refresh((uint32_t)m_GameViewportSize.x,
                               (uint32_t)m_GameViewportSize.y);
    m_Scene->OnGameResize((uint32_t)m_GameViewportSize.x,
                          (uint32_t)m_GameViewportSize.y);
  }
  if(FramebufferInfo fInfo = m_EditorFramebuffer->GetFramebufferInfo();
     m_EditorViewportSize.x > 0.0f && m_EditorViewportSize.y > 0.0f &&
     (fInfo.width != m_EditorViewportSize.x ||
      fInfo.height != m_EditorViewportSize.y))
  {
    m_EditorFramebuffer->Refresh((uint32_t)m_EditorViewportSize.x,
                                 (uint32_t)m_EditorViewportSize.y);
    m_EditorCamera.OnResize(m_EditorViewportSize.x, m_EditorViewportSize.y);
    m_Scene->OnEditorResize((uint32_t)m_EditorViewportSize.x,
                            (uint32_t)m_EditorViewportSize.y, m_EditorCamera);
  }

  if(m_IsScenePanelHovered)
    m_EditorCamera.OnUpdate(dt);

  m_GameFramebuffer->Bind();
  RenderCommand::ClearScreen({0.1f, 0.1f, 0.1f, 1.0f});
  Renderer2D::ResetRendererStats();
  // Game Render Loop
  m_Scene->OnGameUpdate(dt);
  m_GameFramebuffer->Unbind();

  m_EditorFramebuffer->Bind();
  RenderCommand::ClearScreen({0.1f, 0.1f, 0.1f, 1.0f});
  Renderer2D::ResetRendererStats();
  // Editor Render Loop
  m_Scene->OnEditorUpdate(dt, m_EditorCamera);
  m_EditorFramebuffer->Unbind();
}

void SodaCan::OnEvent(Event &event)
{
  m_EditorCamera.OnEvent(event);

  // if(Soda::Input::IsKeyPressed(SD_KEY_END))
  //   m_Scene->DestroyObject(m_Square2);
  // if(Soda::Input::IsKeyPressed(SD_KEY_END))
  //   m_Square.DeleteComponent<SpriteComponent>();
}
void SodaCan::OnResize(uint32_t width, uint32_t height) {}

void SodaCan::OnImGuiUpdate()
{
  static bool dockspaceEnable = true;
  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if(opt_fullscreen)
  {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }
  else
  {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  if(!opt_padding)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", &dockspaceEnable, window_flags);
  if(!opt_padding)
    ImGui::PopStyleVar();

  if(opt_fullscreen)
    ImGui::PopStyleVar(2);

  // Submit the DockSpace
  ImGuiIO &io = ImGui::GetIO();
  if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  if(ImGui::BeginMenuBar())
  {
    if(ImGui::BeginMenu("File"))
    {
      if(ImGui::MenuItem("Close"))
        App::Get().CloseApp();
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Edit"))
    {
      if(ImGui::MenuItem("Editor Settings"))
      {
        m_Panels.ToggleEditWindows(EditWindowsEnum::ShowEditorSettings);
      }
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("View"))
    {
      if(ImGui::MenuItem("Statistics"))
        m_Panels.ToggleViewWindows(ViewWindowsEnum::ShowRenderStatsWindow);
      if(ImGui::MenuItem("Profiler"))
        m_Panels.ToggleViewWindows(ViewWindowsEnum::ShowProfilerWindow);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();

    m_Panels.OnImGuiRender();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // @FIXME: WTF is wrong with scrolling and consuming events?
    // i dont even know if i set it up right
    // i just know that scrolling works when the mouse isnt hovered in other
    // imgui windows.
    ImGui::Begin("Scene");
    {
      m_IsScenePanelFocused = ImGui::IsWindowFocused();
      m_IsScenePanelHovered = ImGui::IsWindowHovered();

      App::Get().GetImGuiLayer()->ShouldConsumeEvents(m_IsScenePanelHovered);

      ImVec2 editorSceneSize = ImGui::GetContentRegionAvail();
      m_EditorViewportSize = {editorSceneSize.x, editorSceneSize.y};
      ImGui::Image((void *)m_EditorFramebuffer->GetFrameTextureID(),
                   ImVec2(m_EditorViewportSize.x, m_EditorViewportSize.y),
                   ImVec2(0, 1), ImVec2(1, 0));

      Object selectedObj = m_Panels.GetSceneListPanel().GetSelectedObject();
      if(selectedObj)
      {
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                          (float)ImGui::GetWindowWidth(),
                          (float)ImGui::GetWindowHeight());

        auto &transform = selectedObj.GetComponent<TransformComponent>();
        glm::mat4 transformComponent = transform.GetTransform();
        glm::vec3 position, rotation, scale;

        ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMat()),
                             glm::value_ptr(m_EditorCamera.GetProjectionMat()),
                             ImGuizmo::OPERATION::TRANSLATE,
                             ImGuizmo::MODE::WORLD,
                             glm::value_ptr(transformComponent));

        // ImGuizmo::Manipulate(
        //     glm::value_ptr(glm::inverse(m_Scene->GetPrimaryCamera()
        //                                     .GetComponent<TransformComponent>()
        //                                     .GetTransform())),
        //     glm::value_ptr(m_Scene->GetPrimaryCamera()
        //                        .GetComponent<CameraComponent>()
        //                        .Camera.GetProjection()),
        //     ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD,
        //     glm::value_ptr(transformComponent));

        if(ImGuizmo::IsUsing())
        {
          ImGuizmo::DecomposeMatrixToComponents(
              glm::value_ptr(transformComponent), glm::value_ptr(position),
              glm::value_ptr(rotation), glm::value_ptr(scale));
          transform.Position = position;
          transform.Rotation = rotation;
          transform.Scale = scale;
        }
      }
    }
    ImGui::End();
    ImGui::Begin("Game");
    {
      m_IsGamePanelFocused = ImGui::IsWindowFocused();
      m_IsGamePanelHovered = ImGui::IsWindowHovered();

      App::Get().GetImGuiLayer()->ShouldConsumeEvents(m_IsGamePanelHovered);

      ImVec2 gameSceneSize = ImGui::GetContentRegionAvail();
      m_GameViewportSize = {gameSceneSize.x, gameSceneSize.y};
      ImGui::Image((void *)m_GameFramebuffer->GetFrameTextureID(),
                   ImVec2(m_GameViewportSize.x, m_GameViewportSize.y),
                   ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();
  }

  ImGui::End();
}

void SodaCan::OnDetach() { Renderer2D::Shutdown(); }
} // namespace Soda