#include "uv_sphere.hpp"

#include <vector>
#include <cmath>

// Generates a sphere's vertex data (x, y, z, u, v)
std::vector<float> UVSphere::GenerateSphere() {
    std::vector<float> vertices;
    float x, y, z, u, v;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // from +pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);  // r * cos(theta)
        y = radius * sinf(stackAngle);         // r * sin(theta)

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;      // from 0 to 2pi

            x = xy * cosf(sectorAngle);        // r * cos(theta) * cos(phi)
            z = xy * sinf(sectorAngle);        // r * cos(theta) * sin(phi)

            u = static_cast<float>(j) / sectorCount;
            v = static_cast<float>(i) / stackCount;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    return vertices;
}

std::vector<uint> UVSphere::GenerateSphereIndices() {
    std::vector<uint> indices;

    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // start of current stack
        int k2 = k1 + sectorCount + 1;      // start of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // Two triangles per quad
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}

UVSphere::UVSphere(
    ShaderManagement::ShaderProgram &sp,
    int sectorCount,
    int stackCount,
    float radius
) : shaderProgram(sp) {

    this->sectorCount = sectorCount;
    this->stackCount = stackCount;
    this->radius = radius;

    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);
    scale = glm::vec3(1.0);

    std::vector<float> sphereVertices = GenerateSphere();
    std::vector<unsigned int> sphereIndices = GenerateSphereIndices();
    CreateSphereVAO(sphereVertices, sphereIndices);
    indexCount = sphereIndices.size();

    UpdateModelMatrix();
};

void UVSphere::Draw() {
  shaderProgram.setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0); // Unbind VAO
}

void UVSphere::CreateSphereVAO(const std::vector<float>& vertices, const std::vector<uint>& indices) {
    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate VBO and upload vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Generate EBO and upload index data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex attribute pointers (position = 0, texcoord = 1)
    GLsizei stride = 5 * sizeof(float);

    // Position: layout(location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates: layout(location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
}
