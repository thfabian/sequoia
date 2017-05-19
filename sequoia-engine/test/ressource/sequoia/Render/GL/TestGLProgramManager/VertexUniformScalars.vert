#version 330 core

uniform bool u_bool_test;
uniform int u_int_test;
uniform uint u_uint_test;
uniform float u_float_test;

in vec3 vertexCoord;

void main() {
  gl_Position.xyz = vertexCoord;
  gl_Position.w = float(u_bool_test) + float(u_int_test) + float(u_uint_test) + u_float_test;
}

