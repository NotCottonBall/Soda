#pragma once

#include <memory>

#include <SodaEngine/Window.h>

namespace SodaEngine
{
// Forward Declarations
class VulkanInstance;

class Application
{
private:
  bool m_Running = false;
  std::unique_ptr<Window> m_Window;

  // Vulkan Stuff
  std::unique_ptr<VulkanInstance> m_VkInstace;

public:
  Application();
  ~Application();

  bool Initialize(std::string appName);
  void Run();
  void Quit();
};
} // namespace SodaEngine
