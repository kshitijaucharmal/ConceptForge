// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>
#include <string>
#include <vector>

// GLM
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "constants.hpp"
#include "projection.hpp"

// Imgui Based UI
#include "editor/gui.hpp"

#include "primitives/cube.hpp"

#include "editor/gizmo.hpp"
#include "editor/inspector.hpp"
#include "editor/asset_browser.hpp"

// Cube positions
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    // glm::vec3( 2.0f,  5.0f, -15.0f),
    // glm::vec3(-1.5f, -2.2f, -2.5f),
    // glm::vec3(-3.8f, -2.0f, -12.3f),
    // glm::vec3( 2.4f, -0.4f, -3.5f),
    // glm::vec3(-1.7f,  3.0f, -7.5f),
    // glm::vec3( 1.3f, -2.0f, -2.5f),
    // glm::vec3( 1.5f,  2.0f, -2.5f),
    // glm::vec3( 1.5f,  0.2f, -1.5f),
    // glm::vec3(-1.3f,  1.0f, -1.5f)
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// camera
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

static glm::vec4 clearColor(0.1, 0.1, 0.1, 1.0f);

int main() {
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME, false);

  ShaderManagement::ShaderProgram shaderProgram(DrawMode::FILLED, vertexShaderPath, fragmentShaderPath);
  InputManagement::Input input(window.window, camera);

  // Projection Logic
  Projection projection;

  // TODO: Camera Fly mode
  // glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetCursorPosCallback(window.window, mouse_callback);
  glfwSetScrollCallback(window.window, scroll_callback);

  // Bind all textures
  shaderProgram.BindTextures();

  Cube cube(shaderProgram);
  cube.Translate(cubePositions[0]);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Main GUI
  Editor::MainGUI mainGui(window.window, ImGui::GetIO());

  Editor::Gizmo gizmo;
  Editor::Inspector inspector;
  Editor::AssetBrowser asset_browser;
  std::string myDirectory = "/home/kshitij";

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {
    // Delta Time calculation
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input processing
    input.processInput(deltaTime);

    // rendering commands
    // Clear Screen with this color
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
    glEnable(GL_DEPTH_TEST);

    projection.Calculate(camera, shaderProgram);

    // Send Shader Data
    cube.Draw();

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

    // Draw All UI
    mainGui.ShowConsole();
    gizmo.Show(cube, projection, camera);
    inspector.Show(cube, gizmo.gizmoOperation, gizmo.gizmoMode);
    asset_browser.Show(myDirectory);

    // Render
    mainGui.RenderFrame();

    glfwSwapBuffers(window.window);
  }

  return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
