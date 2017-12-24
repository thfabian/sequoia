#version 330 core

in vec3 in_Position;

void main() {
  vec3 s = int(5);
  gl_Position.xyz = in_Position + s;
}

