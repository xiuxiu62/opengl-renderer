#version 430 core

uniform sampler2D texture_sampler;

layout (location = 0) in vec2 v_uv;
layout (location = 1) in vec3 v_color;

layout (location = 0) out vec4 f_color;

void main() {
    f_color = texture(texture_sampler, v_uv); 
}
