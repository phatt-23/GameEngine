#include "Engine.h"

class Sandbox : public Engine::Application
{
public:
    Sandbox()
    {
        std::cout << "Hello\n";
    }

    ~Sandbox()
    {

    }
};

Engine::Application *Engine::CreateApplication() {
    return new Sandbox;
}
