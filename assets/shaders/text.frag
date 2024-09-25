#version 460 core

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D text;
uniform vec3 text_color;

void main() {
    vec4 sampled = texture(text, v_uv);
    float d = sampled.r;
    float aaf = fwidth(d);
    float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);

    f_color = vec4(1.0, 1.0, 1.0, alpha);
}
