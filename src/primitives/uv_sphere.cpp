#include "uv_sphere.hpp"

#include <vector>
#include <cmath>

UVSphere::UVSphere() {
    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);
    scale = glm::vec3(1.0);

    name = "UV Sphere";

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    materials.push_back(mat);

    Init();
};

std::vector<float> UVSphere::GenerateSphere() {
    std::vector<float> vertices;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep; // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);
        float y = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);

            // normalized normal
            glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));

            // texture coords (u, v)
            float u = static_cast<float>(j) / sectorCount;
            float v = static_cast<float>(i) / stackCount;

            // Push in position, texcoord, normal
            vertices.insert(vertices.end(), {x, y, z, u, v, norm.x, norm.y, norm.z});
        }
    }

    return vertices;
}

std::vector<uint> UVSphere::GenerateSphereIndices() {
    std::vector<uint> indices;

    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                // First triangle (k1 -> k2 -> k1+1)
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                // Second triangle (k1+1 -> k2 -> k2+1)
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}

void UVSphere::Init(int sectorCount, int stackCount, float radius){
    this->sectorCount = sectorCount;
    this->stackCount = stackCount;
    this->radius = radius;
    std::vector<float> sphereVertices = GenerateSphere();
    std::vector<unsigned int> sphereIndices = GenerateSphereIndices();
    CreateSphereVAO(sphereVertices, sphereIndices);
    indexCount = sphereIndices.size();

    UpdateModelMatrix();
}

void UVSphere::Draw() {
    for(auto &material : materials){
        material->shader->Use();
        material->shader->setMat4("model", model);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO
}

void UVSphere::CreateSphereVAO(const std::vector<float>& vertices, const std::vector<uint>& indices) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLsizei stride = 8 * sizeof(float);

    // Position: layout(location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates: layout(location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normals: layout(location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

