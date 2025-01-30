#include "SD_PCH.h"

#include "GLFWWindow.h"

#include "Soda/Renderer/OpenGL/OpenGLContext.h"

// Events
#include "Soda/Events/AppEvents.h"
#include "Soda/Events/KeyboardEvents.h"
#include "Soda/Events/MouseEvents.h"

namespace Soda
{
static bool s_IsGLFWInitialized = false;

// the glfw error callback function
void GLFW_ErrorCallback(int errorCode, const char *description)
{
  SD_ENGINE_ERROR("ERRORRR... with GLFW... code: {0}::{1}", errorCode,
                  description);
}

// the Create function returns the PlatformWindow class
SodaWindow *SodaWindow::Create(const WindowInfo &windowInfo)
{
  return new WindowsWindow(windowInfo);
}

// and the PlatformWindow class calls the Init() func with the given info
WindowsWindow::WindowsWindow(const WindowInfo &windowInfo)
{
  InitWindow(windowInfo);
}
WindowsWindow::~WindowsWindow() { CloseWindow(); }

void WindowsWindow::InitWindow(const WindowInfo &windowInfo)
{
  // assigning the given data to our Window Data
  m_WindowData.Name = windowInfo.Name;
  m_WindowData.Width = windowInfo.Width;
  m_WindowData.Height = windowInfo.Height;

  if(!s_IsGLFWInitialized)
  {
    int sucess = glfwInit();
    SD_ENGINE_ASSERT(sucess, "Failed to initialize GLFW!");
    glfwSetErrorCallback(GLFW_ErrorCallback);

    s_IsGLFWInitialized = true;
  }

  // we create the glfw window by calling the func and we assign the return
  // value to the m_Window which is our Engine Window
  m_Window = glfwCreateWindow((int)windowInfo.Width, (int)windowInfo.Height,
                              windowInfo.Name.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);
  SD_ENGINE_LOG("Creating Window '{0}'  With Dimentions x : {1}, y : {2}",
                windowInfo.Name, windowInfo.Width, windowInfo.Height);

  m_renderContext = new SodaGL();
  m_renderContext->Init();

  // this function binds our m_WindowData to the m_Window to use the Data where
  // ever we want without declaring it global because the data is bound to the
  // m_Window, we can access the m_WindowData with the m_Window
  glfwSetWindowUserPointer(m_Window, &m_WindowData);
  SetVSync(true);

  /* NOW... the EVENTS FLOOD */
  // this is where we setup events for our window

  // Window Events //
  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width,
                                         int height) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);
    windowData.Width = width;
    windowData.Height = height;

    // we created a EventType class and named the class like this...
    // _<EventClassName> then we give the _Event as callback.
    WindowResizeEvent _WindowResizeEvent(width, height);
    windowData.CallbackFn(_WindowResizeEvent);
  });

  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    WindowCloseEvent _WindowCloseEvent;
    windowData.CallbackFn(_WindowCloseEvent);
  });

  // Keyboard Events //
  glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int mods) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    switch(action)
    {
    case GLFW_PRESS: {
      KeyPressEvent _KeyPressEvent(key, 0);
      windowData.CallbackFn(_KeyPressEvent);
      break;
    }

    case GLFW_REPEAT: {
      // alright for this we name it _KeyRepeatEvent...
      KeyPressEvent _KeyRepeatEvent(key, 1);
      windowData.CallbackFn(_KeyRepeatEvent);
      break;
    }

    case GLFW_RELEASE: {
      KeyReleaseEvent _KeyReleaseEvent(key);
      windowData.CallbackFn(_KeyReleaseEvent);
      break;
    }
    }
  });

  glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int character) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    KeyTypeEvent _KeyTypedEvent(character);
    windowData.CallbackFn(_KeyTypedEvent);
  });

  // Mouse Events //
  glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos,
                                        double yPos) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseMoveEvent _MouseMoveEvent((float)xPos, (float)yPos);
    windowData.CallbackFn(_MouseMoveEvent);
  });

  glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset,
                                     double yOffset) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseScrollEvent _MouseScrollEvent((float)xOffset, (float)yOffset);
    windowData.CallbackFn(_MouseScrollEvent);
  });

  glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button,
                                          int action, int mods) {
    WindowData &windowData = *(WindowData *)glfwGetWindowUserPointer(window);

    switch(action)
    {
    case GLFW_PRESS: {
      MouseClickedEvent _MouseClickEvent(button);
      windowData.CallbackFn(_MouseClickEvent);
      break;
    }

    case GLFW_RELEASE: {
      MouseReleasedEvent _MouseReleaseEvent(button);
      windowData.CallbackFn(_MouseReleaseEvent);
      break;
    }
    }
  });
}

void WindowsWindow::CloseWindow() { glfwDestroyWindow(m_Window); }

void WindowsWindow::OnUpdate()
{
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool status)
{
  if(status)
    // this function waits the given amount of frames to swap buffersa
    // also knows as VSync
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_WindowData.VSync = status;
}

bool WindowsWindow::GetVSyncStatus() const { return m_WindowData.VSync; }

void WindowsWindow::ShowCursor(bool status)
{
  if(status)
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  else
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
} // namespace Soda