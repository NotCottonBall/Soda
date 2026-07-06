#include <print>
#include <string>

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>

#include <SodaEngine/Window.h>

namespace SodaEngine
{
Window::Window(int width, int height, std::string name)
    : m_Width(width), m_Height(height), m_WindowName(name)
{
  m_Window = nullptr;

  Initialize();
}

Window::~Window() { SDL_DestroyWindow(m_Window); }

bool Window::Initialize()
{
  std::println("Initializing Window...");

  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    // @TODO: Assert Here With SDL Error
    std::println("Failed To Initialize SDL_video. \nCause: {}", SDL_GetError());
    return false;
  }
  else
  {
    // @TODO: Info Message
    std::println("Successfully Initialized SDL_video.");
  }

  m_Window = SDL_CreateWindow(m_WindowName.c_str(), m_Width, m_Height,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  if(!m_Window)
  {
    // @TODO: Assert here with SDL Error
    std::println("Failed To Create SDL_Window. \nCause: {}", SDL_GetError());
    // @TODO: should we de init the SDL_video?
    return false;
  }
  else
  {
    // @TODO: Info Message
    std::println("Successfully Created SodaWindow With Name: {}.",
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
    // @TODO: Error Here with SDL Error
    std::println("Failed To Rename Window %s. \nCause: {}", m_WindowName,
                 SDL_GetError());
  }
}
} // namespace SodaEngine
