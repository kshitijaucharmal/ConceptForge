#pragma once

// Extend from entity
#include "entity.hpp"
#include "shaderman.hpp"

#include <vector>

class UVSphere : public SimObject::Entity {
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int indexCount = 0;
    ShaderManagement::ShaderProgram shaderProgram;

public:
    UVSphere(ShaderManagement::ShaderProgram &sp, glm::vec3 pos=glm::vec3(0.0), glm::vec3 rot=glm::vec3(0.0), glm::vec3 sca=glm::vec3(1.0));

    std::vector<float> GenerateSphere(int sectorCount, int stackCount, float radius = 1.0f);
    std::vector<uint> GenerateSphereIndices(int sectorCount, int stackCount);

    void CreateSphereVAO(const std::vector<float>& vertices, const std::vector<uint>& indices);

    void Draw() override;
};
