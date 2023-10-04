#pragma once

#include "SD_PCH.h"

#include "Core.h"
#include "Soda/Events/Events.h"


namespace Soda
{
    class SD_DLL Layer
    {
    public:
        Layer(const std::string layerName = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_LayerName; }
    protected:
        std::string m_LayerName;
    };
}