#include "shaderman.hpp"
#include <iostream>
#include <glad/glad.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "fileloader.hpp"

float vertices[] = {
    // positions         // colors
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
};

namespace ShaderManagement {
    ShaderProgram::ShaderProgram(DrawMode mode) : drawMode{mode} {

        // Set Draw Mode
        switch(drawMode){
            case DrawMode::WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case DrawMode::FILLED:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }

        // Initialize Vertex Array Object (Stores Vertex attribute pointers)
        glGenVertexArrays(1, &VAO);
        // Bind VAO
        glBindVertexArray(VAO);

        // Initialize Vertex Buffer Object
        glGenBuffers(1, &VBO);
        // bind VBO to the buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // As the triangle position does not change frequently
        // GL_STATIC_DRAW is used
        // GL_DYNAMIC_DRAW
        // GL_STREAM_DRAW
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Initialize Element Buffer Object
        // Stores indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void ShaderProgram::InitVertexShader(std::string vertexShaderPath) {
        FileLoader vertexLoader(vertexShaderPath);
        const char* vertexShaderSource = vertexLoader.getData();

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

    void ShaderProgram::InitFragmentShader(std::string fragmentShaderPath) {
        FileLoader fragmentLoader(fragmentShaderPath);
        const char* fragmentShaderSource = fragmentLoader.getData();

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

        // Error handling for linking
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::LINKING_ERR\n" << infoLog << std::endl;
        }
    }

    void ShaderProgram::SendDataToVS(){
        // Sending data into the vertex shader
        // location, n values, value type, normalized?, stride, position offset of type (void*)
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void ShaderProgram::SendDataToFS(){
        // TODO: Design a pipeline to send in these values from somewhere else
        // Calculate / Get Values
        float timeValue = glfwGetTime();

        // Send values
        setFloat("iTime", timeValue);
    }

    // Generic Types
    void ShaderProgram::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
    }
    void ShaderProgram::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void ShaderProgram::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void ShaderProgram::setVec2(const std::string &name, glm::vec2 value) const
    {
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y);
    }
    void ShaderProgram::setVec3(const std::string &name, glm::vec3 value) const
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z);
    }
    void ShaderProgram::setVec4(const std::string &name, glm::vec4 value) const
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z,
value.w);
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void ShaderProgram::Draw(){
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
}
