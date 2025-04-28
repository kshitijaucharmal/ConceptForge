#include <string>

// Includes GLFW and glad
#include <window.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;
std::string WINDOWNAME = "OpenGL Learning";

using namespace WindowManagement;

int main() {
  Window window(WIDTH, HEIGHT, WINDOWNAME);

  // Main Loop
  while (!glfwWindowShouldClose(window.window)) {
    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }

  return 0;
}
