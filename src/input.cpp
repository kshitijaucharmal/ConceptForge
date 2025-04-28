#include "input.hpp"

namespace InputManagement {
  void Input::processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  }
} // namespace InputManagement
