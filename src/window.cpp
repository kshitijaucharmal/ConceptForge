#include <window.hpp>

#include <iostream>
#include <stdexcept>

#include "imgui.h"

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

  // Resize support
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, width, height);

  InitFramebuffer();
}

void Window::InitFramebuffer() {
  glGenTextures(1, &fboTexture);
  glBindTexture(GL_TEXTURE_2D, fboTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Framebuffer is not complete!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::RenderToFBO() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Window::RenderToImGui() {
  ImVec2 size = ImGui::GetContentRegionAvail();
  ImGui::Image((ImTextureID)(intptr_t)fboTexture, size, ImVec2(0, 1), ImVec2(1, 0));
}

void Window::ImGuiBegin(){
  ImGui::SetNextWindowPos(ImVec2(Const::assetBrowserWidth, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(Const::WIDTH - (Const::inspectorWidth + Const::assetBrowserWidth), Const::HEIGHT - Const::consoleHeight), ImGuiCond_Always);

  // Remove padding, (cause its invisible)
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::Begin("Viewport",
               nullptr,
               ImGuiWindowFlags_NoMove |
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoScrollbar |
               ImGuiWindowFlags_NoCollapse |
               ImGuiWindowFlags_NoBackground);

  ImVec2 renderSize = ImGui::GetContentRegionAvail();
  ImGui::InvisibleButton("CatchMouseInput", renderSize);

  // Pop back style vars
  ImGui::PopStyleVar(4);
}

void Window::ImGuiEnd() {
  ImGui::End();
}

// Destructor
Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

} // namespace WindowManagement
