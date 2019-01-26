#ifndef CAMERA_ORBIT_H
#define CAMERA_ORBIT_H

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class CameraOrbit {
  public:
  glm::vec3 target;
  float pitch, yaw;
  float distance;

  int invert_y;
  
  CameraOrbit();
  
  void update(double xpos, double ypos);
   
  glm::vec3 position();
 
  glm::vec3 direction();
  
  glm::mat4 view_matrix();
  
  glm::mat4 proj_matrix();

};

#endif /* CAMERA_ORBIT_H */
