#version 330 core

in vec3 color;
in vec2 texture_coordinate;

out vec4 frag_color;

uniform sampler2D in_texture;

void main() {
    frag_color = texture(in_texture, texture_coordinate);
}
