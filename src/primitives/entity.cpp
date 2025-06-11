
#include "entity.hpp"

using namespace SimObject;

// Rebuild the entire model matrix from scratch using the current transformation properties
void Entity::UpdateModelMatrix() {
  // Always start with identity matrix
  model = glm::mat4(1.0f);

  // Apply transformations in the correct order: Scale -> Rotate -> Translate
  model = glm::translate(model, position);

  // For rotation, convert Euler angles to quaternion for better interpolation
  glm::quat rotationQuat = glm::quat(glm::radians(rotation));
  model = model * glm::mat4_cast(rotationQuat);

  model = glm::scale(model, scale);
}

glm::mat4 Entity::GetModel(){
  return model;
}

void* Entity::GetModelPointer(){
  return glm::value_ptr(model);
}

// === Getters ===
// -------------------------------------------------------------------------
glm::vec3 Entity::GetPosition() {
  return position;
}
glm::vec3 Entity::GetRotation() {
  return rotation;
}
glm::vec3 Entity::GetScale() {
  return scale;
}

// -------------------------------------------------------------------------

// === Setters ===
// -------------------------------------------------------------------------
void Entity::SetPosition(glm::vec3 pos) {
  position = pos;
  UpdateModelMatrix();
}

void Entity::SetRotation(glm::vec3 eulerAngles) {
  rotation = eulerAngles;
  UpdateModelMatrix();
}

void Entity::SetScale(glm::vec3 scale) {
  scale = glm::max(scale, 0.001f);  // Prevent degenerate scale
  UpdateModelMatrix();
}
// -------------------------------------------------------------------------

// === Relative Modifiers ===
// -------------------------------------------------------------------------
void Entity::Translate(glm::vec3 delta) {
  position += delta;
  UpdateModelMatrix();
}

void Entity::Rotate(float angle, glm::vec3 axis) {
  glm::quat deltaQuat = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
  glm::quat currentQuat = glm::quat(glm::radians(rotation));
  glm::quat newQuat = deltaQuat * currentQuat;
  rotation = glm::degrees(glm::eulerAngles(newQuat));
  UpdateModelMatrix();
}

void Entity::Rotate(glm::vec3 eulerDelta) {
  rotation += eulerDelta;
  UpdateModelMatrix();
}

void Entity::Scale(glm::vec3 factorDelta) {
  scale *= factorDelta;
  scale = glm::max(scale, 0.001f);  // Clamp to prevent collapse
  UpdateModelMatrix();
}
// -------------------------------------------------------------------------

void Entity::GUI(){
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

void Entity::Draw(){

}
