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

public:
  Window(int w, int h, std::string name);
  ~Window();
  GLFWwindow *window;
};

} // namespace WindowManagement
