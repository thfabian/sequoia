#version 330 core

// Input
in vec3 in_Position;
in vec4 in_Color;

// Output
out vec4 frag_Color;

// Uniforms
uniform mat4 u_ModelViewProjection;

void main() {
  gl_Position = u_ModelViewProjection * vec4(in_Position, 1.0f);
  frag_Color = in_Color;
}

