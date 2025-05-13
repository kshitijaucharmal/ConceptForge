// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>
#include <string>
#include <vector>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"

// Imgui Based UI
#include "editor/gui.hpp"

#include "primitives/cube.hpp"
#include "constants.hpp"

#include "ImGuizmo.h"

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
  // Capture Mouse

  ShaderManagement::ShaderProgram shaderProgram(DrawMode::FILLED, vertexShaderPath, fragmentShaderPath);
  InputManagement::Input input(window.window, camera);

  // glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetCursorPosCallback(window.window, mouse_callback);
  glfwSetScrollCallback(window.window, scroll_callback);

  shaderProgram.BindTextures();

  Cube cube(shaderProgram.shaderProgram);
  cube.Reset();
  cube.Translate(cubePositions[0]);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Main GUI
  GUIManagement::MainGUI mainGui(window.window, ImGui::GetIO());
  // Matrix transformations for projecting to the screen
  // V(clip) = M(projection)⋅M(view)⋅M(model)⋅V(local)
  static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
  static ImGuizmo::MODE gizmoMode = ImGuizmo::LOCAL;

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {
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

    shaderProgram.Use();

    // Projection Matrix
    glm::mat4 projection = glm::perspective(glm::radians(85.0f), ASPECTRATIO, 0.01f, 100.0f);;
    shaderProgram.setMat4("projection", projection);

    // Camera
    // Vector from position to origin
    glm::mat4 view = camera.GetViewMatrix();
    shaderProgram.setMat4("view", view);

    // Send Shader Data
    shaderProgram.SendDataToShader();
    shaderProgram.setMat4("model", cube.model);
    cube.Draw();

    // check and call events
    glfwPollEvents();

    // Imgui
    if (glfwGetWindowAttrib(window.window, GLFW_ICONIFIED) != 0)
    {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    glDisable(GL_DEPTH_TEST);

    // Create new Frame
    mainGui.NewFrame();

    ImGui::Begin("Controls");
    if (ImGui::RadioButton("Translate", gizmoOperation == ImGuizmo::TRANSLATE)) gizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", gizmoOperation == ImGuizmo::ROTATE)) gizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", gizmoOperation == ImGuizmo::SCALE)) gizmoOperation = ImGuizmo::SCALE;


    if (ImGui::RadioButton("Local", gizmoMode == ImGuizmo::LOCAL)) gizmoMode = ImGuizmo::LOCAL;
    ImGui::SameLine();
    if (ImGui::RadioButton("Global", gizmoMode == ImGuizmo::WORLD)) gizmoMode = ImGuizmo::WORLD;
    ImGui::End();

    ImGuiIO& io = ImGui::GetIO();
    // Debug window to verify mouse capture
    ImGui::Begin("Debug");
    ImGui::Text("ImGuizmo IsOver: %s", ImGuizmo::IsOver() ? "true" : "false");
    ImGui::Text("ImGuizmo IsUsing: %s", ImGuizmo::IsUsing() ? "true" : "false");
    ImGui::Text("Mouse Position: %.1f, %.1f", io.MousePos.x, io.MousePos.y);
    ImGui::Text("ImGui WantCaptureMouse: %s", io.WantCaptureMouse ? "true" : "false");
    ImGui::End();

    // Set up ImGuizmo
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(0, 0, (float)WIDTH, (float)HEIGHT);
    ImGuizmo::BeginFrame();

    ImGuizmo::Manipulate(glm::value_ptr(view),
                         glm::value_ptr(projection),
                         gizmoOperation,
                         gizmoMode,
                         glm::value_ptr(cube.model),
                         nullptr,
                         nullptr);

    // If the gizmo was used, decompose the result and update
    if (ImGuizmo::IsUsing()) {
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::quat rotQuat;

      glm::decompose(cube.model, cube.scale, rotQuat, cube.position, skew, perspective);
      cube.rotation = glm::degrees(glm::eulerAngles(rotQuat)); // Convert to Euler angles

      // Update the model matrix from the new values
      cube.UpdateModelMatrix();
    }

    // Testing
    cube.GUI();

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
