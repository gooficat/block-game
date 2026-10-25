#include "renderer/renderer.h"
#include "resources/resources.h"
#include "window/window.h"
#include <stdint.h>

int main(void)
{
    window_s window;
	renderer_s renderer;
	resource_manager_s resource_manager;
	window_init(&window, 640, 480);
	renderer_init(&renderer, 640, 480);
	resource_manager_init(&resource_manager);
	resource_manager_add_shader(&resource_manager, (char const *[]){"res/basic.vert", "res/basic.frag"});

	while (window.state.state != WINDOW_CLOSING)
	{
		window_update(&window);
	}

	window_close(&window);
	return 0;
}