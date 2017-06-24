#version 330 core

uniform vec2 u_fvec2;
uniform vec3 u_fvec3;
uniform vec4 u_fvec4;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = u_fvec2.x + u_fvec3.x + u_fvec4.x;
}

