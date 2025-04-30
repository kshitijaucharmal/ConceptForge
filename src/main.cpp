// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>
#include <string>
#include "glm/vec4.hpp"

// Imgui Based UI
#include "gui.hpp"


// Shader Paths
const std::string vertexShaderPath = std::string(SHADER_DIR) + "/shader1.vert";
const std::string fragmentShaderPath = std::string(SHADER_DIR) + "/shader1.frag";

// Constants
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const std::string WINDOWNAME = "ConceptForge";

static glm::vec4 clearColor(0.05, 0.05, 0.05, 1.0f);

int main() {
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME);
  InputManagement::Input input;
  ShaderManagement::ShaderProgram shaderProgram(DrawMode::FILLED);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Main GUI
  GUIManagement::MainGUI mainGui(window.window, ImGui::GetIO());


  // Shader Management
  shaderProgram.InitVertexShader(vertexShaderPath);
  shaderProgram.InitFragmentShader(fragmentShaderPath);
  shaderProgram.LinkShaders();
  shaderProgram.BindTexture();

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {
    // input processing
    input.processInput(window.window);

    // rendering commands
    // Clear Screen with this color
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    // Send Shader Data
    shaderProgram.SendDataToVS();
    shaderProgram.SendDataToFS();

    // Draw
    shaderProgram.Draw();

    // check and call events
    glfwPollEvents();

    // Imgui
    if (glfwGetWindowAttrib(window.window, GLFW_ICONIFIED) != 0)
    {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // Create new Frame
    mainGui.NewFrame();

    // Show demo window
    mainGui.DemoWindow(clearColor);

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
  }

  return 0;
}
