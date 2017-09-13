#version 330 core

uniform float u_float_array[2];
uniform vec3 u_fvec3_array[2];

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = u_fvec3_array[0].x + u_fvec3_array[1].x + u_float_array[0] + u_float_array[1];
}
