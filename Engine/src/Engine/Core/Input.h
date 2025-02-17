//
// Created by phatt on 2/10/25.
//
#pragma once

#include <utility>

namespace Engine
{

    class Input 
    {
        static Input* s_Instance;

    public:
        static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
        static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(int keyCode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
    };

}
