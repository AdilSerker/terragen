#ifndef SHADER_H
#define SHADER_H 

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Shader
{
	public:
	GLuint program;
	GLuint vs, fs;

	Shader();
	~Shader();

	void load(const char *vertex_file_path, const char *fragment_file_path);
};

#endif // !SHADER_H