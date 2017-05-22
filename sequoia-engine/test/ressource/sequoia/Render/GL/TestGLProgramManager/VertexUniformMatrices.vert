#version 330 core

uniform mat2 u_fmat2;
uniform mat3 u_fmat3;
uniform mat4 u_fmat4;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = u_fmat2[0][0] + u_fmat3[0][0] + u_fmat4[0][0];
}

