
#include "harness.h"
#include "input/keyboard.h"
#include "stack_trace_handler.h"

SDL_Window* window;
SDL_Thread* game_thread;
renderer* current_renderer;
br_pixelmap* palette;
uint32_t* screen_buffer;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

void Harness_Init(char* name, renderer* renderer) {
    install_signal_handler(name);
    current_renderer = renderer;
    screen_buffer = NULL;
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

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Keyboard_HandleEvent(&event.key);
                break;
            case SDL_QUIT:
                keep_pumping = 0;
            }
        }
    }
    LOG_PANIC("leaving loop");
}

void Harness_Hook_DOSGfxBegin() {
    current_renderer->activate(window);
}

void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    uint8_t palette_index = 0;
    int inc = 0;
    uint8_t* data = src->pixels;
    uint32_t* colors = palette->pixels;
    uint32_t true_color;
    int x;
    int y;

    if (screen_buffer == NULL) {
        screen_buffer = malloc(src->width * src->height * sizeof(uint32_t));
        memset(screen_buffer, 0, src->width * src->height * sizeof(uint32_t));
    }

    //LOG_DEBUG("%d %d, %d", src->width, src->height, src->row_bytes);

    // generate 32 bit texture from src + palette
    for (y = 0; y < src->height; y++) {
        inc = 0;
        for (x = 0; x < src->width; x++) {

            palette_index = (data[y * src->row_bytes + x]);
            true_color = colors[palette_index];
            screen_buffer[y * src->width + x] = true_color;
        }
    }
    current_renderer->doubleBuffer(screen_buffer, window);
}

void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    last_dst = dst;
    last_src = src;
    Harness_RenderScreen(dst, src);
}
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    palette = pm;
    if (last_dst && last_src) {
        Harness_RenderScreen(last_dst, last_src);
    }
}

void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour) {
    uint32_t* colors = palette->pixels;
    colors[i] = colour;
}

void Harness_Hook_KeyBegin() {
    Keyboard_Init();
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return Keyboard_IsKeyDown(pScan_code);
}