#include "renderer/renderer.h"
#include "resources/resources.h"
#include "window/window.h"
#include "world/transform.h"
#include <GL/gl.h>
#include <stdint.h>

int main(void)
{
    window_s window;
	renderer_s renderer;
	resource_manager_s resource_manager;

	size_t my_shader, my_mesh, my_mesh_instance;
	transform_s my_transform;

	struct
	{
		transform_s trans;
		float bound[6];
	} camera = {{{0, 0, 5}, {0, 0, 0}, {1, 1, 1}}, {-2, 2, -2, 2, -2, 2}};

	window_init(&window, 640, 480);
	renderer_init(&renderer, 640, 480);
	resource_manager_init(&resource_manager);

	my_shader = resource_manager_add_shader(&resource_manager, (char const *[]){"res/basic.vert", "res/basic.frag"});

	my_mesh = resource_manager_add_mesh(&resource_manager, "res/cube.obj");

	renderer_set_color(&renderer, (float[4]){0.4f, 0.5f, 1.0f, 1.0f});

	my_transform = (transform_s){
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
	};

	my_mesh_instance = renderer_queue_mesh(&renderer, &resource_manager, my_mesh, my_shader, &my_transform);
	(void)my_mesh_instance;

	while (window.state.state != WINDOW_CLOSING)
	{
		renderer_render(&renderer);
		window_update(&window);
		my_transform.rot[0] += 0.001f;
	}

	window_close(&window);
	return 0;
}