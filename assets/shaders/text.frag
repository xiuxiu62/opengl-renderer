#version 460 core

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D text;
uniform vec3 text_color;

void main() {
    vec4 sampled = vec4(1.0, 0.0, 1.0, texture(text, v_uv).r);
    f_color = vec4(text_color, 1.0) * sampled;
}
