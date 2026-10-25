#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "resources/resources.h"
#include <GL/glew.h>

struct queued_mesh
{
	mesh_s *mesh;
	GLuint shader;
};

typedef struct
{
	float background_color[4];
	struct
	{
		struct queued_mesh *vals;
		size_t len;
	} queue;
} renderer_s;

void renderer_init(renderer_s *renderer, int initial_width, int initial_height);
void renderer_render(renderer_s *renderer);
void renderer_set_color(renderer_s *renderer, float *color);
size_t renderer_queue_mesh(renderer_s *renderer, resource_manager_s *manager, size_t mesh_id, size_t shader_id);

#endif
