#include "setup.hpp"
#include "primitives/uv_sphere.hpp"
#include "primitives/cube.hpp"

using namespace Engine;

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main() {
  ConceptForge forge;

  std::unique_ptr<UVSphere> light = std::make_unique<UVSphere>(forge.shaders[ShaderType::Light].get());
  light->SetPosition(glm::vec3(1.3, 100, 4));
  light->SetRotation(glm::vec3(-0.2f, -1.0f, -0.3f));
  light->SetScale(glm::vec3(0.2));
  light->name = "Light";
  UVSphere* light_ptr = light.get();
  forge.hierarchy.AddEntity(std::move(light));

  // Add a sphere and a Cube
  std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaders[ShaderType::Lit].get());
  sphere->SetPosition(glm::vec3(0, 3, 0));
  sphere->SetRotation(glm::vec3(-34, -2, 88));
  sphere->SetScale(glm::vec3(1, 1, 1));
  forge.hierarchy.AddEntity(std::move(sphere));

  // Many Cubes
  for(unsigned int i = 0; i < 10; i++) {
    std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaders[ShaderType::Lit].get());
    cube->SetPosition(cubePositions[i]);
    float angle = 20.0f * i;
    cube->Rotate(angle, glm::vec3(1.0f, 0.3f, 0.5f));
    forge.hierarchy.AddEntity(std::move(cube));

  }

  // Render Loop
  while (!forge.WindowShouldClose()) {
    forge.DeltaTimeCalc();
    forge.ProcessInput();

    for(auto const &entity : forge.hierarchy.entities){
      auto shader = entity.second->shader;
      // shader->setVec3("viewPos", forge.camera.Position);
      // shader->setVec3("light.direction",  light_ptr->GetRotation());

      // TODO: Set material using local files, and allow chaning in inspector
      shader->setFloat("material.shininess", 32.0f);
    }

    forge.CalcProjection();
    forge.GUIManagement();
    forge.Render();
  }

  return 0;
}
