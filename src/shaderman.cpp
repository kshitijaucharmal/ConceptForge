#include "shaderman.hpp"
#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

// Vertices for a triangle
float vertices[] = {
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  0.0f,  0.5f, 0.0f
};

namespace ShaderManagement {
    ShaderProgram::ShaderProgram() {
        // Initialize Vertex Array Object (Stores Vertex attribute pointers)
        glGenVertexArrays(1, &VAO);
        // Bind VAO
        glBindVertexArray(VAO);

        // Vertex Buffer Object
        glGenBuffers(1, &VBO);
        // bind VBO to the buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // As the triangle position does not change frequently
        // GL_STATIC_DRAW is used
        // GL_DYNAMIC_DRAW
        // GL_STREAM_DRAW
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    void ShaderProgram::InitVertexShader(const char* vertexShaderSource) {
        // Defining That this is a vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Setting the shader source
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        // This is how its diff from vulkan
        // Vulkan takes in SPIR-V compiled shaders, so whole
        // Code does not need to be compiled if shader is changed
        glCompileShader(vertexShader);

        // ErrorHandling
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            exit(0);
        }
    }

    void ShaderProgram::InitFragmentShader(const char* fragmentShaderSource) {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Error handling
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            exit(0);
        }
    }

    void ShaderProgram::LinkShaders(){
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Error handling
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::LINKING_ERR\n" << infoLog << std::endl;
        }

        // TODO: Make another function for this
        // Sending data into the vertex shader
        // location, n values, value type, normalized?, stride, position offset of type (void*)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

        ShaderProgram::~ShaderProgram() {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void ShaderProgram::Draw(){
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
