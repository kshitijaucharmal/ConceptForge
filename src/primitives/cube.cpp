#include "cube.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Cube::Cube(ShaderManagement::ShaderProgram &sp, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
  : shaderProgram(sp) {

  position = pos;
  rotation = rot;
  scale = sca;

  SetupTextures();
  UpdateModelMatrix();
};

void Cube::SetupTextures(){
  // Initialize Vertex Array Object (Stores Vertex attribute pointers)
  glGenVertexArrays(1, &VAO);
  // Initialize Vertex Buffer Object
  glGenBuffers(1, &VBO);
  // Bind VAO
  glBindVertexArray(VAO);

  // bind VBO to the buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

#include <iostream>

void Cube::Draw() {
  shaderProgram.setMat4("model", model);
  Entity::Draw();
}
