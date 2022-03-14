#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"

#define CHECK_GL_ERROR(msg)                      \
    {                                            \
        GLenum e = glGetError();                 \
        if (e) {                                 \
            LOG_PANIC("glError %d: %s", e, msg); \
        }                                        \
    }

void GLRenderer_Init(int width, int height, int render_width, int render_height);
void GLRenderer_SetPalette(uint8_t* rgba_colors);
void GLRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer);
void GLRenderer_EndScene();
void GLRenderer_FullScreenQuad(uint8_t* screen_buffer, int width, int height);
void GLRenderer_Model(br_actor* actor, br_model* model, br_matrix34 model_matrix);
void GLRenderer_BufferTexture(br_pixelmap* pm);
void GLRenderer_BufferMaterial(br_material* mat);
void GLRenderer_ClearBuffers();
void GLRenderer_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer);

#endif