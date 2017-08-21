#version 330 core

// Input
in vec4 frag_Color;
in vec2 frag_TexCoord;

// Output
out vec4 out_Color;

// Uniforms
uniform sampler2D tex0_Sampler;
uniform sampler2D tex1_Sampler;

void main() {
  vec4 texel0 = texture2D(tex0_Sampler, frag_TexCoord.st);
  vec4 texel1 = texture2D(tex1_Sampler, frag_TexCoord.st);
  out_Color = mix(texel0, texel1, 0.25f) + frag_Color;
}
