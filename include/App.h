#ifndef APP_H
#define APP_H

#include <GL/glew.h>

#include "LightDirectional.h"
#include "CameraOrbit.h"
#include "Scene.h"

class App
{
  public:
    ~App();

    void init();

    void render();

  private:
    GLuint VertexArrayID;

    LightDirectional *light;
    Scene *scene;
    CameraOrbit *camera;

    void gl_init();
};

#endif // !APP_H