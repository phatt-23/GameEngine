//
// Created by phatt on 2/12/25.
//
#pragma once

#include <glad/glad.h>

#ifdef ENGINE_OPENGL_CALL_ERROR_CHECK
#include <source_location>
#include "Core/Core.h"
const char* OpenGLErrorTypeToString(GLenum glError);
#endif

#ifdef ENGINE_OPENGL_CALL_ERROR_CHECK
    #define EG_OPENGL_CALL(_CALL)                                                                               \
        while (glGetError() != GL_NO_ERROR);                                                                    \
        _CALL;                                                                                                  \
        if (GLenum glErr = glGetError(); glErr != 0) {                                                          \
            auto loc = std::source_location::current();                                                         \
            EG_CORE_ERROR("OpenGL error '{}' (0x{:x}) returned from call: `{}`! Occured at: {}({}:{}): `{}`",   \
                OpenGLErrorTypeToString(glErr), glErr, #_CALL,                                                  \
                loc.file_name(), loc.line(), loc.column(), loc.function_name());                                \
        }
#else
    #define EG_OPENGL_CALL(_CALL) _CALL
#endif
    
