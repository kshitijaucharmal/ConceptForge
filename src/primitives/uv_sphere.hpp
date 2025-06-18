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

public:
    UVSphere(ShaderManagement::ShaderProgram *sp);
    void Init(int sectorCount=36, int stackCount=18, float radius=0.5);

    int sectorCount;
    int stackCount;
    float radius;

    std::vector<float> GenerateSphere();
    std::vector<uint> GenerateSphereIndices();

    void CreateSphereVAO(const std::vector<float>& vertices, const std::vector<uint>& indices);

    void Draw() override;
};
