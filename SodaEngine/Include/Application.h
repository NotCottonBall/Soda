#pragma once

namespace SodaEngine
{
class Application
{
public:
  Application();

  bool Initialize();
  void Run();

  void Quit();
};
} // namespace SodaEngine
