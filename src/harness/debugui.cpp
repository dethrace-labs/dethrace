#include "debugui.h"

#include "imgui.h"

#include <algorithm>

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

static bool frame_started = false;

void DebugUi_Start(tPlatformState* windowState) {
    if (!gEnableDebugUi) {
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
#if defined(RENDERER_OPENGL3)
#if defined(IOPLATFORM_SDL2)
    ImGui_ImplSDL2_InitForOpenGL(windowState->window, windowState->glContext);
#elif defined(IOPLATFORM_GLFW)
    ImGui_ImplGlfw_InitForOpenGL(windowState->window, true);
#endif
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

extern "C" {
#include "globvars.h"
#include "globvrbm.h"
#include "pedestrn.h"
#include "powerup.h"
}

static void Int_CheckBox(const char *label, int *val) {
    bool b = *val;
    ImGui::Checkbox(label, &b);
    *val = b;
}

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

    ImGui::Begin("Dethrace debugui");
    ImGui::Text("Position: %f, %f, %f", gSelf->t.t.translate.t.v[0], gSelf->t.t.translate.t.v[1], gSelf->t.t.translate.t.v[2]);
    ImGui::InputInt("Number laps", &gTotal_laps);
    ImGui::InputInt("Game credits", &gProgram_state.credits);
    ImGui::InputInt("Credits earned", &gProgram_state.credits_earned);
    ImGui::InputInt("Credits lost", &gProgram_state.credits_lost);
    ImGui::InputInt("Rank", &gProgram_state.rank);
    Int_CheckBox("Game completed", &gProgram_state.game_completed);
    tCar_spec *my_car = &gProgram_state.current_car;
    ImGui::InputInt3("power-up levels", my_car->power_up_levels);
    for (int i = 0; i < 3; i++) {
        my_car->power_up_levels[i] = std::max(0, std::min(20, my_car->power_up_levels[i]));
    }
    Int_CheckBox("Annie", (int*)&gProgram_state.frank_or_anniness);
    Int_CheckBox("Ped harvest", &gPedestrian_harvest);
    {
        int electro = my_car->proxy_ray_distance > 0.f;
        Int_CheckBox("Proxy ray", &electro);
        my_car->proxy_ray_distance = electro ? 6.f : 0.f;
    }
    ImGui::End();

    frame_started = true;
}

void DebugUI_FinishFrame() {
    if (!gEnableDebugUi) {
        return;
    }
    if (!frame_started) {
        return;
    }

    ImGui::Render();
#if defined(RENDERER_OPENGL3)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
    frame_started = false;
}

int DebugUI_MouseCaptured() {
    if (!gEnableDebugUi) {
        return 0;
    }
    return ImGui::GetIO().WantCaptureMouse;
}
