// A SMALL TIP
// if there is a cpp file and also a header file, then check the header file to
// know more about what a function does

#include "SD_PCH.h"

#include "Core.h"

#include "App.h"

#include "Soda/Input/Input.h"
#include "Soda/Input/KeyboardCodes.h"

#include "Soda/Renderer/Render.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// @TODO: this shant be here
#include "GLFW/glfw3.h"

namespace Soda
{
App *App::m_app = nullptr;

App::App(const std::string &windowName)
{
  SD_ENGINE_ASSERT(!m_app, "App already exists!");
  m_app = this;

  m_MainWindow = std::unique_ptr<SodaWindow>(
      SodaWindow::Create(Soda::WindowInfo(windowName)));
  m_MainWindow->SetCallbackFn(BIND_FN(App::OnEvent));

  Renderer::Init(m_MainWindow->GetWindowWidth(),
                 m_MainWindow->GetWindowHeight());

  m_imguiLayer = new ImGuiLayer();
  PushOverlay(m_imguiLayer);
}

App::~App() {}

// this function takes the event it was called with in the callback function
// and dispatches the right function for that event
void App::OnEvent(Event &event)
{
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_FN(App::OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(BIND_FN(App::OnWindowResize));

  for(auto itr = m_LayerStack.end(); itr != m_LayerStack.begin();)
  {
    (*--itr)->OnEvent(event);

    if(event.m_Handled)
      break;
  }
}

void App::Run()
{
  // our main gameloop (engineloop)
  while(IsRunning)
  {
    float time = glfwGetTime();
    Timestep dt = time - m_LastFrameTime;
    m_LastFrameTime = time;

    if(!IsMinimized)
    {
      for(Layer *layer : m_LayerStack)
        layer->OnUpdate(dt);
    }

    m_imguiLayer->Begin();
    for(Layer *layer : m_LayerStack)
      layer->OnImGuiUpdate();
    m_imguiLayer->End();

    m_MainWindow->OnUpdate();
  }
}

void App::CloseApp() { IsRunning = false; }

void App::PushLayer(Layer *layer)
{
  m_LayerStack.PushLayer(layer);
  layer->OnAttach();
}
void App::PushOverlay(Layer *overlay)
{
  m_LayerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

bool App::OnWindowClose(WindowCloseEvent &_WindowCloseEvent)
{
  IsRunning = false;
  return true;
}

bool App::OnWindowResize(WindowResizeEvent &_WindowResizeEvent)
{
  if(_WindowResizeEvent.GetWindowHeight() == 0 ||
     _WindowResizeEvent.GetWindowWidth() == 0)
  {
    IsMinimized = true;
    return false;
  }

  IsMinimized = false;
  Renderer::OnWindowResize(_WindowResizeEvent.GetWindowWidth(),
                           _WindowResizeEvent.GetWindowHeight());

  for(Layer *layer : m_LayerStack)
    layer->OnResize(_WindowResizeEvent.GetWindowWidth(),
                    _WindowResizeEvent.GetWindowHeight());

  return false;
}
} // namespace Soda