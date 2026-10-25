#ifndef RESOURCES_RESOURCES_H
#define RESOURCES_RESOURCES_H

#include <GL/glew.h>
#include <stddef.h>
#include <stdint.h>

struct vert
{
	GLfloat pos[3];
	GLfloat tex[2];
};

struct vert_list
{
	struct vert *vals;
	GLsizei len;
};

struct ind_list
{
	GLuint (*vals)[3];
	GLsizei len;
};

typedef struct
{
	GLuint vao, vbo, ebo;
	GLsizei num_inds;
} mesh_s;

typedef struct
{
	struct
	{
		GLuint *val;
		size_t len;
	} shaders;
	struct
	{
		mesh_s *val;
		size_t len;
	} meshes;
} resource_manager_s;

void resource_manager_init(resource_manager_s *manager);
void resource_manager_free(resource_manager_s *manager);

size_t resource_manager_add_shader(resource_manager_s *manager, char const **paths);
size_t resource_manager_add_mesh(resource_manager_s *manager, char const *path);

#endif
