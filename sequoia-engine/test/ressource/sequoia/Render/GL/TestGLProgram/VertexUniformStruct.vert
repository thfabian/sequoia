#version 330 core

struct ScalarStruct {
  int a;
  float b;
  vec3 c;
};

uniform ScalarStruct u_ScalarStruct;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = float(u_ScalarStruct.a) + u_ScalarStruct.b + u_ScalarStruct.c.x;
}
