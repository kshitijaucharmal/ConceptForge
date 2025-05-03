#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

namespace WindowManagement {

class Window {

private:
  int width;
  int height;
  std::string windowName;
  bool fullscreen;

public:
  Window(int w, int h, std::string name, bool fullscreen);
  ~Window();
  GLFWwindow *window;
};

} // namespace WindowManagement
