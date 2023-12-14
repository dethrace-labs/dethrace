#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"
#include "harness/hooks.h"

typedef enum tOpenGL_profile {
    eOpenGL_profile_core,
    eOpenGL_profile_es,
} tOpenGL_profile;

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
            default:                                       \
                err = "unknown";                           \
                break;                                     \
            }                                              \
            LOG_PANIC("glError %d (%s): %s", e, err, msg); \
        }                                                  \
    }

void GLRenderer_Init(tOpenGL_profile profile, int render_width, int render_height);
void GLRenderer_SetPalette(uint8_t* rgba_colors);
void GLRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
void GLRenderer_EndScene(void);
void GLRenderer_FullScreenQuad(uint8_t* screen_buffer);
void GLRenderer_Model(br_actor* actor, br_model* model, br_material* material, br_token render_type, br_matrix34 model_matrix);
void GLRenderer_BufferTexture(br_pixelmap* pm);
void GLRenderer_BufferMaterial(br_material* mat);
void GLRenderer_BufferModel(br_model* model);
void GLRenderer_ClearBuffers(void);
void GLRenderer_FlushBuffer(tRenderer_flush_type flush_type);
void GLRenderer_FlushBuffers(void);
void GLRenderer_SetViewport(int x, int y, int width, int height);

#endif
