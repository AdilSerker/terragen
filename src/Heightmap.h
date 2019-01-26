#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>

class Heightmap
{
public:
  bool loading;

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

  float sample(glm::vec2 pos);

private:
  void diamondSquare(int size, int level, float range);
};

#endif /* HEIGHTMAP_H */
