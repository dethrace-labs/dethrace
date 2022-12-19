#include "debugui.h"

#include "imgui.h"

#if defined(IOPLATFORM_SDL2)
#include "SDL.h"
#include "backends/imgui_impl_sdl.h"
#include "harness/private/platform_sdl.h"
#elif defined(IOPLATFORM_GLFW)
#include <GLFW/glfw3.h>
#include "backends/imgui_impl_glfw.h"
#include "harness/private/platform_glfw.h"
#endif

#if defined(RENDERER_OPENGL3)
#include "backends/imgui_impl_opengl3.h"
#endif

int gEnableDebugUi = 0;

static bool show_demo_window = true;
static bool frame_started = false;

void DebugUi_Start(void* windowState) {
    if (!gEnableDebugUi) {
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
#if defined(IOPLATFORM_SDL2)
    ImGui_ImplSDL2_InitForOpenGL(((tSDLGLWindowState*)windowState)->window, ((tSDLGLWindowState*)windowState)->glContext);
#elif defined(IOPLATFORM_GLFW)
    ImGui_ImplGlfw_InitForOpenGL(((tGLFWGLWindowState*)windowState)->window, true);
#endif
#if defined(RENDERER_OPENGL3)
    ImGui_ImplOpenGL3_Init();
#endif
}

#if defined(IOPLATFORM_SDL2)
int DebugUI_OnEvent(void* event) {
    if (!gEnableDebugUi) {
        return 0;
    }
    return ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
}
#endif

void DebugUI_StartFrame() {
    if (!gEnableDebugUi) {
        return;
    }
    // Start the Dear ImGui frame
#if defined(RENDERER_OPENGL3)
    ImGui_ImplOpenGL3_NewFrame();
#endif
#if defined(IOPLATFORM_SDL2)
    ImGui_ImplSDL2_NewFrame();
#elif defined(IOPLATFORM_GLFW)
    ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();

    frame_started = true;

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
}

void DebugUI_FinishFrame() {
    if (!gEnableDebugUi) {
        return;
    }
    if (!frame_started) {
        return;
    }
    frame_started = false;

    ImGui::Render();
#if defined(RENDERER_OPENGL3)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

int DebugUI_MouseCaptured() {
    if (!gEnableDebugUi) {
        return 0;
    }
    return ImGui::GetIO().WantCaptureMouse;
}
