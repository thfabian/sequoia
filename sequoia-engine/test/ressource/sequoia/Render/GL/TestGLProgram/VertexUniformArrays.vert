#version 330 core

uniform float u_float_array[2];
uniform vec3 u_fvec3_array[2];

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  vec3 vec = u_fvec3_array[1];
  gl_Position.w = vec.x + u_float_array[1];
}

