#version 330 core

uniform int u_IntTest;
uniform float u_FloatTest;

in vec3 in_VertexCoord;

void main() {
  gl_Position.xyz = in_VertexCoord;
  gl_Position.w = float(u_IntTest) + u_FloatTest;
}

