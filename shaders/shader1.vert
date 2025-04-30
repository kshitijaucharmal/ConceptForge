#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 myColor;
out vec2 TexCoord;

uniform float iTime;

float speed = 5.;

void main()
{
    // gl_Position = vec4(aPos.x * sin(iTime * speed), aPos.y * cos(iTime * speed), aPos.z, 1.0);
    gl_Position = vec4(aPos, 1.0f);
    myColor = aColor;
    TexCoord = aTexCoord;
}
