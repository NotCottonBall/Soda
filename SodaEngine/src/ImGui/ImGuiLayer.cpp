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
} // namespace Soda