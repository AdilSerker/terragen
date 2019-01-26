#include "Shader.h"

Shader::Shader()
{
	program = 0;
	vs = 0;
	fs = 0;
}

Shader::~Shader()
{
	if (vs != 0)
	{
		glDeleteShader(vs);
		vs = 0;
	}
	if (fs != 0)
	{
		glDeleteShader(fs);
		fs = 0;
	}
	if (program != 0)
	{
		glDeleteShader(program);
		program = 0;
	}
}

void Shader::load(const char *vertex_file_path, const char *fragment_file_path)
{

	// Create the shaders
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return exit(1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vs, 1, &VertexSourcePointer, NULL);
	glCompileShader(vs);

	// Check Vertex Shader
	glGetShaderiv(vs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fs, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fs);

	// Check Fragment Shader
	glGetShaderiv(fs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fs, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);
}
