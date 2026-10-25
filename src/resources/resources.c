#include "resources/resources.h"
#include <GL/glew.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resource_manager_init(resource_manager_s *manager)
{
	memset(manager, 0, sizeof *manager);
}

static uint32_t resource_manager_create_shader_piece(char const *path)
{
	uint32_t id;
	{
		char const *suff;
		suff = path + strlen(path) - 4;
		uint32_t kind;
		if (!strcmp(suff, "vert"))
		{
			kind = GL_VERTEX_SHADER;
		}
		else if (!strcmp(suff, "frag"))
		{
			kind = GL_FRAGMENT_SHADER;
		}
		else if (!strcmp(suff, "geom"))
		{
			kind = GL_GEOMETRY_SHADER;
		}
		else
		{
			fprintf(stderr, "Shader has wrong file suffix; cannot determine what type it is!\n");
			exit(EXIT_FAILURE);
		}
		id = glCreateShader(kind);
	}
	return id;
}

uint32_t resource_manager_add_shader(resource_manager_s *manager, char const **paths)
{
	size_t i;
	uint32_t shader;
	struct
	{
		uint32_t *vals;
		size_t len;
	} pieces;
	pieces.vals = NULL;
	pieces.len = 0;

	while (paths[pieces.len])
	{
		size_t idx;
		idx = pieces.len;
		++pieces.len;
		pieces.vals = realloc(pieces.vals, pieces.len * sizeof *pieces.vals);
		pieces.vals[idx] = resource_manager_create_shader_piece(paths[idx]);
	}
	shader = glCreateProgram();
	for (i = 0; i < pieces.len; ++i)
	{
		glAttachShader(shader, pieces.vals[i]);
	}
	glLinkProgram(shader);
	for (i = 0; i < pieces.len; ++i)
	{
		glDeleteShader(pieces.vals[i]);
	}
	free(pieces.vals);
	i = manager->shaders.len;
	++manager->shaders.len;
	manager->shaders.val = realloc(manager->shaders.val, manager->shaders.len);
	manager->shaders.val[i] = shader;
	return i;
}

void resource_manager_free(resource_manager_s *manager)
{
	size_t i;
	for (i = 0; i < manager->shaders.len; ++i)
	{
		glDeleteProgram(manager->shaders.val[i]);
	}
}
