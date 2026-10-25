#include "renderer/renderer.h"
#include "resources/resources.h"
#include "window/window.h"
#include <GL/gl.h>
#include <stdint.h>

int main(void)
{
    window_s window;
	renderer_s renderer;
	resource_manager_s resource_manager;

	size_t my_shader, my_mesh, my_mesh_instance;

	window_init(&window, 640, 480);
	renderer_init(&renderer, 640, 480);
	resource_manager_init(&resource_manager);

	my_shader = resource_manager_add_shader(&resource_manager, (char const *[]){"res/basic.vert", "res/basic.frag"});

	my_mesh = resource_manager_add_mesh(&resource_manager, "res/cube.obj");

	renderer_set_color(&renderer, (float[4]){0.4f, 0.5f, 1.0f, 1.0f});

	my_mesh_instance = renderer_queue_mesh(&renderer, &resource_manager, my_mesh, my_shader);
	(void)my_mesh_instance;

	while (window.state.state != WINDOW_CLOSING)
	{
		renderer_render(&renderer);
		window_update(&window);
	}

	window_close(&window);
	return 0;
}