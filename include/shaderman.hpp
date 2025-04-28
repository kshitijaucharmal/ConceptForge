#pragma once
#include <string>

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

    public:
        ShaderProgram(DrawMode mode);
        ~ShaderProgram();
        void Draw();
        // TODO Remove Source with filename
        void InitVertexShader(std::string vertexShaderPath);
        void InitFragmentShader(std::string fragmentShaderPath);
        void LinkShaders();
    };
}
