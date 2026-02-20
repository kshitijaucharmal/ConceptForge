#version 430 core
layout (location = 0) in vec3 aPos; // Position from your Mesh VAO

uniform mat4 lightSpaceMatrix; // The "Camera" of the light
uniform mat4 model;            // The position of the object

void main() {
    // We transform the vertex directly into the light's world
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
