#include <SodaEnginePCH.h>

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>

#include "Logging/EngineLogger.h"
#include "SodaEngine/SDAssert.h"
#include <SodaEngine/Window.h>

namespace SodaEngine
{
Window::Window(int width, int height, std::string name)
    : m_Width(width), m_Height(height), m_WindowName(name)
{
  m_Window = nullptr;

  SD_ASSERT(Initialize(), "Failed To Create A Window!");
}

Window::~Window() { SDL_DestroyWindow(m_Window); }

bool Window::Initialize()
{
  SDE_LOG_INFO("Initializing Window...");

  if(!SDL_InitSubSystem(SDL_INIT_VIDEO))
  {
    SDE_LOG_CRITICAL("Failed To Initialize SDL_video. \nCause: {}",
                     SDL_GetError());
    return false;
  }
  else
  {
    SDE_LOG_INFO("Successfully Initialized SDL_video.");
  }

  m_Window = SDL_CreateWindow(m_WindowName.c_str(), m_Width, m_Height,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  if(!m_Window)
  {
    SDE_LOG_CRITICAL("Failed To Create SDL_Window. \nCause: {}",
                     SDL_GetError());
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return false;
  }
  else
  {
    SDE_LOG_INFO("Successfully Created SodaWindow With Name: {}.",
                 m_WindowName);
  }

  return true;
}

void Window::Resize(int width, int height)
{
  m_Width = width;
  m_Height = height;
}

void Window::Rename(std::string name)
{
  m_WindowName = name;
  if(!SDL_SetWindowTitle(m_Window, m_WindowName.c_str()))
  {
    SDE_LOG_ERROR("Failed To Rename Window %s. \nCause: {}", m_WindowName,
                  SDL_GetError());
  }
}
} // namespace SodaEngine
