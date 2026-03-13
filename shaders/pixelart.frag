#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture; // The output from your main lighting pass
const int steps = 24;

// OKLab ↔ OKLCh helpers
vec3 oklab_to_oklch(vec3 lab) {
    float C = length(lab.yz);
    float h = (C == 0.0) ? 0.0 : atan(lab.z, lab.y);
    return vec3(lab.x, C, h);
}

vec3 oklch_to_oklab(vec3 lch) {
    float a = lch.y * cos(lch.z);
    float b = lch.y * sin(lch.z);
    return vec3(lch.x, a, b);
}

// Color space conversions
vec3 srgb_to_linear(vec3 srgb) {
    return mix(
    srgb * (1.0 / 12.92),
    pow(max((srgb + 0.055) / 1.055, 0.0), vec3(2.4)),
    step(vec3(0.04045), srgb)
    );
}

vec3 linear_srgb_to_oklab(vec3 c) {
    float l = 0.4122214708*c.r + 0.5363325363*c.g + 0.0514459929*c.b;
    float m = 0.2119034982*c.r + 0.6806995451*c.g + 0.1073969566*c.b;
    float s = 0.0883024619*c.r + 0.2817188376*c.g + 0.6299787005*c.b;

    l = pow(max(l, 0.0), 1.0/3.0);
    m = pow(max(m, 0.0), 1.0/3.0);
    s = pow(max(s, 0.0), 1.0/3.0);

    return vec3(
    0.2104542553*l + 0.7936177850*m - 0.0040720468*s,
    1.9779984951*l - 2.4285922050*m + 0.4505937099*s,
    0.0259040371*l + 0.7827717662*m - 0.8086757660*s
    );
}

vec3 oklab_to_linear_srgb(vec3 c) {
    float l_ = c.x + 0.3963377774 * c.y + 0.2158037573 * c.z;
    float m_ = c.x - 0.1055613458 * c.y - 0.0638541728 * c.z;
    float s_ = c.x - 0.0894841775 * c.y - 1.2914855480 * c.z;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    return vec3(
    4.0767416621 * l - 3.3077115913 * m + 0.2309699292 * s,
    -1.2684380046 * l + 2.6097574011 * m - 0.3413193965 * s,
    -0.0041960863 * l - 0.7034186147 * m + 1.7076147010 * s
    );
}

void main() {
    vec4 texColor = texture(screenTexture, TexCoords);
    vec3 result = texColor.rgb;

    // Apply the "lift" from your original code
    result = max(result, vec3(0.008));

    // OKLab → OKLCh conversion
    vec3 lab = linear_srgb_to_oklab(result);
    vec3 lch = oklab_to_oklch(lab);

    // Lightness adjustment and quantization
    lch.x = pow(lch.x, 0.7);
    lch.x = floor(lch.x * steps + 0.5) / steps;
    lch.y = floor(lch.y * (steps * 1.4) + 0.5) / (steps * 1.4);
    lch.z = floor(lch.z * (steps * 3.0) + 0.5) / (steps * 3.0);

    // Hue wrapping
    const float TAU = 6.28318530718;
    lch.z = mod(lch.z + TAU/2.0, TAU) - TAU/2.0;

    // Convert back to display color
    lab = oklch_to_oklab(lch);
    vec3 linear = oklab_to_linear_srgb(lab);

    // Final gamma correction if your framebuffer isn't sRGB
    // FragColor = vec4(linear, 1.0);
    float gamma = 1.2;
    FragColor = vec4(pow(linear, vec3(1.0/gamma)), 1.0);
}