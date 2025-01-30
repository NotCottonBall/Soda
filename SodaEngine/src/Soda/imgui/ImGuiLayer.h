#pragma once

#include "Soda/_Main/Layers.h"

#include "Soda/Events/AppEvents.h"
#include "Soda/Events/KeyboardEvents.h"
#include "Soda/Events/MouseEvents.h"

namespace Soda
{
class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event &event);
  virtual void OnImGuiUpdate() override;

  void Begin();
  void End();

  void ShouldConsumeEvents(bool value) { m_ConsumeEvents = value; }

private:
  float m_time = 0.0f;

  bool m_ConsumeEvents = false;
};
} // namespace Soda