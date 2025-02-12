//
// Created by phatt on 2/10/25.
//
#pragma once

namespace Engine
{
    class GraphicsContext
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}
