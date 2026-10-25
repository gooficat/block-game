#include "world/transform.h"
#include <math.h>
#include <string.h>

void matrix_identity(float *mat, int w, int h)
{
	int i;
	memset(mat, 0, sizeof *mat * w * h);
	for (i = 0; i < w; ++i)
	{
		mat[i * w + i] = 1;
	}
}

void matrix_multiply(float *a, float *b)
{
	int i, j, k;
	float c[16] = {0};
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			c[j * 4 + i] = 0.0f;
			for (k = 0; k < 4; ++k)
			{
				c[j * 4 + i] += a[k * 4 + i] * b[j * 4 + k];
			}
		}
	}

	memcpy(a, &c, sizeof c);
}

void matrix_translate(float *mat, float *pos)
{
	float tran[16] = {1.0f,	  0.0f,	  0.0f,	  0.0f,

					  0.0f,	  1.0f,	  0.0f,	  0.0f,

					  0.0f,	  0.0f,	  1.0f,	  0.0f,

					  pos[0], pos[1], pos[2], 1.0f};

	matrix_multiply(mat, tran);
}

static void matrix_rotate_x(float *mat, float a)
{
	float c, s;
	c = cosf(a);
	s = sinf(a);
	matrix_multiply(mat, (float[16]){1.0f, 0.0f, 0.0f, 0.0f,

									 0.0f, c, s, 0.0f,

									 0.0f, -s, c, 0.0f,

									 0.0f, 0.0f, 0.0f, 1.0f});
}

static void matrix_rotate_y(float *mat, float a)
{
	float c, s;
	c = cosf(a);
	s = sinf(a);
	matrix_multiply(mat, (float[16]){c, 0.0f, -s, 0.0f,

									 0.0f, 1.0f, 0.0f, 0.0f,

									 s, 0.0f, c, 0.0f,

									 0.0f, 0.0f, 0.0f, 1.0f});
}

static void matrix_rotate_z(float *mat, float a)
{
	float c, s;
	c = cosf(a);
	s = sinf(a);
	matrix_multiply(mat, (float[16]){c, s, 0.0f, 0.0f,

									 -s, c, 0.0f, 0.0f,

									 0.0f, 0.0f, 1.0f, 0.0f,

									 0.0f, 0.0f, 0.0f, 1.0f});
}

void matrix_rotate(float *mat, float *rot)
{
	matrix_rotate_z(mat, rot[2]);
	matrix_rotate_y(mat, rot[1]);
	matrix_rotate_x(mat, rot[0]);
}

void matrix_scale(float *mat, float *scl)
{
	float scal[16] = {scl[0], 0.0f,	  0.0f,	  0.0f,

					  0.0f,	  scl[1], 0.0f,	  0.0f,

					  0.0f,	  0.0f,	  scl[2], 0.0f,

					  0.0f,	  0.0f,	  0.0f,	  1.0f};

	matrix_multiply(mat, scal);
}

void transform_to_matrix(transform_s *trans, float *mat)
{
	matrix_identity(mat, 4, 4);
	matrix_translate(mat, trans->pos);
	matrix_rotate(mat, trans->rot);
	matrix_scale(mat, trans->scl);
}

void matrix_ortho(float *mat, float l, float r, float t, float b, float n, float f)
{
	memcpy(mat,
		   (float[16]){
			   2.0f / (r - l),
			   0.0f,
			   0.0f,
			   0.0f,

			   0.0f,
			   2.0f / (t - b),
			   0.0f,
			   0.0f,

			   0.0f,
			   0.0f,
			   2.0f / (n - f),
			   0.0f,

			   -(r + l) / (r - l),
			   -(t + b) / (t - b),
			   -(f + n) / (f - n),
			   1.0f,
		   },
		   sizeof(float[16]));
}

void transform_to_matrix_view(transform_s *trans, float *mat)
{
	matrix_identity(mat, 4, 4);
	matrix_scale(mat, (float[3]){1.0f / trans->scl[0], 1.0f / trans->scl[1], 1.0f / trans->scl[2]});
	matrix_rotate(mat, (float[3]){-trans->rot[0], -trans->rot[1], -trans->rot[2]});
	matrix_translate(mat, (float[3]){-trans->pos[0], -trans->pos[1], -trans->pos[2]});
}
