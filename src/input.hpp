#pragma once
#include <GLFW/glfw3.h>
#include "camera.hpp"

#include "constants.hpp"

namespace InputManagement {

class Input {

private:
  GLFWwindow* window;
public:
  Camera &camera;
  Input(GLFWwindow* window, Camera &cam);
  void processInput(float deltaTime);
};

} // namespace InputManagement
