//
// Created by phatt on 2/10/25.
//

#include "LinuxInput.h"

#include "Core/EnginePCH.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    bool LinuxInput::IsKeyPressedImpl(int keyCode) {
        auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        int state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::IsMouseButtonPressedImpl(int button) {
        auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        int state = glfwGetKey(window, button);
        return state == GLFW_PRESS;;
    }

    float LinuxInput::GetMouseXImpl() {
        auto[x, _] = GetMousePositionImpl();
        return x;
    }

    float LinuxInput::GetMouseYImpl() {
        auto[_, y] = GetMousePositionImpl();
        return y;
    }

    std::pair<float, float> LinuxInput::GetMousePositionImpl() {
        auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return {xPos, yPos};
    }

}
