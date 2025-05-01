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

// Cube positions
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Shader Paths
const std::string vertexShaderPath = std::string(SHADER_DIR) + "/shader1.vert";
const std::string fragmentShaderPath = std::string(SHADER_DIR) + "/shader1.frag";

// Constants
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
static const std::string WINDOWNAME = "ConceptForge";

static glm::vec4 clearColor(0.05, 0.05, 0.05, 1.0f);

int main() {
  std::cout << ASPECTRATIO << std::endl;
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
  shaderProgram.BindTextures();

  // Matrix transformations for projecting to the screen
  // V(clip) = M(projection)⋅M(view)⋅M(model)⋅V(local)

  // 1. Model Matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

  // 2. View Matrix
  // > To move a camera backwards, is the same as moving the entire scene forward.
  glm::mat4 view = glm::mat4(1.0f);
  // ^ translating the scene in the reverse direction of where we want to move
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  // 3. Projection Matrix
  bool perspective = true;
  glm::mat4 projection;
  if(perspective) projection = glm::perspective(glm::radians(45.0f), ASPECTRATIO, 0.1f, 100.0f);
  else projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, 0.1f, 100.0f);

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {
    // input processing
    input.processInput(window.window);

    // rendering commands
    // Clear Screen with this color
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


    // Send Shader Data
    shaderProgram.SendDataToVS();
    shaderProgram.SendDataToFS();

    // Sending Matrix Transformations to shader
    shaderProgram.setMat4("view", view);
    shaderProgram.setMat4("projection", projection);

    for(unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shaderProgram.setMat4("model", model);

      // Draw
      shaderProgram.Use();
      shaderProgram.Draw();
    }


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

    mainGui.DemoWindow(clearColor);

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  }

  return 0;
}
