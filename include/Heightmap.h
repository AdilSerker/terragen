#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

class Heightmap
{
  public:
	float hscale;
	float vscale;
	float offset;
	std::vector<std::vector<float>> data;

	int range;

	GLuint vbo;
	GLuint tbo;

	Heightmap();

	~Heightmap();

	void generate(int size, float mhscale, float mvscale);
	void smooth(int c);
	void save();

	float sample(glm::vec2 pos);

  private:
	void diamond_square(int size, int level, float range);
	void blur();

	void gen_vbo_tbo_data();
};

#endif /* HEIGHTMAP_H */
