#include "window/window.h"


int main(void)
{
    window_s window;
    window_init(&window, 640, 480);
    while (window.state.state != WINDOW_CLOSING)
    {
        window_update(&window);
    }
    window_close(&window);
    return 0;
}