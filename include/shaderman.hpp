#pragma once

namespace ShaderManagement {
    class ShaderProgram{
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;

    public:
        ShaderProgram();
        ~ShaderProgram();
        void Draw();
        // TODO Remove Source with filename
        void InitVertexShader(const char* vertexShaderSource);
        void InitFragmentShader(const char* fragmentShaderSource);
        void LinkShaders();
    };
}
