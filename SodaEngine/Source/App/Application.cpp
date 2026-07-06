#include <memory>
#include <print>

#include "SDL3/SDL_events.h"
#include <SDL3/SDL_init.h>

#include <SodaEngine/Application.h>

namespace SodaEngine
{
bool Application::Initialize(std::string appName)
{
  m_Running = true;

  std::println("Initializing Application...");
  m_Window = std::make_unique<Window>(1240, 720, appName);

  return m_Running;
}

void Application::Run()
{
  while(m_Running)
  {
    SDL_Event event{0};
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_EVENT_QUIT:
        m_Running = false;
        break;
      case SDL_EVENT_WINDOW_RESIZED:
        m_Window->Resize(event.window.data1, event.window.data2);
        break;
      }
    }
  }
}

void Application::Quit()
{
  // @TODO: Info Message
  std::println("Quiting Application...");
  SDL_Quit();
}
} // namespace SodaEngine
