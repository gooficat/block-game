#include "renderer/renderer.h"
#include "resources/resources.h"
#include "world/transform.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

void renderer_init(renderer_s *renderer, int initial_width, int initial_height)
{
	memset(renderer, 0, sizeof *renderer);
	glewInit();

	glViewport(0, 0, initial_width, initial_height);
}

void renderer_render(renderer_s *renderer)
{
	size_t i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (i = 0; i < renderer->queue.len; ++i)
	{
		float mat[16];
		glUseProgram(renderer->queue.vals[i].shader);

		transform_to_matrix(renderer->queue.vals[i].transform, mat);

		glUniformMatrix4fv(glGetUniformLocation(renderer->queue.vals[i].shader, "model"), 1, GL_FALSE, mat);

		glBindVertexArray(renderer->queue.vals[i].mesh->vao);
		glDrawElements(GL_TRIANGLES, renderer->queue.vals[i].mesh->num_inds * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void renderer_set_color(renderer_s *renderer, float *color)
{
	memcpy(renderer->background_color, color, sizeof renderer->background_color);
	glClearColor(color[0], color[1], color[2], color[3]);
}

size_t renderer_queue_mesh(renderer_s *renderer, resource_manager_s *manager, size_t mesh_id, size_t shader_id,
						   transform_s *transform)
{
	size_t i;
	i = renderer->queue.len;
	++renderer->queue.len;
	renderer->queue.vals = realloc(renderer->queue.vals, renderer->queue.len);
	renderer->queue.vals[i].mesh = &manager->meshes.val[mesh_id];
	renderer->queue.vals[i].shader = manager->shaders.val[shader_id];
	renderer->queue.vals[i].transform = transform;
	return i;
}
