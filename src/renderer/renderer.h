#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

typedef struct
{
	float background_color[4];
	struct
	{
		void *placeholder;
	} queue;
} renderer_s;

void renderer_init(renderer_s *renderer, int initial_width, int initial_height);

#endif
