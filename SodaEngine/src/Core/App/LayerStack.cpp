#include "SD_PCH.h"

#include "LayerStack.h"

namespace Soda
{
LayerStack::LayerStack() {}

LayerStack::~LayerStack()
{
  for(Layer *layer : m_Layers)
    delete layer;
}

void LayerStack::PushLayer(Layer *layer)
{
  m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
  m_LayerIndex++;
}

void LayerStack::PushOverlay(Layer *overlay) { m_Layers.emplace_back(overlay); }

void LayerStack::PopLayer(Layer *layer)
{
  auto itr = std::find(m_Layers.begin(), m_Layers.end(), layer);
  if(itr != m_Layers.end())
  {
    m_Layers.erase(itr);
    m_LayerIndex--;
  }
}

void LayerStack::PopOverlay(Layer *overlay)
{
  auto itr = std::find(m_Layers.begin(), m_Layers.end(), overlay);
  if(itr != m_Layers.end())
    m_Layers.erase(itr);
}

} // namespace Soda