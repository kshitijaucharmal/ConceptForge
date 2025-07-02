#version 430 core
in vec2 uv;
out vec4 FragColor;
void main() {
    FragColor = vec4(uv, 0.5 + 0.5 * sin(uv.x * 10.0), 1.0);
}