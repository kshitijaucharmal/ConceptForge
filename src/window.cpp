#include <window.hpp>

#include <iostream>
#include <stdexcept>

namespace WindowManagement {
// Resize Callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

Window::Window(int w, int h, std::string name, bool fullscreen=false)
    : width{w}, height{h}, windowName{name}, fullscreen{fullscreen} {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


  if(fullscreen){
    const auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Init window
    width = mode->width;
    height = mode->height;
    window = glfwCreateWindow(mode->width, mode->height, windowName.c_str(), monitor, NULL);
  }
  else{
    window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
  }
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
  glEnable(GL_DEPTH_TEST);

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
