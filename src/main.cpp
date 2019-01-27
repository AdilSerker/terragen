#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow *window;

#include "App.h"

static App *app = NULL;

int main(void)
{
	app = new App();
	app->init();

	app->render();

	delete app;

	return 0;
}
