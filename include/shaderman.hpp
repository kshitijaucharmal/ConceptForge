#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum DrawMode {
    WIREFRAME,
    FILLED
};

namespace ShaderManagement {
    class ShaderProgram{
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        DrawMode drawMode;
        // Example
        unsigned int texture1;
        unsigned int texture2;

    public:
        ShaderProgram(DrawMode mode);
        ~ShaderProgram();
        void Draw();
        void InitVertexShader(std::string vertexShaderPath);
        void InitFragmentShader(std::string fragmentShaderPath);
        void LinkShaders();
        void SendDataToVS();
        void SendDataToFS();
        void BindTexture();
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, glm::vec2 value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
        void setVec4(const std::string &name, glm::vec4 value) const;
    };
}
