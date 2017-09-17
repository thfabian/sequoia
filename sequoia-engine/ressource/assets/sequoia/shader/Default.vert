#version 330 core

// Input
in vec3 in_Position;
in vec4 in_Color;
in vec2 in_TexCoord;

// Output
out vec4 frag_Color;
out vec2 frag_TexCoord;

// Uniforms
uniform mat4 u_matMVP;  // Model-View-Projection matrix
uniform mat4 u_matV;    // View matrix
uniform mat4 u_matM;    // Model matrix

void main() {
  gl_Position = u_matMVP * vec4(in_Position, 1.0f);

  frag_Color = in_Color;
  frag_TexCoord = in_TexCoord;
}
