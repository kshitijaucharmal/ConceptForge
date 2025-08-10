#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform float iTime;

// Transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float speed = 5.;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // Pass normals for lighting

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
