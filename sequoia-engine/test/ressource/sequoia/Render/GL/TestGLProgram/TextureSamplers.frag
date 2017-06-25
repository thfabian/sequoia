#version 330 core

out vec4 out_Color;

uniform sampler2D tex0_SamplerFor2DTexture;
uniform sampler2D tex5_SamplerFor2DTexture;

void main() {
  vec2 textureCoords2 = vec2(0.5, 0.5);
  out_Color = texture(tex0_SamplerFor2DTexture, textureCoords2) + 
              texture(tex5_SamplerFor2DTexture, textureCoords2);
}
