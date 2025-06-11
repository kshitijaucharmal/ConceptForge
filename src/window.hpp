#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

#include "constants.hpp"
#include "glm/glm.hpp"
#include "imgui.h"

namespace WindowManagement {

class Window {

private:
  int width;
  int height;
  std::string windowName;
  bool fullscreen;

  GLuint fbo = 0, fboTexture = 0, rbo = 0;

public:
  Window(int w, int h, std::string name, bool fullscreen);
  ~Window();
  GLFWwindow *window;
  void RenderToFBO();    // Render your scene here
  void InitFramebuffer(); // FBO setup

  // Call in this order
  void ImGuiBegin();
  void RenderToImGui();  // Shows FBO texture in ImGui
  ImVec2 viewportPos, viewportSize;
  void ImGuiEnd();
};

} // namespace WindowManagement
