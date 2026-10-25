#include "resources/resources.h"
#include <GL/glew.h>
#include <GL/glext.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resource_manager_init(resource_manager_s *manager)
{
	memset(manager, 0, sizeof *manager);
}

static char *resource_manager_dump_file(char const *path)
{
	FILE *file;
	long len;
	size_t real;
	char *data;

	file = fopen(path, "r");
	fseek(file, 0l, SEEK_END);
	len = ftell(file);
	rewind(file);
	data = malloc(len + 1);
	real = fread(data, 1, len, file);
	if (real != (size_t)len)
	{
		data = realloc(data, real + 1);
	}
	data[real] = '\0';
	fclose(file);
	return data;
}

static GLuint resource_manager_create_shader_piece(char const *path)
{
	GLuint id;
	char *src;
	int succ;
	char log[512];
	succ = 0;
	{
		char const *suff;
		suff = path + strlen(path) - 4;
		GLuint kind;
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

	src = resource_manager_dump_file(path);

	glShaderSource(id, 1, (const GLchar *const *)&src, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		glGetShaderInfoLog(id, sizeof log, NULL, log);
		fprintf(stderr, "SHADER PIECE ERROR: %s\n", log);
		exit(EXIT_FAILURE);
		free(src);
	}

	free(src);

	return id;
}

size_t resource_manager_add_shader(resource_manager_s *manager, char const **paths)
{
	size_t i;
	GLuint shader;
	struct
	{
		GLuint *vals;
		size_t len;
	} pieces;
	int succ;
	char log[512];
	pieces.vals = NULL;
	pieces.len = 0;
	succ = 0;
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

	glGetProgramiv(shader, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		glGetProgramInfoLog(shader, sizeof log, NULL, log);
		fprintf(stderr, "SHADER PROGRAM ERROR: %s\n", log);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < pieces.len; ++i)
	{
		glDeleteShader(pieces.vals[i]);
	}
	free(pieces.vals);
	i = manager->shaders.len;
	++manager->shaders.len;
	manager->shaders.val = realloc(manager->shaders.val, manager->shaders.len * sizeof *manager->shaders.val);
	manager->shaders.val[i] = shader;
	return i;
}

static void resource_manager_read_mesh_obj(struct vert_list *verts, struct ind_list *inds, FILE *file)
{
	char buffer[4096];
	size_t i;
	while (fgets(buffer, 4096, file))
	{
		switch (buffer[0])
		{
		case '#':
			break;
		case 'v':
			i = verts->len;
			++verts->len;
			verts->vals = realloc(verts->vals, verts->len * sizeof *verts->vals);
			sscanf(buffer, "v %f %f %f", &verts->vals[i].pos[0], &verts->vals[i].pos[1], &verts->vals[i].pos[2]);
			break;
		case 'f':
			i = inds->len;
			++inds->len;
			inds->vals = realloc(inds->vals, inds->len * sizeof *inds->vals);
			sscanf(buffer, "f %" SCNu32 " %" SCNu32 "%" SCNu32, &inds->vals[i][0], &inds->vals[i][1],
				   &inds->vals[i][2]);
			--inds->vals[i][0];
			--inds->vals[i][1];
			--inds->vals[i][2];
			break;
		default:
			break;
		}
	}
}

static void resource_manager_read_mesh(struct vert_list *verts, struct ind_list *inds, char const *path)
{
	FILE *file;
	char const *suff;
	suff = path + strlen(path) - 3;

	file = fopen(path, "r");

	if (!strcmp(suff, "obj"))
	{
		resource_manager_read_mesh_obj(verts, inds, file);
	}
}

static void resource_manager_load_mesh(mesh_s *mesh, char const *path)
{
	struct vert_list verts;
	struct ind_list inds;
	memset(&verts, 0, sizeof verts);
	memset(&inds, 0, sizeof inds);

	resource_manager_read_mesh(&verts, &inds, path);

	mesh->num_inds = inds.len;

	glCreateVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	glBindVertexArray(mesh->vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof *verts.vals * verts.len, verts.vals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof *inds.vals * inds.len, inds.vals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof *verts.vals, (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof *verts.vals, (void *)offsetof(typeof(*verts.vals), tex));

	glBindVertexArray(0);

	free(verts.vals);
	free(inds.vals);
}

size_t resource_manager_add_mesh(resource_manager_s *manager, char const *path)
{
	size_t i;
	i = manager->meshes.len;
	++manager->meshes.len;
	manager->meshes.val = realloc(manager->meshes.val, manager->meshes.len * sizeof *manager->meshes.val);
	resource_manager_load_mesh(&manager->meshes.val[i], path);
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
