//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Core/Layer.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"
#include "Event/ApplicationEvent.h"

namespace Engine
{
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.f;
    };
}


