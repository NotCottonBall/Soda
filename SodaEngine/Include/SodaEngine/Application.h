#pragma once

#include <memory>

#include <SodaEngine/Window.h>

namespace SodaEngine
{
class Application
{
private:
  bool m_Running = false;
  std::unique_ptr<Window> m_Window;

public:
  Application() = default;

  bool Initialize(std::string appName);
  void Run();
  void Quit();
};
} // namespace SodaEngine
