#version 330 core

struct TestStruct {
  int a;
  float b;
  vec3 c;
};

uniform TestStruct u_TestStruct[3];

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  for(int i = 0; i < 3; ++i)
    gl_Position.w += float(u_TestStruct[i].a) + u_TestStruct[i].b + u_TestStruct[i].c.x;
}
