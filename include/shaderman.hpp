#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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
        DrawMode drawMode;
        // Example
        unsigned int texture1;
        unsigned int texture2;

    public:
        // TODO: Move to private
        unsigned int shaderProgram;
        ShaderProgram(DrawMode mode);
        ~ShaderProgram();
        void Use();
        void Draw();
        void InitVertexShader(std::string vertexShaderPath);
        void InitFragmentShader(std::string fragmentShaderPath);
        void LinkShaders();
        void SendDataToVS();
        void SendDataToFS();
        void BindTextures();
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, glm::vec2 value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
        void setVec4(const std::string &name, glm::vec4 value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;
    };
}
