#version 330 core

out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0; // map [0,1] to [-1,1]
    float dist = dot(coord, coord);         // squared distance from center

    if (dist > 1.0)
    discard; // outside unit circle, remove pixel

    FragColor = vec4(1.0, 0.2, 0.2, 1.0); // red color
}