#include "debugui.h"

#include "SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include "harness/private/sdl_gl.h"

#include <stdio.h>

int gEnableDebugUi = 0;

static bool show_demo_window = true;
static bool frame_started = false;

static tSDLGLWindowState* gWindowState;

void DebugUi_Start(tSDLGLWindowState* windowState) {
    if (!gEnableDebugUi) {
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    gWindowState = windowState;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(windowState->window, windowState->glContext);
    ImGui_ImplOpenGL3_Init();
}

int DebugUI_OnEvent(SDL_Event* event) {
    if (!gEnableDebugUi) {
        return 0;
    }
    return ImGui_ImplSDL2_ProcessEvent(event);
}

void DebugUI_StartFrame() {
    if (!gEnableDebugUi) {
        return;
    }
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
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
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int DebugUI_MouseCaptured() {
    if (!gEnableDebugUi) {
        return 0;
    }
    return ImGui::GetIO().WantCaptureMouse;
}
