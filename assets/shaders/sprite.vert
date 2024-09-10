#version 400

uniform mat4 projection;

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec2 a_normal;
layout (location = 3) in vec3 a_color;

void main() {
    // gl_Position = vec4(a_position, 0.0, 1.0); 
     gl_Position = projection * vec4(a_position, 0.0, 1.0); 
}
