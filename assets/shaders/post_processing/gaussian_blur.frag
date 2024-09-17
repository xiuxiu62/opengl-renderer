#version 460 core

layout (location = 0) in vec2 v_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[5] = float[] {0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};

void main() {
   vec2 uv_offset = 1.0 / textureSize(image, 0);
   vec3 result = texture(image, v_uv).rgb * weight[0];
   
   if (horizontal) {
      for (int i = 1; i < 5; ++i) {
         result += texture(image, v_uv + vec2(uv_offset.x * i, 0.0)).rgb * weight[i];      
         result += texture(image, v_uv - vec2(uv_offset.x * i, 0.0)).rgb * weight[i];      
      }
   } else {
      for (int i = 1; i < 5; ++i) {
         result += texture(image, v_uv + vec2(0.0, uv_offset.y * i)).rgb * weight[i];      
         result += texture(image, v_uv - vec2(0.0, uv_offset.y * i)).rgb * weight[i];      
      }
   }

   f_color = vec4(result, 1.0);
}
