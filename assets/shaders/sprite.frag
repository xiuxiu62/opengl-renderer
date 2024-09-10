#version 420

uniform sampler2D texture_sampler;

layout (location = 0) in vec2 v_uv;
layout (location = 1) in vec3 v_color;

layout (location = 0) out vec4 f_color;

void main() {
    f_color = texture(texture_sampler, v_uv) * vec4(v_color, 1.0);
    // f_color = vec4(v_color, 0.0);
}
