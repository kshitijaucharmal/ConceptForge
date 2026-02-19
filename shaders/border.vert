#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlineWidth = 0.04;

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vec3 viewNormal = normalize(normalMatrix * aNormal);
    viewPos.xyz += viewNormal * outlineWidth;

    gl_Position = projection * viewPos;
}