#include "shaderman.hpp"
#include <iostream>
#include <glad/glad.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "utilities/fileloader.hpp"
#include "stb_image.h"


namespace ShaderManagement {
    ShaderProgram::ShaderProgram(DrawMode mode, std::string &vertexShaderPath, std::string &fragmentShaderPath) : drawMode{mode} {
        // Set Draw Mode
        switch(drawMode){
            case DrawMode::WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case DrawMode::FILLED:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }

        InitVertexShader(vertexShaderPath);
        InitFragmentShader(fragmentShaderPath);
        LinkShaders();
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

    void ShaderProgram::BindTextures(){
        unsigned int texture1 = BindTexture(TEXTURE_DIR "/awesomeface.png", "texture1", 0, true);
        unsigned int texture2 = BindTexture(TEXTURE_DIR "/wall.jpg", "texture2", 1, false);
    }

    // Texturepath is relative to TEXTURE_DIR for now
    unsigned int ShaderProgram::BindTexture(const char* texturePath, const char* textureShaderName, unsigned int textureLoc, bool flip){
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0 + textureLoc);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(flip);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data) {
            // If Alpha channel exists
            GLenum inputFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, inputFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            std::cerr << "Failed to load texture\n";
        }
        stbi_image_free(data);

        glUseProgram(shaderProgram);
        setInt(textureShaderName, textureLoc);
        return texture;
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
    void ShaderProgram::setMat4(const std::string &name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(shaderProgram);
    }

}
