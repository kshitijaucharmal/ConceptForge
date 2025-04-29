#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 myColor;
uniform float iTime;

float speed = 5.;

void main()
{
    gl_Position = vec4(aPos.x * sin(iTime * speed), aPos.y * cos(iTime * speed), aPos.z, 1.0);
    myColor = aColor;
}
