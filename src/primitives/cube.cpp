#include "cube.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Cube::Cube(unsigned int sp) : shaderProgram(sp) {
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

  Reset();
};


void Cube::Reset(){
  model = glm::mat4(1.0f);
}

void Cube::Translate(glm::vec3 pos) {
  position = pos;
  model = glm::translate(model, position);
}

void Cube::Update(){
  Translate(position);
}

void Cube::Rotate(float angle, glm::vec3 axis) {
  model = glm::rotate(model, glm::radians(angle), axis);
}

void Cube::Use() { glUseProgram(shaderProgram); }

void Cube::Draw() { glDrawArrays(GL_TRIANGLES, 0, 36); }
