#include "SD_PCH.h"

#include "ImGuiLayer.h"

#include "Core/Events/Events.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include "Core/App/App.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Soda
{
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad
  // Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  // setting the font
  // @TODO: Settings should include font size and file and not this code
  io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "SodaCan/assets/fonts/Roboto/static/Roboto-Regular.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF(
      "SodaCan/assets/fonts/Roboto/static/Roboto-Bold.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF(
      "SodaCan/assets/fonts/Roboto/static/Roboto-Italic.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF(
      "SodaCan/assets/fonts/Roboto/static/Roboto-BoldItalic.ttf", 15.0f);

  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();

  if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  SetEditorThemeDark();

  App &app = App::Get();
  GLFWwindow *imWindow =
      static_cast<GLFWwindow *>(app.GetWindow().GetTheWindow());

  ImGui_ImplGlfw_InitForOpenGL(imWindow, true);
  ImGui_ImplOpenGL3_Init("#version 410 core");
}

void ImGuiLayer::OnDetach()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event &event)
{
  if(m_ConsumeEvents)
  {
    ImGuiIO &io = ImGui::GetIO();
    event.m_Handled |=
        event.IsInCategory(EventCategory_Mouse) & io.WantCaptureMouse;
    event.m_Handled |=
        event.IsInCategory(EventCategory_Keyboard) & io.WantCaptureKeyboard;
  }
}

void ImGuiLayer::Begin()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();

  ImGui::NewFrame();
}

void ImGuiLayer::End()
{
  ImGuiIO &imguiIO = ImGui::GetIO();
  App &app = App::Get();
  imguiIO.DisplaySize = ImVec2((float)app.GetWindow().GetWindowWidth(),
                               (float)app.GetWindow().GetWindowHeight());

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if(imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow *suckedWindow = glfwGetCurrentContext();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    glfwMakeContextCurrent(suckedWindow);
  }
}

void ImGuiLayer::OnImGuiUpdate()
{
  // 	static bool show = true;
  // 	ImGui::ShowDemoWindow(&show);
}

// Other FUnctions //
void ImGuiLayer::SetEditorThemeDark()
{
  auto &colors = ImGui::GetStyle().Colors;

  ImVec4 backGroundColor(0.05f, 0.05f, 0.05f, 1.0f);

  ImVec4 primaryColor(0.15f, 0.15f, 0.15f, 1.0f);
  ImVec4 primaryHoveredColor(0.12f, 0.12f, 0.14f, 1.0f);
  ImVec4 primarySelectColor(0.10f, 0.10f, 0.11f, 1.0f);

  ImVec4 tabPrimaryColor(0.18f, 0.18f, 0.18f, 1.0f);
  ImVec4 tabHoveredColor(0.26f, 0.26f, 0.28f, 1.0f);

  // BASE //
  colors[ImGuiCol_WindowBg] = backGroundColor;
  colors[ImGuiCol_MenuBarBg] = backGroundColor;
  colors[ImGuiCol_PopupBg] = backGroundColor;

  // HEADERS //
  colors[ImGuiCol_Header] = primaryColor;
  colors[ImGuiCol_HeaderHovered] = primaryHoveredColor;
  colors[ImGuiCol_HeaderActive] = primarySelectColor;

  // BUTTONS //
  colors[ImGuiCol_Button] = primaryColor;
  colors[ImGuiCol_ButtonHovered] = primaryHoveredColor;
  colors[ImGuiCol_ButtonActive] = primarySelectColor;
  // FRAMES //
  colors[ImGuiCol_FrameBg] = primaryColor;
  colors[ImGuiCol_FrameBgHovered] = primaryHoveredColor;
  colors[ImGuiCol_FrameBgActive] = primarySelectColor;

  // TABS //
  colors[ImGuiCol_Tab] = tabPrimaryColor;
  colors[ImGuiCol_TabHovered] = tabHoveredColor;
  colors[ImGuiCol_TabActive] = backGroundColor;
  colors[ImGuiCol_TabUnfocused] = tabPrimaryColor;
  colors[ImGuiCol_TabUnfocusedActive] = tabHoveredColor;
  // TAB BARS //
  colors[ImGuiCol_TitleBg] = tabPrimaryColor;
  colors[ImGuiCol_TitleBgActive] = tabPrimaryColor;
  colors[ImGuiCol_TitleBgCollapsed] = primaryColor;

  // SCROLLBARS //
  colors[ImGuiCol_ScrollbarBg] = backGroundColor;
  colors[ImGuiCol_ScrollbarGrab] = primaryColor;
  colors[ImGuiCol_ScrollbarGrabHovered] = primaryHoveredColor;
  colors[ImGuiCol_ScrollbarGrabActive] = primarySelectColor;
}
} // namespace Soda