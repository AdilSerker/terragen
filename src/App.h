#ifndef APP_H
#define APP_H

#include <GL/glew.h>

#include "LightDirectional.h"
#include "CameraOrbit.h"
#include "Scene.h"


class App {
    public:

    ~App();

    void init();

    void loop();

    private:
    GLuint VertexArrayID;

    LightDirectional *light;
    Scene *scene;
    CameraOrbit *camera;

    void update();
    void render();
    void gl_init();
    void get_input_keyboard(glm::vec2 *direction_velocity, int *vel, int *strafe, bool *is_crouched);
};

#endif // !APP_H