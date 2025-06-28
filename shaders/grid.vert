#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 MVP;
uniform vec3 gridOffset;

void main() {
    gl_Position = MVP * vec4(aPos + gridOffset, 1.0);
}