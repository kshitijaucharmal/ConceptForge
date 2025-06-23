#include "cube.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) {
  position = pos;
  rotation = rot;
  scale = sca;

  name = "Cube";

  std::shared_ptr<Material> mat = std::make_shared<Material>();
  materials.push_back(mat);

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
  // stride = 8 floats = 8 * sizeof(float)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                     // aPos
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));   // aTexCoord
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));   // aNormal
  glEnableVertexAttribArray(2);
}

void Cube::Draw() {
  for(auto &material : materials){
    material->shader->Use();
    material->shader->setMat4("model", model);
  }

  // Draw
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
