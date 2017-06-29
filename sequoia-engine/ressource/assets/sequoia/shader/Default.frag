#version 330 core

// Input
in vec4 frag_Color;
in vec2 frag_TexCoord;

// Output
out vec4 out_Color;

// Uniforms
uniform sampler2D tex0_Sampler;

void main() {
  out_Color = texture2D(tex0_Sampler, frag_TexCoord.st) + frag_Color;
}
