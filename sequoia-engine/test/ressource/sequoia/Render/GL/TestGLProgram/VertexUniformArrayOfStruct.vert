#version 330 core

struct ScalarStruct {
  int a;
  float b;
  vec3 c;
};

uniform ScalarStruct u_ScalarStruct[3];

in vec3 in_Position;

void main() {
  gl_Position.xyz = in_Position;
  for(int i = 0; i < 3; ++i)
    gl_Position.w += float(u_ScalarStruct[i].a) + u_ScalarStruct[i].b + u_ScalarStruct[i].c.x;
}
