#include "renderer/renderer.h"
#include <GL/glew.h>

void renderer_init(renderer_s *renderer, int initial_width, int initial_height)
{
	(void)renderer;
	glewInit();

	glViewport(0, 0, initial_width, initial_height);
}
