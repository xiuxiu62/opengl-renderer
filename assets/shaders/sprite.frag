#version 430 core

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

uniform Light light;
uniform sampler2D texture_sampler;

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_color;

layout (location = 0) out vec4 f_color;

void main() {
    vec4 tex_color = texture(texture_sampler, v_uv);

    vec3 light_dir = normalize(light.position - v_position);
    float diff = max(dot(normalize(v_normal), light_dir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    vec3 ambient = 0.1 * light.color;
    // vec3 result = (ambient + diffuse) * tex_color.rgb * v_color;
    vec3 combined = (ambient + diffuse) * tex_color.rgb;

    // Debug outputs - uncomment one at a time
    // f_color = vec4(v_position * 0.5 + 0.5, 1.0);  // Visualize world position
    // f_color = vec4(v_normal * 0.5 + 0.5, 1.0);    // Visualize normals
    // f_color = vec4(light_dir * 0.5 + 0.5, 1.0);   // Visualize light direction
    // f_color = vec4(diffuse, 1.0);                 // Visualize diffuse component
    // f_color = vec4(ambient, 1.0);                 // Visualize ambient component
    f_color = vec4(combined, tex_color.a);
}
