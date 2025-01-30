#pragma once

#include "SD_PCH.h"

#include "Core.h"

#include "Soda/Events/AppEvents.h"
#include "Soda/Events/Events.h"
#include "Window.h"

#include "LayerStack.h"
#include "Soda/imgui/ImGuiLayer.h"

namespace Soda
{
class App
{
public:
  App(const std::string &windowName = "Soda Application");
  virtual ~App();

  void CloseApp();

  void Run();
  void OnEvent(Event &event);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);

  inline SodaWindow &GetWindow() { return *m_MainWindow; }

  inline ImGuiLayer *GetImGuiLayer() { return m_imguiLayer; }

  inline static App &Get() { return *m_app; }

private:
  static App *m_app;

  float m_LastFrameTime = 0.0f; // to calculate the delta time

  bool IsMinimized = false; // to check if the window is minimized or not

  // we want a unique pointer because we dont want to deal with all the deleting
  // shit this is our MainWindow where we do the important stuff (we might have
  // multiple windows later on).
  std::unique_ptr<SodaWindow> m_MainWindow;

  ImGuiLayer *m_imguiLayer; // the ImGui Layer

  LayerStack m_LayerStack; // this is where our engine layers will be stored.

  bool OnWindowClose(WindowCloseEvent &_WindowCloseEvent);
  bool OnWindowResize(WindowResizeEvent &_WindowResizeEvent);

  bool IsRunning = true; // to check if the App is running or not
};

App *CreateApp(); // We create an app that we want to show up on the screen in
                  // the main function (start.h)
} // namespace Soda