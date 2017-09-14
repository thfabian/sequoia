#version 330 core

struct TestStruct {
  int a;
  float b;
  vec3 c;
};

uniform TestStruct u_TestStruct;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = float(u_TestStruct.a) + u_TestStruct.b + u_TestStruct.c.x;
}
