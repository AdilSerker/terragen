#ifndef SCENE_H
#define SCENE_H

#include "Heightmap.h"

#include "CameraOrbit.h"
#include "LightDirectional.h"
#include "Shader.h"

class Scene
{
  public:
    Scene();
    ~Scene();

    void draw(LightDirectional *light, CameraOrbit *camera);
    void generate();
    void save();

    Heightmap *heightmap;

  private:
    Shader *shader;
};

#endif // !SCENE_H