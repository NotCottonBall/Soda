#pragma once

#include "SD_PCH.h"

#include "Core/Core.h"
#include "Layers.h"

namespace Soda
{
class LayerStack
{
public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *overlay);

  std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
  std::vector<Layer *>::iterator end() { return m_Layers.end(); }

protected:
  std::vector<Layer *> m_Layers;
  unsigned int m_LayerIndex = 0;

  // we will add overlays to the back of the m_Layers
  // instead of creating a seperate list for overlays and calling overlays
  // seperatly
};
} // namespace Soda