// Includes GLFW and glad
#include "window.hpp"
#include "input.hpp"
#include "shaderman.hpp"

#include <iostream>

// Storing vertex shader as a const char* for now
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const int WIDTH = 800;
const int HEIGHT = 600;
std::string WINDOWNAME = "OpenGL Learning";


int main() {
  WindowManagement::Window window(WIDTH, HEIGHT, WINDOWNAME);
  InputManagement::Input input;
  ShaderManagement::ShaderProgram shaderProgram;

  shaderProgram.InitVertexShader(vertexShaderSource);
  shaderProgram.InitFragmentShader(fragmentShaderSource);
  shaderProgram.LinkShaders();

  // Render Loop
  while (!glfwWindowShouldClose(window.window)) {

    // input processing
    input.processInput(window.window);

    // rendering commands
    // Clear Screen with this color
    glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Draw();

    // check and call events and swap buffers
    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }

  return 0;
}
