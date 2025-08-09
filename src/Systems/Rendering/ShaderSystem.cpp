
#include "ShaderSystem.hpp"

#include "Components/Constants.hpp"
#include "Core/Utilities/FileLoader.hpp"

namespace ShaderSystem {
    void InitShaders(entt::registry &registry){
        auto shaders = registry.ctx().get<ShaderStore>().shaders;
        for (auto &shaderPair : shaders) {
            auto &shader = registry.get<Shader>(shaderPair.second);
            if (shader.initialized){
                continue;
            }

            InitVertexShader(shader, shader.vertexShaderPath);
            InitFragmentShader(shader, shader.fragmentShaderPath);
            LinkShader(shader);

            shader.initialized = true;
            // Detach After linking done
            glDetachShader(shader.shaderID, shader.vertexShader);
            glDetachShader(shader.shaderID, shader.fragmentShader);
        }
    }

    void Use(Shader &shader) {
        glUseProgram(shader.shaderID);

        switch(shader.drawMode){
            case WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case FILLED:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }
    }

    void SetDrawMode(entt::registry &registry, DrawMode mode){
        auto view = registry.view<Shader>();
        for(auto &entity : view){
            auto &shader = view.get<Shader>(entity);
            shader.drawMode = mode;
        }
    }

    void InitVertexShader(Shader &shader, std::string vertexShaderPath){
        FileLoader vertexLoader(vertexShaderPath);
        const char* vertexShaderSource = vertexLoader.getData();

        // Defining That this is a vertex shader
        shader.vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Setting the shader source
        glShaderSource(shader.vertexShader, 1, &vertexShaderSource, NULL);
        // This is how its diff from vulkan
        // Vulkan takes in SPIR-V compiled shaders, so whole
        // Code does not need to be compiled if shader is changed
        glCompileShader(shader.vertexShader);

        // ErrorHandling
        int success;
        char infoLog[512];
        glGetShaderiv(shader.vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader.vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            exit(0);
        }
    }

    void InitFragmentShader(Shader &shader, std::string fragmentShaderPath){
        FileLoader fragmentLoader(fragmentShaderPath);
        const char* fragmentShaderSource = fragmentLoader.getData();

        shader.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader.fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(shader.fragmentShader);

        // Error handling
        int success;
        char infoLog[512];
        glGetShaderiv(shader.fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader.fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            exit(0);
        }

    }

    void LinkShader(Shader &shader){
        shader.shaderID = glCreateProgram();

        glAttachShader(shader.shaderID, shader.vertexShader);
        glAttachShader(shader.shaderID, shader.fragmentShader);
        glLinkProgram(shader.shaderID);

        // Error handling for linking
        int success;
        char infoLog[512];
        glGetProgramiv(shader.shaderID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader.shaderID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::LINKING_ERR\n" << infoLog << std::endl;
        }
    }

    // Generic Types
    void setBool(Shader &shader, const std::string &name, bool value) {
        glUniform1i(glGetUniformLocation(shader.shaderID, name.c_str()), (int)value);
    }
    void setInt(Shader &shader, const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(shader.shaderID, name.c_str()), value);
    }
    void setFloat(Shader &shader, const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(shader.shaderID, name.c_str()), value);
    }
    // ----------------------------------------------------------------------------------------------------------
    void setVec2(Shader &shader, const std::string &name, glm::vec2 value) {
        glUniform2f(glGetUniformLocation(shader.shaderID, name.c_str()), value.x, value.y);
    }
    void setVec2(Shader &shader, const std::string &name, float x, float y) {
        glUniform2f(glGetUniformLocation(shader.shaderID, name.c_str()), x, y);
    }
    void setVec2(Shader &shader, const std::string &name, float a) {
        glUniform2f(glGetUniformLocation(shader.shaderID, name.c_str()), a, a);
    }
    // ----------------------------------------------------------------------------------------------------------
    void setVec3(Shader &shader, const std::string &name, glm::vec3 value) {
        glUniform3f(glGetUniformLocation(shader.shaderID, name.c_str()), value.x, value.y, value.z);
    }
    void setVec3(Shader &shader, const std::string &name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(shader.shaderID, name.c_str()), x, y, z);
    }
    void setVec3(Shader &shader, const std::string &name, float a) {
        glUniform3f(glGetUniformLocation(shader.shaderID, name.c_str()), a, a, a);
    }
    // ----------------------------------------------------------------------------------------------------------
    void setVec4(Shader &shader, const std::string &name, glm::vec4 value) {
        glUniform4f(glGetUniformLocation(shader.shaderID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void setVec4(Shader &shader, const std::string &name, float x, float y, float z, float w) {
        glUniform4f(glGetUniformLocation(shader.shaderID, name.c_str()), x, y, z, w);
    }
    void setVec4(Shader &shader, const std::string &name, float a) {
        glUniform4f(glGetUniformLocation(shader.shaderID, name.c_str()), a, a, a, a);
    }
    // ----------------------------------------------------------------------------------------------------------
    void setMat4(Shader &shader, const std::string &name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

}
