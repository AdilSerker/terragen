#include "Heightmap.h"
#include <math.h>

#include <iostream>

using namespace std;

Heightmap::Heightmap()
{
	hscale = 1.0;
	vscale = 1.0;
	offset = 0.0;
	vbo = 0;
	tbo = 0;
}

Heightmap::~Heightmap()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (tbo != 0)
	{
		glDeleteBuffers(1, &tbo);
		tbo = 0;
	}
}

double random(double min, double max)
{
	double d = (double)rand() / RAND_MAX;
	return min + d * (max - min);
}

int counter = 0;

void Heightmap::diamond_square(int size, int level, float range)
{
	if (level < 1)
		return;

	for (int i = 0; i < size - 1; i += level)
	{
		for (int j = 0; j < size - 1; j += level)
		{
			float a = data[i][j];
			float b = data[i][j + level];
			float c = data[i + level][j];
			float d = data[i + level][j + level];

			data[i + level / 2][j + level / 2] = (a + b + c + d) / 4 + random(-range, range);
		}
	}

	for (int i = 0; i < size - 1; i += level)
	{
		for (int j = 0; j < size - 1; j += level)
		{
			float a = data[i][j];
			float b = data[i][j + level];
			float c = data[i + level][j];
			float d = data[i + level][j + level];
			float e0 = data[i + level / 2][j + level / 2];

			float e1 = (i - level / 2 > 0) ? data[i - level / 2][j + level / 2] : e0;
			float e2 = (j - level / 2 > 0) ? data[i + level / 2][j - level / 2] : e0;
			float e3 = (j + level + level / 2) < size ? data[i + level / 2][j + level + level / 2] : e0;
			float e4 = (i + level + level / 2) < size ? data[i + level + level / 2][j - level / 2] : e0;

			data[i][j + level / 2] = (e1 == e0) ? (a + c + e0) / 3 + random(-range, range) : (a + c + e0 + e1) / 4 + random(-range, range);
			data[i + level / 2][j] = (e2 == e0) ? (a + b + e0) / 3 + random(-range, range) : (a + b + e0 + e2) / 4 + random(-range, range);
			data[i + level][j + level / 2] = (e4 == e0) ? (a + b + e0) / 3 + random(-range, range) : (b + d + e0 + e4) / 4 + random(-range, range);
			data[i + level / 2][j + level] = (e3 == e0) ? (a + b + e0) / 3 + random(-range, range) : (c + d + e0 + e3) / 4 + random(-range, range);
		}
	}

	diamond_square(size, level / 2, range / 2);
}

void Heightmap::generate(int size, float mhscale, float mvscale)
{
	counter = 0;
	vscale = 1.0;
	hscale = 1.0;

	vscale = mvscale * vscale;
	hscale = mhscale * hscale;

	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (tbo != 0)
	{
		glDeleteBuffers(1, &tbo);
		tbo = 0;
	}

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &tbo);

	data.clear();

	for (int x = 0; x < size; ++x)
	{
		std::vector<float> row;

		for (int y = 0; y < size; ++y)
		{
			float f = 0.0f;
			row.push_back(f);
		}
		data.push_back(row);
	}

	float range = 500.0;

	srand(time(NULL));

	data[0][0] = random(0, range);
	data[0][size - 1] = random(0, range);
	data[size - 1][0] = random(0, range);
	data[size - 1][size - 1] = random(0, range);

	diamond_square(size, size - 1, range / 2);

	int w = data.size();
	int h = data[0].size();

	offset = 0.0;
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			offset += data[x][y];
		}
	offset /= w * h;

	printf("Generate Heightmap (%i %i)\n", (int)w, (int)h);

	glm::vec3 *posns = (glm::vec3 *)malloc(sizeof(glm::vec3) * w * h);
	glm::vec3 *norms = (glm::vec3 *)malloc(sizeof(glm::vec3) * w * h);

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			float cx = hscale * x, cy = hscale * y, cw = hscale * w, ch = hscale * h;
			posns[x + y * w] =
				glm::vec3(cx - cw / 2, sample(glm::vec2(cx - cw / 2, cy - ch / 2)),
						  cy - ch / 2);
		}

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			norms[x + y * w] =
				(x > 0 && x < w - 1 && y > 0 && y < h - 1)
					? glm::normalize(glm::mix(
						  glm::cross(posns[(x + 0) + (y + 1) * w] - posns[x + y * w],
									 posns[(x + 1) + (y + 0) * w] - posns[x + y * w]),
						  glm::cross(posns[(x + 0) + (y - 1) * w] - posns[x + y * w],
									 posns[(x - 1) + (y + 0) * w] - posns[x + y * w]),
						  0.5))
					: glm::vec3(0, 1, 0);
		}

	float *vbo_data = (float *)malloc(sizeof(float) * 7 * w * h);

	uint32_t *tbo_data = (uint32_t *)malloc(sizeof(uint32_t) * 3 * 2 *
											((w - 1) / 2) * ((h - 1) / 2));

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			vbo_data[x * 7 + y * 7 * w + 0] = posns[x + y * w].x;
			vbo_data[x * 7 + y * 7 * w + 1] = posns[x + y * w].y;
			vbo_data[x * 7 + y * 7 * w + 2] = posns[x + y * w].z;
			vbo_data[x * 7 + y * 7 * w + 3] = norms[x + y * w].x;
			vbo_data[x * 7 + y * 7 * w + 4] = norms[x + y * w].y;
			vbo_data[x * 7 + y * 7 * w + 5] = norms[x + y * w].z;
			vbo_data[x * 7 + y * 7 * w + 6] = 0;
		}

	free(posns);
	free(norms);

	for (int x = 0; x < (w - 1) / 2; x++)
		for (int y = 0; y < (h - 1) / 2; y++)
		{
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 0] =
				(x * 2 + 0) + (y * 2 + 0) * w;
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 1] =
				(x * 2 + 0) + (y * 2 + 2) * w;
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 2] =
				(x * 2 + 2) + (y * 2 + 0) * w;
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 3] =
				(x * 2 + 2) + (y * 2 + 2) * w;
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 4] =
				(x * 2 + 2) + (y * 2 + 0) * w;
			tbo_data[x * 3 * 2 + y * 3 * 2 * ((w - 1) / 2) + 5] =
				(x * 2 + 0) + (y * 2 + 2) * w;
		}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * w * h, vbo_data,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tbo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 sizeof(uint32_t) * 3 * 2 * ((w - 1) / 2) * ((h - 1) / 2),
				 tbo_data, GL_STATIC_DRAW);

	free(vbo_data);
	free(tbo_data);
}

void Heightmap::save()
{
	char filename[512];
	memcpy(filename, "./heightmaps/generated.txt",
		   strlen("./heightmaps/generated.txt") - 4);
	filename[strlen("./heightmaps/generated.txt") - 4] = '\0';
	strcat(filename, "_ds.txt");

	FILE *file = fopen(filename, "w");

	int size = data.size();

	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < size; ++y)
		{
			fprintf(file, y == size - 1 ? "%f\n" : "%f ", data[x][y]);
		}
	}

	fclose(file);
}

float Heightmap::sample(glm::vec2 pos)
{
	int w = data.size();
	int h = data[0].size();

	pos.x = (pos.x / hscale) + w / 2;
	pos.y = (pos.y / hscale) + h / 2;

	float a0 = fmod(pos.x, 1.0);
	float a1 = fmod(pos.y, 1.0);

	int x0 = (int)std::floor(pos.x), x1 = (int)std::ceil(pos.x);
	int y0 = (int)std::floor(pos.y), y1 = (int)std::ceil(pos.y);

	x0 = x0 < 0 ? 0 : x0;
	x0 = x0 >= w ? w - 1 : x0;
	x1 = x1 < 0 ? 0 : x1;
	x1 = x1 >= w ? w - 1 : x1;
	y0 = y0 < 0 ? 0 : y0;
	y0 = y0 >= h ? h - 1 : y0;
	y1 = y1 < 0 ? 0 : y1;
	y1 = y1 >= h ? h - 1 : y1;

	float s0 = vscale * (data[x0][y0] - offset);
	float s1 = vscale * (data[x1][y0] - offset);
	float s2 = vscale * (data[x0][y1] - offset);
	float s3 = vscale * (data[x1][y1] - offset);

	return (s0 * (1 - a0) + s1 * a0) * (1 - a1) + (s2 * (1 - a0) + s3 * a0) * a1;
}
