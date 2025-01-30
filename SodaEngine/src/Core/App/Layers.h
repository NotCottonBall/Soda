#pragma once

#include "SD_PCH.h"

#include "Core/Timestep.h"

#include "Core/Core.h"
#include "Core/Events/Events.h"

namespace Soda
{
class Layer
{
public:
  Layer(const std::string layerName = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(Timestep dt) {}
  virtual void OnImGuiUpdate() {}
  virtual void OnEvent(Event &event) {}
  virtual void OnResize(uint32_t width, uint32_t height) {}

  inline const std::string &GetName() const { return m_LayerName; }

protected:
  std::string m_LayerName;
};
} // namespace Soda