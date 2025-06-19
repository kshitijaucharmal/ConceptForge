#include "setup.hpp"
#include "primitives/uv_sphere.hpp"
#include "primitives/cube.hpp"
#include "primitives/light_ssbo.hpp"

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
  light->SetPosition(glm::vec3(0.7f,  0.2f,  2.0f));
  light->SetScale(glm::vec3(0.2));
  light->name = "Light";
  UVSphere* light_ptr1 = light.get();
  forge.hierarchy.AddEntity(std::move(light));

  light = std::make_unique<UVSphere>(forge.shaders[ShaderType::Light].get());
  light->SetPosition(glm::vec3(2.3f, -3.3f, -4.0f));
  light->SetScale(glm::vec3(0.2));
  light->name = "Light2";
  UVSphere* light_ptr2 = light.get();
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
  std::vector<PointLight> pointLights = {
      PointLight(glm::vec3(1,4,3), glm::vec3(0.05), glm::vec3(1.0), glm::vec3(1.0)),
      PointLight(glm::vec3(-2,10,0), glm::vec3(0.05), glm::vec3(0.8), glm::vec3(1.0))
  };

  // Render Loop
  while (!forge.WindowShouldClose()) {
    forge.DeltaTimeCalc();
    forge.ProcessInput();

    for(auto const &entity : forge.hierarchy.entities){
      auto shader = entity.second->shader;
      shader->setVec3("viewPos", forge.camera.Position);
      pointLights[0].position = light_ptr1->GetPosition();
      pointLights[1].position = light_ptr2->GetPosition();

      glBindBuffer(GL_SHADER_STORAGE_BUFFER, forge.pointLightSSBO);
      glBufferData(GL_SHADER_STORAGE_BUFFER, pointLights.size() * sizeof(PointLight), pointLights.data(), GL_DYNAMIC_DRAW);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    forge.CalcProjection();
    forge.GUIManagement();
    forge.Render();
  }

  return 0;
}
