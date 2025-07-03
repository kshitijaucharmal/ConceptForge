#version 430 core
struct RayTracerSettings {
    float image_width;
    float image_height;

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    vec3 pixel_loc_00;
    vec3 camera_position;
};

layout(std430, binding = 3) buffer RayTracerSettingsBuffer {
    RayTracerSettings rt;
};

in vec2 uv;
out vec4 FragColor;

float hit_sphere(vec3 center, float radius, vec3 ro, vec3 rd) {
    vec3 oc = center - ro;
    float a = dot(rd, rd);
    float b = -2.0 * dot(rd, oc);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;

    if(discriminant < 0) {
        return -1;
    }
    else{
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 ray_color(vec3 ro, vec3 rd) {

    vec3 center = vec3(0, 0, -1);
    float t = hit_sphere(center, 0.5, ro, rd);

    if(t > 0.0){
        vec3 point = ro + t * rd;
        vec3 N = 0.5 * (normalize(point - center) + 1);
        return N;
    }

    vec3 unit_direction = normalize(rd);
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a) * vec3(1.0) + a * vec3(0.0, 0.0, 1.0);
}

void main() {
    float i = uv.x * rt.image_width;
    float j = uv.y * rt.image_height;

    vec3 pixel_center = rt.pixel_loc_00 + i * rt.pixel_delta_u + j * rt.pixel_delta_v;
    vec3 ray_direction = pixel_center - rt.camera_position;

    vec3 color = ray_color(rt.camera_position, ray_direction);
    FragColor = vec4(color, 1.0);
}