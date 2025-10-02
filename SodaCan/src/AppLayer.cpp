#include "AppLayer.h"

#include "Tools/FileDialogs/tinyfiledialogs.h"
#include <glm/glm.hpp>
#include <imgui.h>

#include "Editor/SceneSerializer.h"
#include "Panels/Panels.h"
#include "SodaInput.h"
#include "Tools/Debug/Logger.h"

namespace Utils
{
// clang-format off
static ImGuizmo::OPERATION GizmoToImGuizmoMode(Soda::GizmoTransformMode mode)
{
  switch(mode)
  {
  case Soda::GizmoTransformMode::MouseOnly:
    // Gotta Figure Out How To Handle This
    break;
  case Soda::GizmoTransformMode::Translation: return ImGuizmo::OPERATION::TRANSLATE;
  case Soda::GizmoTransformMode::Rotation:    return ImGuizmo::OPERATION::ROTATE;
  case Soda::GizmoTransformMode::Scale:       return ImGuizmo::OPERATION::SCALE;
  case Soda::GizmoTransformMode::Universal:   return ImGuizmo::OPERATION::UNIVERSAL;
  case Soda::GizmoTransformMode::None: {
    SD_ENGINE_ERROR(
        "Failed To Select A Gizmo Transform Mode So Defaulting To MouseOnly");
    // Default To MouseOnly
  }
  default: {
    // MouseOnly here aswell
  }
  }
}

static ImGuizmo::MODE GizmoToImGuizmoMode(Soda::GizmoOperationMode mode)
{
  switch (mode)
  {
  case Soda::GizmoOperationMode::Local:   return ImGuizmo::MODE::LOCAL;
  case Soda::GizmoOperationMode::Global:  return ImGuizmo::MODE::WORLD;
  case Soda::GizmoOperationMode::None: {
    SD_ENGINE_ERROR(
      "Failed To Select A Gizmo Operation Mode So Defaulting To Local");
    return ImGuizmo::LOCAL;
  }
  default: {
    return ImGuizmo::LOCAL;
  }
  }
}
// clang-format on
} // namespace Utils

namespace Soda
{
SodaCan::SodaCan() : Layer("SodaCan"), m_EditorCamera(16.0f / 9.0f) {}

void SodaCan::OnAttach()
{
  FramebufferInfo m_FramebufferInfo;
  FramebufferAttachmentSpecifications attachments = {
      FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT,
      FramebufferTextureFormat::Depth24Stensil8};
  m_FramebufferInfo.Attachments = attachments;
  m_FramebufferInfo.width = 16;
  m_FramebufferInfo.height = 9;
  m_EditorFramebuffer = Framebuffer::Create(m_FramebufferInfo);
  m_GameFramebuffer = Framebuffer::Create(m_FramebufferInfo);

  m_Scene = CreateRef<Scene>(m_GameViewportSize.x, m_GameViewportSize.y);

  m_Panels.SetScene(m_Scene);
  ImGuizmo::SetOrthographic(false);

  // SceneSerializer sceneSerializer(m_Scene);
  // sceneSerializer.Deserialize("SodaCan/assets/scenes/exampleScene.stscn");
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

  // Game Render Loop
  m_GameFramebuffer->Bind();
  RenderCommand::ClearScreen({0.1f, 0.1f, 0.1f, 1.0f});
  Renderer2D::ResetRendererStats();
  m_Scene->OnGameUpdate(dt);
  m_GameFramebuffer->Unbind();

  // Editor Render Loop
  m_EditorFramebuffer->Bind();
  RenderCommand::ClearScreen({0.1f, 0.1f, 0.1f, 1.0f});
  int fbClearColor = -1;
  m_EditorFramebuffer->ClearColorAttachment(1, &fbClearColor);
  Renderer2D::ResetRendererStats();
  m_Scene->OnEditorUpdate(dt, m_EditorCamera);
  m_EditorFramebuffer->Unbind();
}

void SodaCan::OnEvent(Event &event)
{
  m_EditorCamera.OnEvent(event);

  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseClickedEvent>(BIND_FN(SodaCan::OnMouseClicked));
  dispatcher.Dispatch<KeyPressEvent>(BIND_FN(SodaCan::OnKeyPressed));
}

bool SodaCan::OnMouseClicked(MouseClickedEvent &mouseClick)
{
  if(mouseClick.GetButtonClicked() == SD_MOUSE_BUTTON_0)
  {
    bool isGuizmoActive = false;
    if(m_Panels.GetSceneListPanel().GetSelectedObject())
      isGuizmoActive = ImGuizmo::IsOver() || ImGuizmo::IsUsing();

    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_SceneViewportBounds[0].x;
    my -= m_SceneViewportBounds[0].y;
    glm::vec2 viewportSize =
        m_SceneViewportBounds[1] - m_SceneViewportBounds[0];

    // For OpenGL
    my = viewportSize.y - my;

    int mouseX = (int)mx;
    int mouseY = (int)my;

    if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x &&
       mouseY < (int)viewportSize.y && !isGuizmoActive)
    {
      m_EditorFramebuffer->Bind();
      int pixel = m_EditorFramebuffer->Read(1, mouseX, mouseY);
      m_EditorFramebuffer->Unbind();
      m_Panels.GetSceneListPanel().SetSelectedObject(
          pixel == -1 ? Object() : Object((entt::entity)pixel, m_Scene.get()));
    }
  }
  return false;
}

void SodaCan::OnResize(uint32_t width, uint32_t height) {}

void SodaCan::OnImGuiUpdate()
{
  static bool dockspaceEnable = true;
  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

  if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  if(!opt_padding)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", &dockspaceEnable, window_flags);
  {
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
        if(ImGui::MenuItem("New", "Ctrl + N"))
        {
          CreateNewScene();
        }
        if(ImGui::MenuItem("Save", "Ctrl + S"))
        {
          SaveScene();
        }
        if(ImGui::MenuItem("Save As...", "Ctrl + Shift + S"))
        {
          SaveSceneAs();
        }
        if(ImGui::MenuItem("Open...", "Ctrl + O"))
        {
          OpenScene();
        }
        ImGui::Separator();
        if(ImGui::MenuItem("Close", "Alt + F4"))
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
      ImVec2 cursorPos = ImGui::GetCursorPos();

      m_IsScenePanelFocused = ImGui::IsWindowFocused();
      m_IsScenePanelHovered = ImGui::IsWindowHovered();
      App::Get().GetImGuiLayer()->ShouldConsumeEvents(!m_IsScenePanelFocused);
      ImVec2 editorSceneSize = ImGui::GetContentRegionAvail();
      m_EditorViewportSize = {editorSceneSize.x, editorSceneSize.y};

      ImGui::Image((void *)m_EditorFramebuffer->GetColorAttachmentID(0),
                   ImVec2(m_EditorViewportSize.x, m_EditorViewportSize.y),
                   ImVec2(0, 1), ImVec2(1, 0));

      ImVec2 minBounds = ImGui::GetWindowPos();
      minBounds.x += cursorPos.x;
      minBounds.y += cursorPos.y;

      ImVec2 maxBound = {m_EditorViewportSize.x + minBounds.x,
                         m_EditorViewportSize.y + minBounds.y};
      m_SceneViewportBounds[0] = {minBounds.x, minBounds.y};
      m_SceneViewportBounds[1] = {maxBound.x, maxBound.y};

      // GUIZMO STUFF
      ImGuizmo::SetDrawlist();
      ImGuizmo::SetRect(m_SceneViewportBounds[0].x, m_SceneViewportBounds[0].y,
                        m_SceneViewportBounds[1].x - m_SceneViewportBounds[0].x,
                        m_SceneViewportBounds[1].y -
                            m_SceneViewportBounds[0].y);
      Object selectedObj = m_Panels.GetSceneListPanel().GetSelectedObject();
      if(selectedObj)
      {
        auto &transform = selectedObj.GetComponent<TransformComponent>();
        glm::mat4 transformComponent = transform.GetTransform();
        glm::vec3 position, rotation, scale;
        ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMat()),
                             glm::value_ptr(m_EditorCamera.GetProjectionMat()),
                             Utils::GizmoToImGuizmoMode(m_GizmoTransformMode),
                             Utils::GizmoToImGuizmoMode(m_GizmoOperationMode),
                             glm::value_ptr(transformComponent));
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

      ImVec2 gameSceneSize = ImGui::GetContentRegionAvail();
      m_GameViewportSize = {gameSceneSize.x, gameSceneSize.y};
      ImGui::Image((void *)m_GameFramebuffer->GetColorAttachmentID(0),
                   ImVec2(m_GameViewportSize.x, m_GameViewportSize.y),
                   ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();
  }
  ImGui::End();
}

bool SodaCan::OnKeyPressed(KeyPressEvent &keyPress)
{
  // if(keyPress.GetKeyCode() == SD_KEY_X)
  // {
  //   Object obj = m_Scene->GetObjectWithName("TestObj");
  //
  //   if(!obj)
  //     return false;
  //
  //   auto &t = obj.GetComponent<TransformComponent>();
  //   t.Rotation = glm::vec3(t.Rotation.x + 5.0f, t.Rotation.y, t.Rotation.z);
  // }
  // if(keyPress.GetKeyCode() == SD_KEY_Y)
  // {
  //   Object obj = m_Scene->GetObjectWithName("TestObj");
  //
  //   if(!obj)
  //     return false;
  //
  //   auto &t = obj.GetComponent<TransformComponent>();
  //   t.Rotation = glm::vec3(t.Rotation.x, t.Rotation.y + 5.0f, t.Rotation.z);
  // }
  // if(keyPress.GetKeyCode() == SD_KEY_Z)
  // {
  //   Object obj = m_Scene->GetObjectWithName("TestObj");
  //
  //   if(!obj)
  //     return false;
  //
  //   auto &t = obj.GetComponent<TransformComponent>();
  //   t.Rotation = glm::vec3(t.Rotation.x, t.Rotation.y, t.Rotation.z + 5.0f);
  // }

  bool ctrl = keyPress.GetKeyCode() == SD_KEY_LEFT_CONTROL ||
              keyPress.GetKeyCode() == SD_KEY_RIGHT_CONTROL;
  bool shift = keyPress.GetKeyCode() == SD_KEY_LEFT_SHIFT ||
               keyPress.GetKeyCode() == SD_KEY_RIGHT_SHIFT;
  bool alt = keyPress.GetKeyCode() == SD_KEY_LEFT_ALT ||
             keyPress.GetKeyCode() == SD_KEY_RIGHT_ALT;

  switch(keyPress.GetKeyCode())
  {
  case SD_KEY_N: {
    if(!ctrl)
      break;
    CreateNewScene();
    break;
  }
  case SD_KEY_O: {
    if(!ctrl)
      break;

    OpenScene();
    break;
  }
  case SD_KEY_S: {
    if(shift && ctrl)
      SaveSceneAs();
    else if(ctrl)
      SaveScene();
    break;
  }
  }

  // TODO: All the guizmo and ImGui stuff should be modularized later on.
  // Including these shortcuts
  if(!(m_EditorCamera.IsSpectating() || ctrl || shift || alt))
  {
    // clang-format off
    switch(keyPress.GetKeyCode())
    {
    case SD_KEY_Q: m_GizmoTransformMode = GizmoTransformMode::MouseOnly; break;
    case SD_KEY_W: m_GizmoTransformMode = GizmoTransformMode::Translation; break;
    case SD_KEY_E: m_GizmoTransformMode = GizmoTransformMode::Rotation; break;
    case SD_KEY_R: m_GizmoTransformMode = GizmoTransformMode::Scale; break;
    case SD_KEY_T: m_GizmoTransformMode = GizmoTransformMode::Universal; break;
    case SD_KEY_O: m_GizmoOperationMode = GizmoOperationMode::Global; break;
    case SD_KEY_P: m_GizmoOperationMode = GizmoOperationMode::Local; break;
    }
    // clang-format on
  }
  return false;
}

void SodaCan::CreateNewScene()
{
  // @TODO: Check For Saves Before Creating A New Scene.

  m_Scene = CreateRef<Scene>();
  m_Panels.SetScene(m_Scene);
  SD_ENGINE_LOG("Created A New Scene");
  m_Scene->OnEditorResize(m_EditorViewportSize.x, m_EditorViewportSize.y,
                          m_EditorCamera);
  m_Scene->OnGameResize(m_GameViewportSize.x, m_GameViewportSize.y);
}
void SodaCan::OpenScene()
{
  // @TODO: Check for current scene saves before opening a new one.

  const char *filters[] = {"*.stscn", "*.sbscn"};
  const char *filepath =
      tinyfd_openFileDialog("Open File...", "", 2, filters, nullptr, 0);
  if(!filepath)
    SD_ENGINE_ERROR("Failed To Open");
  m_Scene = CreateRef<Scene>();
  m_Panels.SetScene(m_Scene);
  SceneSerializer sceneSerializer(m_Scene);
  if(std::filesystem::path(filepath).extension().string() == ".stscn")
  {
    sceneSerializer.Deserialize(filepath);
    SD_ENGINE_LOG("Opened Scene As 'stscn' (Text File) From Path: {}",
                  filepath);
  }
  else if(std::filesystem::path(filepath).extension().string() == ".sbscn")
  {
    SD_ENGINE_ERROR("'.sbscn' (Binary Files) Are Not Supported Yet. You Should "
                    "Save And Open As 'stscn' (Text File) Instead");
    sceneSerializer.DeserializeBinary(filepath);
  }
  m_Scene->OnEditorResize(m_EditorViewportSize.x, m_EditorViewportSize.y,
                          m_EditorCamera);
  m_Scene->OnGameResize(m_GameViewportSize.x, m_GameViewportSize.y);
}
void SodaCan::SaveScene()
{
  // I will think about what to do here later.
}
void SodaCan::SaveSceneAs()
{
  SceneSerializer sceneSerializer(m_Scene);
  const char *filters[] = {"*.stscn", "*.sbscn"};
  const char *filepath =
      tinyfd_saveFileDialog("Save File As...", "", 2, filters, nullptr);
  // @FIXME: we are getting an assert here
  if(!filepath)
  {
    SD_ENGINE_ERROR("Failed To Save");
    return;
  }
  if(std::filesystem::path(filepath).extension().string() == ".stscn")
  {
    sceneSerializer.Serialize(filepath);
    SD_ENGINE_LOG("Saved Scene As '.stscn' (Text File) At Path: {}.", filepath);
  }
  else if(std::filesystem::path(filepath).extension().string() == ".sbscn")
  {
    SD_ENGINE_ERROR("'.sbscn' (Binary Files) Are Not Supported Yet. You Should "
                    "Save And Open As 'stscn' (Text File) Instead");
    sceneSerializer.SerializeBinary(filepath);
  }
}

void SodaCan::OnDetach() { Renderer2D::Shutdown(); }
} // namespace Soda