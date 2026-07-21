#pragma once

struct VkInstance_T;
typedef VkInstance_T *VkInstance;

namespace SodaEngine
{
class VulkanInstance
{
  // Variables
private:
  VkInstance m_Instance;

  // Functions
private:
  bool Initialize();
  bool CheckExtensionSupport(std::vector<const char *> &extensionsRequested);
  bool CheckLayerSupport(std::vector<const char *> &layersRequested);

public:
  VulkanInstance();
  ~VulkanInstance();

  VkInstance Get() { return m_Instance; }
};
} // namespace SodaEngine
