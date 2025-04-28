#include <string>

// Includes GLFW and glad
#include "window.hpp"

#include "input.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
std::string WINDOWNAME = "OpenGL Learning";

int main() {
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME);
  InputManagement::Input input;

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {

    // input processing
    input.processInput(window.window);

    // rendering commands
    // Clear Screen with this color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // check and call events and swap buffers
    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }

  return 0;
}
