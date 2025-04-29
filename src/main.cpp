// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>
#include <string>
const std::string vertexShaderPath = std::string(SHADER_DIR) + "/shader1.vert";
const std::string fragmentShaderPath = std::string(SHADER_DIR) + "/shader1.frag";

static const int WIDTH = 800;
static const int HEIGHT = 600;
std::string WINDOWNAME = "OpenGL Learning";

int main() {
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME);
  InputManagement::Input input;
  ShaderManagement::ShaderProgram shaderProgram(DrawMode::FILLED);

  // Shader Management
  shaderProgram.InitVertexShader(vertexShaderPath);
  shaderProgram.InitFragmentShader(fragmentShaderPath);
  shaderProgram.LinkShaders();

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {
    // input processing
    input.processInput(window.window);

    // rendering commands
    // Clear Screen with this color
    glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Send Data
    shaderProgram.SendDataToVS();
    shaderProgram.SendDataToFS();
    shaderProgram.Draw();

    // check and call events and swap buffers
    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }

  return 0;
}
