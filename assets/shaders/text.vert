#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;

layout (location = 0) out vec2 v_uv;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(a_position, 0.0, 1.0);
    // gl_Position = vec4(a_position, 0.0, 1.0);
    v_uv = a_uv;
}
