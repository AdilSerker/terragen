#include "App.h"

#include <GLFW/glfw3.h>
extern GLFWwindow *window;

#include <iostream>

App::~App()
{
	glDeleteVertexArrays(1, &VertexArrayID);

	delete camera;
	delete light;
	delete scene;

	glfwTerminate();
}

void App::init()
{
	gl_init();

	this->camera = new CameraOrbit();
	this->light = new LightDirectional();
	this->scene = new Scene();

	scene->generate();
}

void App::gl_init()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terragen", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}

	glfwSetWindowPos(window, 200, 200);

	glfwSetWindowUserPointer(window, this);
	glfwSetCharCallback(window, [](GLFWwindow *window, unsigned int codepoint) -> void {
		App *pw = (App *)glfwGetWindowUserPointer(window);

		if (codepoint == 103)
		{
			pw->scene->generate();
		}

		if (codepoint == 115)
		{
			pw->scene->save();
		}

		if (codepoint == 98)
		{
			pw->scene->smooth();
		}

		// std::cout << codepoint << std::endl;
	});

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void App::render()
{

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0)
	{

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		camera->update(xpos, ypos);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearDepth(1.0);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->draw(light, camera);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glfwSwapBuffers(window);
		glfwWaitEvents();
	}
}