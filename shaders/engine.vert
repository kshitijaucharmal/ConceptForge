#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 myColor;
out vec2 TexCoord;

uniform float iTime;

// Transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float speed = 5.;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    myColor = aPos;
    TexCoord = aTexCoord;
}
