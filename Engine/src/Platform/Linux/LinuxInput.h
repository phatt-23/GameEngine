//
// Created by phatt on 2/10/25.
//
#pragma once

#include "Input.h"

namespace Engine
{

    class LinuxInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(int keyCode) override;
        bool IsMouseButtonPressedImpl(int button) override;
        std::pair<float, float> GetMousePositionImpl() override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
    };

}

