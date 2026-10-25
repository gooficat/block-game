#ifndef WORLD_TRANSFORM_H
#define WORLD_TRANSFORM_H

typedef struct
{
	float pos[3];
	float rot[3];
	float scl[3];
} transform_s;

void transform_to_matrix(transform_s *transform, float *matrix);

#endif
