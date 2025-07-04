#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP; // Model-View-Projection matrix

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = 80.0; // Set point size here!
}