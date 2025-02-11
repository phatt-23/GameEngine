//
// Created by phatt on 2/11/25.
//
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
