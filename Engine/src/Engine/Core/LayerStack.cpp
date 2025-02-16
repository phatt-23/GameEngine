//
// Created by phatt on 2/10/25.
//

#include "Core/LayerStack.h"
#include "Core/Core.h"

namespace Engine
{
    LayerStack::LayerStack()
        : m_Layers(), m_Index(0) {}

    LayerStack::~LayerStack()
    {
        EG_PROFILE_FUNCTION();

        for (Layer* layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer *layer) 
    {
        EG_PROFILE_FUNCTION();

        m_Layers.emplace(m_Layers.begin() + m_Index, layer);
        layer->OnAttach();
        m_Index++;
    }

    void LayerStack::PushOverlay(Layer *overlay) 
    {
        EG_PROFILE_FUNCTION();

        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer *layer) 
    {
        EG_PROFILE_FUNCTION();

        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_Index--;
        }
        layer->OnDetach();
    }

    void LayerStack::PopOverlay(Layer *overlay) 
    {
        EG_PROFILE_FUNCTION();

        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
        overlay->OnDetach();
    }

}
