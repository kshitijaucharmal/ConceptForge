#include "shaderman.hpp"
#include <iostream>
#include <glad/glad.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "utilities/fileloader.hpp"

namespace ShaderManagement {
    ShaderProgram::ShaderProgram(){
        // Empty Contructor
    }

    void ShaderProgram::SetDrawMode(DrawMode mode) {
        drawMode = mode;
    }

    void ShaderProgram::ApplyDrawMode(){
        // Set Draw Mode
        switch(drawMode){
            case DrawMode::WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case DrawMode::FILLED:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }
    }

    void ShaderProgram::Init(DrawMode mode, std::string &vertexShaderPath, std::string &fragmentShaderPath) {
        drawMode = mode;

        InitVertexShader(vertexShaderPath);
        InitFragmentShader(fragmentShaderPath);
        LinkShaders();

        glDetachShader(shaderID, vertexShader);
        glDetachShader(shaderID, fragmentShader);
    }

    void ShaderProgram::InitVertexShader(std::string &vertexShaderPath) {
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

    void ShaderProgram::InitFragmentShader(std::string &fragmentShaderPath) {
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
        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertexShader);
        glAttachShader(shaderID, fragmentShader);
        glLinkProgram(shaderID);

        // Error handling for linking
        int success;
        char infoLog[512];
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::LINKING_ERR\n" << infoLog << std::endl;
        }

    }


    void ShaderProgram::SendDataToShader(){
        // TODO: Design a pipeline to send in these values from somewhere else
        // Calculate / Get Values
        float timeValue = glfwGetTime();

        // Send values
        setFloat("iTime", timeValue);
    }

    // Generic Types
    void ShaderProgram::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
    }
    void ShaderProgram::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
    }
    void ShaderProgram::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
    }
    void ShaderProgram::setVec2(const std::string &name, glm::vec2 value) const
    {
        glUniform2f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y);
    }
    void ShaderProgram::setVec3(const std::string &name, glm::vec3 value) const
    {
        glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z);
    }
    void ShaderProgram::setVec4(const std::string &name, glm::vec4 value) const
    {
        glUniform4f(glGetUniformLocation(shaderID, name.c_str()), value.x, value.y, value.z,
value.w);
    }
    void ShaderProgram::setMat4(const std::string &name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(shaderID);
        ApplyDrawMode();
    }

}
