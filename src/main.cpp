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
  sphere->SetPosition(glm::vec3(0, 3, 0));
  sphere->SetRotation(glm::vec3(0, 0, 0));
  sphere->SetScale(glm::vec3(1, 1, 1));
  forge.hierarchy.AddEntity(std::move(sphere));

  std::unique_ptr<Cube> cube = std::make_unique<Cube>(forge.shaders[ShaderType::Lit].get());
  forge.hierarchy.AddEntity(std::move(cube));

  // Render Loop
  while (!forge.WindowShouldClose()) {
    forge.DeltaTimeCalc();
    forge.ProcessInput();

    for(auto const &entity : forge.hierarchy.entities){
      auto lightingShader = entity.second->shader;
      lightingShader->setVec3("viewPos", forge.camera.Position);
      lightingShader->setVec3("light.position",  light_ptr->GetPosition());
      lightingShader->setVec3("light.ambient",  glm::vec3(0.2f));
      lightingShader->setVec3("light.diffuse",  glm::vec3(0.5f));
      lightingShader->setVec3("light.specular", glm::vec3(1.0f));

      // TODO: Set material using local files, and allow chaning in inspector
      lightingShader->setVec3("material.ambient", glm::vec3(1.0f, 0.31f, 0.5f));
      lightingShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.31f, 0.5f));
      lightingShader->setVec3("material.specular", glm::vec3(0.5f));
      lightingShader->setFloat("material.shininess", 16.0f);
    }

    forge.CalcProjection();
    forge.GUIManagement();
    forge.Render();
  }

  return 0;
}
