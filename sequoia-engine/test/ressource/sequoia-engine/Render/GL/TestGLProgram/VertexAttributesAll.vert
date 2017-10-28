#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec4 in_Color;
in vec3 in_Tangent;
in vec3 in_Bitangent;

out vec4 frag_Color;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = in_Normal.x + in_TexCoord.x + in_Tangent.x + in_Bitangent.x + in_Color.x;

  frag_Color = in_Color;
}

