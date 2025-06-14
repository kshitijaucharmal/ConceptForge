#include "cube.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Cube::Cube(ShaderManagement::ShaderProgram &sp, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
  : shaderProgram(sp) {

  position = pos;
  rotation = rot;
  scale = sca;

  name = "Cube";

  SetupVAO();
  SetupVBO();
  SetupVertexData();
  UpdateModelMatrix();
};

void Cube::SetupVAO(){
  // Initialize Vertex Array Object (Stores Vertex attribute pointers)
  glGenVertexArrays(1, &VAO);
  // Bind VAO
  glBindVertexArray(VAO);
}

void Cube::SetupVBO(){
  // Initialize Vertex Buffer Object
  glGenBuffers(1, &VBO);
  // bind VBO to the buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
}

void Cube::SetupVertexData(){
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void Cube::Draw() {
  shaderProgram.setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
