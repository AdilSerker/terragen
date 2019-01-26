#include "App.h"

#include <GLFW/glfw3.h>
extern GLFWwindow *window;

#include <iostream>

// void character_callback(GLFWwindow *window, unsigned int codepoint)
// {
// 	std::cout << codepoint << std::endl;
// }

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

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BASE", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetWindowUserPointer(window, this);
	glfwSetCharCallback(window, [](GLFWwindow *window, unsigned int codepoint) -> void {
		App *pw = (App *)glfwGetWindowUserPointer(window);

		if (codepoint == 114)
		{
			pw->scene->generate();
		}
	});

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

void App::update()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	camera->update(xpos, ypos);
}

void App::render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearDepth(1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->draw(light, camera);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void App::loop()
{

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0)
	{
		// printf("\033c");
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			// printf("FPS: %i\n", nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		update();
		render();

		glfwSwapBuffers(window);
		glfwWaitEvents();
	}
}