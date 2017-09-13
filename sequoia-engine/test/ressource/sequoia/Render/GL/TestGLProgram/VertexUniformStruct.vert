#version 330 core

struct MyStruct {
  int a;
  float b;
};

uniform MyStruct u_MyStruct;

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  gl_Position.w = float(u_MyStruct.a) + u_MyStruct.b;
}
