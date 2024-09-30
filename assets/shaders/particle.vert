#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;

layout (location = 0) out vec2 v_uv;
layout (location = 0) out vec4 v_color;

uniform mat4 camera;
uniform vec2 offset;
uniform vec4 color;

void main() {
    float scale = 10.0f;
    v_uv = a_uv;
    v_color = color;
    gl_Position = camera * vec4((a_position * scale) + offset, 0.0, 1.0);    
}
