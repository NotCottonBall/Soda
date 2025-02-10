#include "AppLayer.h"

#include <glm/glm.hpp>
#include <imgui.h>

#include "Panels/Panels.h"
#include "SodaInput.h"

namespace Soda
{
SodaCan::SodaCan() : Layer("SodaCan"), m_EditorCamera(1280.0f / 720.0f, false)
{
}

void SodaCan::OnAttach()
{
  FramebufferInfo m_FramebufferInfo;
  m_FramebufferInfo.width = 1280;
  m_FramebufferInfo.height = 720;
  m_Framebuffer = Framebuffer::Create(m_FramebufferInfo);

  m_Scene = CreateRef<Scene>();

  m_Square = m_Scene->CreateObject("Square");
  m_Square.AddComponent<SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  m_GameCamera = m_Scene->CreateObject("EditorCamera");
  m_GameCamera.AddComponent<CameraComponent>();

  m_GameCamera.GetComponent<CameraComponent>().Camera.SetOrthoCameraSize(15.0f);

  // scripts
  class CameraController : public ScriptEntity
  {
    void OnStart() {}

    void OnUpdate(Timestep dt) {}

    void OnDestroy() {}
  };
  m_GameCamera.AddComponent<ScriptComponent>().Bind<CameraController>();

  m_Panels.SetScene(m_Scene);
}

void SodaCan::OnUpdate(Timestep dt)
{
  if(FramebufferInfo fInfo = m_Framebuffer->GetFramebufferInfo();
     m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
     (fInfo.width != m_ViewportSize.x || fInfo.height != m_ViewportSize.y))
  {
    m_Framebuffer->Refresh((uint32_t)m_ViewportSize.x,
                           (uint32_t)m_ViewportSize.y);
    m_EditorCamera.WhenResized(m_ViewportSize.x, m_ViewportSize.y);

    m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
  }

  if(m_IsPanelHovered)
    m_EditorCamera.OnUpdate(dt);

  m_Framebuffer->Bind();
  RenderCommand::ClearScreen({0.1f, 0.1f, 0.1f, 1.0f});
  Renderer2D::ResetRendererStats();

  // Render Loop
  Renderer2D::StartScene(m_EditorCamera.GetCamera());
  {
    m_Scene->OnUpdate(dt);
  }
  Renderer2D::StopScene();

  m_Framebuffer->Unbind();
}

void SodaCan::OnEvent(Event &event)
{
  m_EditorCamera.OnEvent(event);

  // if(Soda::Input::IsKeyPressed(SD_KEY_END))
  //   m_Scene->DestroyObject(m_Square2);
  if(Soda::Input::IsKeyPressed(SD_KEY_END))
    m_Square.DeleteComponent<SpriteComponent>();
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
    ImGui::Begin("Scene");
    {
      // Checks is the panel is Focused
      // @FIXME: The Inputs should be consumed by the ImGui Panel and not the
      // mani window.
      m_IsPanelFocused = ImGui::IsWindowFocused();
      m_IsPanelHovered = ImGui::IsWindowHovered();

      App::Get().GetImGuiLayer()->ShouldConsumeEvents(!m_IsPanelHovered);

      // @TODO: put this inside a OnWindowResize Callback somehow
      // because we dont wanna check the scene/viewport each frame,
      // we only wanna do it when we resize.
      ImVec2 sceneSize = ImGui::GetContentRegionAvail();
      m_ViewportSize = {sceneSize.x, sceneSize.y};
      ImGui::Image((void *)m_Framebuffer->GetFrameTextureID(),
                   ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1),
                   ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();
  }

  ImGui::End();
}

void SodaCan::OnDetach() { Renderer2D::Shutdown(); }
} // namespace Soda