#ifndef WINDOW_WINDOW_H
#define WINDOW_WINDOW_H

#include <GLFW/glfw3.h>

typedef struct {
    bool keys[256];
    struct {
        bool btns[5];
        double pos[2];
    } mouse;
} window_input_s;

typedef struct {
    enum {
        WINDOW_PENDING,
        WINDOW_CLOSING,
        WINDOW_MINIMIZED,
        WINDOW_FOCUSED,
        WINDOW_BACKGROUND,
    } state;
    int size[2];
    bool has_resized;
} window_state_s;

typedef struct {
    window_input_s input;
    window_state_s state;
    struct {
        GLFWwindow *window;
    } internal;
} window_s;

void window_init(window_s *window, int initial_width, int initial_height);
void window_update(window_s *window);
void window_close(window_s *window);

#endif