#version 330 core
out vec4 FragColor;

in vec3 myColor;

uniform float iTime;

float speed = 5.;

void main()
{
    FragColor = vec4(myColor.xy, myColor.z * (sin(iTime * speed) / 2.0 + 0.5), 1.0);
}
