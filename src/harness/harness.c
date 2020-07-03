
#include "harness.h"
#include "stack_trace_handler.h"

SDL_Window* window;
SDL_Thread* game_thread;
renderer* current_renderer;

void Harness_Init(char* name, renderer* renderer) {
    install_signal_handler(name);
    current_renderer = renderer;
}

void Harness_RunWindowLoop(harness_game_func* game_func, void* arg) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        LOG_PANIC("SDL_Init Error: %s", SDL_GetError());
    }
    window = SDL_CreateWindow("Dethrace",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        680, 480,
        current_renderer->get_window_flags());

    if (!window) {
        LOG_PANIC("Failed to create window");
    }

    current_renderer->init(window);

    game_thread = SDL_CreateThread(game_func, "game_thread", arg);

    SDL_Event event;
    int keep_pumping = 1;
    while (keep_pumping) {
        if (SDL_WaitEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                keep_pumping = 0;
            }
        }
    }
    LOG_PANIC("leaving loop");
}

void Harness_Hook_ActivateRenderer() {
    current_renderer->activate(window);
}

void Harness_Hook_DoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    //current_renderer.doubleBuffer(dst, src);
}
void Harness_Hook_SetPalette(br_pixelmap* pm) {
    LOG_WARN("TODO: capture palette");
}