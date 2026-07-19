#pragma once

namespace SodaEngine
{
class VulkanInstance
{
private:
  void *m_Instance;

public:
  void *Get() { return m_Instance; }
};
} // namespace SodaEngine
