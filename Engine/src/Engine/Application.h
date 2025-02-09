//
// Created by phatt on 2/8/25.
//
#pragma once

namespace Engine
{

    class Application {
    public:
        Application();
        virtual ~Application();

        void run();
    };

    // DEFINED IN CLIENT
    Application* CreateApplication();
}
