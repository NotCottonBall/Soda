#pragma once

#include "Core/App/Layers.h"

#include "Core/Events/AppEvents.h"
#include "Core/Events/KeyboardEvents.h"
#include "Core/Events/MouseEvents.h"

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