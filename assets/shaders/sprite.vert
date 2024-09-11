#version 430 core

uniform mat4 camera;
uniform mat4 transform;

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_color;

layout (location = 0) out vec3 v_position;
layout (location = 1) out vec2 v_uv;
layout (location = 2) out vec3 v_normal;
layout (location = 3) out vec3 v_color;

void main() {
     vec4 world_position = transform * vec4(a_position, 0.0, 1.0);
     gl_Position = camera * world_position; 
          
     v_position = world_position.xyz;
     v_uv = a_uv;
     v_normal = normalize(mat3(transform) * a_normal);
     v_color = a_color;
}
