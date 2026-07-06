#pragma once

namespace SodaEngine
{
class Application
{
public:
  Application() = default;

  bool Initialize();
  void Run();
  void Quit();
};
} // namespace SodaEngine
