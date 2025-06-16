#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

enum DrawMode {
    WIREFRAME,
    FILLED
};


namespace ShaderManagement {
    enum ShaderType {
        Unlit,
        Lit,
        Light
    };
    class ShaderProgram{
    private:
        unsigned int vertexShader;
        unsigned int fragmentShader;
        DrawMode drawMode;

        void InitVertexShader(std::string &vertexShaderPath);
        void InitFragmentShader(std::string &fragmentShaderPath);
        void ApplyDrawMode();

    public:
        unsigned int shaderProgram;
        ShaderProgram();
        void SetDrawMode(DrawMode mode);
        void Init(DrawMode mode, std::string &vertexShaderPath, std::string &fragmentShaderPath);
        ~ShaderProgram();
        void Use();
        void LinkShaders();
        void SendDataToShader();
        unsigned int BindTexture(const char* texturePath, const char* textureShaderName, unsigned int textureLoc, bool flip=true);
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
