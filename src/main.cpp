#include "setup.hpp"
#include "primitives/uv_sphere.hpp"
#include "primitives/cube.hpp"

using namespace Engine;

int main() {
  ConceptForge forge;

  std::unique_ptr<UVSphere> light = std::make_unique<UVSphere>(forge.shaders[ShaderType::Light].get());
  light->SetPosition(glm::vec3(1.3, 6, 4));
  light->SetScale(glm::vec3(0.2));
  light->name = "Light";
  UVSphere* light_ptr = light.get();
  forge.hierarchy.AddEntity(std::move(light));

  // Add a sphere and a Cube
  std::unique_ptr<UVSphere> sphere = std::make_unique<UVSphere>(forge.shaders[ShaderType::Lit].get());
  sphere->SetPosition(glm::vec3(1, 3, 1));
  sphere->SetRotation(glm::vec3(0, 0, 0));
  sphere->SetScale(glm::vec3(1, 1, 1));
  forge.hierarchy.AddEntity(std::move(sphere));

  std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaders[ShaderType::Lit].get());
  forge.hierarchy.AddEntity(std::move(cube));

  // Render Loop
  while (!forge.WindowShouldClose()) {
    forge.DeltaTimeCalc();
    forge.ProcessInput();
    forge.Render();

    for(auto const &entity : forge.hierarchy.entities){
      entity.second->shaderProgram->setVec3("lightPos", light_ptr->GetPosition());
      entity.second->shaderProgram->setVec3("viewPos", forge.camera.Position);
    }

    forge.CalcProjection();
    forge.GUIManagement();
  }

  return 0;
}
