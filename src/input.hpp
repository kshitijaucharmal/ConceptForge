#pragma once
#include <GLFW/glfw3.h>
#include "camera.hpp"

#include "constants.hpp"

namespace InputManagement {

class Input {

private:
public:
  void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
};

} // namespace InputManagement
