#version 460 core

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D scene;
uniform float threshold;

void main() {
    vec3 color texture(scene, v_uv).rgb; 
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (brightness <= threshhold) {
        color = vec3(0.0, 0.0, 0.0);
    }

    f_color = vec4(color, 1.0);
}
