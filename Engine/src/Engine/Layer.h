//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Event/Event.h"
#include "Core/Timestep.h"

namespace Engine
{
    class Layer {
    public:
        explicit Layer(std::string&& debugName = "Layer")
            : m_DebugName(debugName) {}

        virtual ~Layer() = default;

        virtual void OnEvent(Event& event) {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}

        inline const std::string& GetName() const { return m_DebugName; }
    private:
        std::string m_DebugName;
    };

}


