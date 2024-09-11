#version 430 core

uniform vec2  light_position;
uniform vec3  light_color;
uniform float light_intensity;
uniform float light_radius;

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

void main() {
    vec2 pixel_position = gl_FragCoord.xy;
    float distance = length(light_position - pixel_position);
    float attenuation = 1.0 - smoothstep(0.0, light_radius, distance);

    vec3 final_color = light_color * light_intensity * attenuation;
    f_color = vec4(final_color, 1.0);
}
