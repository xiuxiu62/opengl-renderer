#version 430 core

uniform mat4 camera;
uniform mat4 transform;

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec2 a_normal;
layout (location = 3) in vec3 a_color;

layout (location = 0) out vec2 v_uv;
layout (location = 1) out vec3 v_color;

void main() {
     gl_Position = camera * vec4(a_position, 0.0, 1.0); 
     v_uv = a_uv;
     v_color = a_color;
}
