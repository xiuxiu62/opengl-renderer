#version 460 core

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D scene;
uniform sampler2D bloom_blur;
uniform float bloom_strength;

void main() {
    vec3 hdr_color = texture(scene, v_uv).rgb;
    vec3 bloom_color = texture(bloom_blur, v_uv).rgb;
    hdr_color += bloom_color * bloom_strength;

    vec3 result = hdr_color / (hdr_color + vec3(1.0));

    f_color = vec4(result, 1.0);
}
