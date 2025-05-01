// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>
#include <string>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Imgui Based UI
#include "gui.hpp"


// Shader Paths
const std::string vertexShaderPath = std::string(SHADER_DIR) + "/shader1.vert";
const std::string fragmentShaderPath = std::string(SHADER_DIR) + "/shader1.frag";

// Constants
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float aspect_ratio = (float)WIDTH / (float)HEIGHT;
static const std::string WINDOWNAME = "ConceptForge";

static glm::vec4 clearColor(0.05, 0.05, 0.05, 1.0f);

int main() {
  std::cout << aspect_ratio << std::endl;
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME, false);
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

  float d = 0.0f;
  // Gives an identity matrix
  glm::mat4 trans = glm::mat4(1.0);
  trans = glm::rotate(trans, d, glm::vec3(0.0f, 0.0f, 1.0f));

  unsigned int transformLoc = glGetUniformLocation(shaderProgram.shaderProgram, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

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

    trans = glm::mat4(1.0);
    trans = glm::rotate(trans, d, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

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
    ImGui::Begin("Hello World");
    ImGui::SliderFloat("float", &d, -1.0f * glm::pi<float>(), glm::pi<float>());            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::End();

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
  }

  return 0;
}
