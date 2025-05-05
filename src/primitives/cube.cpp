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
  position = glm::vec3(0.0);
  rotation = glm::vec3(0.0);
  scale = glm::vec3(1.0);
  model = glm::mat4(1.0f);
  Update();
}

// Transform Tools ---------------------------------------------------------
void Cube::Translate(glm::vec3 pos) {
  position = pos;
  model = glm::translate(model, position);
}

void Cube::Rotate(float angle, glm::vec3 axis) {
  model = glm::rotate(model, glm::radians(angle), axis);
}

void Cube::Rotate(glm::vec3 rot){
  rotation = rot;
  glm::quat rotationQuat = glm::quat(glm::radians(rot)); // Convert Euler to quaternion
  glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat); // Convert quaternion to matrix

  model *= rotationMatrix;
}

void Cube::Scale(glm::vec3 factor) {
  model = glm::scale(model, factor);
}

// -------------------------------------------------------------------------

void Cube::Update(){
  Translate(position);
  Rotate(rotation);
  Scale(scale);
}

void Cube::GUI(){
  ImGui::Begin("Transform");                          // Create a window called "Hello, world!" and append into it.
  // Testing
  ImGui::DragFloat3("Scale", glm::value_ptr(scale),0.1,  0.0, 10.0);
  ImGui::DragFloat3("Rotate", glm::value_ptr(rotation),1.,  -180.0, 180.0);
  ImGui::DragFloat3("Position", glm::value_ptr(position),0.01,  -10.0, 10.0);
  ImGui::End();
}


void Cube::Use() { glUseProgram(shaderProgram); }

void Cube::Draw() { glDrawArrays(GL_TRIANGLES, 0, 36); }
