#pragma once
#include <GLFW/glfw3.h>

namespace InputManagement {

class Input {

private:
public:
  void processInput(GLFWwindow *window);
};

} // namespace InputManagement
