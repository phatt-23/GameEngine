//
// Created by phatt 02/15/2025
//
#include "Core/Input.h" 

#include "Platform/Linux/LinuxInput.h"

namespace Engine
{
    Input* Input::s_Instance = new LinuxInput();
}
