#version 460 core

layout (location = 0) in vec2 v_uv;
layout (location = 1) in vec4 v_color;

layout (location = 0) out vec4 f_color;

uniform sampler2D sprite;

void main() {
    vec4 sampled = texture(sprite, v_uv);
    color = sampled * v_color;
}
