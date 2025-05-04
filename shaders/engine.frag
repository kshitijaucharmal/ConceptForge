#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float iTime;
uniform sampler2D texture1;
uniform sampler2D texture2;

float speed = 5.;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}
