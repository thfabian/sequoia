#version 330 core

uniform int u_IntTest;
uniform float u_FloatTest;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = float(u_IntTest) + u_FloatTest;
}

