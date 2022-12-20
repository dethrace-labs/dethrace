#include "harness/private/platform_glfw.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#if defined(RENDERER_OPENGL3)
#include "../renderers/gl/gl_renderer.h"
#define RENDERER gl_renderer
#endif

#include "harness/config.h"
#include "harness/trace.h"

#include "globvars.h"
#include "grafdata.h"
#include "pd/sys.h"

#include "debugui.h"

#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

static int scancode_map[123];
const int scancodes_dethrace2glfw[123] = {
    -1,                         //   0 (LSHIFT || RSHIFT)
    -1,                         //   1 (LALT || RALT)
    -1,                         //   2 (LCTRL || RCTRL)
    -1,                         //   3 (LCTRL || RCTRL)
    GLFW_KEY_CAPS_LOCK,         //   4
    GLFW_KEY_RIGHT_SHIFT,       //   5
    GLFW_KEY_RIGHT_ALT,         //   6
    GLFW_KEY_RIGHT_CONTROL,     //   7
    GLFW_KEY_LEFT_SHIFT,        //   8
    GLFW_KEY_LEFT_ALT,          //   9
    GLFW_KEY_LEFT_CONTROL,      //  10
    GLFW_KEY_0,                 //  11
    GLFW_KEY_1,                 //  12
    GLFW_KEY_2,                 //  13
    GLFW_KEY_3,                 //  14
    GLFW_KEY_4,                 //  15
    GLFW_KEY_5,                 //  16
    GLFW_KEY_6,                 //  17
    GLFW_KEY_7,                 //  18
    GLFW_KEY_8,                 //  19
    GLFW_KEY_9,                 //  20
    GLFW_KEY_A,                 //  21
    GLFW_KEY_B,                 //  22
    GLFW_KEY_C,                 //  23
    GLFW_KEY_D,                 //  24
    GLFW_KEY_E,                 //  25
    GLFW_KEY_F,                 //  26
    GLFW_KEY_G,                 //  27
    GLFW_KEY_H,                 //  28
    GLFW_KEY_I,                 //  29
    GLFW_KEY_J,                 //  30
    GLFW_KEY_K,                 //  31
    GLFW_KEY_L,                 //  32
    GLFW_KEY_M,                 //  33
    GLFW_KEY_N,                 //  34
    GLFW_KEY_O,                 //  35
    GLFW_KEY_P,                 //  36
    GLFW_KEY_Q,                 //  37
    GLFW_KEY_R,                 //  38
    GLFW_KEY_S,                 //  39
    GLFW_KEY_T,                 //  40
    GLFW_KEY_U,                 //  41
    GLFW_KEY_V,                 //  42
    GLFW_KEY_W,                 //  43
    GLFW_KEY_X,                 //  44
    GLFW_KEY_Y,                 //  45
    GLFW_KEY_Z,                 //  46
    GLFW_KEY_GRAVE_ACCENT,      //  47
    GLFW_KEY_MINUS,             //  48
    GLFW_KEY_EQUAL,             //  49
    GLFW_KEY_BACKSPACE,         //  50
    GLFW_KEY_ENTER,             //  51
    GLFW_KEY_KP_ENTER,          //  52
    GLFW_KEY_TAB,               //  53
    GLFW_KEY_SLASH,             //  54
    -1,                         //  55
    GLFW_KEY_SEMICOLON,         //  56
    GLFW_KEY_APOSTROPHE,        //  57
    GLFW_KEY_PERIOD,            //  58
    GLFW_KEY_COMMA,             //  59
    GLFW_KEY_LEFT_BRACKET,      //  60
    GLFW_KEY_RIGHT_BRACKET,     //  61
    GLFW_KEY_BACKSLASH,         //  62
    GLFW_KEY_ESCAPE,            //  63
    GLFW_KEY_INSERT,            //  64
    GLFW_KEY_DELETE,            //  65
    GLFW_KEY_HOME,              //  66
    GLFW_KEY_END,               //  67
    GLFW_KEY_PAGE_UP,           //  68
    GLFW_KEY_PAGE_DOWN,         //  69
    GLFW_KEY_LEFT,              //  70
    GLFW_KEY_RIGHT,             //  71
    GLFW_KEY_UP,                //  72
    GLFW_KEY_DOWN,              //  73
    GLFW_KEY_NUM_LOCK,          //  74
    GLFW_KEY_KP_DIVIDE,         //  75
    GLFW_KEY_KP_MULTIPLY,       //  76
    GLFW_KEY_KP_SUBTRACT,       //  77
    GLFW_KEY_KP_ADD,            //  78
    GLFW_KEY_KP_DECIMAL,        //  79
    GLFW_KEY_KP_EQUAL,          //  80
    GLFW_KEY_KP_0,              //  81
    GLFW_KEY_KP_1,              //  82
    GLFW_KEY_KP_2,              //  83
    GLFW_KEY_KP_3,              //  84
    GLFW_KEY_KP_4,              //  85
    GLFW_KEY_KP_5,              //  86
    GLFW_KEY_KP_6,              //  87
    GLFW_KEY_KP_7,              //  88
    GLFW_KEY_KP_8,              //  89
    GLFW_KEY_KP_9,              //  90
    GLFW_KEY_F1,                //  91
    GLFW_KEY_F2,                //  92
    GLFW_KEY_F3,                //  93
    GLFW_KEY_F4,                //  94
    GLFW_KEY_F5,                //  95
    GLFW_KEY_F6,                //  96
    GLFW_KEY_F7,                //  97
    GLFW_KEY_F8,                //  98
    GLFW_KEY_F9,                //  99
    GLFW_KEY_F10,               // 100
    GLFW_KEY_F11,               // 101
    GLFW_KEY_F12,               // 102
    GLFW_KEY_PRINT_SCREEN,      // 103
    GLFW_KEY_SCROLL_LOCK,       // 104
    GLFW_KEY_PAUSE,             // 105
    GLFW_KEY_SPACE,             // 106
};
static int scancodes_glfw2dethrace[GLFW_KEY_LAST];

static tPlatformState gMainWindow;

static uint8_t sdl_key_state[256];
static struct {
    float x;
    float y;
} glfw_window_scale;

static void error_callback(int error, const char* description)
{
    LOG_PANIC("glfw error: %d:%s\n", error, description);
}

static struct {
    int x;
    int y;
} window_pos;

static void swap_full_screen(void) {
    GLFWmonitor* monitor;

    monitor = glfwGetWindowMonitor(gMainWindow.window);
    if (monitor == NULL) {
        int mw, mh;
        monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(monitor, NULL, NULL, &mw, &mh);
        glfwGetWindowPos(gMainWindow.window, &window_pos.x, &window_pos.y);
        glfwSetWindowMonitor(gMainWindow.window, monitor, 0, 0, mw, mh, GLFW_DONT_CARE);
    } else {
        int w, h;
        RENDERER.GetRenderSize(&w, &h);
        glfwSetWindowMonitor(gMainWindow.window, NULL, window_pos.x, window_pos.y, w, h, GLFW_DONT_CARE);
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F4 && action == GLFW_PRESS && mods == GLFW_MOD_ALT) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && mods == GLFW_MOD_ALT) {
        swap_full_screen();
        return;
    }
    switch (action) {
    case GLFW_PRESS:
    case GLFW_RELEASE:
        {
            int dethrace_key;
            dethrace_key = scancodes_glfw2dethrace[key];
            if (dethrace_key == -1) {
                LOG_WARN("unexpected scan code %d", key);
                return;
            }
            sdl_key_state[dethrace_key] = action == GLFW_PRESS;

            sdl_key_state[0] = sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_LEFT_SHIFT]] || sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_RIGHT_SHIFT]];
            sdl_key_state[1] = sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_LEFT_ALT]] || sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_RIGHT_ALT]];
            sdl_key_state[2] = sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_LEFT_CONTROL]] || sdl_key_state[scancodes_glfw2dethrace[GLFW_KEY_RIGHT_CONTROL]];
            sdl_key_state[3] = sdl_key_state[2];
        }
    }
}


static void resize_callback(GLFWwindow* window, int width, int height) {
    int vp_x, vp_y, vp_w, vp_h;
    int r_w, r_h;
    RENDERER.SetWindowSize(width, height);
    RENDERER.GetViewport(&vp_x, &vp_y, &vp_w, &vp_h);
    RENDERER.GetRenderSize(&r_w, &r_h);
    glfw_window_scale.x = (float)r_w / vp_w;
    glfw_window_scale.y = (float)r_h / vp_h;
}

tRenderer* Window_Create(char* title, int width, int height, int pRender_width, int pRender_height) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        LOG_PANIC("glfwInit failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gMainWindow.window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (gMainWindow.window == NULL) {
       glfwTerminate();
       LOG_PANIC("Failed to create window");
    }

    glfw_window_scale.x = ((float)pRender_width) / width;
    glfw_window_scale.y = ((float)pRender_height) / height;

    if (harness_game_config.start_full_screen) {
       swap_full_screen();
    }

    glfwSetKeyCallback(gMainWindow.window, key_callback);
    glfwSetWindowSizeCallback(gMainWindow.window, resize_callback);

    glfwMakeContextCurrent(gMainWindow.window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    RENDERER.Init(width, height, pRender_width, pRender_height);

    DebugUi_Start(&gMainWindow);

    return &RENDERER;
}

void Window_PollEvents() {
    glfwPollEvents();
    if (glfwWindowShouldClose(gMainWindow.window)) {
       LOG_PANIC("QuitGame");
    }
}

void Window_Swap(int delay_ms_after_swap) {
    glfwMakeContextCurrent(gMainWindow.window);
    glfwSwapBuffers(gMainWindow.window);

//    if (delay_ms_after_swap != 0) {
//        SDL_Delay(delay_ms_after_swap);
//    }
}

void Input_Init() {
    for (size_t i = 0; i < ARRAY_LEN(scancodes_glfw2dethrace); i++) {
        scancodes_glfw2dethrace[i] = -1;
    }
    for (size_t i = 0; i < ARRAY_LEN(scancodes_dethrace2glfw); i++) {
        if (scancodes_dethrace2glfw[i] != -1) {
            scancodes_glfw2dethrace[scancodes_dethrace2glfw[i]] = i;
        }
    }
    for (size_t i = 0; i < ARRAY_LEN(scancode_map); i++) {
        scancode_map[i] = i;
    }
}

int* Input_GetKeyMap() {
    return (int*)scancode_map;
}

int Input_IsKeyDown(unsigned char scan_code) {
    return sdl_key_state[scan_code];
}

void Input_GetMousePosition(int* pX, int* pY) {
    double xpos, ypos;
    int vp_x, vp_y, vp_w, vp_h;

    glfwGetCursorPos(gMainWindow.window, &xpos, &ypos);
    *pX = xpos;
    *pY = ypos;
    RENDERER.GetViewport(&vp_x, &vp_y, &vp_w, &vp_h);
    if (*pX < vp_x) {
        *pX = vp_x;
    } else if (*pX >= vp_x + vp_w) {
        *pX = vp_x + vp_w - 1;
    }
    if (*pY < vp_y) {
        *pY = vp_y;
    } else if (*pY >= vp_y + vp_h) {
        *pY = vp_y + vp_h - 1;
    }
    *pX -= vp_x;
    *pY -= vp_y;
    *pX *= glfw_window_scale.x;
    *pY *= glfw_window_scale.y;

#if defined(DETHRACE_FIX_BUGS)
    // In hires mode (640x480), the menus are still rendered at (320x240),
    // so prescale the cursor coordinates accordingly.
    *pX *= gGraf_specs[gGraf_data_index].phys_width;
    *pX /= gGraf_specs[gReal_graf_data_index].phys_width;
    *pY *= gGraf_specs[gGraf_data_index].phys_height;
    *pY /= gGraf_specs[gReal_graf_data_index].phys_height;
#endif
}

void Input_GetMouseButtons(int* pButton1, int* pButton2) {
    if (DebugUI_MouseCaptured()) {
        *pButton1 = *pButton2 = 0;
        return;
    }
    *pButton1 = glfwGetMouseButton(gMainWindow.window, GLFW_MOUSE_BUTTON_LEFT);
    *pButton2 = glfwGetMouseButton(gMainWindow.window, GLFW_MOUSE_BUTTON_RIGHT);
}
