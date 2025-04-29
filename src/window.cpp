#include <window.hpp>

#include <iostream>
#include <stdexcept>

namespace WindowManagement {
// Resize Callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

Window::Window(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Init window
  window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "Failed to create GLFW window" << std::endl;
  }

  // Make context
  glfwMakeContextCurrent(window);

  // GLAD manages function pointers for OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return;
  }

  // Resize support
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

// Destructor
Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

} // namespace WindowManagement
