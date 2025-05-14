#include "cube.hpp"
#include "vertices_data.hpp"

extern float cubeVertices[];

Cube::Cube(unsigned int sp, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
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

// Rebuild the entire model matrix from scratch using the current transformation properties
void Cube::UpdateModelMatrix() {
  // Always start with identity matrix
  model = glm::mat4(1.0f);

  // Apply transformations in the correct order: Scale -> Rotate -> Translate
  model = glm::translate(model, position);

  // For rotation, convert Euler angles to quaternion for better interpolation
  glm::quat rotationQuat = glm::quat(glm::radians(rotation));
  model = model * glm::mat4_cast(rotationQuat);

  model = glm::scale(model, scale);
}

// Transform Tools ---------------------------------------------------------
void Cube::Translate(glm::vec3 pos) {
  position = pos;
  UpdateModelMatrix();
}

void Cube::Rotate(float angle, glm::vec3 axis) {
  // Convert angle-axis to Euler angles and add to current rotation
  glm::quat rotQuat = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
  glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotQuat));
  rotation += eulerAngles;
  UpdateModelMatrix();
}

void Cube::Rotate(glm::vec3 rot){
  rotation = rot;
  UpdateModelMatrix();
}

void Cube::Scale(glm::vec3 factor) {
  scale = factor;
  scale = glm::max(scale, 0.001f);
  UpdateModelMatrix();
}

// -------------------------------------------------------------------------

void Cube::GUI(){
  // Store previous values to detect changes
  glm::vec3 prevScale = scale;
  glm::vec3 prevRotation = rotation;
  glm::vec3 prevPosition = position;

  ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01,  -10.0, 10.0);
  ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.,  -180.0, 180.0);
  ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1,  0.0, 10.0);

  // Only update model matrix if values changed
  if (scale != prevScale || rotation != prevRotation || position != prevPosition) {
    UpdateModelMatrix();
  }
}

void Cube::Draw() { glDrawArrays(GL_TRIANGLES, 0, 36); }
