#ifndef RESOURCES_RESOURCES_H
#define RESOURCES_RESOURCES_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
	struct
	{
		uint32_t *val;
		size_t len;
	} shaders;
} resource_manager_s;

void resource_manager_init(resource_manager_s *manager);
uint32_t resource_manager_add_shader(resource_manager_s *manager, char const **paths);
void resource_manager_free(resource_manager_s *manager);

#endif
