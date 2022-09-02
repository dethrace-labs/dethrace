#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"
#include "renderers/renderer.h"

#define CHECK_GL_ERROR(msg)                                \
    {                                                      \
        GLenum e = glGetError();                           \
        if (e != GL_NO_ERROR) {                            \
            const char* err;                               \
            switch (e) {                                   \
            case GL_NO_ERROR:                              \
                err = "GL_NO_ERROR";                       \
                break;                                     \
            case GL_INVALID_ENUM:                          \
                err = "GL_INVALID_ENUM";                   \
                break;                                     \
            case GL_INVALID_VALUE:                         \
                err = "GL_INVALID_VALUE";                  \
                break;                                     \
            case GL_INVALID_OPERATION:                     \
                err = "GL_INVALID_OPERATION";              \
                break;                                     \
            case GL_INVALID_FRAMEBUFFER_OPERATION:         \
                err = "GL_INVALID_FRAMEBUFFER_OPERATION";  \
                break;                                     \
            case GL_OUT_OF_MEMORY:                         \
                err = "GL_OUT_OF_MEMORY";                  \
                break;                                     \
            case GL_STACK_UNDERFLOW:                       \
                err = "GL_STACK_UNDERFLOW";                \
                break;                                     \
            case GL_STACK_OVERFLOW:                        \
                err = "GL_STACK_OVERFLOW";                 \
                break;                                     \
            default:                                       \
                err = "unknown";                           \
                break;                                     \
            }                                              \
            LOG_PANIC("glError %d (%s): %s", e, err, msg); \
        }                                                  \
    }

extern tRenderer gl_renderer;

#endif
