#include <malloc.h>

#include "ctr_gfx.h"
#include "citro3d.h"
#include "vshader_shbin.h"

static DVLB_s *vshader_dvlb;
static shaderProgram_s program;
static int8_t uLoc_projection;

static C3D_Mtx topProjection;
static C3D_RenderTarget *topRenderTarget;

static uint8_t *renderTextureData;
static const uint16_t renderTextureWidth = 512;
static const uint16_t renderTextureHeight = 256;
static const uint32_t renderTextureStride = renderTextureWidth * 4;
static const uint32_t renderTextureByteCount = renderTextureStride * renderTextureHeight;

static C3D_Tex render_tex;
static int activeTexFilter;

static bool isN3DS;

void beginRender(bool vSync);
void drawTopRenderTarget(uint32_t clearColor);
void finishRender(void);

void setTextureFilter(int texFilter)
{
    int newTexFilter;

    if (texFilter != -1)
        newTexFilter = texFilter;
    else
        newTexFilter = isN3DS ? GPU_LINEAR : GPU_NEAREST;

    if (newTexFilter != activeTexFilter) {
        C3D_TexSetFilter(&render_tex, GPU_LINEAR, (GPU_TEXTURE_FILTER_PARAM)newTexFilter);
        activeTexFilter = newTexFilter;
    }
}

void drawRect(const float subTexX, const float subTexY, const float subTexW, const float subTexH,
                     const float posX, const float posY, const float newWidth, const float newHeight)
{
    C3D_ImmDrawBegin(GPU_TRIANGLE_STRIP);
    {
        const float texW = renderTextureWidth;
        const float texH = renderTextureHeight;

        const float vertZ = 0.5f;
        const float vertW = 1.f;

        // Bottom left corner
        C3D_ImmSendAttrib(posX, posY, vertZ, vertW);                                   // v0 = position xyzw
        C3D_ImmSendAttrib(subTexX / texW, 1.f - (subTexY + subTexH) / texH, 0.f, 0.f); // v1 = texcoord uv

        // Bottom right corner
        C3D_ImmSendAttrib(newWidth + posX, posY, vertZ, vertW);
        C3D_ImmSendAttrib((subTexX + subTexW) / texW, 1.f - (subTexY + subTexH) / texH, 0.f, 0.f);

        // Top left corner
        C3D_ImmSendAttrib(posX, newHeight + posY, vertZ, vertW);
        C3D_ImmSendAttrib(subTexX / texW, 1.f - subTexY / texH, 0.f, 0.f);

        // Top right corner
        C3D_ImmSendAttrib(newWidth + posX, newHeight + posY, vertZ, vertW);
        C3D_ImmSendAttrib((subTexX + subTexW) / texW, 1.f - subTexY / texH, 0.f, 0.f);
    }
    C3D_ImmDrawEnd();
}

void drawRects(void)
{
    beginRender(false);
    drawTopRenderTarget(0x00000ff);
    drawRect(0, 0, 320, 200, 0, 0, 400, 240);
    finishRender();
}

void ctr_gfx_draw(uint32_t* pixelData, int width, int height)
{

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int bufferIndex = (y * renderTextureWidth + x) * 4;
            ((u32*)renderTextureData)[bufferIndex / sizeof(u32)] = pixelData[y * width + x];
        }
    }

    GSPGPU_FlushDataCache(renderTextureData, renderTextureByteCount);

    C3D_SyncDisplayTransfer((u32*)renderTextureData, GX_BUFFER_DIM(renderTextureWidth, renderTextureHeight),
                            (u32*)render_tex.data, GX_BUFFER_DIM(renderTextureWidth, renderTextureHeight), TEXTURE_TRANSFER_FLAGS);

    GSPGPU_FlushDataCache(render_tex.data, renderTextureByteCount);

    C3D_TexBind(0, &render_tex);

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    drawRects();
}

void beginRender(bool vSync)
{
    C3D_FrameBegin(vSync ? C3D_FRAME_SYNCDRAW : 0);
}

void drawTopRenderTarget(uint32_t clearColor)
{
    C3D_RenderTargetClear(topRenderTarget, C3D_CLEAR_ALL, clearColor, 0);
    C3D_FrameDrawOn(topRenderTarget);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &topProjection);
}

void finishRender()
{
    C3D_FrameEnd(0);
}

void initTransferTexture(void)
{
    renderTextureData = (uint8_t *)linearAlloc(renderTextureByteCount);

    memset(renderTextureData, 0, renderTextureByteCount);

    C3D_TexInitVRAM(&render_tex, renderTextureWidth, renderTextureHeight, GPU_RGBA8);

    setTextureFilter(-1);
}

void ctr_gfx_init()
{
    APT_CheckNew3DS(&isN3DS);

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    topRenderTarget = C3D_RenderTargetCreate(GSP_SCREEN_WIDTH, GSP_SCREEN_HEIGHT_TOP, GPU_RB_RGBA8, GPU_RB_DEPTH16);
    C3D_RenderTargetSetOutput(topRenderTarget, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    vshader_dvlb = DVLB_ParseFile((uint32_t *)vshader_shbin, vshader_shbin_size);
    shaderProgramInit(&program);
    shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
    C3D_BindProgram(&program);

    uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

    C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord

    Mtx_OrthoTilt(&topProjection, 0, GSP_SCREEN_HEIGHT_TOP, 0, GSP_SCREEN_WIDTH, 0.1f, 1.0f, true);

    initTransferTexture();
}

void ctr_gfx_exit()
{
    linearFree(renderTextureData);
    shaderProgramFree(&program);
    DVLB_Free(vshader_dvlb);
    C3D_Fini();
}
