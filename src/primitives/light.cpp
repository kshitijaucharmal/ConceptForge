
#include "light.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Light::Light(ShaderProgram& sp) : lightShader (sp) {
    // Initialize Vertex Array Object (Stores Vertex attribute pointers)
    glGenVertexArrays(1, &VAO);
    // Bind VAO
    glBindVertexArray(VAO);

    // Initialize Vertex Buffer Object
    glGenBuffers(1, &VBO);
    // bind VBO to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
