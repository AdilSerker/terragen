#ifndef LIGHT_DIRECTIONAL_H
#define LIGHT_DIRECTIONAL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class LightDirectional
{
  public:
	glm::vec3 target;
	glm::vec3 position;

	GLuint fbo;
	GLuint buf;
	GLuint tex;

	LightDirectional();

	~LightDirectional();
};

#endif
